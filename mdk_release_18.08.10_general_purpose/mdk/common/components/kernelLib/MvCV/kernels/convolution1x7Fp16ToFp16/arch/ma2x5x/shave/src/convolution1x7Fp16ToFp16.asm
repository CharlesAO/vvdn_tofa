.type mvcvConvolution1x7Fp16ToFp16_asm,@function
.version 0.50.02

.code .text.Convolution1x7Fp16ToFp16
// CoreConvolution1x7:
//void Convolution1x7Fp16ToFp16_asm(half* in(i18), half* out(i17), half conv[7](i16), u32 inWidth(i15))
.lalign
mvcvConvolution1x7Fp16ToFp16_asm:
.nowarn
	iau.incs i18 -16		|| lsu0.ldi.16r v17 i16	|| lsu1.ldil i10 0x10
	lsu0.ldxvi v0 i18 i10		
	lsu1.ld.16r v18 i16		|| iau.incs i16 0x02
	lsu1.ld.16r v19 i16		|| iau.incs i16 0x02	
	lsu0.ldxvi v1 i18 i10	
	lsu1.ld.16r v20 i16		|| iau.incs i16 0x02
	lsu1.ld.16r v21 i16		|| iau.incs i16 0x02
	lsu1.ld.16r v22 i16		|| iau.incs i16 0x02	
	lsu1.ldil i13 convolution1x7Fp16ToFp16_loopConv1x7Fp16ToFp16
	lsu1.ldih i13 convolution1x7Fp16ToFp16_loopConv1x7Fp16ToFp16
	lsu0.ldxvi v1 i18 i10
	lsu1.ld.16r v23 i16		|| iau.shr.u32 i14 i15 3
	
	
  cmu.alignvec v5 v0 v1 10  || vau.mul.f16 v10 v1 v20
  cmu.alignvec v6 v0 v1 12  || vau.mul.f16 v11 v5 v17
  cmu.alignvec v7 v0 v1 14  || vau.mul.f16 v12 v6 v18
  cmu.cpvv v0 v1	             || vau.mul.f16 v13 v7 v19
  vau.add.f16 v14 v10 v11    || lsu0.ldxvi v16 i18 i10
  cmu.alignvec v2 v0 v1 2   
  cmu.alignvec v3 v0 v1 4	  || vau.add.f16 v15 v12 v13
  cmu.alignvec v4 v0 v1 6   || vau.mul.f16 v10 v2 v21
 
  vau.mul.f16 v11 v3 v22    || cmu.alignvec v5 v0 v1 10
  vau.mul.f16 v12 v4 v23    || cmu.alignvec v6 v0 v1 12
  vau.add.f16 v14 v14 v15   || cmu.alignvec v7 v0 v1 14
  vau.add.f16 v9 v10 v11    || cmu.cpvv v0 v1	          
  vau.mul.f16 v10 v1 v20 
  vau.add.f16 v14 v14 v12   || cmu.cpvv v1 v16
  vau.mul.f16 v11 v5 v17 
  vau.mul.f16 v12 v6 v18 
  vau.add.f16 v8 v14 v9
 
  
  vau.mul.f16 v13 v7 v19   || lsu0.ldxvi v16 i18 i10 || bru.rpl i13 i14 convolution1x7Fp16ToFp16_endConv1x7Fp16ToFp16
  vau.add.f16 v14 v10 v11  || cmu.alignvec v2 v0 v1 2
  vau.mul.f16 v10 v2 v21   || lsu1.stxvi v8 i17 i10
  cmu.alignvec v3 v0 v1 4	 || vau.add.f16 v15 v12 v13
  cmu.alignvec v4 v0 v1 6  ||  vau.mul.f16 v11 v3 v22
  vau.mul.f16 v12 v4 v23   || cmu.alignvec v5 v0 v1 10
  convolution1x7Fp16ToFp16_loopConv1x7Fp16ToFp16:
  vau.add.f16 v14 v14 v15  || cmu.alignvec v6 v0 v1 12
  vau.add.f16 v9 v10 v11   || cmu.alignvec v7 v0 v1 14
  vau.mul.f16 v10 v1 v20   || cmu.cpvv v0 v1	          
  vau.add.f16 v14 v14 v12  
  vau.mul.f16 v11 v5 v17   || cmu.cpvv v1 v16
  vau.mul.f16 v12 v6 v18 
  vau.add.f16 v8 v14 v9
  convolution1x7Fp16ToFp16_endConv1x7Fp16ToFp16: 

	bru.jmp i30
	nop 6

.size mvcvConvolution1x7Fp16ToFp16_asm,.-mvcvConvolution1x7Fp16ToFp16_asm
.nowarnend
.end
