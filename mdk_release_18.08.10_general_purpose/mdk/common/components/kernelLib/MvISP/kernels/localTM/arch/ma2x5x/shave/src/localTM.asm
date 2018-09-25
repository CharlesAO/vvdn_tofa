///
/// @file  localTM.asm
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief	 Applies a tone mapping function to the Luma channel.  
///            The mapping function is based on both the original Luma value, 
///            plus a second local brightness input image 
///            Local Tone Mapping increases contrast locally, applying a contrast 
///            curve that is adapted to the local image intensity
///

.type mvispLocalTM_asm,@function
.version 00.51.04

.data .data.localTM
.align 16
 localTM_vect:                                                                       //wt[4][4] = {
        .float16 0.000732421875, 0.002197265625, 0.000244140625, 0.000732421875    //{ 3.0/4096, 9.0/4096, 1.0/4096, 3.0/4096 },
        .float16 0.002197265625, 0.000732421875, 0.000732421875, 0.000244140625   //{ 9.0/4096, 3.0/4096, 3.0/4096, 1.0/4096 },
        .float16 0.000244140625, 0.000732421875, 0.000732421875, 0.002197265625   //{ 1.0/4096, 3.0/4096, 3.0/4096, 9.0/4096 },
        .float16 0.000732421875, 0.000244140625, 0.002197265625, 0.000732421875   //{ 3.0/4096, 1.0/4096, 9.0/4096, 3.0/4096 },
		.float16 1, 8, 10, 14 //20

.code .text.localTM

//void localTM_asm(half** luma_in, u8** bg8, half** output, half *curves, u32 width, u32 run_no)//
//					i18              i17         i16           i15           i14       i13
mvispLocalTM_asm:
.nowarn 9,11,12,10
	IAU.SUB i19 i19 4
	LSU0.ST.32  i20  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i21  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i22  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i23  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i24  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i25  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i26  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i27  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i28  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i29  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i31  i19 ||lsu1.ldil i31, 0x3C00	


	IAU.SUB i19 i19 8
	LSU0.ST.64.l v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v25  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v25  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v26  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v26  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v27  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v27  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v28  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v28  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v29  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v29  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v30  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v30  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v31  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v31  i19  
	
	//iau.sub i24 i24 i24
	//LSU0.ldil i25, 0x2 || LSU1.ldih i25, 0x0   
IAU.SHR.u32 i14 i14 3
iau.sub i14, i14, 2
LSU0.ldil i9, localTM_loop || LSU1.ldih i9, localTM_loop    
LSU0.ldil i11, localTM_vect || LSU1.ldih i11, localTM_vect     
LSU0.ldil  i0, 0x14 || LSU1.ldih i0, 0x0000
LSU0.ldil i10, 0x1 || LSU1.ldih i10, 0x0   ||cmu.cpivr.x16 v30, i31  

LSU0.LDO.64.L v23 i11 0 || LSU1.LDO.64.H v23 i11 8 || IAU.ADD i11 i11 16  
LSU0.LDO.64.L v22 i11 0 || LSU1.LDO.64.H v22 i11 8 || IAU.ADD i11 i11 16  
LSU0.LDO.64.L v21 i11 0 


LSU0.LD.32  i18  i18 //luma_in fp16
LSU0.LD.32  i11  i17 || IAU.ADD i17 i17 4
LSU0.LD.32  i8   i17
LSU0.LD.32  i16  i16

IAU.AND i12 i13 i10
CMU.CMZ.i32  i12
peu.pc1c neq || cmu.cpvv v23 v22      
CMU.CPIV.x16 v21.4 i0.l

IAU.SUB i11 i11 8
IAU.SUB i8  i8  8

LSU0.LD.128.u8.f16  v0 i11 || IAU.ADD i11 i11 8
LSU0.LD.128.u8.f16  v1 i8  || IAU.ADD i8  i8  8


