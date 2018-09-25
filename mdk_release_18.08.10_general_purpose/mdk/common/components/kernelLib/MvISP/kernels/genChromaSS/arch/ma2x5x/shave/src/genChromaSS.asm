//-------------------------------------------------------------------------------
.type mvispGenChromaSS_asm,@function
.version 00.51.05
//                           i18,                   i17,                  i16,       i15,                  i14
//void mvispGenChromaSS(unsigned char* outC[3], unsigned char** inRGB, int eps, float chromaScale[3], unsigned int width)
//{
//  unsigned char *inR0   = inRGB[0]// //first line of R
//  unsigned char *inG0   = inRGB[1]// //first line of G
//  unsigned char *inB0   = inRGB[2]// //first line of B
//  unsigned char *inR1   = inRGB[3]// //second line of R
//  unsigned char *inG1   = inRGB[4]// //second line of G
//  unsigned char *inB1   = inRGB[5]// //second line of B
//  
//  unsigned char  *outCR  = outC[0]//
//  unsigned char  *outCG  = outC[1]//
//  unsigned char  *outCB  = outC[2]//
//  
//  unsigned int i//
//  half luma//
//  half normValHalfR = chromaScale[0] * (255.0f/3.0f)//
//  half normValHalfG = chromaScale[1] * (255.0f/3.0f)//
//  half normValHalfB = chromaScale[2] * (255.0f/3.0f)//
//  half epsilon = (float)eps * 4.0f//
//
//  for(i=0// i<width// i++)
//  {
//      half inRGB[3]//
//      half lumaInv//
//
//      inRGB[0] = inR0[i*2] + inR0[i*2+1] + inR1[i*2] + inR1[i*2+1]//
//      inRGB[1] = inG0[i*2] + inG0[i*2+1] + inG1[i*2] + inG1[i*2+1]//
//      inRGB[2] = inB0[i*2] + inB0[i*2+1] + inB1[i*2] + inB1[i*2+1]//
//      luma = epsilon + (inRGB[0] + inRGB[1] + inRGB[1] + inRGB[2]) * 0.25f//
//      lumaInv = 1.0/luma//
//
//      outCR[i] =  (inRGB[0] * lumaInv*normValHalfR) > 255 ? 255 : (inRGB[0] * lumaInv*normValHalfR)//
//      outCG[i] =  (inRGB[1] * lumaInv*normValHalfG) > 255 ? 255 : (inRGB[1] * lumaInv*normValHalfG)//
//      outCB[i] =  (inRGB[2] * lumaInv*normValHalfB) > 255 ? 255 : (inRGB[2] * lumaInv*normValHalfB)//
//  }
//}
//                           i18,                   i17,                  i16,       i15,                  i14
//void mvispGenChromaSS(unsigned char* outC[3], unsigned char** inRGB, int eps, float chromaScale[3], unsigned int width)
//------------------------------------------------------------------------------- 


.data .data.mvispGenChromaSS
.align 4

.code .text.mvispGenChromaSS
//########################################
.lalign 
mvispGenChromaSS_asm:
.nowarn 10
	                        IAU.SUB i19 i19 4
	LSU0.ST.32  i20  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i21  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i22  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i23  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i24  i19 || IAU.SUB i19 i19 4 
    LSU0.ST.32  i25  i19 || IAU.SUB i19 i19 4
    LSU0.ST.32  i26  i19 || IAU.SUB i19 i19 4
    LSU0.ST.32  i27  i19 
	
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

LSU0.ldil i11, 0x5550 || LSU1.ldih i11, 0x0
LSU0.ldil i12, 0x3400 || LSU1.ldih i12, 0x0
LSU0.ldil i13, 0x3C00 || LSU1.ldih i13, 0x3C00
lsu0.ldi.32	i25 i15
lsu0.ldi.32	i26 i15
lsu0.ldi.32	i27 i15
IAU.SHL i16 i16 2
cmu.cpii.i16.f16 i16 i16

