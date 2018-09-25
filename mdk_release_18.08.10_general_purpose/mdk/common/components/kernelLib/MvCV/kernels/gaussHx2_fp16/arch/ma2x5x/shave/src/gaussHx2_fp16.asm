.type mvcvGaussHx2_fp16_asm,@function
.version 00.50.00

//////
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief   : Containd a function that applay downscale 2x horizontal with a gaussian filters with kernel 5x5. 
//                 Have to be use in combination with GaussVx2 for obtain correct output. 
//                 Gaussian 5x5 filter was decomposed in liniar 1x5, applayed horizontal and vertical
//                 Function not make replicate at margin, assumed the you have 3 more pixels on left and right from input buffer.
.data .rodata.GaussHx2_fp16_asm
.align 16
 gaussHx2_fp16_vect:
		.float16    0.0625F16, 0.25F16,0.375F16

.code .text.GaussHx2_fp16_asm
.align 16

.lalign
mvcvGaussHx2_fp16_asm:
.nowarn
IAU.SUB i19 i19 8
LSU0.ST.64.l  v24  i19  || IAU.SUB i19 i19 8
LSU0.ST.64.h  v24  i19  || IAU.SUB i19 i19 8
LSU0.ST.64.l  v25  i19  || IAU.SUB i19 i19 8
LSU0.ST.64.h  v25  i19  || IAU.SUB i19 i19 8
LSU0.ST.64.l  v26  i19  || IAU.SUB i19 i19 8
LSU0.ST.64.h  v26  i19  || IAU.SUB i19 i19 8
LSU0.ST.64.l  v27  i19  || IAU.SUB i19 i19 8
LSU0.ST.64.h  v27  i19  || IAU.SUB i19 i19 8
LSU0.ST.64.l  v28  i19  || IAU.SUB i19 i19 8
LSU0.ST.64.h  v28  i19  || IAU.SUB i19 i19 8
LSU0.ST.64.l  v29  i19  || IAU.SUB i19 i19 8
LSU0.ST.64.h  v29  i19  || IAU.SUB i19 i19 8
LSU0.ST.64.l  v30  i19  || IAU.SUB i19 i19 8
LSU0.ST.64.h  v30  i19  


LSU0.ldil i1, gaussHx2_fp16_vect || LSU1.ldih i1, gaussHx2_fp16_vect
LSU0.ldil i3, gaussHx2_fp16_loop || LSU1.ldih i3, gaussHx2_fp16_loop
LSU0.ldil i10 gaussHx2_fp16_compensate || LSU1.ldih i10 gaussHx2_fp16_compensate		
LSU0.ldil i11 gaussHx2_fp16_final || LSU1.ldih i11 gaussHx2_fp16_final
LSU0.LDIL i6 0x1f || LSU1.LDIH i6 0x0 
LSU0.LDIL i7 0x20 || LSU1.LDIH i7 0x0 
LSU0.LDi.16r v23 i1
LSU0.LD.32 i18 i18     
LSU0.LD.32 i17 i17    
LSU1.LDi.16r v22 i1  
LSU1.LDi.16r v21 i1

CMU.CMII.i32 i16 i7
PEU.PC1C lt || bru.jmp i10
nop 6

IAU.AND	i9 i16 i6
IAU.SHR.u32  i16 i16 5
LSU0.LDO.64.L v0 i18 0 	 || LSU1.LDO.32 i1 i18 -4   
LSU1.LDO.64.H v0 i18 8   || LSU0.LDO.64.L v1 i18 16 
LSU1.LDO.64.H v1 i18 24  || LSU0.LDO.32 i2 i18 32 || iau.add i18 i18 32
LSU1.LDO.32 i4 i18 -4  	 || LSU0.LDO.64.L v14 i18 0 
LSU1.LDO.64.H v14 i18 8  || LSU0.LDO.64.L v15 i18 16  
LSU1.LDO.64.H v15 i18 24 || LSU0.LDO.32 i5 i18 32 || IAU.ADD i18 i18 32
nop 3
cmu.vdilv.x16 v3, v2, v0, v1  
CMU.ALIGNVEC v5 v3, v3 14		 
CMU.CPIV.x16  v5.0  i1.h   		 || vau.mul.f16 v4 v21 v2 
cmu.vdilv.x16 v17, v16, v14, v15 || vau.add.f16 v8 v5 v3 
CMU.ALIGNVEC v6 v2, v2 14 		 || vau.mul.f16 v18 v21 v16  
CMU.CPIV.x16  v6.0  i1.l 	                                
CMU.ALIGNVEC v7 v2, v2 2		 || vau.mul.f16 v9 v22 v8   


