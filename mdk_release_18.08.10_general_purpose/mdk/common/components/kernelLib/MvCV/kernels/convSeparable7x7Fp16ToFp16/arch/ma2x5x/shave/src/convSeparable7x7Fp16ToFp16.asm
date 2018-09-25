///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief apply a convolution 7x7 Fp16ToFp16 filter, in the case of symmetric kernel.
///

.type mvcvConvSeparable7x7Fp16ToFp16_asm,@function
.version 00.51.04

.code .text.convSeparable7x7Fp16ToFp16

//void convSeparable7x7Fp16ToFp16_asm(half** out(i18), half** in(i17), half conv[4](i16), u32 inWidth(i15))

mvcvConvSeparable7x7Fp16ToFp16_asm: 	 
	 
	 LSU0.LDi.64 i0 i17
	 LSU0.LDi.64 i2 i17
	 LSU0.LDi.64 i4 i17  
	 LSU0.LDi.32 i6 i17 			|| LSU1.LD.32  i18  i18
	 LSU0.LDO.64.L v20 i16 0x00		|| LSU1.LDO.64.H v20 i16 0x08	||	iau.shr.u32 i10, i15, 3
	 lsu0.ldil i7 convSeparable7x7Fp16ToFp16_loop   				||  lsu1.ldih i7 convSeparable7x7Fp16ToFp16_loop 	||	IAU.SUB i19 i19 0x10
	 	 
	 LSU0.STo.64.L v24  i19 0 		||	LSU1.STo.64.H v24  i19 8 	|| 	IAU.SUB i19 i19 0x10
	 LSU0.STo.64.L v25  i19 0 		||	LSU1.STo.64.H v25  i19 8 	|| 	IAU.SUB i19 i19 0x10	 	
	 LSU0.STo.64.L v26  i19 0 		||	LSU1.STo.64.H v26  i19 8 	|| 	IAU.SUB i19 i19 0x10
	 LSU0.STo.64.L v27  i19 0		||	LSU1.STo.64.H v27  i19 8	|| 	IAU.SUB i19 i19 0x10
	 LSU0.STo.64.L v28  i19 0		||	LSU1.STo.64.H v28  i19 8	|| 	IAU.SUB i19 i19 0x10
	 LSU0.STo.64.L v29  i19 0		||	LSU1.STo.64.H v29  i19 8	|| 	IAU.SUB i19 i19 0x10
	 LSU0.STo.64.L v30  i19 0		||	LSU1.STo.64.H v30  i19 8	|| 	IAU.SUB i19 i19 0x10
	 LSU0.STo.64.L v31  i19 0		||	LSU1.STo.64.H v31  i19 8	
	
	 iau.sub i0 i0 0x10		
	 iau.sub i6 i6 0x10				||	lsu0.ldo.64.l v0, i0 0 		||	lsu1.ldo.64.h v0, i0 8 			
	 iau.sub i1 i1 0x10				||	lsu0.ldo.64.l v6, i6 0 		||	lsu1.ldo.64.h v6, i6 8 		
	 iau.sub i5 i5 0x10				||	lsu0.ldo.64.l v1, i1 0 		||	lsu1.ldo.64.h v1, i1 8 		
	 iau.sub i2 i2 0x10				||	lsu0.ldo.64.l v5, i5 0 		||	lsu1.ldo.64.h v5, i5 8 		
	 iau.sub i4 i4 0x10				||	lsu0.ldo.64.l v2, i2 0 		||	lsu1.ldo.64.h v2, i2 8 		
	 iau.sub i3 i3 0x10				||	lsu0.ldo.64.l v4, i4 0 		||	lsu1.ldo.64.h v4, i4 8 		
	 
	 cmu.cpvi.x16 i9.l v20.0		|| 	lsu0.ldo.64.l v3,  i3 0 	||	lsu1.ldo.64.h v3,  i3 8 	|| iau.incs i0 0x10		
	 cmu.cpivr.x16 v21 i9 			||	lsu0.ldo.64.l v7,  i0 0 	||	lsu1.ldo.64.h v7,  i0 8 	|| iau.incs i6 0x10	
	 cmu.cpvi.x16 i9.l v20.1 		|| 	lsu0.ldo.64.l v13, i6 0 	||	lsu1.ldo.64.h v13, i6 8 	|| iau.incs i1 0x10	
	 cmu.cpivr.x16 v22 i9 	 		||	lsu0.ldo.64.l v8,  i1 0 	||	lsu1.ldo.64.h v8,  i1 8 	|| iau.incs i5 0x10
	 cmu.cpvi.x16 i9.l v20.2 		|| 	lsu0.ldo.64.l v12, i5 0 	||	lsu1.ldo.64.h v12, i5 8 	|| iau.incs i2 0x10
	 cmu.cpivr.x16 v23 i9 	 		||	lsu0.ldo.64.l v9,  i2 0 	||	lsu1.ldo.64.h v9,  i2 8 	|| iau.incs i4 0x10
	 cmu.cpvi.x16 i9.l v20.3 		|| 	lsu0.ldo.64.l v11, i4 0 	||	lsu1.ldo.64.h v11, i4 8 	|| iau.incs i3 0x10 
	 cmu.cpivr.x16 v24 i9 	 		||	lsu0.ldo.64.l v10, i3 0 	||	lsu1.ldo.64.h v10, i3 8 	|| iau.incs i0 0x10	                 
	 	
	 vau.add.f16 v0 v0 v6 			|| iau.incs i6 0x10  
	 vau.add.f16 v1 v1 v5 			|| iau.incs i1 0x10 
	 vau.add.f16 v2 v2 v4  			|| iau.incs i5 0x10
	 vau.mul.f16 v6 v0 v21 			|| iau.incs i2 0x10
	 vau.mul.f16 v5 v1 v22			|| iau.incs i4 0x10
	 vau.mul.f16 v4 v2 v23			|| iau.incs i3 0x10 
	 vau.mul.f16 v3 v3 v24 
	 nop 
	 vau.add.f16 v0 v6 v5
	 vau.add.f16 v3 v3 v4			||	lsu0.ldo.64.l v6, i6 0 		||	lsu1.ldo.64.h v6, i6 8 		|| iau.incs i6 0x10	 
	 vau.add.f16 v7 v7 v13   
	 vau.add.f16 v8 v8 v12   		||	lsu0.ldo.64.l v0, i0 0 		||	lsu1.ldo.64.h v0, i0 8 		|| iau.incs i0 0x10	 
	 vau.add.f16 v25 v0 v3 			
	 vau.add.f16 v9 v9 v11   	
	 vau.mul.f16 v13 v7 v21  		||	lsu0.ldo.64.l v1, i1 0 		||	lsu1.ldo.64.h v1, i1 8 		|| iau.incs i1 0x10
	 vau.mul.f16 v12 v8 v22  		||	lsu0.ldo.64.l v5, i5 0 		||	lsu1.ldo.64.h v5, i5 8		|| iau.incs i5 0x10
	 vau.mul.f16 v11 v9 v23 		||	lsu0.ldo.64.l v2, i2 0 		||	lsu1.ldo.64.h v2, i2 8 		|| iau.incs i2 0x10
	 vau.mul.f16 v10 v10 v24 		||	lsu0.ldo.64.l v4, i4 0 		||	lsu1.ldo.64.h v4, i4 8 		|| iau.incs i4 0x10                    
	 vau.add.f16 v0 v0 v6			||	lsu0.ldo.64.l v3, i3 0 		||	lsu1.ldo.64.h v3, i3 8 		|| iau.incs i3 0x10
	 vau.add.f16 v7 v13 v12  
	 vau.add.f16 v10 v10 v11 
	 nop 2                   
	 vau.add.f16 v26 v7 v10  		
	 vau.add.f16 v1 v1 v5 			||	lsu0.ldo.64.l v0, i0 0 		||	lsu1.ldo.64.h v0, i0 8 		||  iau.incs i0 0x10
     vau.add.f16 v2 v2 v4 			||	lsu0.ldo.64.l v6, i6 0 		||	lsu1.ldo.64.h v6, i6 8 		||  iau.incs i6 0x10
	 .nowarn 12
	 vau.mul.f16 v29 v0 v21 		||  cmu.alignvec v14 v25 v26 10 ||	lsu0.ldo.64.l v1, i1 0 		||	lsu1.ldo.64.h v1, i1 8 	|| iau.incs i1 0x10
	 vau.mul.f16 v30 v1 v22 		||  cmu.alignvec v15 v25 v26 12 ||	lsu0.ldo.64.l v5, i5 0 		||	lsu1.ldo.64.h v5, i5 8 	|| iau.incs i5 0x10 
	 vau.mul.f16 v11 v2 v23  		||  cmu.alignvec v16 v25 v26 14 ||	lsu0.ldo.64.l v2, i2 0 		||	lsu1.ldo.64.h v2, i2 8 	|| iau.incs i2 0x10
	 vau.mul.f16 v31 v3 v24    		||	lsu0.ldo.64.l v4, i4 0 		||	lsu1.ldo.64.h v4, i4 8 		||  iau.incs i4 0x10
	 nop 
	 vau.add.f16 v28 v29 v30 		||	lsu0.ldo.64.l v3, i3 0 		||	lsu1.ldo.64.h v3, i3 8 		|| iau.incs i3 0x10
	 vau.add.f16 v31 v31 v11
	 nop 2 
	 vau.add.f16 v27 v28 v31 	
	 vau.add.f16 v0 v0 v6 
     vau.add.f16 v1 v1 v5	 
	 vau.mul.f16 v12 v26 v24		||  cmu.alignvec v17 v26 v27 6   
	 vau.add.f16 v8 v14 v17  		||  cmu.alignvec v18 v26 v27 4   
	 vau.add.f16 v9 v15 v18  		||  cmu.alignvec v19 v26 v27 2    
	.lalign
	 vau.add.f16 v2 v2 v4  || bru.rpl i7 i10// 
	 vau.mul.f16 v29 v0 v21 
	 vau.mul.f16 v30 v1 v22 		||	lsu0.ldo.64.l v0, i0 0 		||	lsu1.ldo.64.h v0, i0 8 		|| iau.incs i0 0x10
	 vau.mul.f16 v11 v2 v23 		||	lsu0.ldo.64.l v6, i6 0 		||	lsu1.ldo.64.h v6, i6 8 		|| iau.incs i6 0x10
	 vau.mul.f16 v31 v3 v24 		||	lsu0.ldo.64.l v1, i1 0 		||	lsu1.ldo.64.h v1, i1 8 		|| iau.incs i1 0x10
	 vau.add.f16 v28 v29 v30		||	lsu0.ldo.64.l v5, i5 0 		||	lsu1.ldo.64.h v5, i5 8 		|| iau.incs i5 0x10    
	 vau.add.f16 v10 v16 v19 		||  cmu.cpvv v25 v26            ||	lsu0.ldo.64.l v2, i2 0 		|| lsu1.ldo.64.h v2, i2 8  || iau.incs i2 0x10         
	 vau.add.f16 v31 v31 v11        ||	lsu0.ldo.64.l v4, i4 0 		||	lsu1.ldo.64.h v4, i4 8 		|| iau.incs i4 0x10  
	 vau.mul.f16 v8 v8 v21   		||  cmu.cpvv v26 v27            ||	lsu0.ldo.64.l v3, i3 0 		|| lsu1.ldo.64.h v3, i3 8  || iau.incs i3 0x10 	         
	 vau.mul.f16 v9 v9 v22   		||  cmu.alignvec v14 v25 v26 10 
	 .nowarnend
	 vau.mul.f16 v10 v10 v23 		||  cmu.alignvec v15 v25 v26 12 
	 vau.add.f16 v27 v28 v31
	 vau.add.f16 v7 v12 v8    		||  cmu.alignvec v16 v25 v26 14 
 convSeparable7x7Fp16ToFp16_loop:
	 vau.add.f16 v9 v9 v10 
	 vau.mul.f16 v12 v26 v24  		||  cmu.alignvec v17 v26 v27 6  
	 vau.add.f16 v8 v14 v17  		||  cmu.alignvec v18 v26 v27 4   
	 vau.add.f16 v7 v7 v9  
	 vau.add.f16 v9 v15 v18  		||  cmu.alignvec v19 v26 v27 2
	 vau.add.f16 v0 v0 v6 
	 vau.add.f16 v1 v1 v5 			||	lsu1.sto.64.l v7, i18 0		||	lsu0.sto.64.h v7, i18 8		|| iau.incs i18 0x10 
	
	 LSU0.LDo.64.H  v31  i19 8 		||	LSU1.LDo.64.L  v31  i19 0	|| 	IAU.ADD i19 i19 0x10
	 LSU0.LDo.64.H  v30  i19 8 		||	LSU1.LDo.64.L  v30  i19 0	|| 	IAU.ADD i19 i19 0x10
	 LSU0.LDo.64.H  v29  i19 8 		||	LSU1.LDo.64.L  v29  i19 0	|| 	IAU.ADD i19 i19 0x10
	 LSU0.LDo.64.H  v28  i19 8 		||	LSU1.LDo.64.L  v28  i19 0	|| 	IAU.ADD i19 i19 0x10
	 LSU0.LDo.64.H  v27  i19 8 		||	LSU1.LDo.64.L  v27  i19 0	|| 	IAU.ADD i19 i19 0x10
	 LSU0.LDo.64.H  v26  i19 8		||	LSU1.LDo.64.L  v26  i19 0	|| 	IAU.ADD i19 i19 0x10
	 LSU0.LDo.64.H  v25  i19 8		||	LSU1.LDo.64.L  v25  i19 0	||	IAU.ADD i19 i19 0x10
	 LSU0.LDo.64.H  v24  i19 8		|| 	LSU1.LDo.64.L  v24  i19 0	|| 	IAU.ADD i19 i19 0x10
		
	BRU.jmp i30
	nop 6
.size mvcvConvSeparable7x7Fp16ToFp16_asm,.-mvcvConvSeparable7x7Fp16ToFp16_asm
.end
 
 
