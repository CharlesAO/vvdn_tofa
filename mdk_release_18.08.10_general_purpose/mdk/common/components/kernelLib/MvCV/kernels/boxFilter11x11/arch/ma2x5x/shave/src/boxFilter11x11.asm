///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

//--------------------------------------------------------------------------------------------------------------
.type mvcvBoxfilter11x11_asm,@function
.version 00.50.05
.data .rodata.boxfilter11x11_asm 
.align 16
 boxFilter11x11_multiply:
		.float16    0.008264F16, 255.0F16


.code .text.boxfilter11x11_asm //text
.align 16
.nowarn
mvcvBoxfilter11x11_asm:
//void boxfilter11x11(u8** in, u8** out, u8 normalize, u32 width)//
//                     i18      i17        i16          i15
	LSU0.ldil i5, boxFilter11x11_second || LSU1.ldih i5, boxFilter11x11_second
	LSU0.ldil i13, 0x1 || LSU1.ldih i13, 0x0
	CMU.CMII.U8 i16 i13
	PEU.PC1C NEQ || BRU.JMP i5
nop 6

	IAU.SHR.u32 i15 i15 3
 	lsu0.ldil i11, boxFilter11x11_multiply     ||  lsu1.ldih i11, boxFilter11x11_multiply
	lsu0.ldil i12, boxFilter11x11_loop         ||  lsu1.ldih i12, boxFilter11x11_loop
	LSU0.LD.16R v0, i11  || IAU.ADD i11 i11 2
	LSU0.LD.32  i0  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i1  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i2  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i3  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i4  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i5  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i6  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i7  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i8  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i9  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i10  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i17  i17 || IAU.SUB i0 i0 8
	LSU0.LD.16R v23, i11  || IAU.SUB i1 i1 8
											
    IAU.SUB i2  i2  8
    IAU.SUB i3  i3  8
    IAU.SUB i4  i4  8
    IAU.SUB i5  i5  8
    IAU.SUB i6  i6  8
    IAU.SUB i7  i7  8
    IAU.SUB i8  i8  8
    IAU.SUB i9  i9  8
    IAU.SUB i10 i10 8
	
