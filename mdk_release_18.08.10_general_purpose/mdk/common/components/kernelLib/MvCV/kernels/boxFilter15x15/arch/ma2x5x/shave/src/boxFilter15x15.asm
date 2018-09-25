///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

//--------------------------------------------------------------------------------------------------------------
.type mvcvBoxfilter15x15_asm,@function
.version 00.50.05
.data .rodata.boxfilter15x15_asm 
.align 16

 boxFilter15x15_multiply:
		.float32    0.00001742919, 255

.nowarn
.code .text.boxfilter15x15_asm //text
.align 16

mvcvBoxfilter15x15_asm:
//void boxfilter13x13(u8** in, u8** out, u8 normalize, u32 width)//
//                     i18      i17        i16          i15
IAU.SUB i19 i19 8
LSU0.ST.64.l v23  i19 || IAU.SUB i19 i19 8
LSU0.ST.64.h v23  i19

	LSU0.ldil i5, boxFilter15x15_second || LSU1.ldih i5, boxFilter15x15_second
	LSU0.ldil i13, 0x1 || LSU1.ldih i13, 0x0
	CMU.CMII.U8 i16 i13
	PEU.PC1C NEQ || BRU.JMP i5
nop 6

	IAU.SHR.u32 i15 i15 3
 	lsu0.ldil i16, boxFilter15x15_multiply     ||  lsu1.ldih i16, boxFilter15x15_multiply
	LSU0.LD.32R v0, i16  || IAU.ADD i16 i16 4
	LSU0.LD.32R v23, i16 
	lsu0.ldil i16, boxFilter15x15_loop         ||  lsu1.ldih i16, boxFilter15x15_loop
	
	LSU0.LD.32  i0   i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i1   i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i2   i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i3   i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i4   i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i5   i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i6   i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i7   i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i8   i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i9   i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i10  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i11  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i12  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i13  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i14  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i17  i17  || IAU.SUB i0  i0  8 

	
	IAU.SUB i1  i1  8
    IAU.SUB i2  i2  8
    IAU.SUB i3  i3  8
    IAU.SUB i4  i4  8
    IAU.SUB i5  i5  8
    IAU.SUB i6  i6  8
    IAU.SUB i7  i7  8
    IAU.SUB i8  i8  8
    IAU.SUB i9  i9  8
    IAU.SUB i10 i10 8
    IAU.SUB i11 i11 8
    IAU.SUB i12 i12 8
    IAU.SUB i13 i13 8
    IAU.SUB i14 i14 8
	IAU.SUB i18 i18 i18
	

	
LSU0.LD.128.u8.u16  v1  i0  || IAU.ADD i0  i0  8
LSU0.LD.128.u8.u16  v2  i1  || IAU.ADD i1  i1  8
LSU0.LD.128.u8.u16  v3  i2  || IAU.ADD i2  i2  8
LSU0.LD.128.u8.u16  v4  i3  || IAU.ADD i3  i3  8
LSU0.LD.128.u8.u16  v5  i4  || IAU.ADD i4  i4  8
LSU0.LD.128.u8.u16  v6  i5  || IAU.ADD i5  i5  8
LSU0.LD.128.u8.u16  v7  i6  || IAU.ADD i6  i6  8

