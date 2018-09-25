///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

//--------------------------------------------------------------------------------------------------------------
.type mvcvBoxfilter5x5_asm,@function
.version 00.50.05
.data .rodata.vect_mask 
.align 16
 boxFilter5x5_multiply:
		.float16    0.04F16, 255.0F16


.code .text.boxfilter5x5 //text
.align 16
.nowarn
mvcvBoxfilter5x5_asm:
//void boxfilter5x5(u8** in, u8** out, u8 normalize, u32 width)//
//                     i18      i17        i16          i15
	LSU0.ldil i5, boxFilter5x5_second || LSU1.ldih i5, boxFilter5x5_second
	LSU0.ldil i13, 0x1 || LSU1.ldih i13, 0x0
	CMU.CMII.U8 i16 i13
	PEU.PC1C NEQ || BRU.JMP i5
nop 6

IAU.SHR.u32 i15 i15 3
lsu0.ldil i7, boxFilter5x5_multiply     ||  lsu1.ldih i7, boxFilter5x5_multiply
lsu0.ldil i8, boxFilter5x5_loop     ||  lsu1.ldih i8, boxFilter5x5_loop
	LSU0.LD.32  i0  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i1  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i2  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i3  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i4  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i17  i17
	LSU0.LD.16R v0, i7 || IAU.ADD i7 i7 2

IAU.SUB i0 i0 8
IAU.SUB i1 i1 8
IAU.SUB i2 i2 8
IAU.SUB i3 i3 8
IAU.SUB i4 i4 8


LSU0.LD.128.u8.u16  v1 i0 || IAU.ADD i0 i0 8
LSU0.LD.128.u8.u16  v2 i1 || IAU.ADD i1 i1 8
LSU0.LD.128.u8.u16  v3 i2 || IAU.ADD i2 i2 8
LSU0.LD.128.u8.u16  v4 i3 || IAU.ADD i3 i3 8
LSU0.LD.128.u8.u16  v5 i4 || IAU.ADD i4 i4 8
LSU0.LD.128.u8.u16   v6  i0 || IAU.ADD i0 i0 8
LSU0.LD.128.u8.u16   v7  i1 || IAU.ADD i1 i1 8
VAU.ACCPZ.u16     v1 || LSU0.LD.128.u8.u16   v8  i2 || IAU.ADD i2 i2 8
VAU.ACCP.u16      v2 || LSU0.LD.128.u8.u16   v9  i3 || IAU.ADD i3 i3 8
VAU.ACCP.u16      v3 || LSU0.LD.128.u8.u16  v10  i4 || IAU.ADD i4 i4 8
VAU.ACCP.u16      v4   || LSU0.LD.128.u8.u16  v11 i0 || IAU.ADD i0 i0 8 
VAU.ACCPW.u16 v16 v5   || LSU0.LD.128.u8.u16  v12 i1 || IAU.ADD i1 i1 8
VAU.ACCPZ.u16      v6  || LSU0.LD.128.u8.u16  v13 i2 || IAU.ADD i2 i2 8
VAU.ACCP.u16       v7  || LSU0.LD.128.u8.u16  v14 i3 || IAU.ADD i3 i3 8
VAU.ACCP.u16       v8  || LSU0.LD.128.u8.u16  v15 i4 || IAU.ADD i4 i4 8
VAU.ACCP.u16       v9
VAU.ACCPW.u16 v17 v10
LSU0.LD.16R v1, i7


VAU.ACCPZ.u16     v11                                || LSU0.LD.128.u8.u16  v11 i0 || IAU.ADD i0 i0 8     
VAU.ACCP.u16      v12                                || LSU0.LD.128.u8.u16  v12 i1 || IAU.ADD i1 i1 8
VAU.ACCP.u16      v13                                || LSU0.LD.128.u8.u16  v13 i2 || IAU.ADD i2 i2 8
VAU.ACCP.u16      v14                                || LSU0.LD.128.u8.u16  v14 i3 || IAU.ADD i3 i3 8
VAU.ACCPW.u16 v18 v15 || cmu.alignvec v19 v16 v17 12 || LSU0.LD.128.u8.u16  v15 i4 || IAU.ADD i4 i4 8
VAU.ACCPZ.u16     v19 || cmu.alignvec v20 v16 v17 14 
VAU.ACCP.u16      v20 || cmu.alignvec v21 v17 v18  2 || bru.rpl i8 i15    
VAU.ACCP.u16      v21 || cmu.alignvec v22 v17 v18  4 
VAU.ACCP.u16      v22 || cmu.cpvv v16 v17            
VAU.ACCPW.u16 v23 v17 || cmu.cpvv v17 v18 
VAU.ACCPZ.u16     v11 
VAU.ACCP.u16      v12  || CMU.CPVV.i16.f16 v6 v23     || LSU0.LD.128.u8.u16  v11 i0 || IAU.ADD i0 i0 8 
 boxFilter5x5_loop:                                              
