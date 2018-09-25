///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvConv3x3fp32Scharr_asm,@function
.version 00.50.00
.nowarn
//void conv3x3fp32Scharr(
//      const u8* src,             -- i18
//      float* dstx,               -- i17
//      float* dsty,               -- i16
//      const float* smooth_k      -- unused
//      int height                 -- i14
//      int width                  -- i13
// )

.code .text.MvCV_conv3x3fp32Scharr
.align 16

.lalign
mvcvConv3x3fp32Scharr_asm:
// use optimized implementations for these sizes
IAU.SUB i10 i13 13
PEU.PC1I EQ || BRU.BRA mvcvConv3x3fp32_13x13_asm
IAU.SUB i10 i13 21
PEU.PC1I EQ || BRU.BRA mvcvConv3x3fp32_21x21_asm
IAU.SUB i14 i14 2

LSU0.LDIL i3 0.03125F16 || IAU.ADD i1 i18 i13
CMU.CPIVR.x16 v20 i3	|| SAU.ADD.i32 i2 i1 i13 || IAU.XOR i8 i8 i8
IAU.ADD i5 i13 0 		|| LSU0.LDIL i8 8		 || LSU1.CP i4 i13
CMU.MIN.i16 i3 i5 i8
nop

LSU0.LD.128.U8.U16 v10 i18 || IAU.ADD i18 i18 i3
LSU0.LD.128.U8.U16 v12 i1  || IAU.ADD i1 i1 i3
LSU0.LD.128.U8.U16 v14 i2  || IAU.ADD i2 i2 i3

IAU.SUB i5 i5 8
PEU.PC1I LTE || LSU0.CP i5 i13
LSU0.LDIL i10 3
LSU0.LDIL i11 10
CMU.CPIVR.x16 v0 i10
CMU.CPIVR.x16 v1 i11

VAU.SUB.i16 v5 v14 v10 || IAU.FEXTI i11 i13 0 3
VAU.MACPZ.i16 v0 v10   || CMU.CMZ.i32 i11
VAU.MACP.i16 v1 v12	   || IAU.SUB i6 i11 2
VAU.MACPW.i16 v2, v0, v14 || CMU.MIN.i16 i3 i5 i8
PEU.PC1C GT || IAU.SUB i6 i6 8


LSU0.LD.128.U8.U16 v11 i18 || IAU.ADD i18 i18 i3
LSU0.LD.128.U8.U16 v13 i1  || IAU.ADD i1 i1 i3
LSU0.LD.128.U8.U16 v15 i2  || IAU.ADD i2 i2 i3
iau.sub i5 i5 8
PEU.PC1I LTE || LSU0.CP i5 i13
IAU.ADD i12 i13 7
IAU.SHR.I32 i12 i12 3
IAU.SHL i6 i6 2		|| LSU0.LDIL i11 mvcvConv3x3fp32Scharr_loop_end
IAU.MUL i12 i12 i14 || LSU0.LDIH i11 mvcvConv3x3fp32Scharr_loop_end

VAU.SUB.i16 v6 v15 v11 || CMU.MIN.i16 i3 i5 i8
VAU.MACPZ.i16 v0 v11
VAU.MACP.i16 v1 v13		  || LSU0.LD.128.U8.U16 v11 i18 || IAU.ADD i18 i18 i3
VAU.MACPW.i16 v3, v0, v15 || LSU0.LD.128.U8.U16 v13 i1  || IAU.ADD i1 i1 i3
nop
CMU.ALIGNVEC v7 v5 v6 2 || LSU0.LD.128.U8.U16 v15 i2  || IAU.ADD i2 i2 i3
CMU.ALIGNVEC v8 v5 v6 4	|| IAU.XOR i9 i9 i9
CMU.ALIGNVEC v4 v2 v3 4 || LSU0.LDIL i9 1
VAU.SUB.i16 v4 v4 v2	|| IAU.SUB i10 i4 2

VAU.MACPZ.i16 v0 v5		|| CMU.MIN.i16 i7 i10 i8
VAU.MACP.i16 v1 v7
VAU.MACPW.i16 v9, v0, v8 || IAU.SUB i12 i12 1
cmu.cmz.i32 i12 || iau.sub i5 i5 8
PEU.PC1C EQ  || BRU.BRA mvcvConv3x3fp32Scharr_end
PEU.PC1I LTE || LSU0.CP i5 i13

