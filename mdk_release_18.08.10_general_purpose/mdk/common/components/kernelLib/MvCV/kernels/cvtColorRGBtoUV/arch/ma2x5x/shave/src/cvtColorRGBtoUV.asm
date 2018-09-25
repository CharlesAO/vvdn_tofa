///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

//--------------------------------------------------------------------------------------------------------------
.type mvcvCvtColorRGBtoUV_asm,@function
.version 00.50.37.8
.data .rodata.cvtColorRGBtoUV_asm 
.align 16
 cvtColorRGBtoUV_RGBtoNV21_coefs:
		.float16 0.299F16, 0.587F16, 0.114F16, 0.0F16, 0.564F16, 0.713F16	
 cvtColorRGBtoUV_clamp:
		.float16 255.0
 cvtColorRGBtoUV_const:
		.float16 128.0
		// cvtColorRGBtoUV_const1:
		//.float16 1.0
.code .text.cvtColorRGBtoUV_asm //text
mvcvCvtColorRGBtoUV_asm:
//void cvtColorRGBtoNV21(u8** inR, u8** inG, u8** inB, u8** uvOut, u32 width, u32 line)
//                           i18      i17        i16          i15      i14        i13    
.nowarn	

	LSU0.LDIL i11 cvtColorRGBtoUV_RGBtoNV21_coefs || LSU1.LDIH i11 cvtColorRGBtoUV_RGBtoNV21_coefs
	LSU0.LDIL i1 cvtColorRGBtoUV_clamp || LSU1.LDIH i1 cvtColorRGBtoUV_clamp
	LSU0.LDIL i4 cvtColorRGBtoUV_const || LSU1.LDIH i4 cvtColorRGBtoUV_const
	LSU0.LDIL i3 cvtColorRGBtoUV_loop || LSU1.LDIH i3 cvtColorRGBtoUV_loop //i3 loop
	lsu0.ldil i8 cvtColorRGBtoUV_endif || lsu1.ldih i8 cvtColorRGBtoUV_endif

	
	lsu0.ldil i5 0x1 || lsu1.ldih i5 0x0
	LSU0.LD.32 i18 i18 || LSU1.LD.32 i17 i17
	LSU0.LD.32 i16 i16 || LSU1.LD.32 i15 i15
	LSU1.LD.16R v9, i1
	
	LSU0.LD.16R v11, i4 || LSU1.LD.64.L v0, i11 || iau.add i11 i11 8 
	LSU0.LD.64.H v0, i11	
	
	iau.shr.u32 i2 i14 3	
	iau.and i0 i13 i5
	cmu.cmz.i32 i0
	peu.pc1c neq || bru.jmp i8 
	nop 6
	
	
	//loading RGB
	LSU0.LDi.128.U8.F16 v1 i18  //Red
	LSU0.LDi.128.U8.F16 v2 i17  //Green
	LSU0.LDi.128.U8.F16 v3 i16  //Blue
	
	
	nop 4
	// calc. y
	VAU.MUL.f16 v7 v0 v1 || lsu0.swzv8 [00000000] 
	VAU.MUL.f16 v2 v0 v2 || lsu0.swzv8 [11111111] 
	VAU.MUL.f16 v8 v0 v3 || lsu0.swzv8 [22222222] 
	nop
	vau.add.f16 v7 v7 v2  
	    LSU0.LDi.128.U8.F16 v2 i17 
		LSU0.LDi.128.U8.F16 v1 i18
	    LSU0.LDi.128.U8.F16 v3 i16		
	vau.add.f16 v8 v7 v8 // yOut
	nop 2	
	vau.sub.f16 v12 v3 v8 
	vau.sub.f16 v15 v1 v8 
		VAU.MUL.f16 v7 v0 v1 || lsu0.swzv8 [00000000] 
	vau.mul.f16 v12 v0 v12 	 || lsu0.swzv8 [44444444] 
	vau.mul.f16 v15 v0 v15   || lsu0.swzv8 [55555555] 
		VAU.MUL.f16 v2 v0 v2 || lsu0.swzv8 [11111111] || bru.rpl i3 i2 
	vau.add.f16 v14 v12 v11  // in v14 is u
	vau.add.f16 v17 v15 v11   // in v17 is v
		VAU.MUL.f16 v8 v0 v3 || lsu0.swzv8 [22222222] 
	vau.add.f16 v18 v14 v14  || lsu0.swzv8 [77553311] //sum u1,u2..
	vau.add.f16 v20 v17 v17  || lsu0.swzv8 [77553311] //sum v1,v2..
		vau.add.f16 v7 v7 v2  
	vau.mul.f16 v19 v18 0.5  || LSU0.LDi.128.U8.F16 v1 i18// 0,2,4,6=avg u1,u2
	vau.mul.f16 v21 v20 0.5  || LSU0.LDi.128.U8.F16 v2 i17  //0,2,4,6=avg v1,v2
		vau.add.f16 v8 v7 v8  
 cvtColorRGBtoUV_loop:
	nop
	cmu.vilv.x16 v4 v5 v21 v19 || LSU1.LDi.128.U8.F16 v3 i16
		vau.sub.f16 v12 v3 v8    || CMU.VSZM.WORD v6 v4 [DD20] 
		vau.sub.f16 v15 v1 v8    || CMU.VSZM.WORD v6 v5 [20DD] 
		VAU.MUL.f16 v7 v0 v1     || lsu0.swzv8 [00000000]  || CMU.CLAMP0.F16 v6 v6 v9    
		vau.mul.f16 v12 v0 v12   || lsu0.swzv8 [44444444]
		vau.mul.f16 v15 v0 v15   || lsu0.swzv8 [55555555]  || LSU1.STi.128.F16.U8 v6 i15
   
		
 cvtColorRGBtoUV_endif:
	bru.jmp i30
	nop 6
	

	
//	bru.jmp i30
//nop 6
	
.size mvcvCvtColorRGBtoUV_asm,.-mvcvCvtColorRGBtoUV_asm
.nowarnend
.end
