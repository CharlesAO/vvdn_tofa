///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

//--------------------------------------------------------------------------------------------------------------
.type mvcvCvtColorRGBfp16ToLumaU8_asm,@function
.version 00.50.37.8
.data .rodata.cvtColorRGBfp16ToLumaU8_asm 
.align 16
 cvtColorRGBfp16ToLumaU8_RGBtoLuma_coefs:
		.float16 0.299F16, 0.587F16, 0.114F16	
 cvtColorRGBfp16ToLumaU8_clamp:
		.float16 255.0
		
.code .text.cvtColorRGBfp16ToLumaU8_asm //text
mvcvCvtColorRGBfp16ToLumaU8_asm:
// void cvtColorRGBfp16ToLumaU8(half* inRGB[3], u8* yOut, u32 width)
//                                       i18      i17        i16     
.nowarn
	
	LSU0.LDIL i6 0x000F || LSU1.LDIH i6 0x0
	LSU0.LDIL i11 cvtColorRGBfp16ToLumaU8_RGBtoLuma_coefs || LSU1.LDIH i11 cvtColorRGBfp16ToLumaU8_RGBtoLuma_coefs
	LSU0.LDIL i4 cvtColorRGBfp16ToLumaU8_clamp || LSU1.LDIH i4 cvtColorRGBfp16ToLumaU8_clamp
	LSU0.LDIL i3 cvtColorRGBfp16ToLumaU8_loop  || LSU1.LDIH i3 cvtColorRGBfp16ToLumaU8_loop 
	lsu0.ldil i8 cvtColorRGBfp16ToLumaU8_compensate || lsu1.ldih i8 cvtColorRGBfp16ToLumaU8_compensate
	lsu0.ldil i9 cvtColorRGBfp16ToLumaU8_final || lsu1.ldih i9 cvtColorRGBfp16ToLumaU8_final || iau.and i7 i16 i6
	nop 6
	
	LSU0.LD.32 i0 i18 || iau.add i18 i18 4 
	LSU0.LD.32 i1 i18 || iau.add i18 i18 4
	LSU0.LD.32 i2 i18 || iau.add i18 i18 4
	nop 6
	LSU0.LD.64.L v0, i11 || iau.shr.u32 i5 i16 4 ||	LSU1.LD.16R v9, i4
	nop 6
	
	cmu.cmz.i32 i5
	peu.pc1c eq || bru.jmp i8
	nop 6
	
	lsu0.ldo.64.l v1 i0 0  || lsu1.ldo.64.h v1 i0 8 || iau.add i0 i0 16 
	lsu0.ldo.64.l v2 i1 0  || lsu1.ldo.64.h v2 i1 8 || iau.add i1 i1 16 
	lsu0.ldo.64.l v3 i2 0  || lsu1.ldo.64.h v3 i2 8 || iau.add i2 i2 16
	lsu0.ldo.64.l v9 i0 0  || lsu1.ldo.64.h v9 i0 8 || iau.add i0 i0 16 
	lsu0.ldo.64.l v10 i1 0 || lsu1.ldo.64.h v10 i1 8 || iau.add i1 i1 16 
	lsu0.ldo.64.l v11 i2 0 || lsu1.ldo.64.h v11 i2 8 || iau.add i2 i2 16 
		nop 2
	vau.mul.f16 v4 v0 v1   || lsu0.swzv8 [00000000]  || lsu1.ldo.64.l v1 i0 0 
	vau.mul.f16 v5 v0 v2   || lsu0.swzv8 [11111111]  || lsu1.ldo.64.h v1 i0 8 || iau.add i0 i0 16
	vau.mul.f16 v6 v0 v3   || lsu0.swzv8 [22222222]  || lsu1.ldo.64.l v2 i1 0 
	vau.mul.f16 v12 v0 v9  || lsu0.swzv8 [00000000]  || lsu1.ldo.64.h v2 i1 8 || iau.add i1 i1 16 
	vau.add.f16 v7 v4 v5   || lsu0.ldo.64.l v3 i2 0 || lsu1.ldo.64.h v3 i2 8 || iau.add i2 i2 16 
	vau.mul.f16 v13 v0 v10 || lsu0.swzv8 [11111111]  || lsu1.ldo.64.l v9 i0 0 || bru.rpl i3 i5 _end_delay_loop_cvtColorRGBfp16ToLumaU8//nop
	vau.mul.f16 v14 v0 v11 || lsu0.swzv8 [22222222]  || lsu1.ldo.64.h v9 i0 8 || iau.add i0 i0 16  

	vau.add.f16 v8 v7 v6   || lsu0.ldo.64.l v10 i1 0  || lsu1.ldo.64.h v10 i1 8 || iau.add i1 i1 16//nop 
 cvtColorRGBfp16ToLumaU8_loop:

	vau.add.f16 v15 v12 v13 || lsu0.ldo.64.l v11 i2 0 || lsu1.ldo.64.h v11 i2 8 || iau.add i2 i2 16
	vau.mul.f16 v4 v0 v1    || lsu0.swzv8 [00000000]   || lsu1.ldo.64.l v1 i0 0 
	vau.mul.f16 v5 v0 v2    || lsu0.swzv8 [11111111]   || lsu1.sti.128.f16.u8f v8 i17//nop 2
	vau.add.f16 v16 v15 v14 || lsu0.ldo.64.h v1 i0 8  || iau.add i0 i0 16|| lsu1.ldo.64.l v2 i1 0
	vau.mul.f16 v6 v0 v3    || lsu0.swzv8 [22222222]   || lsu1.ldo.64.h v2 i1 8  || iau.add i1 i1 16 //nop ---------- 
	vau.mul.f16 v12 v0 v9   || lsu0.swzv8 [00000000]   || lsu1.ldo.64.l v3 i2 0 
	lsu0.sti.128.f16.u8f v16 i17 ||	vau.add.f16 v7 v4 v5   || lsu1.ldo.64.h v3 i2 8 || iau.add i2 i2 16 
	_end_delay_loop_cvtColorRGBfp16ToLumaU8:
	
