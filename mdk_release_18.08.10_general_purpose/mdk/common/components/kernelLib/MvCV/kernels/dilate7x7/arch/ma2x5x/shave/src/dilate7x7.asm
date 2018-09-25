///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvDilate7x7_asm,@function
.version 00.51.04




.data .data.dilate
.align 4
.code .text.dilate


//  void Dilate7x7(u8** src, u8** dst, u8** kernel, u32 width)
//                   i18       i17        i16        i15

mvcvDilate7x7_asm:


IAU.SUB i19 i19 8
LSU0.ST.64.L v24  i19 	|| IAU.SUB i19 i19 8
LSU0.ST.64.H v24  i19 	|| IAU.SUB i19 i19 8
LSU0.ST.64.L v25  i19 	|| IAU.SUB i19 i19 8
LSU0.ST.64.H v25  i19 	|| IAU.SUB i19 i19 8
LSU0.ST.64.L v26  i19 	|| IAU.SUB i19 i19 8
LSU0.ST.64.H v26  i19 	|| IAU.SUB i19 i19 8
LSU0.ST.64.L v27  i19 	|| IAU.SUB i19 i19 8
LSU0.ST.64.H v27  i19 	|| IAU.SUB i19 i19 8
LSU0.ST.64.L v28  i19 	|| IAU.SUB i19 i19 8
LSU0.ST.64.H v28  i19 	|| IAU.SUB i19 i19 8
LSU0.ST.64.L v29  i19 	|| IAU.SUB i19 i19 8
LSU0.ST.64.H v29  i19 	|| IAU.SUB i19 i19 8
LSU0.ST.64.L v30  i19 	|| IAU.SUB i19 i19 8
LSU0.ST.64.H v30  i19 	|| IAU.SUB i19 i19 8
LSU0.ST.64.L v31  i19 	|| IAU.SUB i19 i19 8
LSU0.ST.64.H v31  i19
lsu0.ldo.32 i4 i16 0   ||lsu1.ld.32 i17 i17 || iau.xor i12 i12 i12
lsu0.ldo.32 i5 i16 4   ||lsu1.ldo.32 i6 i16 8
lsu0.ldo.32 i7 i16 12  ||lsu1.ldo.32 i8 i16 16
lsu0.ldo.32 i9 i16 20  ||lsu1.ldo.32 i10 i16 24
cmu.cpzv v0 0x0
cmu.cpzv v1 0x3
cmu.cpzv v4 0x3

//kernel data
                            lsu1.ld.64.l v0 i4
