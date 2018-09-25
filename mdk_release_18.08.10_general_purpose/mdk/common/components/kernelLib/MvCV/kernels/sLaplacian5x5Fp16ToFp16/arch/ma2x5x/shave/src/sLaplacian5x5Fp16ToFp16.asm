///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvSLaplacian5x5Fp16ToFp16_asm,@function
.version 00.51.04

.data .data.slaplacian5x5Fp16ToFp16
.align 16
 //sLaplacian5x5Fp16ToFp16_clampVal:		//in case of needed clamp, erase comments
   //     .float16 255.0
 
 sLaplacian5x5Fp16ToFp16_laplacian:
        .float16 17.0 , 0.0

.code .text.slaplacian5x5Fp16ToFp16

//void laplacian3x3(u8** in, u8** out, u32 inWidth)//
//					i18         i17       i16
mvcvSLaplacian5x5Fp16ToFp16_asm:

	
	LSU0.LDI.64 i0 i18	 	|| IAU.SUB i11 i11 i11
	LSU0.LDI.64 i2 i18	 	|| IAU.SUB i12 i12 i12
	LSU0.LDI.32 i4 i18  	|| IAU.ADD i11 i11 1
	
	//LSU0.LDIL i9 sLaplacian5x5Fp16ToFp16_clampVal  || LSU1.LDIH i9 sLaplacian5x5Fp16ToFp16_clampVal
	LSU0.LDIL i6 sLaplacian5x5Fp16ToFp16_laplacian || LSU1.LDIH i6 sLaplacian5x5Fp16ToFp16_laplacian
	lsu0.ldil i7 sLaplacian5x5Fp16ToFp16_loop      || lsu1.ldih i7 sLaplacian5x5Fp16ToFp16_loop
		
	LSU0.LD.32 i17 i17
	LSU0.LD.16R v20 i6							  || IAU.SHR.u32 i8 i16 3  	//|| LSU1.LD.16r v19 i9 	 			
	IAU.SUB i1 i1 16							
	IAU.SUB i2 i2 16							   
	IAU.SUB i3 i3 16
	
	 
	LSU0.LD.64.L v1  i1 				|| LSU1.LDO.64.H v1  i1 	0x08	|| 		IAU.ADD i1 i1 16
	LSU0.LD.64.L v2  i1 				|| LSU1.LDO.64.H v2  i1 	0x08	|| 		IAU.ADD i1 i1 16
	LSU0.LD.64.L v3  i1 				|| LSU1.LDO.64.H v3  i1 	0x08	|| 		IAU.ADD i1 i1 16
	LSU0.LD.64.L v4  i2 				|| LSU1.LDO.64.H v4  i2 	0x08	|| 		IAU.ADD i2 i2 16
	LSU0.LD.64.L v5  i2 				|| LSU1.LDO.64.H v5  i2 	0x08	|| 		IAU.ADD i2 i2 16
	LSU0.LD.64.L v6  i2 				|| LSU1.LDO.64.H v6  i2 	0x08	|| 		IAU.ADD i2 i2 16
	LSU0.LD.64.L v7  i3 				|| LSU1.LDO.64.H v7  i3 	0x08	|| 		IAU.ADD i3 i3 16
	LSU0.LD.64.L v8  i3 				|| LSU1.LDO.64.H v8  i3 	0x08	|| 		IAU.ADD i3 i3 16
	LSU0.LD.64.L v9  i3 				|| LSU1.LDO.64.H v9  i3 	0x08	|| 		IAU.ADD i3 i3 16	
	LSU0.LD.64.L v0  i0 				|| LSU1.LDO.64.H v0  i0 	0x08	|| 		IAU.ADD i0 i0 16	
	LSU0.LD.64.L v10 i4 				|| LSU1.LDO.64.H v10 i4 	0x08	|| 		IAU.ADD i4 i4 16	
	
	
    
    
    vau.mul.f16 v15 v5 v20 				|| CMU.ALIGNVEC v14 v4 v5 14 		|| bru.rpl i7 i8
	vau.mul.f16 v12 v2 2.0				|| CMU.ALIGNVEC v16 v5 v6 2
	vau.mul.f16 v14 v14 2.0				|| CMU.ALIGNVEC v21 v4 v5 12	
nop	
	vau.mul.f16 v16 v16 2.0				|| CMU.ALIGNVEC v22 v5 v6 4			
	vau.mul.f16 v18 v8 2.0			    || CMU.ALIGNVEC v11 v1 v2 14		
//	cmu.clamp0.f16 v23 v23 v19
//	nop
	vau.accnz.f16 v0					|| CMU.ALIGNVEC v13 v2 v3 2			
	vau.accn.f16 v10   	    			|| CMU.ALIGNVEC v17 v7 v8 14		
	vau.accp.f16 v15  					|| CMU.ALIGNVEC v0 v8 v9 2
	vau.accn.f16 v12   					|| cmu.cpvv v1 v2  		 	|| LSU0.LD.64.L v10 i4 	|| LSU1.LDO.64.H v10 i4 0x08 || 	IAU.ADD i4 i4 16					
	vau.accn.f16 v18				    || cmu.cpvv v2 v3  			|| LSU0.LD.64.L v3  i1	|| LSU1.LDO.64.H v3  i1 0x08 || 	IAU.ADD i1 i1 16  		
	vau.accn.f16 v14					|| cmu.cpvv v4 v5			|| LSU0.LD.64.L v6  i2 	|| LSU1.LDO.64.H v6  i2 0x08 || 	IAU.ADD i2 i2 16
	vau.accn.f16 v16 					|| cmu.cpvv v5 v6 			|| LSU0.LD.64.L v9  i3	|| LSU1.LDO.64.H v9  i3 0x08 || 	IAU.ADD i3 i3 16
 sLaplacian5x5Fp16ToFp16_loop:			
.nowarn 9 
	vau.accn.f16 v21 					|| cmu.cpvv v7 v8			|| LSU0.LD.64.L v0  i0	|| LSU1.LDO.64.H v0  i0 0x08 || 	IAU.ADD i0 i0 16
.nowarnend
	vau.accn.f16 v22   					|| cmu.cpvv v8 v9  
	vau.accn.f16 v11   					 	    
	vau.accn.f16 v13 					|| IAU.ADD i12 i12 1	
	vau.accn.f16 v17   					     
	vau.accnw.f16 v23 v0  				|| CMU.CMII.i32 i11 i12
	PEU.pc1c NEQ 						|| LSU0.ST.64.L v23 i17					|| LSU1.STO.64.H v23 i17 0x08	|| IAU.ADD i17 i17 16
			
	
	nop 3
	BRU.jmp i30
	nop 2
	//cmu.clamp0.f16 v23 v23 v19
	//nop
	LSU0.ST.64.L v23 i17					|| LSU1.STO.64.H	v23 i17 0x08	|| IAU.ADD i17 i17 16
	nop 3
	

    


.size mvcvSLaplacian5x5Fp16ToFp16_asm,.-mvcvSLaplacian5x5Fp16ToFp16_asm
.end
