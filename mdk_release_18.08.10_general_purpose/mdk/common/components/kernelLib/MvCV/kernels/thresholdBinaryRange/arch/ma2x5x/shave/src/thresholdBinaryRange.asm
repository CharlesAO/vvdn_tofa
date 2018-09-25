///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvThresholdBinaryRange_asm,@function
.version 00.51.04

.data .data.thresholdBinaryRange
.align 16
 thresholdBinaryRange_maxVal:
        .short 255

.code .text.thresholdBinaryRange

//void thresholdBinaryRange(u8** in, u8** out,  u8 lowerValue, u8 upperValue, u32 width)
//					          i18       i17          i16          i15          i14
mvcvThresholdBinaryRange_asm:
.nowarn
	LSU0.LDIL i5 0x003F    || LSU1.LDIH i5 0x0 
	LSU0.LD.32  i18  i18    || LSU1.LD.32  i17  i17    || CMU.CPIVR.x8 v5 i16
	lsu0.ldil i6 thresholdBinaryRange_compensate || lsu1.ldih i6 thresholdBinaryRange_compensate 
	LSU0.LDIL i0 thresholdBinaryRange_maxVal || LSU1.LDIH i0 thresholdBinaryRange_maxVal || CMU.CPZV v0 0x3
	lsu0.ldil i2 thresholdBinaryRange_loop   || lsu1.ldih i2 thresholdBinaryRange_loop   || iau.and i4 i14 i5
	lsu0.ldil i7 thresholdBinaryRange_final      || lsu1.ldih i7 thresholdBinaryRange_final      
	LSU0.LD.8R v1, i0 || CMU.CPIVR.x8 v6 i15 || iau.shr.u32 i3 i14 6
	nop 2
	
	cmu.cmz.i32 i3
	peu.pc1c eq || bru.jmp i6
	nop 6
	
	LSU0.LDO.64.L v2 i18 0 || LSU1.LDO.64.H v2 i18 8 || iau.add i18 i18 16 //|| bru.rpl i2 i3
	LSU0.LDO.64.L v3 i18 0 || LSU1.LDO.64.H v3 i18 8 || iau.add i18 i18 16 
	LSU0.LDO.64.L v4 i18 0 || LSU1.LDO.64.H v4 i18 8 || iau.add i18 i18 16 
	LSU0.LDO.64.L v7 i18 0 || LSU1.LDO.64.H v7 i18 8 || iau.add i18 i18 16 
	
	LSU1.STXV v0 i17	|| iau.add i17, i17 16 || bru.rpl i2 i3
	LSU0.STXV v0 i17	|| iau.add i17, i17 16
	LSU1.STXV v0 i17	|| iau.add i17, i17 16
	LSU0.STXV v0 i17	|| iau.sub i17, i17 48 || CMU.CMVV.u8 v6 v2	
	PEU.ANDACC || CMU.CMVV.u8 v2 v5 || lsu1.ldxv v2 i18 || iau.add i18 i18 16
 thresholdBinaryRange_loop:

	PEU.PVL08 gt || lsu0.stxv v1 i17 || iau.add i17 i17 16 || CMU.CMVV.u8 v6 v3
	PEU.ANDACC || CMU.CMVV.u8 v3 v5 || lsu1.ldxv v3 i18 || iau.add i18 i18 16
	PEU.PVL08 gt || lsu0.stxv v1 i17 || iau.add i17 i17 16 || CMU.CMVV.u8 v6 v4
	PEU.ANDACC || CMU.CMVV.u8 v4 v5 || lsu1.ldxv v4 i18 || iau.add i18 i18 16
	PEU.PVL08 gt || lsu0.stxv v1 i17 || iau.add i17 i17 16 || CMU.CMVV.u8 v6 v7
	PEU.ANDACC || CMU.CMVV.u8 v7 v5 || lsu1.ldxv v7 i18 || iau.add i18 i18 16
	PEU.PVL08 gt || lsu0.stxv v1 i17 || iau.add i17 i17 16
	//nop 2
	
	cmu.cmz.i32 i4 //|| iau.sub i18 i18 0x40
	peu.pc1c eq || bru.jmp i7
	nop 6
	
	
	iau.sub i18 i18 0x40
 thresholdBinaryRange_compensate:
	
	lsu0.ldil i10 thresholdBinaryRange_loop2  || lsu1.ldih i10 thresholdBinaryRange_loop2 || iau.shr.u32 i4 i4 3 


    IAU.XOR i1 i1 i1
	IAU.SHR.u32 i11 i14 6
	IAU.MUL i1 i11 64
	nop
	IAU.SUB i1 i14 i1
	
	
	IAU.XOR i11 i11 i11
	IAU.ADD i11 i11 32
	CMU.CMII.u32 i1 i11
	CMU.CMII.u32 i11 i14
	PEU.PCXC EQ,0 || BRU.BRA var32
	PEU.PCXC EQ,1 || BRU.BRA var32
	nop 6
	
	IAU.SUB i11 i11 8
	CMU.CMII.u32 i1 i11
	PEU.PCXC EQ,0 || BRU.BRA var40
	nop 6
	
	IAU.SUB i11 i11 8
	CMU.CMII.u32 i1 i11
	CMU.CMII.u32 i11 i14
	PEU.PCXC EQ,0 || BRU.BRA var48
	PEU.PCXC EQ,1 || BRU.BRA var48
	nop 6
	
	IAU.SUB i11 i11 8
	CMU.CMII.u32 i1 i11
	CMU.CMII.u32 i11 i14
	PEU.PCXC EQ,0 || BRU.BRA varjump
	PEU.PCXC EQ,1 || BRU.BRA varjump
	nop 6
	
	IAU.ADD i11 i11 40
	CMU.CMII.u32 i11 i15
	PEU.PCXC EQ,0 || BRU.BRA var48
	nop 6

varnorm:	
	LSU0.STO.64.L v0  i17 0  || LSU1.STO.64.H v0 i17 8 
	LSU0.STO.64.L v0  i17 16 || LSU1.STO.64.H v0 i17 24
	LSU0.STO.64.L v0  i17 32 || LSU1.STO.64.H v0 i17 40
	LSU0.STO.64.L v0  i17 48 || LSU1.STO.64.H v0 i17 56 
	BRU.BRA varjump
	nop 6
	
var32:
    LSU0.STO.64.L v0  i17 0  || LSU1.STO.64.H v0 i17 8 
	LSU0.STO.64.L v0  i17 16 || LSU1.STO.64.H v0 i17 24
	BRU.BRA varjump
    nop 6
	
var48:	
    LSU0.STO.64.L v0  i17 0  || LSU1.STO.64.H v0 i17 8 
	BRU.BRA varjump
	nop 6
	
var40:
    LSU0.STO.64.L v0  i17 0  || LSU1.STO.64.H v0 i17 8 
	LSU0.STO.64.L v0  i17 16 

varjump:
	nop
	LSU0.LD.64.L v9 i18 || iau.add i18 i18 8 || bru.rpl i10 i4
	nop 2
 thresholdBinaryRange_loop2:
	nop 4
	cmu.cmvv.u8 v9 v5 
	PEU.ANDACC || CMU.CMVV.u8 v6 v9
	PEU.PVL08 gt || LSU0.ST.64.L v1 i17 || iau.add i17 i17 8
	
		
 thresholdBinaryRange_final:
    BRU.jmp i30
	nop 6
.nowarnend
.size mvcvThresholdBinaryRange_asm,.-mvcvThresholdBinaryRange_asm
.end
