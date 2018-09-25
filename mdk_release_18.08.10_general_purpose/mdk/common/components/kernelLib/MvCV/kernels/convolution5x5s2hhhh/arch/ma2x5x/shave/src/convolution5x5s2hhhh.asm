///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvConvolution5x5s2hhhh_asm,@function
.version 00.50.00

//.data .data.convolution5x5s2hhhh
//.align 4

//.align 16
 //convolution5x5s2hhhh_clampVal:
  //      .float16 255.0


.code .text.convolution5x5s2hhhh

//void Convolution5x5s2hhhh_asm(half** in(i18), half** out(i17), half conv[25](i16), u32 inWidth(i15))
//internal computation are made on fp16, output result is fp16
mvcvConvolution5x5s2hhhh_asm:
.nowarn 37
    //Load the address for all the 5 lines: i0, i1, i2, i3, i4
    //Load the kernel values: v10, v11, v12, v13
    //Line 0 ---> v0 v1 v2 v3
    //Line 1 ---> v4 v5 v6 v7
    //Line 2 ---> v16 v17 v18 v19
    //Line 3 ---> v28 v29 v30 v31
    //Line 4 ---> v24 v25 v26 v27


    LSU1.LDi.64 i0 i18  || LSU0.LDIL i13 0x08
    LSU1.LDi.64 i2 i18
    LSU1.LDi.32 i4 i18

    LSU0.LDIL i11 ___conv5x5s2hhhhLoop        || LSU1.LDIH i11 ___conv5x5s2hhhhLoop

    LSU0.LDO.64.L v10 i16 0x00      || LSU1.LDO.64.H v10 i16 0x08   || IAU.INCS i19 -16
    LSU0.LDO.64.L v11 i16 0x10      || LSU1.LDO.64.H v11 i16 0x18   || IAU.AND i13 i13 i15
    LSU0.LDO.64.L v12 i16 0x20      || LSU1.LDO.64.H v12 i16 0x28   || IAU.SHR.u32 i12 i15 4
    LSU0.LDO.64.L v13 i16 0x30      || LSU1.LD.32 i17 i17           || SAU.SUB.i32 i0 i0 16

    SAU.SUB.i32 i1 i1 16 || LSU1.STXV v24 i19 || IAU.INCS i19 -16
    SAU.SUB.i32 i2 i2 16 || LSU0.STXV v25 i19 || IAU.INCS i19 -16
    SAU.SUB.i32 i3 i3 16 || LSU1.STXV v26 i19 || IAU.INCS i19 -16
    SAU.SUB.i32 i4 i4 16 || LSU0.STXV v27 i19 || IAU.INCS i19 -16
    LSU1.STXV v28 i19 || IAU.INCS i19 -16
    LSU0.STXV v29 i19 || IAU.INCS i19 -16
    LSU1.STXV v30 i19 || IAU.INCS i19 -16
    LSU0.STXV v31 i19

    LSU1.LDXV v0 i0 || IAU.INCS i0 0x10
    LSU0.LDXV v1 i0 || IAU.INCS i0 0x10
    LSU1.LDXV v2 i0 || IAU.INCS i0 0x10
    LSU0.LDXV v3 i0 || IAU.SUB i0 i0 0x10

    LSU1.LDXV v4 i1 || IAU.INCS i1 0x10
    LSU0.LDXV v5 i1 || IAU.INCS i1 0x10
    LSU1.LDXV v6 i1 || IAU.INCS i1 0x10
    LSU0.LDXV v7 i1 || IAU.SUB i1 i1 0x10

    LSU1.LDO.64.h v16 i2 0x8
    LSU1.LDO.64.l v17 i2 0x10
    LSU1.LDO.64.h v17 i2 0x18
    LSU1.LDO.64.l v18 i2 0x20
    LSU1.LDO.64.h v18 i2 0x28
    LSU1.LDO.64.l v19 i2 0x30 || IAU.INCS i2 0x20



    CMU.VDILV.x16 v21 v20 v1 v2   || LSU1.LDO.64.l v30 i3 0x20
    CMU.ALIGNVEC v22 v0 v21 14    || VAU.MACPZ.f16 v10 v20 || LSU0.SWZV8 [22222222]  || LSU1.LDO.64.h v30 i3 0x28
    CMU.ALIGNVEC v23 v20 v3 2     || VAU.MACP.f16 v10  v21 || LSU0.SWZV8 [33333333]  || LSU1.LDO.64.h v25 i4 0x18
    CMU.ALIGNVEC v14 v0 v0 14     || VAU.MACP.f16 v10  v22 || LSU0.SWZV8 [11111111]  || LSU1.LDO.64.l v26 i4 0x20
    CMU.ALIGNVEC v9 v14 v20 14    || VAU.MACP.f16 v10  v23 || LSU0.SWZV8 [44444444]  || LSU1.LDO.64.h v26 i4 0x28

    CMU.VDILV.x16 v21 v20 v5 v6   || VAU.MACP.f16 v10  v9  || LSU0.SWZV8 [00000000]  || LSU1.LDO.64.h v28 i3 0x8
	.nowarn 10,12
    CMU.ALIGNVEC v22 v4 v21 14    || VAU.MACP.f16 v10  v20 || LSU0.SWZV8 [77777777]  || LSU1.LDO.64.l v29 i3 0x10
    CMU.ALIGNVEC v23 v20 v7 2     || VAU.MACP.f16 v11  v21 || LSU0.SWZV8 [00000000]  || LSU1.LDO.64.h v29 i3 0x18
    CMU.ALIGNVEC v14 v4 v4 14     || VAU.MACP.f16 v10  v22 || LSU0.SWZV8 [66666666]  || LSU1.LDO.64.l v25 i4 0x10
    CMU.ALIGNVEC v9 v14 v20 14    || VAU.MACP.f16 v11  v23 || LSU0.SWZV8 [11111111]  || LSU1.LDO.64.h v24 i4 0x8

    CMU.VDILV.x16 v21 v20 v17 v18 || VAU.MACP.f16 v10  v9  || LSU0.SWZV8 [55555555]  || LSU1.LDO.64.l v31 i3 0x30 || IAU.INCS i3 0x20
    CMU.ALIGNVEC v22 v16 v21 14   || VAU.MACP.f16 v11  v20 || LSU0.SWZV8 [44444444]  || LSU1.LDO.64.l v27 i4 0x30 || IAU.INCS i4 0x20
    CMU.ALIGNVEC v23 v20 v19 2    || VAU.MACP.f16 v11  v21 || LSU0.SWZV8 [55555555]  || LSU1.LDO.64.h v0 i0 0x8
    CMU.ALIGNVEC v14 v16 v16 14   || VAU.MACP.f16 v11  v22 || LSU0.SWZV8 [33333333]  || LSU1.LDO.64.l v1 i0 0x10
    CMU.ALIGNVEC v9 v14 v20 14    || VAU.MACP.f16 v11  v23 || LSU0.SWZV8 [66666666]  || LSU1.LDO.64.h v1 i0 0x18

    CMU.VDILV.x16 v21 v20 v29 v30 || VAU.MACP.f16 v11  v9   || LSU0.SWZV8 [22222222] || LSU1.LDO.64.l v2 i0 0x20
    CMU.ALIGNVEC v22 v28 v21 14   || VAU.MACP.f16 v12  v20  || LSU0.SWZV8 [11111111] || LSU1.LDO.64.h v2 i0 0x28
    CMU.ALIGNVEC v23 v20 v31 2    || VAU.MACP.f16 v12  v21  || LSU0.SWZV8 [22222222] || LSU1.LDO.64.l v3 i0 0x30  || IAU.INCS i0 0x20
    CMU.ALIGNVEC v14 v28 v28 14   || VAU.MACP.f16 v12  v22  || LSU0.SWZV8 [00000000] || LSU1.LDO.64.h v4 i1 0x8
    CMU.ALIGNVEC v9 v14 v20 14    || VAU.MACP.f16 v12  v23  || LSU0.SWZV8 [33333333] || LSU1.LDO.64.l v5 i1 0x10

    CMU.VDILV.x16 v21 v20 v25 v26 || VAU.MACP.f16 v11  v9   || LSU0.SWZV8 [77777777] || LSU1.LDO.64.h v5 i1 0x18
    CMU.ALIGNVEC v22 v24 v21 14   || VAU.MACP.f16 v12  v20  || LSU0.SWZV8 [66666666] || LSU1.LDO.64.l v6 i1 0x20
    CMU.ALIGNVEC v23 v20 v27 2    || VAU.MACP.f16 v12  v21  || LSU0.SWZV8 [77777777] || LSU1.LDO.64.h v6 i1 0x28
    CMU.ALIGNVEC v14 v24 v24 14   || VAU.MACP.f16 v12  v22  || LSU0.SWZV8 [55555555] || LSU1.LDO.64.l v7 i1 0x30  || IAU.INCS i1 0x20
    CMU.ALIGNVEC v9 v14 v20 14    || VAU.MACP.f16 v13  v23  || LSU0.SWZV8 [00000000]
                                     VAU.MACPW.f16 v15 v12  v9  || LSU0.SWZV8 [44444444]

    LSU1.LDO.64.h v16 i2 0x8
    LSU1.LDO.64.l v17 i2 0x10
    LSU1.LDO.64.h v17 i2 0x18
    LSU1.LDO.64.l v18 i2 0x20
    LSU1.LDO.64.h v18 i2 0x28
    LSU1.LDO.64.l v19 i2 0x30 || IAU.INCS i2 0x20


    CMU.VDILV.x16 v21 v20 v1 v2   || LSU1.LDO.64.l v30 i3 0x20 || bru.rpl i11 i12
    CMU.ALIGNVEC v22 v0 v21 14    || VAU.MACPZ.f16 v10 v20 || LSU0.SWZV8 [22222222] || LSU1.LDO.64.h v30 i3 0x28

    CMU.ALIGNVEC v23 v20 v3 2     || VAU.MACP.f16 v10  v21 || LSU0.SWZV8 [33333333] || LSU1.LDO.64.h v25 i4 0x18
    CMU.ALIGNVEC v14 v0 v0 14     || VAU.MACP.f16 v10  v22 || LSU0.SWZV8 [11111111] || LSU1.LDO.64.l v26 i4 0x20
    CMU.ALIGNVEC v9 v14 v20 14    || VAU.MACP.f16 v10  v23 || LSU0.SWZV8 [44444444] || LSU1.LDO.64.h v26 i4 0x28

    CMU.VDILV.x16 v21 v20 v5 v6   || VAU.MACP.f16 v10  v9  || LSU0.SWZV8 [00000000] || LSU1.LDO.64.h v28 i3 0x8
    CMU.ALIGNVEC v22 v4 v21 14    || VAU.MACP.f16 v10  v20 || LSU0.SWZV8 [77777777] || LSU1.LDO.64.l v29 i3 0x10
    CMU.ALIGNVEC v23 v20 v7 2     || VAU.MACP.f16 v11  v21 || LSU0.SWZV8 [00000000] || LSU1.LDO.64.h v29 i3 0x18
    CMU.ALIGNVEC v14 v4 v4 14     || VAU.MACP.f16 v10  v22 || LSU0.SWZV8 [66666666] || LSU1.LDO.64.l v25 i4 0x10

    CMU.ALIGNVEC v9 v14 v20 14    || VAU.MACP.f16 v11  v23 || LSU0.SWZV8 [11111111] || LSU1.LDO.64.h v24 i4 0x8
    CMU.VDILV.x16 v21 v20 v17 v18 || VAU.MACP.f16 v10  v9  || LSU0.SWZV8 [55555555] || LSU1.LDO.64.l v31 i3 0x30 || IAU.INCS i3 0x20
    CMU.ALIGNVEC v22 v16 v21 14   || VAU.MACP.f16 v11  v20 || LSU0.SWZV8 [44444444] || LSU1.LDO.64.l v27 i4 0x30 || IAU.INCS i4 0x20
    CMU.ALIGNVEC v23 v20 v19 2    || VAU.MACP.f16 v11  v21 || LSU0.SWZV8 [55555555] || LSU1.LDO.64.h v0 i0 0x8
    CMU.ALIGNVEC v14 v16 v16 14   || VAU.MACP.f16 v11  v22 || LSU0.SWZV8 [33333333] || LSU1.LDO.64.l v1 i0 0x10


    CMU.ALIGNVEC v9 v14 v20 14    || VAU.MACP.f16 v11  v23 || LSU0.SWZV8 [66666666] || LSU1.LDO.64.h v1 i0 0x18
    CMU.VDILV.x16 v21 v20 v29 v30 || VAU.MACP.f16 v11  v9  || LSU0.SWZV8 [22222222] || LSU1.LDO.64.l v2 i0 0x20
    CMU.ALIGNVEC v22 v28 v21 14   || VAU.MACP.f16 v12  v20 || LSU0.SWZV8 [11111111] || LSU1.LDO.64.h v2 i0 0x28
    CMU.ALIGNVEC v23 v20 v31 2    || VAU.MACP.f16 v12  v21 || LSU0.SWZV8 [22222222] || LSU1.LDO.64.l v3 i0 0x30 || IAU.INCS i0 0x20
    CMU.ALIGNVEC v14 v28 v28 14   || VAU.MACP.f16 v12  v22 || LSU0.SWZV8 [00000000] || LSU1.LDO.64.h v4 i1 0x8
    CMU.ALIGNVEC v9 v14 v20 14    || VAU.MACP.f16 v12  v23 || LSU0.SWZV8 [33333333] || LSU1.LDO.64.l v5 i1 0x10

    CMU.VDILV.x16 v21 v20 v25 v26 || VAU.MACP.f16 v11  v9  || LSU0.SWZV8 [77777777] || LSU1.LDO.64.h v5 i1 0x18
    CMU.ALIGNVEC v22 v24 v21 14   || VAU.MACP.f16 v12  v20 || LSU0.SWZV8 [66666666] || LSU1.LDO.64.l v6 i1 0x20
    CMU.ALIGNVEC v23 v20 v27 2    || VAU.MACP.f16 v12  v21 || LSU0.SWZV8 [77777777] || LSU1.LDO.64.h v6 i1 0x28
    CMU.ALIGNVEC v14 v24 v24 14   || VAU.MACP.f16 v12  v22 || LSU0.SWZV8 [55555555] || LSU1.LDO.64.l v7 i1 0x30 || IAU.INCS i1 0x20
    CMU.ALIGNVEC v9 v14 v20 14    || VAU.MACP.f16 v13  v23 || LSU0.SWZV8 [00000000]
                                     VAU.MACPW.f16 v15 v12  v9  || LSU0.SWZV8 [44444444]
    LSU1.ST.64.l v15 i17
    LSU1.STO.64.h v15 i17 0x8
    LSU1.LDO.64.h v16 i2 0x8 || IAU.INCS i17 0x10

    ___conv5x5s2hhhhLoop:
    LSU1.LDO.64.l v17 i2 0x10
    LSU1.LDO.64.h v17 i2 0x18
    LSU1.LDO.64.l v18 i2 0x20
    LSU1.LDO.64.h v18 i2 0x28
    LSU1.LDO.64.l v19 i2 0x30 || IAU.INCS i2 0x20
    nop


    CMU.CMZ.i32 i13
    PEU.PC1C EQ || BRU.BRA ___conv5x5s2hhhhEnd

    CMU.VDILV.x16 v21 v20 v1 v2
    CMU.ALIGNVEC v22 v0 v21 14    || VAU.MACPZ.f16 v10 v20 || LSU0.SWZV8 [22222222]

    CMU.ALIGNVEC v23 v20 v3 2     || VAU.MACP.f16 v10  v21 || LSU0.SWZV8 [33333333]
    CMU.ALIGNVEC v14 v0 v0 14     || VAU.MACP.f16 v10  v22 || LSU0.SWZV8 [11111111]
    CMU.ALIGNVEC v9 v14 v20 14    || VAU.MACP.f16 v10  v23 || LSU0.SWZV8 [44444444]

    CMU.VDILV.x16 v21 v20 v5 v6   || VAU.MACP.f16 v10  v9  || LSU0.SWZV8 [00000000]
    CMU.ALIGNVEC v22 v4 v21 14    || VAU.MACP.f16 v10  v20 || LSU0.SWZV8 [77777777]
    CMU.ALIGNVEC v23 v20 v7 2     || VAU.MACP.f16 v11  v21 || LSU0.SWZV8 [00000000]
    CMU.ALIGNVEC v14 v4 v4 14     || VAU.MACP.f16 v10  v22 || LSU0.SWZV8 [66666666]

    CMU.ALIGNVEC v9 v14 v20 14    || VAU.MACP.f16 v11  v23 || LSU0.SWZV8 [11111111]
    CMU.VDILV.x16 v21 v20 v17 v18 || VAU.MACP.f16 v10  v9  || LSU0.SWZV8 [55555555]
    CMU.ALIGNVEC v22 v16 v21 14   || VAU.MACP.f16 v11  v20 || LSU0.SWZV8 [44444444]
    CMU.ALIGNVEC v23 v20 v19 2    || VAU.MACP.f16 v11  v21 || LSU0.SWZV8 [55555555]
    CMU.ALIGNVEC v14 v16 v16 14   || VAU.MACP.f16 v11  v22 || LSU0.SWZV8 [33333333]


    CMU.ALIGNVEC v9 v14 v20 14    || VAU.MACP.f16 v11  v23 || LSU0.SWZV8 [66666666]
    CMU.VDILV.x16 v21 v20 v29 v30 || VAU.MACP.f16 v11  v9  || LSU0.SWZV8 [22222222]
    CMU.ALIGNVEC v22 v28 v21 14   || VAU.MACP.f16 v12  v20 || LSU0.SWZV8 [11111111]
    CMU.ALIGNVEC v23 v20 v31 2    || VAU.MACP.f16 v12  v21 || LSU0.SWZV8 [22222222]
    CMU.ALIGNVEC v14 v28 v28 14   || VAU.MACP.f16 v12  v22 || LSU0.SWZV8 [00000000]
    CMU.ALIGNVEC v9 v14 v20 14    || VAU.MACP.f16 v12  v23 || LSU0.SWZV8 [33333333]

    CMU.VDILV.x16 v21 v20 v25 v26 || VAU.MACP.f16 v11  v9  || LSU0.SWZV8 [77777777]
    CMU.ALIGNVEC v22 v24 v21 14   || VAU.MACP.f16 v12  v20 || LSU0.SWZV8 [66666666]
    CMU.ALIGNVEC v23 v20 v27 2    || VAU.MACP.f16 v12  v21 || LSU0.SWZV8 [77777777]
    CMU.ALIGNVEC v14 v24 v24 14   || VAU.MACP.f16 v12  v22 || LSU0.SWZV8 [55555555]
    CMU.ALIGNVEC v9 v14 v20 14    || VAU.MACP.f16 v13  v23 || LSU0.SWZV8 [00000000]
                                     VAU.MACPW.f16 v15 v12  v9  || LSU0.SWZV8 [44444444]
    LSU1.ST.64.l v15 i17
 // LSU1.STO.64.h v15 i17 0x8
    LSU1.LDO.64.h v16 i2 0x8 || iau.incs i17 0x10
   
   
   ___conv5x5s2hhhhEnd:
    LSU1.LDXV v31 i19 || IAU.INCS i19 0x10
	.nowarnend
    LSU0.LDXV v30 i19 || IAU.INCS i19 0x10
    LSU1.LDXV v29 i19 || IAU.INCS i19 0x10
    LSU0.LDXV v28 i19 || IAU.INCS i19 0x10
    LSU1.LDXV v27 i19 || IAU.INCS i19 0x10
    LSU0.LDXV v26 i19 || IAU.INCS i19 0x10
    LSU1.LDXV v25 i19 || IAU.INCS i19 0x10
    LSU0.LDXV v24 i19 || IAU.INCS i19 0x10

    
    BRU.jmp i30
    nop 6
.nowarnend
.size mvcvConvolution5x5s2hhhh_asm,.-mvcvConvolution5x5s2hhhh_asm
.end
