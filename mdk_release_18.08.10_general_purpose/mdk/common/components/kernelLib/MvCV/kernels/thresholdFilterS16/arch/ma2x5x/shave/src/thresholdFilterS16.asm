///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvThresholdFilterS16_asm,@function
.version 0.50.00



.code .text.mvcvThresholdFilterS16
.lalign
mvcvThresholdFilterS16_asm:

//void mvcvThresholdFilterS16(s16* line, u32 threshold, u32 width, u32* indexList, u32* count)
//                                  i18        i17           i16          i15            i14


lsu0.ld.128.i16.i32 v2  i18  || lsu1.ldil i10 ___thresholdFilterS16Loop       || iau.incs i18 0x8      || sau.xor i0 i0 i0
lsu0.ld.128.i16.i32 v6  i18  || lsu1.ldih i10 ___thresholdFilterS16Loop       || iau.incs i18 0x8
vau.xor v0 v0 v0      || cmu.cpiv.x32 v5.0 i0    || iau.add i11 i0 4 || lsu0.cp i13 i15
iau.add i1 i0 1       || cmu.cpivr.x32 v3 i17    || vau.xor v1 v1 v1
iau.add i2 i0 2       || sau.add.i32 i3 i0 3     || cmu.cpivr.x32 v0 i11
sau.sub.i32 i16 i16 8 || cmu.cpiv.x32 v5.1 i1
cmu.cpiv.x32 v5.2 i2  || vau.xor v8 v8 v8        || lsu0.ld.128.i16.i32 v2  i18 || iau.incs i18 0x8
lsu0.ld.128.i16.i32 v6  i18  || iau.incs i18 0x8 || vau.mul.i32 v4 v2 v2
vau.mul.i32 v7 v6 v6  || cmu.cpiv.x32 v5.3 i3
iau.shr.u32 i12 i16 3

cmu.cmvv.i32 v4 v3    || vau.add.i32 v5 v5 v0 || lsu0.cp i4 v4.0
peu.pvv32 gte         || vau.add.i32 v1 v1 v5
cmu.cmvv.i32 v7 v3
peu.pvv32 gte         || vau.add.i32 v8 v8 v5
cmu.cmii.i32 i4 i17   || lsu0.cp i5 v1.1       || lsu1.cp i6 v1.2 || vau.mul.i32 v4 v2 v2
peu.pc1c gte          || lsu0.sti.32 i0 i15
cmu.cmii.i32 i5 i0    || lsu0.cp i7 v1.3       || vau.add.i32 v5 v5 v0
peu.pccx.neq 0x11     || lsu0.sti.32 i5 i15    || cmu.cmii.i32 i6 i0   || vau.mul.i32 v7 v6 v6
peu.pccx.neq 0x11     || lsu0.sti.32 i6 i15    || cmu.cmii.i32 i7 i0   || lsu1.cp i1 v8.0
peu.pccx.neq 0x11     || lsu0.sti.32 i7 i15    || cmu.cmii.i32 i1 i0   || lsu1.cp i2 v8.1
peu.pccx.neq 0x11     || lsu0.sti.32 i1 i15    || cmu.cmii.i32 i2 i0   || lsu1.cp i3 v8.2
peu.pccx.neq 0x11     || lsu0.sti.32 i2 i15    || cmu.cmii.i32 i3 i0   || lsu1.cp i9 v8.3
peu.pccx.neq 0x11     || lsu0.sti.32 i3 i15    || cmu.cmii.i32 i9 i0   || vau.xor v1 v1 v1
peu.pccx.neq 0x11     || lsu0.sti.32 i9 i15    || cmu.cmvv.i32 v4 v3


peu.pvv32 gte         || vau.add.i32 v1 v1 v5 || bru.rpl i10 i12
vau.add.i32 v5 v5 v0  || cmu.cpivr.x32 v8 i0  || lsu0.ld.128.i16.i32 v2 i18 || iau.incs i18 0x8
cmu.cmvv.i32 v7 v3    || lsu1.cp i4 v1.0      || lsu0.ld.128.i16.i32 v6 i18 || iau.incs i18 0x8
peu.pvv32 gte         || vau.add.i32 v8 v8 v5
cmu.cmii.i32 i4 i0    || lsu0.cp i5 v1.1      || lsu1.cp i6 v1.2 || vau.add.i32 v5 v5 v0
peu.pccx.neq 0x10     || lsu0.sti.32 i4 i15   || cmu.cmii.i32 i5 i0   || lsu1.cp i7 v1.3
___thresholdFilterS16Loop:
.nowarn 10
peu.pccx.neq 0x10     || lsu0.sti.32 i5 i15   || cmu.cmii.i32 i6 i0   || lsu1.cp i1 v8.0
.nowarnend
peu.pccx.neq 0x10     || lsu0.sti.32 i6 i15   || cmu.cmii.i32 i7 i0   || lsu1.cp i2 v8.1
peu.pccx.neq 0x10     || lsu0.sti.32 i7 i15   || cmu.cmii.i32 i1 i0   || lsu1.cp i3 v8.2
peu.pccx.neq 0x10     || lsu0.sti.32 i1 i15   || cmu.cmii.i32 i2 i0   || vau.mul.i32 v4 v2 v2
peu.pccx.neq 0x10     || lsu0.sti.32 i2 i15   || cmu.cmii.i32 i3 i0   || lsu1.cp i9 v8.3
peu.pccx.neq 0x10     || lsu0.sti.32 i3 i15   || cmu.cmii.i32 i9 i0   || vau.mul.i32 v7 v6 v6
peu.pccx.neq 0x10     || lsu0.sti.32 i9 i15   || cmu.cmvv.i32 v4 v3   || vau.xor v1 v1 v1


bru.jmp i30
iau.sub i13 i15 i13
iau.shr.u32 i13 i13 2
lsu0.st.32 i13 i14
nop 3
.size mvcvThresholdFilterS16_asm,.-mvcvThresholdFilterS16_asm
.end
