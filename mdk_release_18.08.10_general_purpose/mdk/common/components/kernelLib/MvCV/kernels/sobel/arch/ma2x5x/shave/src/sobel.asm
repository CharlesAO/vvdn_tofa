///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvSobel_asm,@function
.version 00.50.05
.data .rodata.sobel
.align 16

.code .text.sobel //text
.align 16

mvcvSobel_asm:
//void sobel(u8** in, u8** out, u32 width)
//             i18      i17        i16    
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

    
	lsu0.ldil i12, sobel_loop     ||  lsu1.ldih i12, sobel_loop
	LSU0.LDi.64  i0  i18 
	LSU0.LD.32  i2  i18 
	LSU0.LD.32  i17  i17 
	nop 6
	IAU.SHR.u32 i16 i16 3


IAU.SUB i0 i0 8
IAU.SUB i1 i1 8
IAU.SUB i2 i2 8



LSU0.LD.128.U8F.F16 v0 i0 || IAU.ADD i0 i0 8
LSU0.LD.128.U8F.F16 v3 i1 || IAU.ADD i1 i1 8
LSU0.LD.128.U8F.F16 v6 i2 || IAU.ADD i2 i2 8
              
LSU0.LD.128.U8F.F16 v1 i0 || IAU.ADD i0 i0 8
LSU0.LD.128.U8F.F16 v4 i1 || IAU.ADD i1 i1 8
LSU0.LD.128.U8F.F16 v7 i2 || IAU.ADD i2 i2 8
              
LSU0.LD.128.U8F.F16 v2 i0 || IAU.ADD i0 i0 8 
LSU0.LD.128.U8F.F16 v5 i1 || IAU.ADD i1 i1 8
LSU0.LD.128.U8F.F16 v8 i2 || IAU.ADD i2 i2 8
nop 6


cmu.alignvec v9  v0 v1 14 
cmu.alignvec v10 v1 v2  2 

cmu.alignvec v11 v3 v4 14 
cmu.alignvec v12 v4 v5  2 

cmu.alignvec v13 v6 v7 14 
cmu.alignvec v14 v7 v8  2 

//gx & //gy
VAU.sub.f16 v15 v9  v10 
VAU.sub.f16 v16 v11 v12
VAU.sub.f16 v17 v13 v14
vau.sub.f16 v21  v9  v13
vau.add.f16 v18 v16 v15
vau.add.f16 v19 v17 v16
vau.sub.f16 v22  v1   v7
vau.sub.f16 v23 v10  v14 || cmu.cpvv v0 v1 
vau.add.f16 v20 v18 v19  || cmu.cpvv v1 v2 || bru.rpl i12 i16
vau.add.f16 v24 v21 v22  || cmu.cpvv v3 v4 || LSU0.LD.128.U8F.F16 v2 i0 || IAU.ADD i0 i0 8
vau.add.f16 v25 v23 v22  || cmu.cpvv v4 v5
vau.mul.f16 v27 v20 v20  || cmu.cpvv v6 v7 || LSU0.LD.128.U8F.F16 v5 i1 || IAU.ADD i1 i1 8
cmu.cpvv v7 v8
vau.add.f16 v26 v24 v25 ||LSU0.LD.128.U8F.F16 v8 i2 || IAU.ADD i2 i2 8
nop 2
vau.mul.f16 v28 v26 v26 || cmu.alignvec v9  v0 v1 14 
cmu.alignvec v10 v1 v2  2 
cmu.alignvec v11 v3 v4 14 || VAU.sub.f16 v15 v9  v10
vau.add.f16 v29 v28 v27 || cmu.alignvec v12 v4 v5  2 
cmu.alignvec v13 v6 v7 14 || VAU.sub.f16 v16 v11 v12
cmu.alignvec v14 v7 v8  2 
SAU.SQT i4  v29.0 || VAU.sub.f16 v17 v13 v14
SAU.SQT i5  v29.1 || vau.sub.f16 v21  v9  v13
SAU.SQT i6  v29.2 || vau.add.f16 v18 v16 v15
SAU.SQT i7  v29.3 || vau.add.f16 v19 v17 v16
SAU.SQT i8  v29.4 || vau.sub.f16 v22  v1   v7
SAU.SQT i9  v29.5 || CMU.CPIV.x16  v29.0   i4.l   
SAU.SQT i10 v29.6 || CMU.CPIV.x16  v29.1   i5.l   
 sobel_loop:
SAU.SQT i11 v29.7 || CMU.CPIV.x16  v29.2   i6.l   
				     CMU.CPIV.x16  v29.3   i7.l   
				     CMU.CPIV.x16  v29.4   i8.l   
				     CMU.CPIV.x16  v29.5   i9.l   
				     CMU.CPIV.x16  v29.6  i10.l   
				     CMU.CPIV.x16  v29.7  i11.l   
LSU0.ST.128.f16.u8f v29 i17 || IAU.ADD i17 i17 8 || vau.sub.f16 v23 v10  v14 || cmu.cpvv v0 v1 






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
	nop 6
	bru.jmp i30
nop 6

	
	
.size mvcvSobel_asm,.-mvcvSobel_asm
.end
