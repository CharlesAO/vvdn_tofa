///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvDilate5x5_asm,@function
.version 00.51.04

// Local defines
// Function arguments
.set srcImage1 i18
.set destImage i17
.set kernel i16
.set widthImage i15
.set heightImage i14
.set k i13

// Others registers

.set zero i11

.data .data.dilate
.align 4
.code .text.dilate

//  void Dilate5x5(u8** src, u8** dst, u8** kernel, u32 width, u32 height)
//                   i18       i17        i16        i15         i14
mvcvDilate5x5_asm:
.nowarn 10
lsu0.ldil i10 mvcvDilate5x5_asmloop || lsu1.ldih i10 mvcvDilate5x5_asmloop 
lsu0.ldil i12 0x16 || lsu1.ldih i12 0x0 
LSU0.STO.64.H V24,i19,-8 || LSU1.STO.64.L V24,i19,-16 || iau.incs i19,-16
LSU0.STO.64.H V25,i19,-8 || LSU1.STO.64.L V25,i19,-16 || iau.incs i19,-16

LSU0.STO.64.H V26,i19,-8 || LSU1.STO.64.L V26,i19,-16 || iau.incs i19,-16
LSU0.STO.64.H V27,i19,-8 || LSU1.STO.64.L V27,i19,-16 || iau.incs i19,-16

LSU0.STO.64.H V28,i19,-8 || LSU1.STO.64.L V28,i19,-16 || iau.incs i19,-16
LSU0.STO.64.H V29,i19,-8 || LSU1.STO.64.L V29,i19,-16 || iau.incs i19,-16

LSU0.STO.64.H V30,i19,-8 || LSU1.STO.64.L V30,i19,-16 || iau.incs i19,-16
LSU0.STO.64.H V31,i19,-8 || LSU1.STO.64.L V31,i19,-16 || iau.incs i19,-16
nop
iau.shr.i32 i14 i15 4
iau.shl i13 i14 4
iau.sub i11 i15 i13
lsu0.ldo.32 i17 i17 0
lsu0.ldo.32 i0 i18 0   || lsu1.ldo.32 i5 i16 0
lsu0.ldo.32 i1 i18 4   || lsu1.ldo.32 i6 i16 4
lsu0.ldo.32 i2 i18 8   || lsu1.ldo.32 i7 i16 8
lsu0.ldo.32 i3 i18 12  || lsu1.ldo.32 i8 i16 12
lsu0.ldo.32 i4 i18 16  || lsu1.ldo.32 i9 i16 16
nop 6 

lsu1.ld.64.l v13 i5
lsu1.ld.64.l v17 i6
lsu1.ld.64.l v21 i7
lsu1.ld.64.l v25 i8
lsu1.ld.64.l v29 i9
nop 6
cmu.vilv.x8 v13 v0 v13 v13
cmu.vilv.x8 v17 v1 v17 v17
cmu.vilv.x8 v21 v2 v21 v21
cmu.vilv.x8 v25 v3 v25 v25
cmu.vilv.x8 v29 v4 v29 v29


cmu.cmii.i32 i15 i12
peu.pc1c lt || bru.bra mvcvDilate5x5_asmcompensate
nop 6
bru.rpl i10 i14
LSU0.LDO.64.L v0  i0  0 || LSU1.LDO.64.H v0  i0 8 
LSU0.LDO.64.L v1  i1  0 || LSU1.LDO.64.H v1  i1 8 
LSU0.LDO.64.L v2  i2  0 || LSU1.LDO.64.H v2  i2 8 
LSU0.LDO.64.L v3  i3  0 || LSU1.LDO.64.H v3  i3 8 
LSU0.LDO.64.L v4  i4  0 || LSU1.LDO.64.H v4  i4 8 

