///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief   
///

.type mvcvArithmeticSubFp16ToFp16_asm,@function
.version 00.51.00
//-------------------------------------------------------------------------------	
.code .text.arithmeticSubFp16ToFp16
//	void arithmeticSubFp16ToFp16_asm(	fp16** src1, fp16** src2, fp16** dst, u32 width)
//			   							i18			i17				i16			i15	
.lalign
mvcvArithmeticSubFp16ToFp16_asm:
.nowarn 10	
	LSU0.LD.32 i18 i18 			|| LSU1.LD.32 i17 i17			|| IAU.SHR.u32 i0 i15 5
	LSU0.LD.32 i16 i16 			|| IAU.SHL i2 i0 5	


	LSU0.LDIL i1 arithmeticSubFp16ToFp16_loopSubFp16	|| LSU1.LDIH i1 arithmeticSubFp16ToFp16_loopSubFp16	
	IAU.SUB i2 i15 i2		
	IAU.XOR 	i10 i10 i10
	
	IAU.INCS	i10 0x10
	nop
	LSU0.LDXVi v0 i18 i10
	LSU1.LDXVi v1 i17 i10
	LSU0.LDXVi v2 i18 i10
	LSU1.LDXVi v3 i17 i10
	CMU.CMZ.i32 i0
	PEU.PC1C eq || BRU.BRA arithmeticSubFp16ToFp16_compensateSubFp16
	LSU0.LDXVi v4 i18 i10
	LSU1.LDXVi v5 i17 i10
	LSU0.LDXVi v6 i18 i10
	LSU1.LDXVi v7 i17 i10	
	LSU0.LDIL i4 0x20 
	IAU.ADD i14 i16 16
	
	.lalign
	LSU0.LDXVi v0 	i18 i10	|| BRU.RPL i1 i0
	LSU1.LDXVi v1 	i17 i10	|| VAU.SUB.f16 v8 v0 v1
	LSU0.LDXVi v2 	i18 i10	
	LSU1.LDXVi v3 	i17 i10	|| VAU.SUB.f16 v9 v2 v3 
	LSU0.LDXVi v4 	i18 i10	  
 arithmeticSubFp16ToFp16_loopSubFp16:
	LSU1.LDXVi v5 	i17 i10	|| VAU.SUB.f16 v10 v4 v5
	LSU0.LDXVi v6 	i18 i10	 
	LSU1.LDXVi v7 	i17 i10	|| VAU.SUB.f16 v11 v6 v7
	LSU0.STXVi v8   i16 i4
	LSU1.STXVi v9   i14 i4
	LSU0.STXVi v10  i16 i4
	LSU1.STXVi v11  i14 i4
	
	
 arithmeticSubFp16ToFp16_compensateSubFp16:
	VAU.SUB.f16 v8 v0 v1	
	VAU.SUB.f16 v9 v2 v3		
	VAU.SUB.f16 v10 v4 v5 || IAU.SHR.u32 i2 i2 3	
	
	
	PEU.PC1I gt || LSU0.ST.64.L v8  i16
	PEU.PC1I gt || LSU0.STO.64.H v8 i16 0x08
	IAU.INCS i2 -1
	PEU.PC1I gt || LSU0.STO.64.L v9 i16 0x10
	PEU.PC1I gt || LSU0.STO.64.H v9 i16 0x18
	IAU.INCS i2 -1
	PEU.PC1I gt || LSU0.STO.64.L v10	i16 0x20
	PEU.PC1I gt || LSU0.STO.64.H v10 i16 0x28
	
	BRU.JMP i30
	nop 6
	.nowarnend
.size mvcvArithmeticSubFp16ToFp16_asm,.-mvcvArithmeticSubFp16ToFp16_asm
.end