LSU0.LD.128.u8.u16  v1  i0 || IAU.ADD i0  i0 8
LSU0.LD.128.u8.u16  v2  i1 || IAU.ADD i1  i1 8
LSU0.LD.128.u8.u16  v3  i2 || IAU.ADD i2  i2 8
LSU0.LD.128.u8.u16  v4  i3 || IAU.ADD i3  i3 8
LSU0.LD.128.u8.u16  v5  i4 || IAU.ADD i4  i4 8
LSU0.LD.128.u8.u16  v6  i5 || IAU.ADD i5  i5 8
LSU0.LD.128.u8.u16  v7  i6 || IAU.ADD i6  i6 8
VAU.ACCPZ.u16     v1       || LSU0.LD.128.u8.u16  v8  i7 || IAU.ADD i7  i7 8
VAU.ACCP.u16      v2       || LSU0.LD.128.u8.u16  v9  i8 || IAU.ADD i8  i8 8
VAU.ACCP.u16      v3       || LSU0.LD.128.u8.u16  v10 i9 || IAU.ADD i9  i9 8
VAU.ACCP.u16      v4       || LSU0.LD.128.u8.u16  v11 i10 || IAU.ADD i10 i10 8
VAU.ACCP.u16      v5       || LSU0.LD.128.u8.u16  v1  i0 || IAU.ADD i0  i0 8
VAU.ACCP.u16      v6       || LSU0.LD.128.u8.u16  v2  i1 || IAU.ADD i1  i1 8
VAU.ACCP.u16      v7       || LSU0.LD.128.u8.u16  v3  i2 || IAU.ADD i2  i2 8
VAU.ACCP.u16      v8       || LSU0.LD.128.u8.u16  v4  i3 || IAU.ADD i3  i3 8
VAU.ACCP.u16      v9       || LSU0.LD.128.u8.u16  v5  i4 || IAU.ADD i4  i4 8
VAU.ACCP.u16      v10      || LSU0.LD.128.u8.u16  v6  i5 || IAU.ADD i5  i5 8
VAU.ACCPW.u16 v20 v11      || LSU0.LD.128.u8.u16  v7  i6 || IAU.ADD i6  i6 8
VAU.ACCPZ.u16     v1       || LSU0.LD.128.u8.u16  v8  i7 || IAU.ADD i7  i7 8
VAU.ACCP.u16      v2       || LSU0.LD.128.u8.u16  v9  i8 || IAU.ADD i8  i8 8
VAU.ACCP.u16      v3       || LSU0.LD.128.u8.u16  v10 i9 || IAU.ADD i9  i9 8
VAU.ACCP.u16      v4       || LSU0.LD.128.u8.u16  v11 i10 || IAU.ADD i10 i10 8
VAU.ACCP.u16      v5       || LSU0.LD.128.u8.u16  v1  i0 || IAU.ADD i0  i0 8 
VAU.ACCP.u16      v6       || LSU0.LD.128.u8.u16  v2  i1 || IAU.ADD i1  i1 8
VAU.ACCP.u16      v7       || LSU0.LD.128.u8.u16  v3  i2 || IAU.ADD i2  i2 8
VAU.ACCP.u16      v8       || LSU0.LD.128.u8.u16  v4  i3 || IAU.ADD i3  i3 8
VAU.ACCP.u16      v9       || LSU0.LD.128.u8.u16  v5  i4 || IAU.ADD i4  i4 8
VAU.ACCP.u16      v10      || LSU0.LD.128.u8.u16  v6  i5 || IAU.ADD i5  i5 8
VAU.ACCPW.u16 v21 v11      || LSU0.LD.128.u8.u16  v7  i6 || IAU.ADD i6  i6 8
VAU.ACCPZ.u16     v1       || LSU0.LD.128.u8.u16  v8  i7 || IAU.ADD i7  i7 8
VAU.ACCP.u16      v2       || LSU0.LD.128.u8.u16  v9  i8 || IAU.ADD i8  i8 8
VAU.ACCP.u16      v3       || LSU0.LD.128.u8.u16  v10 i9 || IAU.ADD i9  i9 8
VAU.ACCP.u16      v4       || LSU0.LD.128.u8.u16  v11 i10 || IAU.ADD i10 i10 8   
VAU.ACCP.u16      v5   
VAU.ACCP.u16      v6   
VAU.ACCP.u16      v7   
VAU.ACCP.u16      v8   
VAU.ACCP.u16      v9  || bru.rpl i12 i15 
VAU.ACCP.u16      v10  
VAU.ACCPW.u16 v22 v11 || cmu.alignvec  v12 v20 v21 6 
VAU.ACCPZ.u16     v12 || cmu.alignvec  v13 v20 v21 8 
VAU.ACCP.u16      v13 || cmu.alignvec  v14 v20 v21 10
VAU.ACCP.u16      v14 || cmu.alignvec  v15 v20 v21 12
VAU.ACCP.u16      v15 || cmu.alignvec  v16 v20 v21 14
VAU.ACCP.u16      v16 || cmu.alignvec  v17 v21 v22 2   || LSU0.LD.128.u8.u16  v1  i0 || IAU.ADD i0  i0 8 
VAU.ACCP.u16      v21 || cmu.alignvec  v18 v21 v22 4   || LSU0.LD.128.u8.u16  v2  i1 || IAU.ADD i1  i1 8
VAU.ACCP.u16      v17 || cmu.alignvec  v19 v21 v22 6   || LSU0.LD.128.u8.u16  v3  i2 || IAU.ADD i2  i2 8
VAU.ACCP.u16      v18 || cmu.alignvec  v15 v21 v22 8   || LSU0.LD.128.u8.u16  v4  i3 || IAU.ADD i3  i3 8
VAU.ACCP.u16      v19 || cmu.alignvec  v16 v21 v22 10  || LSU0.LD.128.u8.u16  v5  i4 || IAU.ADD i4  i4 8
VAU.ACCP.u16      v15 || cmu.cpvv v20 v21              || LSU0.LD.128.u8.u16  v6  i5 || IAU.ADD i5  i5 8
VAU.ACCPW.u16 v12 v16 || cmu.cpvv v21 v22              || LSU0.LD.128.u8.u16  v7  i6 || IAU.ADD i6  i6 8
VAU.ACCPZ.u16     v1  || LSU0.LD.128.u8.u16  v8  i7 || IAU.ADD i7  i7 8
VAU.ACCP.u16      v2  || LSU0.LD.128.u8.u16  v9  i8 || IAU.ADD i8  i8 8 || CMU.CPVV.i16.f16 v13 v12 
 boxFilter11x11_loop:

