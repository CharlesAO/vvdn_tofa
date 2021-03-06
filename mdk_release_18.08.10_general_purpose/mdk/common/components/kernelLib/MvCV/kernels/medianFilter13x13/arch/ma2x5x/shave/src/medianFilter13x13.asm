///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///


// Median denoise
.type mvcvMedianFilter13x13_asm,@function
.version 0.50
.ifndef MEDIAN_CFG
.set MEDIAN_CFG 1
.set StackPtr	 	i19

.ifndef SVU_COMMON_MACROS
.set SVU_COMMON_MACROS

//stack manipulation macros

.macro PUSH_V_REG VREG
lsu0.sto64.h \VREG,StackPtr,-8 || lsu1.sto64.l \VREG,StackPtr,-16 || iau.incs StackPtr,-16
.endm

.macro POP_V_REG VREG
lsu0.ldo64.l \VREG,StackPtr,0 || lsu1.ldo64.h \VREG, StackPtr, 8 || iau.incs StackPtr,16
.endm

.macro PUSH_2_32BIT_REG REG1, REG2
lsu0.sto32 \REG1,StackPtr,-4 || lsu1.sto32 \REG2,StackPtr,-8 || iau.incs StackPtr,-8
.endm

.macro POP_2_32BIT_REG REG1, REG2
lsu0.ldo32 \REG2,StackPtr,0 || lsu1.ldo32 \REG1,StackPtr,4 || iau.incs StackPtr,8
.endm

.macro PUSH_1_32BIT_REG REG
lsu0.sto32 \REG,StackPtr,-4 || iau.incs StackPtr,-4
.endm

.macro POP_1_32BIT_REG REG
lsu0.ldo32 \REG,StackPtr,0 || iau.incs StackPtr,4
.endm

.endif

.set MED_KWIDTH    13
.set MED_KHEIGHT   13
.set MED_MEDIAN   (MED_KWIDTH*MED_KHEIGHT/2+1)



.endif

// Filter, histogram based method for median calculation, kernel sizes up to 15x15, comprised of 2 steps per pixel :
// 1. histogram update for each pixel substract from histo prev edge pixels, add to histo new edge pixels
// 2. search for median by identifying 16 value bin and after that the actual value in the selected bin
// i16 : pointer to input row pointers (i1..i15 : input row pointers)
// i17 : destination row pointer
// i18 : line width
// v16..31 : 8bit histogram.

//void 								i18					i17				i16
//medianFilterImplementation (uint32_t widthLine, uint8_t *outLine, uint8_t ** inLine)
//{
//	uint32_t i = 0//
//    int j = 0//
//    int histogram[256]//
//    int e = 0//
//
//	for (i=0// i<256// i++) histogram[i] = 0//
//	// build initial histogram
//	for (i=0// i<MED_HEIGHT// i++)
//	{
//		for (j=0// j<MED_WIDTH// j++)
//		{
//			e =inLine[i][j-(MED_WIDTH>>1)]//
//			histogram[e]++//
//		}
//	}
//	for (i=0// i<widthLine// i++)
//	{
//		e = 0//
//		j = 0//
//		// use histogram
//		while (j<256)
//		{
//			e+=histogram[j]//
//			if (e<MED_LIMIT)
//			{
//				j++//
//			} else
//			{
//				outLine[i] = j//
//				j = 256//
//			}
//		}
//		// substract previous values from histogram
//		for (j=0// j<MED_HEIGHT// j++)
//		{
//			e = inLine[j][i-(MED_WIDTH>>1)]//
//			histogram[e]--//
//		}
//		// add next values to histogram
//		for (j=0// j<MED_HEIGHT// j++)
//		{
//			e = inLine[j][i+1+(MED_WIDTH>>1)]//
//			histogram[e]++//
//		}
//	}
//}

.data .rodata.medianFilter13x13_asm
.align 4


