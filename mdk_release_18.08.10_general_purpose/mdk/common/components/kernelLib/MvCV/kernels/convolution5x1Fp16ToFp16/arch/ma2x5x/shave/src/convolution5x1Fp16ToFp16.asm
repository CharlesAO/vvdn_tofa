///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvConvolution5x1Fp16ToFp16_asm,@function
.version 00.50.00

.code .text.Convolution5x1Fp16ToFp16_asm

//void Convolution5x1Fp16ToFp16_asm(u8 **in(i18), u8 **out(i17), half* filter(i16), u32 inWidth(i15))
.lalign
mvcvConvolution5x1Fp16ToFp16_asm:
.nowarn
	LSU0.LDI.64 i1 i18
	lsu0.ldil i13 convolution5x1Fp16ToFp16_loopConv5x1Fp16ToFp16	|| lsu1.ldih i13 convolution5x1Fp16ToFp16_loopConv5x1Fp16ToFp16
	lsu0.ldil i12 0x10 				
	LSU0.LDI.64 i3 i18
	LSU0.LDI.32 i5 i18
	lsu0.ldxv v0 i16
	iau.shr.u32 i14 i15 3	
	LSU1.LD.32 i17 i17 		
	
	lsu1.ldxvi v1 i1 i12
	lsu0.ldxvi v2 i2 i12   
	lsu1.ldxvi v3 i3 i12   
	lsu0.ldxvi v4 i4 i12  
	lsu1.ldxvi v5 i5 i12 
	nop 2
	cmu.cpvi.x16  i8.l, v0.0	
	cmu.cpivr.x16 v18, i8		|| lsu0.ldxvi v1 i1 i12
	cmu.cpvi.x16  i8.l, v0.1	|| vau.mul.f16 v11 v1 v18
	cmu.cpivr.x16 v19, i8		|| lsu0.ldxvi v2 i2 i12
	cmu.cpvi.x16  i8.l, v0.2	|| vau.mul.f16 v12 v2 v19
	cmu.cpivr.x16 v20, i8		|| lsu0.ldxvi v3 i3 i12
	cmu.cpvi.x16  i8.l, v0.3	|| vau.mul.f16 v13 v3 v20
	cmu.cpivr.x16 v21, i8		|| lsu0.ldxvi v4 i4 i12 
	cmu.cpvi.x16  i8.l, v0.4	|| vau.mul.f16 v14 v4 v21
	cmu.cpivr.x16 v22, i8		|| lsu0.ldxvi v5 i5 i12
	vau.mul.f16 v15 v5 v22
	
	
	vau.add.f16 v6 v11 v12	
	vau.add.f16 v7 v13 v14
	vau.mul.f16 v11 v1 v18	|| lsu1.ldxvi v1 i1 i12
	vau.add.f16 v8 v15 v6
	vau.mul.f16 v12 v2 v19	|| lsu0.ldxvi v2 i2 i12
	vau.mul.f16 v13 v3 v20	|| lsu1.ldxvi v3 i3 i12
	vau.add.f16 v9 v8 v7
	vau.mul.f16 v14 v4 v21	|| lsu0.ldxvi v4 i4 i12 
	vau.mul.f16 v15 v5 v22	|| lsu1.ldxvi v5 i5 i12
	
	
	.lalign
	vau.add.f16 v6 v11 v12	|| bru.rpl i13 i14 convolution5x1Fp16ToFp16_end_loopConv5x1Fp16ToFp16
	vau.add.f16 v7 v13 v14	|| lsu0.stxvi v9 i17 i12
 convolution5x1Fp16ToFp16_loopConv5x1Fp16ToFp16:
	vau.mul.f16 v11 v1 v18	|| lsu1.ldxvi v1 i1 i12	
	vau.add.f16 v8 v15 v6
	vau.mul.f16 v12 v2 v19	|| lsu0.ldxvi v2 i2 i12
	vau.mul.f16 v13 v3 v20	|| lsu1.ldxvi v3 i3 i12
	vau.add.f16 v9 v8 v7
	vau.mul.f16 v14 v4 v21	|| lsu0.ldxvi v4 i4 i12 
	vau.mul.f16 v15 v5 v22	|| lsu1.ldxvi v5 i5 i12
 convolution5x1Fp16ToFp16_end_loopConv5x1Fp16ToFp16:

    BRU.JMP i30   

	NOP 6
	
.size mvcvConvolution5x1Fp16ToFp16_asm,.-mvcvConvolution5x1Fp16ToFp16_asm
.nowarnend
.end
