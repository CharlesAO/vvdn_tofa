///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

//--------------------------------------------------------------------------------------------------------------
.type mvcvCvtColorNV21toRGB_asm,@function
.version 00.50.37.8
.data .rodata.cvtColorNV21toRGB_asm 
.align 16
 cvtColorNV21toRGB_NV21toRGB_coefs:
		.float16 0.344F16, 0.714F16	
 cvtColorNV21toRGB_clamp:
		.float16 255.0
 cvtColorNV21toRGB_const:
		.float16 128.0
 cvtColorNV21toRGB_coef1:
		.float16 1.402F16
 cvtColorNV21toRGB_coef2:
		.float16 1.772F16
.code .text.cvtColorNV21toRGB_asm //text
mvcvCvtColorNV21toRGB_asm:
//void cvtColorNV21toRGB(u8** yin, u8** uvin, u8** outR, u8** outG, u8** outB, u32 width)
//                          i18      i17        i16          i15      i14          i13      
	
	LSU0.LD.32 i18 i18 || LSU1.LD.32 i17 i17  // yIn and uvIn
	LSU0.LD.32 i16 i16 || LSU1.LD.32 i15 i15  //outR and outG
	LSU0.LD.32 i14 i14	//outB
nop 6
	lsu0.ldil i12 cvtColorNV21toRGB_NV21toRGB_coefs || lsu1.ldih i12 cvtColorNV21toRGB_NV21toRGB_coefs
	LSU0.LDIL i1 cvtColorNV21toRGB_const || LSU1.LDIH i1 cvtColorNV21toRGB_const
	LSU0.LDIL i2 cvtColorNV21toRGB_coef1 || LSU1.LDIH i2 cvtColorNV21toRGB_coef1
	LSU0.LDIL i4 cvtColorNV21toRGB_coef2 || LSU1.LDIH i4 cvtColorNV21toRGB_coef2	
	LSU0.LDIL i3 cvtColorNV21toRGB_loopp || LSU1.LDIH i3 cvtColorNV21toRGB_loopp // i3 loop
	LSU0.LDIL i0 cvtColorNV21toRGB_clamp || LSU1.LDIH i0 cvtColorNV21toRGB_clamp


	//loading y and uv
	LSU0.LD.128.U8.F16 v4 i17 || iau.add i17 i17 8 // uv
	LSU0.LD.16R v1, i1 	// vect with 128
	LSU0.LD.128.U8.F16 v3 i18 || iau.add i18 i18 8 // y
	
	
	iau.shr.u32 i5 i13 3
	LSU0.LD.16R v6, i2 ||  LSU1.LD.32R v10, i12 // vect with 1.402F16
												// vect with 0.343F16, 0.714F16....
	

	LSU0.LD.16R v11, i4  // vect with 0.772F16
	LSU0.LD.16R v0, i0 	// vect with 255
	nop
	
	vau.sub.f16 v5 v4 v1 || bru.rpl i3 i5 // uv-128
nop 2
	 
	// calc R
	vau.mul.f16 v7 v5 v6 || lsu0.swzv8 [77553311] 
	vau.mul.f16 v12 v5 v10 // mul between uv-128 and vect with coefs
nop
	vau.add.f16 v8 v3 v7 // outR 
	// end of calc R
	
	// calc G
	vau.add.f16 v13 v12 v12 || lsu0.swzv8 [77553311] 
	vau.mul.f16 v16 v5  v11 || lsu0.swzv8 [66442200]
nop
	cmu.cpvv v13 v13  || LSU1.SWZC8 [66442200] 
	vau.sub.f16 v14 v3 v13  // outG
	// end of calc G
	
	//calc B
	vau.add.f16 v17 v3 v16 // outB
	//end of calc b
 cvtColorNV21toRGB_loopp:
 .nowarn 10
	CMU.CLAMP0.F16 v9 v8 v0 || LSU0.LD.128.U8.F16 v4 i17 || iau.add i17 i17 8 // uv// outR clamp
.nowarnend
	CMU.CLAMP0.F16 v15 v14 v0 ||LSU0.LD.128.U8.F16 v3 i18 || iau.add i18 i18 8 // y// outG clamp
	CMU.CLAMP0.F16 v18 v17 v0 // outB clamp
	
	//store outR, outG, outB
	LSU0.ST.128.F16.U8 v9 i16 || IAU.ADD i16 i16 8
	LSU0.ST.128.F16.U8 v15 i15 || IAU.ADD i15 i15 8
	LSU0.ST.128.F16.U8 v18 i14 || IAU.ADD i14 i14 8
	nop
	//nop
	// end loop
	
	bru.jmp i30
nop 6
	
	
.size mvcvCvtColorNV21toRGB_asm,.-mvcvCvtColorNV21toRGB_asm
.end