LSU0.LDO.64.L v4 i18 0 || LSU1.LDO.64.H v4 i18 8 || IAU.ADD i18 i18 16 
LSU0.LD.128.u8.f16  v2 i11 || IAU.ADD i11 i11 4//bgidx[0] + 1
LSU0.LD.128.u8.f16  v3 i8  || IAU.ADD i8  i8  4//bgidx[1] + 1
nop 4
cmu.clamp0.f16 v4, v4, v30
nop
cmu.alignvec v6 v0 v2 14 //bgidx[0]
cmu.alignvec v7 v1 v3 14 //bgidx[1]
vau.mul.f16 v8  v23 v6 || lsu0.swzv8 [00000000]     
vau.mul.f16 v9  v23 v2 || lsu0.swzv8 [11111111]        
vau.mul.f16 v10  v23 v7 || lsu0.swzv8 [22222222]     
vau.mul.f16 v11  v23 v3 || lsu0.swzv8 [33333333]     
vau.add.f16 v8 v9 v8 || cmu.alignvec v23 v23 v23 8 
vau.mul.f16 v9  v23 v6 || lsu0.swzv8 [00000000]     
vau.add.f16 v6 v10 v11                                               
vau.mul.f16 v7  v23 v2 || lsu0.swzv8 [11111111]    
vau.mul.f16 v10  v23 v7 || lsu0.swzv8 [22222222]    
vau.add.f16 v11 v8 v6    || cmu.alignvec v2 v2 v2 8   
vau.add.f16 v6 v9 v7 || cmu.cpvv v0 v2            
vau.mul.f16 v7  v23 v3 || lsu0.swzv8 [33333333]                                    || LSU1.LDO.64.H v4 i18 8 
vau.mul.f16 v8 v21 v4   || lsu0.swzv8 [33333333]      || cmu.alignvec v3 v3 v3 8   || LSU1.LDO.64.L v4 i18 0 || IAU.ADD i18 i18 16  
vau.mul.f16 v9 v21 v11   || lsu0.swzv8 [11111111]      || cmu.cpvv v1 v3           || LSU1.LD.128.u8.f16  v2 i11 || IAU.ADD i11 i11 4//bgidx[0] + 1  
vau.add.f16 v10 v10 v7 || cmu.alignvec v23 v23 v23 8                               || LSU1.LD.128.u8.f16  v3 i8  || IAU.ADD i8  i8  4//bgidx[1] + 1
CMU.CPVV.f16.i16s v12  v8                            
CMU.CPVV.f16.i16s v15  v9                               || IAU.SUB  i7  i7  i7
vau.add.f16 v14 v6 v10 || cmu.cpvv.i16.f16  v16 v12     || IAU.SUB  i6  i6  i6  
CMU.CPVV.i16.f16  v15  v15                               || IAU.SUB  i5  i5  i5
cmu.clamp0.f16 v4, v4, v30
nop
vau.mul.f16 v17 v21 v14  || lsu0.swzv8 [11111111]                                      || IAU.SUB  i4  i4  i4 || cmu.alignvec v6 v0 v2 14 //bgidx[0]                                                 
vau.sub.f16 v18 v8 v16                                  || IAU.SUB  i3  i3  i3                                || cmu.alignvec v7 v1 v3 14 //bgidx[1]     
vau.sub.f16 v19 v9 v15                                  || IAU.SUB  i2  i2  i2
                                                                                                                 vau.mul.f16 v8  v23 v6 || lsu0.swzv8 [00000000]                                                          
vau.sub.f16 v20 v21 v18     || lsu0.swzv8 [00000000]                                  || IAU.SUB  i1  i1  i1
cmu.alignvec v12 v17 v17 2                               || IAU.SUB  i0  i0  i0                               || vau.mul.f16 v9  v23 v2 || lsu0.swzv8 [11111111]  
CMU.CPVV.f16.i16s v17  v12                               || IAU.SUB  i27  i27  i27                            || vau.mul.f16 v10  v23 v7 || lsu0.swzv8 [22222222]    
                                                                                                                 vau.mul.f16 v11  v23 v3 || lsu0.swzv8 [33333333]                                                          
vau.mul.f16 v14 v21 v16   || lsu0.swzv8 [22222222]      || CMU.CPVV.i16.f16  v17  v17   || IAU.SUB  i26  i26  i26
                                                                                                                 vau.add.f16 v8 v9 v8                                               
vau.sub.f16 v16 v12 v17                                   || IAU.SUB  i25  i25  i25                            || cmu.alignvec v23 v23 v23 8   
                                                                                                                  vau.mul.f16 v9  v23 v6 || lsu0.swzv8 [00000000]                                                         
                                                            IAU.SUB  i24  i24  i24                             || vau.add.f16 v6 v10 v11       
