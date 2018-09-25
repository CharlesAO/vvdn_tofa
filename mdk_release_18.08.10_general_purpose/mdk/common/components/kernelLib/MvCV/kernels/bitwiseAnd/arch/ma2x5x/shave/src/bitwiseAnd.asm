///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief   
///

.type mvcvBitwiseAnd_asm,@function
.version 00.51.05
//-------------------------------------------------------------------------------	
.code .text.bitwiseAnd
//	void bitwiseAnd(	u8** src1, 	u8** src2, 	u8** dst, 	u32 width)
//			   			i18		i17		i16		i15	
mvcvBitwiseAnd_asm:
 .nowarn	
	LSU0.LD.32 i18 i18 			|| LSU1.LD.32 i17 i17			|| IAU.SHR.u32 i0 i15 6
	LSU0.LD.32 i16 i16 			|| IAU.SHL i2, i0, 6			|| LSU1.LDIL i10, 16
	LSU0.LDIL i1, bitwiseAnd_loop_end	|| LSU1.LDIH i1, bitwiseAnd_loop_end	|| IAU.SUB i2, i15, i2	
	                                                               IAU.SUB i19 i19 0x40 
	LSU0.LDIL i6, bitwiseAnd_compensate_loop_end || LSU1.LDIH i6, bitwiseAnd_compensate_loop_end
	NOP 2
	
	LSU0.LDXVi v0, i18 i10
	LSU1.LDXVi v1, i17 i10
	LSU0.LDXVi v2, i18 i10
	LSU1.LDXVi v3, i17 i10
	LSU0.LDXVi v4, i18 i10
	LSU1.LDXVi v5, i17 i10
	LSU0.LDXVi v6, i18 i10
	LSU1.LDXVi v7, i17 i10	
	CMU.CMZ.i32 i0
	PEU.PC1C eq || BRU.BRA bitwiseAnd_compensate
	NOP 3
	IAU.SUB i3 i19 0
	lsu0.ldil i4, 32
	iau.add i14, i16 16
	
 bitwiseAnd_loop_start:
	LSU0.LDXVi v0, i18 i10	|| BRU.RPL i1 i0
	LSU1.LDXVi v1, i17 i10	|| VAU.AND v8 v0 v1
	LSU0.LDXVi v2, i18 i10	
	LSU1.LDXVi v3, i17 i10	|| VAU.AND v9 v2 v3 
	LSU0.LDXVi v4, i18 i10	 
 bitwiseAnd_loop_end:	

	LSU1.LDXVi v5, i17 i10	|| VAU.AND v10 v4 v5

	LSU0.LDXVi v6, i18 i10	 
	LSU1.LDXVi v7, i17 i10	|| VAU.AND v11 v6 v7
	LSU0.STXVi v8   i16 i4
	LSU1.STXVi v9   i14 i4
	LSU0.STXVi v10  i16 i4
	LSU1.STXVi v11  i14 i4
	
	CMU.CMZ.i32 i2 	
	PEU.PC1C eq	|| BRU.BRA bitwiseAnd_final
	
 bitwiseAnd_compensate:		
	VAU.AND v8 v0 v1	|| IAU.SUB i5 i19 0
	LSU0.STXV v8  i3 	|| IAU.INCS i3 16		|| VAU.AND v9 v2 v3		 
	LSU1.STXV v9  i3 	|| IAU.INCS i3 16		|| VAU.AND v10 v4 v5	
	LSU0.STXV v10 i3 	|| IAU.INCS i3 16		|| VAU.AND v11 v6 v7
	LSU1.STXV v11 i3 	|| IAU.SHR.u32 i2 i2 3		
		
 bitwiseAnd_compensate_loop:
	LSU0.LDI.64.L v0 i5	|| BRU.RPL i6 i2
 bitwiseAnd_compensate_loop_end:
	NOP 6
	LSU0.STI.64.L v0 i16
	
 bitwiseAnd_final:
	BRU.JMP i30
	IAU.ADD i19 i19 0x40
	NOP 5
.size mvcvBitwiseAnd_asm,.-mvcvBitwiseAnd_asm
 .nowarnend
.end
