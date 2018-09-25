.type mvcvConvolution1x5Fp16ToFp16_asm,@function
.version 0.50

.code .text.Convolution1x5Fp16ToFp16
// CoreConvolution1x5:
//void Convolution1x5Fp16ToFp16_asm(u8** in(i18), u8** out(i17), half conv[5](i16), u32 inWidth(i15))
//internal computation are made on fp16, output result is fp16
.lalign
mvcvConvolution1x5Fp16ToFp16_asm:
.nowarn
	lsu0.ld.32 i0  i18
	lsu0.ld.32 i17 i17	
	lsu0.ldil i3 convolution1x5Fp16ToFp16_loopConv1x5Fp16ToFp16	|| lsu1.ldih i3 convolution1x5Fp16ToFp16_loopConv1x5Fp16ToFp16
	lsu0.ldil i1 0x10 	
	lsu0.ldxv v9  i16
	iau.shr.u32 i4 i15 3
	nop
	iau.incs i0 -16		
	lsu0.ldxv v0 i0	|| iau.incs i0 0x10
	lsu1.ldxvi v1 i0 i1	
	nop 6
	
	lsu0.ldxvi v1 i0 i1
	cmu.alignvec v2 v0, v1 12 	
	cmu.alignvec v3 v0, v1 14 
	cmu.cpvv v0 v1	
	nop 4
	cmu.alignvec v4 v0, v1 2
	cmu.alignvec v5 v0, v1 4
	
	vau.mul.f16 v6   v9 v2	|| lsu1.swzv8 [00000000] 	|| cmu.alignvec v2 v0, v1 12	|| lsu0.ldxvi v1 i0 i1
	vau.mul.f16 v7 	 v9 v3	|| lsu1.swzv8 [11111111]	
	vau.mul.f16 v8   v9 v0	|| lsu1.swzv8 [22222222]	 
	vau.mul.f16 v10  v9 v4	|| lsu1.swzv8 [33333333]	|| cmu.alignvec v3 v0, v1 14 
	vau.mul.f16 v11  v9 v5	|| lsu1.swzv8 [44444444]	
	vau.add.f16 v12 v6 v7	|| cmu.cpvv v0 v1			
	nop 
	vau.add.f16 v13 v8 v10  || cmu.alignvec v4 v0, v1 2
	vau.mul.f16 v6  v9 v2	|| lsu1.swzv8 [00000000]  	
	vau.add.f16 v14 v12 v11	
	vau.mul.f16 v7 	 v9 v3	|| lsu1.swzv8 [11111111]	|| cmu.alignvec v5 v0, v1 4
	vau.mul.f16 v8   v9 v0	|| lsu1.swzv8 [22222222] 	 
	vau.add.f16 v15 v14 v13	|| cmu.alignvec v2 v0, v1 12 || lsu0.ldo.64.l v1 i0 0 || lsu1.ldo.64.h v1 i0 8 || IAU.ADD i0 i0 i1	
	vau.mul.f16 v10  v9	v4  || lsu1.swzv8 [33333333]	|| cmu.alignvec v3 v0, v1 14 
	vau.mul.f16 v11  v9	v5  || lsu1.swzv8 [44444444]	
	nop 3
	.lalign
	vau.add.f16 v12 v6 v7	|| cmu.cpvv v0 v1			|| bru.rpl i3 i4 convolution1x5Fp16ToFp16_end_loopConv1x5Fp16ToFp16
	vau.add.f16 v13 v8 v10  || cmu.alignvec v4 v0, v1 2	|| lsu1.stxvi v15 i17 i1
 convolution1x5Fp16ToFp16_loopConv1x5Fp16ToFp16:
	vau.mul.f16 v6  v9 v2	|| lsu0.swzv8 [00000000]  	
	vau.add.f16 v14 v12 v11	|| lsu1.ldxvi v1 i0 i1	
	vau.mul.f16 v7 	 v9 v3	|| lsu0.swzv8 [11111111]	|| cmu.alignvec v5 v0, v1 4
	vau.mul.f16 v8   v9 v0	|| lsu1.swzv8 [22222222] 	 
	vau.add.f16 v15 v14 v13	|| cmu.alignvec v2 v0, v1 12	
	vau.mul.f16 v10  v9	v4  || lsu1.swzv8 [33333333]	|| cmu.alignvec v3 v0, v1 14 
	vau.mul.f16 v11  v9	v5  || lsu1.swzv8 [44444444]		
 convolution1x5Fp16ToFp16_end_loopConv1x5Fp16ToFp16:
	
	
	bru.jmp i30
	nop 6

.size mvcvConvolution1x5Fp16ToFp16_asm,.-mvcvConvolution1x5Fp16ToFp16_asm
.nowarnend
.end