//1400576090
	
	 cmu.cmz.i32 i7
	 peu.pc1c eq || bru.jmp i9
	 nop 5
	 iau.sub i0 i0 48
	 iau.sub i1 i1 48
	 iau.sub i2 i2 48
	
 cvtColorRGBfp16ToLumaU8_compensate:
 LSU0.LDIL i10 cvtColorRGBfp16ToLumaU8_loop2  || LSU1.LDIH i10 cvtColorRGBfp16ToLumaU8_loop2 || iau.shr.u32 i7 i7 3 

 nop 6
	lsu0.ldo.64.l v1 i0 0 || lsu1.ldo.64.h v1 i0 8 || iau.add i0 i0 16 
	lsu0.ldo.64.l v2 i1 0 || lsu1.ldo.64.h v2 i1 8 || iau.add i1 i1 16 
	lsu0.ldo.64.l v3 i2 0 || lsu1.ldo.64.h v3 i2 8 || iau.add i2 i2 16 
	nop 4
	vau.mul.f16 v4 v0 v1   || lsu0.swzv8 [00000000] || lsu1.ldo.64.l v1 i0 0 || bru.rpl i10 i7 
	vau.mul.f16 v5 v0 v2   || lsu0.swzv8 [11111111] || lsu1.ldo.64.h v1 i0 8 || iau.add i0 i0 16
	vau.mul.f16 v6 v0 v3   || lsu0.swzv8 [22222222] || lsu1.ldo.64.l v2 i1 0  
	lsu0.ldo.64.h v2 i1 8  || iau.add i1 i1 16 //nop 
	
	vau.add.f16 v7 v4 v5   || lsu0.ldo.64.l v3 i2 0 || lsu1.ldo.64.h v3 i2 8 || iau.add i2 i2 16 
	nop 2
 cvtColorRGBfp16ToLumaU8_loop2:
	vau.add.f16 v8 v7 v6
	nop 2
	lsu0.sti.128.f16.u8f v8 i17 
	nop 3
	
	
 cvtColorRGBfp16ToLumaU8_final:	
	bru.jmp i30
	nop 6
.size mvcvCvtColorRGBfp16ToLumaU8_asm,.-mvcvCvtColorRGBfp16ToLumaU8_asm
.nowarnend
.end	
	