VAU.ACCPZ.u16     v1 || LSU0.LD.128.u8.u16  v8  i7  || IAU.ADD i7  i7  8
VAU.ACCP.u16      v2 || LSU0.LD.128.u8.u16  v9  i8  || IAU.ADD i8  i8  8
VAU.ACCP.u16      v3 || LSU0.LD.128.u8.u16  v10 i9  || IAU.ADD i9  i9  8
VAU.ACCP.u16      v4 || LSU0.LD.128.u8.u16  v11 i10 || IAU.ADD i10 i10 8
VAU.ACCP.u16      v5 || LSU0.LD.128.u8.u16  v12 i11 || IAU.ADD i11 i11 8
VAU.ACCP.u16      v6 || LSU0.LD.128.u8.u16  v13 i12 || IAU.ADD i12 i12 8
VAU.ACCP.u16      v7 || LSU0.LD.128.u8.u16  v14 i13 || IAU.ADD i13 i13 8
VAU.ACCP.u16      v8 || LSU0.LD.128.u8.u16  v15 i14 || IAU.ADD i14 i14 8
VAU.ACCP.u16      v9   || LSU0.LD.128.u8.u16  v1  i0  || IAU.ADD i0  i0  8
VAU.ACCP.u16      v10  || LSU0.LD.128.u8.u16  v2  i1  || IAU.ADD i1  i1  8
VAU.ACCP.u16      v11  || LSU0.LD.128.u8.u16  v3  i2  || IAU.ADD i2  i2  8
VAU.ACCP.u16      v12  || LSU0.LD.128.u8.u16  v4  i3  || IAU.ADD i3  i3  8
VAU.ACCP.u16      v13  || LSU0.LD.128.u8.u16  v5  i4  || IAU.ADD i4  i4  8
VAU.ACCP.u16      v14  || LSU0.LD.128.u8.u16  v6  i5  || IAU.ADD i5  i5  8
VAU.ACCPW.u16 v20 v15  || LSU0.LD.128.u8.u16  v7  i6  || IAU.ADD i6  i6  8   
VAU.ACCPZ.u16     v1  || LSU0.LD.128.u8.u16  v8  i7  || IAU.ADD i7  i7  8 
VAU.ACCP.u16      v2  || LSU0.LD.128.u8.u16  v9  i8  || IAU.ADD i8  i8  8
VAU.ACCP.u16      v3  || LSU0.LD.128.u8.u16  v10 i9  || IAU.ADD i9  i9  8
VAU.ACCP.u16      v4  || LSU0.LD.128.u8.u16  v11 i10 || IAU.ADD i10 i10 8
VAU.ACCP.u16      v5  || LSU0.LD.128.u8.u16  v12 i11 || IAU.ADD i11 i11 8
VAU.ACCP.u16      v6  || LSU0.LD.128.u8.u16  v13 i12 || IAU.ADD i12 i12 8
VAU.ACCP.u16      v7  || LSU0.LD.128.u8.u16  v14 i13 || IAU.ADD i13 i13 8
VAU.ACCP.u16      v8  || LSU0.LD.128.u8.u16  v15 i14 || IAU.ADD i14 i14 8
VAU.ACCP.u16      v9  || LSU0.LD.128.u8.u16  v1  i0  || IAU.ADD i0  i0  8 
VAU.ACCP.u16      v10 || LSU0.LD.128.u8.u16  v2  i1  || IAU.ADD i1  i1  8
VAU.ACCP.u16      v11 || LSU0.LD.128.u8.u16  v3  i2  || IAU.ADD i2  i2  8
VAU.ACCP.u16      v12 || LSU0.LD.128.u8.u16  v4  i3  || IAU.ADD i3  i3  8
VAU.ACCP.u16      v13 || LSU0.LD.128.u8.u16  v5  i4  || IAU.ADD i4  i4  8
VAU.ACCP.u16      v14 || LSU0.LD.128.u8.u16  v6  i5  || IAU.ADD i5  i5  8
VAU.ACCPW.u16 v21 v15 || LSU0.LD.128.u8.u16  v7  i6  || IAU.ADD i6  i6  8   
VAU.ACCPZ.u16     v1 || LSU0.LD.128.u8.u16  v8  i7  || IAU.ADD i7  i7  8
VAU.ACCP.u16      v2 || LSU0.LD.128.u8.u16  v9  i8  || IAU.ADD i8  i8  8
VAU.ACCP.u16      v3 || LSU0.LD.128.u8.u16  v10 i9  || IAU.ADD i9  i9  8
VAU.ACCP.u16      v4 || LSU0.LD.128.u8.u16  v11 i10 || IAU.ADD i10 i10 8
VAU.ACCP.u16      v5 || LSU0.LD.128.u8.u16  v12 i11 || IAU.ADD i11 i11 8
VAU.ACCP.u16      v6 || LSU0.LD.128.u8.u16  v13 i12 || IAU.ADD i12 i12 8
VAU.ACCP.u16      v7 || LSU0.LD.128.u8.u16  v14 i13 || IAU.ADD i13 i13 8
VAU.ACCP.u16      v8 || LSU0.LD.128.u8.u16  v15 i14 || IAU.ADD i14 i14 8
VAU.ACCP.u16      v9    
VAU.ACCP.u16      v10   
VAU.ACCP.u16      v11  || bru.rpl i16 i15 
VAU.ACCP.u16      v12   
VAU.ACCP.u16      v13   
VAU.ACCP.u16      v14   
VAU.ACCPW.u16 v22 v15  || cmu.alignvec  v1  v20 v21 2   
VAU.ACCPZ.u16     v1   || cmu.alignvec  v2  v20 v21 4 
VAU.ACCP.u16      v2   || cmu.alignvec  v3  v20 v21 6 
VAU.ACCP.u16      v3   || cmu.alignvec  v4  v20 v21 8 
VAU.ACCP.u16      v4   || cmu.alignvec  v5  v20 v21 10
VAU.ACCP.u16      v5   || cmu.alignvec  v6  v20 v21 12
VAU.ACCP.u16      v6   || cmu.alignvec  v7  v20 v21 14
VAU.ACCP.u16      v7   || cmu.alignvec  v8  v21 v22 2 
VAU.ACCP.u16      v8   || cmu.alignvec  v9  v21 v22 4 
VAU.ACCP.u16      v9   || cmu.alignvec  v10 v21 v22 6  || LSU0.LD.128.u8.u16  v1  i0  || IAU.ADD i0  i0  8 
VAU.ACCP.u16      v10  || cmu.alignvec  v11 v21 v22 8  || LSU0.LD.128.u8.u16  v2  i1  || IAU.ADD i1  i1  8
VAU.ACCP.u16      v11  || cmu.alignvec  v12 v21 v22 10 || LSU0.LD.128.u8.u16  v3  i2  || IAU.ADD i2  i2  8
VAU.ACCP.u16      v12  || cmu.alignvec  v13 v21 v22 12 || LSU0.LD.128.u8.u16  v4  i3  || IAU.ADD i3  i3  8
VAU.ACCP.u16      v13  || cmu.alignvec  v14 v21 v22 14 || LSU0.LD.128.u8.u16  v5  i4  || IAU.ADD i4  i4  8
VAU.ACCP.u16      v14  || cmu.cpvv v20 v21             || LSU0.LD.128.u8.u16  v6  i5  || IAU.ADD i5  i5  8
VAU.ACCPW.u16 v15 v21  || cmu.cpvv v21 v22             || LSU0.LD.128.u8.u16  v7  i6  || IAU.ADD i6  i6  8
VAU.ACCPZ.u16     v1 || LSU0.LD.128.u8.u16  v8  i7  || IAU.ADD i7  i7  8 || cmu.cpivr.x32 v16 i18
VAU.ACCP.u16      v2 || LSU0.LD.128.u8.u16  v9  i8  || IAU.ADD i8  i8  8 || cmu.vilv.x16 v17, v18, v16, v15
VAU.ACCP.u16      v3 || LSU0.LD.128.u8.u16  v10 i9  || IAU.ADD i9  i9  8 || CMU.CPVV.i32.f32 v17 v17
VAU.ACCP.u16      v4 || LSU0.LD.128.u8.u16  v11 i10 || IAU.ADD i10 i10 8 || CMU.CPVV.i32.f32 v18 v18
vau.mul.f32 v17 v17 v0
 boxFilter15x15_loop:
vau.mul.f32 v18 v18 v0
VAU.ACCP.u16      v5 || LSU0.LD.128.u8.u16  v12 i11 || IAU.ADD i11 i11 8
VAU.ACCP.u16      v6 || LSU0.LD.128.u8.u16  v13 i12 || IAU.ADD i12 i12 8
VAU.ACCP.u16      v7 || LSU0.LD.128.u8.u16  v14 i13 || IAU.ADD i13 i13 8 ||CMU.CPVV.f32.f16 v18 v18
VAU.ACCP.u16      v8 || LSU0.LD.128.u8.u16  v15 i14 || IAU.ADD i14 i14 8 ||cmu.alignvec v18 v18 v18 8
VAU.ACCP.u16      v9 || CMU.CPVV.f32.f16 v18 v17
VAU.ACCP.u16      v10   || LSU0.ST.128.f16.u8f v18 i17 || IAU.ADD i17 i17 8



LSU0.LD.64.h  v23  i19 || IAU.ADD i19 i19 8    
LSU0.LD.64.l  v23  i19 || IAU.ADD i19 i19 8
bru.jmp i30
nop 6
	
	
 boxFilter15x15_second:

	IAU.SHR.u32 i15 i15 3
	lsu0.ldil i16, boxFilter15x15_loop1         ||  lsu1.ldih i16, boxFilter15x15_loop1
	
	LSU0.LD.32  i0   i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i1   i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i2   i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i3   i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i4   i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i5   i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i6   i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i7   i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i8   i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i9   i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i10  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i11  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i12  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i13  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i14  i18  || IAU.ADD i18 i18 4
	LSU0.LD.32  i17  i17  || 	IAU.SUB i0  i0  8


	IAU.SUB i1  i1  8
    IAU.SUB i2  i2  8
    IAU.SUB i3  i3  8
    IAU.SUB i4  i4  8
    IAU.SUB i5  i5  8
    IAU.SUB i6  i6  8
    IAU.SUB i7  i7  8
    IAU.SUB i8  i8  8
    IAU.SUB i9  i9  8
    IAU.SUB i10 i10 8
    IAU.SUB i11 i11 8
    IAU.SUB i12 i12 8
    IAU.SUB i13 i13 8
    IAU.SUB i14 i14 8
	IAU.SUB i18 i18 i18
	

	
