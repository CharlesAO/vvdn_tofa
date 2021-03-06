///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvDilate_asm,@function
.version 00.51.04

// Local defines
// Function arguments
.set srcImage1 i18
.set destImage i17
.set kernel i16
.set widthImage i15
.set heightImage i14
.set k i13
 .nowarn
// Others registers

.code .text.dilate
//void Dilate(u8** src, u8** dst, u8** kernel, u32 width, u32 height, u8 k)
//                 i18       i17       i16         i15        i14        i13


mvcvDilate_asm:
// Verify if the kernel size is less than 8
LSU0.LDIL    i3 8
LSU0.LDIL    i6 dilate_End_Of_Main_Loop              || LSU1.LDIH    i6 dilate_End_Of_Main_Loop
LSU0.LD.32    i17 i17   							  || IAU.SUB      i12 i13 1
NOP 2

// i7 counter - coloumn   // Number of pixels per line to: i5 = width
LSU1.LDIL    i0  0      || CMU.CPII     i7  i13

// i8 counter - line
LSU0.LDIL    i14 0      || CMU.CPII     i8  i13
CMU.CPII  	 i5 i15
CMU.CPIVR.x8 v0 i0

LSU0.LDIL    i3 16
IAU.SUB      i2 i13 1 ||   CMU.CPVV     v4  v0
IAU.SUB      i2 i3 i2 ||   CMU.CPVV     v7  v0
IAU.ADD      i1 i2 i2

//If width < i1 then
CMU.CMII.u32 i15 i1 || IAU.ADD i11 i17 i15
PEU.PC1C LT || CMU.CPII i5 i1

CMU.CPII     i4 i5 || lsu0.ldo.64.l v8 i11 0
SAU.DIV.u32  i5 i5 i1 || lsu0.ldo.64.h v8 i11 8
CMU.CPII     i1 i2
NOP 6

CMU.CPII     i10  i18 || VAU.SUB.i8  v4 v4 v4
CMU.CPII     i9   i16 || VAU.SUB.i8  v7 v7 v7
LSU0.LD.32    i15  i10
LSU0.LD.32    i12  i9
NOP 3


 dilate_Main_Loop:

    BRU.RPL  i6  i5

 dilate_Start_Computing:
 dilate_Repeat_On_Lines:
NOP 3
        LSU0.LD.8      i0 i12       || IAU.ADD       i12  i12  1
        IAU.ADD    i15 i15 i14
        // Load source values
        LSU0.LDO.64.L  v1 i15  0    || LSU1.LDO.64.H  v1   i15  8     || IAU.ADD i15  i15  i2
        LSU0.LDO.64.L  v5 i15  0    || LSU1.LDO.64.H  v5   i15  8     || IAU.ADD i15  i15  i1
NOP 3
 dilate_Repeat_On_Coloumns:
            // Load the kernel value
            CMU.CPIVR.x8 v2 i0
            IAU.SUB    i7 i7  1
            // Multiply the source value with the kernel value
            VAU.MUL.i8  v3  v1  v2 ||  LSU0.LD.8 i0 i12 || IAU.ADD i12  i12  1
            VAU.MUL.i8  v6  v5  v2

            CMU.CMZ.i8 i7

            // Loop on coloumns
            PEU.PC1C GT          || BRU.BRA dilate_Repeat_On_Coloumns
            CMU.ALIGNVEC v1  V1, v1 1
            CMU.ALIGNVEC v5 V5, v5 1

            // Compute the maximum number
            CMU.MAX.u8 v4 v4  v3
            CMU.MAX.u8 v7 v7  v6
NOP 2

        IAU.SUB    i8  i8  1
        CMU.CMZ.i8 i8

        // Loop on lines
        PEU.PC1C GT                  || BRU.BRA dilate_Repeat_On_Lines
            IAU.ADD   i10  i10 4
            IAU.ADD   i9   i9  4     || LSU0.LD.32  i15 i10
            LSU0.LD.32 i12  i9       || CMU.CPII   i7  i13
NOP 3

        CMU.CPII     i10 i18

 dilate_End_Of_Main_Loop:

        CMU.CPII     i9  i16
        // Store the maximum value

 LSU0.STO.64.L v4  i17  0  || LSU1.STO.64.H v4  i17  8    || IAU.ADD   i17 i17  i2  || CMU.CPII  i8  i13
        LSU0.STO.64.L v7  i17  0  || LSU1.STO.64.H v7  i17  8    || IAU.ADD   i17 i17  i1
        CMU.CPVV     v4  v0      || IAU.ADD  i14 i14  i2         || LSU0.LD.32 i12  i9      || LSU1.LD.32 i15 i10
        CMU.CPVV     v7  v0      || IAU.ADD  i14 i14  i1
NOP 2


// Compensate the remaining pixels
 dilate_Compensate_Last_Bits:

IAU.SUB i0 i4 i14

CMU.CMZ.i8 i0
PEU.PC1C GT  || BRU.BRA dilate_Start_Computing
IAU.SHR.u32  i2 i0 1
IAU.SUB i1 i0 i2
NOP 4

 lsu0.sto.64.l v8 i11 0
 lsu0.sto.64.h v8 i11 8
BRU.JMP i30
NOP 6

.size mvcvDilate_asm,.-mvcvDilate_asm
 .nowarnend
.end
