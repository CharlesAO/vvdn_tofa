 /// @file
 /// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
 ///            For License Warranty see: common/license.txt//
 ///
 /// @brief
 ///

.type mvcvAddV2Fp16_asm,@function
.version 00.51.04

.data .data.addV2Fp16

.code .text.addV2Fp16

.lalign
//void mvcvAddV2Fp16 (half** dst, half** input, u32 width)//
  //                     i18           i17        i16

mvcvAddV2Fp16_asm:
.nowarn
lsu0.ld.32 i18 i18
lsu0.ldi.32 i15 i17
lsu0.ld.32 i10 i17
iau.shr.u32 i4 i16 5  || lsu0.ldil i0 addV2Fp16_loop || lsu1.ldih i0 addV2Fp16_loop
iau.shl i6 i4 5       || lsu0.ldil i7 addV2Fp16_compensate || lsu1.ldih i7 addV2Fp16_compensate
iau.sub i5 i16 i6     || lsu0.ldil i9 0x8 || lsu1.ldih i9 0x0
nop 3
nop


lsu0.ldo.64.l v1 i15 0 || lsu1.ldo.64.h v1 i15 8 
lsu0.ldo.64.l v4 i15 16 || lsu1.ldo.64.h v4 i15 24 
lsu0.ldo.64.l v7 i15 32 || lsu1.ldo.64.h v7 i15 40 
lsu0.ldo.64.l v10 i15 48 || lsu1.ldo.64.h v10 i15 56 || iau.add i15 i15 64
lsu0.ldo.64.l v0 i10 0  || lsu1.ldo.64.h v0 i10 8 || cmu.cmz.i32 i4
peu.pc1c eq || bru.bra addV2Fp16_compensate
lsu0.ldo.64.l v3 i10 16 || lsu1.ldo.64.h v3 i10 24 
lsu0.ldo.64.l v6 i10 32 || lsu1.ldo.64.h v6 i10 40 
lsu0.ldo.64.l v9 i10 48 || lsu1.ldo.64.h v9 i10 56  || iau.add i10 i10 64
nop 3

vau.add.f16 v2 v1 v0   || lsu0.ldo.64.l v1 i15 0 || lsu1.ldo.64.h v1 i15 8 || bru.rpl i0 i4
vau.add.f16 v5 v3 v4   || lsu0.ldo.64.l v4 i15 16 || lsu1.ldo.64.h v4 i15 24 
vau.add.f16 v8 v6 v7   || lsu0.ldo.64.l v7 i15 32 || lsu1.ldo.64.h v7 i15 40 
vau.add.f16 v11 v9 v10 || lsu0.ldo.64.l v10 i15 48 || lsu1.ldo.64.h v10 i15 56 || iau.add i15 i15 64
                          lsu0.ldo.64.l v0 i10 0  || lsu1.ldo.64.h v0 i10 8 
                          addV2Fp16_loop:
                          lsu0.ldo.64.l v3 i10 16 || lsu1.ldo.64.h v3 i10 24 
                          lsu0.ldo.64.l v6 i10 32 || lsu1.ldo.64.h v6 i10 40 
                          lsu0.ldo.64.l v9 i10 48 || lsu1.ldo.64.h v9 i10 56  || iau.add i10 i10 64
lsu0.sto.64.l v2 i18 0  || lsu1.sto.64.h v2 i18 8 
lsu0.sto.64.l v5 i18 16  || lsu1.sto.64.h v5 i18 24 
lsu0.sto.64.l v8 i18 32  || lsu1.sto.64.h v8 i18 40 
lsu0.sto.64.l v11 i18 48  || lsu1.sto.64.h v11 i18 56 || iau.add i18 i18 64

addV2Fp16_compensate:

cmu.cmz.i32 i5
peu.pc1c eq || bru.bra addV2Fp16_exit
vau.add.f16 v2 v1 v0
vau.add.f16 v5 v3 v4
vau.add.f16 v8 v6 v7
vau.add.f16 v11 v9 v10
nop 2
lsu0.sto.64.l v2 i18 0  || lsu1.sto.64.h v2 i18 8                   || IAU.INCS i5 -8
peu.pc1i gt || lsu0.sto.64.l v5 i18 16  || lsu1.sto.64.h v5 i18 24    || IAU.INCS i5 -8
peu.pc1i gt || lsu0.sto.64.l v8 i18 32  || lsu1.sto.64.h v8 i18 40    || IAU.INCS i5 -8
peu.pc1i gt || lsu0.sto.64.l v11 i18 48  || lsu1.sto.64.h v11 i18 56  || IAU.INCS i5 -8

addV2Fp16_exit:

BRU.jmp i30
nop 6
.nowarnend
.size mvcvAddV2Fp16_asm,.-mvcvAddV2Fp16_asm
.end