LSU0.LDi.64  i0  i18
LSU0.LD.32  i2  i18
cmu.cpii.f32.f16 i25 i25
cmu.cpii.f32.f16 i26 i26
cmu.cpii.f32.f16 i27 i27
SAU.mul.f16 i25 i25 i11
SAU.mul.f16 i26 i26 i11
SAU.mul.f16 i27 i27 i11
nop 3
cmu.cpiv.x16 v31.0 i16.l
cmu.cpiv.x16 v31.1 i25.l
cmu.cpiv.x16 v31.2 i26.l
cmu.cpiv.x16 v31.3 i27.l
cmu.cpiv.x16 v31.4 i12.l 



LSU0.LDi.64  i3  i17
LSU0.LDi.64  i5  i17
LSU0.LD.64  i7  i17
nop 6
LSU0.ldil i9, 0x7 || LSU1.ldih i9, 0x0
IAU.AND i24 i9 i14 //  see if we have remaining pixels to compute
IAU.SHR.u32 i14 i14 3



LSU0.ldil i16, genChromaSS_asm_loop || LSU1.ldih i16, genChromaSS_asm_loop
LSU0.ldil i15, 0x5bf8


LSU0.LDi.128.u8.f16 v0 i3  || LSU1.LDi.128.u8.f16 v3 i6				
LSU0.LDi.128.u8.f16 v1 i4  || LSU1.LDi.128.u8.f16 v4 i7 
LSU0.LDi.128.u8.f16 v2 i5  || LSU1.LDi.128.u8.f16 v5 i8 
LSU0.LDi.128.u8.f16  v6 i3 || LSU1.LDi.128.u8.f16  v9 i6 
LSU0.LDi.128.u8.f16  v7 i4 || LSU1.LDi.128.u8.f16 v10 i7
LSU0.LDi.128.u8.f16  v8 i5 || LSU1.LDi.128.u8.f16 v11 i8
nop 

                                                             VAU.ADD.f16 v0 v0 v3                                
                                                             VAU.ADD.f16 v1 v1 v4                                
                                                             VAU.ADD.f16 v2 v2 v5                                
                                                             VAU.ADD.f16 v6 v6 v9    || cmu.alignvec v12 v0 v0 2 
LSU0.LDi.128.u8.f16 v0 i3  || LSU1.LDi.128.u8.f16 v3 i6	  || VAU.ADD.f16 v7 v7 v10   || cmu.alignvec v13 v1 v1 2 			
LSU0.LDi.128.u8.f16 v1 i4  || LSU1.LDi.128.u8.f16 v4 i7   || VAU.ADD.f16 v8 v8 v11   || cmu.alignvec v14 v2 v2 2 
LSU0.LDi.128.u8.f16 v2 i5  || LSU1.LDi.128.u8.f16 v5 i8   || VAU.ADD.f16 v18 v0 v12	|| cmu.alignvec v12 v6 v6 2 
LSU0.LDi.128.u8.f16  v6 i3 || LSU1.LDi.128.u8.f16  v9 i6  || VAU.ADD.f16 v19 v1 v13	|| cmu.alignvec v13 v7 v7 2 
LSU0.LDi.128.u8.f16  v7 i4 || LSU1.LDi.128.u8.f16 v10 i7  || VAU.ADD.f16 v20 v2 v14	|| cmu.alignvec v14 v8 v8 2	
LSU0.LDi.128.u8.f16  v8 i5 || LSU1.LDi.128.u8.f16 v11 i8  || VAU.ADD.f16 v21 v6 v12			    
															 VAU.ADD.f16 v22 v7 v13						  
                                                             VAU.ADD.f16 v23 v8 v14	

															 
                                                             VAU.ADD.f16 v0 v0 v3                                || CMU.VDILV.x16 v18 v27   v18 v21                          
                                                             VAU.ADD.f16 v1 v1 v4                                || CMU.VDILV.x16 v19 v28   v19 v22                        
                                                             VAU.ADD.f16 v2 v2 v5                                || CMU.VDILV.x16 v20 v29   v20 v23  
                                                                                                                 vau.add.f16 v24 v27 v28    
															                                                     vau.add.f16 v25 v28 v29
                                                             VAU.ADD.f16 v6 v6 v9    
