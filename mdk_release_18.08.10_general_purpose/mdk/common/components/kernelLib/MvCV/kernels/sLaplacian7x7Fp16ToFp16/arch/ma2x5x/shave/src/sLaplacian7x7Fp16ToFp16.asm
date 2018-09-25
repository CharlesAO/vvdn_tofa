///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvSLaplacian7x7Fp16ToFp16_asm,@function
.version 00.51.04

.data .data.slaplacian7x7Fp16ToFp16
.align 16
 //sLaplacian7x7Fp16ToFp16_clampVal:		//if clamp is needed , erase comments
//        .float16 255.0

 sLaplacian7x7Fp16ToFp16_laplacian1:
        .float16 2, 4, 6, 8, 10
   //		 .float16 10, 8, 6, 4, 2
 sLaplacian7x7Fp16ToFp16_laplacian:
 	//	 .float16 7, 5, 3, 1
        .float16 1, 3, 5, 7
		
.code .text.slaplacian7x7Fp16ToFp16

//void laplacian7x7Fp16ToFp16(u8** in, u8** out, u32 inWidth)//
//					i18         i17       i16
mvcvSLaplacian7x7Fp16ToFp16_asm:


    
	//LSU0.LDIL i7 sLaplacian7x7Fp16ToFp16_clampVal		 || LSU1.LDIH i7 sLaplacian7x7Fp16ToFp16_clampVal			
	LSU0.LDIL i8 sLaplacian7x7Fp16ToFp16_laplacian  	 || LSU1.LDIH i8 sLaplacian7x7Fp16ToFp16_laplacian
	LSU0.LDIL i10 sLaplacian7x7Fp16ToFp16_laplacian1     || LSU1.LDIH i10 sLaplacian7x7Fp16ToFp16_laplacian1
	lsu0.ldil i9 sLaplacian7x7Fp16ToFp16_loop 			 || lsu1.ldih i9 sLaplacian7x7Fp16ToFp16_loop	 		
	
    LSU0.LDI.64  i0  i18  							    // || LSU1.LD.16R v21 i7  
    LSU0.LDI.64  i2  i18  								 || IAU.SUB i12 i12 i12
    LSU0.LDI.64  i4  i18  								 || IAU.SUB i13 i13 i13
    LSU0.LDI.32  i6  i18  								 || LSU1.LD.32 i17 i17 
	
	
    LSU0.LD.64.L v22 i8  || LSU1.LDO.64.H v22 i8   0x08	  	 	 
	LSU0.LD.64.L v23 i10 || LSU1.LDO.64.H v23 i10  0x08		
	
	
	IAU.SHR.u32 i11 i16 3
	
	IAU.SUB i0 i0 16			
	LSU0.LD.64.L v0  i0	 || LSU1.LDO.64.H v0  i0   0x08	 ||	IAU.ADD i0 i0 16
	LSU0.LD.64.L v1  i0	 || LSU1.LDO.64.H v1  i0   0x08  ||	IAU.SUB i1 i1 16
	LSU0.LD.64.L v3  i1	 || LSU1.LDO.64.H v3  i1   0x08	 ||	IAU.ADD i1 i1 16
	LSU0.LD.64.L v4  i1	 || LSU1.LDO.64.H v4  i1   0x08	 ||	IAU.SUB i2 i2 16
	LSU0.LD.64.L v5  i2	 || LSU1.LDO.64.H v5  i2   0x08	 ||	IAU.ADD i2 i2 16
	LSU0.LD.64.L v6  i2	 || LSU1.LDO.64.H v6  i2   0x08	 ||	IAU.SUB i3 i3 16
	LSU0.LD.64.L v8  i3	 || LSU1.LDO.64.H v8  i3   0x08	 ||	IAU.ADD i3 i3 16
	LSU0.LD.64.L v9  i3	 || LSU1.LDO.64.H v9  i3   0x08	 ||	IAU.SUB i4 i4 16
	LSU0.LD.64.L v11 i4	 || LSU1.LDO.64.H v11 i4   0x08	 ||	IAU.ADD i4 i4 16
	LSU0.LD.64.L v12 i4	 || LSU1.LDO.64.H v12 i4   0x08	 ||	IAU.SUB i5 i5 16
	LSU0.LD.64.L v14 i5	 || LSU1.LDO.64.H v14 i5   0x08	 ||	IAU.ADD i0 i0 16
	
	LSU0.LD.64.L v1  i0	 || LSU1.LDO.64.H v1  i0   0x08  || IAU.ADD i5 i5 16//!!!!!!!!!!
	
	LSU0.LD.64.L v15 i5	 || LSU1.LDO.64.H v15 i5   0x08	 ||	IAU.SUB i6 i6 16
	LSU0.LD.64.L v17 i6	 || LSU1.LDO.64.H v17 i6   0x08	 ||	IAU.ADD i6 i6 16
	LSU0.LD.64.L v18 i6	 || LSU1.LDO.64.H v18 i6   0x08	 || CMU.ALIGNVEC v20 v0 v1 10	
	 
	//NOP 6
	//BRU.SWIH 0x1f
	
	vau.macnz.f16 v23 v20  || lsu0.swzv8 [44444444] 	 || CMU.ALIGNVEC v20 v0 v1 12  			|| bru.rpl i9 i11  	 									
	vau.macn.f16  v22 v20  || lsu0.swzv8 [22222222]  	 || CMU.ALIGNVEC v20 v0 v1 14 			|| IAU.ADD i1 i1 16										
	vau.macn.f16  v23 v20  || lsu0.swzv8 [00000000] 	 || CMU.CPVV v0 v1						|| LSU1.LD.64.L v4 i1									
	vau.macn.f16  v22 v0   || lsu0.swzv8 [00000000]  	 || CMU.ALIGNVEC v20 v0 v1 2 			|| LSU1.LDO.64.H v4 i1 0x08								
	vau.macn.f16  v23 v20  || lsu0.swzv8 [00000000]  	 || CMU.ALIGNVEC v20 v0 v1 4 		
	vau.macn.f16  v22 v20  || lsu0.swzv8 [22222222]		 || CMU.ALIGNVEC v20 v0 v1 6  	   
	vau.macn.f16  v23 v20  || lsu0.swzv8 [44444444] 	 || CMU.ALIGNVEC v20 v3 v4 10 
	   
	vau.macn.f16  v22 v20  || lsu0.swzv8 [22222222]	 	 || CMU.ALIGNVEC v20 v3 v4 14			|| IAU.ADD i2 i2 16									
	vau.macp.f16  v22 v20  || lsu0.swzv8 [11111111] 	 || CMU.CPVV v3 v4						|| LSU1.LD.64.L v6 i2								
	vau.macp.f16  v23 v3   || lsu0.swzv8 [11111111] 	 || CMU.ALIGNVEC v20 v3 v4 2			|| LSU1.LDO.64.H v6 i2	0x08						
	vau.macp.f16  v22 v20  || lsu0.swzv8 [11111111] 	 || CMU.ALIGNVEC v20 v3 v4 6 
	vau.macn.f16  v22 v20  || lsu0.swzv8 [22222222]	     || CMU.ALIGNVEC v20 v5 v6 10 
	
	vau.macn.f16  v23 v20  || lsu0.swzv8 [00000000] 	 || CMU.ALIGNVEC v20 v5 v6 12													
	vau.macp.f16  v22 v20  || lsu0.swzv8 [11111111] 	 || CMU.ALIGNVEC v20 v5 v6 14		      								
	vau.macp.f16  v23 v20  || lsu0.swzv8 [22222222] 	 || CMU.CPVV v5 v6 						|| IAU.ADD i3 i3 16							
	vau.macp.f16  v22 v5   || lsu0.swzv8 [33333333]      || CMU.ALIGNVEC v20 v5 v6 2 			|| LSU1.LD.64.L v9 i3 					
	vau.macp.f16  v23 v20  || lsu0.swzv8 [22222222]      || CMU.ALIGNVEC v20 v5 v6 4            || LSU1.LDO.64.H v9 i3  0x08
	vau.macp.f16  v22 v20  || lsu0.swzv8 [11111111]      || CMU.ALIGNVEC v20 v5 v6 6 
	vau.macn.f16  v23 v20  || lsu0.swzv8 [00000000]      || CMU.ALIGNVEC v20 v8 v9 10
	 
	vau.macn.f16  v22 v20  || lsu0.swzv8 [00000000]		 || CMU.ALIGNVEC v20 v8 v9 12														
	vau.macp.f16  v23 v20  || lsu0.swzv8 [11111111] 	 || CMU.ALIGNVEC v20 v8 v9 14  															
	vau.macp.f16  v22 v20  || lsu0.swzv8 [33333333] 	 || CMU.CPVV v8 v9						|| IAU.ADD i4 i4 16							
	vau.macp.f16  v23 v8   || lsu0.swzv8 [33333333]	     || CMU.ALIGNVEC v20 v8 v9 2  			|| LSU1.LD.64.L v12 i4							
	vau.macp.f16  v22 v20  || lsu0.swzv8 [33333333] 	 || CMU.ALIGNVEC v20 v8 v9 4 			|| LSU1.LDO.64.H v12 i4 0x08
	vau.macp.f16  v23 v20  || lsu0.swzv8 [11111111] 	 || CMU.ALIGNVEC v20 v8 v9 6 
	vau.macn.f16  v22 v20  || lsu0.swzv8 [00000000] 	 || CMU.ALIGNVEC v20 v11 v12 10  
	
	vau.macn.f16  v23 v20  || lsu0.swzv8 [00000000] 	 || CMU.ALIGNVEC v20 v11 v12 12   										
	vau.macp.f16  v22 v20  || lsu0.swzv8 [11111111] 	 || CMU.ALIGNVEC v20 v11 v12 14         || IAU.ADD i5 i5 16    					
	vau.macp.f16  v23 v20  || lsu0.swzv8 [22222222] 	 || cmu.cpvv v11 v12					|| LSU1.LD.64.L v15 i5		
	vau.macp.f16  v22 v11  || lsu0.swzv8 [33333333]  	 || CMU.ALIGNVEC v20 v11 v12 2			|| LSU1.LDO.64.H v15 i5 0x08	
	vau.macp.f16  v23 v20  || lsu0.swzv8 [22222222] 	 || CMU.ALIGNVEC v20 v11 v12 4  
	vau.macp.f16  v22 v20  || lsu0.swzv8 [11111111] 	 || CMU.ALIGNVEC v20 v11 v12 6 
	vau.macn.f16  v23 v20  || lsu0.swzv8 [00000000] 	 || CMU.ALIGNVEC v20 v14 v15 10  
	
	vau.macn.f16  v22 v20  || lsu0.swzv8 [22222222]	 	 || CMU.ALIGNVEC v20 v14 v15 14    		|| IAU.ADD i6 i6 16					
	vau.macp.f16  v22 v20  || lsu0.swzv8 [11111111] 	 || CMU.CPVV v14 v15  				    || LSU1.LD.64.L v18 i6			
	vau.macp.f16  v23 v14  || lsu0.swzv8 [11111111] 	 || CMU.ALIGNVEC v20 v14 v15 2   		|| LSU1.LDO.64.H v18 i6	0x08
	vau.macp.f16  v22 v20  || lsu0.swzv8 [11111111] 	 || CMU.ALIGNVEC v20 v14 v15 6
	vau.macn.f16  v22 v20  || lsu0.swzv8 [22222222] 	 || CMU.ALIGNVEC v20 v17 v18 10  
		
	vau.macn.f16  v23 v20  || lsu0.swzv8 [44444444] 	 || CMU.ALIGNVEC v20 v17 v18 12  		
	vau.macn.f16  v22 v20  || lsu0.swzv8 [22222222] 	 || CMU.ALIGNVEC v20 v17 v18 14 
	vau.macn.f16  v23 v20  || lsu0.swzv8 [00000000] 	 || CMU.CPVV v17 v18   
	vau.macn.f16  v22 v17  || lsu0.swzv8 [00000000]  	 || CMU.ALIGNVEC v20 v17 v18 2	  
	vau.macn.f16  v23 v20  || lsu0.swzv8 [00000000] 	 || CMU.ALIGNVEC v20 v17 v18 4								                 
	vau.macn.f16  v22 v20  || lsu0.swzv8 [22222222] 	 || CMU.ALIGNVEC v20 v17 v18 6
	vau.macnw.f16 v19 v23 v20   || lsu0.swzv8 [44444444] || CMU.ALIGNVEC v20 v0 v1 10		
	
		
 	CMU.CMII.i32 i12 i13 
 	NOP//PEU.pc1c NEQ 		   || CMU.CLAMP0.F16 v19 v19 v21
sLaplacian7x7Fp16ToFp16_loop:		
 	IAU.ADD i0 i0 16	
 	PEU.pc1c NEQ		   || LSU0.ST.64.L v19 i17		 || LSU1.STO.64.H v19 i17 0x08			|| IAU.ADD i17 i17 16 	
 	IAU.ADD i12 i12 1 	
 	LSU0.LD.64.L v1  i0	  || LSU1.LDO.64.H v1  i0   0x08	
 	nop 3




	BRU.jmp i30
	//CMU.CLAMP0.F16 v19 v19 v21
	nop 2
 	LSU0.ST.64.L v19 i17		 || LSU1.STO.64.H v19 i17 0x08			|| IAU.ADD i17 i17 16 
	nop 3

.size mvcvSLaplacian7x7Fp16ToFp16_asm,.-mvcvSLaplacian7x7Fp16ToFp16_asm
.end
