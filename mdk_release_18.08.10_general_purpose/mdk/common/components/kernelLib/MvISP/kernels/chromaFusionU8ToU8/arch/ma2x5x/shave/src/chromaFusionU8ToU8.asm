///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvispChromaFusionU8ToU8_asm,@function
.version 00.51.04

.code .text.ChromaFusionU8ToU8

//u8* inCRshort, i18
//u8* inCGshort, i17
//u8* inCBshort, i16
//u8* inCRlong,  i15
//u8* inCGlong,  i14
//u8* inCBlong,  i13 
//half* inAlpha, i12 
//u8* outR,      i11
//u8* outG,      i10 **
//u8* outB,       i9**
//u32 width      i8**
mvispChromaFusionU8ToU8_asm:
lsu0.ldo.32 i10 i19 0 //outG
lsu0.ldo.32  i9 i19 4 //outB
lsu0.ldo.32  i8 i19 8 //width
nop 6
lsu0.ldil i0 0x3c00
iau.shr.u32 i8 i8 3
lsu0.ldil i1 mvispChromaFusionU8ToU8_loop || lsu1.ldih i1 mvispChromaFusionU8ToU8_loop 
cmu.cpivr.x16 v7 i0 // 1fp16

lsu0.ldo.64.l v6 i12 0 || lsu1.ldo.64.h v6 i12 8 || iau.add i12 i12 16
LSU0.LDi.128.u8.f16 v3 i15 //inCRlong,
LSU0.LDi.128.u8.f16 v4 i14 //inCGlong,
LSU0.LDi.128.u8.f16 v5 i13 //inCBlong,
LSU0.LDi.128.u8.f16 v0 i18 //inCRshort,
LSU0.LDi.128.u8.f16 v1 i17 //inCGshort,
LSU0.LDi.128.u8.f16 v2 i16 //inCBshort,
vau.sub.f16 v11 v7 v6
vau.mul.f16  v8 v3 v6 
vau.mul.f16  v9 v4 v6 

vau.mul.f16 v10 v5 v6    || lsu0.ldo.64.l v6 i12 0 || lsu1.ldo.64.h v6 i12 8 || iau.add i12 i12 16 || bru.rpl i1 i8
vau.mul.f16 v12 v0 v11   || LSU0.LDi.128.u8.f16 v3 i15 //inCRlong,
vau.mul.f16 v13 v1 v11   || LSU0.LDi.128.u8.f16 v4 i14 //inCGlong,
mvispChromaFusionU8ToU8_loop:
vau.mul.f16 v14 v2 v11   || LSU0.LDi.128.u8.f16 v5 i13 //inCBlong,
vau.add.f16 v15  v8 v12  || LSU0.LDi.128.u8.f16 v0 i18 //inCRshort,
vau.add.f16 v16  v9 v13  || LSU0.LDi.128.u8.f16 v1 i17 //inCGshort,
vau.add.f16 v17 v10 v14  || LSU0.LDi.128.u8.f16 v2 i16 //inCBshort,
LSU0.STi.128.f16.u8 v15 i11 || vau.sub.f16 v11 v7 v6
LSU0.STi.128.f16.u8 v16 i10 || vau.mul.f16  v8 v3 v6
LSU0.STi.128.f16.u8 v17  i9 || vau.mul.f16  v9 v4 v6

BRU.JMP i30
nop 6
.size mvispChromaFusionU8ToU8_asm,.-mvispChromaFusionU8ToU8_asm
.end
