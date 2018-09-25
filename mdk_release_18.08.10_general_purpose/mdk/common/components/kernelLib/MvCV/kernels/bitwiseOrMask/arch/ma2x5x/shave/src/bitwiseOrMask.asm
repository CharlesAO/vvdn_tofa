///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief   
///

.type mvcvBitwiseOrMask_asm,@function
.version 00.51.05
//-------------------------------------------------------------------------------	
.code .text.bitwiseOrMask
//	void bitwiseOrMask(	u8** src1Addr, 	u8** src2Addr, 	u8** dstAddr,	u8** maskAddr, 	u32 width)
//			   					i18				i17					i16				i15				i14
mvcvBitwiseOrMask_asm:
 .nowarn
	
	LSU0.LD.32 i18 i18 			|| LSU1.LD.32 i17 i17			|| IAU.SHR.u32 i0 i14 6
	LSU0.LD.32 i16 i16 			|| LSU1.LD.32 i15 i15			|| IAU.SHL i2, i0, 6
	LSU0.LDIL i1, bitwiseOrMask_loop 		|| LSU1.LDIH i1, bitwiseOrMask_loop	
	LSU0.LDIL i4, bitwiseOrMask_compensate || LSU1.LDIH i4, bitwiseOrMask_compensate	|| IAU.SUB i19 i19 0x40 
	LSU0.LDIL i10 16
	VAU.XOR v12 v12 v12
	NOP
	
	LSU0.LDXVi v0	i18 i10
	LSU1.LDXVi v1 	i17 i10
	LSU0.LDXVi v23 	i15	i10	
	LSU1.LDXVi v2 	i18 i10
	LSU0.LDXVi v3 	i17 i10
	LSU1.LDXVi v22 	i15 i10	
	LSU0.LDXVi v4 	i18 i10
	LSU1.LDXVi v5 	i17 i10
	LSU0.LDXVi v21 	i15	i10	
	LSU1.LDXVi v6 	i18 i10
	LSU0.LDXVi v7 	i17 i10
	LSU1.LDXVi v20 	i15	i10
	
	CMU.CMZ.i32 i0
	PEU.PC1C eq 	|| BRU.JMP i4
	IAU.SUB i3 i19 0
	LSU0.LDIL i6, bitwiseOrMask_compensate_loop 
	LSU1.LDIH i6, bitwiseOrMask_compensate_loop
	IAU.SUB i2, i14, i2
	NOP 2
	
	
	LSU1.LDXVi v0  i18  i10	|| BRU.RPL i1 i0
	LSU0.LDXVi v1  i17  i10	|| VAU.OR v8 v0 v1
	LSU1.LDXVi v2  i18  i10	
	LSU0.LDXVi v3  i17  i10	|| VAU.OR v9 v2 v3 
	LSU1.LDXVi v4  i18  i10 	 
	LSU0.LDXVi v5  i17  i10 || VAU.OR v10 v4 v5	
	LSU1.LDXVi v6  i18  i10 	
	LSU0.LDXVi v7  i17  i10 || VAU.OR v11 v6 v7	
	LSU1.LDXVi v23 i15  i10 || CMU.CMVV.u8 v23 v12	
 bitwiseOrMask_loop:
	PEU.PVL08 gt 			|| LSU0.STXVi v8 i16 i10
	LSU1.LDXVi v22 i15	i10 || CMU.CMVV.u8 v22 v12	
	PEU.PVL08 gt			|| LSU0.STXVi v9 i16 i10	
	LSU1.LDXVi v21 i15  i10 || CMU.CMVV.u8 v21 v12
	PEU.PVL08 gt 			|| LSU0.STXVi v10 i16 i10		
	LSU1.LDXVi v20 i15  i10 || CMU.CMVV.u8 v20 v12
	PEU.PVL08 gt 			|| LSU0.STXVi v11 i16 i10	
	
	CMU.CMZ.i32 i2	
	PEU.PC1C eq 	|| BRU.BRA bitwiseOrMask_final 
	NOP 6
	
	
 bitwiseOrMask_compensate:

	VAU.OR v8 v0 v1		|| IAU.SUB i5 i19 0

	LSU0.STO.64.L v8  i3 0 	|| LSU1.STO.64.H v8 i3  8	|| VAU.OR v9 v2 v3		 
	LSU0.STO.64.L v9  i3 16 || LSU1.STO.64.H v9 i3  24	|| VAU.OR v10 v4 v5	
	LSU0.STO.64.L v10 i3 32 || LSU1.STO.64.H v10 i3 40	|| VAU.OR v11 v6 v7	|| IAU.SUB i15 i15 0x40
	LSU0.STO.64.L v11 i3 48 || LSU1.STO.64.H v11 i3 56	|| IAU.SHR.u32 i2 i2 3	
	
	
	LSU0.LD.64.L v0 i5	|| LSU1.LD.64.L v20 i15  || BRU.RPL i6 i2		
	IAU.ADD i5 i5 8
 bitwiseOrMask_compensate_loop:
	NOP 4
	IAU.ADD i15 i15 8
	CMU.CMVV.u8 v20 v12 
	PEU.PVL08 gt 		|| LSU0.ST.64.L v0 i16	|| IAU.ADD i16 i16 8
	
	
 bitwiseOrMask_final:
	BRU.JMP i30
	IAU.ADD i19 i19 0x40
	NOP 5
.size mvcvBitwiseOrMask_asm,.-mvcvBitwiseOrMask_asm
 .nowarnend
.end