CMU.VILV.x16 v17 v12 v17 v15                             || IAU.SUB  i23  i23  i23                             || vau.mul.f16 v7  v23 v2 || lsu0.swzv8 [11111111]   
CMU.VILV.x16 v16 v15 v16 v19                              || IAU.SUB  i22  i22  i22                            || vau.mul.f16 v10  v23 v7 || lsu0.swzv8 [22222222]   
vau.sub.f16 v19 v21 v16   || lsu0.swzv8 [00000000]                                    || IAU.SUB  i21  i21  i21
vau.add.f16  v12 v14 v17                                  
                                                                                                               vau.add.f16 v11 v8 v6                                            
vau.mul.f16 v15 v20 v19                                  || IAU.SUB  i20  i20  i20                             || cmu.alignvec v2 v2 v2 8     
vau.mul.f16 v17 v20 v16                                                                                        || cmu.cpvv v0 v2
vau.mul.f16 v19 v18 v19  
vau.mul.f16 v16 v18 v16  
CMU.CPVV.f16.i16s  v12   v12                                                                                   || vau.add.f16 v6 v9 v7  
nop
VAU.ADD.i16 v22 v12 v12
                                                                                                                 vau.mul.f16 v7  v23 v3 || lsu0.swzv8 [33333333]                                    || LSU1.LDO.64.H v4 i18 8 
                                                                                                                 vau.mul.f16 v8 v21 v4   || lsu0.swzv8 [33333333]      || cmu.alignvec v3 v3 v3 8   || LSU1.LDO.64.L v4 i18 0 || IAU.ADD i18 i18 16  
                                                                                                                 vau.mul.f16 v9 v21 v11   || lsu0.swzv8 [11111111]      || cmu.cpvv v1 v3           || LSU1.LD.128.u8.f16  v2 i11 || IAU.ADD i11 i11 4//bgidx[0] + 1  
                                                                                                                 vau.add.f16 v10 v10 v7 || cmu.alignvec v23 v23 v23 8                               || LSU1.LD.128.u8.f16  v3 i8  || IAU.ADD i8  i8  4//bgidx[1] + 1
							
