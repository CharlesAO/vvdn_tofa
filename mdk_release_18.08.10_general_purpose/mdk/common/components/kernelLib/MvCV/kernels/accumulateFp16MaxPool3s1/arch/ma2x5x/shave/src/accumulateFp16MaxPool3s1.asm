/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvAccumulateFp16MaxPool3s1_asm,@function
.version 00.51.04

.data .data.AccumulateFp16MaxPool3s1
.align 4

.code .text.AccumulateFp16MaxPool3s1
//void mvcvAccumulateFp16MaxPool3s1 (half** dst, half** input, u32 width)//
//                                        i18           i17        i16
.lalign
mvcvAccumulateFp16MaxPool3s1_asm:
.nowarn
lsu0.ld.32 i18 i18
lsu0.ld.32 i17 i17
nop 6
iau.shr.u32 i4 i16 4  || lsu0.ldil i0 AccumulateFp16MaxPool3s1_loop || lsu1.ldih i0 AccumulateFp16MaxPool3s1_loop || cmu.cpii i10 i18
iau.shl i6 i4 4
iau.sub i5 i16 i6 


lsu0.ldo.64.l v0 i17 0 || lsu1.ldo.64.h v0 i17 8 
lsu0.ldo.64.l v1 i17 16 || lsu1.ldo.64.l v3 i10 16 || iau.add i17 i17 16
lsu0.ldo.64.l v2 i10 0 || lsu1.ldo.64.h v2 i10 8 || iau.add i10 i10 16
lsu0.ldo.64.l v13 i17 0 || lsu1.ldo.64.h v13 i17 8 
lsu0.ldo.64.l v14 i17 16 || lsu1.ldo.64.l v15 i10 16 || iau.add i17 i17 16 || cmu.cmz.i32 i4
peu.pc1c eq || bru.bra AccumulateFp16MaxPool3s1_compensate
lsu0.ldo.64.l v16 i10 0 || lsu1.ldo.64.h v16 i10 8 || iau.add i10 i10 16
nop 5
cmu.alignvec v4 v0 v1 2




cmu.alignvec v5 v0 v1 4                                 || lsu0.ldo.64.l v0 i17 0 || lsu1.ldo.64.h v0 i17 8 || bru.rpl i0 i4
cmu.alignvec v6 v2 v3 2                                 || lsu0.ldo.64.l v1 i17 16 || lsu1.ldo.64.l v3 i10 16 || iau.add i17 i17 16
cmu.alignvec v7 v2 v3 4    || vau.add.f16  v8 v0 v2     || lsu0.ldo.64.l v2 i10 0 || lsu1.ldo.64.h v2 i10 8 || iau.add i10 i10 16
cmu.alignvec v17 v13 v14 2 || vau.add.f16  v9 v4 v6     || lsu0.ldo.64.l v13 i17 0 || lsu1.ldo.64.h v13 i17 8 
cmu.alignvec v18 v13 v14 4 || vau.add.f16 v10 v5 v7     || lsu0.ldo.64.l v14 i17 16 || lsu1.ldo.64.l v15 i10 16 || iau.add i17 i17 16
cmu.alignvec v19 v16 v15 2 || vau.add.f16  v21 v13 v16  || lsu0.ldo.64.l v16 i10 0 || lsu1.ldo.64.h v16 i10 8 || iau.add i10 i10 16
cmu.alignvec v20 v16 v15 4 || vau.add.f16  v22 v17 v19
                              vau.add.f16 v23 v18 v20
							  AccumulateFp16MaxPool3s1_loop:
nop 
cmu.max.f16 v11 v8 v9
cmu.max.f16 v22 v21 v22
cmu.max.f16 v12 v11 v10
cmu.max.f16 v23 v22 v23

lsu0.sto.64.l v12 i18 0  || lsu1.sto.64.h v12 i18 8 || iau.add i18 i18 16
lsu0.sto.64.l v23 i18 0  || lsu1.sto.64.h v23 i18 8 || iau.add i18 i18 16 || cmu.alignvec v4 v0 v1 2


AccumulateFp16MaxPool3s1_compensate:
cmu.cmz.i32 i5
peu.pc1c eq || bru.bra AccumulateFp16MaxPool3s1_exit


cmu.alignvec v4 v0 v1 2
cmu.alignvec v5 v0 v1 4
cmu.alignvec v6 v2 v3 2
cmu.alignvec v7 v2 v3 4

vau.add.f16  v8 v0 v2
vau.add.f16  v9 v4 v6
vau.add.f16 v10 v5 v7
nop 
cmu.max.f16 v11 v8 v9
nop
cmu.max.f16 v12 v11 v10
nop 
lsu0.sto.64.l v12 i18 0  || lsu1.sto.64.h v12 i18 8 || iau.add i18 i18 16


AccumulateFp16MaxPool3s1_exit:


BRU.jmp i30
nop 6
.nowarnend
.size mvcvAccumulateFp16MaxPool3s1_asm,.-mvcvAccumulateFp16MaxPool3s1_asm
.end
