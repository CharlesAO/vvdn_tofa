///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvMaxPool2x2s2hh_asm,@function
.version 00.50.01

.data .data.maxPool2x2s2hh
.align 4
.code .text.maxPool2x2s2hh

//void maxPool2x2s2hh(half** src, half** dst, u32 outputWidth)
//                          i18         i17        i16
mvcvMaxPool2x2s2hh_asm:


    lsu1.ld.64 i0 i18  || lsu0.ldil i3 ___maxPool2x2s2hhLoop
    lsu0.ldil i2 0x10  || lsu1.ldih i3 ___maxPool2x2s2hhLoop
    lsu1.ld.32 i17 i17 || iau.sub i5 i2 9
    iau.shr.u32 i4 i16 3
    iau.and i6 i5 i16
    nop 2
    lsu0.ldxvi v0 i0 i2
    lsu1.ldxvi v1 i1 i2
    lsu0.ldxvi v10 i0 i2
    lsu1.ldxvi v11 i1 i2
    nop 4
    cmu.alignvec v2 v0 v0 2
    cmu.alignvec v3 v1 v1 2


    cmu.alignvec v12 v10 v10 2 || bru.rpl i3 i4
    cmu.alignvec v13 v11 v11 2
    cmu.max.f16 v4 v0 v1       || lsu0.ldxvi v0 i0 i2
    cmu.max.f16 v14 v10 v11    || lsu1.ldxvi v1 i1 i2
    cmu.max.f16 v5 v4 v2       || lsu0.ldxvi v10 i0 i2
    cmu.max.f16 v15 v14 v12    || lsu1.ldxvi v11 i1 i2

    ___maxPool2x2s2hhLoop:
    cmu.max.f16 v6 v5 v3
    cmu.max.f16 v16 v15 v13
    nop
    cmu.vdilv.x16 v7 v8 v6 v6
    cmu.vdilv.x16 v17 v18 v16 v16
    lsu0.sti.64.l v8 i17      || cmu.alignvec v2 v0 v0 2
    lsu0.sti.64.l v18 i17     || cmu.alignvec v3 v1 v1 2

    //Compensate
    cmu.cmz.i32 i6
    peu.pc1c eq || bru.jmp i30
    cmu.alignvec v2 v0 v0 2
    cmu.alignvec v3 v1 v1 2
    nop
    cmu.max.f16 v4 v0 v1
    nop
    cmu.max.f16 v5 v4 v2 ||  bru.jmp i30
    nop
    cmu.max.f16 v6 v5 v3
    iau.sub i6 i6 4
    cmu.vdilv.x16 v7 v8 v6 v6
    cmu.cmz.i32 i6
    peu.pc1c gte || lsu0.sti.64.l v8 i17

.size mvcvMaxPool2x2s2hh_asm,.-mvcvMaxPool2x2s2hh_asm
.end
