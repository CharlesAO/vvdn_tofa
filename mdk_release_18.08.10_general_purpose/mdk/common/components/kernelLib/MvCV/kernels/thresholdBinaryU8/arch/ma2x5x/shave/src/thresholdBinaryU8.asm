///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvThresholdBinaryU8_asm,@function
.version 00.51.04

.data .data.thresholdBinaryU8
.align 16
 thresholdBinaryU8_maxVal:
        .short 255

.code .text.thresholdBinaryU8

//void thresholdBinaryU8(u8** in, u8** out, u8 threshold, u32 width)
//					       i18       i17       i16          i15 
mvcvThresholdBinaryU8_asm:
.nowarn
	LSU0.LDIL i5 0x003F || LSU1.LDIH i5 0x0 
	LSU0.LD.32  i18  i18 || LSU1.LD.32  i17  i17 
	LSU0.LDIL i0 thresholdBinaryU8_maxVal || LSU1.LDIH i0 thresholdBinaryU8_maxVal || CMU.CPZV v0 0x3
	lsu0.ldil i2 thresholdBinaryU8_loop || lsu1.ldih i2 thresholdBinaryU8_loop || iau.and i4 i15 i5
	lsu0.ldil i6 thresholdBinaryU8_compensate || lsu1.ldih i6 thresholdBinaryU8_compensate
	lsu0.ldil i7 thresholdBinaryU8_final || lsu1.ldih i7 thresholdBinaryU8_final //|| iau.sub i19 i19 0x40
	nop 5
 	LSU0.LD.8R v1, i0 || CMU.CPIVR.x8 v5 i16 || iau.shr.u32 i3 i15 6

	cmu.cmz.i32 i3
	peu.pc1c eq || bru.jmp i6
	nop 6
	
	LSU0.LDO.64.L v4  i18 0 || LSU1.LDO.64.H v4  i18 8 || iau.add i18 i18 16  
	LSU0.LDO.64.L v6  i18 0 || LSU1.LDO.64.H v6  i18 8 || iau.add i18 i18 16
	LSU0.LDO.64.L v7  i18 0 || LSU1.LDO.64.H v7  i18 8 || iau.add i18 i18 16
	LSU0.LDO.64.L v8  i18 0 || LSU1.LDO.64.H v8  i18 8 || iau.add i18 i18 16
	
	LSU1.STXV v0 i17	|| iau.add i17, i17 16 || bru.rpl i2 i3 
	LSU0.STXV v0 i17	|| iau.add i17, i17 16
	LSU1.STXV v0 i17	|| iau.add i17, i17 16
	LSU0.STXV v0 i17	|| iau.sub i17, i17 48
	cmu.cmvv.u8 v4 v5 || LSU1.LDXV v4 i18 || iau.add i18 i18 16 
	
thresholdBinaryU8_loop:
	PEU.PVL08 gt || LSU0.STXV v1 i17 || iau.add i17 i17 16  
	cmu.cmvv.u8 v6 v5 || LSU1.LDXV v6 i18 || iau.add i18 i18 16  
	PEU.PVL08 gt || LSU0.STXV v1 i17 || iau.add i17 i17 16  
	cmu.cmvv.u8 v7 v5 || LSU1.LDXV v7 i18 || iau.add i18 i18 16 
	PEU.PVL08 gt || LSU0.STXV v1 i17 || iau.add i17 i17 16  
	cmu.cmvv.u8 v8 v5 || LSU1.LDXV v8 i18 || iau.add i18 i18 16 
	PEU.PVL08 gt || LSU0.STXV v1 i17 || iau.add i17 i17 16 
	
	cmu.cmz.i32 i4 || iau.sub i18 i18 0x40
	peu.pc1c eq || bru.jmp i7
	nop 5
	
 thresholdBinaryU8_compensate:

	lsu0.ldil i10 thresholdBinaryU8_loop2  || lsu1.ldih i10 thresholdBinaryU8_loop2 || iau.shr.u32 i4 i4 3 


 
    IAU.XOR i1 i1 i1
	IAU.SHR.u32 i11 i15 6
	IAU.MUL i1 i11 64
	nop
	IAU.SUB i1 i15 i1
	
	
	IAU.XOR i11 i11 i11
	IAU.ADD i11 i11 32
	CMU.CMII.u32 i1 i11
	CMU.CMII.u32 i11 i15
	PEU.PCXC EQ,0 || BRU.BRA var32
	PEU.PCXC EQ,1 || BRU.BRA var32
	nop 6
	
	IAU.SUB i11 i11 8
	CMU.CMII.u32 i1 i11
	PEU.PCXC EQ,0 || BRU.BRA var40
	nop 6
	
	IAU.SUB i11 i11 8
	CMU.CMII.u32 i1 i11
	CMU.CMII.u32 i11 i15
	PEU.PCXC EQ,0 || BRU.BRA var48
	PEU.PCXC EQ,1 || BRU.BRA var48
	nop 6
	
	IAU.SUB i11 i11 8
	CMU.CMII.u32 i1 i11
	CMU.CMII.u32 i11 i15
	PEU.PCXC EQ,0 || BRU.BRA varjump
	PEU.PCXC EQ,1 || BRU.BRA varjump
	nop 6
	
	IAU.ADD i11 i11 40
	CMU.CMII.u32 i11 i15
	PEU.PCXC EQ,0 || BRU.BRA var48
	nop 6
	

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
	LSU0.LD.64.L v4 i18 || iau.add i18 i18 8 || bru.rpl i10 i4
	nop
 thresholdBinaryU8_loop2:
	nop 5
	cmu.cmvv.u8 v4 v5 
	PEU.PVL08 gt || LSU0.ST.64.L v1 i17 || iau.add i17 i17 8

	
 thresholdBinaryU8_final:
    BRU.jmp i30
	nop 6
.nowarnend
.size mvcvThresholdBinaryU8_asm,.-mvcvThresholdBinaryU8_asm
.end