CMU.CPVV.i16.F16 v4 v4|| iau.shl i7 i9 i7
CMU.CPVV.i16.F16 v9 v9|| iau.sub i7 i7 1
VAU.MUL.F16 v4 v4 v20 || LSU0.CP v2 v3
VAU.MUL.F16 v9 v9 v20 || LSU0.CP v5 v6 || CMU.CMASK V21 i7 2
CMU.CMZ.i32 v21

mvcvConv3x3fp32Scharr_loop:
BRU.RPL i11 i12	|| CMU.CPVV.F16L.F32 v16 v4 || iau.shr.u32 i7 i7 4
VAU.SUB.i16 v6 v15 v11 || CMU.CPVV.F16H.F32 v17 v4
VAU.MACPZ.i16 v0 v11   || CMU.CPVV.F16L.F32 v18 v9
VAU.MACP.i16 v1 v13	   || CMU.CPVV.F16H.F32 v19 v9
VAU.MACPW.i16 v3, v0, v15	|| LSU0.STXV v16 i17 || IAU.ADD i17 i17 16 || CMU.MIN.i16 i3 i5 i8 || PEU.PVL032 NEQ
LSU1.STXV v18 i16 || IAU.ADD i16 i16 16 || PEU.PVL032 NEQ || CMU.CMASK V21 i7 2
CMU.CMZ.i32 v21
CMU.ALIGNVEC v7 v5 v6 2 || LSU0.STXV v17 i17 || IAU.ADD i17 i17 16 || PEU.PVL032 NEQ
CMU.ALIGNVEC v8 v5 v6 4 || LSU1.STXV v19 i16 || IAU.ADD i16 i16 16 || PEU.PVL032 NEQ
CMU.ALIGNVEC v4 v2 v3 4 || IAU.SUB i4 i4 8	 || sau.sub.i32 i5 i5 8
VAU.SUB.i16 v4 v4 v2	|| LSU0.LD.128.U8.U16 v11 i18 || IAU.ADD i18 i18 i3 || CMU.CMZ.i16 i4
PEU.PC1C LTE || LSU0.CP i4 i13
VAU.MACPZ.i16 v0 v5		|| LSU0.LD.128.U8.U16 v13 i1  || IAU.SUB i10 i4 2
VAU.MACP.i16 v1 v7		|| LSU0.LD.128.U8.U16 v15 i2  || IAU.ADD i1 i1 i3 || CMU.MIN.i16 i7 i10 i8
VAU.MACPW.i16 v9, v0, v8 || IAU.ADD i2 i2 i3
mvcvConv3x3fp32Scharr_loop_end:
PEU.PC1S LTE || LSU0.CP i5 i13
CMU.CPVV.i16.F16 v4 v4 || iau.shl i7 i9 i7
PEU.PC1C LTE  || IAU.ADD i16 i16 i6   || SAU.ADD.I32 i17 i17 i6

CMU.CPVV.i16.F16 v9 v9  || iau.sub i7 i7 1
VAU.MUL.F16 v4 v4 v20	|| LSU1.CP v2 v3
VAU.MUL.F16 v9 v9 v20	|| LSU1.CP v5 v6 || CMU.CMASK V21 i7 2
CMU.CMZ.i32 v21
// end of loop

mvcvConv3x3fp32Scharr_end:
CMU.CPVV.F16L.F32 v16 v4
CMU.CPVV.F16H.F32 v17 v4
CMU.CPVV.F16L.F32 v18 v9
CMU.CPVV.F16H.F32 v19 v9 || IAU.SHR.u32 i7 i7 4
PEU.PVL032 NEQ || LSU0.STXV v16 i17 || IAU.ADD i17 i17 16
PEU.PVL032 NEQ || LSU1.STXV v18 i16 || IAU.ADD i16 i16 16
CMU.CMASK V21 i7 2
CMU.CMZ.i32 v21
PEU.PVL032 NEQ || LSU0.STXV v17 i17 || IAU.ADD i17 i17 16
PEU.PVL032 NEQ || LSU1.STXV v19 i16 || IAU.ADD i16 i16 16

BRU.JMP i30
nop 6


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


.lalign
mvcvConv3x3fp32_13x13_asm:
// load in the 3 rows we require (un-aligned loads)
// src1

