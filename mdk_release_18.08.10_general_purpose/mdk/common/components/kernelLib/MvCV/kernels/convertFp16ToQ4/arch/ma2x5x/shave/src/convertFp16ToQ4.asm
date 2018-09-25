/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvConvertFp16ToQ4_asm,@function
.version 00.51.04

.data .data.convertFp16ToQ4
.align 4

.code .text.convertFp16ToQ4
//mvcvConvertFp16ToQ4(half* in, u8* out, u32 width)
//                      i18       i17       i16

.align 16
mvcvConvertFp16ToQ4_asm:
.nowarn
CMU.CPTI    i14 P_CFG
CMU.CPTI    i10 P_CFG //make a copy for state register
lsu0.ldil i13, 0x0002     ||  lsu1.ldih i13, 0x0000 //set rounding mode to + infinite
IAU.OR i14 i14 i13
CMU.CPIT   P_CFG i14 //set rounding mode

lsu0.ldil i0 16
cmu.cpii.i16.f16 i0 i0
nop
lsu0.ldil i1 .LmvcvConvertFp16ToQ4_loop || lsu1.ldih i1 .LmvcvConvertFp16ToQ4_loop 
cmu.cpivr.x16 v0 i0
iau.shr.u32 i15 i16 5
iau.shl i12 i15 5 
iau.sub i11 i16 i12

cmu.cmz.i32 i15
peu.pc1c eq || bru.bra .LmvcvConvertFp16ToQ4_compensate
nop 6

lsu0.ldo.64.l v1 i18 0 || lsu1.ldo.64.h v1 i18 8 || iau.add i18 i18 16
lsu0.ldo.64.l v5 i18 0 || lsu1.ldo.64.h v5 i18 8 || iau.add i18 i18 16
lsu0.ldo.64.l v9 i18 0 || lsu1.ldo.64.h v9 i18 8 || iau.add i18 i18 16
lsu0.ldo.64.l v13 i18 0 || lsu1.ldo.64.h v13 i18 8 || iau.add i18 i18 16
nop 3
vau.mul.f16 v2 v1 v0     || lsu0.ldo.64.l v1 i18 0 || lsu1.ldo.64.h v1 i18 8 || iau.add i18 i18 16
vau.mul.f16 v6 v5 v0     || lsu0.ldo.64.l v5 i18 0 || lsu1.ldo.64.h v5 i18 8 || iau.add i18 i18 16
vau.mul.f16 v10 v9 v0    || lsu0.ldo.64.l v9 i18 0 || lsu1.ldo.64.h v9 i18 8 || iau.add i18 i18 16
vau.mul.f16 v14 v13 v0   || lsu0.ldo.64.l v13 i18 0 || lsu1.ldo.64.h v13 i18 8 || iau.add i18 i18 16
vau.add.f16 v3 v2 0.5
vau.add.f16 v7 v6 0.5
vau.add.f16 v11 v10 0.5
vau.add.f16 v15 v14 0.5


cmu.cpvv.f16.i16s v4 v3   || vau.mul.f16 v2 v1 v0     || lsu0.ldo.64.l v1 i18 0 || lsu1.ldo.64.h v1 i18 8 || iau.add i18 i18 16 || bru.rpl i1 i15
.LmvcvConvertFp16ToQ4_loop:
cmu.cpvv.f16.i16s v8 v7   || vau.mul.f16 v6 v5 v0     || lsu0.ldo.64.l v5 i18 0 || lsu1.ldo.64.h v5 i18 8 || iau.add i18 i18 16
cmu.cpvv.f16.i16s v12 v11 || vau.mul.f16 v10 v9 v0    || lsu0.ldo.64.l v9 i18 0 || lsu1.ldo.64.h v9 i18 8 || iau.add i18 i18 16
cmu.cpvv.f16.i16s v16 v15 || vau.mul.f16 v14 v13 v0   || lsu0.ldo.64.l v13 i18 0 || lsu1.ldo.64.h v13 i18 8 || iau.add i18 i18 16

lsu0.sto.64.l v4 i17 0 || lsu1.sto.64.h v4 i17 8 || iau.add i17 i17 16 || vau.add.f16 v3 v2 0.5
lsu0.sto.64.l v8 i17 0 || lsu1.sto.64.h v8 i17 8 || iau.add i17 i17 16 || vau.add.f16 v7 v6 0.5
lsu0.sto.64.l v12 i17 0 || lsu1.sto.64.h v12 i17 8 || iau.add i17 i17 16 || vau.add.f16 v11 v10 0.5
lsu0.sto.64.l v16 i17 0 || lsu1.sto.64.h v16 i17 8 || iau.add i17 i17 16 || vau.add.f16 v15 v14 0.5

iau.sub i18 i18 16
iau.sub i18 i18 16
iau.sub i18 i18 16
iau.sub i18 i18 16
iau.sub i18 i18 16
iau.sub i18 i18 16
iau.sub i18 i18 16
iau.sub i18 i18 16
.LmvcvConvertFp16ToQ4_compensate:

cmu.cmz.i32 i11
peu.pc1c eq || bru.bra .LmvcvConvertFp16ToQ4_exit
lsu0.ldo.64.l v1 i18 0 || lsu1.ldo.64.h v1 i18 8 || iau.add i18 i18 16
lsu0.ldo.64.l v5 i18 0 || lsu1.ldo.64.h v5 i18 8 || iau.add i18 i18 16
lsu0.ldo.64.l v9 i18 0 || lsu1.ldo.64.h v9 i18 8 || iau.add i18 i18 16
lsu0.ldo.64.l v13 i18 0 || lsu1.ldo.64.h v13 i18 8 || iau.add i18 i18 16
nop 3
vau.mul.f16 v2 v1 v0
vau.mul.f16 v6 v5 v0
vau.mul.f16 v10 v9 v0
vau.mul.f16 v14 v13 v0
vau.add.f16 v3 v2 0.5
vau.add.f16 v7 v6 0.5
vau.add.f16 v11 v10 0.5
vau.add.f16 v15 v14 0.5

cmu.cpvv.f16.i16s v4 v3
cmu.cpvv.f16.i16s v8 v7
cmu.cpvv.f16.i16s v12 v11
cmu.cpvv.f16.i16s v16 v15
lsu0.sto.64.l v4 i17 0 || lsu1.sto.64.h v4 i17 8                     || IAU.INCS i11 -8
peu.pc1i gt || lsu0.sto.64.l v8 i17 16 || lsu1.sto.64.h v8 i17 24    || IAU.INCS i11 -8
peu.pc1i gt || lsu0.sto.64.l v12 i17 32 || lsu1.sto.64.h v12 i17 40  || IAU.INCS i11 -8
peu.pc1i gt || lsu0.sto.64.l v16 i17 48 || lsu1.sto.64.h v16 i17 56  || IAU.INCS i11 -8


.LmvcvConvertFp16ToQ4_exit:
CMU.CPIT   P_CFG i10 //restore state register to value that it had before
bru.jmp i30
nop 6
.nowarnend
.size mvcvConvertFp16ToQ4_asm,.-mvcvConvertFp16ToQ4_asm
.end
