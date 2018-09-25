///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvInterpolatePixelBilinearS16_asm,@function
.version 00.50.05

.data .rodata.InterpolatePixelBilinearS16_asm
.align 16

.code .text.InterpolatePixelBilinearS16_asm
.align 16


//float interpolatePixelBilinearS16(short* line1, short* line2, float x, float y)
//                                           i18          i17       i16     i15

mvcvInterpolatePixelBilinearS16_asm:
CMU.CPII.f32.i32s i0 i16  || SAU.FRAC.f32 i6 i16  || VAU.XOR v5 v5 v5
CMU.CPII.f32.i32s i1 i15  || SAU.FRAC.f32 i7 i15  || VAU.ADD.f32 v5 v5 1.0
IAU.SHL i0 i0 1           || CMU.CPIV.x32 v3.0 i6
CMU.CPIV.x32 v3.1, i7     || IAU.ADD i2 i18 i0
IAU.ADD i3 i17 i0         || VAU.SUB.f32 v4 v5 v3
LSU0.LD.32R v1 i3         || LSU1.LD.32R v0 i2
NOP 3
CMU.VILV.x32 v3 v4 v3 v4
CMU.VILV.x32 v7 v4 v3 v3
VAU.MUL.f32 v2 v3 v4      || LSU0.SWZMV4.WORD [1010] [UUUU]
CMU.ALIGNVEC v0 v0 v1 12
CMU.CPVV.i16.f32 v0 v0
NOP
VAU.MUL.f32 v6 v2 v0
BRU.JMP i30
NOP
SAU.SUMX.f32 i18 v6
NOP 4
.size mvcvInterpolatePixelBilinearS16_asm,.-mvcvInterpolatePixelBilinearS16_asm
.end
