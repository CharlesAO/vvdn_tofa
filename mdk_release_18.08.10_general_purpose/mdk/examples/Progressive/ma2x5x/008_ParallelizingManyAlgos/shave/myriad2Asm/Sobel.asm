///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.version 00.50.05
.data .rodata.sobel
.align 16

.code .text.sobel //text
.align 16

sobel_asm:
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

VAU.sub.f16 v15 v9  v10
VAU.sub.f16 v16 v11 v12
VAU.sub.f16 v17 v13 v14
vau.sub.f16 v21  v9  v13
vau.add.f16 v18 v16 v15                        || LSU0.LD.128.U8F.F16 v2 i0 || IAU.ADD i0 i0 8
vau.add.f16 v19 v17 v16                        || LSU0.LD.128.U8F.F16 v5 i1 || IAU.ADD i1 i1 8
vau.sub.f16 v22  v1   v7                       || LSU0.LD.128.U8F.F16 v8 i2 || IAU.ADD i2 i2 8
vau.sub.f16 v23 v10  v14 || cmu.cpvv v0 v1 
vau.add.f16 v20 v18 v19  || cmu.cpvv v1 v2
vau.add.f16 v24 v21 v22  || cmu.cpvv v3 v4
vau.add.f16 v25 v23 v22  || cmu.cpvv v4 v5
vau.mul.f16 v27 v20 v20  || cmu.cpvv v6 v7
cmu.cpvv v7 v8
                                                  cmu.alignvec v9  v0 v1 14
                                                  cmu.alignvec v10 v1 v2  2
                                                  cmu.alignvec v11 v3 v4 14
                                                  cmu.alignvec v12 v4 v5  2
                                                  cmu.alignvec v13 v6 v7 14
                                                  cmu.alignvec v14 v7 v8  2



vau.add.f16 v26 v24 v25
VAU.sub.f16 v15 v9  v10
VAU.sub.f16 v16 v11 v12

vau.mul.f16 v28 v26 v26
VAU.sub.f16 v17 v13 v14
vau.sub.f16 v21  v9  v13

vau.add.f16 v29 v28 v27
vau.add.f16 v18 v16 v15                        || LSU0.LD.128.U8F.F16 v2 i0 || IAU.ADD i0 i0 8
vau.add.f16 v19 v17 v16                        || LSU0.LD.128.U8F.F16 v5 i1 || IAU.ADD i1 i1 8
SAU.SQT i4  v29.0  || vau.sub.f16 v22  v1   v7                       || LSU0.LD.128.U8F.F16 v8 i2 || IAU.ADD i2 i2 8
SAU.SQT i5  v29.1  || vau.sub.f16 v23 v10  v14 || cmu.cpvv v0 v1
SAU.SQT i6  v29.2  || vau.add.f16 v20 v18 v19  || cmu.cpvv v1 v2
SAU.SQT i7  v29.3  || vau.add.f16 v24 v21 v22  || cmu.cpvv v3 v4
SAU.SQT i8  v29.4  || vau.add.f16 v25 v23 v22  || cmu.cpvv v4 v5
SAU.SQT i9  v29.5 || vau.mul.f16 v27 v20 v20  || cmu.cpvv v6 v7
SAU.SQT i10 v29.6 || cmu.cpvv v7 v8
SAU.SQT i11 v29.7
cmu.alignvec v9  v0 v1 14
cmu.alignvec v10 v1 v2  2
cmu.alignvec v11 v3 v4 14
cmu.alignvec v12 v4 v5  2
cmu.alignvec v13 v6 v7 14
cmu.alignvec v14 v7 v8  2 
//gx & //gy
vau.add.f16 v26 v24 v25
VAU.sub.f16 v15 v9  v10  || IAU.ALIGN i4 i4 i4 2
VAU.sub.f16 v16 v11 v12  || IAU.ALIGN i5 i4 i5 2    //v11
vau.mul.f16 v28 v26 v26  || IAU.ALIGN i6 i6 i6 2    //v12
VAU.sub.f16 v17 v13 v14  || IAU.ALIGN i7 i6 i7 2
vau.sub.f16 v21  v9  v13 || IAU.ALIGN i8 i8 i8 2     //v13
vau.add.f16 v29 v28 v27                                                                           || IAU.ALIGN i9 i8 i9 2 //v9
vau.add.f16 v18 v16 v15                        || LSU0.LD.128.U8F.F16 v2 i0 || IAU.ADD i0 i0 8    || cmu.cpiv.x32 v30.0 i5
vau.add.f16 v19 v17 v16                        || LSU0.LD.128.U8F.F16 v5 i1 || IAU.ADD i1 i1 8    ||                        cmu.cpvv v3 v4
SAU.SQT i4  v29.0  || vau.sub.f16 v22  v1   v7                       || LSU0.LD.128.U8F.F16 v8 i2 || IAU.ADD i2 i2 8 ||     cmu.cpvv v4 v5
SAU.SQT i5  v29.1  || vau.sub.f16 v23 v10  v14 || cmu.cpvv v0 v1     || IAU.ALIGN i10 i10 i10 2  //v14
SAU.SQT i6  v29.2  || vau.add.f16 v20 v18 v19  || cmu.cpvv v1 v2     || IAU.ALIGN i11 i10 i11 2  //v10
SAU.SQT i7  v29.3  || vau.add.f16 v24 v21 v22  ||   cmu.cpvv v6 v7
SAU.SQT i8  v29.4  || vau.add.f16 v25 v23 v22  ||   cmu.cpvv v7 v8
SAU.SQT i9  v29.5 || vau.mul.f16 v27 v20 v20
SAU.SQT i10 v29.6
SAU.SQT i11 v29.7 || cmu.cpiv.x32 v30.1 i7
cmu.cpiv.x32 v30.2 i9
cmu.cpiv.x32 v30.3 i11