LSU0.LDO.64.H v5  i0  -8 || LSU1.LDO.64.L v5  i0 16 || iau.add i0 i0 16
LSU0.LDO.64.H v6  i1  -8 || LSU1.LDO.64.L v6  i1 16 || iau.add i1 i1 16
LSU0.LDO.64.H v7  i2  -8 || LSU1.LDO.64.L v7  i2 16 || iau.add i2 i2 16
LSU0.LDO.64.H v8  i3  -8 || LSU1.LDO.64.L v8  i3 16 || iau.add i3 i3 16
LSU0.LDO.64.H v9  i4  -8 || LSU1.LDO.64.L v9  i4 16 || iau.add i4 i4 16
nop 6

cmu.alignvec v10 v5 v0 14
cmu.alignvec v11 v5 v0 15
cmu.alignvec v12 v0 v5 1
cmu.alignvec v5 v0 v5 2

cmu.alignvec v14 v6 v1 14
cmu.alignvec v15 v6 v1 15
cmu.alignvec v16 v1 v6 1
cmu.alignvec v6 v1 v6 2

cmu.alignvec v18 v7 v2 14
cmu.alignvec v19 v7 v2 15
cmu.alignvec v20 v2 v7 1
cmu.alignvec v7 v2 v7 2

cmu.alignvec v22 v8 v3 14
cmu.alignvec v23 v8 v3 15
cmu.alignvec v24 v3 v8 1
cmu.alignvec v8 v3 v8 2

cmu.alignvec v26 v9 v4 14
cmu.alignvec v27 v9 v4 15
cmu.alignvec v28 v4 v9 1
cmu.alignvec v9 v4 v9 2

vau.mul.i8 v10 v13 v10 ||lsu1.swzv8 [00000000] 
vau.mul.i8 v11 v13 v11 ||lsu1.swzv8 [11111111] 
vau.mul.i8  v0 v13  v0 ||lsu1.swzv8 [22222222] 
vau.mul.i8 v12 v13 v12 ||lsu1.swzv8 [33333333] 
vau.mul.i8 v5  v13 v5  ||lsu1.swzv8 [44444444] 

vau.mul.i8 v14 v17 v14 ||lsu1.swzv8 [00000000] 
vau.mul.i8 v15 v17 v15 ||lsu1.swzv8 [11111111] 
vau.mul.i8  v1 v17  v1 ||lsu1.swzv8 [22222222] 
vau.mul.i8 v16 v17 v16 ||lsu1.swzv8 [33333333] 
vau.mul.i8 v6  v17 v6  ||lsu1.swzv8 [44444444] 

vau.mul.i8 v18 v21 v18 ||lsu1.swzv8 [00000000] 
vau.mul.i8 v19 v21 v19 ||lsu1.swzv8 [11111111] 
vau.mul.i8  v2 v21  v2 ||lsu1.swzv8 [22222222] 
vau.mul.i8 v20 v21 v20 ||lsu1.swzv8 [33333333] 
vau.mul.i8 v7  v21 v7  ||lsu1.swzv8 [44444444] 

vau.mul.i8 v22 v25 v22 ||lsu1.swzv8 [00000000] 
vau.mul.i8 v23 v25 v23 ||lsu1.swzv8 [11111111] 
vau.mul.i8  v3 v25  v3 ||lsu1.swzv8 [22222222] 
vau.mul.i8 v24 v25 v24 ||lsu1.swzv8 [33333333] 
vau.mul.i8 v8  v25 v8  ||lsu1.swzv8 [44444444] 

vau.mul.i8 v26 v29 v26 ||lsu1.swzv8 [00000000] 
vau.mul.i8 v27 v29 v27 ||lsu1.swzv8 [11111111] 
vau.mul.i8  v4 v29  v4 ||lsu1.swzv8 [22222222] 
vau.mul.i8 v28 v29 v28 ||lsu1.swzv8 [33333333] 
vau.mul.i8 v9  v29 v9  ||lsu1.swzv8 [44444444] 
nop 4
cmu.max.u8 v11 v10 v11
cmu.max.u8 v12 v0 v12
cmu.max.u8 v14 v5 v14
cmu.max.u8 v15 v15 v1
cmu.max.u8 v16 v16 v6
cmu.max.u8 v19 v18 v19
cmu.max.u8 v20 v2 v20
cmu.max.u8 v22 v7 v22
cmu.max.u8 v23 v23 v3
cmu.max.u8 v24 v24 v8
cmu.max.u8 v26 v26 v27
cmu.max.u8 v28 v4 v28

