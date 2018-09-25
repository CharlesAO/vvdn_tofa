///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvArithmeticSubmask_asm,@function
.version 00.51.04

.data .data.arithmeticSubmask
.align 4
.code .text.arithmeticSubmask

//void arithmeticSubmask(u8** src1, u8** src2, u8** dst, u8** mask, u32 width) 
//					       i18         i17       i16       i15         i14
 .nowarn
mvcvArithmeticSubmask_asm:

    LSU0.LD.32  i18  i18 || LSU1.LD.32  i17  i17 
    LSU0.LD.32  i16  i16 || LSU1.LD.32 i15 i15 || IAU.SHR.u32 i1 i14 6 || CMU.CPZV v0 0x3
	LSU0.LDIL i2 0x003F || LSU1.LDIH i2 0x0 
	nop 5
	lsu1.ldil i5 arithmeticSubmask_loop || lsu0.ldih i5 arithmeticSubmask_loop 
	LSU0.ldil i7 arithmeticSubmask_final || LSU1.ldih i7 arithmeticSubmask_final || iau.sub i19 i19 0x40
	LSU0.ldil i8 arithmeticSubmask_compensate || LSU1.ldih i8 arithmeticSubmask_compensate 
	nop 5
	

	
	LSU0.LDO.64.L v1  i18 0    || LSU1.LDO.64.H v1  i18 8 || iau.add i18 i18 16 
	LSU0.LDO.64.L v2  i17 0    || LSU1.LDO.64.H v2  i17 8 || sau.add.i32 i17 i17 16 || IAU.AND i3 i14 i2
	LSU0.LDO.64.L v13 i15 0    || LSU1.LDO.64.H v13 i15 8 || iau.add i15 i15 16 
	LSU0.LDO.64.L v3  i18 0    || LSU1.LDO.64.H v3  i18 8 || iau.add i18 i18 16 
	LSU0.LDO.64.L v4  i17 0    || LSU1.LDO.64.H v4  i17 8 || iau.add i17 i17 16 
	LSU0.LDO.64.L v14 i15 0    || LSU1.LDO.64.H v14 i15 8 || iau.add i15 i15 16 

	LSU0.LDO.64.L v5  i18 0    || LSU1.LDO.64.H v5  i18 8 || iau.add i18 i18 16
	LSU0.LDO.64.L v6  i17 0    || LSU1.LDO.64.H v6  i17 8 || iau.add i17 i17 16
	
	nop 6
	cmu.cmz.i8 i1
	peu.pc1c eq || bru.jmp i8
	nop 6
	
	LSU0.LDO.64.L v15 i15 0    || LSU1.LDO.64.H v15 i15 8 || iau.add i15 i15 16 || bru.rpl i5 i1 _end_delay_loop_arithmeticSubmask
		
	LSU0.LDO.64.L v7  i18 0    || LSU1.LDO.64.H v7  i18 8  || iau.add i18 i18 16 
	LSU0.LDO.64.L v8  i17 0    || LSU1.LDO.64.H v8  i17 8  || iau.add i17 i17 16
	LSU0.LDO.64.L v16 i15 0    || LSU1.LDO.64.H v16 i15 8  || iau.add i15 i15 16 || VAU.ISUBS.U8 v9 v1 v2
	cmu.cmz.i8 v13 || LSU1.LDXV v1 i18   || iau.add i18 i18 16
	peu.pvl08 gt || lsu0.sto.64.l v9 i16 0 || cmu.alignvec v19 v13 v0 8
	cmu.cmz.i8 v19 || LSU0.LDXV v2 i17   || iau.add i17 i17 16
	peu.pvl18 gt || lsu1.sto.64.h v9 i16 8 || iau.add i16 i16 16 || VAU.ISUBS.U8 v10 v3 v4
	
	cmu.cmz.i8 v14 || LSU1.LDXV v13 i15  || iau.add i15 i15 16
	peu.pvl08 gt || lsu0.sto.64.l v10 i16 0 || cmu.alignvec v19 v14 v0 8
	cmu.cmz.i8 v19 || LSU0.LDXV v3 i18   || iau.add i18 i18 16
	peu.pvl18 gt || lsu1.sto.64.h v10 i16 8 || iau.add i16 i16 16 || VAU.ISUBS.U8 v11 v5 v6 
	
	cmu.cmz.i8 v15 || LSU1.LDXV v4 i17   || iau.add i17 i17 16
 arithmeticSubmask_loop:
	peu.pvl08 gt || lsu0.sto.64.l v11 i16 0 || cmu.alignvec v19 v15 v0 8
	cmu.cmz.i8 v19 || LSU0.LDXV v14 i15  || iau.add i15 i15 16
	peu.pvl18 gt || lsu1.sto.64.h v11 i16 8 || iau.add i16 i16 16 || VAU.ISUBS.U8 v12 v7 v8
	cmu.cmz.i8 v16 || LSU1.LDXV v5 i18   || iau.add i18 i18 16
	peu.pvl08 gt || lsu0.sto.64.l v12 i16 0 || cmu.alignvec v19 v16 v0 8
	cmu.cmz.i8 v19 || LSU0.LDXV v6 i17   || iau.add i17 i17 16
	peu.pvl18 gt || lsu1.sto.64.h v12 i16 8 || iau.add i16 i16 16
	_end_delay_loop_arithmeticSubmask::
	
	
	
	cmu.cmz.i32 i3 //|| sau.sub.i32 i9 i19 0 
	peu.pc1c eq || bru.jmp i7
	nop 6
	
 arithmeticSubmask_compensate:
	nop 4
	LSU0.LDO.64.L v7  i18 0    || LSU1.LDO.64.H v7  i18 8  || iau.add i18 i18 16  || VAU.ISUBS.U8 v9 v1 v2
	LSU0.LDO.64.L v8  i17 0    || LSU1.LDO.64.H v8  i17 8  || iau.add i17 i17 16  || VAU.ISUBS.U8 v10 v3 v4
	
	VAU.ISUBS.U8 v11 v5 v6 || lsu0.ldil i6 arithmeticSubmask_loop2 || lsu1.ldih i6 arithmeticSubmask_loop2 || iau.sub i15 i15 0x20
	sau.sub.i32 i9 i19 0 
	nop 4
	LSU0.STO.64.L v9 i9 0 || LSU1.STO.64.H v9 i9 8 || VAU.ISUBS.U8 v12 v7 v8
	LSU0.STO.64.L v10 i9 16 || LSU1.STO.64.H v10 i9 24 
	LSU0.STO.64.L v11 i9 32 || LSU1.STO.64.H v11 i9 40  
	LSU0.STO.64.L v12 i9 48 || LSU1.STO.64.H v12 i9 56 || iau.shr.u32 i3 i3 3
	LSU0.LD.64.L v1 i9 || LSU1.LD.64.L v20 i15 || iau.add i15 i15 8 || bru.rpl i6 i3
	nop
 arithmeticSubmask_loop2:
	nop 5
	cmu.cmz.i8 v20 || iau.add i9 i9 8
	PEU.PVL08 gt || LSU0.STo.64.L v1 i16 0 || iau.add i16 i16 8
		
	
	nop 6
	
 arithmeticSubmask_final:
    BRU.jmp i30
    iau.add i19 i19 0x40
	nop 5

.size mvcvArithmeticSubmask_asm,.-mvcvArithmeticSubmask_asm
.nowarnend
.end

	//cmu.cmvv.u8 v13 v0 || LSU1.LDXV v1 i18   || iau.add i18 i18 16
	//PEU.PVL08 gt       || LSU0.STXV v9  i16  || iau.add i16 i16 16  || VAU.ISUBS.U8 v10 v3 v4
	//cmu.cmvv.u8 v14 v0 || LSU1.LDXV v2 i17   || iau.add i17 i17 16
	//PEU.PVL08 gt       || LSU0.STXV v10 i16  || iau.add i16 i16 16  || VAU.ISUBS.U8 v11 v5 v6 
	//cmu.cmvv.u8 v15 v0 || LSU1.LDXV v13 i15  || iau.add i15 i15 16
	//PEU.PVL08 gt       || LSU0.STXV v11 i16  || iau.add i16 i16 16  || VAU.ISUBS.U8 v12 v7 v8
	//cmu.cmvv.u8 v16 v0 || LSU1.LDXV v3 i18   || iau.add i18 i18 16
	//PEU.PVL08 gt       || LSU0.STXV v12  i16 || iau.add i16 i16 16  
    
