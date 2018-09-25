///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief   
///

.type mvcvSumOfSquaredDiff7x7U8ToU32_asm,@function
.version 00.51.05
//-------------------------------------------------------------------------------
	
.code .text.sumOfSquaredDiff7x7U8ToU32
//void sumOfSquaredDiff7x7U8ToU32(u8** in1(i18), u8** in2(i17), u32** out(i16), u32 width(i15))
mvcvSumOfSquaredDiff7x7U8ToU32_asm:
.nowarn
	LSU0.LDI.32 i0  i18  	|| LSU1.LDIL i14, sumOfSquaredDiff7x7U8ToU32_loop 
	LSU0.LDI.32 i7  i17 	|| LSU1.LDIH i14, sumOfSquaredDiff7x7U8ToU32_loop
	LSU0.LDI.32 i1  i18 	|| LSU1.LD.32 i16 i16		|| IAU.SHR.u32 i15 i15 3 
	LSU0.LDI.32 i8  i17		|| IAU.SUB i15 i15 1
	LSU0.LDI.32 i2  i18 	
	LSU0.LDI.32 i9  i17 
	LSU0.LDI.32 i3  i18 		
	lsu0.ldi.32 i10 i17		|| IAU.SUB i0 i0 4		
	LSU0.LDI.32 i4  i18 	|| iau.sub i7 i7 4
	lsu0.ldi.32 i11 i17		|| IAU.SUB i1 i1 4		
	LSU0.LDI.32 i5  i18 	|| IAU.SUB i8 i8 4		
	lsu0.ldi.32 i12 i17		|| IAU.SUB i2 i2 4		
	LSU0.LDI.32 i6  i18		|| IAU.SUB i9 i9 4		
	lsu0.ldi.32 i13 i17 	|| IAU.SUB i3 i3 4	
		
	lsu0.ldi.128.u8.u16 v0 i0	|| lsu1.ldi.128.u8.u16 v3 i7
	lsu0.ldi.128.u8.u16 v1 i0	|| lsu1.ldi.128.u8.u16 v4 i7
	lsu0.ldi.128.u8.u16 v0 i1	|| lsu1.ldi.128.u8.u16 v3 i8
	lsu0.ldi.128.u8.u16 v1 i1	|| lsu1.ldi.128.u8.u16 v4 i8
	
	IAU.SUB i19 i19 16
	LSU0.STo.64.L v24 i19 0	||	LSU1.STo.64.H v24 i19 8	|| IAU.SUB i19 i19 16
	LSU0.STo.64.L v25 i19 0	||	LSU1.STo.64.H v25 i19 8	
				
	vau.sub.i16 v23 v0 v3			|| IAU.SUB i10 i10 4
	vau.sub.i16 v22 v1 v4			|| IAU.SUB i4  i4  4
	vau.sub.i16 v0  v0 v3			|| iau.sub i11 i11 4
	vau.sub.i16 v1  v1 v4			|| IAU.SUB i5  i5  4					
	
	cmu.cpvv.i16.i32 v20 v23		|| iau.sub i12 i12 4
	cmu.cpvv.i16.i32 v18 v22		|| IAU.SUB i6  i6  4	||	vau.mul.i32 v20 v20 v20
	cmu.cpvv.i16.i32 v6  v0			|| iau.sub i13 i13 4	||	vau.mul.i32 v18 v18 v18
	cmu.cpvv.i16.i32 v8  v1			|| vau.mul.i32 v6 v6 v6	
	
	cmu.alignvec v23 v23 v23 8		|| 	vau.mul.i32 v8 v8 v8	
	cmu.alignvec v22 v22 v22 8
	cmu.alignvec v0  v0  v0  8
	cmu.alignvec v1  v1  v1  8	
		
	cmu.cpvv.i16.i32 v19 v23			
	cmu.cpvv.i16.i32 v17 v22		|| vau.mul.i32 v19 v19 v19
	cmu.cpvv.i16.i32 v7 v0			|| vau.mul.i32 v17 v17 v17
	cmu.cpvv.i16.i32 v9 v1			|| vau.mul.i32 v7 v7 v7		|| lsu0.ldi.128.u8.u16 v0 i2	|| lsu1.ldi.128.u8.u16 v3 i9
	vau.mul.i32 v9 v9 v9										|| lsu0.ldi.128.u8.u16 v1 i2	|| lsu1.ldi.128.u8.u16 v4 i9						
	nop
	
	vau.add.i32 v23 v6 v20
	vau.add.i32 v22 v7 v19
	vau.add.i32 v21 v8 v18
	vau.add.i32 v20 v9 v17
	
	vau.sub.i16 v0 v0 v3		
	vau.sub.i16 v1 v1 v4				
	
	cmu.cpvv.i16.i32 v6 v0
	cmu.cpvv.i16.i32 v8 v1			|| vau.mul.i32 v6 v6 v6
	
	cmu.alignvec v0 v0 v0 8			|| vau.mul.i32 v8 v8 v8
	cmu.alignvec v1 v1 v1 8
	
	cmu.cpvv.i16.i32 v7 v0	
	cmu.cpvv.i16.i32 v9 v1			|| vau.mul.i32 v7 v7 v7		|| lsu0.ldi.128.u8.u16 v0 i3	|| lsu1.ldi.128.u8.u16 v3 i10
	vau.mul.i32 v9 v9 v9										|| lsu0.ldi.128.u8.u16 v1 i3	|| lsu1.ldi.128.u8.u16 v4 i10				
	nop
	
	vau.add.i32 v23 v6 v23
	vau.add.i32 v22 v7 v22
	vau.add.i32 v21 v8 v21
	vau.add.i32 v20 v9 v20
	
	vau.sub.i16 v0 v0 v3		
	vau.sub.i16 v1 v1 v4				
	
	cmu.cpvv.i16.i32 v6 v0
	cmu.cpvv.i16.i32 v8 v1			|| vau.mul.i32 v6 v6 v6
	
	cmu.alignvec v0 v0 v0 8			|| vau.mul.i32 v8 v8 v8
	cmu.alignvec v1 v1 v1 8
	
	cmu.cpvv.i16.i32 v7 v0	
	cmu.cpvv.i16.i32 v9 v1			|| vau.mul.i32 v7 v7 v7		|| lsu0.ldi.128.u8.u16 v0 i4	|| lsu1.ldi.128.u8.u16 v3 i11
	vau.mul.i32 v9 v9 v9										|| lsu0.ldi.128.u8.u16 v1 i4	|| lsu1.ldi.128.u8.u16 v4 i11				
	nop
	
	vau.add.i32 v23 v6 v23
	vau.add.i32 v22 v7 v22
	vau.add.i32 v21 v8 v21
	vau.add.i32 v20 v9 v20
	
	vau.sub.i16 v0 v0 v3		
	vau.sub.i16 v1 v1 v4				
	
	cmu.cpvv.i16.i32 v6 v0
	cmu.cpvv.i16.i32 v8 v1			|| vau.mul.i32 v6 v6 v6
	
	cmu.alignvec v0 v0 v0 8			|| vau.mul.i32 v8 v8 v8
	cmu.alignvec v1 v1 v1 8
	
	cmu.cpvv.i16.i32 v7 v0	
	cmu.cpvv.i16.i32 v9 v1			|| vau.mul.i32 v7 v7 v7		|| lsu0.ldi.128.u8.u16 v0 i5	|| lsu1.ldi.128.u8.u16 v3 i12
	vau.mul.i32 v9 v9 v9										|| lsu0.ldi.128.u8.u16 v1 i5	|| lsu1.ldi.128.u8.u16 v4 i12			
	nop
	
	vau.add.i32 v23 v6 v23
	vau.add.i32 v22 v7 v22
	vau.add.i32 v21 v8 v21
	vau.add.i32 v20 v9 v20
	
	vau.sub.i16 v0 v0 v3		
	vau.sub.i16 v1 v1 v4				
	
	cmu.cpvv.i16.i32 v6 v0
	cmu.cpvv.i16.i32 v8 v1			|| vau.mul.i32 v6 v6 v6
	
	cmu.alignvec v0 v0 v0 8			|| vau.mul.i32 v8 v8 v8
	cmu.alignvec v1 v1 v1 8
	
	cmu.cpvv.i16.i32 v7 v0
	cmu.cpvv.i16.i32 v9 v1			|| vau.mul.i32 v7 v7 v7		|| lsu0.ldi.128.u8.u16 v0 i6	|| lsu1.ldi.128.u8.u16 v3 i13
	vau.mul.i32 v9 v9 v9										|| lsu0.ldi.128.u8.u16 v1 i6	|| lsu1.ldi.128.u8.u16 v4 i13				
	nop
	
	vau.add.i32 v23 v6 v23
	vau.add.i32 v22 v7 v22
	vau.add.i32 v21 v8 v21
	vau.add.i32 v20 v9 v20
	
	vau.sub.i16 v0 v0 v3
	vau.sub.i16 v1 v1 v4
	
	cmu.cpvv.i16.i32 v6 v0
	cmu.cpvv.i16.i32 v8 v1			|| vau.mul.i32 v6 v6 v6
	
	cmu.alignvec v0 v0 v0 8			|| vau.mul.i32 v8 v8 v8
	cmu.alignvec v1 v1 v1 8
	
	cmu.cpvv.i16.i32 v7 v0
	cmu.cpvv.i16.i32 v9 v1			|| vau.mul.i32 v7 v7 v7
	vau.mul.i32 v9 v9 v9				
	nop
	
	vau.add.i32 v23 v6 v23
	vau.add.i32 v22 v7 v22
	vau.add.i32 v21 v8 v21
	vau.add.i32 v20 v9 v20
								 			 
	VAU.ACCPZ.i32 v22	|| CMU.ALIGNVEC v0 v23 v22 4								
	VAU.ACCP.i32 v0	 	|| CMU.ALIGNVEC v1 v23 v22 8	
	VAU.ACCP.i32 v1     || CMU.ALIGNVEC v2 v23 v22 12   
	VAU.ACCP.i32 v2     || CMU.ALIGNVEC v3 v22 v21 4	
	vau.accp.i32 v3		|| cmu.alignvec v4 v22 v21 8	
	vau.accp.i32 v4		|| cmu.alignvec v5 v22 v21 12	
	VAU.ACCPW.i32 v24 v5								
	 
	vau.accpz.i32 v21	||	cmu.alignvec v0 v22 v21 4	|| lsu0.ldi.128.u8.u16 v0 i0	|| lsu1.ldi.128.u8.u16 v7  i7								
	vau.accp.i32 v0		|| cmu.alignvec v1 v22 v21 8	|| lsu0.ldi.128.u8.u16 v1 i1	|| lsu1.ldi.128.u8.u16 v8  i8
	vau.accp.i32 v1		|| cmu.alignvec v2 v22 v21 12	|| lsu0.ldi.128.u8.u16 v2 i2	|| lsu1.ldi.128.u8.u16 v9  i9
	vau.accp.i32 v2		|| cmu.alignvec v3 v21 v20 4	|| lsu0.ldi.128.u8.u16 v3 i3	|| lsu1.ldi.128.u8.u16 v10 i10
	vau.accp.i32 v3		|| cmu.alignvec v4 v21 v20 8	|| lsu0.ldi.128.u8.u16 v4 i4	|| lsu1.ldi.128.u8.u16 v11 i11
	vau.accp.i32 v4		|| cmu.alignvec v5 v21 v20 12	|| lsu0.ldi.128.u8.u16 v5 i5	|| lsu1.ldi.128.u8.u16 v12 i12
	vau.accpw.i32 v25 v5 								|| lsu0.ldi.128.u8.u16 v6 i6	|| lsu1.ldi.128.u8.u16 v13 i13
	 
	VAU.SUB.i16 v0 v0 v7
	vau.sub.i16 v1 v1 v8	
 	VAU.SUB.i16 v2 v2 v9	|| lsu0.sto.64.l v24 i16 0	|| lsu1.sto.64.h v24 i16 8		
	VAU.SUB.i16 v3 v3 v10	|| lsu0.sto.64.l v25 i16 16	|| lsu1.sto.64.h v25 i16 24		|| iau.incs i16 32
		
	vau.sub.i32 v22 v21 0
	vau.sub.i32 v21 v20 0


	CMU.CMZ.i32 i15
	PEU.PC1C eq || BRU.BRA sumOfSquaredDiff7x7U8ToU32_end
	NOP 6

	
	vau.sub.i16 v4 v4 v11		|| BRU.RPL i14 i15, sumOfSquaredDiff7x7U8ToU32_asm_delay_slot
	vau.sub.i16 v5 v5 v12
	vau.sub.i16 v6 v6 v13		||	cmu.cpvv.i16.i32 v14 v0
	
	
	cmu.cpvv.i16.i32 v15 v1		||	VAU.MUL.i32 v14 v14 v14	
	cmu.cpvv.i16.i32 v16 v2		||	vau.mul.i32 v15 v15 v15
	cmu.cpvv.i16.i32 v17 v3		||	VAU.MUL.i32 v16 v16 v16
	cmu.cpvv.i16.i32 v18 v4		||	vau.mul.i32 v17 v17 v17
	cmu.cpvv.i16.i32 v19 v5		||	VAU.MUL.i32 v18 v18 v18
	cmu.cpvv.i16.i32 v20 v6		||	vau.mul.i32 v19 v19 v19
	
	cmu.alignvec v0 v0 v0 8		||	VAU.MUL.i32 v20 v20 v20
	cmu.alignvec v1 v1 v1 8		||	VAU.ACCPZ.i32 v14
	cmu.alignvec v2 v2 v2 8		||	VAU.ACCP.i32 v15
	cmu.alignvec v3 v3 v3 8		||	VAU.ACCP.i32 v16
	cmu.alignvec v4 v4 v4 8		||	VAU.ACCP.i32 v17
	cmu.alignvec v5 v5 v5 8		||	VAU.ACCP.i32 v18
	cmu.alignvec v6 v6 v6 8		||	VAU.ACCP.i32 v19
	
	cmu.cpvv.i16.i32 v0 v0		||	VAU.ACCPW.i32 v23 v20
	cmu.cpvv.i16.i32 v1 v1		||	VAU.MUL.i32 v0 v0 v0
	cmu.cpvv.i16.i32 v2 v2		||	vau.mul.i32 v1 v1 v1
	cmu.cpvv.i16.i32 v3 v3		||	VAU.MUL.i32 v2 v2 v2
	cmu.cpvv.i16.i32 v4 v4		||	vau.mul.i32 v3 v3 v3
	cmu.cpvv.i16.i32 v5 v5		||	VAU.MUL.i32 v4 v4 v4
	cmu.cpvv.i16.i32 v6 v6		||	vau.mul.i32 v5 v5 v5
			 		
	VAU.MUL.i32 v6 v6 v6
				
	VAU.ACCPZ.i32 v0
	VAU.ACCP.i32 v1
	VAU.ACCP.i32 v2
	VAU.ACCP.i32 v3
	VAU.ACCP.i32 v4
	VAU.ACCP.i32 v5
	VAU.ACCPW.i32 v20 v6
	
	vau.accpz.i32 v21	|| cmu.alignvec v0 v22 v21 4									
	vau.accp.i32 v0		|| cmu.alignvec v1 v22 v21 8		
	vau.accp.i32 v1		|| cmu.alignvec v2 v22 v21 12		
	vau.accp.i32 v2		|| cmu.alignvec v3 v21 v23 4		
	vau.accp.i32 v3		|| cmu.alignvec v4 v21 v23 8		
	vau.accp.i32 v4		|| cmu.alignvec v5 v21 v23 12		
	vau.accpw.i32 v24 v5										
	
	vau.accpz.i32 v23	|| cmu.alignvec v0 v21 v23 4		|| lsu0.ldi.128.u8.u16 v0 i0 	|| lsu1.ldi.128.u8.u16 v7  i7									
	vau.accp.i32 v0		|| cmu.alignvec v1 v21 v23 8		|| lsu0.ldi.128.u8.u16 v1 i1	|| lsu1.ldi.128.u8.u16 v8  i8
	vau.accp.i32 v1		|| cmu.alignvec v2 v21 v23 12		|| lsu0.ldi.128.u8.u16 v2 i2	|| lsu1.ldi.128.u8.u16 v9  i9
	vau.accp.i32 v2		|| cmu.alignvec v3 v23 v20 4		|| lsu0.ldi.128.u8.u16 v3 i3	|| lsu1.ldi.128.u8.u16 v10 i10
	vau.accp.i32 v3		|| cmu.alignvec v4 v23 v20 8		|| lsu0.ldi.128.u8.u16 v4 i4	|| lsu1.ldi.128.u8.u16 v11 i11
	vau.accp.i32 v4		|| cmu.alignvec v5 v23 v20 12		|| lsu0.ldi.128.u8.u16 v5 i5	|| lsu1.ldi.128.u8.u16 v12 i12
