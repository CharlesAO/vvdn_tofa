
.type mvcvConvert12BppTo8Bpp_asm,@function
.version 00.50.00

/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.data .data.convert12BppTo8Bpp
.align 4
.code .text.convert12BppTo8Bpp
.lalign


//void convert12BppTo8Bpp(u8* dst, u8* src, u32 width)
//                             i18     i17      i16


mvcvConvert12BppTo8Bpp_asm:
LSU0.LDI.64.l v0 i17 || IAU.SHR.i32 i16 i16 0x02
LSU0.LDI.64.l v0 i17
LSU0.LDI.64.l v0 i17
LSU0.LDI.64.l v0 i17
LSU0.LDI.64.l v0 i17 || SAU.SUB.i32 i16 i16 0x01
LSU0.LDI.64.l v0 i17
LSU0.LDI.64.l v0 i17
LSU0.LDI.64.l v0 i17 || VAU.SHR.U16 v1 v0 4
LSU0.LDI.64.l v0 i17 || VAU.SHR.U16 v1 v0 4
LSU0.LDI.64.l v0 i17 || VAU.SHR.U16 v1 v0 4 || CMU.VDILV.x8 v3 v2 v1 v1

bru.rpi i16 || LSU0.LDI.64.l v0 i17 || VAU.SHR.U16 v1 v0 4 || CMU.VDILV.x8 v3 v2 v1 v1 || LSU1.ST.64.l v2 i18 || IAU.INCS i18 0x04
CMU.CPVI.x32 i5 v2.0
LSU1.ST.32 i5 i18

bru.jmp i30
nop 6
.size mvcvConvert12BppTo8Bpp_asm,.-mvcvConvert12BppTo8Bpp_asm
.end