///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvConvolution5x1_asm,@function
.version 00.70.01

.code .text.Convolution5x1_asm
.align 16

//void Convolution5x1_asm(u8 **in(i18), u8 **out(i17), half* filter(i16), u32 inWidth(i15))
mvcvConvolution5x1_asm:
.nowarn
 
	lsu0.ldi.64 i1 i18				
	lsu0.ldil i11 convolution5x1_loopConv5x1 	|| lsu1.ldih i11 convolution5x1_loopConv5x1
	lsu0.ldi.64 i3 i18			 	|| lsu1.ldi.16r v19 i16 
	lsu0.ldi.32 i5 i18				
	lsu0.ld.32 i17 i17				|| lsu1.ldi.16r v20 i16
	iau.shr.u32 i14 i15 3			
	nop
   
   
	lsu0.ldi.128.u8f.f16 v1 i1	|| lsu1.ldi.16r v21 i16	
	lsu0.ldi.128.u8f.f16 v2 i2	
	lsu0.ldi.128.u8f.f16 v3 i3	|| lsu1.ldi.16r v22 i16
	lsu0.ldi.128.u8f.f16 v4 i4	
	lsu0.ldi.128.u8f.f16 v5 i5	|| lsu1.ldi.16r v23 i16	
	nop 2
	

	vau.mul.f16 v6  v1 v19	|| lsu0.ldi.128.u8f.f16 v1 i1
	vau.mul.f16 v7  v2 v20	|| lsu0.ldi.128.u8f.f16 v2 i2
	vau.mul.f16 v8  v3 v21	|| lsu0.ldi.128.u8f.f16 v3 i3
	vau.mul.f16 v9  v4 v22	|| lsu0.ldi.128.u8f.f16 v4 i4
	vau.mul.f16 v10 v5 v23	|| lsu0.ldi.128.u8f.f16 v5 i5
		
	
	vau.add.f16 v11 v6  v7	
	vau.add.f16 v12 v8  v9

	vau.mul.f16 v6  v1  v19	|| lsu0.ldi.128.u8f.f16 v1 i1
	vau.add.f16 v13 v11 v10
	vau.mul.f16 v7  v2  v20	|| lsu0.ldi.128.u8f.f16 v2 i2
	vau.mul.f16 v8  v3  v21	|| lsu0.ldi.128.u8f.f16 v3 i3
	vau.add.f16 v14 v13 v12
	vau.mul.f16 v9  v4  v22	|| lsu0.ldi.128.u8f.f16 v4 i4
	vau.mul.f16 v10 v5  v23	|| lsu0.ldi.128.u8f.f16 v5 i5
	
	
	.lalign
	vau.add.f16 v11 v6  v7	|| bru.rpl i11 i14 convolution5x1_loopEndConv5x1
	vau.add.f16 v12 v8  v9	|| lsu0.sti.128.f16.u8f v14 i17	
 convolution5x1_loopConv5x1:
	vau.mul.f16 v6  v1  v19	|| lsu0.ldi.128.u8f.f16 v1 i1
	vau.add.f16 v13 v11 v10
	vau.mul.f16 v7  v2  v20	|| lsu0.ldi.128.u8f.f16 v2 i2
	vau.mul.f16 v8  v3  v21	|| lsu0.ldi.128.u8f.f16 v3 i3
	vau.add.f16 v14 v13 v12
	vau.mul.f16 v9  v4  v22	|| lsu0.ldi.128.u8f.f16 v4 i4
	vau.mul.f16 v10 v5  v23	|| lsu0.ldi.128.u8f.f16 v5 i5
 convolution5x1_loopEndConv5x1:
	
	
   //lsu0.sti.128.f16.u8f v14 i17
	
	
    bru.jmp i30
	nop 6
.size mvcvConvolution5x1_asm,.-mvcvConvolution5x1_asm
.nowarnend
.end