LSU0.LDi.128.u8.f16 v0 i3  	                              || VAU.ADD.f16 v7 v7 v10    	|| cmu.alignvec v12 v0 v0 2 	
LSU0.LDi.128.u8.f16 v3 i6                                                                                      || vau.add.f16 v26 v24 v25
LSU0.LDi.128.u8.f16 v1 i4                                 || VAU.ADD.f16 v8 v8 v11    || cmu.alignvec v13 v1 v1 2
LSU0.LDi.128.u8.f16 v4 i7                                 || VAU.ADD.f16 v18 v0 v12	  || cmu.alignvec v14 v2 v2 2
LSU0.LDi.128.u8.f16 v2 i5                                                                                       || vau.mul.f16 v26 v31 v26 || LSU1.SWZV8 [44444444] 
LSU0.LDi.128.u8.f16 v5 i8                                 || VAU.ADD.f16 v19 v1 v13	 || cmu.alignvec v12 v6 v6 2                                                                                  
LSU0.LDi.128.u8.f16  v6 i3                                || VAU.ADD.f16 v20 v2 v14		|| cmu.alignvec v13 v7 v7 2  
LSU0.LDi.128.u8.f16  v9 i6                                                                                   ||   vau.add.f16 v26 v31 v26 || LSU1.SWZV8 [00000000] || cmu.alignvec v14 v8 v8 2
LSU0.LDi.128.u8.f16  v7 i4                                || VAU.ADD.f16 v21 v6 v12	                                                                               
LSU0.LDi.128.u8.f16 v10 i7                                || VAU.ADD.f16 v22 v7 v13	                      				                                                                          
LSU0.LDi.128.u8.f16  v8 i5   		                      || VAU.ADD.f16 v23 v8 v14  || lsu1.cp  i9 v26.0 
LSU0.LDi.128.u8.f16 v11 i8                                || SAU.DIV.f16 i20 i13  i9 || lsu1.cp i10 v26.1  
															 SAU.DIV.f16 i21 i13 i10 || lsu1.cp i11 v26.2 || cmu.cpvv v15 v27	
                                                             SAU.DIV.f16 i22 i13 i11 || lsu1.cp i12 v26.3 || cmu.cpvv v16 v28 			 
                                                             SAU.DIV.f16 i23 i13 i12                      || cmu.cpvv v17 v29	
                                                                                                           

                                                             VAU.ADD.f16 v0 v0 v3                                || CMU.VDILV.x16 v18 v27   v18 v21                          
                                                             VAU.ADD.f16 v1 v1 v4                                || CMU.VDILV.x16 v19 v28   v19 v22                        
                                                             VAU.ADD.f16 v2 v2 v5                                || CMU.VDILV.x16 v20 v29   v20 v23  
                                                                                                                 vau.add.f16 v24 v27 v28    
															                                                     vau.add.f16 v25 v28 v29            || cmu.cpiv.x32  v30.0 i20
                                                             VAU.ADD.f16 v6 v6 v9                                                                   || cmu.cpiv.x32  v30.1 i21
