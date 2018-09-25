///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

//--------------------------------------------------------------------------------------------------------------
.type mvcvBoxfilter7x7_asm,@function
.version 00.70.00
.nowarn
.data .rodata.boxfilter7x7_asm
.align 16
 boxFilter7x7_multiply:
        .float16  0.020408163F16, 255.0F16


// void boxfilter7x7(u8** in (i18), u8** out (i17), u8 normalize (i16), u32 width (i15))
.code .text.boxfilter7x7_asm
.align 16

mvcvBoxfilter7x7_asm:
	LSU0.ldil i5, boxFilter7x7_second || LSU1.ldih i5, boxFilter7x7_second
	LSU0.ldil i13, 0x1 || LSU1.ldih i13, 0x0
	CMU.CMII.U8 i16 i13
	PEU.PC1C NEQ || BRU.JMP i5
nop 6

	IAU.SHR.u32 i15 i15 3
 	lsu0.ldil i7, boxFilter7x7_multiply     ||  lsu1.ldih i7, boxFilter7x7_multiply
	lsu0.ldil i8, boxFilter7x7_loop     ||  lsu1.ldih i8, boxFilter7x7_loop
	LSU0.LD.16R v0, i7  || IAU.ADD i7 i7 2
	LSU0.LD.32  i0  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i1  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i2  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i3  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i4  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i5  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i6  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i17  i17 || IAU.SUB i0 i0 8
	LSU0.LD.16R v17, i7  || IAU.SUB i1 i1 8

IAU.SUB i2 i2 8
IAU.SUB i3 i3 8
IAU.SUB i4 i4 8
IAU.SUB i5 i5 8
IAU.SUB i6 i6 8

LSU0.LD.128.u8.u16  v1 i0 || IAU.ADD i0 i0 8
LSU0.LD.128.u8.u16  v2 i1 || IAU.ADD i1 i1 8
LSU0.LD.128.u8.u16  v3 i2 || IAU.ADD i2 i2 8
LSU0.LD.128.u8.u16  v4 i3 || IAU.ADD i3 i3 8
LSU0.LD.128.u8.u16  v5 i4 || IAU.ADD i4 i4 8
LSU0.LD.128.u8.u16  v6 i5 || IAU.ADD i5 i5 8
LSU0.LD.128.u8.u16  v7 i6 || IAU.ADD i6 i6 8
VAU.ACCPZ.u16     v1 || LSU0.LD.128.u8.u16  v8  i0 || IAU.ADD i0 i0 8
VAU.ACCP.u16      v2 || LSU0.LD.128.u8.u16  v9  i1 || IAU.ADD i1 i1 8
VAU.ACCP.u16      v3 || LSU0.LD.128.u8.u16  v10 i2 || IAU.ADD i2 i2 8
VAU.ACCP.u16      v4 || LSU0.LD.128.u8.u16  v11 i3 || IAU.ADD i3 i3 8
VAU.ACCP.u16      v5 || LSU0.LD.128.u8.u16  v12 i4 || IAU.ADD i4 i4 8
VAU.ACCP.u16      v6 || LSU0.LD.128.u8.u16  v13 i5 || IAU.ADD i5 i5 8
VAU.ACCPW.u16 v21 v7 || LSU0.LD.128.u8.u16  v14 i6 || IAU.ADD i6 i6 8
VAU.ACCPZ.u16     v8   || LSU0.LD.128.u8.u16  v1  i0 || IAU.ADD i0 i0 8    
VAU.ACCP.u16      v9   || LSU0.LD.128.u8.u16  v2  i1 || IAU.ADD i1 i1 8
VAU.ACCP.u16      v10  || LSU0.LD.128.u8.u16  v3  i2 || IAU.ADD i2 i2 8
VAU.ACCP.u16      v11  || LSU0.LD.128.u8.u16  v4  i3 || IAU.ADD i3 i3 8
VAU.ACCP.u16      v12  || LSU0.LD.128.u8.u16  v5  i4 || IAU.ADD i4 i4 8
VAU.ACCP.u16      v13  || LSU0.LD.128.u8.u16  v6  i5 || IAU.ADD i5 i5 8
VAU.ACCPW.u16 v22 v14  || LSU0.LD.128.u8.u16  v7  i6 || IAU.ADD i6 i6 8
VAU.ACCPZ.u16     v1                                 || LSU0.LD.128.u8.u16  v1  i0 || IAU.ADD i0 i0 8 
VAU.ACCP.u16      v2                                 || LSU0.LD.128.u8.u16  v2  i1 || IAU.ADD i1 i1 8
VAU.ACCP.u16      v3                                 || LSU0.LD.128.u8.u16  v3  i2 || IAU.ADD i2 i2 8
VAU.ACCP.u16      v4                                 || LSU0.LD.128.u8.u16  v4  i3 || IAU.ADD i3 i3 8
VAU.ACCP.u16      v5                                 || LSU0.LD.128.u8.u16  v5  i4 || IAU.ADD i4 i4 8
VAU.ACCP.u16      v6                                 || LSU0.LD.128.u8.u16  v6  i5 || IAU.ADD i5 i5 8
VAU.ACCPW.u16 v23 v7  || cmu.alignvec  v8 v21 v22 10 || LSU0.LD.128.u8.u16  v7  i6 || IAU.ADD i6 i6 8
VAU.ACCPZ.u16     v8  || cmu.alignvec  v9 v21 v22 12 || bru.rpl i8 i15 
VAU.ACCP.u16      v9  || cmu.alignvec v10 v21 v22 14
VAU.ACCP.u16      v10 || cmu.alignvec v11 v22 v23  2
VAU.ACCP.u16      v11 || cmu.alignvec v12 v22 v23  4
VAU.ACCP.u16      v12 || cmu.alignvec v13 v22 v23  6
VAU.ACCP.u16      v13 || cmu.cpvv v21 v22
VAU.ACCPW.u16 v14 v22 || cmu.cpvv v22 v23
VAU.ACCPZ.u16     v1 
VAU.ACCP.u16      v2  || CMU.CPVV.i16.f16 v15 v14     || LSU0.LD.128.u8.u16  v1  i0 || IAU.ADD i0 i0 8
 boxFilter7x7_loop: 

