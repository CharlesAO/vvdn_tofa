/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvAccumulateFp16MaxPool2s2_asm,@function
.version 00.51.04

.data .data.AccumulateFp16MaxPool2s2
.align 4

.code .text.AccumulateFp16MaxPool2s2
//void mvcvAccumulateFp16MaxPool2s2 (half** dst, half** input, u32 width)//
//                                        i18           i17        i16
.lalign
mvcvAccumulateFp16MaxPool2s2_asm:
.nowarn
lsu0.ld.32 i18 i18
lsu0.ld.32 i17 i17
nop 6
iau.shr.u32 i4 i16 4  || lsu0.ldil i0 AccumulateFp16MaxPool2s2_loop || lsu1.ldih i0 AccumulateFp16MaxPool2s2_loop || cmu.cpii i10 i18
iau.shl i6 i4 4
iau.sub i5 i16 i6 


lsu0.ldo.64.l v0 i17 0 || lsu1.ldo.64.h v0 i17 8 
lsu0.ldo.64.l v1 i17 16 || lsu1.ldo.64.l v3 i10 16 || iau.add i17 i17 16
lsu0.ldo.64.l v2 i10 0 || lsu1.ldo.64.h v2 i10 8 || iau.add i10 i10 16  
lsu0.ldo.64.l v11 i17 0 || lsu1.ldo.64.h v11 i17 8 
lsu0.ldo.64.l v12 i17 16 || lsu1.ldo.64.l v13 i10 16 || iau.add i17 i17 16 || cmu.cmz.i32 i4
peu.pc1c eq || bru.bra AccumulateFp16MaxPool2s2_compensate
lsu0.ldo.64.l v14 i10 0 || lsu1.ldo.64.h v14 i10 8 || iau.add i10 i10 16  
nop 3
cmu.alignvec v4 v0 v1 2
cmu.alignvec v5 v2 v3 2

cmu.alignvec v15 v11 v12 2 || vau.add.f16  v6 v0 v2  || lsu0.ldo.64.l v0 i17 0 || lsu1.ldo.64.h v0 i17 8 || bru.rpl i0 i4
cmu.alignvec v16 v14 v13 2 || vau.add.f16  v7 v4 v5  || lsu0.ldo.64.l v1 i17 16 || lsu1.ldo.64.l v3 i10 16 || iau.add i17 i17 16
                           vau.add.f16  v17 v11 v14  || lsu0.ldo.64.l v2 i10 0 || lsu1.ldo.64.h v2 i10 8 || iau.add i10 i10 16  
                           vau.add.f16  v18 v15 v16  || lsu0.ldo.64.l v11 i17 0 || lsu1.ldo.64.h v11 i17 8 
                                                        lsu0.ldo.64.l v12 i17 16 || lsu1.ldo.64.l v13 i10 16 || iau.add i17 i17 16
AccumulateFp16MaxPool2s2_loop:
cmu.max.f16 v8 v6 v7                                 || lsu0.ldo.64.l v14 i10 0 || lsu1.ldo.64.h v14 i10 8 || iau.add i10 i10 16  
cmu.max.f16 v19 v17 v18
nop
cmu.vdilv.x16 v10 v9 v8 v8
cmu.vdilv.x16 v21 v20 v19 v19
lsu0.sto.64.l v9 i18 0  || lsu1.sto.64.l v20 i18 8  || iau.add i18 i18 16 || cmu.alignvec v4 v0 v1 2
cmu.alignvec v5 v2 v3 2



AccumulateFp16MaxPool2s2_compensate:
cmu.cmz.i32 i5
peu.pc1c eq || bru.bra AccumulateFp16MaxPool2s2_exit
cmu.alignvec v4 v0 v1 2
cmu.alignvec v5 v2 v3 2
vau.add.f16  v6 v0 v2
vau.add.f16  v7 v4 v5
nop 2
cmu.max.f16 v8 v6 v7
nop
cmu.vdilv.x16 v10 v9 v8 v8
lsu0.sto.64.l v9 i18 0  
AccumulateFp16MaxPool2s2_exit:



BRU.jmp i30
nop 6
.nowarnend
.size mvcvAccumulateFp16MaxPool2s2_asm,.-mvcvAccumulateFp16MaxPool2s2_asm
.end
