///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief   
///

.type mvcvBitwiseNot_asm,@function
.version 00.51.05
//-------------------------------------------------------------------------------	
.code .text.bitwiseNot
//	void bitwiseNot(	u8** src, 	u8** dst, 	u32 width)
//			   			i18			i17			i16		
mvcvBitwiseNot_asm:
 .nowarn	
	LSU0.LD.32 i18 i18 						|| IAU.SHR.u32 i0 i16 6
	LSU1.LDIL i3, 32						|| IAU.SHL i2, i0, 6
	LSU0.LDIL i1, bitwiseNot_loop_end					
	LSU1.LDIH i1, bitwiseNot_loop_end
	LSU0.LDIL i6, bitwiseNot_compensate_loop_end 
	LSU1.LD.32 i17 i17	
	IAU.SUB i2, i16, i2
	
	IAU.ADD i8 i18 16
	LSU0.LDXVi v0, i18 	i3
	LSU1.LDXVi v1, i8 	i3


	CMU.CMZ.i32 i0
	PEU.PC1C eq || BRU.BRA bitwiseNot_compensate
	LSU0.LDXVi v2, i18 	i3
	LSU1.LDXVi v3, i8 	i3
	LSU0.LDIH i6, bitwiseNot_compensate_loop_end
	IAU.SUB i19 i19 0x40
	IAU.SUB i4 i19 0
	IAU.ADD i14, i17 16
	
	
 bitwiseNot_loop_start:
 .lalign

	VAU.NOT v10 v0	|| LSU0.LDXVi v0, i18 	i3	|| BRU.RPL i1 i0
 bitwiseNot_loop_end:
	VAU.NOT v11 v1	|| LSU1.LDXVi v1, i8 	i3
	VAU.NOT v12 v2  || LSU0.LDXVi v2, i18 	i3
	VAU.NOT v13 v3  || LSU1.LDXVi v3, i8 	i3
	LSU0.STXVi v10  i17 i3
	LSU1.STXVi v11  i14 i3
	LSU0.STXVi v12  i17 i3
	LSU1.STXVi v13  i14 i3
	
	CMU.CMZ.i32 i2 	
	PEU.PC1C eq	|| BRU.BRA bitwiseNot_final	
	
 bitwiseNot_compensate:
	IAU.SUB i5 i19 0
	VAU.NOT v10 v0				
	LSU0.STXV v10  i4 		|| IAU.INCS i4 16		|| VAU.NOT v11 v1
	LSU1.STXV v11  i4 		|| IAU.INCS i4 16		|| VAU.NOT v12 v2	
	LSU0.STXV v12  i4 		|| IAU.INCS i4 16		|| VAU.NOT v13 v3
	LSU1.STXV v13  i4 		|| IAU.SHR.u32 i2 i2 3		
		
 bitwiseNot_compensate_loop:
	LSU0.LDI.64.L v0 i5	|| BRU.RPL i6 i2
 bitwiseNot_compensate_loop_end:
	NOP 6
	LSU0.STI.64.L v0 i17
	
 bitwiseNot_final:
	BRU.JMP i30

	IAU.ADD i19 i19 0x40
	NOP 5
	.nowarnend
.size mvcvBitwiseNot_asm,.-mvcvBitwiseNot_asm
.end