LSU0.LD.128.U8.U16 v10, i18   || iau.incs i18, 8  || lsu1.ldil i5, 3
LSU0.LD.128.U8.U16 v11, i18   || iau.incs i18, 5  || lsu1.ldil i6, 10
// src2
LSU0.LD.128.U8.U16 v12, i18   || iau.incs i18, 8  || cmu.cpivr.x16 v16, i5
LSU0.LD.128.U8.U16 v13, i18   || iau.incs i18, 5  || cmu.cpivr.x16 v17, i6 || lsu1.ldil i8, mvcvConv3x3fp32_13x13_loop_end
// src3
LSU0.LD.128.U8.U16 v14, i18   || iau.incs i18, 8 || lsu1.ldih i8, mvcvConv3x3fp32_13x13_loop_end
LSU0.LD.128.U8.U16 v15, i18   || iau.incs i18, 5

// convervative
 nop 3

// b0[2] = src3*k0 + src2*k1 + src1*k0
vau.macpz.i16      v10, v16 || bru.rpl i8 i14
vau.macp.i16       v12, v17
vau.macpw.i16  v0, v14, v16
vau.macpz.i16      v11, v16
vau.macp.i16       v13, v17
vau.macpw.i16  v1, v15, v16
nop 2

// b1[2]  = src3 - src1
vau.sub.i16    v2, v14, v10
vau.sub.i16    v3, v15, v11 || cmu.cpvv v10 v12
cmu.cpvv v12 v14
// b0[0]  = b0[2] shifted left by two
CMU.ALIGNVEC   v4, v0, v1, 4 || lsu0.cp v11 v13
CMU.ALIGNVEC v5, V1, v1, 4 || lsu0.cp v13 v15
// b1[0]  = b1[2] shifted left by two
CMU.ALIGNVEC   v6, v2, v3, 4 || LSU0.LD.128.U8.U16 v14, i18   || iau.incs i18, 8
CMU.ALIGNVEC v7, V3, v3, 4 || LSU0.LD.128.U8.U16 v15, i18   || iau.incs i18, 5
// b1[1]  = b1[2] shifted left by one
CMU.ALIGNVEC   v8, v2, v3, 2
CMU.ALIGNVEC v9, V3, v3, 2


// for dstx and dsty, first do calc, then convert to fp16, then scale by 1/32, then convert to fp32
// dstx = b0[0] - b0[2]
vau.sub.i16    v4, v4, v0    || lsu0.ldil i0, 0.03125F16
vau.sub.i16    v5, v5, v1    || cmu.cpivr.x16 v18, i0

// dsty = b1[2]*k0 + b1[1]*k1 +b1[0]*k0//
vau.macpz.i16      v2, v16   || cmu.cpvv.i16.f16 v4, v4
vau.macp.i16       v8, v17   || cmu.cpvv.i16.f16 v5, v5
vau.macpw.i16  v0, v6, v16
vau.macpz.i16      v3, v16
vau.macp.i16       v9, v17
vau.macpw.i16  v1, v7, v16
NOP
vau.mul.f16   v4, v4, v18   || cmu.cpvv.i16.f16 v0, v0
vau.mul.f16   v5, v5, v18
vau.mul.f16   v0, v0, v18
cmu.cpvv.f16l.f32 v19 v4
cmu.cpvv.f16h.f32 v20 v4
cmu.cpvv.i16.f16 v1, v1  || LSU0.ST.64.L v19,  i17  || iau.incs i17, 8
LSU0.ST.64.H v19,  i17  || iau.incs i17, 8
cmu.cpvv.f16l.f32 v21 v5 || LSU0.ST.64.L v20,  i17  || iau.incs i17, 8
vau.mul.f16   v1, v1, v18    || cmu.cpvi.x32 i12, v21.2  || LSU0.ST.64.H v20,  i17  || iau.incs i17, 8
cmu.cpvv.f16l.f32 v19 v0 || LSU0.ST.64.L v21,  i17  || iau.incs i17, 8
mvcvConv3x3fp32_13x13_loop_end:
cmu.cpvv.f16h.f32 v20 v0 || LSU0.ST.32   i12,  i17 || iau.incs i17, 4
cmu.cpvv.f16l.f32 v21 v1 || LSU0.ST.64.L v19,  i16  || iau.incs i16, 8
cmu.cpvi.x32 i12, v21.2  || LSU0.ST.64.H v19,  i16  || iau.incs i16, 8
LSU0.ST.64.L v20,  i16  || iau.incs i16, 8
LSU0.ST.64.H v20,  i16  || iau.incs i16, 8
LSU0.ST.64.L v21,  i16  || iau.incs i16, 8
LSU0.ST.32  i12,  i16 || iau.incs i16, 4

 bru.jmp i30
 nop 6

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


