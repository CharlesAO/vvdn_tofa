///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

//--------------------------------------------------------------------------------------------------------------
.type mvcvCvtColorRGBtoUV420_asm,@function
.version 00.50.37.8
.data .rodata.cvtColorRGBtoUV420_asm 
.align 16
 cvtColorRGBtoUV420_RGBtoUV420_coefs:
	 .float16 0.14713F16, 0.28886F16, 0.436F16, 0.615F16, 0.51499F16, 0.10001F16	
 cvtColorRGBtoUV420_clamp:
	 .float16 255.0
 cvtColorRGBtoUV420_const:
	 .float16 128.0
.code .text.cvtColorRGBtoUV420_asm //text
mvcvCvtColorRGBtoUV420_asm:
//void cvtColorRGBtoUV420(u8** inR, u8** inG, u8** inB, u8** uOut, u8** vOut, u32 width)
//                          i18      i17        i16          i15      i14          i13    
	
		
	LSU0.LDIL i11 cvtColorRGBtoUV420_RGBtoUV420_coefs || LSU1.LDIH i11 cvtColorRGBtoUV420_RGBtoUV420_coefs
	LSU0.LDIL i1 cvtColorRGBtoUV420_clamp 			  || LSU1.LDIH i1 cvtColorRGBtoUV420_clamp
	LSU0.LDIL i4 cvtColorRGBtoUV420_const 			  || LSU1.LDIH i4 cvtColorRGBtoUV420_const
	LSU0.LDIL i3 cvtColorRGBtoUV420_loop  			  || LSU1.LDIH i3 cvtColorRGBtoUV420_loop //i3 loop	
 	lsu0.ldil i5 0x1     			  || lsu1.ldih i5 0x0     || CMU.CPZV v20 0x3

	LSU0.LD.32 i16 i16 || LSU1.LD.32 i15 i15
	LSU0.LD.32 i18 i18 || LSU1.LD.32 i17 i17 
	LSU0.LD.32 i14 i14 
	LSU0.LD.16R v11, i4  || LSU1.LD.64.L v0, i11 || iau.add i11 i11 8 
	LSU0.LD.64.H v0, i11 || LSU1.LD.16R v10, i1  ||iau.shr.u32 i2 i13 3 
	nop 2
  
	//loading RGB
	LSU0.LDi.128.U8.F16 v3 i16 || IAU.SHR.u32 i13 i13 1  //Blue 
	LSU0.LDi.128.U8.F16 v1 i18 || IAU.ADD i0 i15 i13     //Red
	LSU0.LDi.128.U8.F16 v2 i17 || IAU.ADD i12 i14 i13    //Green
    LSU0.LD.64.l v23 i0 || LSU1.LD.64.h v23 i12 
	nop 3
	
	vau.mul.f16 v6 v0 v3 || lsu0.swzv8 [22222222] 
	vau.mul.f16 v4 v0 v1 || lsu0.swzv8 [00000000] 
	vau.mul.f16 v5 v0 v2 || lsu0.swzv8 [11111111] 
		vau.mul.f16 v13 v0 v1 || lsu0.swzv8 [33333333] 
	vau.sub.f16 v7 v6 v4   || LSU1.LD.128.U8.F16 v3 i16 || IAU.ADD i16 i16 8
	vau.sub.f16 v8 v11 v5  || LSU1.LD.128.U8.F16 v1 i18 || IAU.ADD i18 i18 8 
		vau.mul.f16 v14 v0 v2 || lsu0.swzv8 [44444444] 
		vau.mul.f16 v12 v0 v3 || lsu0.swzv8 [55555555] 
		
	vau.add.f16 v9 v8 v7 //u
	vau.sub.f16 v15 v13 v14      || LSU1.LDi.128.U8.F16 v2 i17 || bru.rpl i3 i2
	vau.sub.f16 v16 v11 v12
	vau.mul.f16 v6 v0 v3 		|| lsu0.swzv8 [22222222]
	vau.add.f16 v18 v9 v9    	|| lsu0.swzv8 [77553311] 
	vau.add.f16 v17 v15 v16 //v
	vau.mul.f16 v4 v0 v1 		|| lsu0.swzv8 [00000000] 
	vau.mul.f16 v18 v18 0.5
	vau.add.f16 v19 v17 v17     || lsu0.swzv8 [77553311] 
	vau.mul.f16 v5 v0 v2 		|| lsu0.swzv8 [11111111]
	vau.add.f16 v21 v18 v20  	|| lsu0.swzv8 [77556420] 
	
	vau.mul.f16 v19 v19 0.5

cvtColorRGBtoUV420_loop:
.nowarn 10
	vau.mul.f16 v13 v0 v1 		|| lsu0.swzv8 [33333333]
.nowarnend
	vau.sub.f16 v7 v6 v4
	vau.add.f16 v22 v19 v20  	|| lsu0.swzv8 [77556420] || LSU1.LDi.128.U8.F16 v3 i16 || cmu.clamp0.i16 v21 v21 v10
	vau.sub.f16 v8 v11 v5 		|| LSU1.LDi.128.U8.F16 v1 i18 
    vau.mul.f16 v14 v0 v2 		|| lsu0.swzv8 [44444444] || LSU1.ST.128.F16.U8 v21 i15 || iau.add i15 i15 4 
    vau.mul.f16 v12 v0 v3 		|| lsu0.swzv8 [55555555] || cmu.clamp0.i16 v22 v22 v10
	vau.add.f16 v9 v8 v7		|| LSU1.ST.128.F16.U8 v22 i14 || iau.add i14 i14 4
	
bru.jmp i30
LSU0.ST.64.l v23 i0 || LSU1.ST.64.h v23 i12
nop 5

.size mvcvCvtColorRGBtoUV420_asm,.-mvcvCvtColorRGBtoUV420_asm
.end
