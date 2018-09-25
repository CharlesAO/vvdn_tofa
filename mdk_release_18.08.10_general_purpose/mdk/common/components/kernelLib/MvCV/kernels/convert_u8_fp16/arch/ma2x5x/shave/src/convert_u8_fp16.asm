/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvConvert_u8_fp16_asm,@function
.version 00.51.04

.data .data.convert_u8_fp16
.align 4

.code .text.convert_u8_fp16

//                      i18      i17       i16
//mvcvConvert_u8_fp16(u8* in, half* out, u32 width)
.nowarn
.lalign
mvcvConvert_u8_fp16_asm:
IAU.SHR.u32 i14 i16 6
    || LSU1.LDIL i12 mvcvConvert_u8_fp16_end_loop
    || LSU0.LDIH i12 mvcvConvert_u8_fp16_end_loop
CMU.CMZ.i32 i14 || IAU.ADD i15 i18 8
IAU.ADD i13 i15 8
PEU.PC1C EQ || BRU.BRA mvcvConvert_u8_fp16_compensation_64
LSU1.LDIL i0 0x3F || LSU0.LDIH i0 0x0 || IAU.ADD i11 i13 8 // ==> mask = 63 (0x3F)
IAU.ADD i9 i11 8
LSU1.LDIL i1 0x08   || LSU0.LDIH i1 0x0 || IAU.ADD i7 i9 8  // ==> i1 = 8
LSU1.LDIL i2 0x40   || LSU0.LDIH i2 0x0 || IAU.ADD i5 i7 8  // ==> i2 = 64
IAU.ADD i3 i5 8
IAU.AND i10 i16 i0

.lalign
mvcvConvert_u8_fp16_start_loop:
BRU.RPL i12 i14
LSU1.LD.128.u8.f16 v0 i18 || LSU0.LD.128.u8.f16 v1 i15 || IAU.ADD i18 i18 64 || SAU.ADD.i32 i15 i15 i2
LSU1.LD.128.u8.f16 v2 i13 || LSU0.LD.128.u8.f16 v3 i11 || IAU.ADD i13 i13 64 || SAU.ADD.i32 i11 i11 i2
LSU1.LD.128.u8.f16 v4 i9 || LSU0.LD.128.u8.f16 v5 i7 || IAU.ADD i9 i9 64 || SAU.ADD.i32 i7 i7 i2
LSU1.LD.128.u8.f16 v6 i5 || LSU0.LD.128.u8.f16 v7 i3 || IAU.ADD i5 i5 64 || SAU.ADD.i32 i3 i3 i2
nop 3

LSU1.ST.64.l v0 i17  || LSU0.STO.64.h v0 i17 8 || IAU.ADD i17 i17 16
.lalign
mvcvConvert_u8_fp16_end_loop:

LSU1.ST.64.l v1 i17  || LSU0.STO.64.h v1 i17 8 || IAU.ADD i17 i17 16

LSU1.ST.64.l v2 i17  || LSU0.STO.64.h v2 i17 8 || IAU.ADD i17 i17 16
LSU1.ST.64.l v3 i17  || LSU0.STO.64.h v3 i17 8 || IAU.ADD i17 i17 16
LSU1.ST.64.l v4 i17  || LSU0.STO.64.h v4 i17 8 || IAU.ADD i17 i17 16
LSU1.ST.64.l v5 i17  || LSU0.STO.64.h v5 i17 8 || IAU.ADD i17 i17 16
LSU1.ST.64.l v6 i17  || LSU0.STO.64.h v6 i17 8 || IAU.ADD i17 i17 16
LSU1.ST.64.l v7 i17  || LSU0.STO.64.h v7 i17 8 || IAU.ADD i17 i17 16

IAU.AND i10 i16 i0
PEU.PC1I EQ || BRU.JMP i30
nop 6

.lalign
mvcvConvert_u8_fp16_compensation_64:
CMU.CMII.u32 i10 i1
PEU.PC1C LT
    || BRU.BRA mvcvConvert_u8_fp16_compensation_8
LSU1.LD.128.u8.f16 v0 i18 || IAU.ADD i18 i18 8
nop 5
BRU.BRA mvcvConvert_u8_fp16_compensation_64
LSU1.ST.64.l v0 i17  || LSU0.STO.64.h v0 i17 8 || IAU.ADD i17 i17 16
IAU.SUB i10 i10 8
nop 4

.lalign
mvcvConvert_u8_fp16_compensation_8:
CMU.CMZ.i32 i10
PEU.PC1C EQ || BRU.JMP i30
CMU.CPVI.x32 i15 v0.0 // 2
CMU.CPVI.x32 i13 v0.1 // 4
CMU.CPVI.x32 i11 v0.2 // 6
CMU.CPVI.x32 i9  v0.3 // 8
nop 4

LSU0.ST.16 i15 i17 || IAU.SUB i10 i10 1 || SAU.ADD.I32 i17 i17 2     // 1
CMU.CMZ.i32 i10 || IAU.SHR.u32 i15 i15 16
PEU.PC1C GT || LSU0.ST.16 i15 i17 || IAU.SUB i10 i10 1 || SAU.ADD.I32 i17 i17 2 // 2
CMU.CMZ.i32 i10
PEU.PC1C GT || LSU0.ST.16 i13 i17 || IAU.SUB i10 i10 1 || SAU.ADD.I32 i17 i17 2 // 3
CMU.CMZ.i32 i10 || IAU.SHR.u32 i13 i13 16
PEU.PC1C GT || LSU0.ST.16 i13 i17 || IAU.SUB i10 i10 1 || SAU.ADD.I32 i17 i17 2 // 4
BRU.JMP i30 || CMU.CMZ.i32 i10
PEU.PC1C GT || LSU0.ST.16 i11 i17 || IAU.SUB i10 i10 1 || SAU.ADD.I32 i17 i17 2// 5
CMU.CMZ.i32 i10 || IAU.SHR.u32 i11 i11 16
PEU.PC1C GT || LSU0.ST.16 i11 i17 || IAU.SUB i10 i10 1 || SAU.ADD.I32 i17 i17 2// 6
CMU.CMZ.i32 i10
PEU.PC1C GT || LSU0.ST.16 i9 i17 || IAU.SUB i10 i10 1 || SAU.ADD.I32 i17 i17 2// 7
nop

.size mvcvConvert_u8_fp16_asm,.-mvcvConvert_u8_fp16_asm
.nowarnend
.end
