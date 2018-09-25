/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvConvert_fp16_u8_asm,@function
.version 00.51.04

.data .data.convert_fp16_u8
.align 4

.code .text.convert_fp16_u8
//mvcvConvert_fp16_u8(half* in, u8* out, u32 width)
.nowarn
.align 16
mvcvConvert_fp16_u8_asm:
IAU.SHR.u32 i14 i16 6
    || LSU1.LDIL i12 mvcvConvert_fp16_u8_end_loop
    || LSU0.LDIH i12 mvcvConvert_fp16_u8_end_loop
CMU.CMZ.i32 i14
PEU.PC1C EQ || BRU.BRA mvcvConvert_fp16_u8_compensation_64
LSU1.LDIL i0 0x3F || LSU0.LDIH i0 0x0  // ==> mask = 63 (0x3F)
IAU.AND i10 i16 i0
LSU1.LDIL i1 0x08   || LSU0.LDIH i1 0x0  // ==> i1 = 8
LSU1.LDIL i2 0x5bf8 || LSU0.LDIH i2 0x0  // ==> i2 = 0x00005bf8, (f16)255.0
CMU.CPIVR.x16 v8 i2
nop
//bru.rpim 0
mvcvConvert_fp16_u8_start_loop:
BRU.RPL i12 i14
LSU1.LD.64.l  v0 i18 || LSU0.LDO.64.h v0  i18 8 || IAU.ADD  i18  i18 16
    || SAU.ADD.i32 i15 i17 8
LSU1.LD.64.l v1  i18 || LSU0.LDO.64.h v1  i18 8 || IAU.ADD  i18  i18 16
LSU1.LD.64.l v2  i18 || LSU0.LDO.64.h v2  i18 8 || IAU.ADD  i18  i18 16
    || SAU.ADD.i32 i13 i15 8
LSU1.LD.64.l v3  i18 || LSU0.LDO.64.h v3  i18 8 || IAU.ADD  i18  i18 16
LSU1.LD.64.l v4  i18 || LSU0.LDO.64.h v4  i18 8 || IAU.ADD  i18  i18 16
    || SAU.ADD.i32 i11 i13 8
LSU1.LD.64.l v5  i18 || LSU0.LDO.64.h v5  i18 8 || IAU.ADD  i18  i18 16
LSU1.LD.64.l v6  i18 || LSU0.LDO.64.h v6  i18 8 || IAU.ADD  i18  i18 16
    || SAU.ADD.i32 i9 i11 8
LSU1.LD.64.l v7  i18 || LSU0.LDO.64.h v7  i18 8 || IAU.ADD  i18  i18 16

mvcvConvert_fp16_u8_end_loop:
LSU1.ST.128.F16.U8 v0 i17 || LSU0.ST.128.F16.U8 v1 i15
    || SAU.ADD.i32 i7 i9 8
nop
LSU1.ST.128.F16.U8 v2 i13 || LSU0.ST.128.F16.U8 v3 i11
    || SAU.ADD.i32 i5 i7 8
nop
LSU1.ST.128.F16.U8 v4 i9 || LSU0.ST.128.F16.U8 v5 i7
    || SAU.ADD.i32 i3 i5 8
nop
LSU1.ST.128.F16.U8 v6 i5 || LSU0.ST.128.F16.U8 v7 i3 || IAU.ADD i17 i3 8

IAU.AND i10 i16 i0
PEU.PC1I EQ || BRU.JMP i30
nop 6

mvcvConvert_fp16_u8_compensation_64:
CMU.CMII.u32 i10 i1
PEU.PC1C LT
    || BRU.BRA mvcvConvert_fp16_u8_compensation_8
LSU1.LD.64.l  v0 i18 || LSU0.LDO.64.h v0  i18 8 || IAU.ADD i18 i18 16
nop 5
BRU.BRA mvcvConvert_fp16_u8_compensation_64
LSU1.ST.128.F16.U8 v0 i17 || IAU.ADD i17 i17 8
IAU.SUB i10 i10 8
nop 4

mvcvConvert_fp16_u8_compensation_8:

CMU.CMZ.i32 i10

CMU.CLAMP0.f16 v0 v0 v8
PEU.PC1C EQ || BRU.JMP i30
CMU.CPVV.f16.i16s v0 v0
nop
CMU.CPVV.u16.u8s v0 v0
nop
CMU.CPVI.x32 i4 v0.0
CMU.CPVI.x32 i6 v0.1


LSU0.ST.8 i4 i17 || IAU.SUB i10 i10 1 || SAU.ADD.I32 i17 i17 1     // 1
CMU.CMZ.i32 i10 || IAU.SHR.u32 i4 i4 8
PEU.PC1C GT || LSU0.ST.8 i4 i17 || IAU.SUB i10 i10 1 || SAU.ADD.I32 i17 i17 1 // 2
CMU.CMZ.i32 i10 || IAU.SHR.u32 i4 i4 8
PEU.PC1C GT || LSU0.ST.8 i4 i17 || IAU.SUB i10 i10 1 || SAU.ADD.I32 i17 i17 1 // 3
CMU.CMZ.i32 i10 || IAU.SHR.u32 i4 i4 8
PEU.PC1C GT || LSU0.ST.8 i4 i17 || IAU.SUB i10 i10 1 || SAU.ADD.I32 i17 i17 1 // 4
BRU.JMP i30  || CMU.CMZ.i32 i10
PEU.PC1C GT || LSU0.ST.8 i6 i17 || IAU.SUB i10 i10 1 || SAU.ADD.I32 i17 i17 1// 5

CMU.CMZ.i32 i10 || IAU.SHR.u32 i6 i6 8
PEU.PC1C GT || LSU0.ST.8 i6 i17 || IAU.SUB i10 i10 1 || SAU.ADD.I32 i17 i17 1// 6
CMU.CMZ.i32 i10 || IAU.SHR.u32 i6 i6 8
PEU.PC1C GT || LSU0.ST.8 i6 i17 || IAU.SUB i10 i10 1 || SAU.ADD.I32 i17 i17 1// 7

nop

.size mvcvConvert_fp16_u8_asm,.-mvcvConvert_fp16_u8_asm
.nowarnend
.end