LSU0.LDi.128.u8.f16 v0 i3  	                              || VAU.ADD.f16 v7 v7 v10    	|| cmu.alignvec v12 v0 v0 2 	                            || lsu1.cp  v30.2 i22
LSU0.LDi.128.u8.f16 v3 i6                                                                                      || vau.add.f16 v26 v24 v25           || cmu.cpiv.x32  v30.3 i23
LSU0.LDi.128.u8.f16 v1 i4                                 || VAU.ADD.f16 v8 v8 v11    || cmu.alignvec v13 v1 v1 2
LSU0.LDi.128.u8.f16 v4 i7                                 || VAU.ADD.f16 v18 v0 v12	  || cmu.alignvec v14 v2 v2 2
LSU0.LDi.128.u8.f16 v2 i5                                                                                       || vau.mul.f16 v26 v31 v26 || LSU1.SWZV8 [44444444] 
LSU0.LDi.128.u8.f16 v5 i8                                 || VAU.ADD.f16 v19 v1 v13	 || cmu.alignvec v12 v6 v6 2                                                                                  
LSU0.LDi.128.u8.f16  v6 i3                                || VAU.ADD.f16 v20 v2 v14		|| cmu.alignvec v13 v7 v7 2  
LSU0.LDi.128.u8.f16  v9 i6                                                                                   ||   vau.add.f16 v26 v31 v26 || LSU1.SWZV8 [00000000] || cmu.alignvec v14 v8 v8 2
LSU0.LDi.128.u8.f16  v7 i4                                || VAU.ADD.f16 v21 v6 v12	       
vau.mul.f16 v12 v31 v30 || LSU1.SWZV8 [11111111]         || cmu.cpivr.x16 v24 i15                                                                
LSU0.LDi.128.u8.f16 v10 i7                                || VAU.ADD.f16 v22 v7 v13	     
vau.mul.f16 v13 v31 v30 || LSU1.SWZV8 [22222222]            
vau.mul.f16 v12 v12 v15      				                                                                          
LSU0.LDi.128.u8.f16  v8 i5   		                      || VAU.ADD.f16 v23 v8 v14  || lsu1.cp  i9 v26.0 || cmu.cpvv v15 v27	
vau.mul.f16 v13 v13 v16 
vau.mul.f16 v14 v31 v30 || LSU1.SWZV8 [33333333]   || cmu.cpvv v16 v28 	
LSU0.LDi.128.u8.f16 v11 i8                                || SAU.DIV.f16 i20 i13  i9 || lsu1.cp i10 v26.1  || CMU.CLAMP0.f16 v12 v12 v24
															 SAU.DIV.f16 i21 i13 i10 || lsu1.cp i11 v26.2  || CMU.CLAMP0.f16 v13 v13 v24	
															 vau.mul.f16 v14 v14 v17 
                                                             SAU.DIV.f16 i22 i13 i11 || lsu1.cp i12 v26.3 	|| cmu.cpvv v17 v29		 
                                                             SAU.DIV.f16 i23 i13 i12  

	                                 
                                                             VAU.ADD.f16 v0 v0 v3                                || CMU.VDILV.x16 v18 v27   v18 v21   || bru.rpl i16 i14	                       
                                                             VAU.ADD.f16 v1 v1 v4                                || CMU.VDILV.x16 v19 v28   v19 v22                        
                                                             VAU.ADD.f16 v2 v2 v5                                || CMU.VDILV.x16 v20 v29   v20 v23  
                                                                                                                 vau.add.f16 v24 v27 v28    || LSU0.STi.128.f16.u8 v12 i0 || CMU.CLAMP0.f16 v14 v14 v24
															                                                     vau.add.f16 v25 v28 v29            || lsu1.cp  v30.0 i20 || LSU0.STi.128.f16.u8 v13 i1	
                                                             VAU.ADD.f16 v6 v6 v9                                                                   || cmu.cpiv.x32  v30.1 i21 || LSU0.STi.128.f16.u8 v14 i2	
