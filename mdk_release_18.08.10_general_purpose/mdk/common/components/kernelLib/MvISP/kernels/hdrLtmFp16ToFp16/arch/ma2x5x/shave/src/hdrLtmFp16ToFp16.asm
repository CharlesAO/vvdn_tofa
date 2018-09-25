/////
///// @file      
///// @copyright All code copyright Movidius Ltd 2012, all rights reserved
/////            For License Warranty see: common/license.txt
/////
///// @brief     Extract Chroma differences CrCgCb from (debayerd) RGB image
/////
//-------------------------------------------------------------------------------
.type mvispHdrLtmFp16ToFp16_asm,@function
.version 00.51.05

.data .data.mvispHdrLtmFp16ToFp16_asm
.align 4

.code .text.mvispHdrLtmFp16ToFp16_asm  
//void mvispHdrLtmFp16ToFp16(half* inLumaFusion, half* inGaussUp, half* output, u32 width, HdrLtmParam* params)
//                                  i18                i17            i16           i15            i14 
//########################################
.lalign 
mvispHdrLtmFp16ToFp16_asm:
lsu0.ldil i0 mvispHdrLtmFp16ToFp16_loop || lsu1.ldil i1 0x3c00
lsu1.ldih i0 mvispHdrLtmFp16ToFp16_loop || cmu.cpivr.x16 v19 i1 || lsu0.ldi.32 i1 i14  
vau.xor v20 v20 v20 || lsu0.ldil i4 0xaa3b || lsu1.ldih i4 0x3fb8 
iau.shr.u32 i15 i15 3 || lsu0.ldi.32 i2 i14 
lsu0.ldi.32 i3 i14 
lsu0.ldi.32 i5 i14 
nop 3
cmu.cpii.f32.f16 i1 i1
cmu.cpii.f32.f16 i2 i2
cmu.cpii.f32.f16 i3 i3
cmu.cpii.f32.f16 i4 i4
cmu.cpii.f32.f16 i5 i5
cmu.cpivr.x16 v1 i1 //thr1
cmu.cpivr.x16 v2 i2 //thr2
cmu.cpivr.x16 v3 i3 //thr3
cmu.cpivr.x16 v13 i4 //oneoverln2
cmu.cpivr.x16 v15 i5 //eps
lsu0.ldo.64.l v0 i18 0 || lsu1.ldo.64.h v0 i18 8 || iau.add i18 i18 16//inLumaFusion
lsu0.ldo.64.l v4 i17 0 || lsu1.ldo.64.h v4 i17 8 //inGaussUp//inLumaFusion[i] - inGaussUp[i]//
nop 6
vau.sub.f16 v5 v4 v0 //inGaussUp[i] - inLumaFusion[i]
nop 2
vau.abs.f16 v6 v5 //bli_fabs(inGaussUp[i] - inLumaFusion[i])
nop
cmu.min.f16 v7 v6 v1 //chmin
nop
vau.add.f16 v8 v0 v7 //+
nop 2
vau.sub.f16 v9 v0 v7 || cmu.cmz.f16 v5//-
nop 2
peu.pvl016 gte || lsu0.cp v4 v8
peu.pvl016 lt  || lsu0.cp v4 v9
vau.mul.f16 v10 v4 v2 || cmu.cpvv v21 v4//inGaussUp[i] * thr2
bru.rpl i0 i15

lsu0.sto.64.l v21 i17 0 || lsu1.sto.64.h v21 i17 8 || iau.add i17 i17 16//inGaussUp
lsu0.ldo.64.l v0 i18 0 || lsu1.ldo.64.h v0 i18 8 || iau.add i18 i18 16//inLumaFusion
vau.sub.f16 v11 v0 v4 || lsu0.ldo.64.l v4 i17 0 || lsu1.ldo.64.h v4 i17 8 //inGaussUp//inLumaFusion[i] - inGaussUp[i]//
nop 2
vau.add.f16 v12 v10 v11 //interAux
nop 2
vau.mul.f16 v14 v12 v13 //interAux * oneoverln2
nop 2
SAU.EXP2 i1 v14.0 || vau.sub.f16 v5 v4 v0 //inGaussUp[i] - inLumaFusion[i]
SAU.EXP2 i2 v14.1
SAU.EXP2 i3 v14.2
SAU.EXP2 i4 v14.3 || vau.abs.f16 v6 v5 //bli_fabs(inGaussUp[i] - inLumaFusion[i])
SAU.EXP2 i5 v14.4
SAU.EXP2 i6 v14.5 || cmu.cpiv.x16 v16.0  i1.l
SAU.EXP2 i7 v14.6 || cmu.cpiv.x16 v16.1  i2.l
SAU.EXP2 i8 v14.7 || cmu.cpiv.x16 v16.2  i3.l
cmu.cpiv.x16 v16.3  i4.l
cmu.cpiv.x16 v16.4  i5.l
cmu.cpiv.x16 v16.5  i6.l
cmu.cpiv.x16 v16.6  i7.l
cmu.cpiv.x16 v16.7  i8.l
vau.sub.f16 v17 v16 v15 || cmu.min.f16 v7 v6 v1 //chmin//eps
nop 
mvispHdrLtmFp16ToFp16_loop:
vau.add.f16 v8 v0 v7 //+
vau.mul.f16 v18 v17 v3 // *thr3
vau.sub.f16 v9 v0 v7 || cmu.cmz.f16 v5//-
peu.pvl016 gte || lsu0.cp v4 v8
CMU.CLAMPAB.f16 v18 v20 v19 
peu.pvl116 lt  || lsu1.cp v4 v9
lsu0.sto.64.l v18 i16 0 || lsu1.sto.64.h v18 i16 8 || iau.add i16 i16 16 || vau.mul.f16 v10 v4 v2 || cmu.cpvv v21 v4//inGaussUp[i] * thr2

bru.jmp i30
nop 6
.size mvispHdrLtmFp16ToFp16_asm,.-mvispHdrLtmFp16ToFp16_asm
.end