cmu.max.u8 v12 v11 v12
cmu.max.u8 v15 v14 v15
cmu.max.u8 v19 v16 v19
cmu.max.u8 v22 v20 v22
cmu.max.u8 v24 v23 v24
cmu.max.u8 v28 v26 v28

cmu.max.u8 v15 v12 v15
cmu.max.u8 v22 v19 v22
cmu.max.u8 v28 v24 v28

cmu.max.u8 v22 v15 v22
cmu.max.u8 v28 v28 v9
nop
cmu.max.u8 v28 v22 v28
nop
lsu1.st.64.l v28 i17 || lsu0.sto.64.h v28 i17 8 || iau.incs i17 16 
mvcvDilate5x5_asmloop:
nop 7

cmu.cmz.i32 i11
peu.pc1c eq || bru.bra mvcvDilate5x5_asmexit
mvcvDilate5x5_asmcompensate:
LSU0.LDO.64.L v0  i0  0 || LSU1.LDO.64.H v0  i0 8 
LSU0.LDO.64.L v1  i1  0 || LSU1.LDO.64.H v1  i1 8 
LSU0.LDO.64.L v2  i2  0 || LSU1.LDO.64.H v2  i2 8 
LSU0.LDO.64.L v3  i3  0 || LSU1.LDO.64.H v3  i3 8 
LSU0.LDO.64.L v4  i4  0 || LSU1.LDO.64.H v4  i4 8 

LSU0.LDO.64.H v5  i0  -8 || LSU1.LDO.64.L v5  i0 16 || iau.add i0 i0 16
LSU0.LDO.64.H v6  i1  -8 || LSU1.LDO.64.L v6  i1 16 || iau.add i1 i1 16
LSU0.LDO.64.H v7  i2  -8 || LSU1.LDO.64.L v7  i2 16 || iau.add i2 i2 16
LSU0.LDO.64.H v8  i3  -8 || LSU1.LDO.64.L v8  i3 16 || iau.add i3 i3 16
LSU0.LDO.64.H v9  i4  -8 || LSU1.LDO.64.L v9  i4 16 || iau.add i4 i4 16
nop 6

cmu.alignvec v10 v5 v0 14
cmu.alignvec v11 v5 v0 15
cmu.alignvec v12 v0 v5 1
cmu.alignvec v5 v0 v5 2

cmu.alignvec v14 v6 v1 14
cmu.alignvec v15 v6 v1 15
cmu.alignvec v16 v1 v6 1
cmu.alignvec v6 v1 v6 2

cmu.alignvec v18 v7 v2 14
cmu.alignvec v19 v7 v2 15
cmu.alignvec v20 v2 v7 1
cmu.alignvec v7 v2 v7 2
cmu.alignvec v22 v8 v3 14
cmu.alignvec v23 v8 v3 15
cmu.alignvec v24 v3 v8 1
cmu.alignvec v8 v3 v8 2
cmu.alignvec v26 v9 v4 14
cmu.alignvec v27 v9 v4 15
cmu.alignvec v28 v4 v9 1
cmu.alignvec v9 v4 v9 2