VAU.ACCP.u16      v3 || LSU0.LD.128.u8.u16  v10 i9 || IAU.ADD i9  i9 8

vau.mul.f16 v13 v13 v0
VAU.ACCP.u16      v4 || LSU0.LD.128.u8.u16  v11 i10 || IAU.ADD i10 i10 8 
VAU.ACCP.u16      v5
VAU.ACCP.u16      v6 || CMU.CLAMP0.f16 v13 v13 v23  
VAU.ACCP.u16      v7 
VAU.ACCP.u16      v8 || LSU0.ST.128.f16.u8 v13 i17 || IAU.ADD i17 i17 8
	
	bru.jmp i30
nop 6
	
	
 boxFilter11x11_second:
	IAU.SHR.u32 i15 i15 3
	lsu0.ldil i12, boxFilter11x11_loop1         ||  lsu1.ldih i12, boxFilter11x11_loop1
	LSU0.LD.32  i0  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i1  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i2  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i3  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i4  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i5  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i6  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i7  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i8  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i9  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i10  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i17  i17 || IAU.SUB i0 i0 8
	IAU.SUB i1 i1 8
											
    IAU.SUB i2  i2  8
    IAU.SUB i3  i3  8
    IAU.SUB i4  i4  8
    IAU.SUB i5  i5  8
    IAU.SUB i6  i6  8
    IAU.SUB i7  i7  8
    IAU.SUB i8  i8  8
    IAU.SUB i9  i9  8
    IAU.SUB i10 i10 8
	
LSU0.LD.128.u8.u16  v1  i0 || IAU.ADD i0  i0 8
LSU0.LD.128.u8.u16  v2  i1 || IAU.ADD i1  i1 8
LSU0.LD.128.u8.u16  v3  i2 || IAU.ADD i2  i2 8
LSU0.LD.128.u8.u16  v4  i3 || IAU.ADD i3  i3 8
LSU0.LD.128.u8.u16  v5  i4 || IAU.ADD i4  i4 8
LSU0.LD.128.u8.u16  v6  i5 || IAU.ADD i5  i5 8
LSU0.LD.128.u8.u16  v7  i6 || IAU.ADD i6  i6 8
VAU.ACCPZ.u16     v1 || LSU0.LD.128.u8.u16  v8  i7 || IAU.ADD i7  i7 8
VAU.ACCP.u16      v2 || LSU0.LD.128.u8.u16  v9  i8 || IAU.ADD i8  i8 8
VAU.ACCP.u16      v3 || LSU0.LD.128.u8.u16  v10 i9 || IAU.ADD i9  i9 8
VAU.ACCP.u16      v4 || LSU0.LD.128.u8.u16  v11 i10 || IAU.ADD i10 i10 8 
VAU.ACCP.u16      v5  || LSU0.LD.128.u8.u16  v1  i0 || IAU.ADD i0  i0 8 
VAU.ACCP.u16      v6  || LSU0.LD.128.u8.u16  v2  i1 || IAU.ADD i1  i1 8
VAU.ACCP.u16      v7  || LSU0.LD.128.u8.u16  v3  i2 || IAU.ADD i2  i2 8
VAU.ACCP.u16      v8  || LSU0.LD.128.u8.u16  v4  i3 || IAU.ADD i3  i3 8
VAU.ACCP.u16      v9  || LSU0.LD.128.u8.u16  v5  i4 || IAU.ADD i4  i4 8
VAU.ACCP.u16      v10 || LSU0.LD.128.u8.u16  v6  i5 || IAU.ADD i5  i5 8
VAU.ACCPW.u16 v20 v11 || LSU0.LD.128.u8.u16  v7  i6 || IAU.ADD i6  i6 8
VAU.ACCPZ.u16     v1  || LSU0.LD.128.u8.u16  v8  i7 || IAU.ADD i7  i7 8
VAU.ACCP.u16      v2  || LSU0.LD.128.u8.u16  v9  i8 || IAU.ADD i8  i8 8
VAU.ACCP.u16      v3  || LSU0.LD.128.u8.u16  v10 i9 || IAU.ADD i9  i9 8
VAU.ACCP.u16      v4  || LSU0.LD.128.u8.u16  v11 i10 || IAU.ADD i10 i10 8
VAU.ACCP.u16      v5  || LSU0.LD.128.u8.u16  v1  i0 || IAU.ADD i0  i0 8 
VAU.ACCP.u16      v6  || LSU0.LD.128.u8.u16  v2  i1 || IAU.ADD i1  i1 8
VAU.ACCP.u16      v7  || LSU0.LD.128.u8.u16  v3  i2 || IAU.ADD i2  i2 8
VAU.ACCP.u16      v8  || LSU0.LD.128.u8.u16  v4  i3 || IAU.ADD i3  i3 8
VAU.ACCP.u16      v9  || LSU0.LD.128.u8.u16  v5  i4 || IAU.ADD i4  i4 8
VAU.ACCP.u16      v10 || LSU0.LD.128.u8.u16  v6  i5 || IAU.ADD i5  i5 8
VAU.ACCPW.u16 v21 v11 || LSU0.LD.128.u8.u16  v7  i6 || IAU.ADD i6  i6 8
                      LSU0.LD.128.u8.u16  v8  i7 || IAU.ADD i7  i7 8
                      LSU0.LD.128.u8.u16  v9  i8 || IAU.ADD i8  i8 8
                      LSU0.LD.128.u8.u16  v10 i9 || IAU.ADD i9  i9 8
                      LSU0.LD.128.u8.u16  v11 i10 || IAU.ADD i10 i10 8
