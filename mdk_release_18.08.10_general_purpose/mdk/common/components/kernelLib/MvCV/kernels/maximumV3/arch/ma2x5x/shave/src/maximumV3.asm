///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///// This kernel performs 1 maximum line from 3 input lines
///// @param[in] in        - 3 Input lines
///// @param[out] out      - 1 Output line
///// @param[in] width     - Width of input line

//--------------------------------------------------------------------------------------------------------------
.type mvcvMaximumV3_asm,@function
.version 00.50.05
.data .rodata.vect_mask
.align 4
.salign 16

.code .text.MaximumV3 //text
.salign 16

.align 16
//void mvcvMaximumV3(u8** in, u8** out, u32 width)
//                         i18      i17       i16
mvcvMaximumV3_asm:
.nowarn 10,11
lsu0.ldi.32 i0 i18 || iau.shr.u32 i3 i16 4
lsu0.ldi.32 i1 i18 || IAU.SHL i4 i3 4
lsu0.ldi.32 i2 i18 || IAU.SUB i5 i16 i4
nop 6
lsu0.ldi.32 i12 i17 || lsu1.ldil i11 mvcvMaximumV3loop
                       lsu1.ldih i11 mvcvMaximumV3loop

CMU.CMZ.i32 i3
PEU.PC1C eq || BRU.BRA mvcvMaximumV3loop_compensate

lsu0.ldo.64.l v0  i0  0 || lsu1.ldo.64.h v0  i0  8 || iau.add i0  i0  16
lsu0.ldo.64.l v3  i0  0 || lsu1.ldo.64.h v3  i0  8 || iau.add i0  i0  16
lsu0.ldo.64.l v1  i1  0 || lsu1.ldo.64.h v1  i1  8 || iau.add i1  i1  16
lsu0.ldo.64.l v4  i1  0 || lsu1.ldo.64.h v4  i1  8 || iau.add i1  i1  16
lsu0.ldo.64.l v2  i2  0 || lsu1.ldo.64.h v2  i2  8 || iau.add i2  i2  16
lsu0.ldo.64.l v5  i2  0 || lsu1.ldo.64.h v5  i2  8 || iau.add i2  i2  16
nop 3
                          lsu0.ldo.64.l v0  i0  0 || lsu1.ldo.64.h v0  i0  8 || iau.add i0  i0  16 || bru.rpl i11 i3
						  mvcvMaximumV3loop:
                          lsu0.ldo.64.l v3  i0  0 || lsu1.ldo.64.h v3  i0  8 || iau.add i0  i0  16
cmu.max.f16 v12 v0 v1  || lsu0.ldo.64.l v1  i1  0 || lsu1.ldo.64.h v1  i1  8 || iau.add i1  i1  16
cmu.max.f16 v6 v3 v4   || lsu0.ldo.64.l v4  i1  0 || lsu1.ldo.64.h v4  i1  8 || iau.add i1  i1  16
cmu.max.f16 v14 v12 v2 || lsu0.ldo.64.l v2  i2  0 || lsu1.ldo.64.h v2  i2  8 || iau.add i2  i2  16
cmu.max.f16 v7 v6 v5   || lsu0.ldo.64.l v5  i2  0 || lsu1.ldo.64.h v5  i2  8 || iau.add i2  i2  16
lsu0.sto.64.l v14 i12 0 || lsu1.sto.64.h v14 i12 8 || iau.add i12 i12 16
lsu0.sto.64.l v7 i12 0 || lsu1.sto.64.h v7 i12 8 || iau.add i12 i12 16


mvcvMaximumV3loop_compensate:
nop 3
cmu.max.f16 v12 v0 v1
nop
cmu.max.f16 v14 v12 v2 || IAU.SUB i5 i5 0
nop
PEU.PC1I gt || lsu0.sto.64.l v14 i12 0 || lsu1.sto.64.h v14 i12 8 || iau.add i12 i12 16



bru.jmp i30
nop 6
.nowarnend
.size mvcvMaximumV3_asm,.-mvcvMaximumV3_asm
.end