vau.mul.i8 v10 v13 v10 ||lsu1.swzv8 [00000000] 
vau.mul.i8 v11 v13 v11 ||lsu1.swzv8 [11111111] 
vau.mul.i8  v0 v13  v0 ||lsu1.swzv8 [22222222] 
vau.mul.i8 v12 v13 v12 ||lsu1.swzv8 [33333333] 
vau.mul.i8 v5  v13 v5  ||lsu1.swzv8 [44444444] 
vau.mul.i8 v14 v17 v14 ||lsu1.swzv8 [00000000] 
vau.mul.i8 v15 v17 v15 ||lsu1.swzv8 [11111111] 
vau.mul.i8  v1 v17  v1 ||lsu1.swzv8 [22222222] 
vau.mul.i8 v16 v17 v16 ||lsu1.swzv8 [33333333] 
vau.mul.i8 v6  v17 v6  ||lsu1.swzv8 [44444444] 
vau.mul.i8 v18 v21 v18 ||lsu1.swzv8 [00000000] 
vau.mul.i8 v19 v21 v19 ||lsu1.swzv8 [11111111] 
vau.mul.i8  v2 v21  v2 ||lsu1.swzv8 [22222222] 
vau.mul.i8 v20 v21 v20 ||lsu1.swzv8 [33333333] 
vau.mul.i8 v7  v21 v7  ||lsu1.swzv8 [44444444] 
vau.mul.i8 v22 v25 v22 ||lsu1.swzv8 [00000000] 
vau.mul.i8 v23 v25 v23 ||lsu1.swzv8 [11111111] 
vau.mul.i8  v3 v25  v3 ||lsu1.swzv8 [22222222] 
vau.mul.i8 v24 v25 v24 ||lsu1.swzv8 [33333333] 
vau.mul.i8 v8  v25 v8  ||lsu1.swzv8 [44444444] 
vau.mul.i8 v26 v29 v26 ||lsu1.swzv8 [00000000] 
vau.mul.i8 v27 v29 v27 ||lsu1.swzv8 [11111111] 
vau.mul.i8  v4 v29  v4 ||lsu1.swzv8 [22222222] 
vau.mul.i8 v28 v29 v28 ||lsu1.swzv8 [33333333] 
vau.mul.i8 v9  v29 v9  ||lsu1.swzv8 [44444444] 
nop 2
cmu.max.u8 v11 v10 v11
cmu.max.u8 v12 v0 v12
cmu.max.u8 v14 v5 v14
cmu.max.u8 v15 v15 v1
cmu.max.u8 v16 v16 v6
cmu.max.u8 v19 v18 v19
cmu.max.u8 v20 v2 v20
cmu.max.u8 v11 v9 v11
cmu.max.u8 v22 v7 v22
cmu.max.u8 v23 v23 v3
cmu.max.u8 v24 v24 v8
cmu.max.u8 v26 v26 v27
cmu.max.u8 v28 v4 v28
cmu.max.u8 v12 v11 v12
cmu.max.u8 v15 v14 v15
cmu.max.u8 v19 v16 v19
cmu.max.u8 v22 v20 v22
cmu.max.u8 v24 v23 v24
cmu.max.u8 v28 v26 v28
cmu.max.u8 v15 v12 v15
cmu.max.u8 v22 v19 v22
cmu.max.u8 v28 v24 v28
cmu.max.u8 v22 v15 v22
cmu.max.u8 v28 v28 v9
nop
cmu.max.u8 v28 v22 v28
nop
lsu1.st.64.l v28 i17 
mvcvDilate5x5_asmexit:

LSU0.LDO.64.L V31,i19,0 || LSU1.LDO.64.H V31, i19, 8 || iau.incs i19,16
LSU0.LDO.64.L V30,i19,0 || LSU1.LDO.64.H V30, i19, 8 || iau.incs i19,16
LSU0.LDO.64.L V29,i19,0 || LSU1.LDO.64.H V29, i19, 8 || iau.incs i19,16
LSU0.LDO.64.L V28,i19,0 || LSU1.LDO.64.H V28, i19, 8 || iau.incs i19,16
LSU0.LDO.64.L V27,i19,0 || LSU1.LDO.64.H V27, i19, 8 || iau.incs i19,16
LSU0.LDO.64.L V26,i19,0 || LSU1.LDO.64.H V26, i19, 8 || iau.incs i19,16
LSU0.LDO.64.L V25,i19,0 || LSU1.LDO.64.H V25, i19, 8 || iau.incs i19,16
LSU0.LDO.64.L V24,i19,0 || LSU1.LDO.64.H V24, i19, 8 || iau.incs i19,16

BRU.JMP i30
NOP 6
.nowarnend
.size mvcvDilate5x5_asm,.-mvcvDilate5x5_asm
.end