LSU0.LDi.128.u8.f16 v0 i3  	                              || VAU.ADD.f16 v7 v7 v10    	|| cmu.alignvec v12 v0 v0 2 	                            || lsu1.cp  v30.2 i22
LSU0.LDi.128.u8.f16 v3 i6                                                                                      || vau.add.f16 v26 v24 v25           || cmu.cpiv.x32  v30.3 i23
LSU0.LDi.128.u8.f16 v1 i4                                 || VAU.ADD.f16 v8 v8 v11    || cmu.alignvec v13 v1 v1 2
LSU0.LDi.128.u8.f16 v4 i7                                 || VAU.ADD.f16 v18 v0 v12	  || cmu.alignvec v14 v2 v2 2
LSU0.LDi.128.u8.f16 v2 i5                                                                                       || vau.mul.f16 v26 v31 v26 || LSU1.SWZV8 [44444444] 
LSU0.LDi.128.u8.f16 v5 i8                                 || VAU.ADD.f16 v19 v1 v13	 || cmu.alignvec v12 v6 v6 2                                                                                  
LSU0.LDi.128.u8.f16  v6 i3                                || VAU.ADD.f16 v20 v2 v14		|| cmu.alignvec v13 v7 v7 2  
LSU0.LDi.128.u8.f16  v9 i6                                                                                   ||   vau.add.f16 v26 v31 v26 || LSU1.SWZV8 [00000000] || cmu.alignvec v14 v8 v8 2
LSU0.LDi.128.u8.f16  v7 i4                                || VAU.ADD.f16 v21 v6 v12	       
vau.mul.f16 v12 v31 v30 || LSU1.SWZV8 [11111111]         || cmu.cpivr.x16 v24 i15                                                                
LSU0.LDi.128.u8.f16 v10 i7                                || VAU.ADD.f16 v22 v7 v13	     
vau.mul.f16 v13 v31 v30 || LSU1.SWZV8 [22222222]            
vau.mul.f16 v12 v12 v15      				                                                                          
LSU0.LDi.128.u8.f16  v8 i5   		                      || VAU.ADD.f16 v23 v8 v14  || lsu1.cp  i9 v26.0 || cmu.cpvv v15 v27	
 genChromaSS_asm_loop:
vau.mul.f16 v13 v13 v16 
vau.mul.f16 v14 v31 v30 || LSU1.SWZV8 [33333333]   || cmu.cpvv v16 v28 	
LSU0.LDi.128.u8.f16 v11 i8                                || SAU.DIV.f16 i20 i13  i9 || lsu1.cp i10 v26.1  || CMU.CLAMP0.f16 v12 v12 v24
															 SAU.DIV.f16 i21 i13 i10 || lsu1.cp i11 v26.2  || CMU.CLAMP0.f16 v13 v13 v24	
															 vau.mul.f16 v14 v14 v17 
                                                             SAU.DIV.f16 i22 i13 i11 || lsu1.cp i12 v26.3 	|| cmu.cpvv v17 v29		 
                                                             SAU.DIV.f16 i23 i13 i12                      


//nop 6
//bru.swih 0x1f
//nop 6


LSU0.ldil i9, 0x40 || LSU1.ldih i9, 0x0
IAU.SUB i3 i3 i9
IAU.SUB i4 i4 i9
IAU.SUB i5 i5 i9
IAU.SUB i6 i6 i9
IAU.SUB i7 i7 i9
IAU.SUB i8 i8 i9


LSU0.ldil i15, genChromaSS_asm_exit || LSU1.ldih i15, genChromaSS_asm_exit	  
cmu.cmz.i32 i24
peu.pc1c eq || bru.jmp i15
nop 6
LSU0.ldil i15, 0x5bf8

LSU0.LDi.128.u8.f16 v0 i3 || LSU1.LDi.128.u8.f16 v3 i6
LSU0.LDi.128.u8.f16 v1 i4 || LSU1.LDi.128.u8.f16 v4 i7
LSU0.LDi.128.u8.f16 v2 i5 || LSU1.LDi.128.u8.f16 v5 i8

nop 4


VAU.ADD.f16 v0 v0 v3
VAU.ADD.f16 v1 v1 v4
VAU.ADD.f16 v2 v2 v5