cmu.cpvv v13 v15  
CMU.CPVV.f16.i16s v12  v8  
CMU.CPVV.f16.i16s v15  v9  
cmu.cpvv.i16.f16  v11 v12
CMU.CPVV.i16.f16  v15  v15 
CMU.CPVI.x16  i0.l  v22.0 || vau.add.i16 v26 v21 v22 || lsu0.swzv8 [44444444]  
cmu.alignvec v6 v0 v2 14 || vau.add.f16 v14 v6 v10 
cmu.alignvec v7 v1 v3 14 //bgidx[1]   
cmu.cpvv v28 v19      
vau.mul.f16 v14 v21 v14  || lsu0.swzv8 [11111111]  || CMU.CPVI.x16  i1.l  v22.1  
CMU.CPVI.x16  i2.l  v22.2  || vau.sub.f16 v18 v8 v11 
CMU.CPVI.x16  i3.l  v22.3  || IAU.ADD i0 i15 i0   || vau.sub.f16 v19 v9 v15  
cmu.alignvec v12 v14 v14 2 || IAU.ADD i1 i15 i1   || LSU0.LD.32 i0 i0 ||  vau.mul.f16 v8  v23 v6 || lsu1.swzv8 [00000000]     
cmu.cpvv v24 v17           || IAU.ADD i2 i15 i2   || LSU0.LD.32 i1 i1 ||  vau.sub.f16 v20 v21 v18     || lsu1.swzv8 [00000000]                                    
CMU.CPVV.f16.i16s v17  v12                                                || vau.mul.f16 v9  v23 v2 || lsu0.swzv8 [11111111]  
CMU.CPVI.x16  i4.l  v22.4                                                 || vau.mul.f16 v10  v23 v7 || lsu0.swzv8 [22222222]  
CMU.CPVV.i16.f16  v17  v17  || IAU.ADD i3 i15 i3   || LSU0.LD.32 i2 i2    || vau.mul.f16 v11  v23 v3 || lsu1.swzv8 [33333333] 
CMU.CPVI.x16  i5.l  v22.5   || vau.mul.f16 v14 v21 v11   || lsu0.swzv8 [22222222]     
cmu.cpvv v29 v16   || IAU.ADD i4 i15 i4   || LSU0.LD.32 i3 i3 ||    vau.add.f16 v8 v9 v8    
vau.sub.f16 v16 v12 v17                          || cmu.alignvec v23 v23 v23 8   
CMU.CPVI.x16  i6.l  v22.6   || IAU.ADD i5 i15 i5   || LSU0.LD.32 i4 i4 ||  vau.mul.f16 v9  v23 v6 || lsu1.swzv8 [00000000]
CMU.CPVI.x16  i7.l  v22.7   || IAU.ADD i6 i15 i6   || LSU0.LD.32 i5 i5 ||  vau.add.f16 v6 v10 v11 
CMU.VILV.x16 v17 v12 v17 v15                       || vau.mul.f16 v7  v23 v2 || lsu0.swzv8 [11111111]   
CMU.VILV.x16 v16 v15 v16 v19                       || vau.mul.f16 v10  v23 v7 || lsu0.swzv8 [22222222]   
CMU.CPVI.x16  i20.l  v26.0 || IAU.ADD i7 i15 i7   || LSU0.LD.32 i6 i6                         || vau.sub.f16 v19 v21 v16   || lsu1.swzv8 [00000000]         
CMU.CPVI.x16  i21.l  v26.1 || IAU.ADD i20 i15 i20 || LSU0.LD.32 i7 i7    || lsu1.cp v25.0 i0  || vau.add.f16  v12 v14 v17                                  
CMU.CPVI.x16  i22.l  v26.2 || IAU.ADD i21 i15 i21 || LSU0.LD.32 i20 i20	 || lsu1.cp v25.1 i1  ||  vau.add.f16 v11 v8 v6  
vau.mul.f16 v15 v20 v19                                          || cmu.alignvec v2 v2 v2 8  || LSU0.LDO.64.H v4 i18 8 || IAU.SUB i0 i0 i0 || lsu1.ldih i1 0  ////////   
vau.mul.f16 v17 v20 v16                                          || cmu.cpvv v0 v2           || LSU0.LDO.64.L v4 i18 0 || IAU.ADD i18 i18 16  
CMU.CPVI.x16  i23.l  v26.3 || IAU.ADD i22 i15 i22 || LSU0.LD.32 i21 i21	 || lsu1.cp v25.2 i2 || vau.mul.f16 v19 v18 v19  
CMU.CPVI.x16  i24.l  v26.4 || IAU.ADD i23 i15 i23 || LSU0.LD.32 i22 i22	 || lsu1.cp v25.3 i3 || vau.mul.f16 v16 v18 v16  
CMU.CPVV.f16.i16s  v12   v12     || vau.add.f16 v6 v9 v7  || lsu1.cp v26.0 i4  || LSU0.LD.128.u8.f16  v2 i11 || IAU.ADD i11 i11 4//bgidx[0] + 1  
CMU.CPVI.x16  i25.l  v26.5 || IAU.ADD i24 i15 i24 || LSU0.LD.32 i23 i23	  || vau.mul.f16 v7  v23 v3 || lsu1.swzv8 [33333333]                                    
vau.mul.f16 v8 v21 v4   || lsu1.swzv8 [33333333]      || cmu.alignvec v3 v3 v3 8   || LSU0.LD.128.u8.f16  v3 i8  || IAU.ADD i8  i8  4//bgidx[1] + 1
vau.mul.f16 v9 v21 v11   || lsu0.swzv8 [11111111]      || cmu.cpvv v1 v3   || IAU.ADD i25 i15 i25 	   || lsu1.cp v26.1 i5        
vau.add.f16 v10 v10 v7 || cmu.alignvec v23 v23 v23 8     || LSU0.LD.32 i24 i24    || lsu1.cp v18.0 i20   || IAU.SUB i2 i2 i2                                                                         
CMU.CPVI.x16  i26.l  v26.6 || LSU0.LD.32 i25 i25	   || lsu1.cp v26.2 i6  || IAU.SUB i3 i3 i3
CMU.CPVI.x16  i27.l  v26.7 || IAU.ADD i26 i15 i26      || lsu1.cp v18.1 i21  || lsu0.ldih i4 0
                              IAU.ADD i27 i15 i27 || LSU0.LD.32 i26 i26	   || lsu1.cp v26.3 i7	|| cmu.vszm.byte i5 i5 [zzzz]														 