VAU.ACCP.u16      v3                                  || LSU0.LD.128.u8.u16  v2  i1 || IAU.ADD i1 i1 8

vau.mul.f16 v16 v15 v0                                || LSU0.LD.128.u8.u16  v3  i2 || IAU.ADD i2 i2 8                                  
VAU.ACCP.u16      v4                                 || LSU0.LD.128.u8.u16  v4  i3 || IAU.ADD i3 i3 8 
VAU.ACCP.u16      v5                                 || LSU0.LD.128.u8.u16  v5  i4 || IAU.ADD i4 i4 8 
VAU.ACCP.u16      v6  || CMU.CLAMP0.f16 v18 v16 v17  || LSU0.LD.128.u8.u16  v6  i5 || IAU.ADD i5 i5 8 
LSU0.LD.128.u8.u16  v7  i6 || IAU.ADD i6 i6 8
VAU.ACCPW.u16 v23 v7  || cmu.alignvec  v8 v21 v22 10 || LSU0.ST.128.f16.u8 v18 i17 || IAU.ADD i17 i17 8


BRU.JMP i30
nop 6
	
 boxFilter7x7_second:

	IAU.SHR.u32 i15 i15 3

	lsu0.ldil i8, boxFilter7x7_loop1     ||  lsu1.ldih i8, boxFilter7x7_loop1
	LSU0.LD.32  i0  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i1  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i2  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i3  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i4  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i5  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i6  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i17  i17 || IAU.SUB i0 i0 8

IAU.SUB i1 i1 8
IAU.SUB i2 i2 8
IAU.SUB i3 i3 8
IAU.SUB i4 i4 8
IAU.SUB i5 i5 8
IAU.SUB i6 i6 8

