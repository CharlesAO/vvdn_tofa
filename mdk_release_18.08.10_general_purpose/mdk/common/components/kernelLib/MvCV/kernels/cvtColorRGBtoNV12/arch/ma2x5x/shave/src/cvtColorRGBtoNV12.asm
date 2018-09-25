///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

//--------------------------------------------------------------------------------------------------------------
.type mvcvCvtColorRGBtoNV12_asm,@function
.version 00.50.37.8
.nowarn 10
.data .rodata.cvtColorRGBtoNV12_asm 
.align 16
 cvtColorRGBtoNV12_RGBtoNV12_coefs:
		.float16 0.299F16, 0.587F16, 0.114F16, 0.0F16, 0.564F16, 0.713F16	
 cvtColorRGBtoNV12_clamp:
		.float16 255.0
 cvtColorRGBtoNV12_const:
		.float16 128.0
		// cvtColorRGBtoNV12f_const1:
		//.float16 1.0
.code .text.cvtColorRGBtoNV12_asm //text
mvcvCvtColorRGBtoNV12_asm:
//void cvtColorRGBtoNV12(u8** inR, u8** inG, u8** inB, u8** yOut, u8** uvOut, u32 width, u32 k)
//                       i18      i17        i16          i15      i14          i13       i12
	
	IAU.SUB i19 i19 16
	LSU0.STo.64.L v24  i19 0 || LSU1.STo.64.H v24  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.L v25  i19 0 || LSU1.STo.64.H v25  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.L v26  i19 0 || LSU1.STo.64.H v26  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.L v27  i19 0 || LSU1.STo.64.H v27  i19 8
 
	
	// load coefs and const
	LSU0.LDIL i11 cvtColorRGBtoNV12_RGBtoNV12_coefs || LSU1.LDIH i11 cvtColorRGBtoNV12_RGBtoNV12_coefs
	LSU0.LDIL i1 cvtColorRGBtoNV12_clamp || LSU1.LDIH i1 cvtColorRGBtoNV12_clamp
	LSU0.LDIL i4 cvtColorRGBtoNV12_const || LSU1.LDIH i4 cvtColorRGBtoNV12_const
	
	LSU0.LDIL i3 cvtColorRGBtoNV12_loop  || LSU1.LDIH i3 cvtColorRGBtoNV12_loop //i3 loop
	//======
	lsu0.ldil i8 cvtColorRGBtoNV12_endif || lsu1.ldih i8 cvtColorRGBtoNV12_endif
	//======
	lsu0.ldil i5 0x1 || lsu1.ldih i5 0x0
	iau.shr.u32 i2 i13 3	

	
	LSU0.LD.32 i18 i18 || LSU1.LD.32 i17 i17   || iau.and i0 i12 i5 
	LSU0.LD.32 i16 i16 || LSU1.LD.32 i15 i15    ||  cmu.cmz.i32 i0  
	peu.pc1c neq  || bru.jmp i8 
	LSU0.LD.32 i14 i14 || LSU1.LD.16R v9, i1
	
	LSU0.LD.16R v11, i4 || LSU1.LD.64.L v0, i11 || IAu.add i11 i11 8 //put in v0 coefs = like a vector
	LSU0.LD.64.H v0, i11	

nop 3
	//loading RGB

	LSU0.LD.128.U8.F16 v1 i18 || IAU.ADD i18 i18 8 //Red
	LSU0.LD.128.U8.F16 v2 i17 || IAU.ADD i17 i17 8 //Green
	LSU0.LD.128.U8.F16 v3 i16 || IAU.ADD i16 i16 8 //Blue
nop 4
	
	// calc. y
	VAU.MUL.f16 v4 v0 v1 || lsu0.swzv8 [00000000] // reg * 0.299
	                 
	VAU.MUL.f16 v5 v0 v2 || lsu0.swzv8 [11111111] //green * 0.587
	                 
	VAU.MUL.f16 v6 v0 v3 || lsu0.swzv8 [22222222] //blue * 0.144
nop
	vau.add.f16 v7 v4 v5 
nop 2
	vau.add.f16 v8 v7 v6 // yOut
	//end of calc y
	
	//=======
	 
nop 
LSU0.LD.128.U8.F16 v1 i18 || IAU.ADD i18 i18 8
	//=======
	
	//---------------------------------------------
	// calc. u
	vau.sub.f16 v12 v3 v8 
	vau.sub.f16 v15 v1 v8
nop
	vau.mul.f16 v13 v0 v12 || lsu0.swzv8 [44444444] 
	vau.mul.f16 v16 v0 v15 || lsu0.swzv8 [55555555] 