.lalign
mvcvConv3x3fp32_21x21_asm:
// load in the 3 rows we require (un-aligned loads)
// src1
LSU0.LD.128.U8.u16 v10 i18   || iau.incs i18 8
LSU0.LD.128.U8.u16 v11 i18   || iau.incs i18 8
LSU0.LD.128.U8.u16 v12 i18   || iau.incs i18 5
// src2
LSU0.LD.128.U8.u16 v13 i18   || iau.incs i18 8
LSU0.LD.128.U8.u16 v14 i18   || iau.incs i18 8
LSU0.LD.128.U8.u16 v15 i18   || iau.incs i18 5
// src3
LSU0.LD.128.U8.u16 v16 i18   || iau.incs i18 8
LSU0.LD.128.U8.u16 v17 i18   || iau.incs i18 8
LSU0.LD.128.U8.u16 v18 i18   || iau.incs i18 5

lsu1.ldil i8 mvcvConv3x3fp32_21x21_loop_end
lsu1.ldih i8 mvcvConv3x3fp32_21x21_loop_end
lsu1.ldil i5 3
cmu.cpivr.x16 v0 i5	|| lsu1.ldil i6 10
cmu.cpivr.x16 v1 i6	|| lsu0.ldil i0 0.03125F16 || iau.sub i19 i19 64
cmu.cpivr.x16 v20 i0    || LSU0.ST.64.H v22 i19 || iau.sub i19 i19 64

vau.sub.i16     v5 v16 v10 || LSU0.ST.64.L v22 i19 || iau.sub i19 i19 64
vau.sub.i16     v6 v17 v11 || LSU0.ST.64.H v23 i19 || iau.sub i19 i19 64
vau.sub.i16     v7 v18 v12 || LSU0.ST.64.L v23 i19 || iau.sub i19 i19 64
vau.macpz.i16       v10 v0 || LSU0.ST.64.H v24 i19 || iau.sub i19 i19 64
vau.macp.i16        v13 v1 || LSU0.ST.64.L v24 i19 || iau.sub i19 i19 64
vau.macpw.i16    v2 v16 v0 || LSU0.ST.64.H v25 i19 || iau.sub i19 i19 64
vau.macpz.i16       v11 v0 || LSU0.ST.64.L v25 i19 || iau.sub i19 i19 64
vau.macp.i16        v14 v1 || LSU0.ST.64.H v26 i19 || iau.sub i19 i19 64
vau.macpw.i16    v3 v17 v0 || LSU0.ST.64.L v26 i19 || iau.sub i19 i19 64
vau.macpz.i16       v12 v0 || LSU0.ST.64.H v27 i19 || iau.sub i19 i19 64
vau.macp.i16        v15 v1 || LSU0.ST.64.L v27 i19 || iau.sub i19 i19 64
vau.macpw.i16    v4 v18 v0 || LSU0.ST.64.H v28 i19 || iau.sub i19 i19 64

vau.macpz.i16 v5 v0		|| CMU.ALIGNVEC v27 v5 v6 2 || LSU0.ST.64.L v28 i19 || iau.sub i19 i19 64
vau.macp.i16  v27 v1	|| CMU.ALIGNVEC v9 v5 v6 4  || LSU0.ST.64.H v29 i19 || iau.sub i19 i19 64
vau.macpw.i16 v5 v9 v0	|| CMU.ALIGNVEC v28 v6 v7 2 || LSU0.ST.64.L v29 i19
vau.macpz.i16 v6 v0		|| CMU.ALIGNVEC v29 v6 v7 4
vau.macp.i16  v28 v1	|| CMU.ALIGNVEC v21 v7 v7 2
vau.macpw.i16 v6 v29 v0	|| CMU.ALIGNVEC v22 v7 v7 4	|| LSU0.CP v10 v13
vau.macpz.i16 v7 v0		|| CMU.ALIGNVEC v8 v2 v3 4	|| LSU0.CP v11 v14
vau.macp.i16  v21 v1	|| CMU.ALIGNVEC v9 v3 v4 4	|| LSU0.CP v12 v15
vau.macpw.i16 v7 v22 v0	|| CMU.ALIGNVEC v28 v4 v4 4	|| LSU0.CP v13 v16
cmu.cpvv.i16.f16 v5 v5	|| LSU0.CP v14 v17
cmu.cpvv.i16.f16 v6 v6	|| LSU0.CP v15 v18
VAU.SUB.i16 v2 v8 v2	|| LSU0.LD.128.U8.u16 v16 i18   || iau.incs i18 8
VAU.SUB.i16 v3 v9 v3	|| LSU0.LD.128.U8.u16 v17 i18   || iau.incs i18 8
VAU.SUB.i16 v4 v28 v4	|| LSU0.LD.128.U8.u16 v18 i18   || iau.incs i18 5
cmu.cpvv.i16.f16 v2 v2	|| vau.mul.f16 v5 v5 v20
cmu.cpvv.i16.f16 v3 v3	|| vau.mul.f16 v6 v6 v20
cmu.cpvv.i16.f16 v4 v4 || vau.mul.f16 v2 v2 v20
cmu.cpvv.i16.f16 v7 v7	|| vau.mul.f16 v3 v3 v20
vau.mul.f16 v4 v4 v20
vau.mul.f16 v7 v7 v20  || IAU.SUB i14 i14 1

