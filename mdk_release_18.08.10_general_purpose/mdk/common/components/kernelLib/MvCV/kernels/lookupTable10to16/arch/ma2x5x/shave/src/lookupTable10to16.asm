///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvLUT10to16_asm,@function
.version 00.50.34.0

//void LookupTable(const u16** src, u16** dest, const u16* lut, u32 width, u32 height)

.data .data.lut10to16
.align 4
.code .text.lut10to16
mvcvLUT10to16_asm:



lsu0.ldil i1, lookupTable10to16_loop_end  || lsu1.ldih i1, lookupTable10to16_loop_end
LSU0.LD.32 i18, i18                                        //load the beginning address of src
LSU0.LD.32 i17, i17                                        //load the beginning address of dst
lsu1.ldil i6 0x03ff                                       //loads value to and width with
cmu.cpii  i16, i16                                        //load LUT pointer
iau.shr.u32 i2, i15, 2 || cmu.cpivr.x32 v6 i6
iau.xor i0 i0 i0       || cmu.cpivr.x32 v5 i16
nop

 lookupTable10to16_pre_processing:
LSU0.LDI.128.U16.U32 v0 i18
nop 6
vau.and v1, v0, v6
VAU.SHL.X32 v2, v1, 0x01
nop
vau.add.i32 v3, v2, v5
nop


cmu.cpvi i0, v3.0
cmu.cpvi i10, v3.1 || LSU0.LD.16 i4, i0
cmu.cpvi i11, v3.2 || LSU0.LD.16 i5, i10
cmu.cpvi i3, v3.3  || LSU0.LD.16 i6, i11
LSU0.LD.16 i7, i3
nop 2

LSU0.LDI.128.U16.U32 v0, i18
nop 6
vau.and v1, v0, v6
VAU.SHL.X32 v2, v1, 0x01
nop
vau.add.i32 v3, v2, v5
nop


 lookupTable10to16_loop_end_start:
// (Myriad2 COMPATIBILITY ISSUE): BRU.RPL: changed order of parameters at line number 56, in LookUpT10to16.asm
LSU0.LDI.128.U16.U32 v0, i18  ||  bru.rpl i1 i2
nop 6

LSU0.STI.16 i4, i17 ||cmu.cpvi i0, v3.0  || vau.and v1, v0, v6
 lookupTable10to16_loop_end:
 .nowarn 10
LSU0.STI.16 i5, i17 ||cmu.cpvi i10, v3.1  || LSU1.LD.16 i4, i0  || VAU.SHL.X32 v2, v1, 0x01
.nowarnend
LSU0.STI.16 i6, i17 ||cmu.cpvi i11, v3.2  || LSU1.LD.16 i5, i10
LSU0.STI.16 i7, i17 ||cmu.cpvi i3, v3.3   || LSU1.LD.16 i6, i11 || vau.add.i32 v3, v2, v5
LSU1.LD.16 i7, i3
nop 3


bru.jmp i30
nop 6
.size mvcvLUT10to16_asm,.-mvcvLUT10to16_asm
.end
