///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

//--------------------------------------------------------------------------------------------------------------
.type mvcvCornerMinEigenVal_fp32_asm,@function
.version 00.50.05
.data .rodata.CornerMinEigenVal_fp32
.align 16
 cornerMinEigenVal_fp32_multiply:
		.float32    0.111111F32, 0.5F32, 1.5F32, 255.0F32


.code .text.CornerMinEigenVal_fp32 //text
.align 16

mvcvCornerMinEigenVal_fp32_asm:
//CornerMinEigenVal_fp32(u8 **in_lines, fp32 **out_line, fp32 *buff u32 width)//
//                            i18            i17                i16      i15
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
	LSU0.ST.64.h v31  i19  || IAU.SUB i19 i19 4
	LSU0.ST.32  i20  i19   || IAU.SUB i19 i19 4
	LSU0.ST.32  i21  i19   || IAU.SUB i19 i19 4
	LSU0.ST.32  i22  i19   || IAU.SUB i19 i19 4
	LSU0.ST.32  i23  i19

	LSU0.LD.32  i0  i18 || IAU.ADD i18 i18 4 //line 0
	LSU0.LD.32  i1  i18 || IAU.ADD i18 i18 4 //line 1
	LSU0.LD.32  i2  i18 || IAU.ADD i18 i18 4 //line 2
	LSU0.LD.32  i3  i18 || IAU.ADD i18 i18 4 //line 3
	LSU0.LD.32  i4  i18 || IAU.ADD i18 i18 4 //line 4
	LSU0.LD.32  i17  i17
	nop 6


	lsu0.ldil i14, cornerMinEigenVal_fp32_multiply     ||  lsu1.ldih i14, cornerMinEigenVal_fp32_multiply
	LSU0.LD.32r   v31   i14 || IAU.ADD i14 i14 4
	LSU0.LD.32r   v30   i14 || IAU.ADD i14 i14 4
	LSU0.LD.32r   v29   i14 || IAU.ADD i14 i14 4
	LSU0.LD.32r   v28   i14


	IAU.ADD i5 i16 8
	IAU.ADD i6 i5 i15
	IAU.ADD i6 i6 i15
	IAU.ADD i6 i6 0x10
	IAU.ADD i7 i6 i15
	IAU.ADD i7 i7 i15
	IAU.ADD i7 i7 0x10
	IAU.ADD i8 i7 i15
	IAU.ADD i8 i8 i15
	IAU.ADD i8 i8 0x10
	IAU.ADD i9 i8 i15
	IAU.ADD i9 i9 i15
	IAU.ADD i9 i9 0x10
	IAU.ADD i10 i9 i15
	IAU.ADD i10 i10 i15
	IAU.ADD i10 i10 0x10
	IAU.SUB i0 i0 8
	IAU.SUB i1 i1 8
	IAU.SUB i2 i2 8
	IAU.SUB i3 i3 8
	IAU.SUB i4 i4 8

lsu0.ldil i13, cornerMinEigenVal_fp32_loop     ||  lsu1.ldih i13, cornerMinEigenVal_fp32_loop
IAU.SHR.u32 i12 i15 3
IAU.ADD i12 i12 1

	LSU0.LD.128.u8.f16 v0  i0  || IAU.ADD  i0 i0 8
	LSU0.LD.128.u8.f16 v1  i0
	LSU0.LD.128.u8.f16 v2  i1  || IAU.ADD  i1 i1 8
	LSU0.LD.128.u8.f16 v3  i1
	LSU0.LD.128.u8.f16 v4  i2  || IAU.ADD  i2 i2 8
	LSU0.LD.128.u8.f16 v5  i2  || bru.rpl i13 i12
	LSU0.LD.128.u8.f16 v6  i3  || IAU.ADD  i3 i3 8
	LSU0.LD.128.u8.f16 v7  i3
	LSU0.LD.128.u8.f16 v8  i4  || IAU.ADD  i4 i4 8 || cmu.alignvec v10 v0 v1 12
	LSU0.LD.128.u8.f16 v9  i4  || cmu.alignvec v11 v0 v1 14

	cmu.alignvec v12 v2 v3 12
	cmu.alignvec v13 v2 v3 14
	cmu.alignvec v14 v4 v5 12  //Gx
	cmu.alignvec v15 v4 v5 14 || VAU.ADD.f16 v20 v10 v14
	cmu.alignvec v16 v6 v7 12
	cmu.alignvec v17 v6 v7 14 || VAU.ADD.f16 v21 v12 v16
	cmu.alignvec v18 v8 v9 12
	cmu.alignvec v19 v8 v9 14 || VAU.ADD.f16 v22 v14 v18