VAU.ADD.i16 v22 v12 v12  ||  CMU.VDILV.x16 v25 v26 v25 v26                         || LSU0.LD.32 i27 i27  || lsu1.cp v18.2 i22 || IAU.SUB i6 i6 i6	
cmu.clamp0.f16 v4, v4, v30|| lsu0.ldih i21 0    
nop
vau.mul.f16  v24  v26 v13    || IAU.SUB i7 i7 i7  || cmu.vszm.byte i20 i20 [zzzz]                                   
cmu.cpvv v13 v15  || vau.mul.f16  v27  v25 v24	 || IAU.SUB i22 i22 i22							
CMU.CPVV.f16.i16s v12  v8  														|| 			          lsu1.cp v18.3 i23 
CMU.CPVV.f16.i16s v15  v9 || vau.add.i16 v26 v21 v22 || lsu0.swzv8 [44444444]  	|| 			          lsu1.cp v25.0 i24 || IAU.SUB i23 i23 i23 
cmu.cpvv.i16.f16  v11 v12 	|| vau.add.f16 v14 v6 v10 		                 || 			          lsu1.cp v25.1 i25 || IAU.SUB i24 i24 i24 
CMU.CPVV.i16.f16  v15  v15  												 || 			          lsu1.cp v25.2 i26 || IAU.SUB i25 i25 i25
CMU.CPVI.x16  i0.l  v22.0  	|| 						lsu1.cp v25.3 i27	|| IAU.SUB i26 i26 i26																					  
vau.mul.f16 v14 v21 v14  || lsu0.swzv8 [11111111]  || CMU.VDILV.x16 v31 v30 v18 v25 || IAU.SUB i27 i27 i27
cmu.alignvec v6 v0 v2 14 || vau.mul.f16 v30 v30 v28
cmu.alignvec v7 v1 v3 14 || vau.mul.f16 v31 v31 v29
cmu.cpvv v28 v19  || vau.sub.f16 v18 v8 v11 
CMU.CPVI.x16  i1.l  v22.1  || vau.add.f16  v30  v27  v24
CMU.CPVI.x16  i2.l  v22.2  || vau.add.f16 v31 v31 v30
CMU.CPVI.x16  i3.l  v22.3  || IAU.ADD i0 i15 i0   || vau.sub.f16 v19 v9 v15  
cmu.alignvec v12 v14 v14 2 || IAU.ADD i1 i15 i1   || LSU0.LD.32 i0 i0 ||  vau.mul.f16 v8  v23 v6 || lsu1.swzv8 [00000000]     
cmu.cpvv v24 v17           || IAU.ADD i2 i15 i2   || LSU0.LD.32 i1 i1 ||  vau.sub.f16 v20 v21 v18     || lsu1.swzv8 [00000000]                                    
CMU.CPVV.f16.i16s v17  v12                                                || vau.mul.f16 v9  v23 v2 || lsu0.swzv8 [11111111]  
CMU.CPVI.x16  i4.l  v22.4                                                 || vau.mul.f16 v10  v23 v7 || lsu0.swzv8 [22222222]  
CMU.CPVV.i16.f16  v17  v17  || IAU.ADD i3 i15 i3   || LSU0.LD.32 i2 i2    || vau.mul.f16 v11  v23 v3 || lsu1.swzv8 [33333333] 
CMU.CPVI.x16  i5.l  v22.5   || vau.mul.f16 v14 v21 v11   || lsu0.swzv8 [22222222]     
cmu.cpvv v29 v16   || IAU.ADD i4 i15 i4   || LSU0.LD.32 i3 i3 ||    vau.add.f16 v8 v9 v8    
vau.sub.f16 v16 v12 v17                          || cmu.alignvec v23 v23 v23 8   
CMU.CPVI.x16  i6.l  v22.6   || IAU.ADD i5 i15 i5   || LSU0.LD.32 i4 i4 ||  vau.mul.f16 v9  v23 v6 || lsu1.swzv8 [00000000]
CMU.CPVI.x16  i7.l  v22.7   || IAU.ADD i6 i15 i6   || LSU0.LD.32 i5 i5 ||  vau.add.f16 v6 v10 v11 
CMU.VILV.x16 v17 v12 v17 v15                       || vau.mul.f16 v7  v23 v2 || lsu0.swzv8 [11111111]   
CMU.VILV.x16 v16 v15 v16 v19                       || vau.mul.f16 v10  v23 v7 || lsu0.swzv8 [22222222]   
CMU.CPVI.x16  i20.l  v26.0 || IAU.ADD i7 i15 i7   || LSU0.LD.32 i6 i6                         || vau.sub.f16 v19 v21 v16   || lsu1.swzv8 [00000000]         
CMU.CPVI.x16  i21.l  v26.1 || IAU.ADD i20 i15 i20 || LSU0.LD.32 i7 i7    || lsu1.cp v25.0 i0  || vau.add.f16  v12 v14 v17                                  
CMU.CPVI.x16  i22.l  v26.2 || IAU.ADD i21 i15 i21 || LSU0.LD.32 i20 i20	 || lsu1.cp v25.1 i1  ||  vau.add.f16 v11 v8 v6  
vau.mul.f16 v15 v20 v19                                          || cmu.alignvec v2 v2 v2 8  || LSU0.LDO.64.H v4 i18 8 || IAU.SUB i0 i0 i0 || lsu1.ldih i1 0  ////////   
vau.mul.f16 v17 v20 v16                                          || cmu.cpvv v0 v2           || LSU0.LDO.64.L v4 i18 0 || IAU.ADD i18 i18 16  
CMU.CPVI.x16  i23.l  v26.3 || IAU.ADD i22 i15 i22 || LSU0.LD.32 i21 i21	 || lsu1.cp v25.2 i2 || vau.mul.f16 v19 v18 v19  
CMU.CPVI.x16  i24.l  v26.4 || IAU.ADD i23 i15 i23 || LSU0.LD.32 i22 i22	 || lsu1.cp v25.3 i3 || vau.mul.f16 v16 v18 v16  
CMU.CPVV.f16.i16s  v12   v12     || vau.add.f16 v6 v9 v7  || lsu1.cp v26.0 i4  || LSU0.LD.128.u8.f16  v2 i11 || IAU.ADD i11 i11 4//bgidx[0] + 1  
CMU.CPVI.x16  i25.l  v26.5 || IAU.ADD i24 i15 i24 || LSU0.LD.32 i23 i23	  || vau.mul.f16 v7  v23 v3 || lsu1.swzv8 [33333333]                                    
vau.mul.f16 v8 v21 v4   || lsu1.swzv8 [33333333]      || cmu.alignvec v3 v3 v3 8   || LSU0.LD.128.u8.f16  v3 i8  || IAU.ADD i8  i8  4//bgidx[1] + 1
vau.mul.f16 v9 v21 v11   || lsu0.swzv8 [11111111]      || cmu.cpvv v1 v3   || IAU.ADD i25 i15 i25 	   || lsu1.cp v26.1 i5        
vau.add.f16 v10 v10 v7 || cmu.alignvec v23 v23 v23 8     || LSU0.LD.32 i24 i24    || lsu1.cp v18.0 i20   || IAU.SUB i2 i2 i2                                                                         
vau.add.f16 v31 v30 v31   || CMU.CPVI.x16  i26.l  v26.6 || LSU0.LD.32 i25 i25	   || lsu1.cp v26.2 i6  || IAU.SUB i3 i3 i3
CMU.CPVI.x16  i27.l  v26.7 || IAU.ADD i26 i15 i26      || lsu1.cp v18.1 i21  || lsu0.ldih i4 0
                              IAU.ADD i27 i15 i27 || LSU0.LD.32 i26 i26	   || lsu1.cp v26.3 i7	|| cmu.vszm.byte i5 i5 [zzzz]														 
