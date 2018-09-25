///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvSLaplacian7x7_asm,@function
.version 00.51.04

.data .data.slaplacian7x7
.align 16
 sLaplacian7x7_clampVal:
        .float16 255.0
 sLaplacian7x7_laplacian:
        .short 3, 5, 6, 7, 10
 sLaplacian7x7_laplacian1:
        .short 1, 2, 4, 8 

		
.code .text.slaplacian7x7

//void laplacian7x7(u8** in, u8** out, u32 inWidth)//
//					i18         i17       i16
mvcvSLaplacian7x7_asm:

	IAU.SUB i19 i19 8
	LSU0.ST.64.L v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v25  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v25  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v26  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v26  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v27  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v27  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v28  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v28  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v29  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v29  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v30  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v30  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v31  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v31  i19  

	LSU0.LDIL i7 sLaplacian7x7_clampVal || LSU1.LDIH i7 sLaplacian7x7_clampVal
	LSU0.LDIL i8 sLaplacian7x7_laplacian || LSU1.LDIH i8 sLaplacian7x7_laplacian
	LSU0.LDIL i10 sLaplacian7x7_laplacian1 || LSU1.LDIH i10 sLaplacian7x7_laplacian1
	lsu0.ldil i9 sLaplacian7x7_loop || lsu1.ldih i9 sLaplacian7x7_loop
	
    LSU0.LD.32  i0  i18 || IAU.ADD i18 i18 4 
    LSU0.LD.32  i1  i18 || IAU.ADD i18 i18 4
    LSU0.LD.32  i2  i18 || IAU.ADD i18 i18 4 
    LSU0.LD.32  i3  i18 || IAU.ADD i18 i18 4 
    LSU0.LD.32  i4  i18 || IAU.ADD i18 i18 4 
    LSU0.LD.32  i5  i18 || IAU.ADD i18 i18 4 
    LSU0.LD.32  i6  i18 || IAU.ADD i18 i18 4 
	LSU0.LD.32  i17  i17
	nop
	LSU0.LD.16R v21 i7 || LSU1.LD.64.L v22 i8 || iau.add i8 i8 8
	LSU0.LD.64.H v22 i8 || LSU1.LD.64.L v23 i10 || iau.add i10 i10 8
	LSU0.LD.64.H v23 i10
	IAU.SHR.u32 i11 i16 3 
	
		iau.sub i0 i0 8
		iau.sub i1 i1 8
		iau.sub i2 i2 8
		iau.sub i3 i3 8
		iau.sub i4 i4 8
		iau.sub i5 i5 8
		iau.sub i6 i6 8
		
	LSU0.LDi.128.U8.U16 v0 i0 
	LSU0.LDi.128.U8.U16 v1 i0  
	LSU0.LDi.128.U8.U16 v2 i0 
	LSU0.LDi.128.U8.U16 v3 i1  
	LSU0.LDi.128.U8.U16 v4 i1  
	LSU0.LDi.128.U8.U16 v30 i1
	LSU0.LDi.128.U8.U16 v8 i3 
	LSU0.LDi.128.U8.U16 v9 i3  
	LSU0.LDi.128.U8.U16 v5 i2  
	LSU0.LDi.128.U8.U16 v6 i2
	
	LSU0.LDi.128.U8.U16 v11 i4   
	LSU0.LDi.128.U8.U16 v12 i4 
	LSU0.LDi.128.U8.U16 v14 i5 
	LSU0.LDi.128.U8.U16 v15 i5  
	LSU0.LDi.128.U8.U16 v17 i6  
	LSU0.LDi.128.U8.U16 v18 i6 
	LSU0.LD.128.U8.U16 v7 i2  || iau.sub i2 i2 8
	LSU0.LD.128.U8.U16 v10 i3 || iau.sub i3 i3 8
	LSU0.LD.128.U8.U16 v13 i4 || iau.sub i4 i4 8 
	LSU0.LD.128.U8.U16 v16 i5 || iau.sub i5 i5 8  
	LSU0.LD.128.U8.U16 v19 i6 || iau.sub i6 i6 8  
	
	vau.macnz.u16 v23 v1  || lsu0.swzv8 [00000000] || CMU.ALIGNVEC v20 v0 v1 10  	
	vau.macn.u16  v22 v20 || lsu0.swzv8 [44444444] || CMU.ALIGNVEC v24 v0 v1 12 
	vau.macn.u16  v22 v24 || lsu0.swzv8 [11111111] || CMU.ALIGNVEC v25 v0 v1 14
	vau.macn.u16  v23 v25 || lsu0.swzv8 [11111111] || CMU.ALIGNVEC v26 v1 v2 2 
	vau.macn.u16  v23 v26 || lsu0.swzv8 [11111111] || CMU.ALIGNVEC v27 v1 v2 4 
	
	vau.macn.u16 v22 v27 || lsu0.swzv8 [11111111] || CMU.ALIGNVEC v28 v1 v2 6     
	vau.macn.u16 v22 v28 || lsu0.swzv8 [44444444] || CMU.ALIGNVEC v20 v3 v4 10    || bru.rpl i9 i11
	vau.macn.u16 v22 v20 || lsu0.swzv8 [11111111] || CMU.ALIGNVEC v25 v3 v4 14
	vau.macp.u16 v22 v25 || lsu0.swzv8 [00000000] || cmu.cpvv v0 v1
	vau.macp.u16 v23 v4  || lsu0.swzv8 [22222222] || CMU.ALIGNVEC v26 v4 v30 2
	vau.macp.u16 v22 v26 || lsu0.swzv8 [00000000] || CMU.ALIGNVEC v28 v4 v30 6
	vau.macn.u16 v22 v28 || lsu0.swzv8 [11111111] || CMU.ALIGNVEC v20 v5 v6 10 
	vau.macn.u16 v23 v20 || lsu0.swzv8 [11111111] || CMU.ALIGNVEC v24 v5 v6 12 
	vau.macp.u16 v22 v24 || lsu0.swzv8 [00000000] || CMU.ALIGNVEC v25 v5 v6 14
	vau.macp.u16 v22 v25 || lsu0.swzv8 [22222222] || cmu.cpvv v1 v2 		      || LSU1.LDi.128.U8.U16 v2 i0 
	vau.macp.u16 v22 v6  || lsu0.swzv8 [33333333] || CMU.ALIGNVEC v26 v6 v7 2 
	vau.macp.u16 v22 v26 || lsu0.swzv8 [22222222] || CMU.ALIGNVEC v27 v6 v7 4 
	vau.macp.u16 v22 v27 || lsu0.swzv8 [00000000] || CMU.ALIGNVEC v28 v6 v7 6  
	vau.macn.u16 v23 v28 || lsu0.swzv8 [11111111] || CMU.ALIGNVEC v20 v8 v9 10 
	vau.macn.u16 v23 v20 || lsu0.swzv8 [00000000] || CMU.ALIGNVEC v24 v8 v9 12 
	vau.macp.u16 v23 v24 || lsu0.swzv8 [22222222] || CMU.ALIGNVEC v25 v8 v9 14
	vau.macp.u16 v22 v25 || lsu0.swzv8 [33333333] || cmu.cpvv v3 v4
	vau.macp.u16 v23 v9  || lsu0.swzv8 [33333333] || CMU.ALIGNVEC v26 v9 v10 2    || LSU1.LDi.128.U8.U16 v8 i3
	vau.macp.u16 v22 v26 || lsu0.swzv8 [33333333] || CMU.ALIGNVEC v27 v9 v10 4    || LSU1.LDi.128.U8.U16 v9 i3
	vau.macp.u16 v23 v27 || lsu0.swzv8 [22222222] || CMU.ALIGNVEC v28 v9 v10 6 
	vau.macn.u16 v23 v28 || lsu0.swzv8 [00000000] || CMU.ALIGNVEC v20 v11 v12 10 
	vau.macn.u16 v23 v20 || lsu0.swzv8 [11111111] || CMU.ALIGNVEC v24 v11 v12 12  || LSU1.LDi.128.U8.U16 v11 i4
	vau.macp.u16 v22 v24 || lsu0.swzv8 [00000000] || CMU.ALIGNVEC v25 v11 v12 14  || LSU1.LD.128.U8.U16 v10 i3   || iau.sub i3 i3 8
	vau.macp.u16 v22 v25 || lsu0.swzv8 [22222222] || cmu.cpvv v4 v30              || LSU1.LDi.128.U8.U16 v30 i1 
	vau.macp.u16 v22 v12 || lsu0.swzv8 [33333333] || CMU.ALIGNVEC v26 v12 v13 2   || LSU1.LDi.128.U8.U16 v12 i4 
	vau.macp.u16 v22 v26 || lsu0.swzv8 [22222222] || CMU.ALIGNVEC v27 v12 v13 4 
	vau.macp.u16 v22 v27 || lsu0.swzv8 [00000000] || CMU.ALIGNVEC v28 v12 v13 6   || LSU1.LD.128.U8.U16 v13 i4   || iau.sub i4 i4 8 
	vau.macn.u16 v23 v28 || lsu0.swzv8 [11111111] || CMU.ALIGNVEC v20 v14 v15 10 
	vau.macn.u16 v22 v20 || lsu0.swzv8 [11111111] || CMU.ALIGNVEC v25 v14 v15 14  || LSU1.LDi.128.U8.U16 v14 i5 
	vau.macp.u16 v22 v25 || lsu0.swzv8 [00000000] || CMU.ALIGNVEC v26 v15 v16 2   || LSU1.LDi.128.U8.U16 v15 i5  
	vau.macp.u16 v23 v15 || lsu0.swzv8 [22222222] || CMU.ALIGNVEC v28 v15 v16 6   || LSU1.LDi.128.U8.U16 v17 i6 
	vau.macp.u16 v22 v26 || lsu0.swzv8 [00000000] || CMU.ALIGNVEC v20 v17 v18 10  || LSU1.LD.128.U8.U16 v16 i5   || iau.sub i5 i5 8  
	vau.macn.u16 v22 v28 || lsu0.swzv8 [11111111] || CMU.ALIGNVEC v24 v17 v18 12
	vau.macn.u16 v22 v20 || lsu0.swzv8 [44444444] || CMU.ALIGNVEC v25 v17 v18 14  || LSU1.LDi.128.U8.U16 v18 i6 	
	vau.macn.u16 v22 v24 || lsu0.swzv8 [11111111] || CMU.ALIGNVEC v26 v18 v19 2   || LSU1.LDi.128.U8.U16 v5 i2
	vau.macn.u16 v23 v25 || lsu0.swzv8 [11111111] || CMU.ALIGNVEC v27 v18 v19 4   || LSU1.LDi.128.U8.U16 v6 i2
	vau.macn.u16 v23 v18 || lsu0.swzv8 [00000000] || CMU.ALIGNVEC v28 v18 v19 6   || LSU1.LD.128.U8.U16 v19 i6   || iau.sub i6 i6 8  
	vau.macn.u16 v23 v26 || lsu0.swzv8 [11111111] || CMU.ALIGNVEC v20 v0 v1 10	  || LSU1.LD.128.U8.U16 v7 i2 || iau.sub i2 i2 8 
	vau.macn.u16 v22 v27 || lsu0.swzv8 [11111111] || CMU.ALIGNVEC v24 v0 v1 12								                 
 sLaplacian7x7_loop:
 .nowarn 10
	vau.macnw.u16 v31 v22 v28 || lsu0.swzv8 [44444444] || CMU.ALIGNVEC v25 v0 v1 14
.nowarnend
	vau.macnz.u16 v23 v1  || lsu0.swzv8 [00000000] || CMU.ALIGNVEC v26 v1 v2 2
		vau.macn.u16  v22 v20 || lsu0.swzv8 [44444444] || CMU.ALIGNVEC v27 v1 v2 4
		vau.macn.u16  v22 v24 || lsu0.swzv8 [11111111] 
		vau.macn.u16  v23 v25 || lsu0.swzv8 [11111111] || cmu.clamp0.i16 v28 v31 v21 
		vau.macn.u16 v23 v26  || lsu0.swzv8 [11111111]  //||
		vau.macn.u16 v22 v27 || lsu0.swzv8 [11111111] || CMU.ALIGNVEC v28 v1 v2 6 || LSU1.STi.128.U16.U8 v28 i17  
	
	
	LSU0.LD.64.H  v31  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v31  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v30  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v30  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v29  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v29  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v28  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v28  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v27  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v27  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v26  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v26  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v25  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v25  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v24  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v24  i19 || IAU.ADD i19 i19 8
	
    BRU.jmp i30
	nop 6

.size mvcvSLaplacian7x7_asm,.-mvcvSLaplacian7x7_asm
.end
