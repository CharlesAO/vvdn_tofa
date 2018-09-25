///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

//--------------------------------------------------------------------------------------------------------------
.type mvcvBoxfilter9x9_asm,@function
.version 00.50.05
.data .rodata.boxfilter9x9_asm 
.align 16
 boxFilter9x9_multiply:
		.float16    0.012345F16, 255.0F16


.code .text.boxfilter9x9_asm //text
.align 16
.nowarn
mvcvBoxfilter9x9_asm:
//void boxfilter9x9(u8** in, u8** out, u8 normalize, u32 width)//
//                     i18      i17        i16          i15
	LSU0.ldil i5, boxFilter9x9_second || LSU1.ldih i5, boxFilter9x9_second
	LSU0.ldil i13, 0x1 || LSU1.ldih i13, 0x0
	CMU.CMII.U8 i16 i13
	PEU.PC1C NEQ || BRU.JMP i5
nop 6

	IAU.SHR.u32 i15 i15 3
 	lsu0.ldil i9, boxFilter9x9_multiply     ||  lsu1.ldih i9, boxFilter9x9_multiply
	lsu0.ldil i10, boxFilter9x9_loop        ||  lsu1.ldih i10, boxFilter9x9_loop
	LSU0.LD.16R v0, i9  || IAU.ADD i9 i9 2
	LSU0.LD.32  i0  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i1  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i2  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i3  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i4  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i5  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i6  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i7  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i8  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i17  i17 || IAU.SUB i0 i0 8
	LSU0.LD.16R v23, i9  || IAU.SUB i1 i1 8

IAU.SUB i2 i2 8
IAU.SUB i3 i3 8
IAU.SUB i4 i4 8
IAU.SUB i5 i5 8
IAU.SUB i6 i6 8
IAU.SUB i7 i7 8
IAU.SUB i8 i8 8



LSU0.LD.128.u8.u16  v1 i0 || IAU.ADD i0 i0 8
LSU0.LD.128.u8.u16  v2 i1 || IAU.ADD i1 i1 8
LSU0.LD.128.u8.u16  v3 i2 || IAU.ADD i2 i2 8
LSU0.LD.128.u8.u16  v4 i3 || IAU.ADD i3 i3 8
LSU0.LD.128.u8.u16  v5 i4 || IAU.ADD i4 i4 8
LSU0.LD.128.u8.u16  v6 i5 || IAU.ADD i5 i5 8
LSU0.LD.128.u8.u16  v7 i6 || IAU.ADD i6 i6 8
VAU.ACCPZ.u16     v1 || LSU0.LD.128.u8.u16  v8 i7 || IAU.ADD i7 i7 8
VAU.ACCP.u16      v2 || LSU0.LD.128.u8.u16  v9 i8 || IAU.ADD i8 i8 8
VAU.ACCP.u16      v3 || LSU0.LD.128.u8.u16  v1 i0 || IAU.ADD i0 i0 8
VAU.ACCP.u16      v4 || LSU0.LD.128.u8.u16  v2 i1 || IAU.ADD i1 i1 8
VAU.ACCP.u16      v5 || LSU0.LD.128.u8.u16  v3 i2 || IAU.ADD i2 i2 8
VAU.ACCP.u16      v6 || LSU0.LD.128.u8.u16  v4 i3 || IAU.ADD i3 i3 8
VAU.ACCP.u16      v7 || LSU0.LD.128.u8.u16  v5 i4 || IAU.ADD i4 i4 8
VAU.ACCP.u16      v8 || LSU0.LD.128.u8.u16  v6 i5 || IAU.ADD i5 i5 8
VAU.ACCPW.u16 v20 v9 || LSU0.LD.128.u8.u16  v7 i6 || IAU.ADD i6 i6 8
VAU.ACCPZ.u16     v1 || LSU0.LD.128.u8.u16  v8 i7 || IAU.ADD i7 i7 8
VAU.ACCP.u16      v2 || LSU0.LD.128.u8.u16  v9 i8 || IAU.ADD i8 i8 8
VAU.ACCP.u16      v3 || LSU0.LD.128.u8.u16  v1 i0 || IAU.ADD i0 i0 8 
VAU.ACCP.u16      v4 || LSU0.LD.128.u8.u16  v2 i1 || IAU.ADD i1 i1 8
VAU.ACCP.u16      v5 || LSU0.LD.128.u8.u16  v3 i2 || IAU.ADD i2 i2 8
VAU.ACCP.u16      v6 || LSU0.LD.128.u8.u16  v4 i3 || IAU.ADD i3 i3 8
VAU.ACCP.u16      v7 || LSU0.LD.128.u8.u16  v5 i4 || IAU.ADD i4 i4 8
VAU.ACCP.u16      v8 || LSU0.LD.128.u8.u16  v6 i5 || IAU.ADD i5 i5 8
VAU.ACCPW.u16 v21 v9 || LSU0.LD.128.u8.u16  v7 i6 || IAU.ADD i6 i6 8
VAU.ACCPZ.u16     v1 || LSU0.LD.128.u8.u16  v8 i7 || IAU.ADD i7 i7 8
VAU.ACCP.u16      v2 || LSU0.LD.128.u8.u16  v9 i8 || IAU.ADD i8 i8 8
VAU.ACCP.u16      v3 
VAU.ACCP.u16      v4 
VAU.ACCP.u16      v5 
VAU.ACCP.u16      v6 
VAU.ACCP.u16      v7 
VAU.ACCP.u16      v8 
VAU.ACCPW.u16 v22 v9  || cmu.alignvec  v10 v20 v21 8 || bru.rpl i10 i15 
VAU.ACCPZ.u16     v10 || cmu.alignvec  v11 v20 v21 10
VAU.ACCP.u16      v11 || cmu.alignvec  v12 v20 v21 12 
VAU.ACCP.u16      v12 || cmu.alignvec  v13 v20 v21 14 || LSU0.LD.128.u8.u16  v1 i0 || IAU.ADD i0 i0 8
VAU.ACCP.u16      v13 || cmu.alignvec  v14 v21 v22 2  || LSU0.LD.128.u8.u16  v2 i1 || IAU.ADD i1 i1 8
VAU.ACCP.u16      v14 || cmu.alignvec  v15 v21 v22 4  || LSU0.LD.128.u8.u16  v3 i2 || IAU.ADD i2 i2 8
VAU.ACCP.u16      v15 || cmu.alignvec  v16 v21 v22 6  || LSU0.LD.128.u8.u16  v4 i3 || IAU.ADD i3 i3 8
VAU.ACCP.u16      v21 || cmu.alignvec  v17 v21 v22 8  || LSU0.LD.128.u8.u16  v5 i4 || IAU.ADD i4 i4 8
VAU.ACCP.u16      v17 || cmu.cpvv v20 v21             || LSU0.LD.128.u8.u16  v6 i5 || IAU.ADD i5 i5 8
VAU.ACCPW.u16 v18 v16 || cmu.cpvv v21 v22             || LSU0.LD.128.u8.u16  v7 i6 || IAU.ADD i6 i6 8
VAU.ACCPZ.u16     v1  || LSU0.LD.128.u8.u16  v8 i7 || IAU.ADD i7 i7 8
VAU.ACCP.u16      v2 || LSU0.LD.128.u8.u16  v9 i8 || IAU.ADD i8 i8 8 || CMU.CPVV.i16.f16 v19 v18 
 boxFilter9x9_loop:

