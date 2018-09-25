///
/// @file
/// @copyright All code copyright Movidius Ltd 2013  all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvConvolution5x5_asm,@function
.version 00.51.05

.data .data.convolution5x5
.align 16
 convolution5x5_clampVal:
        .float16 255.0

.code .text.convolution5x5

//void Convolution5x5_asm(u8** in(i18)  u8** out(i17)  half conv[25](i16)  u32 inWidth(i15))
mvcvConvolution5x5_asm:
    
    //Load the address for all the 5 lines: i0, i1, i2, i3, i4
    //Load the kernel values: v10, v11, v12, v13    

	LSU0.LDiL i10 0x0    	|| LSU1.LDiH i10 0x0
	LSU0.LDi.64 i0 i18
	LSU0.LDi.64 i2 i18
	LSU0.LDi.32 i4 i18
	LSU0.LDiL i6 convolution5x5_clampVal    	|| LSU1.LDiH i6 convolution5x5_clampVal
	LSU0.LDiL i11 convolution5x5_forLoop    	|| LSU1.LDiH i11 convolution5x5_forLoop
	LSU0.LDO.64.L v10 i16 0x00		|| LSU1.LDO.64.H v10 i16 0x08
	LSU0.LDO.64.L v11 i16 0x10		|| LSU1.LDO.64.H v11 i16 0x18
	LSU0.LDO.64.L v12 i16 0x20		|| LSU1.LDO.64.H v12 i16 0x28 		|| IAU.SUB i0 i0 8
	LSU0.LDO.64.L v13 i16 0x30  	|| LSU1.LDi.128.u8f.f16 v0 i0 		|| IAU.SUB i1 i1 8
	LSU1.LD.32 i17 i17              || LSU0.LDi.128.u8f.f16 v1 i1 		|| IAU.SUB i2 i2 8
	IAU.SUB i3 i3 8					|| LSU0.LDi.128.u8f.f16 v2 i2   	|| LSU1.LDi.128.u8f.f16 v5 i0 
	IAU.SUB i4 i4 8					|| LSU0.LDi.128.u8f.f16 v3 i3   	|| LSU1.LDi.128.u8f.f16 v6 i1 
									   LSU0.LDi.128.u8f.f16 v4 i4   	|| LSU1.LDi.128.u8f.f16 v7 i2 
	IAU.SHR.u32 i15 i15 3 			|| LSU0.LDi.128.u8f.f16 v8 i3    
									   LSU0.LDi.128.u8f.f16 v9 i4		|| LSU1.LD.16r v22  i6
        
    
    LSU1.LDi.128.u8f.f16 v5 i0
	nop
    CMU.ALIGNVEC v20 v0 v5 12 
	NOP 2
    

    
    
        // Main Loop
        VAU.MACPZ.f16 v10  v20    	|| LSU0.SWZV8 [00000000]	|| CMU.ALIGNVEC v20 v0 v5 14  	|| LSU1.LDI.128.u8f.f16 v6 i1  || BRU.RPL i11 i15
        VAU.MACP.f16  v10  v20		|| LSU0.SWZV8 [11111111]  	|| CMU.CPVV v0 v5    			
        VAU.MACP.f16  v10  v0     	|| LSU0.SWZV8 [22222222]  	|| CMU.ALIGNVEC v20 v0 v5  2   
        VAU.MACP.f16  v10  v20    	|| LSU0.SWZV8 [33333333]  	|| CMU.ALIGNVEC v20 v0 v5  4   
        VAU.MACP.f16  v10  v20    	|| LSU0.SWZV8 [44444444]  	|| CMU.ALIGNVEC v20 v1 v6 12           				
        VAU.MACP.f16  v10  v20    	|| LSU0.SWZV8 [55555555]  	|| CMU.ALIGNVEC v20 v1 v6 14 	|| LSU1.LDI.128.u8f.f16 v7 i2
        VAU.MACP.f16  v10  v20    	|| LSU0.SWZV8 [66666666]  	|| CMU.CPVV v1 v6 
        VAU.MACP.f16  v10  v1    	|| LSU0.SWZV8 [77777777]  	|| CMU.ALIGNVEC v20 v1 v6  2 
        VAU.MACP.f16  v11  v20    	|| LSU0.SWZV8 [00000000]  	|| CMU.ALIGNVEC v20 v1 v6  4 
        VAU.MACP.f16  v11  v20    	|| LSU0.SWZV8 [11111111]  	|| CMU.ALIGNVEC v20 v2 v7 12    
        VAU.MACP.f16  v11  v20    	|| LSU0.SWZV8 [22222222]	|| CMU.ALIGNVEC v20 v2 v7 14 	|| LSU1.LDI.128.u8f.f16 v8 i3
        VAU.MACP.f16  v11  v20    	|| LSU0.SWZV8 [33333333]	|| CMU.CPVV v2 v7
        VAU.MACP.f16  v11  v2    	|| LSU0.SWZV8 [44444444]	|| CMU.ALIGNVEC v20 v2 v7  2
        VAU.MACP.f16  v11  v20    	|| LSU0.SWZV8 [55555555]	|| CMU.ALIGNVEC v20 v2 v7  4 
        VAU.MACP.f16  v11  v20    	|| LSU0.SWZV8 [66666666]	|| CMU.ALIGNVEC v20 v3 v8 12       
        VAU.MACP.f16  v11  v20    	|| LSU0.SWZV8 [77777777]  	|| CMU.ALIGNVEC v20 v3 v8 14 	|| LSU1.LDI.128.u8f.f16 v9 i4
        VAU.MACP.f16  v12  v20    	|| LSU0.SWZV8 [00000000]  	|| CMU.CPVV v3 v8
        VAU.MACP.f16  v12  v3    	|| LSU0.SWZV8 [11111111]  	|| CMU.ALIGNVEC v20 v3 v8  2 
        VAU.MACP.f16  v12  v20    	|| LSU0.SWZV8 [22222222]  	|| CMU.ALIGNVEC v20 v3 v8  4 
        VAU.MACP.f16  v12  v20    	|| LSU0.SWZV8 [33333333]  	|| CMU.ALIGNVEC v20 v4 v9 12         
        VAU.MACP.f16  v12  v20    	|| LSU0.SWZV8 [44444444]  	|| CMU.ALIGNVEC v20 v4 v9 14 
        VAU.MACP.f16  v12  v20    	|| LSU0.SWZV8 [55555555]  	|| CMU.CPVV v4 v9
        VAU.MACP.f16  v12  v4    	|| LSU0.SWZV8 [66666666]  	|| CMU.ALIGNVEC v20 v4 v9  2 
        VAU.MACP.f16  v12  v20    	|| LSU0.SWZV8 [77777777]  	|| CMU.ALIGNVEC v20 v4 v9  4       
        VAU.MACPW.f16 v21  v13  v20 || LSU0.SWZV8 [00000000] 
		CMU.CLAMP0.f16 v21 v21 v22	
		NOP		
 convolution5x5_forLoop: 
		NOP
		LSU0.ST.128.f16.u8f v21 i17 || IAU.ADD i17 i17 i10
		LSU1.LDi.128.u8f.f16 v5 i0		
		lsu0.ldil i10 8
		nop 2
		CMU.ALIGNVEC v20 v0 v5 12 
	      
		  
		  
	BRU.jmp i30
    CMU.CLAMP0.f16 v21 v21 v22
    NOP       
    LSU0.ST.128.f16.u8f v21 i17 
    NOP 3
.size mvcvConvolution5x5_asm,.-mvcvConvolution5x5_asm
.end