Vau.add.f16 v20 v20 v12
Vau.add.f16 v21 v21 v14
Vau.add.f16 v22 v22 v16
Vau.add.f16 v20 v20 v12
Vau.add.f16 v21 v21 v14
Vau.add.f16 v22 v22 v16
Vau.sub.f16 v20 v20 v1
Vau.sub.f16 v21 v21 v3
Vau.sub.f16 v22 v22 v5
Vau.sub.f16 v20 v20 v5
Vau.sub.f16 v21 v21 v7
Vau.sub.f16 v22 v22 v9
Vau.sub.f16 v20 v20 v3
Vau.sub.f16 v21 v21 v5
Vau.sub.f16 v22 v22 v7
Vau.sub.f16 v20 v20 v3
Vau.sub.f16 v21 v21 v5
Vau.sub.f16 v22 v22 v7

//Gy
VAU.ADD.f16 v23 v10 v1
VAU.ADD.f16 v24 v12 v3
VAU.ADD.f16 v25 v14 v5
vau.add.f16 v23 v23 v11
vau.add.f16 v24 v24 v13
VAU.ADD.f16 v25 v25 v15
vau.add.f16 v23 v23 v11
vau.add.f16 v24 v24 v13
VAU.ADD.f16 v25 v25 v15
vau.sub.f16 v23 v23 v14
vau.sub.f16 v24 v24 v16
vau.sub.f16 v25 v25 v18
vau.sub.f16 v23 v23 v5
vau.sub.f16 v24 v24 v7
vau.sub.f16 v25 v25 v9
vau.sub.f16 v23 v23 v15 || LSU0.LD.128.u8.f16 v0  i0  || IAU.ADD  i0 i0 8
vau.sub.f16 v24 v24 v17 || LSU0.LD.128.u8.f16 v1  i0
vau.sub.f16 v25 v25 v19 || LSU0.LD.128.u8.f16 v2  i1  || IAU.ADD  i1 i1 8
vau.sub.f16 v23 v23 v15 || LSU0.LD.128.u8.f16 v3  i1
vau.sub.f16 v24 v24 v17 || LSU0.LD.128.u8.f16 v4  i2  || IAU.ADD  i2 i2 8
 cornerMinEigenVal_fp32_loop:
 .nowarn 10
vau.sub.f16 v25 v25 v19
.nowarnend
LSU0.STO.64.L v20 i5  0 || LSU1.STO.64.H v20 i5  8 || IAU.ADD i5  i5  16
LSU0.STO.64.L v21 i6  0 || LSU1.STO.64.H v21 i6  8 || IAU.ADD i6  i6  16
LSU0.STO.64.L v22 i7  0 || LSU1.STO.64.H v22 i7  8 || IAU.ADD i7  i7  16
LSU0.STO.64.L v23 i8  0 || LSU1.STO.64.H v23 i8  8 || IAU.ADD i8  i8  16
LSU0.STO.64.L v24 i9  0 || LSU1.STO.64.H v24 i9  8 || IAU.ADD i9  i9  16
LSU0.STO.64.L v25 i10 0 || LSU1.STO.64.H v25 i10 8 || IAU.ADD i10 i10 16

IAU.SUB i5 i5 i15
IAU.SUB i5 i5 i15
IAU.SUB i5 i5 16
IAU.SUB i6 i6 i15
IAU.SUB i6 i6 i15
IAU.SUB i6 i6 16
IAU.SUB i7 i7 i15
IAU.SUB i7 i7 i15
IAU.SUB i7 i7 16
IAU.SUB i8 i8 i15
IAU.SUB i8 i8 i15
IAU.SUB i8 i8 16
IAU.SUB i9 i9 i15
IAU.SUB i9 i9 i15
IAU.SUB i9 i9 16
IAU.SUB i10 i10 i15
IAU.SUB i10 i10 i15
IAU.SUB i10 i10 16


lsu0.ldil i13, cornerMinEigenVal_fp32_loop2     ||  lsu1.ldih i13, cornerMinEigenVal_fp32_loop2
IAU.SHR.u32 i12 i15 2



LSU0.LD.128.f16.f32  v1  i5  || IAU.ADD i5  i5  8 || bru.rpl i13 i12
LSU0.LD.128.f16.f32  v2  i6  || IAU.ADD i6  i6  8 || LSU1.LD.128.f16.f32  v7  i5  //dx2
LSU0.LD.128.f16.f32  v3  i7  || IAU.ADD i7  i7  8 || LSU1.LD.128.f16.f32  v8  i6  //dx3
LSU0.LD.128.f16.f32  v4  i8  || IAU.ADD i8  i8  8 || LSU1.LD.128.f16.f32  v9  i7  //dy1
LSU0.LD.128.f16.f32  v5  i9  || IAU.ADD i9  i9  8 || LSU1.LD.128.f16.f32  v10 i8  //dy2
LSU0.LD.128.f16.f32  v6  i10 || IAU.ADD i10 i10 8 || LSU1.LD.128.f16.f32  v11 i9  //dy3
LSU1.LD.128.f16.f32  v12 i10                                                     //
VAU.XOR v25 v25 v25
VAU.XOR v26 v26 v26
VAU.XOR v27 v27 v27