VAU.ACCP.u16      v13                                 || LSU0.LD.128.u8.u16  v12 i1 || IAU.ADD i1 i1 8
vau.mul.f16 v8 v6 v0                                  || LSU0.LD.128.u8.u16  v13 i2 || IAU.ADD i2 i2 8
VAU.ACCP.u16      v14                                 || LSU0.LD.128.u8.u16  v14 i3 || IAU.ADD i3 i3 8
VAU.ACCPW.u16 v18 v15 || cmu.alignvec v19 v16 v17 12  || LSU0.LD.128.u8.u16  v15 i4 || IAU.ADD i4 i4 8
CMU.CLAMP0.f16 v8 v8 v1
nop
VAU.ACCPZ.u16     v19 || cmu.alignvec v20 v16 v17 14  || LSU0.ST.128.f16.u8 v8 i17 || IAU.ADD i17 i17 8




	
	
	bru.jmp i30
nop 6

 boxFilter5x5_second:
	

	IAU.SHR.u32 i15 i15 3

lsu0.ldil i8, boxFilter5x5_loop1     ||  lsu1.ldih i8, boxFilter5x5_loop1
	LSU0.LD.32  i0  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i1  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i2  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i3  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i4  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i17  i17
nop 
IAU.SUB i0 i0 8
IAU.SUB i1 i1 8
IAU.SUB i2 i2 8
IAU.SUB i3 i3 8
IAU.SUB i4 i4 8


LSU0.LD.128.u8.u16  v1 i0 || IAU.ADD i0 i0 8
LSU0.LD.128.u8.u16  v2 i1 || IAU.ADD i1 i1 8
LSU0.LD.128.u8.u16  v3 i2 || IAU.ADD i2 i2 8
LSU0.LD.128.u8.u16  v4 i3 || IAU.ADD i3 i3 8
LSU0.LD.128.u8.u16  v5 i4 || IAU.ADD i4 i4 8
LSU0.LD.128.u8.u16   v6  i0 || IAU.ADD i0 i0 8
LSU0.LD.128.u8.u16   v7  i1 || IAU.ADD i1 i1 8
VAU.ACCPZ.u16     v1 || LSU0.LD.128.u8.u16   v8  i2 || IAU.ADD i2 i2 8
VAU.ACCP.u16      v2 || LSU0.LD.128.u8.u16   v9  i3 || IAU.ADD i3 i3 8
VAU.ACCP.u16      v3 || LSU0.LD.128.u8.u16  v10  i4 || IAU.ADD i4 i4 8
VAU.ACCP.u16      v4  || LSU0.LD.128.u8.u16  v11 i0 || IAU.ADD i0 i0 8 
VAU.ACCPW.u16 v16 v5  || LSU0.LD.128.u8.u16  v12 i1 || IAU.ADD i1 i1 8
VAU.ACCPZ.u16      v6 || LSU0.LD.128.u8.u16  v13 i2 || IAU.ADD i2 i2 8
VAU.ACCP.u16       v7 || LSU0.LD.128.u8.u16  v14 i3 || IAU.ADD i3 i3 8
VAU.ACCP.u16       v8 || LSU0.LD.128.u8.u16  v15 i4 || IAU.ADD i4 i4 8
VAU.ACCP.u16       v9
VAU.ACCPW.u16 v17 v10
VAU.ACCPZ.u16     v11
VAU.ACCP.u16      v12 
VAU.ACCP.u16      v13 || bru.rpl i8 i15
VAU.ACCP.u16      v14                                 || LSU0.LD.128.u8.u16  v11 i0 || IAU.ADD i0 i0 8 
VAU.ACCPW.u16 v18 v15 || cmu.alignvec v19 v16 v17 12  || LSU0.LD.128.u8.u16  v12 i1 || IAU.ADD i1 i1 8
 boxFilter5x5_loop1:

VAU.ACCPZ.u16     v19 || cmu.alignvec v20 v16 v17 14  || LSU0.LD.128.u8.u16  v13 i2 || IAU.ADD i2 i2 8

VAU.ACCP.u16      v20 || cmu.alignvec v21 v17 v18  2  || LSU0.LD.128.u8.u16  v14 i3 || IAU.ADD i3 i3 8
VAU.ACCP.u16      v21 || cmu.alignvec v22 v17 v18  4  || LSU0.LD.128.u8.u16  v15 i4 || IAU.ADD i4 i4 8
VAU.ACCP.u16      v22 || cmu.cpvv v16 v17
VAU.ACCPW.u16 v23 v17 || cmu.cpvv v17 v18
VAU.ACCPZ.u16     v11
VAU.ACCP.u16      v12 || LSU0.STO.64.l v23 i17 0 || LSU1.STO.64.h v23 i17 8 || IAU.ADD i17 i17 16


	
	bru.jmp i30
nop 6
.size mvcvBoxfilter5x5_asm,.-mvcvBoxfilter5x5_asm
.nowarnend
.end