VAU.ACCP.u16      v3 

vau.mul.f16 v19 v19 v0
VAU.ACCP.u16      v4 
VAU.ACCP.u16      v5 
VAU.ACCP.u16      v6 || CMU.CLAMP0.f16 v19 v19 v23  
VAU.ACCP.u16      v7 
VAU.ACCP.u16      v8 || LSU0.ST.128.f16.u8 v19 i17 || IAU.ADD i17 i17 8

	

	
	
	bru.jmp i30
nop 6
	
	
 boxFilter9x9_second:

	IAU.SHR.u32 i15 i15 3
	lsu0.ldil i10, boxFilter9x9_loop1        ||  lsu1.ldih i10, boxFilter9x9_loop1
	LSU0.LD.32  i0  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i1  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i2  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i3  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i4  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i5  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i6  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i7  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i8  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i17  i17 || IAU.SUB i0 i0 8
	
	
IAU.SUB i1 i1 8
IAU.SUB i2 i2 8
IAU.SUB i3 i3 8
IAU.SUB i4 i4 8
IAU.SUB i5 i5 8
IAU.SUB i6 i6 8
IAU.SUB i7 i7 8
IAU.SUB i8 i8 8



LSU0.LD.128.u8.u16  v1 i0 || IAU.ADD i0 i0 8
LSU0.LD.128.u8.u16  v2 i1 || IAU.ADD i1 i1 8
LSU0.LD.128.u8.u16  v3 i2 || IAU.ADD i2 i2 8
LSU0.LD.128.u8.u16  v4 i3 || IAU.ADD i3 i3 8
LSU0.LD.128.u8.u16  v5 i4 || IAU.ADD i4 i4 8
LSU0.LD.128.u8.u16  v6 i5 || IAU.ADD i5 i5 8
LSU0.LD.128.u8.u16  v7 i6 || IAU.ADD i6 i6 8
VAU.ACCPZ.u16     v1 || LSU0.LD.128.u8.u16  v8 i7 || IAU.ADD i7 i7 8
VAU.ACCP.u16      v2 || LSU0.LD.128.u8.u16  v9 i8 || IAU.ADD i8 i8 8
VAU.ACCP.u16      v3 || LSU0.LD.128.u8.u16  v1 i0 || IAU.ADD i0 i0 8
VAU.ACCP.u16      v4 || LSU0.LD.128.u8.u16  v2 i1 || IAU.ADD i1 i1 8
VAU.ACCP.u16      v5 || LSU0.LD.128.u8.u16  v3 i2 || IAU.ADD i2 i2 8
VAU.ACCP.u16      v6 || LSU0.LD.128.u8.u16  v4 i3 || IAU.ADD i3 i3 8
VAU.ACCP.u16      v7 || LSU0.LD.128.u8.u16  v5 i4 || IAU.ADD i4 i4 8
VAU.ACCP.u16      v8 || LSU0.LD.128.u8.u16  v6 i5 || IAU.ADD i5 i5 8
VAU.ACCPW.u16 v20 v9 || LSU0.LD.128.u8.u16  v7 i6 || IAU.ADD i6 i6 8
VAU.ACCPZ.u16     v1 || LSU0.LD.128.u8.u16  v8 i7 || IAU.ADD i7 i7 8
VAU.ACCP.u16      v2 || LSU0.LD.128.u8.u16  v9 i8 || IAU.ADD i8 i8 8
VAU.ACCP.u16      v3 || LSU0.LD.128.u8.u16  v1 i0 || IAU.ADD i0 i0 8  
VAU.ACCP.u16      v4 || LSU0.LD.128.u8.u16  v2 i1 || IAU.ADD i1 i1 8
VAU.ACCP.u16      v5 || LSU0.LD.128.u8.u16  v3 i2 || IAU.ADD i2 i2 8
VAU.ACCP.u16      v6 || LSU0.LD.128.u8.u16  v4 i3 || IAU.ADD i3 i3 8
VAU.ACCP.u16      v7 || LSU0.LD.128.u8.u16  v5 i4 || IAU.ADD i4 i4 8
VAU.ACCP.u16      v8 || LSU0.LD.128.u8.u16  v6 i5 || IAU.ADD i5 i5 8
VAU.ACCPW.u16 v21 v9 || LSU0.LD.128.u8.u16  v7 i6 || IAU.ADD i6 i6 8
VAU.ACCPZ.u16     v1      || LSU0.LD.128.u8.u16  v8 i7 || IAU.ADD i7 i7 8
VAU.ACCP.u16      v2      || LSU0.LD.128.u8.u16  v9 i8 || IAU.ADD i8 i8 8
VAU.ACCP.u16      v3                                    || LSU0.LD.128.u8.u16  v1 i0 || IAU.ADD i0 i0 8 || bru.rpl i10 i15 
VAU.ACCP.u16      v4                                    || LSU0.LD.128.u8.u16  v2 i1 || IAU.ADD i1 i1 8
VAU.ACCP.u16      v5                                    || LSU0.LD.128.u8.u16  v3 i2 || IAU.ADD i2 i2 8
VAU.ACCP.u16      v6                                    || LSU0.LD.128.u8.u16  v4 i3 || IAU.ADD i3 i3 8
VAU.ACCP.u16      v7                                    || LSU0.LD.128.u8.u16  v5 i4 || IAU.ADD i4 i4 8
VAU.ACCP.u16      v8                                    || LSU0.LD.128.u8.u16  v6 i5 || IAU.ADD i5 i5 8
VAU.ACCPW.u16 v22 v9  || cmu.alignvec  v10 v20 v21 8    || LSU0.LD.128.u8.u16  v7 i6 || IAU.ADD i6 i6 8
VAU.ACCPZ.u16     v10 || cmu.alignvec  v11 v20 v21 10   || LSU0.LD.128.u8.u16  v8 i7 || IAU.ADD i7 i7 8
VAU.ACCP.u16      v11 || cmu.alignvec  v12 v20 v21 12   || LSU0.LD.128.u8.u16  v9 i8 || IAU.ADD i8 i8 8
VAU.ACCP.u16      v12 || cmu.alignvec  v13 v20 v21 14
VAU.ACCP.u16      v13 || cmu.alignvec  v14 v21 v22 2
 boxFilter9x9_loop1:
VAU.ACCP.u16      v14 || cmu.alignvec  v15 v21 v22 4
VAU.ACCP.u16      v15 || cmu.alignvec  v16 v21 v22 6
VAU.ACCP.u16      v21 || cmu.alignvec  v17 v21 v22 8
VAU.ACCP.u16      v17 || cmu.cpvv v20 v21
VAU.ACCPW.u16 v18 v16 || cmu.cpvv v21 v22
VAU.ACCPZ.u16     v1   
VAU.ACCP.u16      v2  || LSU0.STO.64.l v18 i17 0 || LSU1.STO.64.h v18 i17 8 || IAU.ADD i17 i17 16


	
	
	bru.jmp i30
nop 6
.size mvcvBoxfilter9x9_asm,.-mvcvBoxfilter9x9_asm
.nowarnend
.end