LSU0.LD.128.u8.u16  v1  i0  || IAU.ADD i0  i0  8
LSU0.LD.128.u8.u16  v2  i1  || IAU.ADD i1  i1  8
LSU0.LD.128.u8.u16  v3  i2  || IAU.ADD i2  i2  8
LSU0.LD.128.u8.u16  v4  i3  || IAU.ADD i3  i3  8
LSU0.LD.128.u8.u16  v5  i4  || IAU.ADD i4  i4  8
LSU0.LD.128.u8.u16  v6  i5  || IAU.ADD i5  i5  8
LSU0.LD.128.u8.u16  v7  i6  || IAU.ADD i6  i6  8
VAU.ACCPZ.u16     v1  || LSU0.LD.128.u8.u16  v8  i7  || IAU.ADD i7  i7  8
VAU.ACCP.u16      v2  || LSU0.LD.128.u8.u16  v9  i8  || IAU.ADD i8  i8  8
VAU.ACCP.u16      v3  || LSU0.LD.128.u8.u16  v10 i9  || IAU.ADD i9  i9  8
VAU.ACCP.u16      v4  || LSU0.LD.128.u8.u16  v11 i10 || IAU.ADD i10 i10 8
VAU.ACCP.u16      v5  || LSU0.LD.128.u8.u16  v12 i11 || IAU.ADD i11 i11 8
VAU.ACCP.u16      v6  || LSU0.LD.128.u8.u16  v13 i12 || IAU.ADD i12 i12 8
VAU.ACCP.u16      v7  || LSU0.LD.128.u8.u16  v14 i13 || IAU.ADD i13 i13 8
VAU.ACCP.u16      v8  || LSU0.LD.128.u8.u16  v15 i14 || IAU.ADD i14 i14 8
VAU.ACCP.u16      v9  || LSU0.LD.128.u8.u16  v1  i0  || IAU.ADD i0  i0  8
VAU.ACCP.u16      v10 || LSU0.LD.128.u8.u16  v2  i1  || IAU.ADD i1  i1  8
VAU.ACCP.u16      v11 || LSU0.LD.128.u8.u16  v3  i2  || IAU.ADD i2  i2  8
VAU.ACCP.u16      v12 || LSU0.LD.128.u8.u16  v4  i3  || IAU.ADD i3  i3  8
VAU.ACCP.u16      v13 || LSU0.LD.128.u8.u16  v5  i4  || IAU.ADD i4  i4  8
VAU.ACCP.u16      v14 || LSU0.LD.128.u8.u16  v6  i5  || IAU.ADD i5  i5  8
VAU.ACCPW.u16 v20 v15 || LSU0.LD.128.u8.u16  v7  i6  || IAU.ADD i6  i6  8
VAU.ACCPZ.u16     v1  || LSU0.LD.128.u8.u16  v8  i7  || IAU.ADD i7  i7  8
VAU.ACCP.u16      v2  || LSU0.LD.128.u8.u16  v9  i8  || IAU.ADD i8  i8  8
VAU.ACCP.u16      v3  || LSU0.LD.128.u8.u16  v10 i9  || IAU.ADD i9  i9  8
VAU.ACCP.u16      v4  || LSU0.LD.128.u8.u16  v11 i10 || IAU.ADD i10 i10 8
VAU.ACCP.u16      v5  || LSU0.LD.128.u8.u16  v12 i11 || IAU.ADD i11 i11 8
VAU.ACCP.u16      v6  || LSU0.LD.128.u8.u16  v13 i12 || IAU.ADD i12 i12 8
VAU.ACCP.u16      v7  || LSU0.LD.128.u8.u16  v14 i13 || IAU.ADD i13 i13 8
VAU.ACCP.u16      v8  || LSU0.LD.128.u8.u16  v15 i14 || IAU.ADD i14 i14 8
VAU.ACCP.u16      v9  
VAU.ACCP.u16      v10 
VAU.ACCP.u16      v11 
VAU.ACCP.u16      v12 
VAU.ACCP.u16      v13 
VAU.ACCP.u16      v14 
VAU.ACCPW.u16 v21 v15 