lsu0.ldo.32 i4  i18 0    || lsu1.ld.64.l v1 i5
lsu0.ldo.32 i5  i18 4    || lsu1.ld.64.l v2 i6
lsu0.ldo.32 i6  i18 8    || lsu1.ld.64.l v3 i7 || iau.add i13 i17 i15
lsu0.ldo.32 i7  i18 12   || lsu1.ld.64.l v4 i8
lsu0.ldo.32 i8  i18 16   || lsu1.ld.64.l v5 i9
lsu0.ldo.32 i9  i18 20   || lsu1.ld.64.l v6 i10
lsu0.ldo.32 i10 i18 24
lsu0.ld.64.l v31 i13
nop 5
lsu0.ldil i0 0xffff
lsu0.ldil i11 0xffff || lsu1.ldih i11 0x00ff
nop 6
cmu.cpivr.x16 v30 i0
cmu.cpiv.x32 v30.1 i11
vau.and v0 v0 v30 || iau.sub i4 i4   16
vau.and v1 v1 v30 || iau.sub i5 i5   16
vau.and v2 v2 v30 || iau.sub i6 i6   16
vau.and v3 v3 v30 || iau.sub i7 i7   16
vau.and v4 v4 v30 || iau.sub i8 i8   16
vau.and v5 v5 v30 || iau.sub i9 i9   16
vau.and v6 v6 v30 || iau.sub i10 i10 16
cmu.vilv.x8 v0 v24 v0 v0
cmu.vilv.x8 v1 v25 v1 v1
cmu.vilv.x8 v2 v26 v2 v2
cmu.vilv.x8 v3 v27 v3 v3
cmu.vilv.x8 v4 v28 v4 v4
cmu.vilv.x8 v5 v29 v5 v5
cmu.vilv.x8 v6 v30 v6 v6
lsu1.ldo.64.l  v8 i4   0 || lsu0.ldo.64.h v8 i4   8 ||iau.add i4 i4 16
lsu1.ldo.64.l  v15 i4   0 || lsu0.ldo.64.h v15 i4 8  ||iau.add i4 i4 16
nop 6
dilate7x7_loop:
iau.incs i12 16
//line 0
cmu.alignvec v23 v8 v15  13
vau.mul.i8 v16 v0 v23 ||lsu1.swzv8 [00000000] ||cmu.alignvec v23 v8 v15  14
vau.mul.i8 v17 v0 v23 ||lsu1.swzv8 [11111111] ||cmu.alignvec v23 v8 v15  15
vau.mul.i8 v18 v0 v23 ||lsu1.swzv8 [22222222] ||cmu.alignvec v23 v15 v8  0
lsu1.ldo.64.l  v8 i4   0 || lsu0.ldo.64.h v8 i4   8 ||iau.add i4 i4 16
nop 6
vau.mul.i8 v19 v0 v23 ||lsu1.swzv8 [33333333] ||cmu.alignvec v23 v15 v8  1
vau.mul.i8 v20 v0 v23 ||lsu1.swzv8 [44444444] ||cmu.alignvec v23 v15 v8  2
vau.mul.i8 v21 v0 v23 ||lsu1.swzv8 [55555555] ||cmu.alignvec v23 v15 v8  3
vau.mul.i8 v22 v0 v23 ||lsu1.swzv8 [66666666]
cmu.max.u8 v16 v16 v17 ||lsu1.ldo.64.l  v8 i5   0 || lsu0.ldo.64.h v8 i5   8  ||iau.add i5 i5 16
cmu.max.u8 v18 v18 v19 ||lsu1.ldo.64.l  v15 i5   0 || lsu0.ldo.64.h v15 i5 8  ||iau.add i5 i5 16
cmu.max.u8 v20 v20 v21
cmu.max.u8 v16 v16 v18
cmu.max.u8 v20 v20 v22
nop
cmu.max.u8 v9 v16 v20
//line 1
nop
cmu.alignvec v23 v8 v15  13
vau.mul.i8 v16 v1 v23 ||lsu0.swzv8 [00000000] ||cmu.alignvec v23 v8 v15  14
vau.mul.i8 v17 v1 v23 ||lsu0.swzv8 [11111111] ||cmu.alignvec v23 v8 v15  15
vau.mul.i8 v18 v1 v23 ||lsu0.swzv8 [22222222] ||cmu.alignvec v23 v15 v8  0
lsu1.ldo.64.l  v8 i5   0 || lsu0.ldo.64.h v8 i5   8 ||iau.add i5 i5 16
nop 6
vau.mul.i8 v19 v1 v23 ||lsu0.swzv8 [33333333] ||cmu.alignvec v23 v15 v8  1
vau.mul.i8 v20 v1 v23 ||lsu0.swzv8 [44444444] ||cmu.alignvec v23 v15 v8  2
vau.mul.i8 v21 v1 v23 ||lsu0.swzv8 [55555555] ||cmu.alignvec v23 v15 v8  3
vau.mul.i8 v22 v1 v23 ||lsu0.swzv8 [66666666]
cmu.max.u8 v16 v16 v17 || lsu1.ldo.64.l  v8 i6   0 || lsu0.ldo.64.h v8 i6   8 ||iau.add i6 i6 16
cmu.max.u8 v18 v18 v19 ||lsu1.ldo.64.l  v15 i6   0 || lsu0.ldo.64.h v15 i6 8  ||iau.add i6 i6 16
cmu.max.u8 v20 v20 v21
cmu.max.u8 v16 v16 v18
cmu.max.u8 v20 v20 v22
nop
cmu.max.u8 v10 v16 v20
//line 2
nop
cmu.alignvec v23 v8 v15  13
vau.mul.i8 v16 v2 v23 ||lsu1.swzv8 [00000000] ||cmu.alignvec v23 v8 v15  14
vau.mul.i8 v17 v2 v23 ||lsu1.swzv8 [11111111] ||cmu.alignvec v23 v8 v15  15
vau.mul.i8 v18 v2 v23 ||lsu1.swzv8 [22222222] ||cmu.alignvec v23 v15 v8  0
lsu1.ldo.64.l  v8 i6   0 || lsu0.ldo.64.h v8 i6   8 ||iau.add i6 i6 16
nop 6
vau.mul.i8 v19 v2 v23 ||lsu1.swzv8 [33333333] ||cmu.alignvec v23 v15 v8  1
vau.mul.i8 v20 v2 v23 ||lsu1.swzv8 [44444444] ||cmu.alignvec v23 v15 v8  2
vau.mul.i8 v21 v2 v23 ||lsu1.swzv8 [55555555] ||cmu.alignvec v23 v15 v8  3
vau.mul.i8 v22 v2 v23 ||lsu1.swzv8 [66666666]
cmu.max.u8 v16 v16 v17 || lsu1.ldo.64.l  v8 i7   0 || lsu0.ldo.64.h v8 i7   8 ||iau.add i7 i7 16
cmu.max.u8 v18 v18 v19 || lsu1.ldo.64.l  v15 i7   0 || lsu0.ldo.64.h v15 i7 8  ||iau.add i7 i7 16
cmu.max.u8 v20 v20 v21
cmu.max.u8 v16 v16 v18
cmu.max.u8 v20 v20 v22
nop
cmu.max.u8 v11 v16 v20