VAU.ACCPZ.u16     v1       
VAU.ACCP.u16      v2       
VAU.ACCP.u16      v3 || bru.rpl i12 i15      
VAU.ACCP.u16      v4                                   || LSU0.LD.128.u8.u16  v1  i0 || IAU.ADD i0  i0 8 
VAU.ACCP.u16      v5                                   || LSU0.LD.128.u8.u16  v2  i1 || IAU.ADD i1  i1 8
VAU.ACCP.u16      v6                                   || LSU0.LD.128.u8.u16  v3  i2 || IAU.ADD i2  i2 8
VAU.ACCP.u16      v7                                   || LSU0.LD.128.u8.u16  v4  i3 || IAU.ADD i3  i3 8
VAU.ACCP.u16      v8                                   || LSU0.LD.128.u8.u16  v5  i4 || IAU.ADD i4  i4 8
VAU.ACCP.u16      v9                                   || LSU0.LD.128.u8.u16  v6  i5 || IAU.ADD i5  i5 8
VAU.ACCP.u16      v10                                  || LSU0.LD.128.u8.u16  v7  i6 || IAU.ADD i6  i6 8
VAU.ACCPW.u16 v22 v11 || cmu.alignvec  v12 v20 v21 6   || LSU0.LD.128.u8.u16  v8  i7 || IAU.ADD i7  i7 8
VAU.ACCPZ.u16     v12 || cmu.alignvec  v13 v20 v21 8   || LSU0.LD.128.u8.u16  v9  i8 || IAU.ADD i8  i8 8
VAU.ACCP.u16      v13 || cmu.alignvec  v14 v20 v21 10  || LSU0.LD.128.u8.u16  v10 i9 || IAU.ADD i9  i9 8
VAU.ACCP.u16      v14 || cmu.alignvec  v15 v20 v21 12  || LSU0.LD.128.u8.u16  v11 i10 || IAU.ADD i10 i10 8
VAU.ACCP.u16      v15 || cmu.alignvec  v16 v20 v21 14
VAU.ACCP.u16      v16 || cmu.alignvec  v17 v21 v22 2 
VAU.ACCP.u16      v21 || cmu.alignvec  v18 v21 v22 4 
 boxFilter11x11_loop1:

VAU.ACCP.u16      v17 || cmu.alignvec  v19 v21 v22 6

VAU.ACCP.u16      v18 || cmu.alignvec  v15 v21 v22 8
VAU.ACCP.u16      v19 || cmu.alignvec  v16 v21 v22 10
VAU.ACCP.u16      v15 || cmu.cpvv v20 v21  
VAU.ACCPW.u16 v12 v16 || cmu.cpvv v21 v22  
VAU.ACCPZ.u16     v1    
VAU.ACCP.u16      v2  || LSU0.STO.64.l v12 i17 0 || LSU1.STO.64.h v12 i17 8 || IAU.ADD i17 i17 16


												 
	bru.jmp i30
nop 6
	
	
	
.size mvcvBoxfilter11x11_asm,.-mvcvBoxfilter11x11_asm
.nowarnend
.end
