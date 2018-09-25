///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvMaxPool3x3s2hh_asm,@function
.version 00.50.00

.data .data.maxPool3x3s2hh
.align 4
.code .text.maxPool3x3s2hh

//void maxPool3x3s2hh(half** src, half** dst, u32 outputWidth)
//                          i18         i17        i16
mvcvMaxPool3x3s2hh_asm:


    lsu1.ldi.64 i0 i18   || lsu0.ldil i3 0x10
    lsu1.ld.32 i2 i18    || vau.xor v9 v9 v9  || lsu0.ldil i5 ___maxPool3x3s2hhLoop
    iau.shr.u32 i4 i16 3 || lsu0.ldih i5 ___maxPool3x3s2hhLoop
    lsu0.ld.32 i17 i17
    iau.xor i6 i6 i6
    iau.incs i6 7
    iau.and i7 i6 i16
    lsu1.ldxvi v0 i0 i3
    lsu0.ldxvi v1 i1 i3
    lsu1.ldxvi v2 i2 i3
    lsu0.ldxvi v10 i0 i3
    lsu1.ldxvi v11 i1 i3
    lsu0.ldxvi v12 i2 i3
    lsu1.ldxvi v20 i0 i3
    lsu0.ldxvi v21 i1 i3
    lsu1.ldxvi v22 i2 i3
    nop 2


    cmu.vdilv.x16 v3 v4 v0 v10  || lsu0.ldxvi v10 i0 i3
    cmu.vdilv.x16 v5 v6 v1 v11  || lsu1.ldxvi v11 i1 i3
    cmu.vdilv.x16 v7 v8 v2 v12  || lsu0.ldxvi v12 i2 i3
    cmu.alignvec v14 v4 v20 2   || lsu1.ldxvi v20 i0 i3
    cmu.alignvec v16 v6 v21 2   || lsu0.ldxvi v21 i1 i3
    cmu.alignvec v18 v8 v22 2   || lsu1.ldxvi v22 i2 i3

    cmu.max.f16 v13 v3 v4       || vau.or v0 v9 v20
    cmu.max.f16 v15 v5 v6       || vau.or v1 v9 v21
    cmu.max.f16 v17 v7 v8       || vau.or v2 v9 v22
    cmu.max.f16 v13 v13 v15
    cmu.max.f16 v15 v14 v16
    cmu.max.f16 v13 v13 v17
    cmu.max.f16 v15 v15 v18
    nop
    cmu.max.f16 v19 v13 v15
    iau.sub i4 i4 1


    cmu.vdilv.x16 v3 v4 v0 v10  || lsu0.ldxvi v10 i0 i3 || bru.rpl i5 i4
    cmu.vdilv.x16 v5 v6 v1 v11  || lsu1.ldxvi v11 i1 i3
    cmu.vdilv.x16 v7 v8 v2 v12  || lsu0.ldxvi v12 i2 i3
    cmu.alignvec v14 v4 v20 2   || lsu1.ldxvi v20 i0 i3
    cmu.alignvec v16 v6 v21 2   || lsu0.ldxvi v21 i1 i3
    cmu.alignvec v18 v8 v22 2   || lsu1.ldxvi v22 i2 i3

    cmu.max.f16 v13 v3 v4       || lsu0.sti.64.l v19 i17
    cmu.max.f16 v15 v5 v6       || lsu0.sti.64.h v19 i17
    cmu.max.f16 v17 v7 v8       || vau.or v0 v9 v20
    ___maxPool3x3s2hhLoop:
    cmu.max.f16 v13 v13 v15     || vau.or v1 v9 v21
    cmu.max.f16 v15 v14 v16     || vau.or v2 v9 v22
    cmu.max.f16 v13 v13 v17
    cmu.max.f16 v15 v15 v18
    nop
    cmu.max.f16 v19 v13 v15
    nop

    //Compensate
    cmu.cmz.i16 i7
    peu.pc1c eq || bru.jmp i30
    cmu.vdilv.x16 v3 v4 v0 v10 || lsu0.sti.64.l v19 i17
    cmu.vdilv.x16 v5 v6 v1 v11 || lsu0.sti.64.h v19 i17
    cmu.vdilv.x16 v7 v8 v2 v12
    cmu.alignvec v14 v4 v20 2
    cmu.alignvec v16 v6 v21 2
    cmu.alignvec v18 v8 v22 2
    cmu.max.f16 v13 v3 v4
    cmu.max.f16 v15 v5 v6
    cmu.max.f16 v17 v7 v8
    cmu.max.f16 v13 v13 v15
    cmu.max.f16 v15 v14 v16 || bru.jmp i30
    cmu.max.f16 v13 v13 v17
    cmu.max.f16 v15 v15 v18
    nop
    cmu.max.f16 v19 v13 v15
    nop
    lsu0.sti.64.l v19 i17

.size mvcvMaxPool3x3s2hh_asm,.-mvcvMaxPool3x3s2hh_asm
.end
