///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

//--------------------------------------------------------------------------------------------------------------
.type mvcvCvtColorRGBtoLuma_asm,@function
.version 00.50.37.8
.data .rodata.cvtColorRGBtoLuma_asm 
.align 16
 cvtColorRGBtoLuma_RGBtoLuma_coefs:
		.float16 0.299F16, 0.587F16, 0.114F16	
 cvtColorRGBtoLuma_clamp:
		.float16 255.0
		
.code .text.cvtColorRGBtoLuma_asm //text
mvcvCvtColorRGBtoLuma_asm:
//void cvtColorRGBtoLuma(u8** inR, u8** inG, u8** inB, u8** yOut, u32 width)
//                       i18      i17        i16          i15      i14  
.nowarn
	// load coefs and const
	LSU0.LDIL i5 0x000F || LSU1.LDIH i5 0x0
	LSU0.LDIL i11 cvtColorRGBtoLuma_RGBtoLuma_coefs || LSU1.LDIH i11 cvtColorRGBtoLuma_RGBtoLuma_coefs
	LSU0.LDIL i1 cvtColorRGBtoLuma_clamp || LSU1.LDIH i1 cvtColorRGBtoLuma_clamp
	LSU0.LDIL i3 cvtColorRGBtoLuma_loop  || LSU1.LDIH i3 cvtColorRGBtoLuma_loop 
	lsu0.ldil i6 cvtColorRGBtoLuma_compensate || lsu1.ldih i6 cvtColorRGBtoLuma_compensate
	lsu0.ldil i7 cvtColorRGBtoLuma_final || lsu1.ldih i7 cvtColorRGBtoLuma_final
	
	
	LSU0.LD.32 i18 i18 || LSU1.LD.32 i17 i17
	LSU0.LD.32 i16 i16 || LSU1.LD.32 i15 i15 || iau.and i4 i14 i5
	LSU0.LD.64.L v0, i11 || iau.shr.u32 i2 i14 4 ||	LSU1.LD.16R v9, i1
	nop 6
	

	cmu.cmz.i32 i2
	peu.pc1c eq || bru.jmp i6
	nop 6

	LSU0.LDi.128.U8.F16 v1 i18 
	LSU0.LDi.128.U8.F16 v2 i17 
	LSU0.LDi.128.U8.F16 v3 i16 
	LSU0.LDi.128.U8.F16 v11 i18 
	LSU0.LDi.128.U8.F16 v12 i17 
	LSU0.LDi.128.U8.F16 v13 i16 
	nop 
	
	
	VAU.MUL.f16 v4 v0 v1 || lsu0.swzv8 [00000000] || LSU1.LDi.128.U8.F16 v1 i18
	VAU.MUL.f16 v5 v0 v2 || lsu0.swzv8 [11111111] || LSU1.LDi.128.U8.F16 v2 i17
	VAU.MUL.f16 v6 v0 v3 || lsu0.swzv8 [22222222] || LSU1.LDi.128.U8.F16 v3 i16
	
	VAU.MUL.f16 v14 v0 v11 || lsu0.swzv8 [00000000] || LSU1.LDi.128.U8.F16 v11 i18    
	vau.add.f16 v7 v4 v5   || LSU1.LDi.128.U8.F16 v12 i17 
	VAU.MUL.f16 v15 v0 v12 || lsu0.swzv8 [11111111]  || LSU1.LDi.128.U8.F16 v13 i16       
	VAU.MUL.f16 v16 v0 v13 || lsu0.swzv8 [22222222]      
	vau.add.f16 v8 v7 v6    
	vau.add.f16 v17 v14 v15 
		VAU.MUL.f16 v4 v0 v1  || lsu0.swzv8 [00000000]  || LSU1.LDi.128.U8.F16 v2 i17 || bru.rpl i3 i2
	    VAU.MUL.f16 v5 v0 v2  || lsu0.swzv8 [11111111] || LSU1.LDi.128.U8.F16 v3 i16
	CMU.CLAMP0.F16 v10 v8 v9  || VAU.MUL.f16 v6 v0 v3  || lsu0.swzv8 [22222222] || LSU1.LDi.128.U8.F16 v1 i18 
 cvtColorRGBtoLuma_loop:

		VAU.MUL.f16 v14 v0 v11 || lsu0.swzv8 [00000000] || LSU1.LDi.128.U8.F16 v11 i18 

		LSU0.STi.128.F16.U8 v10 i15 || vau.add.f16 v18 v17 v16
		vau.add.f16 v7 v4 v5 || LSU1.LDi.128.U8.F16 v12 i17
		VAU.MUL.f16 v15 v0 v12 || lsu0.swzv8 [11111111]  || LSU1.LDi.128.U8.F16 v13 i16   
	CMU.CLAMP0.F16 v20 v18 v9 || VAU.MUL.f16 v16 v0 v13 || lsu0.swzv8 [22222222]
		vau.add.f16 v8 v7 v6
	LSU0.STi.128.F16.U8 v20 i15 || vau.add.f16 v17 v14 v15 
	
	
	cmu.cmz.i32 i4 
	peu.pc1c eq || bru.jmp i7
	nop 5
		
	iau.sub i18 i18 32
	iau.sub i17 i17 32
	iau.sub i16 i16 32
			
 cvtColorRGBtoLuma_compensate:

	LSU0.LDIL i3 cvtColorRGBtoLuma_loop2  || LSU1.LDIH i3 cvtColorRGBtoLuma_loop2 || iau.shr.u32 i4 i4 3 

	LSU0.LDi.128.U8.F16 v1 i18 || LSU1.LDi.128.U8.F16 v2 i17 
	LSU0.LDi.128.U8.F16 v3 i16 
	nop 6
	VAU.MUL.f16 v4 v0 v1 || lsu0.swzv8 [00000000] || LSU1.LD.128.U8.F16 v1 i18 || IAU.ADD i18 i18 8 || bru.rpl i3 i4
	VAU.MUL.f16 v5 v0 v2 || lsu0.swzv8 [11111111] || LSU1.LD.128.U8.F16 v2 i17 || IAU.ADD i17 i17 8 		
	VAU.MUL.f16 v6 v0 v3 || lsu0.swzv8 [22222222] || LSU1.LD.128.U8.F16 v3 i16 || IAU.ADD i16 i16 8 		
	nop
	vau.add.f16 v7 v4 v5 
	nop 
 cvtColorRGBtoLuma_loop2:
	nop
	vau.add.f16 v8 v7 v6 // yOut
	nop 2
	CMU.CLAMP0.F16 v10 v8 v9
	nop 
	LSU0.ST.128.F16.U8 v10 i15 || IAU.ADD i15 i15 8
	
		
			
 cvtColorRGBtoLuma_final:	
	bru.jmp i30
	nop 6
.nowarnend
.size mvcvCvtColorRGBtoLuma_asm,.-mvcvCvtColorRGBtoLuma_asm
.end	
