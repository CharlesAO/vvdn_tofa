///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvArithmeticAdd_asm,@function
.version 00.51.04

.data .data.arithmeticAdd
.align 4
.code .text.arithmeticAdd

//void arithmeticAdd(u8** src1, u8** src2, u8** dst, u32 width)
//					   i18         i17       i16       i15
mvcvArithmeticAdd_asm:
.nowarn	
	
	LSU0.LDIL i2 0x003F || LSU1.LDIH i2 0x0 
	lsu0.ldil i4 0x10   || lsu1.ldil i6 0x20 
    LSU0.LD.32  i18  i18 || LSU1.LD.32  i17  i17 
    lsu1.ldil i5 arithmeticAdd_loop || lsu0.ldih i5 arithmeticAdd_loop || IAU.AND i3 i15 i2
	LSU0.ldil i7 arithmeticAdd_final || LSU1.ldih i7 arithmeticAdd_final
	LSU0.ldil i9 arithmeticAdd_compensate || LSU1.ldih i9 arithmeticAdd_compensate 
	LSU0.LD.32  i16  i16 || IAU.SHR.u32 i1 i15 6
	nop 3

	cmu.cmz.i8 i1
	peu.pc1c eq || bru.jmp i9
	nop 6

	LSU0.LDO.64.L v1  i18 0     || LSU1.LDO.64.H v1  i18 8  
	LSU0.LDO.64.L v2  i17 0     || LSU1.LDO.64.H v2  i17 8
	LSU0.LDO.64.L v3  i18 16    || LSU1.LDO.64.H v3  i18 24 
	LSU0.LDO.64.L v4  i17 16    || LSU1.LDO.64.H v4  i17 24 
	LSU0.LDO.64.L v5  i18 32    || LSU1.LDO.64.H v5  i18 40  
	LSU0.LDO.64.L v6  i17 32    || LSU1.LDO.64.H v6  i17 40 
	LSU0.LDO.64.L v7  i18 48    || LSU1.LDO.64.H v7  i18 56 || iau.add i18 i18 64 
	LSU0.LDO.64.L v8  i17 48    || LSU1.LDO.64.H v8  i17 56 || iau.add i17 i17 64
	
	LSU1.LDXVI v1 i18 i4 || VAU.IADDS.U8 v9 v1 v2 || bru.rpl i5 i1 _end_delay_loop_arithmeticAdd
	LSU0.LDXVI v2 i17 i4 
		LSU1.STXVI v9 i16 i4 
	LSU0.LDXVI v3 i18 i4 || VAU.IADDS.U8 v10 v3 v4 
	LSU1.LDXVI v4 i17 i4 
 arithmeticAdd_loop:
		LSU0.STXVI v10 i16 i4
	LSU1.LDXVI v5 i18 i4 || VAU.IADDS.U8 v11 v5 v6  
	LSU0.LDXVI v6 i17 i4 
		LSU1.STXVI v11 i16 i4
	LSU0.LDXVI v7 i18 i4 || VAU.IADDS.U8 v12 v7 v8  
	LSU1.LDXVI v8 i17 i4 
		LSU0.STXVI v12 i16 i4
	_end_delay_loop_arithmeticAdd:
	
	iau.sub i18 i18 64
	iau.sub i17 i17 64
	
	cmu.cmz.i32 i3 
	peu.pc1c eq || bru.jmp i7
nop 6
	
 arithmeticAdd_compensate:



	LSU0.LDO.64.L v1  i18 0     || LSU1.LDO.64.H v1  i18 8
	LSU0.LDO.64.L v2  i17 0     || LSU1.LDO.64.H v2  i17 8
	LSU0.LDO.64.L v3  i18 16    || LSU1.LDO.64.H v3  i18 24 
	LSU0.LDO.64.L v4  i17 16    || LSU1.LDO.64.H v4  i17 24
	LSU0.LDO.64.L v5  i18 32    || LSU1.LDO.64.H v5  i18 40 
	LSU0.LDO.64.L v6  i17 32    || LSU1.LDO.64.H v6  i17 40 
	LSU0.LDO.64.L v7  i18 48    || LSU1.LDO.64.H v7  i18 56 
	LSU0.LDO.64.L v8  i17 48    || LSU1.LDO.64.H v8  i17 56  
	nop 6



	VAU.IADDS.U8 v9 v1 v2 
	VAU.IADDS.U8 v10 v3 v4 
	VAU.IADDS.U8 v11 v5 v6 	
	VAU.IADDS.U8 v12 v7 v8 || IAU.SUB i3, i3, 8 


	PEU.PC1I GTE || LSU0.STO.64.L v9  i16  0 || IAU.SUB i3, i3, 8 
	PEU.PC1I GTE || LSU0.STO.64.H v9  i16  8 || IAU.SUB i3, i3, 8 
	BRU.jmp i30
	PEU.PC1I GTE || LSU0.STO.64.L v10  i16 16 || IAU.SUB i3, i3, 8 
	PEU.PC1I GTE || LSU0.STO.64.H v10  i16  24 || IAU.SUB i3, i3, 8 
	PEU.PC1I GTE || LSU0.STO.64.L v11  i16  32 || IAU.SUB i3, i3, 8 
	PEU.PC1I GTE || LSU0.STO.64.H v11  i16  40 || IAU.SUB i3, i3, 8 
	PEU.PC1I GTE || LSU0.STO.64.L v12  i16  48 || IAU.SUB i3, i3, 8 
	PEU.PC1I GTE || LSU0.STO.64.H v12  i16  56 || IAU.SUB i3, i3, 8 


 arithmeticAdd_final:
    BRU.jmp i30
nop 6
.nowarnend
.size mvcvArithmeticAdd_asm,.-mvcvArithmeticAdd_asm
.end