//line 3
nop 6
cmu.alignvec v23 v8 v15  13
vau.mul.i8 v16 v3 v23 ||lsu1.swzv8 [00000000] ||cmu.alignvec v23 v8 v15  14
vau.mul.i8 v17 v3 v23 ||lsu1.swzv8 [11111111] ||cmu.alignvec v23 v8 v15  15
vau.mul.i8 v18 v3 v23 ||lsu1.swzv8 [22222222] ||cmu.alignvec v23 v15 v8  0
lsu1.ldo.64.l  v8 i7   0 || lsu0.ldo.64.h v8 i7   8 ||iau.add i7 i7 16
nop 6
vau.mul.i8 v19 v3 v23 ||lsu1.swzv8 [33333333] ||cmu.alignvec v23 v15 v8  1
vau.mul.i8 v20 v3 v23 ||lsu1.swzv8 [44444444] ||cmu.alignvec v23 v15 v8  2
vau.mul.i8 v21 v3 v23 ||lsu1.swzv8 [55555555] ||cmu.alignvec v23 v15 v8  3
vau.mul.i8 v22 v3 v23 ||lsu1.swzv8 [66666666]
cmu.max.u8 v16 v16 v17 || lsu1.ldo.64.l  v8 i8   0 || lsu0.ldo.64.h v8 i8   8  ||iau.add i8 i8 16
cmu.max.u8 v18 v18 v19 || lsu1.ldo.64.l  v15 i8   0 || lsu0.ldo.64.h v15 i8 8  ||iau.add i8 i8 16
cmu.max.u8 v20 v20 v21
cmu.max.u8 v16 v16 v18
cmu.max.u8 v20 v20 v22
nop
cmu.max.u8 v12 v16 v20
nop
//line 4
cmu.alignvec v23 v8 v15  13
vau.mul.i8 v16 v4 v23 ||lsu1.swzv8 [00000000] ||cmu.alignvec v23 v8 v15  14
vau.mul.i8 v17 v4 v23 ||lsu1.swzv8 [11111111] ||cmu.alignvec v23 v8 v15  15
vau.mul.i8 v18 v4 v23 ||lsu1.swzv8 [22222222] ||cmu.alignvec v23 v15 v8  0
lsu1.ldo.64.l  v8 i8   0 || lsu0.ldo.64.h v8 i8   8 ||iau.add i8 i8 16
nop 6
vau.mul.i8 v19 v4 v23 ||lsu1.swzv8 [33333333] ||cmu.alignvec v23 v15 v8  1
vau.mul.i8 v20 v4 v23 ||lsu1.swzv8 [44444444] ||cmu.alignvec v23 v15 v8  2
vau.mul.i8 v21 v4 v23 ||lsu1.swzv8 [55555555] ||cmu.alignvec v23 v15 v8  3
vau.mul.i8 v22 v4 v23 ||lsu1.swzv8 [66666666]
cmu.max.u8 v16 v16 v17 || lsu1.ldo.64.l  v8 i9   0 || lsu0.ldo.64.h v8 i9   8 ||iau.add i9 i9 16
cmu.max.u8 v18 v18 v19 || lsu1.ldo.64.l  v15 i9   0 || lsu0.ldo.64.h v15 i9 8  ||iau.add i9 i9 16
cmu.max.u8 v20 v20 v21
cmu.max.u8 v16 v16 v18
cmu.max.u8 v20 v20 v22
nop
cmu.max.u8 v13 v16 v20

