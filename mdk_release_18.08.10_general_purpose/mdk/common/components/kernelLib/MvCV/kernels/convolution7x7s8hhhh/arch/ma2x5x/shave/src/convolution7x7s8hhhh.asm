///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvConvolution7x7s8hhhh_asm,@function
.version 00.51.04

.data .data.convolution7x7s8hhhh
.align 16
 convolution7x7s8hhhh_clampVal:
        .float16 255.0


.code .text.convolution7x7s8hhhh

//void Convolution7x7s8hhhh_asm(u8** in(i18), u8** out(i17), half conv[49](i16), u32 inWidth(i15))
//internal computation are made on fp16, output result is fp16
mvcvConvolution7x7s8hhhh_asm:
iau.xor i9 i9 i9
iau.xor i8 i8 i8
	LSU0.LDIL i10 conv_loopStartConv7x7s8hhhh	|| LSU1.LDIH i10 conv_loopStartConv7x7s8hhhh
    LSU0.LDi.64  i0  i18        ||LSU1.LDO.64.L v14 i16 0x00 ||iau.shr.u32 i15, i15, 0x3
	LSU0.LDi.64  i2  i18        ||LSU1.LDO.64.H v14 i16 0x08 
	LSU0.LDi.64  i4  i18        ||LSU1.LDO.64.L v15 i16 0x10 
	LSU0.LDi.32  i6  i18        ||LSU1.LDO.64.H v15 i16 0x18 
	LSU0.LDO.64.L v16 i16 0x20    ||LSU1.LDO.64.H v16 i16 0x28
	LSU0.LDO.64.L v17 i16 0x30    ||LSU1.LDO.64.H v17 i16 0x38
	LSU0.LDO.64.L v18 i16 0x40    ||LSU1.LDO.64.H v18 i16 0x48
	LSU0.LDO.64.L v19 i16 0x50    ||LSU1.LDO.64.H v19 i16 0x58 ||iau.sub i0, i0, 0x10
	LSU0.LDO.64.L v20 i16 0x60    	                           ||iau.sub i1, i1, 0x10
	iau.sub i2, i2, 0x10 ||LSU0.LDXV v0  i0
	iau.sub i3, i3, 0x10 ||LSU1.LDXV v1  i1	
	iau.sub i4, i4, 0x10 ||LSU0.LDXV v2  i2
	iau.sub i5, i5, 0x10 ||LSU1.LDXV v3  i3
	iau.sub i6, i6, 0x10 ||LSU0.LDXV v4  i4
	LSU1.LDXV v5  i5|| iau.add i0, i0, 0x10
	LSU0.LDXV v6  i6|| iau.add i1, i1, 0x10
	LSU1.LDXV v7  i0|| iau.add i2, i2, 0x10
	LSU0.LDXV v8  i1|| iau.add i3, i3, 0x10
	LSU1.LDXV v9  i2|| iau.add i4, i4, 0x10
	LSU0.LDXV v10 i3|| iau.add i5, i5, 0x10
	LSU1.LDXV v11 i4|| iau.add i6, i6, 0x10
	LSU0.LDXV v12 i5|| iau.add i0, i0, 0x10
	LSU1.LDXV v13 i6	|| iau.add i1, i1, 0x10
	iau.add i2, i2, 0x10
	iau.add i3, i3, 0x10
	iau.add i4, i4, 0x10
	iau.add i5, i5, 0x10
	iau.add i6, i6, 0x10
nop
	LSU1.LDXV v7  i0 || iau.add i0, i0, 0x10
	nop 
	LSU1.LD.32 i17, i17	
                                                       cmu.alignvec v22, v0 , v7, 10 