LSU0.LD.128.u8.u16  v1  i0  || IAU.ADD i0  i0  8     
LSU0.LD.128.u8.u16  v2  i1  || IAU.ADD i1  i1  8
LSU0.LD.128.u8.u16  v3  i2  || IAU.ADD i2  i2  8
LSU0.LD.128.u8.u16  v4  i3  || IAU.ADD i3  i3  8
LSU0.LD.128.u8.u16  v5  i4  || IAU.ADD i4  i4  8
LSU0.LD.128.u8.u16  v6  i5  || IAU.ADD i5  i5  8
LSU0.LD.128.u8.u16  v7  i6  || IAU.ADD i6  i6  8 
LSU0.LD.128.u8.u16  v8  i7  || IAU.ADD i7  i7  8
LSU0.LD.128.u8.u16  v9  i8  || IAU.ADD i8  i8  8
LSU0.LD.128.u8.u16  v10 i9  || IAU.ADD i9  i9  8
LSU0.LD.128.u8.u16  v11 i10 || IAU.ADD i10 i10 8
LSU0.LD.128.u8.u16  v12 i11 || IAU.ADD i11 i11 8
LSU0.LD.128.u8.u16  v13 i12 || IAU.ADD i12 i12 8
LSU0.LD.128.u8.u16  v14 i13 || IAU.ADD i13 i13 8
LSU0.LD.128.u8.u16  v15 i14 || IAU.ADD i14 i14 8

