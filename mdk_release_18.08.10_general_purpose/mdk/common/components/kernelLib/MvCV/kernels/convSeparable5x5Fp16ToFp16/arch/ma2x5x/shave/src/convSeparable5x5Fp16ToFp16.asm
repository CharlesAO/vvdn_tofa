///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief apply a convolution 5x5 Fp16ToFp16 filter, in the case of symmetric kernel.
///

.type mvcvConvSeparable5x5Fp16ToFp16_asm,@function
.version 00.51.04

.code .text.convSeparable5x5Fp16ToFp16

//void convSeparable5x5Fp16ToFp16_asm(half** out(i18), half** in(i17), half conv[3](i16), u32 inWidth(i15))                               

mvcvConvSeparable5x5Fp16ToFp16_asm: 

	LSU0.LDi.64 i0 i17
	LSU0.LDi.64 i2 i17
	LSU0.LDi.32 i4 i17 			|| 	LSU1.LD.32  i18  i18
	LSU0.LDO.64.L v20 i16 0x00	|| 	LSU1.LDO.64.H v20 i16 0x08	||	iau.shr.u32 i10, i15, 3
	lsu0.ldil i7 ___convSeparable5x5Fp16ToFp16_loop   			||  lsu1.ldih i7 ___convSeparable5x5Fp16ToFp16_loop 	||	IAU.SUB i19 i19 0x10
	
	
	LSU0.STo.64.L v24  i19 0 	||	LSU1.STo.64.H v24  i19 8 	|| 	IAU.SUB i19 i19 0x10
	LSU0.STo.64.L v25  i19 0 	||	LSU1.STo.64.H v25  i19 8 	|| 	IAU.SUB i19 i19 0x10	 	
	LSU0.STo.64.L v26  i19 0 	||	LSU1.STo.64.H v26  i19 8 	|| 	IAU.SUB i19 i19 0x10
	LSU0.STo.64.L v27  i19 0	||	LSU1.STo.64.H v27  i19 8	
		
	iau.sub i0 i0 0x10
	iau.sub i1 i1 0x10 			||	lsu0.ldo.64.l v0, i0 0 		||	lsu1.ldo.64.h v0, i0 8 
	iau.sub i4 i4 0x10			||  lsu0.ldo.64.l v1, i1 0 		||	lsu1.ldo.64.h v1, i1 8 
	iau.sub i3 i3 0x10 			||	lsu0.ldo.64.l v4, i4 0 		||	lsu1.ldo.64.h v4, i4 8  
	iau.sub i2 i2 0x10			||  lsu0.ldo.64.l v3, i3 0 		||	lsu1.ldo.64.h v3, i3 8  	
	
	cmu.cpvi.x16 i5.l v20.0  	|| 	lsu0.ldo.64.l v2, i2 0 		||	lsu1.ldo.64.h v2, i2 8 		|| iau.incs i0 0x10    
	cmu.cpivr.x16 v21 i5     	|| 	lsu0.ldo.64.l v5, i0 0 		||	lsu1.ldo.64.h v5, i0 8 		|| iau.incs i4 0x10 
	cmu.cpvi.x16 i5.l v20.1  	|| 	lsu0.ldo.64.l v9, i4 0 		||	lsu1.ldo.64.h v9, i4 8 		|| iau.incs i1 0x10
	cmu.cpivr.x16 v22 i5     	|| 	lsu0.ldo.64.l v6, i1 0 		||	lsu1.ldo.64.h v6, i1 8 		|| iau.incs i3 0x10
	cmu.cpvi.x16 i5.l v20.2  	||	lsu0.ldo.64.l v8, i3 0 		||	lsu1.ldo.64.h v8, i3 8 		|| iau.incs i2 0x10
	cmu.cpivr.x16 v23 i5     	|| 	lsu0.ldo.64.l v7, i2 0 		||	lsu1.ldo.64.h v7, i2 8 		|| iau.incs i0 0x10 
		
	vau.add.f16 v12 v0 v4 		|| 	iau.incs i4 0x10		
	vau.add.f16 v20 v1 v3 		|| 	iau.incs i1 0x10		
	vau.mul.f16 v2 v2 v23 		|| 	iau.incs i3 0x10		
	vau.mul.f16 v12 v12 v21 	|| 	iau.incs i2 0x10		
	vau.mul.f16 v20 v20 v22		||	lsu0.ldo.64.l v0, i0 0 		||	lsu1.ldo.64.h v0, i0 8 		|| iau.incs i0 0x10 		
	nop
	vau.add.f16 v4 v2 v12		||	lsu0.ldo.64.l v4, i4 0 		||	lsu1.ldo.64.h v4, i4 8 		|| iau.incs i4 0x10 
	vau.add.f16 v5 v5 v9  		||  lsu0.ldo.64.l v1, i1 0 		||	lsu1.ldo.64.h v1, i1 8 		|| iau.incs i1 0x10 
	vau.add.f16 v6 v6 v8  		||  lsu0.ldo.64.l v3, i3 0 		||	lsu1.ldo.64.h v3, i3 8 		|| iau.incs i3 0x10 	 			
	vau.mul.f16 v7 v7 v23 		||  lsu0.ldo.64.l v2, i2 0 		||	lsu1.ldo.64.h v2, i2 8 		|| iau.incs i2 0x10 
	vau.mul.f16 v5 v5 v21 
	vau.mul.f16 v6 v6 v22 
	vau.add.f16 v16 v4 v20
	vau.add.f16 v9 v7 v5 		||	lsu0.ldo.64.l v0, i0 0 		||	lsu1.ldo.64.h v0, i0 8 		|| iau.incs i0 0x10 
	vau.add.f16 v12 v0 v4  		||	lsu0.ldo.64.l v4, i4 0 		||	lsu1.ldo.64.h v4, i4 8 		|| iau.incs i4 0x10
	vau.add.f16 v20 v1 v3  		||  lsu0.ldo.64.l v1, i1 0 		||	lsu1.ldo.64.h v1, i1 8 		|| iau.incs i1 0x10	
	vau.add.f16 v11 v9 v6  		||  lsu0.ldo.64.l v3, i3 0 		||	lsu1.ldo.64.h v3, i3 8 		|| iau.incs i3 0x10
	vau.mul.f16 v8 v2 v23 		||  lsu0.ldo.64.l v2, i2 0 		||	lsu1.ldo.64.h v2, i2 8 		|| iau.incs i2 0x10 
	vau.mul.f16 v12 v12 v21  
	vau.mul.f16 v20 v20 v22 
	nop
	vau.add.f16 v8 v8 v12       ||  cmu.alignvec v14 v16 v11 12 ||	lsu0.ldo.64.l v0, i0 0 		||	lsu1.ldo.64.h v0, i0 8 		|| iau.incs i0 0x10
	vau.add.f16 v12 v0 v4   	||	lsu0.ldo.64.l v4, i4 0 		||	lsu1.ldo.64.h v4, i4 8 		||  iau.incs i4 0x10 
	vau.add.f16 v7 v1 v3    	||  lsu0.ldo.64.l v1, i1 0 		||	lsu1.ldo.64.h v1, i1 8 		||  iau.incs i1 0x10
	vau.add.f16 v17 v8 v20      ||  cmu.alignvec v15 v16 v11 14 ||  lsu0.ldo.64.l v3, i3 0 		||	lsu1.ldo.64.h v3, i3 8 		|| iau.incs i3 0x10
	vau.mul.f16 v8 v2 v23   	||  lsu0.ldo.64.l v2, i2 0 		||	lsu1.ldo.64.h v2, i2 8 		||  iau.incs i2 0x10  
	vau.mul.f16 v12 v12 v21 
	vau.mul.f16 v7 v7 v22 
	.lalign
	vau.mul.f16 v10 v11 v23     || 	cmu.alignvec v18 v11 v17 4 	|| 	bru.rpl i7 i10 			 
	vau.add.f16 v24 v14 v18     || 	cmu.alignvec v19 v11 v17 2 
	vau.add.f16 v25 v19 v15	    || 	cmu.cpvv v16 v11            
	vau.add.f16 v8 v8 v12 							
	vau.mul.f16 v26 v24 v21     || 	cmu.cpvv v11 v17 
	vau.mul.f16 v25 v25 v22	    || 	cmu.alignvec v14 v16 v11 12
	vau.add.f16 v17 v8 v7 
 ___convSeparable5x5Fp16ToFp16_loop:
 .nowarn 9
	vau.add.f16 v9 v10 v26      ||  cmu.alignvec v15 v16 v11 14	||	lsu0.ldo.64.l v0, i0 0 		||  lsu1.ldo.64.h v0, i0 8 		|| iau.incs i0 0x10
 .nowarnend
	vau.add.f16 v12 v0 v4  		||	lsu0.ldo.64.l v4, i4 0 		||	lsu1.ldo.64.h v4, i4 8 		||  iau.incs i4 0x10 
	vau.add.f16 v7 v1 v3    	||  lsu0.ldo.64.l v3, i3 0 		||	lsu1.ldo.64.h v3, i3 8 		||  iau.incs i3 0x10
	vau.add.f16 v27 v9 v25		||	lsu0.ldo.64.l v1, i1 0 		||	lsu1.ldo.64.h v1, i1 8 		||  iau.incs i1 0x10
	vau.mul.f16 v8 v2 v23  	 	||  lsu0.ldo.64.l v2, i2 0 		||	lsu1.ldo.64.h v2, i2 8 		||  iau.incs i2 0x10
	vau.mul.f16 v12 v12 v21 
	vau.mul.f16 v7 v7 v22   	||	lsu1.sto.64.l v27, i18 0	||	lsu0.sto.64.h v27, i18 8	||	iau.incs i18 0x10  
	
	
	LSU0.LDo.64.H  v27  i19 8 	||	LSU1.LDo.64.L  v27  i19 0	|| 	IAU.ADD i19 i19 0x10
	LSU0.LDo.64.H  v26  i19 8	||	LSU1.LDo.64.L  v26  i19 0	|| 	IAU.ADD i19 i19 0x10
	LSU0.LDo.64.H  v25  i19 8	||	LSU1.LDo.64.L  v25  i19 0	||	IAU.ADD i19 i19 0x10
	LSU0.LDo.64.H  v24  i19 8	|| 	LSU1.LDo.64.L  v24  i19 0	|| 	IAU.ADD i19 i19 0x10

	BRU.jmp i30
	nop 6
.size mvcvConvSeparable5x5Fp16ToFp16_asm,.-mvcvConvSeparable5x5Fp16ToFp16_asm
.end
	 
