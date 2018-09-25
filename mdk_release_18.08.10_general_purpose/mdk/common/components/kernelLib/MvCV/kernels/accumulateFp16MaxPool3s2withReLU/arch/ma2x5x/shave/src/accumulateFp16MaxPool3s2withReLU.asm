/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvAccumulateFp16MaxPool3s2withReLU_asm,@function
.version 00.51.04

.data .data.AccumulateFp16MaxPool3s2withReLU
.align 4

.code .text.AccumulateFp16MaxPool3s2withReLU
//void mvcvAccumulateFp16MaxPool3s2withReLU (half** dst, half** input, u32 width)//
//                                        i18           i17        i16
.lalign
mvcvAccumulateFp16MaxPool3s2withReLU_asm:
.nowarn 9,10,12
lsu0.ld.32 i18 i18 || lsu1.ldil i0 65504
lsu0.ld.32 i17 i17 || cmu.cpii.i32.f16 i1 i0 
nop
cmu.cpivr.x16 v23 i1
nop 6
//bru.rpim 0
iau.shr.u32 i4 i16 3  || lsu0.ldil i0 AccumulateFp16MaxPool3s2withReLU_loop || lsu1.ldih i0 AccumulateFp16MaxPool3s2withReLU_loop
//iau.shl i6 i4 5       || lsu0.ldil i7 AccumulateFp16MaxPool3s2withReLU_compensate || lsu1.ldih i7 AccumulateFp16MaxPool3s2withReLU_compensate

cmu.cpii i10 i18
lsu0.ldo.64.l v0 i17 0 || lsu1.ldo.64.h v0 i17 8
lsu0.ldo.64.l v1 i17 16 || lsu1.ldo.64.l v3 i10 16 || iau.add i17 i17 16
lsu0.ldo.64.l v2 i10 0 || lsu1.ldo.64.h v2 i10 8 || iau.add i10 i10 16  
nop 5
cmu.alignvec v4 v0 v1 2
cmu.alignvec v5 v0 v1 4                          || lsu0.ldo.64.l v0 i17 0 || lsu1.ldo.64.h v0 i17 8 || bru.rpl i0 i4
cmu.alignvec v6 v2 v3 2 || vau.add.f16  v8 v0 v2 || lsu0.ldo.64.l v1 i17 16 || lsu1.ldo.64.l v3 i10 16 || iau.add i17 i17 16
cmu.alignvec v7 v2 v3 4 || vau.add.f16  v9 v4 v6 || lsu0.ldo.64.l v2 i10 0 || lsu1.ldo.64.h v2 i10 8 || iau.add i10 i10 16  
vau.add.f16 v10 v5 v7

nop 
cmu.max.f16 v11 v8 v9
AccumulateFp16MaxPool3s2withReLU_loop:
nop
cmu.max.f16 v12 v11 v10
nop 
cmu.clamp0.f16 v15 v12 v23
nop
cmu.vdilv.x16 v13 v14 v15 v15
lsu0.sto.64.l v14 i18 0  || iau.add i18 i18 8 || cmu.alignvec v4 v0 v1 2


BRU.jmp i30
nop 6
.nowarnend
.size mvcvAccumulateFp16MaxPool3s2withReLU_asm,.-mvcvAccumulateFp16MaxPool3s2withReLU_asm
.end
