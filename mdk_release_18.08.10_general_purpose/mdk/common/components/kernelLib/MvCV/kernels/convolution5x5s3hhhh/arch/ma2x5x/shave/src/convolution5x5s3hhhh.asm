///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvConvolution5x5s3hhhh_asm,@function
.version 00.50.00

.data .data.convolution5x5s3hhhh
.align 16

.code .text.convolution5x5s3hhhh

//void Convolution5x5s3hhhh_asm(half** in(i18), half** out(i17), half conv[25](i16), u32 inWidth(i15))
mvcvConvolution5x5s3hhhh_asm:

    lsu1.ldil i9 ___conv5x5s3hhhh || lsu0.ldih i9 ___conv5x5s3hhhh
    lsu1.ldi.64 i0 i18            || lsu0.ldil i5 0x10
    lsu1.ldi.64 i2 i18
    lsu1.ld.32  i4 i18            || iau.xor i10 i10 i10
    lsu1.ldo.64.l v20 i16 0x00    || lsu0.ldo.64.h v20 i16 0x08 || iau.add i8 i10 24
    lsu1.ldo.64.l v21 i16 0x10    || lsu0.ldo.64.h v21 i16 0x18 || sau.div.u32 i8 i15 i8
    lsu1.ldo.64.l v22 i16 0x20    || lsu0.ldo.64.h v22 i16 0x28
    lsu1.ldo.64.l v23 i16 0x30    || lsu0.ldo.64.h v23 i16 0x38
    iau.incs i0 -16               || lsu1.ld.32 i17 i17
    lsu1.ldxvi v0 i0 i5 || iau.incs i1 -16
    lsu0.ldxvi v1 i1 i5 || iau.incs i2 -16
    lsu1.ldxvi v2 i2 i5 || iau.incs i3 -16
    lsu0.ldxvi v3 i3 i5 || iau.incs i4 -16
    lsu1.ldxvi v4 i4 i5

    lsu0.ldxvi v5 i0 i5
    lsu1.ldxvi v6 i1 i5
    lsu0.ldxvi v7 i2 i5
    lsu1.ldxvi v8 i3 i5
    lsu0.ldxvi v9 i4 i5

    lsu1.ldxvi v5 i0 i5
    iau.mul i12 i8 24
    nop 2
    cmu.alignvec v15 v0 v5 12 || iau.sub i11 i15 i12

    //L0
    vau.macpz.f16 v20 v15     || lsu0.swzv8 [00000000] || cmu.alignvec v15 v0 v5 14 || lsu1.ldxvi v6 i1 i5 || bru.rpl i9 i8
    vau.macp.f16  v20 v15     || lsu0.swzv8 [11111111] || cmu.cpvv v0 v5
    vau.macp.f16  v20 v0      || lsu0.swzv8 [22222222] || cmu.alignvec v15 v0 v5 2
    vau.macp.f16  v20 v15     || lsu0.swzv8 [33333333] || cmu.alignvec v15 v0 v5 4
    vau.macp.f16  v20 v15     || lsu0.swzv8 [44444444] || cmu.alignvec v15 v1 v6 12
    //L1
    vau.macp.f16  v20 v15     || lsu0.swzv8 [55555555] || cmu.alignvec v15 v1 v6 14 || lsu1.ldxvi v7 i2 i5
    vau.macp.f16  v20 v15     || lsu0.swzv8 [66666666] || cmu.cpvv v1 v6
    vau.macp.f16  v20 v1      || lsu0.swzv8 [77777777] || cmu.alignvec v15 v1 v6 2
    vau.macp.f16  v21 v15     || lsu0.swzv8 [00000000] || cmu.alignvec v15 v1 v6 4
    vau.macp.f16  v21 v15     || lsu0.swzv8 [11111111] || cmu.alignvec v15 v2 v7 12
    //L2
    vau.macp.f16  v21 v15     || lsu0.swzv8 [22222222] || cmu.alignvec v15 v2 v7 14 || lsu1.ldxvi v8 i3 i5
    vau.macp.f16  v21 v15     || lsu0.swzv8 [33333333] || cmu.cpvv v2 v7
    vau.macp.f16  v21 v2      || lsu0.swzv8 [44444444] || cmu.alignvec v15 v2 v7 2
    vau.macp.f16  v21 v15     || lsu0.swzv8 [55555555] || cmu.alignvec v15 v2 v7 4
    vau.macp.f16  v21 v15     || lsu0.swzv8 [66666666] || cmu.alignvec v15 v3 v8 12
    //L3
    vau.macp.f16  v21 v15     || lsu0.swzv8 [77777777] || cmu.alignvec v15 v3 v8 14 || lsu1.ldxvi v9 i4 i5
    vau.macp.f16  v22 v15     || lsu0.swzv8 [00000000] || cmu.cpvv v3 v8
    vau.macp.f16  v22 v3      || lsu0.swzv8 [11111111] || cmu.alignvec v15 v3 v8 2
    vau.macp.f16  v22 v15     || lsu0.swzv8 [22222222] || cmu.alignvec v15 v3 v8 4
    vau.macp.f16  v22 v15     || lsu0.swzv8 [33333333] || cmu.alignvec v15 v4 v9 12
    //L4
    vau.macp.f16  v22 v15     || lsu0.swzv8 [44444444] || cmu.alignvec v15 v4 v9 14
    vau.macp.f16  v22 v15     || lsu0.swzv8 [55555555] || cmu.cpvv v4 v9
    vau.macp.f16  v22 v4      || lsu0.swzv8 [66666666] || cmu.alignvec v15 v4 v9 2
    vau.macp.f16  v22 v15     || lsu0.swzv8 [77777777] || cmu.alignvec v15 v4 v9 4
    vau.macpw.f16 v14 v23 v15 || lsu0.swzv8 [00000000] || cmu.alignvec v15 v0 v5 12
    cmu.cpvi.x16 i6.l v14.2   || iau.incs i15 -8
    cmu.cpvi.x16 i6.h v14.5
    lsu0.ldxvi v5 i0 i5
    iau.add i10 i10 0
    lsu0.sti.32 i6 i17        || peu.pc1i gt
    iau.incs i10 1
    nop

    //L0
    vau.macpz.f16 v20 v15     || lsu0.swzv8 [00000000] || cmu.alignvec v15 v0 v5 14 || lsu1.ldxvi v6 i1 i5
    vau.macp.f16  v20 v15     || lsu0.swzv8 [11111111] || cmu.cpvv v0 v5
    vau.macp.f16  v20 v0      || lsu0.swzv8 [22222222] || cmu.alignvec v15 v0 v5 2
    vau.macp.f16  v20 v15     || lsu0.swzv8 [33333333] || cmu.alignvec v15 v0 v5 4
    vau.macp.f16  v20 v15     || lsu0.swzv8 [44444444] || cmu.alignvec v15 v1 v6 12
    //L1
    vau.macp.f16  v20 v15     || lsu0.swzv8 [55555555] || cmu.alignvec v15 v1 v6 14 || lsu1.ldxvi v7 i2 i5
    vau.macp.f16  v20 v15     || lsu0.swzv8 [66666666] || cmu.cpvv v1 v6
    vau.macp.f16  v20 v1      || lsu0.swzv8 [77777777] || cmu.alignvec v15 v1 v6 2
    vau.macp.f16  v21 v15     || lsu0.swzv8 [00000000] || cmu.alignvec v15 v1 v6 4
    vau.macp.f16  v21 v15     || lsu0.swzv8 [11111111] || cmu.alignvec v15 v2 v7 12
    //L2
    vau.macp.f16  v21 v15     || lsu0.swzv8 [22222222] || cmu.alignvec v15 v2 v7 14 || lsu1.ldxvi v8 i3 i5
    vau.macp.f16  v21 v15     || lsu0.swzv8 [33333333] || cmu.cpvv v2 v7
    vau.macp.f16  v21 v2      || lsu0.swzv8 [44444444] || cmu.alignvec v15 v2 v7 2
    vau.macp.f16  v21 v15     || lsu0.swzv8 [55555555] || cmu.alignvec v15 v2 v7 4
    vau.macp.f16  v21 v15     || lsu0.swzv8 [66666666] || cmu.alignvec v15 v3 v8 12
    //L3
    vau.macp.f16  v21 v15     || lsu0.swzv8 [77777777] || cmu.alignvec v15 v3 v8 14 || lsu1.ldxvi v9 i4 i5
    vau.macp.f16  v22 v15     || lsu0.swzv8 [00000000] || cmu.cpvv v3 v8
    vau.macp.f16  v22 v3      || lsu0.swzv8 [11111111] || cmu.alignvec v15 v3 v8 2
    vau.macp.f16  v22 v15     || lsu0.swzv8 [22222222] || cmu.alignvec v15 v3 v8 4
    vau.macp.f16  v22 v15     || lsu0.swzv8 [33333333] || cmu.alignvec v15 v4 v9 12
    //L4
    vau.macp.f16  v22 v15     || lsu0.swzv8 [44444444] || cmu.alignvec v15 v4 v9 14
    vau.macp.f16  v22 v15     || lsu0.swzv8 [55555555] || cmu.cpvv v4 v9
    vau.macp.f16  v22 v4      || lsu0.swzv8 [66666666] || cmu.alignvec v15 v4 v9 2
    vau.macp.f16  v22 v15     || lsu0.swzv8 [77777777] || cmu.alignvec v15 v4 v9 4
    vau.macpw.f16 v14 v23 v15 || lsu0.swzv8 [00000000] || cmu.alignvec v15 v0 v5 12
    nop
    cmu.cpvi.x16 i6.l v14.0   || iau.incs i15 -8
    cmu.cpvi.x16 i6.h v14.3   || lsu0.ldxvi v5 i0 i5
    cmu.cpvi.x16 i7.l v14.6
    lsu1.sti.32 i6 i17
    lsu1.sti.16 i7 i17
    nop

    //L0
    vau.macpz.f16 v20 v15     || lsu0.swzv8 [00000000] || cmu.alignvec v15 v0 v5 14 || lsu1.ldxvi v6 i1 i5
    vau.macp.f16  v20 v15     || lsu0.swzv8 [11111111] || cmu.cpvv v0 v5
    vau.macp.f16  v20 v0      || lsu0.swzv8 [22222222] || cmu.alignvec v15 v0 v5 2
    vau.macp.f16  v20 v15     || lsu0.swzv8 [33333333] || cmu.alignvec v15 v0 v5 4
    vau.macp.f16  v20 v15     || lsu0.swzv8 [44444444] || cmu.alignvec v15 v1 v6 12
    //L1
    vau.macp.f16  v20 v15     || lsu0.swzv8 [55555555] || cmu.alignvec v15 v1 v6 14 || lsu1.ldxvi v7 i2 i5
    vau.macp.f16  v20 v15     || lsu0.swzv8 [66666666] || cmu.cpvv v1 v6
    vau.macp.f16  v20 v1      || lsu0.swzv8 [77777777] || cmu.alignvec v15 v1 v6 2
    vau.macp.f16  v21 v15     || lsu0.swzv8 [00000000] || cmu.alignvec v15 v1 v6 4
    vau.macp.f16  v21 v15     || lsu0.swzv8 [11111111] || cmu.alignvec v15 v2 v7 12
    //L2
    vau.macp.f16  v21 v15     || lsu0.swzv8 [22222222] || cmu.alignvec v15 v2 v7 14 || lsu1.ldxvi v8 i3 i5
    vau.macp.f16  v21 v15     || lsu0.swzv8 [33333333] || cmu.cpvv v2 v7
    vau.macp.f16  v21 v2      || lsu0.swzv8 [44444444] || cmu.alignvec v15 v2 v7 2
    vau.macp.f16  v21 v15     || lsu0.swzv8 [55555555] || cmu.alignvec v15 v2 v7 4
    vau.macp.f16  v21 v15     || lsu0.swzv8 [66666666] || cmu.alignvec v15 v3 v8 12
    //L3
    vau.macp.f16  v21 v15     || lsu0.swzv8 [77777777] || cmu.alignvec v15 v3 v8 14 || lsu1.ldxvi v9 i4 i5
    vau.macp.f16  v22 v15     || lsu0.swzv8 [00000000] || cmu.cpvv v3 v8
    vau.macp.f16  v22 v3      || lsu0.swzv8 [11111111] || cmu.alignvec v15 v3 v8 2
    vau.macp.f16  v22 v15     || lsu0.swzv8 [22222222] || cmu.alignvec v15 v3 v8 4
    vau.macp.f16  v22 v15     || lsu0.swzv8 [33333333] || cmu.alignvec v15 v4 v9 12
    //L4
    vau.macp.f16  v22 v15     || lsu0.swzv8 [44444444] || cmu.alignvec v15 v4 v9 14
    vau.macp.f16  v22 v15     || lsu0.swzv8 [55555555] || cmu.cpvv v4 v9
    vau.macp.f16  v22 v4      || lsu0.swzv8 [66666666] || cmu.alignvec v15 v4 v9 2
    vau.macp.f16  v22 v15     || lsu0.swzv8 [77777777] || cmu.alignvec v15 v4 v9 4
    vau.macpw.f16 v14 v23 v15 || lsu0.swzv8 [00000000] || cmu.alignvec v15 v0 v5 12
    nop 2
    ___conv5x5s3hhhh:
    nop
    cmu.cpvi.x16 i6.l v14.1   || iau.incs i15 -8
    cmu.cpvi.x16 i6.h v14.4   || lsu0.ldxvi v5 i0 i5
    cmu.cpvi.x16 i7.l v14.7
    lsu0.sti.32 i6 i17
    lsu0.sti.16 i7 i17
    nop



    //Compensate
    vau.macpz.f16 v20 v15     || lsu0.swzv8 [00000000] || cmu.alignvec v15 v0 v5 14 || lsu1.ldxvi v6 i1 i5
    vau.macp.f16  v20 v15     || lsu0.swzv8 [11111111] || cmu.cpvv v0 v5
    vau.macp.f16  v20 v0      || lsu0.swzv8 [22222222] || cmu.alignvec v15 v0 v5 2
    vau.macp.f16  v20 v15     || lsu0.swzv8 [33333333] || cmu.alignvec v15 v0 v5 4
    vau.macp.f16  v20 v15     || lsu0.swzv8 [44444444] || cmu.alignvec v15 v1 v6 12
    //L1
    vau.macp.f16  v20 v15     || lsu0.swzv8 [55555555] || cmu.alignvec v15 v1 v6 14 || lsu1.ldxvi v7 i2 i5
    vau.macp.f16  v20 v15     || lsu0.swzv8 [66666666] || cmu.cpvv v1 v6
    vau.macp.f16  v20 v1      || lsu0.swzv8 [77777777] || cmu.alignvec v15 v1 v6 2
    vau.macp.f16  v21 v15     || lsu0.swzv8 [00000000] || cmu.alignvec v15 v1 v6 4
    vau.macp.f16  v21 v15     || lsu0.swzv8 [11111111] || cmu.alignvec v15 v2 v7 12
    //L2
    vau.macp.f16  v21 v15     || lsu0.swzv8 [22222222] || cmu.alignvec v15 v2 v7 14 || lsu1.ldxvi v8 i3 i5
    vau.macp.f16  v21 v15     || lsu0.swzv8 [33333333] || cmu.cpvv v2 v7
    vau.macp.f16  v21 v2      || lsu0.swzv8 [44444444] || cmu.alignvec v15 v2 v7 2
    vau.macp.f16  v21 v15     || lsu0.swzv8 [55555555] || cmu.alignvec v15 v2 v7 4
    vau.macp.f16  v21 v15     || lsu0.swzv8 [66666666] || cmu.alignvec v15 v3 v8 12
    //L3
    vau.macp.f16  v21 v15     || lsu0.swzv8 [77777777] || cmu.alignvec v15 v3 v8 14 || lsu1.ldxvi v9 i4 i5
    vau.macp.f16  v22 v15     || lsu0.swzv8 [00000000] || cmu.cpvv v3 v8
    vau.macp.f16  v22 v3      || lsu0.swzv8 [11111111] || cmu.alignvec v15 v3 v8 2
    vau.macp.f16  v22 v15     || lsu0.swzv8 [22222222] || cmu.alignvec v15 v3 v8 4
    vau.macp.f16  v22 v15     || lsu0.swzv8 [33333333] || cmu.alignvec v15 v4 v9 12
    //L4
    vau.macp.f16  v22 v15     || lsu0.swzv8 [44444444] || cmu.alignvec v15 v4 v9 14
    vau.macp.f16  v22 v15     || lsu0.swzv8 [55555555] || cmu.cpvv v4 v9
    vau.macp.f16  v22 v4      || lsu0.swzv8 [66666666] || cmu.alignvec v15 v4 v9 2
    vau.macp.f16  v22 v15     || lsu0.swzv8 [77777777] || cmu.alignvec v15 v4 v9 4
    vau.macpw.f16 v14 v23 v15 || lsu0.swzv8 [00000000] || cmu.alignvec v15 v0 v5 12
    cmu.cpvi.x16 i6.l v14.2
    cmu.cpvi.x16 i6.h v14.5
    lsu0.sti.32 i6 i17 || lsu1.ldxv v5 i0
    cmu.cmz.i32 i11
    peu.pc1c eq || bru.jmp i30
    nop 2


    //L0
    vau.macpz.f16 v20 v15     || lsu0.swzv8 [00000000] || cmu.alignvec v15 v0 v5 14 || lsu1.ldxv v6 i1
    vau.macp.f16  v20 v15     || lsu0.swzv8 [11111111] || cmu.cpvv v0 v5
    vau.macp.f16  v20 v0      || lsu0.swzv8 [22222222] || cmu.alignvec v15 v0 v5 2
    vau.macp.f16  v20 v15     || lsu0.swzv8 [33333333] || cmu.alignvec v15 v0 v5 4
    vau.macp.f16  v20 v15     || lsu0.swzv8 [44444444] || cmu.alignvec v15 v1 v6 12
    //L1
    vau.macp.f16  v20 v15     || lsu0.swzv8 [55555555] || cmu.alignvec v15 v1 v6 14 || lsu1.ldxv v7 i2
    vau.macp.f16  v20 v15     || lsu0.swzv8 [66666666] || cmu.cpvv v1 v6
    vau.macp.f16  v20 v1      || lsu0.swzv8 [77777777] || cmu.alignvec v15 v1 v6 2
    vau.macp.f16  v21 v15     || lsu0.swzv8 [00000000] || cmu.alignvec v15 v1 v6 4
    vau.macp.f16  v21 v15     || lsu0.swzv8 [11111111] || cmu.alignvec v15 v2 v7 12
    //L2
    vau.macp.f16  v21 v15     || lsu0.swzv8 [22222222] || cmu.alignvec v15 v2 v7 14 || lsu1.ldxv v8 i3
    vau.macp.f16  v21 v15     || lsu0.swzv8 [33333333] || cmu.cpvv v2 v7
    vau.macp.f16  v21 v2      || lsu0.swzv8 [44444444] || cmu.alignvec v15 v2 v7 2
    vau.macp.f16  v21 v15     || lsu0.swzv8 [55555555] || cmu.alignvec v15 v2 v7 4
    vau.macp.f16  v21 v15     || lsu0.swzv8 [66666666] || cmu.alignvec v15 v3 v8 12
    //L3
    vau.macp.f16  v21 v15     || lsu0.swzv8 [77777777] || cmu.alignvec v15 v3 v8 14 || lsu1.ldxv v9 i4
    vau.macp.f16  v22 v15     || lsu0.swzv8 [00000000] || cmu.cpvv v3 v8
    vau.macp.f16  v22 v3      || lsu0.swzv8 [11111111] || cmu.alignvec v15 v3 v8 2
    vau.macp.f16  v22 v15     || lsu0.swzv8 [22222222] || cmu.alignvec v15 v3 v8 4
    vau.macp.f16  v22 v15     || lsu0.swzv8 [33333333] || cmu.alignvec v15 v4 v9 12
    //L4
    vau.macp.f16  v22 v15     || lsu0.swzv8 [44444444] || cmu.alignvec v15 v4 v9 14
    vau.macp.f16  v22 v15     || lsu0.swzv8 [55555555] || cmu.cpvv v4 v9
    vau.macp.f16  v22 v4      || lsu0.swzv8 [66666666] || cmu.alignvec v15 v4 v9 2
    vau.macp.f16  v22 v15     || lsu0.swzv8 [77777777] || cmu.alignvec v15 v4 v9 4
    vau.macpw.f16 v14 v23 v15 || lsu0.swzv8 [00000000]
    cmu.cpvi.x16 i6.l v14.0
    cmu.cpvi.x16 i7.l v14.3
    cmu.cpvi.x16 i13.l v14.6 || iau.sub i15 i15 3
    cmu.cmz.i32 i15          || iau.sub i15 i15 3
    peu.pc1c gte             || lsu0.sti.16 i6 i17
    cmu.cmz.i32 i15          || iau.sub i15 i15 3
    peu.pc1c gte             || lsu0.sti.16 i7 i17
    cmu.cmz.i32 i15
    peu.pc1c gte             || lsu0.sti.16 i13 i17
    cmu.cpvi.x16 i6.l v14.1
    cmu.cpvi.x16 i7.l v14.4
    cmu.cpvi.x16 i13.l v14.7 || iau.sub i15 i15 3  ||  bru.jmp i30
    cmu.cmz.i32 i15          || iau.sub i15 i15 3
    peu.pc1c gte             || lsu0.sti.16 i6 i17
    cmu.cmz.i32 i15          || iau.sub i15 i15 3
    peu.pc1c gte             || lsu0.sti.16 i7 i17
    cmu.cmz.i32 i15
    peu.pc1c gte             || lsu0.st.16 i13 i17

.size mvcvConvolution5x5s3hhhh_asm,.-mvcvConvolution5x5s3hhhh_asm
.end