VAU.ADD.i16 v22 v12 v12  ||  CMU.VDILV.x16 v25 v26 v25 v26                         || LSU0.LD.32 i27 i27  || lsu1.cp v18.2 i22 || IAU.SUB i6 i6 i6
cmu.cpivr.x16 v30, i31
cmu.clamp0.f16 v4, v4, v30
nop

vau.mul.f16  v24  v26 v13    || IAU.SUB i7 i7 i7  || cmu.vszm.byte i20 i20 [zzzz]  || lsu0.ldih i21 0          || bru.rpl i9 i14                                  
lsu1.cp v13 v15  || vau.mul.f16  v27  v25 v24	 || IAU.SUB i22 i22 i22		|| LSU0.STO.64.L v31 i16 0 || cmu.cpivr.x16 v30, i31					
CMU.CPVV.f16.i16s v12  v8  														|| 			          lsu1.cp v18.3 i23 || LSU0.STO.64.H v31 i16 8 || IAU.ADD i16 i16 16
CMU.CPVV.f16.i16s v15  v9 || vau.add.i16 v26 v21 v22 || lsu0.swzv8 [44444444]  	|| 			          lsu1.cp v25.0 i24 || IAU.SUB i23 i23 i23 
cmu.cpvv.i16.f16  v11 v12 	|| vau.add.f16 v14 v6 v10 		                 || 			          lsu1.cp v25.1 i25 || IAU.SUB i24 i24 i24 
CMU.CPVV.i16.f16  v15  v15  												 || 			          lsu1.cp v25.2 i26 || IAU.SUB i25 i25 i25
CMU.CPVI.x16  i0.l  v22.0  	|| 						lsu1.cp v25.3 i27	|| IAU.SUB i26 i26 i26	||lsu0.cp v20, v30																				  
vau.mul.f16 v14 v21 v14  || lsu0.swzv8 [11111111]  || CMU.VDILV.x16 v31 v30 v18 v25 || IAU.SUB i27 i27 i27
cmu.alignvec v6 v0 v2 14 || vau.mul.f16 v30 v30 v28
cmu.alignvec v7 v1 v3 14 || vau.mul.f16 v31 v31 v29
lsu1.cp v28 v19  || vau.sub.f16 v18 v8 v11 || cmu.clamp0.f16 v4, v4, v20
CMU.CPVI.x16  i1.l  v22.1  || vau.add.f16  v30  v27  v24
CMU.CPVI.x16  i2.l  v22.2  || vau.add.f16 v31 v31 v30
CMU.CPVI.x16  i3.l  v22.3  || IAU.ADD i0 i15 i0   || vau.sub.f16 v19 v9 v15  
cmu.alignvec v12 v14 v14 2 || IAU.ADD i1 i15 i1   || LSU0.LD.32 i0 i0 ||  vau.mul.f16 v8  v23 v6 || lsu1.swzv8 [00000000]     
cmu.cpvv v24 v17           || IAU.ADD i2 i15 i2   || LSU0.LD.32 i1 i1 ||  vau.sub.f16 v20 v21 v18     || lsu1.swzv8 [00000000]                                    
CMU.CPVV.f16.i16s v17  v12                                                || vau.mul.f16 v9  v23 v2 || lsu0.swzv8 [11111111]  
CMU.CPVI.x16  i4.l  v22.4                                                 || vau.mul.f16 v10  v23 v7 || lsu0.swzv8 [22222222]  
CMU.CPVV.i16.f16  v17  v17  || IAU.ADD i3 i15 i3   || LSU0.LD.32 i2 i2    || vau.mul.f16 v11  v23 v3 || lsu1.swzv8 [33333333] 
CMU.CPVI.x16  i5.l  v22.5   || vau.mul.f16 v14 v21 v11   || lsu0.swzv8 [22222222]     
cmu.cpvv v29 v16   || IAU.ADD i4 i15 i4   || LSU0.LD.32 i3 i3 ||    vau.add.f16 v8 v9 v8    
vau.sub.f16 v16 v12 v17                          || cmu.alignvec v23 v23 v23 8   
CMU.CPVI.x16  i6.l  v22.6   || IAU.ADD i5 i15 i5   || LSU0.LD.32 i4 i4 ||  vau.mul.f16 v9  v23 v6 || lsu1.swzv8 [00000000]
CMU.CPVI.x16  i7.l  v22.7   || IAU.ADD i6 i15 i6   || LSU0.LD.32 i5 i5 ||  vau.add.f16 v6 v10 v11 
CMU.VILV.x16 v17 v12 v17 v15                       || vau.mul.f16 v7  v23 v2 || lsu0.swzv8 [11111111]   
CMU.VILV.x16 v16 v15 v16 v19                       || vau.mul.f16 v10  v23 v7 || lsu0.swzv8 [22222222]   
CMU.CPVI.x16  i20.l  v26.0 || IAU.ADD i7 i15 i7   || LSU0.LD.32 i6 i6                         || vau.sub.f16 v19 v21 v16   || lsu1.swzv8 [00000000]         
CMU.CPVI.x16  i21.l  v26.1 || IAU.ADD i20 i15 i20 || LSU0.LD.32 i7 i7    || lsu1.cp v25.0 i0  || vau.add.f16  v12 v14 v17                                  
CMU.CPVI.x16  i22.l  v26.2 || IAU.ADD i21 i15 i21 || LSU0.LD.32 i20 i20	 || lsu1.cp v25.1 i1  ||  vau.add.f16 v11 v8 v6  
vau.mul.f16 v15 v20 v19                                          || cmu.alignvec v2 v2 v2 8  || LSU0.LDO.64.H v4 i18 8 || IAU.SUB i0 i0 i0 || lsu1.ldih i1 0  ////////   
vau.mul.f16 v17 v20 v16                                          || cmu.cpvv v0 v2           || LSU0.LDO.64.L v4 i18 0 || IAU.ADD i18 i18 16  
CMU.CPVI.x16  i23.l  v26.3 || IAU.ADD i22 i15 i22 || LSU0.LD.32 i21 i21	 || lsu1.cp v25.2 i2 || vau.mul.f16 v19 v18 v19  
CMU.CPVI.x16  i24.l  v26.4 || IAU.ADD i23 i15 i23 || LSU0.LD.32 i22 i22	 || lsu1.cp v25.3 i3 || vau.mul.f16 v16 v18 v16  
CMU.CPVV.f16.i16s  v12   v12     || vau.add.f16 v6 v9 v7  || lsu1.cp v26.0 i4  || LSU0.LD.128.u8.f16  v2 i11 || IAU.ADD i11 i11 4//bgidx[0] + 1  
CMU.CPVI.x16  i25.l  v26.5 || IAU.ADD i24 i15 i24 || LSU0.LD.32 i23 i23	  || vau.mul.f16 v7  v23 v3 || lsu1.swzv8 [33333333]                                    
 localTM_loop:
