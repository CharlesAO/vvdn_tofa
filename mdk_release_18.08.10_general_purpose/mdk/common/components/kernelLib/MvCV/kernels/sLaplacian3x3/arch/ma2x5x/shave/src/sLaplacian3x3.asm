///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvSLaplacian3x3_asm,@function
.version 00.51.04

.data .data.slaplacian3x3
.align 16
 sLaplacian3x3_clampVal:
        .float16 255.0

.code .text.laplacian3x3

//void laplacian3x3(u8** in, u8** out, u32 inWidth)//
//					i18         i17       i16
mvcvSLaplacian3x3_asm:
	LSU0.LDIL i3 sLaplacian3x3_clampVal || LSU1.LDIH i3 sLaplacian3x3_clampVal
	lsu0.ldil i5 sLaplacian3x3_loop || lsu1.ldih i5 sLaplacian3x3_loop
	lsu0.ldil i9 sLaplacian3x3_compensate || lsu1.ldih i9 sLaplacian3x3_compensate
	LSU0.LDIL i8 0x000F || LSU1.LDIH i8 0x0 
	
    LSU0.LD.32  i0  i18 || IAU.ADD i18 i18 4 //line 1
    LSU0.LD.32  i1  i18 || IAU.ADD i18 i18 4 // line 2
    LSU0.LD.32  i2  i18 || IAU.ADD i18 i18 4 || LSU1.LD.32  i17  i17 // line 3
nop 3
    LSU0.LD.16R v20, i3 || IAU.AND i10 i16 i8//|| CMU.CPZV v0 0x3
	IAU.SHR.u32 i6 i16 4 
	
	cmu.cmz.i8 i6
	peu.pc1c eq || bru.jmp i9
nop 6
	
	iau.sub i1 i1 8  
	LSU0.LD.128.U8.U16 v2 i1 || iau.add i1 i1 8  
	LSU0.LD.128.U8.U16 v3 i1 || iau.add i1 i1 8 
	
	LSU1.LDi.128.U8.U16 v4 i1  
	LSU0.LDI.128.U8.U16 v5 i2 
	LSU0.LDI.128.U8.U16 v1 i0 
	LSU0.LDI.128.U8.U16 v8 i2 
	LSU0.LDI.128.U8.U16 v9 i0 
	LSU0.LDi.128.U8.U16 v10 i1
nop 5

	vau.shl.x16 v19 v3 2         || CMU.ALIGNVEC v11 v2 v3 14  
		vau.shl.x16 v21 v4 2     || CMU.ALIGNVEC v14 v3 v4 14  
	vau.sub.i16 v19 v19 v1       || CMU.ALIGNVEC v12 v3 v4 2    || LSU1.LDi.128.U8.U16 v4 i1
		vau.sub.i16 v21 v21 v9   || LSU0.LDI.128.U8.U16 v1 i0    
	vau.sub.i16 v17 v19 v5 		 || LSU0.LDi.128.U8.U16 v10 i1 
		vau.sub.i16 v18 v21 v8   
	vau.sub.i16 v17 v17 v11  	 || cmu.cpvv v2 v4 			    || LSU0.LDI.128.U8.U16 v9 i0  || bru.rpl i5 i6
		vau.sub.i16 v18 v18 v14  || CMU.ALIGNVEC v15 v4 v10 2   || LSU0.LDI.128.U8.U16 v5 i2  
	vau.sub.i16 v17 v17 v12  	 || cmu.cpvv v3 v10 		    || LSU0.LDI.128.U8.U16 v8 i2  
 sLaplacian3x3_loop:	
	vau.sub.i16 v18 v18 v15     || CMU.ALIGNVEC v12 v3 v4 2         
		vau.shl.x16 v19 v3 2    || cmu.clamp0.i16 v17 v17 v20         
		vau.shl.x16 v21 v4 2    || LSU1.LDi.128.U8.U16 v4 i1
		vau.sub.i16 v19 v19 v1  || LSU0.STI.128.U16.U8 v17 i17   || LSU1.LDi.128.U8.U16 v10 i1 ||  CMU.ALIGNVEC v14 v3 v4 14//|| cmu.cpvv v6 v3
		vau.sub.i16 v21 v21 v9  ||cmu.clamp0.i16 v18 v18 v20     
		vau.sub.i16 v17 v19 v5  || LSU0.LDI.128.U8.U16 v1 i0    
	vau.sub.i16 v18 v21 v8      ||LSU0.STI.128.U16.U8 v18 i17    || CMU.ALIGNVEC v11 v2 v3 14    

	
	cmu.cmz.i8 i10
	peu.pc1c eq || bru.jmp i30
	nop 6
	
	iau.sub i0 i0 24
	iau.sub i2 i2 16
	iau.sub i1 i1 40
 sLaplacian3x3_compensate:
	LSU0.LDI.128.U8.U16 v8 i2 
	iau.sub i1 i1 8  
	LSU0.LD.128.U8.U16 v6 i1 || iau.add i1 i1 8  
	LSU0.LD.128.U8.U16 v7 i1 || iau.add i1 i1 8 
	LSU0.LDI.128.U8.U16 v9 i0 
	LSU1.LD.128.U8.U16 v10 i1  
	nop 6
	
	vau.shl.x16 v0 v7 2 
	vau.accnz.u16 v9     	
	vau.accp.u16 v0 	|| CMU.ALIGNVEC v11 v6 v7 14  
	vau.accn.u16 v8 	|| CMU.ALIGNVEC v12 v7 v10 2
	vau.accn.u16 v11 	 
	vau.accnw.u16 v13 v12  			
	nop 3
	cmu.clamp0.i16 v13 v13 v20 
	nop
	LSU0.STI.128.U16.U8 v13 i17  
	
	BRU.jmp i30
	nop 6

.size mvcvSLaplacian3x3_asm,.-mvcvSLaplacian3x3_asm
.end
	