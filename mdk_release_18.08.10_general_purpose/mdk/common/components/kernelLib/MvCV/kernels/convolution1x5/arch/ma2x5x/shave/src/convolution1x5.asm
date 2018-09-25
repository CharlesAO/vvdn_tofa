///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvConvolution1x5_asm,@function
.version 00.70.01

.code .text.Convolution1x5_asm
.align 16

//void Convolution1x5_asm(u8 **in(i18), u8 **out(i17), half* filter(i16), u32 inWidth(i15))
mvcvConvolution1x5_asm:
.nowarn
	lsu0.ldi.32 i0 i18				|| lsu1.ldi.16r v0 i16
	lsu0.ldil i13 convolution1x5_loopConv1x5	|| lsu1.ldih i13 convolution1x5_loopConv1x5
	lsu0.ldi.16r v1 i16				|| iau.shr.u32 i14 i15 3
	lsu0.ld.32 i17 i17 
	lsu0.ldi.16r v2 i16
	nop 
	lsu0.ldi.16r v3 i16				 
	iau.incs i0 -8
	lsu0.ldi.128.u8f.f16 v5 i0			
	lsu0.ldi.16r v4 i16
	lsu0.ldi.128.u8f.f16 v6 i0
	nop 2
	lsu0.ldi.128.u8f.f16 v6 i0
	nop 3
	cmu.alignvec v7  v5 v6 12	|| vau.mul.f16 v11 v6  v2
	cmu.alignvec v8  v5 v6 14	|| vau.mul.f16 v12 v7  v0
	cmu.cpvv v5 v6				|| vau.mul.f16 v13 v8  v1
	cmu.alignvec v9  v5 v6 2
	cmu.alignvec v10 v5 v6 4	|| vau.mul.f16 v14 v9  v3	|| lsu0.ldi.128.u8f.f16 v6 i0
								   vau.mul.f16 v15 v10 v4	
	
	vau.add.f16 v16 v11 v12	
	vau.add.f16 v17 v13 v14	
	vau.mul.f16 v11 v6  v2	|| cmu.alignvec v7  v5 v6 12
	vau.add.f16 v18 v16 v15	|| cmu.alignvec v8  v5 v6 14
	vau.mul.f16 v12 v7  v0	|| cmu.cpvv v5 v6
	vau.mul.f16 v13 v8  v1	|| cmu.alignvec v9  v5 v6 2
	vau.add.f16 v19 v17 v18	|| cmu.alignvec v10 v5 v6 4
	vau.mul.f16 v14 v9  v3	|| lsu1.ldi.128.u8f.f16 v6 i0
	vau.mul.f16 v15 v10 v4
	
	.lalign
	vau.add.f16 v16 v11 v12	|| bru.rpl i13 i14 convolution1x5_loopEndConv1x5
	vau.add.f16 v17 v13 v14	|| lsu0.sti.128.f16.u8f v19 i17 || cmu.alignvec v7  v5 v6 12
 convolution1x5_loopConv1x5:
	vau.mul.f16 v11 v6  v2	|| cmu.alignvec v8  v5 v6 14
	vau.add.f16 v18 v16 v15	|| cmu.cpvv v5 v6
	vau.mul.f16 v12 v7  v0	
	vau.mul.f16 v13 v8  v1	|| cmu.alignvec v9  v5 v6 2
	vau.add.f16 v19 v17 v18	|| cmu.alignvec v10 v5 v6 4
	vau.mul.f16 v14 v9  v3	|| lsu1.ldi.128.u8f.f16 v6 i0
	vau.mul.f16 v15 v10 v4
 convolution1x5_loopEndConv1x5:
	
	bru.jmp i30
	nop
	nop 5
	
.size mvcvConvolution1x5_asm,.-mvcvConvolution1x5_asm
.nowarnend
.end
