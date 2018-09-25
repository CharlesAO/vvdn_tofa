///
/// @file
/// @copyright All code copyright Movidius Ltd 2013  all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvConvolution5x5Fp16ToFp16_asm,@function
.version 00.51.05

.data .data.convolution5x5Fp16ToFp16
.align 4

//.align 16
//___clampVal:
//        .float16 255.0

.code .text.convolution5x5Fp16ToFp16

//void Convolution5x5_asm(u8** in(i18)  u8** out(i17)  half conv[25](i16)  u32 inWidth(i15))
mvcvConvolution5x5Fp16ToFp16_asm:
    
    //Load the address for all the 5 lines: i0, i1, i2, i3, i4
    //Load the kernel values: v10, v11, v12, v13    
    

	LSU0.LDi.64 i0 i18              || IAU.SUB i5 i5 i5	
	LSU0.LDi.64 i2 i18              || IAU.SUB i7 i7 i7
	LSU0.LDi.32 i4 i18

	LSU0.LDiL i11 ___forLoop    	|| LSU1.LDiH i11 ___forLoop
	
	LSU0.LDO.64.L v10 i16 0x00	 	|| LSU1.LDO.64.H v10 i16 0x08 	
	LSU0.LDO.64.L v11 i16 0x10		|| LSU1.LDO.64.H v11 i16 0x18 
	LSU0.LDO.64.L v12 i16 0x20		|| LSU1.LDO.64.H v12 i16 0x28 		 || IAU.SHR.u32 i15 i15 3 		
	LSU0.LDO.64.L v13 i16 0x30  	|| LSU1.LD.32 i17 i17		  		 || IAU.SUB i0 i0 16 

	LSU0.LD.64.L v0 i0   	        || LSU1.LDO.64.H v0 i0 0x08	 		 || IAU.SUB i1 i1 16	       
	LSU0.LD.64.L v1 i1       	    || LSU1.LDO.64.H v1 i1 0x08	 		 || IAU.SUB i2 i2 16	
	LSU0.LD.64.L v2 i2          	|| LSU1.LDO.64.H v2 i2 0x08  		 || IAU.SUB i3 i3 16		
	LSU0.LD.64.L v3 i3 	        	|| LSU1.LDO.64.H v3 i3 0x08			 || IAU.SUB i4 i4 16 				
	LSU0.LD.64.L v4 i4     		    || LSU1.LDO.64.H v4 i4 0x08   		 || IAU.ADD i0 i0 16   
    LSU0.LD.64.L v5 i0 				|| LSU1.LDO.64.H v5 i0 0x08   		 || IAU.ADD i1 i1 16 
	LSU0.LD.64.L v6 i1    		    || LSU1.LDO.64.H v6 i1 0x08   		 || IAU.ADD i2 i2 16
	LSU0.LD.64.L v7 i2	 			|| LSU1.LDO.64.H v7 i2 0x08   		 || IAU.ADD i3 i3 16
	LSU0.LD.64.L v8 i3 				|| LSU1.LDO.64.H v8 i3 0x08   		 || IAU.ADD i4 i4 16
	LSU0.LD.64.L v9 i4   			|| LSU1.LDO.64.H v9 i4 0x08   		 || IAU.ADD i0 i0 16
	LSU0.LD.64.L v15 i0   			|| LSU1.LDO.64.H v15 i0 0x08  		 || IAU.ADD i1 i1 16
	LSU0.LD.64.L v16 i1   			|| LSU1.LDO.64.H v16 i1 0x08    	 || IAU.ADD i2 i2 16        
	LSU0.LD.64.L v17 i2     		|| LSU1.LDO.64.H v17 i2 0x08  		 || IAU.ADD i3 i3 16										 	
	LSU0.LD.64.L v18 i3  			|| LSU1.LDO.64.H v18 i3 0x08  		 || IAU.ADD i4 i4 16 
	LSU0.LD.64.L v19 i4      		|| LSU1.LDO.64.H v19 i4 0x08  		 || CMU.ALIGNVEC v20 v0 v5 12   
	      
        // Main Loop
        VAU.MACPZ.f16 v10  v20    	|| LSU0.SWZV8 [00000000]			 || CMU.ALIGNVEC v20 v0 v5  14      || BRU.RPL i11 i15
        VAU.MACP.f16  v10  v20		|| LSU0.SWZV8 [11111111]  			 || CMU.CPVV v0 v5    		   
        VAU.MACP.f16  v10  v0     	|| LSU0.SWZV8 [22222222]  		     || CMU.ALIGNVEC v20 v0 v15  2   
        VAU.MACP.f16  v10  v20    	|| LSU0.SWZV8 [33333333]  			 || CMU.ALIGNVEC v20 v0 v15  4  
        VAU.MACP.f16  v10  v20    	|| LSU0.SWZV8 [44444444]  			 || CMU.ALIGNVEC v20 v1 v6  12     				
        VAU.MACP.f16  v10  v20    	|| LSU0.SWZV8 [55555555]  			 || CMU.ALIGNVEC v20 v1 v6  14  
        VAU.MACP.f16  v10  v20    	|| LSU0.SWZV8 [66666666]  			 || CMU.CPVV v1 v6 			         
        VAU.MACP.f16  v10  v1    	|| LSU0.SWZV8 [77777777]  			 || CMU.ALIGNVEC v20 v1 v16  2 
        VAU.MACP.f16  v11  v20    	|| LSU0.SWZV8 [00000000]  			 || CMU.ALIGNVEC v20 v1 v16  4 
        VAU.MACP.f16  v11  v20    	|| LSU0.SWZV8 [11111111]  			 || CMU.ALIGNVEC v20 v2 v7  12    
        VAU.MACP.f16  v11  v20    	|| LSU0.SWZV8 [22222222]			 || CMU.ALIGNVEC v20 v2 v7  14 
        VAU.MACP.f16  v11  v20    	|| LSU0.SWZV8 [33333333]			 || CMU.CPVV v2 v7				
        VAU.MACP.f16  v11  v2    	|| LSU0.SWZV8 [44444444]			 || CMU.ALIGNVEC v20 v2 v17  2
        VAU.MACP.f16  v11  v20    	|| LSU0.SWZV8 [55555555]			 || CMU.ALIGNVEC v20 v2 v17  4 
        VAU.MACP.f16  v11  v20    	|| LSU0.SWZV8 [66666666]			 || CMU.ALIGNVEC v20 v3 v8  12    
        VAU.MACP.f16  v11  v20    	|| LSU0.SWZV8 [77777777]  			 || CMU.ALIGNVEC v20 v3 v8  14 	
        VAU.MACP.f16  v12  v20    	|| LSU0.SWZV8 [00000000]  			 || CMU.CPVV v3 v8
        VAU.MACP.f16  v12  v3    	|| LSU0.SWZV8 [11111111]  			 || CMU.ALIGNVEC v20 v3 v18  2 
        VAU.MACP.f16  v12  v20    	|| LSU0.SWZV8 [22222222]  			 || CMU.ALIGNVEC v20 v3 v18  4 
        VAU.MACP.f16  v12  v20    	|| LSU0.SWZV8 [33333333]  			 || CMU.ALIGNVEC v20 v4 v9  12         
        VAU.MACP.f16  v12  v20    	|| LSU0.SWZV8 [44444444]  			 || CMU.ALIGNVEC v20 v4 v9  14 
        VAU.MACP.f16  v12  v20    	|| LSU0.SWZV8 [55555555]  			 || CMU.CPVV v4 v9
        VAU.MACP.f16  v12  v4    	|| LSU0.SWZV8 [66666666]  			 || CMU.ALIGNVEC v20 v4 v19  2 
        VAU.MACP.f16  v12  v20    	|| LSU0.SWZV8 [77777777]  			 || CMU.ALIGNVEC v20 v4 v19  4       
        VAU.MACPW.f16 v21  v13  v20 || LSU0.SWZV8 [00000000]    		 || CMU.CPVV v5 v15 				 || IAU.ADD i0 i0 16
        
		LSU0.LDO.64.L v15 i0 0x00   || LSU1.LDO.64.H v15 i0 0x08 		 || IAU.ADD i1 i1 16              	 || CMU.CPVV v6 v16  
		LSU0.LDO.64.L v16 i1 0x00   || LSU1.LDO.64.H v16 i1 0x08 		 || IAU.ADD i2 i2 16		     	 || CMU.CPVV v7 v17