.code .text.medianFilter13x13_asm //text
.align 16
mvcvMedianFilter13x13_asm:
.nowarn
LSU1.LD.32  i17  i17
//preserve IRF values (in order to avoid crashing C code)
LSU0.STO.32 I20,i19,-4 || LSU1.STO.32 I21,i19,-8 || iau.incs i19,-8
LSU0.STO.32 I22,i19,-4 || LSU1.STO.32 I23,i19,-8 || iau.incs i19,-8
BRU.RFB16.Z 0		||	LSU1.LDIL i21, MED_KHEIGHT
BRU.RPI i21, 0x1	||	LSU0.LDI.32 i1, i16
BRU.RFB16.Z 0
LSU0.STO.64.H V24,i19,-8 || LSU1.STO.64.L V24,i19,-16 || iau.incs i19,-16
LSU0.STO.64.H V25,i19,-8 || LSU1.STO.64.L V25,i19,-16 || iau.incs i19,-16
LSU0.STO.64.H V26,i19,-8 || LSU1.STO.64.L V26,i19,-16 || iau.incs i19,-16
LSU0.STO.64.H V27,i19,-8 || LSU1.STO.64.L V27,i19,-16 || iau.incs i19,-16
LSU0.STO.64.H V28,i19,-8 || LSU1.STO.64.L V28,i19,-16 || iau.incs i19,-16
LSU0.STO.64.H V29,i19,-8 || LSU1.STO.64.L V29,i19,-16 || iau.incs i19,-16
LSU0.STO.64.H V30,i19,-8 || LSU1.STO.64.L V30,i19,-16 || iau.incs i19,-16
LSU0.STO.64.H V31,i19,-8 || LSU1.STO.64.L V31,i19,-16 || iau.incs i19,-16
//load the row pointers from the buffer
// init phase build histogram at the start of row
CMU.CPZV v16, 0x3 || IAU.SUB i1, i1, (MED_KWIDTH>>1) || LSU1.LDIL i21, 0
CMU.CPZV v20, 0x3 || LSU1.LDIL i22, (MED_KHEIGHT-1)
CMU.CPZV v24, 0x3 || LSU1.LDIL i20, medianFilter13x13_Median_HistogramRow	||	LSU0.LDIL i23, medianFilter13x13_Restore_And_Exit
CMU.CPZV v28, 0x3 || LSU1.LDIH i20, medianFilter13x13_Median_HistogramRow	||	LSU0.LDIH i23, medianFilter13x13_Restore_And_Exit



.lalign
 medianFilter13x13_Median_HistogramCol:
BRU.RPS i20, MED_KWIDTH || LSU0.LDI.8 i21, i1
nop
nop
nop
nop
 medianFilter13x13_Median_HistogramRow:
CMU.CMZ.i32 i22
nop

CMU.LUT.U8 i20, i21, 0x2
nop
IAU.ADDSU i20, i20, 0x01
CMU.LUTW.X8    i20, i21, 0x2
nop
PEU.PC1C 0x6 || BRU.BRA medianFilter13x13_Median_HistogramCol
PEU.PC1C 0x6 || BRU.RFB16.I 0x01 || IAU.SUB i22, i22, 0x01
PEU.PC1C 0x6 || IAU.SUB i1, i1, (MED_KWIDTH>>1)
PEU.PC1C 0x6 || LSU1.LDIL i20, medianFilter13x13_Median_HistogramRow
PEU.PC1C 0x6 || LSU1.LDIH i20, medianFilter13x13_Median_HistogramRow
PEU.PC1C 0x1 || BRU.RFB16.Z 0
nop

.lalign

 medianFilter13x13_Median_Loop:
// search the histogram for median on 16bin
                                                            SAU.SUMX.U8 i0, v16
                                                            SAU.SUMX.U8 i0, v17 || LSU1.LDIL i21, MED_MEDIAN
                CMU.CPVV v0, v16 || IAU.SUB i21, i21, i0 || SAU.SUMX.U8 i0, v18 || LSU1.LDIL i22, 0x00