vau.mul.f16 v8 v21 v4   || lsu1.swzv8 [33333333]      || cmu.alignvec v3 v3 v3 8   || LSU0.LD.128.u8.f16  v3 i8  || IAU.ADD i8  i8  4//bgidx[1] + 1
vau.mul.f16 v9 v21 v11   || lsu0.swzv8 [11111111]      || cmu.cpvv v1 v3   || IAU.ADD i25 i15 i25 	   || lsu1.cp v26.1 i5        
vau.add.f16 v10 v10 v7 || cmu.alignvec v23 v23 v23 8     || LSU0.LD.32 i24 i24    || lsu1.cp v18.0 i20   || IAU.SUB i2 i2 i2                                                                         
vau.add.f16 v31 v30 v31   || CMU.CPVI.x16  i26.l  v26.6 || LSU0.LD.32 i25 i25	   || lsu1.cp v26.2 i6  || IAU.SUB i3 i3 i3
CMU.CPVI.x16  i27.l  v26.7 || IAU.ADD i26 i15 i26      || lsu1.cp v18.1 i21  || lsu0.ldih i4 0
                              IAU.ADD i27 i15 i27 || LSU0.LD.32 i26 i26	   || lsu1.cp v26.3 i7	|| cmu.vszm.byte i5 i5 [zzzz]														 