cmu.alignvec v12 v4 v5  2  || bru.rpl i12 i16
cmu.alignvec v10 v1 v2  2
vau.add.f16 v26 v24 v25  || cmu.alignvec v9  v0 v1 14
VAU.sub.f16 v15 v9  v10  || IAU.ALIGN i4 i4 i4 2 || cmu.alignvec v11 v3 v4 14
VAU.sub.f16 v16 v11 v12  || IAU.ALIGN i5 i4 i5 2 || cmu.alignvec v14 v7 v8  2
vau.mul.f16 v28 v26 v26  || IAU.ALIGN i6 i6 i6 2 || cmu.alignvec v13 v6 v7 14
VAU.sub.f16 v17 v13 v14  || IAU.ALIGN i7 i6 i7 2
vau.sub.f16 v21  v9  v13 || IAU.ALIGN i8 i8 i8 2        || LSU0.ST.128.f16.u8f v30 i17
vau.add.f16 v29 v28 v27                                                                           || IAU.ALIGN i9 i8 i9 2 //v9
vau.add.f16 v18 v16 v15                        || LSU0.LD.128.U8F.F16 v2 i0 || IAU.ADD i0 i0 8    || cmu.cpiv.x32 v30.0 i5
vau.add.f16 v19 v17 v16                        || LSU0.LD.128.U8F.F16 v5 i1 || IAU.ADD i1 i1 8    ||                        cmu.cpvv v3 v4
SAU.SQT i4  v29.0  || vau.sub.f16 v22  v1   v7                       || LSU0.LD.128.U8F.F16 v8 i2 || IAU.ADD i2 i2 8 ||     cmu.cpvv v4 v5
 sobel_loop:
SAU.SQT i5  v29.1  || vau.sub.f16 v23 v10  v14 || cmu.cpvv v0 v1     || IAU.ALIGN i10 i10 i10 2  //v14
SAU.SQT i6  v29.2  || vau.add.f16 v20 v18 v19  || cmu.cpvv v1 v2     || IAU.ALIGN i11 i10 i11 2  //v10
SAU.SQT i7  v29.3  || vau.add.f16 v24 v21 v22  ||   cmu.cpvv v6 v7    || IAU.ADD i17 i17 8
SAU.SQT i8  v29.4  || vau.add.f16 v25 v23 v22  ||   cmu.cpvv v7 v8
SAU.SQT i9  v29.5 || vau.mul.f16 v27 v20 v20   || cmu.cpiv.x32 v30.1 i7
SAU.SQT i10 v29.6 || cmu.cpiv.x32 v30.2 i9
SAU.SQT i11 v29.7 || cmu.cpiv.x32 v30.3 i11





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



.end