VAU.mul.f32 v13  v1   v1
VAU.mul.f32 v14  v7    v7
VAU.mul.f32 v15  v4   v4
VAU.mul.f32 v16  v10   v10
VAU.mul.f32 v17  v1   v4   || cmu.alignvec v19 v13 v14 4
VAU.mul.f32 v18  v7    v10 || cmu.alignvec v20 v13 v14 8
cmu.alignvec v21 v15 v16 4 || VAu.add.f32  v25 v25 v13
cmu.alignvec v22 v15 v16 8 || VAu.add.f32  v26 v26 v15
cmu.alignvec v23 v17 v18 4 || VAu.add.f32  v27 v27 v17
cmu.alignvec v24 v17 v18 8 || VAu.add.f32  v25 v25 v19
VAu.add.f32  v26 v26 v21
VAu.add.f32  v27 v27 v23
VAu.add.f32  v25 v25 v20
VAu.add.f32  v26 v26 v22
VAu.add.f32  v27 v27 v24

VAU.mul.f32 v13  v2    v2
VAU.mul.f32 v14  v8    v8
VAU.mul.f32 v15  v5    v5
VAU.mul.f32 v16  v11   v11
VAU.mul.f32 v17  v2    v5   || cmu.alignvec v19 v13 v14 4
VAU.mul.f32 v18  v8    v11  || cmu.alignvec v20 v13 v14 8
cmu.alignvec v21 v15 v16 4 || VAu.add.f32  v25 v25 v13
cmu.alignvec v22 v15 v16 8 || VAu.add.f32  v26 v26 v15
cmu.alignvec v23 v17 v18 4 || VAu.add.f32  v27 v27 v17
cmu.alignvec v24 v17 v18 8 || VAu.add.f32  v25 v25 v19
VAu.add.f32  v26 v26 v21
VAu.add.f32  v27 v27 v23
VAu.add.f32  v25 v25 v20
VAu.add.f32  v26 v26 v22
VAu.add.f32  v27 v27 v24




VAU.mul.f32 v13  v3    v3
VAU.mul.f32 v14  v9    v9
VAU.mul.f32 v15  v6    v6
VAU.mul.f32 v16  v12   v12
VAU.mul.f32 v17  v3    v6   || cmu.alignvec v19 v13 v14 4
VAU.mul.f32 v18  v9    v12  || cmu.alignvec v20 v13 v14 8
cmu.alignvec v21 v15 v16 4 || VAu.add.f32  v25 v25 v13
cmu.alignvec v22 v15 v16 8 || VAu.add.f32  v26 v26 v15
cmu.alignvec v23 v17 v18 4 || VAu.add.f32  v27 v27 v17
cmu.alignvec v24 v17 v18 8 || VAu.add.f32  v25 v25 v19
VAu.add.f32  v26 v26 v21 ||lsu0.ldil i11, 0x59df      ||  lsu1.ldih i11, 0x5f37
VAu.add.f32  v27 v27 v23 || CMU.CPIVR.x32  v5   i11
VAu.add.f32  v25 v25 v20 || lsu0.ldil i11, 0x0     ||  lsu1.ldih i11, 0x7f80
VAu.add.f32  v26 v26 v22 || CMU.CPIVR.x32  v6   i11
VAu.add.f32  v27 v27 v24




VAU.MUl.f32 v25 v25 v31
VAU.MUl.f32 v26 v26 v31
VAU.MUl.f32 v27 v27 v31     //b
VAU.MUl.f32 v25 v25 v30		//a
VAU.MUl.f32 v26 v26 v30	    //c
nop



VAU.mul.f32 v0 v27 v27     //b*b
vau.sub.f32 v1 v25 v26     //a-c
vau.add.f32 v2 v25 v26     //a+c
nop
VAU.mul.f32 v3 v1  v1	   //a-c * a-c
nop 2
VAU.ADD.f32 v4 v3  v0      // (a-c) * (a-c)	+ b*b
nop 2
vau.mul.f32 v7 v4 v30   //number * 0.5
nop 2
VAU.SHR.u32 v8 v4 1 // j >> 1
nop 2
vau.sub.i32 v9 v5 v8 //0x5f3759df - ( j >> 1 )//
nop 2
vau.mul.f32 v10 v9 v9
nop 2
vau.mul.f32 v11 v10 v7
nop 2
vau.sub.f32 v12 v29 v11
nop 2
vau.mul.f32 v13 v9 v12
nop 2
vau.mul.f32 v14 v13 v13
nop 2
vau.mul.f32 v15 v14 v7
nop 2
vau.sub.f32 v16 v29 v15
nop 2
vau.mul.f32 v17 v13 v16
nop 2
vau.mul.f32 v18 v17 v4
nop 2
vau.sub.f32 v19 v2 v18
nop 2

CMU.CMVV.u32 v19 v6
PEU.PVV32 gt || vau.xor v19 v19 v19
cornerMinEigenVal_fp32_loop2:
LSU0.STO.64.L  v19 i17  0    || LSU1.STO.64.H v19 i17  8 || IAU.ADD i17 i17 16
nop 6


LSU0.LD.32  i23  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i22  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i21  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i20  i19 || IAU.ADD i19 i19 4

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
.size mvcvCornerMinEigenVal_fp32_asm,.-mvcvCornerMinEigenVal_fp32_asm
.end