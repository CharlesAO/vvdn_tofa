///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief apply a convolution 3x3 Separable Fp16ToFp16 filter, in the case of symmetric kernel.
///

.type mvcvConvSeparable3x3Fp16ToFp16_asm,@function
.version 00.51.04
.nowarn
.code .text.convSeparable3x3Fp16ToFp16

//void convSeparable3x3Fp16ToFp16_asm(half* out(i18), half** in(i17), half conv[2](i16), u32 inWidth(i15))
.lalign
mvcvConvSeparable3x3Fp16ToFp16_asm:
    lsu0.ldo.64  i1,  i17, 0x00			|| iau.shr.u32 i15, i15, 3		|| lsu1.ldil i10 0x08 
    lsu0.ld.64.l  v20, i16 				|| lsu1.ldo.32  i3  i17, 0x08
    nop 
    lsu0.ldil i12, ___end_loop_convSeparable3x3Fp16ToFp16_asm  ||  lsu1.ldih i12, ___end_loop_convSeparable3x3Fp16ToFp16_asm
    nop 3
    iau.sub i1, i1, 0x10					
    cmu.cpvi.x16 i0.l, v20.0	||		iau.sub i3, i3, 0x10 		||	lsu0.ldo.64.l v1, i1 0		||	lsu1.ldo.64.h v1, i1 8			
    cmu.cpivr.x16 v21, i0 		||		iau.sub i2, i2, 0x10 
    cmu.cpvi.x16 i0.l, v20.1	||		lsu0.ldo.64.l v3, i3 0		||	lsu1.ldo.64.h v3, i3 8		||	iau.incs i3 0x10
    cmu.cpivr.x16 v22, i0   	||		lsu0.ldo.64.l v2, i2 0		||	lsu1.ldo.64.h v2, i2 8 		||	iau.incs i1 0x10  
      	 
    lsu0.ldo.64.l v1, i1 0		||		lsu1.ldo.64.h v1, i1 8		||	iau.incs i2 0x10
    lsu0.ldo.64.l v3, i3 0		||		lsu1.ldo.64.h v3, i3 8		||	iau.incs i1 0x10
    lsu0.ldo.64.l v2, i2 0		||		lsu1.ldo.64.h v2, i2 8 		||	iau.incs i3 0x10
    lsu0.ldo.64.l v1, i1 0		||		lsu1.ldo.64.h v1, i1 8		||	iau.incs i2 0x10 
    lsu0.ldo.64.l v3, i3 0		||		lsu1.ldo.64.h v3, i3 8		||	iau.incs i1 0x10
    lsu0.ldo.64.l v2, i2 0		||		lsu1.ldo.64.h v2, i2 8 		||	iau.incs i3 0x10
    
    vau.add.f16 v4, v1  v3		||		iau.incs i2 0x10
    vau.mul.f16 v5, v22 v2				
	vau.add.f16 v4, v1  v3   	||		lsu0.ldo.64.l v1, i1 0		||	lsu1.ldo.64.h v1, i1 8		||	iau.incs i1  0x10	
    vau.mul.f16 v6, v21 v4		||		lsu0.ldo.64.l v3, i3 0		||	lsu1.ldo.64.h v3, i3 8		||	iau.incs i3 0x10		
    vau.mul.f16 v5, v22 v2		||		lsu0.ldo.64.l v2, i2 0		||	lsu1.ldo.64.h v2, i2 8 		||	iau.incs i2 0x10	
    vau.mul.f16 v6, v21 v4 
    vau.add.f16 v7, v5  v6		||		lsu0.ldo.64.l v1, i1 0		||	lsu1.ldo.64.h v1, i1 8		||	iau.incs i1 0x10
    vau.add.f16 v4, v1  v3		||		lsu0.ldo.64.l v3, i3 0		||	lsu1.ldo.64.h v3, i3 8		||	iau.incs i3 0x10
    vau.add.f16 v8, v5  v6		||		lsu0.ldo.64.l v2, i2 0		||	lsu1.ldo.64.h v2, i2 8 		||	iau.incs i2 0x10
    vau.mul.f16 v5, v22 v2		
   	vau.mul.f16 v6, v21 v4
    vau.add.f16 v4, v1  v3		||		lsu0.ldo.64.l v1, i1 0		||	lsu1.ldo.64.h v1, i1 8		||	iau.incs i1 0x10
    vau.mul.f16 v5, v22 v2		||		lsu0.ldo.64.l v3, i3 0		||	lsu1.ldo.64.h v3, i3 8		||	iau.incs i3 0x10
    vau.add.f16 v9, v5  v6		||		lsu0.ldo.64.l v2, i2 0		||	lsu1.ldo.64.h v2, i2 8 		||	iau.incs i2 0x10
    vau.mul.f16 v6, v21 v4		
  
    vau.add.f16 v4,  v1  v3     ||		cmu.alignvec v10, v7, v8, 14
    vau.mul.f16 v5,  v22 v2     ||		cmu.alignvec v12, v8, v9, 2
    vau.add.f16 v13, v10 v12    ||		cmu.cpvv v11, v8
    vau.add.f16 v9,  v5  v6     ||		cmu.cpvv v7, v8
    vau.mul.f16 v6,  v21  v4   	||		cmu.cpvv v8, v9
    vau.mul.f16 v14, v22 v11	
    vau.mul.f16 v15, v21 v13	
   
.lalign    
    vau.add.f16 v4, v1 v3		|| 		lsu0.ldo.64.l v1, i1 0		||	lsu1.ldo.64.h v1, i1 8		||	iau.incs i1 0x10 		||	cmu.alignvec v10, v7, v8, 14   	||		bru.rpl i12, i15, ___delay_slot_convSeparable3x3Fp16ToFp16_asm
___end_loop_convSeparable3x3Fp16ToFp16_asm:

	vau.mul.f16 v5,  v22 v2    	||		cmu.alignvec v12, v8, v9, 2	||	lsu0.ldo.64.l v3, i3 0		||	lsu1.ldo.64.h v3, i3 8	||	iau.incs i3 0x10				

	vau.add.f16 v13, v10 v12	||		cmu.cpvv v11, v8			||	lsu0.ldo.64.l v2, i2 0		||	lsu1.ldo.64.h v2, i2 8 	||	iau.incs i2 0x10			      				 
    vau.add.f16 v9,  v5  v6     ||		cmu.cpvv v7,  v8		
   	vau.add.f16 v16, v14 v15 	||		cmu.cpvv v8,  v9 			   			
    vau.mul.f16 v6,  v21 v4		 			                          
    vau.mul.f16 v14, v22 v11	
    vau.mul.f16 v15, v21 v13 	||		lsu1.sto.64.l v16, i18 0	||	lsu0.sto.64.h v16, i18 8	||	iau.incs i18 0x10    
___delay_slot_convSeparable3x3Fp16ToFp16_asm:

	bru.jmp i30
	nop 6
.nowarnend
.size mvcvConvSeparable3x3Fp16ToFp16_asm,.-mvcvConvSeparable3x3Fp16ToFp16_asm
.end
 
