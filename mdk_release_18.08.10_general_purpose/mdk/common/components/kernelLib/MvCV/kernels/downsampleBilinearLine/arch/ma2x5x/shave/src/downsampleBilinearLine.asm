.type mvcvDownsampleBilinearLine_asm,@function
.version 00.50.00

//////
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///

.data .rodata.mvcvDownsampleBilinearLine_asm
.align 16

.code .text.mvcvDownsampleBilinearLine_asm
.align 16
.nowarn
.lalign
mvcvDownsampleBilinearLine_asm:
//mvcvDownsampleBilinearLine(u16 *inLine, u32 *tempLine, u16 *outLine, u32 inWidth, u32 select)
//                                 i18           i17           i16          i15         i14
iau.shr.u32 i15 i15 4
LSU0.ldil i0, mvcvDownsampleBilinearLine_asm_second || LSU1.ldih i0, mvcvDownsampleBilinearLine_asm_second

cmu.cmz.i32 i14
peu.pc1c neq || bru.jmp i0
nop 6

LSU0.ldil i1, mvcvDownsampleBilinearLine_asm_loop || LSU1.ldih i1, mvcvDownsampleBilinearLine_asm_loop


lsu0.ldi.128.u16.u32 v0 i18
lsu0.ldi.128.u16.u32 v1 i18
lsu0.ldi.128.u16.u32 v5 i18
lsu0.ldi.128.u16.u32 v6 i18
nop 2

lsu0.ldi.128.u16.u32 v0 i18 || bru.rpl i1 i15
lsu0.ldi.128.u16.u32 v1 i18
mvcvDownsampleBilinearLine_asm_loop:
lsu0.ldi.128.u16.u32 v5 i18
cmu.vdilv.x32 v3 v2 v0 v1 || lsu0.ldi.128.u16.u32 v6 i18
cmu.vdilv.x32 v8 v7 v5 v6
vau.add.i32 v4 v2 v3
vau.add.i32 v9 v7 v8

lsu0.sto.64.l v4 i17 0 || lsu1.sto.64.h v4 i17 8 || iau.add i17 i17 16
lsu0.sto.64.l v9 i17 0 || lsu1.sto.64.h v9 i17 8 || iau.add i17 i17 16

bru.jmp i30
nop 6

mvcvDownsampleBilinearLine_asm_second:
.nowarn 10
LSU0.ldil i1, mvcvDownsampleBilinearLine_asm_loop1 || LSU1.ldih i1, mvcvDownsampleBilinearLine_asm_loop1
.nowarnend
LSU0.ldil i2, 0x2
cmu.cpivr.x32 v7 i2


lsu0.ldi.128.u16.u32 v0 i18
lsu0.ldi.128.u16.u32 v1 i18
lsu0.ldi.128.u16.u32 v12 i18
lsu0.ldi.128.u16.u32 v13 i18
lsu0.ldo.64.l v5 i17 0 || lsu1.ldo.64.h v5 i17 8 || iau.add i17 i17 16
lsu0.ldo.64.l v14 i17 0 || lsu1.ldo.64.h v14 i17 8 || iau.add i17 i17 16
nop 3
cmu.vdilv.x32 v3 v2 v0 v1   || bru.rpl i1 i15
cmu.vdilv.x32 v16 v15 v12 v13
vau.add.i32 v4 v2 v3                   || lsu0.ldi.128.u16.u32 v0 i18
vau.add.i32 v6 v5 v7                   || lsu0.ldi.128.u16.u32 v1 i18
vau.add.i32 v17 v15 v16                || lsu0.ldi.128.u16.u32 v12 i18
vau.add.i32 v18 v14 v7                 || lsu0.ldi.128.u16.u32 v13 i18
mvcvDownsampleBilinearLine_asm_loop1:

vau.add.i32 v8 v4 v6                   || lsu0.ldo.64.l v5 i17 0 || lsu1.ldo.64.h v5 i17 8 || iau.add i17 i17 16
vau.add.i32 v19 v17 v18                || lsu0.ldo.64.l v14 i17 0 || lsu1.ldo.64.h v14 i17 8 || iau.add i17 i17 16

vau.shr.u32 v9 v8 2
vau.shr.u32 v20 v19 2
cmu.vdilv.x16 v11 v10 v9 v9
lsu0.sti.64.l v10 i16 || cmu.vdilv.x16 v22 v21 v20 v20
lsu0.sti.64.l v21 i16 || cmu.vdilv.x32 v3 v2 v0 v1

bru.jmp i30
nop 6
.size mvcvDownsampleBilinearLine_asm,.-mvcvDownsampleBilinearLine_asm
.nowarnend
.end
