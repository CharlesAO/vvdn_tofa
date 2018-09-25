///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    Color space transformation: YUV to RGB
///

.type mvcvCvtColorKernelYUVToRGB_asm,@function
.version 00.51.05
//-------------------------------------------------------------------------------
.data .rodata.cvtColorKernelYUVToRGB
.align 16
 cvtColorKernelYUVToRGB_YUV2RGB_coefs:  //-128      1.402        0.344      0.714        1.772
	.float16 -0.501960F16, 1.402F16, 0.344F16, 0.714F16, 1.772F16
//									i18					i17					i16					i15			i14				
//cvtColorKernelYUVToRGB_asm(unsigned char* yIn, unsigned char* uIn, unsigned char* vIn, unsigned char* out, u32 width)// 

.code .text.cvtColorKernelYUVToRGB
mvcvCvtColorKernelYUVToRGB_asm:
.nowarn 10
	// function parameters:
	// i18 - y in buffer
	// i17 - u in buffer
	// i16 - v in buffer
	// i15 - out buffer
	// i14 - width
	lsu0.ldil i1, 0x2422     ||  lsu1.ldih i1, 0x2242
	lsu0.ldil i2, 0x4224     ||  lsu1.ldih i2, 0x4444
	lsu0.ldil i3, 0x4444     ||  lsu1.ldih i3, 0x2422
	lsu0.ldil i4, 0x2242     ||  lsu1.ldih i4, 0x4224
	lsu0.ldil i5, 0x4224     ||  lsu1.ldih i5, 0x2422
	lsu0.ldil i6, 0x2242     ||  lsu1.ldih i6, 0x4224
	
	lsu0.ldil i0, cvtColorKernelYUVToRGB_YUV2RGB_coefs     ||  lsu1.ldih i0, cvtColorKernelYUVToRGB_YUV2RGB_coefs
	lsu0.ldil i10, cvtColorKernelYUVToRGB_loop     ||  lsu1.ldih i10, cvtColorKernelYUVToRGB_loop
	IAU.SHR.i32 i14 i14 3	
	lsu0.ldo.64.l v0 i0 0 || lsu1.ldo.64.h v0 i0 8
	vau.xor v18 v18 v18 
	vau.xor v19 v19 v19 
	vau.xor v20 v20 v20 
	vau.xor v21 v21 v21 

	 
	 
	LSU0.LD.128.u8f.f16 v1 i18 || IAU.ADD i18 i18 8 
	LSU0.LD.128.u8f.f16 v2 i17 || IAU.ADD i17 i17 4 //u_in
	LSU0.LD.128.u8f.f16 v3 i16 || IAU.ADD i16 i16 4 //v_in
	nop 5 
	CMU.VILV.x16 v4 v6  v2 v2 
	CMU.VILV.x16 v5 v7  v3 v3                     
	vau.add.f16 v6 v0 v4 || LSU0.SWZV8 [00000000] || LSU1.LD.128.u8f.f16 v2 i17 || IAU.ADD i17 i17 4 //y//u_in//v_in//b
	vau.add.f16 v7 v0 v5 || LSU0.SWZV8 [00000000] || LSU1.LD.128.u8f.f16 v3 i16 || IAU.ADD i16 i16 4//u//g//v
	nop
	vau.mul.f16  v9 v0 v6 || LSU0.SWZV8 [44444444] // u*1.772
	vau.mul.f16  v8 v0 v7 || LSU0.SWZV8 [11111111] // v*1.402
	vau.mul.f16 v10 v0 v7 || LSU0.SWZV8 [33333333] || bru.rpl i10 i14// v*0.714  
	vau.mul.f16 v11 v0 v6 || LSU0.SWZV8 [22222222] // u*0.344
	nop 2
	vau.add.f16 v14 v10 v11 || CMU.VILV.x16 v4 v6  v2 v2 
	vau.add.f16 v12 v1 v8   || CMU.VILV.x16 v5 v7  v3 v3//r
	vau.add.f16 v13 v1 v9  
	vau.sub.f16 v15 v1 v14 
	vau.add.f16 v6 v0 v4 || LSU0.SWZV8 [00000000] || LSU1.LD.128.u8f.f16 v2 i17 || IAU.ADD i17 i17 4
    vau.add.f16 v7 v0 v5 || LSU0.SWZV8 [00000000] || LSU1.LD.128.u8f.f16 v3 i16 || IAU.ADD i16 i16 4
	CMU.VILV.x16 v16 v17  v15 v12 || LSU0.LD.128.u8f.f16 v1 i18 || IAU.ADD i18 i18 8//u_in
	CMU.CPIT   C_CMU0 i1          
	PEU.PVV16 GT || VAU.OR v19 v16 v18 || LSU0.SWZV8 [54732710] || CMU.CPIT   C_CMU0 i2
	PEU.PVV16 GT || VAU.OR v20 v16 v18 || LSU0.SWZV8 [77777767] || CMU.CPIT   C_CMU0 i3
 cvtColorKernelYUVToRGB_loop:
	PEU.PVV16 GT || VAU.OR v20 v17 v18 || LSU0.SWZV8 [27107777] || CMU.CPIT   C_CMU0 i4
	PEU.PVV16 GT || VAU.OR v21 v17 v18 || LSU0.SWZV8 [77675473] || CMU.CPIT   C_CMU0 i1
	PEU.PVV16 LT || VAU.OR v19 v13 v18 || LSU0.SWZV8 [77177077] || CMU.CPIT   C_CMU0 i5
	PEU.PVV16 LT || VAU.OR v20 v13 v18 || LSU0.SWZV8 [74773772] || CMU.CPIT   C_CMU0 i6
	PEU.PVV16 LT || VAU.OR v21 v13 v18 || LSU0.SWZV8 [77767757] || LSU1.STI.128.f16.u8f v19 i15 
	vau.mul.f16  v9 v0 v6 || LSU0.SWZV8 [44444444] || LSU1.STI.128.f16.u8f v20 i15 
	vau.mul.f16  v8 v0 v7 || LSU0.SWZV8 [11111111] || LSU1.STI.128.f16.u8f v21 i15 

	bru.jmp i30
nop 6
.nowarnend
.size mvcvCvtColorKernelYUVToRGB_asm,.-mvcvCvtColorKernelYUVToRGB_asm
.end