.lalign
mvcvConv3x3fp32_21x21_loop:
BRU.RPL i8 i14 || cmu.cpvv.f16l.f32 v28 v5
vau.sub.i16 v5 v16 v10 || cmu.cpvv.f16h.f32 v29 v5
cmu.cpvv.f16l.f32 v23 v6 || LSU0.STO.64.L v28,  i16 0x00
vau.sub.i16    v6 v17 v11 || cmu.cpvv.f16h.f32 v24 v6 || LSU0.STO.64.H v28 i16 0x08
vau.sub.i16    v7 v18 v12 || cmu.cpvv.f16l.f32 v26 v7 || LSU0.STO.64.L v29 i16 0x10
vau.macpz.i16      v10 v0 || cmu.cpvv.f16l.f32 v9 v2  || LSU0.STO.64.H v29 i16 0x18
vau.macp.i16       v13 v1 || cmu.cpvv.f16h.f32 v27 v2 || LSU0.STO.64.L v23 i16 0x20
vau.macpw.i16   v2 v16 v0 || cmu.cpvv.f16l.f32 v21 v3 || LSU0.STO.64.H v23 i16 0x28
vau.macpz.i16      v11 v0 || cmu.cpvv.f16h.f32 v22 v3 || LSU0.STO.64.L v27 i17 0x10
vau.macp.i16       v14 v1 || cmu.cpvv.f16l.f32 v25 v4 || LSU0.STO.64.H v27 i17 0x18
vau.macpw.i16   v3 v17 v0 || cmu.cpvi.x32 i0 v25.2    || LSU0.STO.64.L v9  i17 0x00
vau.macpz.i16      v12 v0 || cmu.cpvi.x32 i1 v26.2    || LSU0.STO.64.H v9  i17 0x08
vau.macp.i16       v15 v1 || LSU0.STO.64.L v21 i17 0x20
vau.macpw.i16   v4 v18 v0 || LSU0.STO.64.H v21 i17 0x28

vau.macpz.i16 v5 v0		|| CMU.ALIGNVEC v27 v5 v6 2 || LSU0.STO.64.L v22 i17 0x30
vau.macp.i16  v27 v1	|| CMU.ALIGNVEC v9 v5 v6 4  || LSU0.STO.64.H v22 i17 0x38
vau.macpw.i16 v5 v9 v0	|| CMU.ALIGNVEC v28 v6 v7 2 || LSU0.STO.64.L v24 i16 0x30
vau.macpz.i16 v6 v0		|| CMU.ALIGNVEC v29 v6 v7 4 || LSU0.STO.64.H v24 i16 0x38
vau.macp.i16  v28 v1	|| CMU.ALIGNVEC v21 v7 v7 2
vau.macpw.i16 v6 v29 v0	|| CMU.ALIGNVEC v22 v7 v7 4	|| LSU0.CP v10 v13
vau.macpz.i16 v7 v0		|| CMU.ALIGNVEC v8 v2 v3 4	|| LSU0.CP v11 v14
vau.macp.i16  v21 v1	|| CMU.ALIGNVEC v9 v3 v4 4	|| LSU0.CP v12 v15
vau.macpw.i16 v7 v22 v0	|| CMU.ALIGNVEC v28 v4 v4 4	|| LSU0.CP v13 v16
cmu.cpvv.i16.f16 v5 v5	|| LSU0.CP v14 v17
cmu.cpvv.i16.f16 v6 v6	|| LSU0.CP v15 v18
VAU.SUB.i16 v2 v8 v2	|| LSU0.LD.128.U8.u16 v16 i18   || iau.incs i18, 8
VAU.SUB.i16 v3 v9 v3	|| LSU0.LD.128.U8.u16 v17 i18   || iau.incs i18, 8
.lalign
mvcvConv3x3fp32_21x21_loop_end:
VAU.SUB.i16 v4 v28 v4	|| LSU0.LD.128.U8.u16 v18, i18   || iau.incs i18, 5
cmu.cpvv.i16.f16 v2 v2	|| vau.mul.f16 v5 v5 v20 || LSU0.STO.64.L v25 i17 0x40 || IAU.ADD i17 i17 0x48
cmu.cpvv.i16.f16 v3 v3	|| vau.mul.f16 v6 v6 v20 || LSU0.STO.64.L v26 i16 0x40 || IAU.ADD i16 i16 0x48
cmu.cpvv.i16.f16 v4 v4  || vau.mul.f16 v2 v2 v20
cmu.cpvv.i16.f16 v7 v7	|| vau.mul.f16 v3 v3 v20
vau.mul.f16 v4 v4 v20	|| LSU0.ST.32 i0 i17  || iau.incs i17 4
vau.mul.f16 v7 v7 v20	|| LSU0.ST.32 i1 i16  || iau.incs i16 4

