///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvConvolution7x1Fp16ToFp16_asm,@function
.version 00.50.00

.code .text.convolution7x1Fp16ToFp16_asm

//void convolution7x1Fp16ToFp16_asm(u8 **in(i18), u8 *out(i17), half* filter(i16), u32 inWidth(i15))
.lalign
mvcvConvolution7x1Fp16ToFp16_asm:

lsu0.ld.64 i1 i18						|| 	lsu1.ldi.16r v17 i16 
lsu0.ldil i13 convolution7x1Fp16ToFp16_loopConv7x1Fp16ToFp16	|| 	lsu1.ldih i13 convolution7x1Fp16ToFp16_loopConv7x1Fp16ToFp16
lsu0.ldil i12 0x10 						|| 	lsu1.ldi.16r v18 i16
lsu0.ldo.64 i3 i18 0x08						
lsu0.ldo.64 i5 i18 0x10					|| 	lsu1.ldi.16r v19 i16	
lsu0.ldo.32 i7 i18 0x18	
lsu1.ldi.16r v20 i16
nop
lsu1.ldi.16r v21 i16
nop
lsu0.ldxvi v1 i1 i12					|| lsu1.ldi.16r v22 i16		
lsu1.ldxvi v2 i2 i12   					|| iau.shr.u32 i14 i15 3		
lsu0.ldxvi v3 i3 i12   						
lsu1.ldxvi v4 i4 i12  
lsu0.ldxvi v5 i5 i12  
lsu1.ldxvi v6 i6 i12  
lsu0.ldxvi v7 i7 i12  
lsu1.ldi.16r v23 i16


vau.macpz.f16 v1 v17	|| lsu1.ldxvi v1 i1 i12
vau.macp.f16 v2 v18		|| lsu0.ldxvi v2 i2 i12
vau.macp.f16 v3 v19		|| lsu1.ldxvi v3 i3 i12
vau.macp.f16 v4 v20		|| lsu0.ldxvi v4 i4 i12
vau.macp.f16 v5 v21		|| lsu1.ldxvi v5 i5 i12
vau.macp.f16 v6 v22		|| lsu0.ldxvi v6 i6 i12  
vau.macpw.f16 v8 v7 v23	|| lsu1.ldxvi v7 i7 i12    

iau.sub i11 i14 1
peu.pc1i lte || bru.bra convolution7x1Fp16ToFp16_endConv7x1Fp16ToFp16
nop 6


vau.macpz.f16 	v1 v17		|| bru.rpl i13 i11
vau.macp.f16 	v2 v18	 
vau.macp.f16 	v3 v19	
vau.macp.f16 	v4 v20		 
vau.macp.f16 	v5 v21		 
vau.macp.f16 	v6 v22
vau.macpw.f16 	v8 v7 v23 || lsu0.stxvi v8 i17 i12
lsu1.ldxvi v1 i1 i12
 convolution7x1Fp16ToFp16_endConv7x1Fp16ToFp16:
 convolution7x1Fp16ToFp16_loopConv7x1Fp16ToFp16:
 .nowarn 10
lsu0.ldxvi v2 i2 i12
.nowarnend
lsu1.ldxvi v3 i3 i12
lsu0.ldxvi v4 i4 i12
lsu1.ldxvi v5 i5 i12	
lsu0.ldxvi v6 i6 i12  
lsu1.ldxvi v7 i7 i12
nop 	

bru.jmp i30  	
lsu0.stxv v8 i17 
nop 5
.size mvcvConvolution7x1Fp16ToFp16_asm,.-mvcvConvolution7x1Fp16ToFp16_asm
.end
