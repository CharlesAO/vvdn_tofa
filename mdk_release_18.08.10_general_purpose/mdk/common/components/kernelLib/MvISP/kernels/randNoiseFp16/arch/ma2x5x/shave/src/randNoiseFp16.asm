///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief  generate a random noise on a fp16, [0,1] interval image, in interval +-0.5*strength parameter
///

.type mvispRandNoiseFp16_asm,@function
.version 00.51.04

.data .data.randNoisefp16
.align 16
_seedListAsm:
        .short  48983, 45013, 19197, 10539, 13476,  3369, 31472, 22447
        .short  30400, 22649, 46471, 44385, 60118, 49067, 18879, 42139
        .short  23493, 18725, 21065, 19968, 12366, 25348, 52182, 63519
        .short  25325, 21617,   337, 20501, 18752, 29257,   457, 12295
        .short  27449,  9644, 50556, 31831, 40725, 53234, 49595, 64496
        .short  12042,  8516, 29204, 24711, 51586, 31526, 18924, 58697
        .short  23228, 25686, 44433, 44726, 58042, 22242, 50523, 41777
        .short  6150, 50968,  5916,   369,  2049,  4224, 55504, 58211 
        .short  46402, 45323, 62350, 41020, 54912, 17622, 55249, 30196
        .short  9648, 50460,  3153, 17172, 44300,  3994, 52286, 58207 
        .short  4872, 13075,  2892, 45101, 32394,  8084, 32543, 54467
        .short  8175, 25086, 16030, 36775,  7742, 29155, 41415, 32929
_mskBitAsm:                
        .short 0xA011, 0

.code .text.randNoisefp16
                       //i18            i17             i16                 i15
//void randNoiseU16(half *output, half *input, float noiseStrength, unsigned int width)
.lalign
mvispRandNoiseFp16_asm:
.nowarn
    cmu.cpti i1, P_SVID
    iau.shl i1, i1, 4           ||lsu1.ldil i0, _seedListAsm    ||lsu0.ldih i0, _seedListAsm
    iau.add i0, i0, i1
    LSU0.LDXV v0, i0 
    nop 5
    lsu1.ldil i2, 0xA011  
    vau.xor  v22, v22, v22      || cmu.cpivr.x16 v23, i2 // v23 = bit mask//// v22 = 0
    vau.and v8, v0, v23
    vau.shr.u16  v14, v0, 1     ||cmu.vilv.x16 v9, v10, v8, v22
    SAU.ONESX.x32 I3, V9        || cmu.cpii.f32.f16 i16, i16
    SAU.ONESX.x32 I4, V10       ||lsu0.ldil i8, 0x1c04          ||cmu.cpivr.x16 v20, i16 // v20 = strength// 0.003921509 (1/255) in fp16
    cmu.cpivr.x16 v21, i8 // v21 = 1/255
    CMU.CPIVD v11.0, I3         ||vau.mul.f16 v20, v20, v21
    cmu.cpvv.u8.u16 v11, v11
    vau.shl.x16  v12, v11, 15
    iau.shr.u32 i15, i15, 3
    vau.or v15, v12, v14
    vau.and v8, v15, v23        ||cmu.vdilv.x8 v16, v6, v15, v15
    vau.shr.u16  v14, v15, 1    ||cmu.vilv.x16 v9, v10, v8, v22
    SAU.ONESX.x32 I3, V9        || cmu.cpvv.i8.f16 v17, v16
    SAU.ONESX.x32 I4, V10
    vau.mul.f16 v18, v17, v20
    CMU.CPIVD v11.0, I3
    cmu.cpvv.u8.u16 v11, v11
    vau.shl.x16  v12, v11, 15   || lsu0.ldil i9, 0x3c00 // 1.0 in fp16
    cmu.cpivr.x16 v5, i9 // v5 = 1.0 
    vau.or v15, v12, v14
    vau.and v8, v15, v23        ||cmu.vdilv.x8 v16, v6, v15, v15
    vau.shr.u16  v14, v15, 1    ||cmu.vilv.x16 v9, v10, v8, v22
    SAU.ONESX.x32 I3, V9        || cmu.cpvv.i8.f16 v17, v16         ||vau.add.f16 v2, v18, v5
    SAU.ONESX.x32 I4, V10
    vau.mul.f16 v18, v17, v20
    CMU.CPIVD v11.0, I3         ||LSU0.LDXV v3  i17                 || iau.incs i17, 0x10
    cmu.cpvv.u8.u16 v11, v11
    vau.shl.x16  v12, v11, 15
    lsu1.ldil i12, end_loop_randNoisefp16        ||lsu0.ldih i12, end_loop_randNoisefp16
    vau.or v15, v12, v14
// start loop
.lalign
    bru.rpl i12, i15, end_ds_randNoisefp16  ||vau.and v8, v15, v23   ||cmu.vdilv.x8 v16, v6, v15, v15
    vau.shr.u16  v14, v15, 1    ||cmu.vilv.x16 v9, v10, v8, v22
    SAU.ONESX.x32 I3, V9        || cmu.cpvv.i8.f16 v17, v16        ||vau.add.f16 v2, v18, v5
end_loop_randNoisefp16:
    SAU.ONESX.x32 I4, V10       ||vau.mul.f16 v1, v2, v3 
    vau.mul.f16 v18, v17, v20
    CMU.CPIVD v11.0, I3         ||LSU0.LDXV v3  i17 || iau.incs i17, 0x10
    cmu.cpvv.u8.u16 v11, v11
    vau.shl.x16  v12, v11, 15   ||cmu.clamp0.f16 v4, v1, v5
    nop
    vau.or v15, v12, v14        || LSU0.STXV v4  i18 || iau.incs i18, 0x10
end_ds_randNoisefp16:     
// end loop 
 
 bru.jmp i30
 LSU0.STXV v15, i0 
 nop 5
 .nowarnend
.size mvispRandNoiseFp16_asm,.-mvispRandNoiseFp16_asm
.end