cmu.cpvv.f16l.f32 v9 v2
cmu.cpvv.f16h.f32 v27 v2
cmu.cpvv.f16l.f32 v28 v5	|| LSU0.STO.64.L v9 i17 0x00
cmu.cpvv.f16h.f32 v29 v5	|| LSU0.STO.64.H v9 i17 0x08
cmu.cpvv.f16l.f32 v21 v3	|| LSU0.STO.64.L v27 i17 0x10
cmu.cpvv.f16h.f32 v22 v3	|| LSU0.STO.64.H v27 i17 0x18
cmu.cpvv.f16l.f32 v23 v6	|| LSU0.STO.64.L v28 i16 0x00
cmu.cpvv.f16h.f32 v24 v6	|| LSU0.STO.64.H v28 i16 0x08
cmu.cpvv.f16l.f32 v25 v4	|| LSU0.STO.64.L v29 i16 0x10
cmu.cpvv.f16l.f32 v26 v7	|| LSU0.STO.64.H v29 i16 0x18
cmu.cpvi.x32 i0 v25.2		|| LSU0.STO.64.L v21 i17 0x20
cmu.cpvi.x32 i1 v26.2		|| LSU0.STO.64.H v21 i17 0x28

LSU0.STO.64.L v22 i17 0x30
LSU0.STO.64.H v22 i17 0x38
LSU0.STO.64.L v23 i16 0x20
LSU0.STO.64.H v23 i16 0x28
LSU0.STO.64.L v24 i16 0x30
LSU0.STO.64.H v24 i16 0x38
LSU0.STO.64.L v25 i17 0x40
LSU0.STO.64.L v26 i16 0x40
IAU.ADD i17 i17 0x48
IAU.ADD i16 i16 0x48
LSU0.ST.32 i0 i17  || iau.incs i17 4
LSU0.ST.32 i1 i16  || iau.incs i16 4

LSU0.LD.64.L v29 i19 || IAU.ADD i19 i19 64
LSU0.LD.64.H v29 i19 || IAU.ADD i19 i19 64
LSU0.LD.64.L v28 i19 || IAU.ADD i19 i19 64
LSU0.LD.64.H v28 i19 || IAU.ADD i19 i19 64
LSU0.LD.64.L v27 i19 || IAU.ADD i19 i19 64
LSU0.LD.64.H v27 i19 || IAU.ADD i19 i19 64
LSU0.LD.64.L v26 i19 || IAU.ADD i19 i19 64
LSU0.LD.64.H v26 i19 || IAU.ADD i19 i19 64
LSU0.LD.64.L v25 i19 || IAU.ADD i19 i19 64
LSU0.LD.64.H v25 i19 || IAU.ADD i19 i19 64
LSU0.LD.64.L v24 i19 || IAU.ADD i19 i19 64
LSU0.LD.64.H v24 i19 || IAU.ADD i19 i19 64
LSU0.LD.64.L v23 i19 || IAU.ADD i19 i19 64
LSU0.LD.64.H v23 i19 || IAU.ADD i19 i19 64
LSU0.LD.64.L v22 i19 || IAU.ADD i19 i19 64
LSU0.LD.64.H v22 i19 || IAU.ADD i19 i19 64

 bru.jmp i30
 nop 6
.nowarnend
.size mvcvConv3x3fp32Scharr_asm,.-mvcvConv3x3fp32Scharr_asm
.end
