///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Downscale to half resolution in each direction with bilinear (sw) kernel// 
///            Fp16 in / U8 out
///

//--------------------------------------------------------------------------------------------------------------
.type mvispScale05BilinHV_Fp16U8_asm,@function
.version 00.50.37.8
.data .rodata.scale05BilinHV_Fp16U8_asm 
.align 16
 scale05BilinHV_Fp16U8_clamp:
	 .float16 255.0
 scale05BilinHV_Fp16U8_val:
	 .float16 0.25
.code .text.scale05BilinHV_Fp16U8_asm //text
mvispScale05BilinHV_Fp16U8_asm:
//void mvispScale05BilinHV_Fp16U8(half** in, u8** out, u32 inWidth)//
//                          i18        i17        i16       
	.nowarn
	LSU0.LDIL i3 scale05BilinHV_Fp16U8_loop  		 || LSU1.LDIH i3 scale05BilinHV_Fp16U8_loop 
	LSU0.LDIL i4 scale05BilinHV_Fp16U8_clamp   		 || LSU1.LDIH i4 scale05BilinHV_Fp16U8_clamp 
	LSU0.LDIL i6 scale05BilinHV_Fp16U8_val     		 || LSU1.LDIH i6 scale05BilinHV_Fp16U8_val 
	LSU0.LDIL i7 0x000F 			 				 || LSU1.LDIH i7 0x0 
	nop 6
	LSU0.LD.32 i0 i18 || LSU1.LD.32 i17 i17 || iau.add i18 i18 4
	LSU0.LD.32 i1 i18 || CMU.CPZV v20 0x3 
	LSU0.LD.16R v19, i6 || LSU1.LD.16R v22, i4 || iau.shr.u32 i2 i16 4
	nop 6
	cmu.cpvv v21 v19 || iau.and i11 i16 i7

	lsu0.ldo.64.l v1 i0 0 || lsu1.ldo.64.h v1 i0 8 || iau.add i0 i0 16  
	lsu0.ldo.64.l v2 i1 0 || lsu1.ldo.64.h v2 i1 8 || iau.add i1 i1 16 
	lsu0.ldo.64.l v8 i0 0 || lsu1.ldo.64.h v8 i0 8 || iau.add i0 i0 16 
	lsu0.ldo.64.l v9 i1 0 || lsu1.ldo.64.h v9 i1 8 || iau.add i1 i1 16
	
	lsu0.ldo.64.l v3 i0 0 || lsu1.ldo.64.h v3 i0 8 || iau.add i0 i0 16 
	lsu0.ldo.64.l v6 i1 0 || lsu1.ldo.64.h v6 i1 8 || iau.add i1 i1 16 
	lsu0.ldo.64.l v12 i0 0 || lsu1.ldo.64.h v12 i0 8 || iau.add i0 i0 16 
	lsu0.ldo.64.l v15 i1 0 || lsu1.ldo.64.h v15 i1 8 || iau.add i1 i1 16
	
		lsu0.ldo.64.l v1 i0 0 || lsu1.ldo.64.h v1 i0 8 || iau.add i0 i0 16
	    lsu0.ldo.64.l v2 i1 0 || lsu1.ldo.64.h v2 i1 8 || iau.add i1 i1 16
		lsu0.ldo.64.l v8 i0 0 || lsu1.ldo.64.h v8 i0 8 || iau.add i0 i0 16 
		lsu0.ldo.64.l v9 i1 0 || lsu1.ldo.64.h v9 i1 8 || iau.add i1 i1 16
	
	vau.add.f16 v0 v1 v2   || lsu0.ldo.64.l v3 i0 0 || lsu1.ldo.64.h v3 i0 8 || iau.add i0 i0 16 
	vau.add.f16 v10 v8 v9  || lsu0.ldo.64.l v6 i1 0 || lsu1.ldo.64.h v6 i1 8 || iau.add i1 i1 16
		vau.add.f16 v16 v3 v6     || lsu0.ldo.64.l v12 i0 0   || lsu1.ldo.64.h v12 i0 8 || iau.add i0 i0 16 
		vau.add.f16 v17 v12 v15   || lsu0.ldo.64.l v15 i1 0   || lsu1.ldo.64.h v15 i1 8 || iau.add i1 i1 16
	vau.add.f16 v4 v0 v0 || lsu0.swzv8 [77553311] 
	vau.add.f16 v11 v10 v10 || lsu0.swzv8 [77553311] 
		vau.add.f16 v18 v16 v16 || lsu0.swzv8 [77553311]    
		vau.add.f16 v19 v17 v17 || lsu0.swzv8 [77553311]  
	vau.add.f16 v4 v4 v20 || lsu0.swzv8 [66666420] 
	
	vau.add.f16 v11 v11 v20 || lsu0.swzv8 [66666420]  || bru.rpl i3 i2 _end_delay_loop_scale05HVBilin_Fp16U8
	vau.add.f16 v18 v18 v20 || lsu0.swzv8 [66666420] 
	vau.add.f16 v19 v19 v20 || lsu0.swzv8 [66666420] || lsu1.ldo.64.l v1 i0 0
	vau.add.f16 v0 v1 v2    || cmu.alignvec v13 v11 v4 8  ||  lsu0.ldo.64.l v2 i1 0 || lsu1.ldo.64.h v1 i0 8 || iau.add i0 i0 16
	vau.add.f16 v10 v8 v9   || cmu.alignvec v14 v13 v11 8 ||  lsu0.ldo.64.l v8 i0 0 || lsu1.ldo.64.h v2 i1 8 || iau.add i1 i1 16 
	vau.mul.f16 v14 v14 v21 || lsu0.ldo.64.l v9 i1 0   || lsu1.ldo.64.h v8 i0 8 || iau.add i0 i0 16 
	vau.add.f16 v16 v3 v6   || cmu.alignvec v18 v19 v18 8 ||  lsu0.ldo.64.l v3 i0 0 || lsu1.ldo.64.h v9 i1 8 || iau.add i1 i1 16
 scale05BilinHV_Fp16U8_loop:
	vau.add.f16 v17 v12 v15 || cmu.alignvec v5 v18 v19 8 ||  lsu0.ldo.64.l v6 i1 0  || lsu1.ldo.64.h v3 i0 8 || iau.add i0 i0 16    
	vau.mul.f16 v5 v5 v21  || lsu0.ldo.64.l v12 i0 0 || lsu1.ldo.64.h v6 i1 8 || iau.add i1 i1 16  
	vau.add.f16 v4 v0 v0  || lsu0.swzv8 [77553311]    || lsu1.ldo.64.h v12 i0 8 || iau.add i0 i0 16
	vau.add.f16 v11 v10 v10 || lsu0.swzv8 [77553311]  || lsu1.ldo.64.l v15 i1 0 
	vau.add.f16 v18 v16 v16 || lsu0.swzv8 [77553311]  || lsu1.ldo.64.h v15 i1 8 || iau.add i1 i1 16
	vau.add.f16 v19 v17 v17 || lsu0.swzv8 [77553311]  || lsu1.st.128.f16.u8f v14 i17 || iau.add i17 i17 8  
	vau.add.f16 v4 v4 v20 || lsu0.swzv8 [66666420]    || lsu1.sti.128.f16.u8f v5 i17 
	_end_delay_loop_scale05HVBilin_Fp16U8:

	cmu.cmz.i32 i11 
	peu.pc1c eq || bru.jmp i30
		
		vau.add.f16 v11 v11 v20 || lsu0.swzv8 [66666420]  
	nop 2
		cmu.alignvec v13 v11 v4 8  
		cmu.alignvec v14 v13 v11 8
		vau.mul.f16 v14 v14 v21 
	nop	2
		lsu1.st.128.f16.u8f v14 i17 
	
	bru.jmp i30
	nop 6
	.nowarnend
.size mvispScale05BilinHV_Fp16U8_asm,.-mvispScale05BilinHV_Fp16U8_asm
.end
	

	
	