___forLoop:		
.nowarn 10
		LSU0.LDO.64.L v17 i2 0x00   || LSU1.LDO.64.H v17 i2 0x08 		 || IAU.ADD i3 i3 16			 	 || CMU.CPVV v8 v18
.nowarnend
		LSU0.LDO.64.L v18 i3 0x00   || LSU1.LDO.64.H v18 i3 0x08 		 || IAU.ADD i4 i4 16			 	 || CMU.CPVV v9 v19
		LSU0.LDO.64.L v19 i4 0x00   || LSU1.LDO.64.H v19 i4 0x08 								  	
		CMU.CMII.i32 i5 i7
	 	peu.pc1c NEQ				|| LSU0.ST.64.L v21 i17		 		 || LSU1.STO.64.H v21 i17 0x08       || IAU.ADD i17 i17 16   
	 	IAU.ADD i5 i5 1             || CMU.ALIGNVEC v20 v0 v5 12 
		NOP
	
		
		
	BRU.jmp i30
   		
   	LSU0.ST.64.L v21 i17		|| LSU1.STO.64.H v21 i17 0x08 || IAU.ADD i17 i17 16 
	nop 5
   		
    
    
.size mvcvConvolution5x5Fp16ToFp16_asm,.-mvcvConvolution5x5Fp16ToFp16_asm
.end