//line 5
nop
cmu.alignvec v23 v8 v15  13
vau.mul.i8 v16 v5 v23 ||lsu1.swzv8 [00000000] ||cmu.alignvec v23 v8 v15  14
vau.mul.i8 v17 v5 v23 ||lsu1.swzv8 [11111111] ||cmu.alignvec v23 v8 v15  15
vau.mul.i8 v18 v5 v23 ||lsu1.swzv8 [22222222] ||cmu.alignvec v23 v15 v8  0
lsu1.ldo.64.l  v8 i9   0 || lsu0.ldo.64.h v8 i9   8 ||iau.add i9 i9 16
nop 6
vau.mul.i8 v19 v5 v23 ||lsu1.swzv8 [33333333] ||cmu.alignvec v23 v15 v8  1
vau.mul.i8 v20 v5 v23 ||lsu1.swzv8 [44444444] ||cmu.alignvec v23 v15 v8  2
vau.mul.i8 v21 v5 v23 ||lsu1.swzv8 [55555555] ||cmu.alignvec v23 v15 v8  3
vau.mul.i8 v22 v5 v23 ||lsu1.swzv8 [66666666]
cmu.max.u8 v16 v16 v17 ||lsu1.ldo.64.l  v8 i10   0   || lsu0.ldo.64.h v8 i10   8 ||iau.add i10 i10 16
cmu.max.u8 v18 v18 v19 || lsu1.ldo.64.l  v15 i10   0 || lsu0.ldo.64.h v15 i10 8  ||iau.add i10 i10 16
cmu.max.u8 v20 v20 v21
cmu.max.u8 v16 v16 v18
cmu.max.u8 v20 v20 v22
nop
cmu.max.u8 v14 v16 v20
//line 6
nop
cmu.alignvec v23 v8 v15  13
vau.mul.i8 v16 v6 v23 ||lsu1.swzv8 [00000000] ||cmu.alignvec v23 v8 v15  14
vau.mul.i8 v17 v6 v23 ||lsu1.swzv8 [11111111] ||cmu.alignvec v23 v8 v15  15
vau.mul.i8 v18 v6 v23 ||lsu1.swzv8 [22222222] ||cmu.alignvec v23 v15 v8  0
lsu1.ldo.64.l  v8 i10   0 || lsu0.ldo.64.h v8 i10   8 ||iau.add i10 i10 16
nop 6
vau.mul.i8 v19 v6 v23 ||lsu1.swzv8 [33333333] ||cmu.alignvec v23 v15 v8  1
vau.mul.i8 v20 v6 v23 ||lsu1.swzv8 [44444444] ||cmu.alignvec v23 v15 v8  2
vau.mul.i8 v21 v6 v23 ||lsu1.swzv8 [55555555] ||cmu.alignvec v23 v15 v8  3
vau.mul.i8 v22 v6 v23 ||lsu1.swzv8 [66666666]
cmu.max.u8 v16 v16 v17
cmu.max.u8 v18 v18 v19
cmu.max.u8 v20 v20 v21
cmu.max.u8 v16 v16 v18
cmu.max.u8 v20 v20 v22
nop
cmu.max.u8 v24 v16 v20
cmu.max.u8 v9 v9 v10
cmu.max.u8 v11 v11 v12
cmu.max.u8 v13 v13 v14
cmu.max.u8 v9 v9 v11
cmu.max.u8 v13 v13 v24
nop
cmu.max.u8 v9 v9 v13
nop
lsu1.st.64.l v9 i17 || lsu0.sto.64.h v9 i17 8 || iau.incs i17 16
nop
iau.sub i4 i4 32
iau.sub i5 i5 32
iau.sub i6 i6 32
iau.sub i7 i7 32
iau.sub i8 i8 32
iau.sub i9 i9 32
iau.sub i10 i10 32

lsu1.ldo.64.l  v8 i4   0 || lsu0.ldo.64.h v8 i4   8 ||iau.add i4 i4 16
lsu1.ldo.64.l  v15 i4   0 || lsu0.ldo.64.h v15 i4 8  ||iau.add i4 i4 16
cmu.cmii.i32 i12 i15

nop 6
peu.pc1c LT || bru.bra dilate7x7_loop
nop 6
lsu0.st.64.l v31 i13
LSU0.LD.64.H  v31  i19 || IAU.ADD i19 i19 8
LSU0.LD.64.L  v31  i19 || IAU.ADD i19 i19 8
LSU0.LD.64.H  v30  i19 || IAU.ADD i19 i19 8
LSU0.LD.64.L  v30  i19 || IAU.ADD i19 i19 8
LSU0.LD.64.H  v29  i19 || IAU.ADD i19 i19 8
LSU0.LD.64.L  v29  i19 || IAU.ADD i19 i19 8
LSU0.LD.64.H  v28  i19 || IAU.ADD i19 i19 8
LSU0.LD.64.L  v28  i19 || IAU.ADD i19 i19 8
LSU0.LD.64.H  v27  i19 || IAU.ADD i19 i19 8
LSU0.LD.64.L  v27  i19 || IAU.ADD i19 i19 8
LSU0.LD.64.H  v26  i19 || IAU.ADD i19 i19 8
LSU0.LD.64.L  v26  i19 || IAU.ADD i19 i19 8
LSU0.LD.64.H  v25  i19 || IAU.ADD i19 i19 8
LSU0.LD.64.L  v25  i19 || IAU.ADD i19 i19 8
LSU0.LD.64.H  v24  i19 || IAU.ADD i19 i19 8
LSU0.LD.64.L  v24  i19 || IAU.ADD i19 i19 8


bru.jmp i30

nop 6

.size mvcvDilate7x7_asm,.-mvcvDilate7x7_asm
.end