CMU.CPIV.x16  v7.7  i2.l         || LSU0.LDO.64.L v0 i18 0   || LSU1.LDO.32 i1 i18 -4    || bru.rpl i3 i16 _end_delay_loop_gaussH2
CMU.ALIGNVEC v19 v17, v17 14	 || vau.add.f16 v10 v6 v7    || LSU1.LDO.64.H v0 i18 8   || LSU0.LDO.64.L v1 i18 16 
CMU.CPIV.x16  v19.0  i4.h   	 || vau.add.f16 v11 v9 v4    || LSU1.LDO.64.H v1 i18 24  || LSU0.LDO.32 i2 i18 32  || iau.add i18 i18 32
CMU.ALIGNVEC v20 v16, v16 14 	 || vau.add.f16 v25 v19 v17  || LSU1.LDO.32 i4 i18 -4    || LSU0.LDO.64.L v14 i18 0 
CMU.CPIV.x16  v20.0  i4.l   	 || vau.mul.f16 v12 v23 v10  || LSU1.LDO.64.H v14 i18 8  || LSU0.LDO.64.L v15 i18 16   

CMU.ALIGNVEC v24 v16, v16 2 	 || LSU1.LDO.64.H v15 i18 24 || LSU0.LDO.32 i5 i18 32    || IAU.ADD i18 i18 32
CMU.CPIV.x16  v24.7  i5.l   	 || vau.mul.f16 v26 v22 v25  
vau.add.f16 v27 v20 v24 
vau.add.f16 v13 v11 v12  
cmu.vdilv.x16 v3, v2, v0, v1  	 || vau.add.f16 v28 v26 v18			

 gaussHx2_fp16_loop:
CMU.ALIGNVEC v5 v3, v3 14		 || vau.mul.f16 v29 v23 v27 		 	
CMU.CPIV.x16  v5.0  i1.h   		 || vau.mul.f16 v4 v21 v2 
cmu.vdilv.x16 v17, v16, v14, v15 || vau.add.f16 v8 v5 v3 
vau.add.f16 v30 v28 v29
CMU.ALIGNVEC v6 v2, v2 14 		 || vau.mul.f16 v18 v21 v16  
CMU.CPIV.x16  v6.0  i1.l 		 || LSU0.STO.64.L v13 i17 0 || LSU1.STO.64.H v13 i17 8 || IAU.ADD i17 i17 16
CMU.ALIGNVEC v7 v2, v2 2		 || vau.mul.f16 v9 v22 v8   || LSU0.STO.64.L v30 i17 0 || LSU1.STO.64.H v30 i17 8 || IAU.ADD i17 i17 16
_end_delay_loop_gaussH2:


IAU.SUB i18 i18 64
CMU.CMZ.i32 i9
PEU.PC1C eq || bru.jmp i11
nop 6

 gaussHx2_fp16_compensate:
LSU0.LDO.32 i1 i18 -4 
LSU0.LDO.64.L v0 i18 0 || LSU1.LDO.64.H v0 i18 8 || IAU.ADD i18 i18 16  
LSU0.LDO.64.L v1 i18 0 || LSU1.LDO.64.H v1 i18 8 || IAU.ADD i18 i18 16
LSU0.LDo.32  i2 i18  0
nop 5
cmu.vdilv.x16 v3, v2, v0, v1    
vau.mul.f16 v4 v21 v2  || CMU.ALIGNVEC v5 v3, v3 14
CMU.CPIV.x16  v5.0  i1.h   
vau.add.f16 v8 v5 v3 || CMU.ALIGNVEC v6 v2, v2 14
CMU.CPIV.x16  v6.0  i1.l   
CMU.ALIGNVEC v7 v2, v2 2
vau.mul.f16 v9 v22 v8  || CMU.CPIV.x16  v7.7  i2.l   
vau.add.f16 v10 v6 v7
nop
vau.add.f16 v11 v9 v4
vau.mul.f16 v12 v23 v10
nop 2
vau.add.f16 v13 v11 v12
nop 2
LSU0.STO.64.L v13 i17 0 || LSU1.STO.64.H v13 i17 8 || IAU.ADD i17 i17 16


 gaussHx2_fp16_final:
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

bru.jmp i30
nop 6
.size mvcvGaussHx2_fp16_asm,.-mvcvGaussHx2_fp16_asm
.nowarnend
.end