LSU0.LD.128.u8.u16  v1 i0 || IAU.ADD i0 i0 8
LSU0.LD.128.u8.u16  v2 i1 || IAU.ADD i1 i1 8
LSU0.LD.128.u8.u16  v3 i2 || IAU.ADD i2 i2 8
LSU0.LD.128.u8.u16  v4 i3 || IAU.ADD i3 i3 8
LSU0.LD.128.u8.u16  v5 i4 || IAU.ADD i4 i4 8
LSU0.LD.128.u8.u16  v6 i5 || IAU.ADD i5 i5 8
LSU0.LD.128.u8.u16  v7 i6 || IAU.ADD i6 i6 8
VAU.ACCPZ.u16     v1  || LSU0.LD.128.u8.u16  v8  i0 || IAU.ADD i0 i0 8
VAU.ACCP.u16      v2  || LSU0.LD.128.u8.u16  v9  i1 || IAU.ADD i1 i1 8
VAU.ACCP.u16      v3  || LSU0.LD.128.u8.u16  v10 i2 || IAU.ADD i2 i2 8
VAU.ACCP.u16      v4  || LSU0.LD.128.u8.u16  v11 i3 || IAU.ADD i3 i3 8
VAU.ACCP.u16      v5  || LSU0.LD.128.u8.u16  v12 i4 || IAU.ADD i4 i4 8
VAU.ACCP.u16      v6  || LSU0.LD.128.u8.u16  v13 i5 || IAU.ADD i5 i5 8
VAU.ACCPW.u16 v21 v7  || LSU0.LD.128.u8.u16  v14 i6 || IAU.ADD i6 i6 8
VAU.ACCPZ.u16     v8  || LSU0.LD.128.u8.u16  v1  i0 || IAU.ADD i0 i0 8   
VAU.ACCP.u16      v9  || LSU0.LD.128.u8.u16  v2  i1 || IAU.ADD i1 i1 8
VAU.ACCP.u16      v10 || LSU0.LD.128.u8.u16  v3  i2 || IAU.ADD i2 i2 8
VAU.ACCP.u16      v11 || LSU0.LD.128.u8.u16  v4  i3 || IAU.ADD i3 i3 8
VAU.ACCP.u16      v12 || LSU0.LD.128.u8.u16  v5  i4 || IAU.ADD i4 i4 8
VAU.ACCP.u16      v13 || LSU0.LD.128.u8.u16  v6  i5 || IAU.ADD i5 i5 8
VAU.ACCPW.u16 v22 v14 || LSU0.LD.128.u8.u16  v7  i6 || IAU.ADD i6 i6 8
VAU.ACCPZ.u16     v1 
VAU.ACCP.u16      v2 
VAU.ACCP.u16      v3 || bru.rpl i8 i15  
VAU.ACCP.u16      v4                                 || LSU0.LD.128.u8.u16  v1  i0 || IAU.ADD i0 i0 8
VAU.ACCP.u16      v5                                 || LSU0.LD.128.u8.u16  v2  i1 || IAU.ADD i1 i1 8
VAU.ACCP.u16      v6                                 || LSU0.LD.128.u8.u16  v3  i2 || IAU.ADD i2 i2 8
VAU.ACCPW.u16 v23 v7  || cmu.alignvec  v8 v21 v22 10 || LSU0.LD.128.u8.u16  v4  i3 || IAU.ADD i3 i3 8
VAU.ACCPZ.u16     v8  || cmu.alignvec  v9 v21 v22 12 || LSU0.LD.128.u8.u16  v5  i4 || IAU.ADD i4 i4 8
VAU.ACCP.u16      v9  || cmu.alignvec v10 v21 v22 14 || LSU0.LD.128.u8.u16  v6  i5 || IAU.ADD i5 i5 8
 boxFilter7x7_loop1:  

VAU.ACCP.u16      v10 || cmu.alignvec v11 v22 v23  2 || LSU0.LD.128.u8.u16  v7  i6 || IAU.ADD i6 i6 8

VAU.ACCP.u16      v11 || cmu.alignvec v12 v22 v23  4
VAU.ACCP.u16      v12 || cmu.alignvec v13 v22 v23  6
VAU.ACCP.u16      v13 || cmu.cpvv v21 v22
VAU.ACCPW.u16 v14 v22 || cmu.cpvv v22 v23
VAU.ACCPZ.u16     v1 
VAU.ACCP.u16      v2  || LSU0.STO.64.l v14 i17 0 || LSU1.STO.64.h v14 i17 8 || IAU.ADD i17 i17 16





BRU.JMP i30
nop 6
	
	
.size mvcvBoxfilter7x7_asm,.-mvcvBoxfilter7x7_asm
.nowarnend
.end