cmu.alignvec v12 v0 v0 2
VAU.ADD.f16 v18 v0 v12 || cmu.alignvec v13 v1 v1 2
VAU.ADD.f16 v19 v1 v13 || cmu.alignvec v14 v2 v2 2
VAU.ADD.f16 v20 v2 v14

CMU.VDILV.x16 v18 v21 v18 v18
CMU.VDILV.x16 v19 v22 v19 v19
CMU.VDILV.x16 v20 v23 v20 v20 



vau.add.f16 v24 v21 v22
vau.add.f16 v25 v22 v23
nop 2
vau.add.f16 v26 v24 v25
nop 2
vau.mul.f16 v26 v31 v26 || LSU1.SWZV8 [44444444] 
nop 2
vau.add.f16 v26 v31 v26 || LSU1.SWZV8 [00000000] 
nop 2



cmu.cpvi.x32  i9 v26.0
SAU.DIV.f16 i20 i13  i9 || cmu.cpvi.x32 i10 v26.1
SAU.DIV.f16 i21 i13 i10 || cmu.cpvi.x32 i11 v26.2
SAU.DIV.f16 i22 i13 i11 || cmu.cpvi.x32 i12 v26.3
SAU.DIV.f16 i23 i13 i12
nop 4
cmu.cpiv.x32  v15.0 i20
cmu.cpiv.x32  v15.1 i21
cmu.cpiv.x32  v15.2 i22
cmu.cpiv.x32  v15.3 i23



vau.mul.f16 v12 v31 v15 || LSU1.SWZV8 [11111111] 
vau.mul.f16 v13 v31 v15 || LSU1.SWZV8 [22222222] 
vau.mul.f16 v14 v31 v15 || LSU1.SWZV8 [33333333] 



vau.mul.f16 v12 v12 v21
vau.mul.f16 v13 v13 v22
vau.mul.f16 v14 v14 v23 || cmu.cpivr.x16 v8 i15

CMU.CLAMP0.f16 v12 v12 v8
CMU.CLAMP0.f16 v13 v13 v8
CMU.CLAMP0.f16 v14 v14 v8
CMU.CPVV.f16.i16s v12 v12
CMU.CPVV.f16.i16s v13 v13
CMU.CPVV.f16.i16s v14 v14

CMU.CPVI.x16 i3.l v12.0
LSU0.STO.8 i3 i0 0 || CMU.CPVI.x16 i4.l v12.1  
LSU0.STO.8 i4 i0 1 || CMU.CPVI.x16 i5.l v12.2  
LSU0.STO.8 i5 i0 2 || CMU.CPVI.x16 i6.l v12.3  
LSU0.STO.8 i6 i0 3 || CMU.CPVI.x16 i3.l v13.0  
LSU0.STO.8 i3 i1 0 || CMU.CPVI.x16 i4.l v13.1  
LSU0.STO.8 i4 i1 1 || CMU.CPVI.x16 i5.l v13.2  
LSU0.STO.8 i5 i1 2 || CMU.CPVI.x16 i6.l v13.3  
LSU0.STO.8 i6 i1 3 || CMU.CPVI.x16 i3.l v14.0  
LSU0.STO.8 i3 i2 0 || CMU.CPVI.x16 i4.l v14.1  
LSU0.STO.8 i4 i2 1 || CMU.CPVI.x16 i5.l v14.2  
LSU0.STO.8 i5 i2 2 || CMU.CPVI.x16 i6.l v14.3  
LSU0.STO.8 i6 i2 3   


 genChromaSS_asm_exit:


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


    LSU0.LD.32  i27  i19 || IAU.ADD i19 i19 4
    LSU0.LD.32  i26  i19 || IAU.ADD i19 i19 4
    LSU0.LD.32  i25  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i24  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i23  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i22  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i21  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i20  i19 || IAU.ADD i19 i19 4
  

bru.jmp i30 
nop 6
.nowarnend
.size mvispGenChromaSS_asm,.-mvispGenChromaSS_asm
.end

