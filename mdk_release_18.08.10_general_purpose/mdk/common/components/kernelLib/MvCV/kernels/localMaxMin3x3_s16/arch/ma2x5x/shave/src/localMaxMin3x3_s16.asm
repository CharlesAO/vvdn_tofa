///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvLocalMaxMin3x3_s16_asm,@function
.version 00.51.05
//-------------------------------------------------------------------------------

///// void mvcvLocalMaxMin3x3_s16( s16** inBuffer, u32 candidateLocationIn[], u32 countIn, u32 minLocationList[], u32 maxLocationList[], u32* minCount, u32* maxCount)
/////                                   i18                i17                     i16                 i15                  i14                  i13              i12


.code .text.mvcvLocalMaxMin3x3_s16_asm
.align 16

.lalign
mvcvLocalMaxMin3x3_s16_asm:
.nowarn
cmu.cpiv.x32 v23.0 i20 || iau.xor i9 i9 i9 || LSU0.LDO.32 i1 i18 0 || LSU1.LDO.32 i2 i18 4
cmu.cpiv.x32 v23.1 i21 || iau.xor i11 i11 i11 || LSU0.LDO.32 i3 i18 8
cmu.cpiv.x32 v23.2 i22 || LSU0.LDIL i0 localMaxMin3x3_s16_exit || LSU1.LDIH i0 localMaxMin3x3_s16_exit
cmu.cmz.i32 i16 || LSU0.LDIL i10 localMaxMin3x3_s16_loop || LSU1.LDIH i10 localMaxMin3x3_s16_loop
peu.pc1c eq || bru.jmp i0
lsu0.ldi.32 i0 i17

nop 6
iau.SHL i4 i0 1  || lsu0.ldi.32 i0 i17
iau.add i5 i1 i4 || sau.add.i32 i18 i0 0
iau.add i6 i2 i4 || Sau.add.i32 i7 i3 i4
nop
LSU0.LDo.64.l v2 i6 -2
LSU0.LDo.64.l v1 i5 -2 || lsu1.cp i20 i18
LSU0.LDo.64.l v3 i7 -2
iau.SHL i4 i0 1  || lsu0.ldi.32 i0 i17
iau.add i5 i1 i4 || sau.add.i32 i18 i0 0
iau.add i6 i2 i4 || Sau.add.i32 i7 i3 i4
nop 3
cmu.cpvi.x16 i8.l v2.1 || lsu1.cp i22 i20 || LSU0.LDo.64.l v2 i6 -2 || bru.rpl i10 i16
cmu.cpivr.x16 v4 i8       || LSU0.LDo.64.l v1 i5 -2 || lsu1.cp i20 i18
localMaxMin3x3_s16_loop:
cmu.vdilv.x16 v6 v7 v2 v2 || LSU0.LDo.64.l v3 i7 -2 ||  iau.SHL i4 i0 1  || lsu1.ldi.32 i0 i17
cmu.vilv.x16  v5 v6 v1 v3 || lsu0.cp i21 v7.0

cmu.cpiv.x32  v5.3 i21    || iau.add i5 i1 i4 || sau.add.i32 i18 i0 0
CMU.CMVV.i16  v4 v5       || iau.add i6 i2 i4 || Sau.add.i32 i7 i3 i4

PEU.PC8C.AND 0x2 || IAU.ADD i9 i9 1   || LSU0.STI.32 i22 i14 //it is a maxima store it
PEU.PC8C.AND 0x4 || IAU.ADD i11 i11 1 || LSU0.STI.32 i22 i15 //it is a minima store it
nop


localMaxMin3x3_s16_exit:
LSU0.ST.32 i9 i12 || LSU1.ST.32 i11 i13 || BRU.JMP i30
cmu.cpvi.x32 i20 v23.0
cmu.cpvi.x32 i21 v23.1
cmu.cpvi.x32 i22 v23.2
NOP 3


.nowarnend
.size mvcvLocalMaxMin3x3_s16_asm,.-mvcvLocalMaxMin3x3_s16_asm
.end