sumOfSquaredDiff7x7U8ToU32_loop:
	vau.accpw.i32 v25 v5									|| lsu0.ldi.128.u8.u16 v6 i6	|| lsu1.ldi.128.u8.u16 v13 i13	
	vau.sub.i16 v0 v0 v7									
	vau.sub.i16 v1 v1 v8	
	vau.sub.i16 v2 v2 v9
	vau.sub.i16 v3 v3 v10	||	lsu0.sto.64.l v24 i16 0		||	lsu1.sto.64.h v24 i16 8		
	vau.sub.i32 v22 v23	0	||	lsu0.sto.64.l v25 i16 16	||	lsu1.sto.64.h v25 i16 24	|| iau.incs i16 32
	vau.sub.i32 v21 v20 0
sumOfSquaredDiff7x7U8ToU32_asm_delay_slot:	


sumOfSquaredDiff7x7U8ToU32_end:
	LSU0.LDo.64.L v25 i19 0 ||	LSU1.LDo.64.H v25 i19 8 || IAU.ADD i19 i19 16    	
	LSU0.LDo.64.L v24 i19 0 ||	LSU1.LDo.64.H v24 i19 8 || IAU.ADD i19 i19 16    
	

	BRU.jmp i30
	NOP 6
.size mvcvSumOfSquaredDiff7x7U8ToU32_asm,.-mvcvSumOfSquaredDiff7x7U8ToU32_asm
	.nowarnend
.end
