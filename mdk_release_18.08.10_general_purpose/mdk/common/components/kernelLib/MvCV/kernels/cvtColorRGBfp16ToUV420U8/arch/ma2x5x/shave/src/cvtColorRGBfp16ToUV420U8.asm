///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

//--------------------------------------------------------------------------------------------------------------
.type mvcvCvtColorRGBfp16ToUV420U8_asm,@function
.version 00.50.37.8
.data .rodata.cvtColorRGBfp16ToUV420U8_asm 
.align 16
 cvtColorRGBfp16ToUV420U8_RGBtoUV420_coefs:
	.float16 0.14713F16, 0.28886F16, 0.436F16, 0.615F16, 0.51499F16, 0.10001F16
 cvtColorRGBfp16ToUV420U8_clamp:
	.float16 1.0
 cvtColorRGBfp16ToUV420U8_const:
	.float16 0.5019F16 // 128/255....[0,1]
.code .text.cvtColorRGBfp16ToUV420U8_asm //text
mvcvCvtColorRGBfp16ToUV420U8_asm:
.nowarn 10
// void  cvtColorRGBfp16ToUV420U8(half* inRGB[3], u8* uOut, u8* vOut, u32 width)
//                                       i18        i17        i16      i15         
IAU.SHR.U32 I13 I15 1
    IAU.SUB i19 i19 8
	LSU0.ST.64.l v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v27  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v27  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v25  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v25  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v26  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v26  i19  || IAU.ADD I14 I17 I13

	LSU0.LDIL i6 0x0001 || LSU1.LDIH i6 0x0 || IAU.ADD I12 I16 I13
	LSU0.LDIL i11 cvtColorRGBfp16ToUV420U8_RGBtoUV420_coefs || LSU1.LDIH i11 cvtColorRGBfp16ToUV420U8_RGBtoUV420_coefs
	LSU0.LDIL i4 cvtColorRGBfp16ToUV420U8_const 			  || LSU1.LDIH i4 cvtColorRGBfp16ToUV420U8_const
	LSU0.LDIL i3 cvtColorRGBfp16ToUV420U8_loop  || LSU1.LDIH i3 cvtColorRGBfp16ToUV420U8_loop 
	nop 6
	LSU0.LD.32 i0 i18 || iau.add i18 i18 4 || LSU1.LDO.64.L V27 I14 0
	LSU0.LD.32 i1 i18 || iau.add i18 i18 4 || LSU1.LDO.64.H V27 I12 0
	LSU0.LD.32 i2 i18 || iau.add i18 i18 4
	LSU0.LD.16R v11, i4  || LSU1.LD.64.L v0, i11 || iau.add i11 i11 8 
	LSU0.LD.64.H v0, i11 || iau.shr.u32 i5 i15 3 || CMU.CPZV v23 0x3
	nop 4
		
		
	lsu0.ldo.64.l v3 i2 0 || lsu1.ldo.64.h v3 i2 8 || iau.add i2 i2 16
	lsu0.ldo.64.l v2 i1 0 || lsu1.ldo.64.h v2 i1 8 || iau.add i1 i1 16 //g
	lsu0.ldo.64.l v1 i0 0 || lsu1.ldo.64.h v1 i0 8 || iau.add i0 i0 16 
	nop 4
	vau.mul.f16 v4 v0 v3 || lsu0.swzv8 [22222222] || lsu1.ldo.64.l v3 i2 0 
	vau.mul.f16 v5 v0 v2 || lsu0.swzv8 [11111111] || lsu1.ldo.64.h v3 i2 8 || iau.add i2 i2 16 
	vau.mul.f16 v6 v0 v1 || lsu0.swzv8 [00000000] || lsu1.ldo.64.l v2 i1 0  
	vau.mul.f16 v10 v0 v1 || lsu0.swzv8 [33333333]  || lsu1.ldo.64.h v2 i1 8 || iau.add i1 i1 16 
	vau.sub.f16 v7 v4 v5 
	vau.mul.f16 v12 v0 v2 || lsu0.swzv8 [44444444]  
	vau.mul.f16 v13 v0 v3 || lsu0.swzv8 [55555555] 
	vau.sub.f16 v8 v7 v6 || lsu0.ldo.64.l v1 i0 0 || lsu1.ldo.64.h v1 i0 8 || iau.add i0 i0 16 
	vau.mul.f16 v4 v0 v3 || lsu0.swzv8 [22222222] 
	vau.sub.f16 v14 v10 v12   
	vau.add.f16 v9 v8 v11 
	vau.mul.f16 v5 v0 v2 || lsu0.swzv8 [11111111] 
	
	vau.sub.f16 v15 v14 v13 || bru.rpl i3 i5 _end_delay_loop_CvtColor
	vau.add.f16 v17 v9 v9   || lsu0.swzv8 [77553311]
	vau.mul.f16 v6 v0 v1    || lsu0.swzv8 [00000000] 
	vau.mul.f16 v10 v0 v1   || lsu0.swzv8 [33333333] 
	vau.add.f16 v18 v17 v23 || lsu0.swzv8 [77556420]
	vau.add.f16 v16 v15 v11 
	vau.sub.f16 v7 v4 v5    || lsu0.ldo.64.l v3 i2 0 || lsu1.ldo.64.h v3 i2 8 || iau.add i2 i2 16
	vau.mul.f16 v21 v18 0.5 || lsu0.ldo.64.l v2 i1 0 || lsu1.ldo.64.h v2 i1 8 || iau.add i1 i1 16
	vau.add.f16 v19 v16 v16 || lsu0.swzv8 [77553311]
	vau.mul.f16 v12 v0 v2   || lsu0.swzv8 [44444444] 
	vau.mul.f16 v13 v0 v3   || lsu0.swzv8 [55555555] 
 cvtColorRGBfp16ToUV420U8_loop:

	vau.add.f16 v20 v19 v23 || lsu0.swzv8 [77556420]
	vau.sub.f16 v8 v7 v6    || lsu0.ldo.64.l v1 i0 0 || lsu1.ldo.64.h v1 i0 8 || iau.add i0 i0 16
	vau.mul.f16 v4 v0 v3    || lsu0.swzv8 [22222222]
	vau.mul.f16 v22 v20 0.5 
	vau.sub.f16 v14 v10 v12 
	vau.add.f16 v9 v8 v11   || lsu0.st.128.f16.u8f v21 i17 || iau.add i17 i17 4
	vau.mul.f16 v5 v0 v2    || lsu0.swzv8 [11111111] || lsu1.st.128.f16.u8f v22 i16 || iau.add i16 i16 4 
	_end_delay_loop_CvtColor:
	
	//bru.rpim 0
	LSU0.STO.64.L V27 I17 0 ||  	LSU1.STO.64.H V27 I16 0
    LSU0.LD.64.h  v26  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v26  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v25  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v25  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v27  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v27  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v24  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v24  i19 || IAU.ADD i19 i19 8	
	
	
	bru.jmp i30
	nop 6
.nowarnend
.size mvcvCvtColorRGBfp16ToUV420U8_asm,.-mvcvCvtColorRGBfp16ToUV420U8_asm
.end	
	