VAU.ACCPZ.u16     v1    
VAU.ACCP.u16      v2    
VAU.ACCP.u16      v3 || bru.rpl i16 i15   
VAU.ACCP.u16      v4    
VAU.ACCP.u16      v5    
VAU.ACCP.u16      v6    
VAU.ACCP.u16      v7    
VAU.ACCP.u16      v8    
VAU.ACCP.u16      v9    
VAU.ACCP.u16      v10   
VAU.ACCP.u16      v11   
VAU.ACCP.u16      v12   
VAU.ACCP.u16      v13   
VAU.ACCP.u16      v14   
VAU.ACCPW.u16 v22 v15  || cmu.alignvec  v1  v20 v21 2   
VAU.ACCPZ.u16     v1   || cmu.alignvec  v2  v20 v21 4  || LSU0.LD.128.u8.u16  v1  i0  || IAU.ADD i0  i0  8 
VAU.ACCP.u16      v2   || cmu.alignvec  v3  v20 v21 6  || LSU0.LD.128.u8.u16  v2  i1  || IAU.ADD i1  i1  8 
VAU.ACCP.u16      v3   || cmu.alignvec  v4  v20 v21 8  || LSU0.LD.128.u8.u16  v3  i2  || IAU.ADD i2  i2  8 
VAU.ACCP.u16      v4   || cmu.alignvec  v5  v20 v21 10 || LSU0.LD.128.u8.u16  v4  i3  || IAU.ADD i3  i3  8 
VAU.ACCP.u16      v5   || cmu.alignvec  v6  v20 v21 12 || LSU0.LD.128.u8.u16  v5  i4  || IAU.ADD i4  i4  8 
VAU.ACCP.u16      v6   || cmu.alignvec  v7  v20 v21 14 || LSU0.LD.128.u8.u16  v6  i5  || IAU.ADD i5  i5  8 
VAU.ACCP.u16      v7   || cmu.alignvec  v8  v21 v22 2  || LSU0.LD.128.u8.u16  v7  i6  || IAU.ADD i6  i6  8  
VAU.ACCP.u16      v8   || cmu.alignvec  v9  v21 v22 4  || LSU0.LD.128.u8.u16  v8  i7  || IAU.ADD i7  i7  8 
VAU.ACCP.u16      v9   || cmu.alignvec  v10 v21 v22 6  || LSU0.LD.128.u8.u16  v9  i8  || IAU.ADD i8  i8  8 
VAU.ACCP.u16      v10  || cmu.alignvec  v11 v21 v22 8  || LSU0.LD.128.u8.u16  v10 i9  || IAU.ADD i9  i9  8 
 boxFilter15x15_loop1:

VAU.ACCP.u16      v11  || cmu.alignvec  v12 v21 v22 10 || LSU0.LD.128.u8.u16  v11 i10 || IAU.ADD i10 i10 8 

VAU.ACCP.u16      v12  || cmu.alignvec  v13 v21 v22 12 || LSU0.LD.128.u8.u16  v12 i11 || IAU.ADD i11 i11 8 
VAU.ACCP.u16      v13  || cmu.alignvec  v14 v21 v22 14 || LSU0.LD.128.u8.u16  v13 i12 || IAU.ADD i12 i12 8 
VAU.ACCP.u16      v14  || cmu.cpvv v20 v21             || LSU0.LD.128.u8.u16  v14 i13 || IAU.ADD i13 i13 8 
VAU.ACCPW.u16 v15 v21  || cmu.cpvv v21 v22             || LSU0.LD.128.u8.u16  v15 i14 || IAU.ADD i14 i14 8  
VAU.ACCPZ.u16     v1                              
VAU.ACCP.u16      v2   || LSU0.STO.64.l v15 i17 0 || LSU1.STO.64.h v15 i17 8 || IAU.ADD i17 i17 16

LSU0.LD.64.h  v23  i19 || IAU.ADD i19 i19 8    
LSU0.LD.64.l  v23  i19 || IAU.ADD i19 i19 8
bru.jmp i30
nop 6
	
.size mvcvBoxfilter15x15_asm,.-mvcvBoxfilter15x15_asm
.nowarnend
.end