// start loop													   
															
	VAU.MACPZ.f16 v14,  v22  ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v0 , v7, 12 || bru.rpl i10 i15	
	VAU.MACP.f16  v14,  v22  ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v0 , v7, 14
	VAU.MACP.f16  v14,  v22  ||lsu1.swzv8 [22222222] ||cmu.cpvv v0, v7 
	VAU.MACP.f16  v14,  v0   ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v0 , v7 , 2  ||LSU0.LDXV v8  i1 || iau.add i1, i1, 0x10
	VAU.MACP.f16  v14,  v22  ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v0 , v7 , 4
	VAU.MACP.f16  v14,  v22  ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v0 , v7 , 6
	VAU.MACP.f16  v14,  v22  ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v1 , v8 , 10
	VAU.MACP.f16  v14,  v22  ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v1 , v8 , 12
	VAU.MACP.f16  v15,  v22  ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v1 , v8 , 14
	VAU.MACP.f16  v15,  v22  ||lsu1.swzv8 [11111111] ||cmu.cpvv v1, v8
	VAU.MACP.f16  v15,  v1   ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v1 , v8 , 2   ||LSU0.LDXV v9  i2 || iau.add i2, i2, 0x10
	VAU.MACP.f16  v15,  v22  ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v1 , v8 , 4
	VAU.MACP.f16  v15,  v22  ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v1 , v8 , 6
	VAU.MACP.f16  v15,  v22  ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v2 , v9 , 10 
	VAU.MACP.f16  v15,  v22  ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v2 , v9 , 12
	VAU.MACP.f16  v15,  v22  ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v2 , v9 , 14
	VAU.MACP.f16  v16,  v22  ||lsu1.swzv8 [00000000] ||cmu.cpvv v2 , v9 
	VAU.MACP.f16  v16,  v2   ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v2 , v9 , 2  ||LSU0.LDXV v10  i3 || iau.add i3, i3, 0x10
	VAU.MACP.f16  v16,  v22  ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v2 , v9 , 4
	VAU.MACP.f16  v16,  v22  ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v2 , v9 , 6
	VAU.MACP.f16  v16,  v22  ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v3 , v10, 10 
	VAU.MACP.f16  v16,  v22  ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v3 , v10, 12
	VAU.MACP.f16  v16,  v22  ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v3 , v10, 14
	VAU.MACP.f16  v16,  v22  ||lsu1.swzv8 [77777777] ||cmu.cpvv v3 , v10
	VAU.MACP.f16  v17,  v3   ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v3 , v10, 2 ||LSU0.LDXV v11  i4 || iau.add i4, i4, 0x10
	VAU.MACP.f16  v17,  v22  ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v3 , v10, 4
	VAU.MACP.f16  v17,  v22  ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v3 , v10, 6
	VAU.MACP.f16  v17,  v22  ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v4 , v11, 10 
	VAU.MACP.f16  v17,  v22  ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v4 , v11, 12
	VAU.MACP.f16  v17,  v22  ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v4 , v11, 14
	VAU.MACP.f16  v17,  v22  ||lsu1.swzv8 [66666666] ||cmu.cpvv v4 , v11
	VAU.MACP.f16  v17,  v4   ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v4 , v11, 2 ||LSU0.LDXV v12  i5 || iau.add i5, i5, 0x10
	VAU.MACP.f16  v18,  v22  ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v4 , v11, 4
	VAU.MACP.f16  v18,  v22  ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v4 , v11, 6
	VAU.MACP.f16  v18,  v22  ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v5 , v12, 10 
	VAU.MACP.f16  v18,  v22  ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v5 , v12, 12
	VAU.MACP.f16  v18,  v22  ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v5 , v12, 14
	VAU.MACP.f16  v18,  v22  ||lsu1.swzv8 [55555555] ||cmu.cpvv v5 , v12
	VAU.MACP.f16  v18,  v5   ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v5 , v12, 2 ||LSU0.LDXV v13  i6 || iau.add i6, i6, 0x10
	VAU.MACP.f16  v18,  v22  ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v5 , v12, 4
	VAU.MACP.f16  v19,  v22  ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v5 , v12, 6
	VAU.MACP.f16  v19,  v22  ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v6 , v13, 10 
	VAU.MACP.f16  v19,  v22  ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v6 , v13, 12
	VAU.MACP.f16  v19,  v22  ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v6 , v13, 14
	VAU.MACP.f16  v19,  v22  ||lsu1.swzv8 [44444444] ||cmu.cpvv v6 , v13
	VAU.MACP.f16  v19,  v6   ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v6 , v13, 2 
	VAU.MACP.f16  v19,  v22  ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v6 , v13, 4
	VAU.MACP.f16  v19,  v22  ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v6 , v13, 6
	VAU.MACPW.f16 v23, v20,  v22  ||lsu1.swzv8 [00000000] 	|| cmu.vdilv.x16 v21 v23 v23 v23
	cmu.vdilv.x16 v21 v23 v23 v23
	cmu.vdilv.x16 v21 v23 v23 v23
	conv_loopStartConv7x7s8hhhh:
	cmu.cmii.i32 i8, i9 || lsu0.cp i7 v23.0
	peu.pc1c NEQ || LSU0.ST.16 i7 i17 	|| IAU.ADD i17 i17 2
	iau.add i8 i8 1
	LSU1.LDXV v7  i0 || iau.add i0, i0, 0x10
	nop 
	nop
    cmu.alignvec v22, v0 , v7, 10 
//end loop	
nop
	cmu.vdilv.x16 v21 v23 v23 v23
	cmu.vdilv.x16 v21 v23 v23 v23
	cmu.vdilv.x16 v21 v23 v23 v23
	lsu0.cp i7 v23.0
	LSU0.ST.16 i7 i17 	|| IAU.ADD i17 i17 2
	BRU.JMP i30
	nop 6
.size mvcvConvolution7x7s8hhhh_asm,.-mvcvConvolution7x7s8hhhh_asm
.end
