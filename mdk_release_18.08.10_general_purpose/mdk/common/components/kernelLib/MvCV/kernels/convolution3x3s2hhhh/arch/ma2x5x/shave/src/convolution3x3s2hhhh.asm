///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvConvolution3x3s2hhhh_asm,@function
.version 00.50.00

//.data .data.convolution3x3s2hhhh
//.align 4

//.align 16
 //convolution3x3s2hhhh_clampVal:
  //      .float16 255.0

.nowarn
.code .text.convolution3x3s2hhhh

//void Convolution3x3s2hhhh_asm(half** in(i18), half** out(i17), half conv[9](i16), u32 inWidth(i15))
//internal computation are made on fp16, output result is fp16
mvcvConvolution3x3s2hhhh_asm:

    LSU1.LDIL i13 ___conv3x3s2hhhhLoop || LSU0.LDIH i13 ___conv3x3s2hhhhLoop
    LSU0.LDI.64  i0  i18               || LSU1.LDXV v22 i16     || IAU.XOR i3 i3 i3
    LSU0.LDI.32  i2  i18               || IAU.SHR.u32 i14 i15 4
    LSU0.LDo.64.l v23 i16 0x10         || LSU1.LD.32 i17 i17    || IAU.INCS i3 8
    IAU.INCS i19 -16
    LSU1.STXV v24 i19 || IAU.INCS i19 -16
    LSU0.STXV v25 i19 || IAU.INCS i19 -16
    LSU1.STXV v26 i19 || IAU.AND i3 i3 i15
    IAU.INCS i0 -16
    IAU.INCS i1 -16
    IAU.INCS i2 -16

    //line 0
    LSU0.LDXV v0 i0 || IAU.INCS i0 0x10
    LSU1.LDXV v1 i0 || IAU.INCS i0 0X10
    LSU0.LDXV v2 i0 || IAU.INCS i0 0X10
    nop 7
    CMU.VDILV.x16 v21 v20 v1 v2
    CMU.ALIGNVEC v19 v0 v21 14  || VAU.MUL.f16 v18 v22 v21 || LSU0.SWZV8 [22222222]
    VAU.MUL.f16 v17 v22 v20     || LSU0.SWZV8 [11111111]   || CMU.CPVV v0 v2
    VAU.MUL.f16 v16 v22 v19     || LSU0.SWZV8 [00000000]

    //line1
    LSU0.LDXV v3 i1 || IAU.INCS i1 0X10
    LSU1.LDXV v4 i1 || IAU.INCS i1 0X10
    LSU0.LDXV v5 i1 || IAU.INCS i1 0X10
    LSU1.LDXV v1 i0 || IAU.INCS i0 0X10
    LSU0.LDXV v2 i0 || IAU.INCS i0 0X10
    nop 5
    CMU.VDILV.x16 v21 v20 v4 v5
    CMU.ALIGNVEC v19 v3 v21 14  || VAU.MUL.f16 v15 v22 v21 || LSU0.SWZV8 [55555555]
    VAU.MUL.f16 v14 v22 v20     || LSU0.SWZV8 [44444444]   || CMU.CPVV v3 v5
    VAU.MUL.f16 v13 v22 v19     || LSU0.SWZV8 [33333333]

    //line2
    LSU0.LDXV v6 i2 || IAU.INCS i2 0X10
    LSU1.LDXV v7 i2 || IAU.INCS i2 0X10
    LSU0.LDXV v8 i2 || IAU.INCS i2 0X10
    nop 7
    CMU.VDILV.x16 v21 v20 v7 v8
    CMU.ALIGNVEC v19 v6 v21 14  || VAU.MUL.f16 v12 v23 v21 || LSU0.SWZV8 [00000000]
    VAU.MUL.f16 v11 v22 v20     || LSU0.SWZV8 [77777777]   || CMU.CPVV v6 v8
    VAU.MUL.f16 v10 v22 v19     || LSU0.SWZV8 [66666666]
    VAU.ADD.f16 v26 v18 v17
    VAU.ADD.f16 v25 v16 v15

    VAU.ADD.f16 v7 v14 v13      || BRU.RPL i13 i14 ___conv3x3s2hhhhEndLoop
    LSU1.LDXV v4 i1             || IAU.INCS i1 0X10        || VAU.ADD.f16 v24 v12 v11
    LSU0.LDXV v5 i1             || IAU.INCS i1 0X10
    VAU.ADD.f16 v9 v26 v25      || LSU1.LDXV v1 i0         || IAU.INCS i0 0X10
    CMU.VDILV.x16 v21 v20 v1 v2 || VAU.ADD.f16 v7 v7 v24   || LSU0.LDXV v7 i2                    || IAU.INCS i2 0X10
    CMU.ALIGNVEC v19 v0 v21 14  || VAU.MUL.f16 v18 v22 v21 || LSU1.SWZV8 [22222222]
    VAU.MUL.f16 v17 v22 v20     || LSU0.SWZV8 [11111111]   || CMU.CPVV v0 v2  || LSU1.LDXV v8 i2 || IAU.INCS i2 0X10
    VAU.ADD.f16 v9 v9 v7
    VAU.MUL.f16 v16 v22 v19     || LSU0.SWZV8 [00000000]
    VAU.ADD.f16 v26 v18 v17
    LSU1.LDXV v2 i0             || IAU.INCS i0 0X10 || CMU.VDILV.x16 v21 v20 v4 v5 || VAU.ADD.f16 v9 v9 v10

    ___conv3x3s2hhhhLoop:

    CMU.ALIGNVEC v19 v3 v21 14  || VAU.MUL.f16 v15 v22 v21 || LSU0.SWZV8 [55555555]

    VAU.MUL.f16 v14 v22 v20     || LSU0.SWZV8 [44444444]   || CMU.CPVV v3 v5
    VAU.MUL.f16 v13 v22 v19     || LSU0.SWZV8 [33333333]   || LSU1.STXV v9 i17     || IAU.INCS i17 0x10
    VAU.ADD.f16 v25 v16 v15     || CMU.VDILV.x16 v21 v20 v7 v8
    CMU.ALIGNVEC v19 v6 v21 14  || VAU.MUL.f16 v12 v23 v21 || LSU0.SWZV8 [00000000]
    VAU.MUL.f16 v11 v22 v20     || LSU0.SWZV8 [77777777]   || CMU.CPVV v6 v8
    VAU.MUL.f16 v10 v22 v19     || LSU0.SWZV8 [66666666]
    ___conv3x3s2hhhhEndLoop:


    CMU.CMZ.i32 i3
    PEU.PC1C EQ || BRU.BRA ___conv3x3s2hhhhEnd
    //compensate if width is not N*16
    VAU.ADD.f16 v7 v14 v13
    VAU.ADD.f16 v24 v12 v11
    VAU.ADD.f16 v9 v26 v25
    VAU.ADD.f16 v7 v7 v10
    nop
    VAU.ADD.f16 v9 v9 v24
    nop 2
    VAU.ADD.f16 v9 v9 v7
    nop 2
    LSU0.STXV v9 i17     || IAU.INCS i17 0x10


    ___conv3x3s2hhhhEnd:
    LSU1.LDXV v26 i19 || IAU.INCS i19 0x10
    LSU0.LDXV v25 i19 || IAU.INCS i19 0x10
    LSU1.LDXV v24 i19 || IAU.INCS i19 0x10
    BRU.JMP i30
    nop 6
.size mvcvConvolution3x3s2hhhh_asm,.-mvcvConvolution3x3s2hhhh_asm
.nowarnend
.end