VAU.ADD.i16 v22 v12 v12  ||  CMU.VDILV.x16 v25 v26 v25 v26                         || LSU0.LD.32 i27 i27  || lsu1.cp v18.2 i22 || IAU.SUB i6 i6 i6

//// calup 1 
LSU0.STO.64.L v31 i16 0 	||LSU1.STO.64.H v31 i16 8 || IAU.ADD i16 i16 16 

////calup 2
vau.mul.f16  v24  v26 v13
vau.mul.f16  v27  v25 v24
nop 2 
lsu1.cp v18.3 i23 ||IAU.SUB i23 i23 i23 
lsu1.cp v25.0 i24 || IAU.SUB i24 i24 i24
lsu1.cp v25.1 i25 || IAU.SUB i25 i25 i25 
lsu1.cp v25.2 i26 || IAU.SUB i26 i26 i26
lsu1.cp v25.3 i27 || IAU.SUB i27 i26 i27
CMU.VDILV.x16 v31 v30 v18 v25
vau.mul.f16 v30 v30 v28
vau.mul.f16 v31 v31 v29
nop 
vau.add.f16  v30  v27  v24
vau.add.f16 v31 v31 v30
nop 2
vau.add.f16 v31 v30 v31  
nop 2 
LSU0.STO.64.L v31 i16 0 	||LSU1.STO.64.H v31 i16 8 || IAU.ADD i16 i16 16


	LSU0.LD.64.h  v31  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v31  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v30  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v30  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v29  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v29  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v28  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v28  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v27  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v27  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v26  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v26  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v25  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v25  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v24  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v24  i19 || IAU.ADD i19 i19 8	
	
	LSU0.LD.32  i31  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i29  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i28  i19 || IAU.ADD i19 i19 4	
	LSU0.LD.32  i27  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i26  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i25  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i24  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i23  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i22  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i21  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i20  i19 || IAU.ADD i19 i19 4
	
    BRU.jmp i30
	nop 6
.nowarnend
.size mvispLocalTM_asm,.-mvispLocalTM_asm
.end