bru.rpl i3 i2
	
	vau.add.f16 v14 v13 v11 || LSU0.LD.128.U8.F16 v2 i17 || IAU.ADD i17 i17 8// in v14 is u
	vau.add.f16 v17 v16 v11 || LSU0.LD.128.U8.F16 v3 i16 || IAU.ADD i16 i16 8 // in v17 is v
  nop
	vau.add.f16 v18 v14 v14 || lsu0.swzv8 [77553311] //sum u1,u2..
	vau.add.f16 v20 v17 v17 || lsu0.swzv8 [77553311] //sum v1,v2..
	VAU.MUL.f16 v4 v0 v1    || lsu0.swzv8 [00000000] // reg * 0.299
	
	vau.mul.f16 v19 v18 0.5 // 0,2,4,6=avg u1,u2
	// end of calc. u
	
	// calc. v
	
	vau.mul.f16 v21 v20 0.5 //0,2,4,6=avg v1,v2
	// end of calc. v	
	VAU.MUL.f16 v5 v0 v2 || lsu0.swzv8 [11111111] //green * 0.587
	VAU.MUL.f16 v6 v0 v3 || lsu0.swzv8 [22222222] //blue * 0.144
	//------------
	
	cmu.vilv.x16 v25 v24 v21 v19 
	CMU.VSZM.WORD v26 v25 [DD20] || vau.add.f16 v7 v4 v5 
	CMU.VSZM.WORD v26 v24 [20DD] //in v26 is uvOut
	CMU.CLAMP0.F16 v27 v26 v9      // in v27 is clamp uvOut
	
	vau.add.f16 v8 v7 v6  

 cvtColorRGBtoNV12_loop:
	//-----------------------------------------
	 LSU0.ST.128.F16.U8 v27 i14 || IAU.ADD i14 i14 8  //store uvOut
	//========
	//========
	
	CMU.CLAMP0.F16 v10 v8 v9 
	
	vau.sub.f16 v12 v3 v8     ||  LSU0.LD.128.U8.F16 v1 i18 || IAU.ADD i18 i18 8 

	vau.sub.f16 v15 v1 v8
	LSU0.ST.128.F16.U8 v10 i15 || IAU.ADD i15 i15 8  //store yOut
	vau.mul.f16 v13 v0 v12 || lsu0.swzv8 [44444444]     
	vau.mul.f16 v16 v0 v15 || lsu0.swzv8 [55555555]   




 
	LSU0.LDo.64.L  v27  i19 0 || LSU1.LDo.64.H  v27  i19 8 || IAU.ADD i19 i19 16     
	LSU0.LDo.64.L  v26  i19 0 || LSU1.LDo.64.H  v26  i19 8 || IAU.ADD i19 i19 16
	LSU0.LDo.64.L  v25  i19 0 || LSU1.LDo.64.H  v25  i19 8 || IAU.ADD i19 i19 16
	LSU0.LDo.64.L  v24  i19 0 || LSU1.LDo.64.H  v24  i19 8 || IAU.ADD i19 i19 16
	bru.jmp i30
	nop 6

	
 cvtColorRGBtoNV12_endif:
	
	LSU0.LD.128.U8.F16 v1 i18 || IAU.ADD i18 i18 8  //RED
	LSU0.LD.128.U8.F16 v2 i17 || iau.ADD i17 i17 8 //Green
	LSU0.LD.128.U8.F16 v3 i16 || IAU.ADD i16 i16 8 //Blue
LSU0.LDIL i3 cvtColorRGBtoNV12_loop2 || LSU1.LDIH i3 cvtColorRGBtoNV12_loop2     //i3 loop
	
	NOP 3

	
	// calc. y
	VAU.MUL.f16 v4 v0 v1 || lsu0.swzv8 [00000000] // reg * 0.299
	VAU.MUL.f16 v5 v0 v2 || lsu0.swzv8 [11111111]     //green * 0.587               
	VAU.MUL.f16 v6 v0 v3 || lsu0.swzv8 [22222222]  //blue * 0.144
nop
	vau.add.f16 v7 v4 v5 

	LSU0.LD.128.U8.F16 v1 i18 || IAU.ADD i18 i18 8     
	LSU0.LD.128.U8.F16 v2 i17 || IAU.ADD i17 i17 8     || LSU1.LD.128.U8.F16 v3 i16 || sAU.ADD.i32 i16 i16 8 
	nop 2
	
	LSU0.LD.128.U8.F16 v1 i18 || IAU.ADD i18 i18 8     ||  bru.rpl i3 i2
	vau.add.f16 v8 v7 v6 // yOut
	LSU0.LD.128.U8.F16 v2 i17 || IAU.ADD i17 i17 8     || LSU1.LDI.128.U8.F16 v3 i16 
    
	//end of calc y

	//=======
	//========
 cvtColorRGBtoNV12_loop2:
nop
	CMU.CLAMP0.F16 v10 v8 v9 || VAU.MUL.f16 v5 v0 v2 || lsu1.swzv8 [11111111]   
	VAU.MUL.f16 v4 v0 v1     || lsu0.swzv8 [00000000] // reg * 0.299
	LSU0.ST.128.F16.U8 v10 i15 || IAU.ADD i15 i15 8  //store yOut
	VAU.MUL.f16 v6 v0 v3 || lsu0.swzv8 [22222222]   //blue * 0.144
	vau.add.f16 v7 v4 v5 
	
	LSU0.LDo.64.L  v27  i19 0 || LSU1.LDo.64.H  v27  i19 8 || IAU.ADD i19 i19 16                        
	LSU0.LDo.64.L  v26  i19 0 || LSU1.LDo.64.H  v26  i19 8 || IAU.ADD i19 i19 16
	LSU0.LDo.64.L  v25  i19 0 || LSU1.LDo.64.H  v25  i19 8 || IAU.ADD i19 i19 16
	LSU0.LDo.64.L  v24  i19 0 || LSU1.LDo.64.H  v24  i19 8 || IAU.ADD i19 i19 16
	bru.jmp i30  
	nop 6
.nowarnend
.size mvcvCvtColorRGBtoNV12_asm,.-mvcvCvtColorRGBtoNV12_asm
.end