PEU.PC1I 0x2 || CMU.CPVV v0, v17 || IAU.SUB i21, i21, i0 || SAU.SUMX.U8 i0, v19 || LSU1.LDIL i22, 0x10
PEU.PC1I 0x2 || CMU.CPVV v0, v18 || IAU.SUB i21, i21, i0 || SAU.SUMX.U8 i0, v20 || LSU1.LDIL i22, 0x20
PEU.PC1I 0x2 || CMU.CPVV v0, v19 || IAU.SUB i21, i21, i0 || SAU.SUMX.U8 i0, v21 || LSU1.LDIL i22, 0x30
PEU.PC1I 0x2 || CMU.CPVV v0, v20 || IAU.SUB i21, i21, i0 || SAU.SUMX.U8 i0, v22 || LSU1.LDIL i22, 0x40
PEU.PC1I 0x2 || CMU.CPVV v0, v21 || IAU.SUB i21, i21, i0 || SAU.SUMX.U8 i0, v23 || LSU1.LDIL i22, 0x50
PEU.PC1I 0x2 || CMU.CPVV v0, v22 || IAU.SUB i21, i21, i0 || SAU.SUMX.U8 i0, v24 || LSU1.LDIL i22, 0x60
PEU.PC1I 0x2 || CMU.CPVV v0, v23 || IAU.SUB i21, i21, i0 || SAU.SUMX.U8 i0, v25 || LSU1.LDIL i22, 0x70
PEU.PC1I 0x2 || CMU.CPVV v0, v24 || IAU.SUB i21, i21, i0 || SAU.SUMX.U8 i0, v26 || LSU1.LDIL i22, 0x80
PEU.PC1I 0x2 || CMU.CPVV v0, v25 || IAU.SUB i21, i21, i0 || SAU.SUMX.U8 i0, v27 || LSU1.LDIL i22, 0x90
PEU.PC1I 0x2 || CMU.CPVV v0, v26 || IAU.SUB i21, i21, i0 || SAU.SUMX.U8 i0, v28 || LSU1.LDIL i22, 0xA0
PEU.PC1I 0x2 || CMU.CPVV v0, v27 || IAU.SUB i21, i21, i0 || SAU.SUMX.U8 i0, v29 || LSU1.LDIL i22, 0xB0
PEU.PC1I 0x2 || CMU.CPVV v0, v28 || IAU.SUB i21, i21, i0 || SAU.SUMX.U8 i0, v30 || LSU1.LDIL i22, 0xC0
PEU.PC1I 0x2 || CMU.CPVV v0, v29 || IAU.SUB i21, i21, i0 || SAU.SUMX.U8 i0, v31 || LSU1.LDIL i22, 0xD0
PEU.PC1I 0x2 || CMU.CPVV v0, v30 || IAU.SUB i21, i21, i0                        || LSU1.LDIL i22, 0xE0
PEU.PC1I 0x2 || CMU.CPVV v0, v31 || IAU.SUB i21, i21, i0                        || LSU1.LDIL i22, 0xF0
// once the 16bin is identified search each element for the solution
                CMU.CPVV.u8.u16 v1, v0   || LSU1.SWZMC4.WORD [3232] [UUUU]
                CMU.CPVV.u8.u16 v0, v0   || LSU1.LDIL i20, 0
                CMU.CPVI.x16 i0.l, v1.7  || IAU.SUB i20, i20, i21
                CMU.CPVI.x16 i0.l, v1.6  || IAU.SUB i20, i20, i0  || LSU1.LDIL i21, 0x0F
PEU.PC1I 0x3 || CMU.CPVI.x16 i0.l, v1.5  || IAU.SUB i20, i20, i0  || LSU1.LDIL i21, 0x0E
PEU.PC1I 0x3 || CMU.CPVI.x16 i0.l, v1.4  || IAU.SUB i20, i20, i0  || LSU1.LDIL i21, 0x0D
PEU.PC1I 0x3 || CMU.CPVI.x16 i0.l, v1.3  || IAU.SUB i20, i20, i0  || LSU1.LDIL i21, 0x0C
PEU.PC1I 0x3 || CMU.CPVI.x16 i0.l, v1.2  || IAU.SUB i20, i20, i0  || LSU1.LDIL i21, 0x0B
PEU.PC1I 0x3 || CMU.CPVI.x16 i0.l, v1.1  || IAU.SUB i20, i20, i0  || LSU1.LDIL i21, 0x0A
PEU.PC1I 0x3 || CMU.CPVI.x16 i0.l, v1.0  || IAU.SUB i20, i20, i0  || LSU1.LDIL i21, 0x09
PEU.PC1I 0x3 || CMU.CPVI.x16 i0.l, v0.7  || IAU.SUB i20, i20, i0  || LSU1.LDIL i21, 0x08
PEU.PC1I 0x3 || CMU.CPVI.x16 i0.l, v0.6  || IAU.SUB i20, i20, i0  || LSU1.LDIL i21, 0x07
PEU.PC1I 0x3 || CMU.CPVI.x16 i0.l, v0.5  || IAU.SUB i20, i20, i0  || LSU1.LDIL i21, 0x06
PEU.PC1I 0x3 || CMU.CPVI.x16 i0.l, v0.4  || IAU.SUB i20, i20, i0  || LSU1.LDIL i21, 0x05
PEU.PC1I 0x3 || CMU.CPVI.x16 i0.l, v0.3  || IAU.SUB i20, i20, i0  || LSU1.LDIL i21, 0x04
PEU.PC1I 0x3 || CMU.CPVI.x16 i0.l, v0.2  || IAU.SUB i20, i20, i0  || LSU1.LDIL i21, 0x03
PEU.PC1I 0x3 || CMU.CPVI.x16 i0.l, v0.1  || IAU.SUB i20, i20, i0  || LSU1.LDIL i21, 0x02
PEU.PC1I 0x3 || CMU.CPVI.x16 i0.l, v0.0  || IAU.SUB i20, i20, i0  || LSU1.LDIL i21, 0x01
PEU.PC1I 0x4                             || IAU.ADD i22, i22, i21
IAU.SUB i18, i18, 0x01 || LSU0.STI.8 i22, i17

// update histogram and loop to start
PEU.PC1I 0x1 || BRU.JMP i23
PEU.PC1I 0x6 || LSU0.LDO.8 i21, i1, -MED_KWIDTH
nop 3
PEU.PC1I 0x6 || LSU0.LDI.8 i21, i1
PEU.PC1I 0x6 || LSU1.LDIL i22, MED_KHEIGHT
nop
 medianFilter13x13_Median_HistogramLoop:
CMU.LUT.U8 i20, i21, 0x2      || IAU.SUBSU i22, i22, 0x01
CMU.CMZ.i32  i22              || BRU.RFB16.I 0x01
IAU.SUBSU i20, i20, 0x01      || PEU.PCCX.NEQ 0x10 || BRU.BRA medianFilter13x13_Median_HistogramLoop || LSU0.LDO.8 i21, i1, -MED_KWIDTH
CMU.LUTW.X8    i20, i21, 0x2  || PEU.PCCX.EQ  0x10 || BRU.BRA medianFilter13x13_Median_Loop
CMU.LUT.U8 i20, i21, 0x2
nop
IAU.ADDSU i20, i20, 0x01 || PEU.PCCX.NEQ 0x10 || LSU0.LDI.8 i21, i1
CMU.LUTW.X8    i20, i21, 0x2
nop
BRU.RFB16.Z 0
 medianFilter13x13_Restore_And_Exit:
IAU.SUB i1 i1 i1
cmu.cpit B_RFB i1
LSU0.LDO.64.L V31,i19,0 || LSU1.LDO.64.H V31, i19, 8 || iau.incs i19,16
LSU0.LDO.64.L V30,i19,0 || LSU1.LDO.64.H V30, i19, 8 || iau.incs i19,16
LSU0.LDO.64.L V29,i19,0 || LSU1.LDO.64.H V29, i19, 8 || iau.incs i19,16
LSU0.LDO.64.L V28,i19,0 || LSU1.LDO.64.H V28, i19, 8 || iau.incs i19,16
LSU0.LDO.64.L V27,i19,0 || LSU1.LDO.64.H V27, i19, 8 || iau.incs i19,16
LSU0.LDO.64.L V26,i19,0 || LSU1.LDO.64.H V26, i19, 8 || iau.incs i19,16
LSU0.LDO.64.L V25,i19,0 || LSU1.LDO.64.H V25, i19, 8 || iau.incs i19,16
LSU0.LDO.64.L V24,i19,0 || LSU1.LDO.64.H V24, i19, 8 || iau.incs i19,16
LSU0.LDO.32 I23,i19,0 || LSU1.LDO.32 I22,i19,4 || iau.incs i19,8
LSU0.LDO.32 I21,i19,0 || LSU1.LDO.32 I20,i19,4 || iau.incs i19,8


.nowarnend
BRU.JMP i30
NOP 6
.size mvcvMedianFilter13x13_asm,.-mvcvMedianFilter13x13_asm
.end
