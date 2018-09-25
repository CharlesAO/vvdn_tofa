///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvArithmeticSubU16_asm,@function
.version 00.51.04

.data .data.arithmeticSubU16
.align 4
.code .text.arithmeticSubU16

//void arithmeticSub(u8** src1, u8** src2, u8** dst, u32 width)
//					   i18         i17       i16       i15
mvcvArithmeticSubU16_asm:
.nowarn
LSU0.LDIL i2 0x001F || LSU1.LDIH i2 0x0 
LSU0.ldil i7 arithmeticSubU16_final || LSU1.ldih i7 arithmeticSubU16_final 
LSU0.ldil i9 arithmeticSubU16_compensate || LSU1.ldih i9 arithmeticSubU16_compensate
LSU0.ldil i5, arithmeticSubU16_loop || LSU1.ldih i5, arithmeticSubU16_loop
lsu0.ld.32 i18 i18  || IAU.AND i3 i15 i2
lsu0.ld.32 i17 i17  || IAU.SHR.u32 i12 i15 5
lsu0.ld.32 i16 i16



cmu.cmz.i8 i12
peu.pc1c eq || bru.jmp i9
nop 6

lsu0.ldo.64.l v0 i18 0  || lsu1.ldo.64.h v0 i18 8  
lsu0.ldo.64.l v1 i17 0  || lsu1.ldo.64.h v1 i17 8  
lsu0.ldo.64.l v2 i18 16 || lsu1.ldo.64.h v2 i18 24 
lsu0.ldo.64.l v3 i17 16 || lsu1.ldo.64.h v3 i17 24 
lsu0.ldo.64.l v4 i18 32 || lsu1.ldo.64.h v4 i18 40 
lsu0.ldo.64.l v5 i17 32 || lsu1.ldo.64.h v5 i17 40 
lsu0.ldo.64.l v6 i18 48 || lsu1.ldo.64.h v6 i18 56 || iau.add i18 i18 64 
lsu0.ldo.64.l v7 i17 48 || lsu1.ldo.64.h v7 i17 56 || iau.add i17 i17 64 
nop 6



lsu0.ldo.64.l v0 i18 0  || lsu1.ldo.64.h v0 i18 8  || bru.rpl i5 i12
lsu0.ldo.64.l v1 i17 0  || lsu1.ldo.64.h v1 i17 8                          || vau.sub.i16  v8 v0 v1
lsu0.ldo.64.l v2 i18 16 || lsu1.ldo.64.h v2 i18 24                         || vau.sub.i16  v9 v2 v3
lsu0.ldo.64.l v3 i17 16 || lsu1.ldo.64.h v3 i17 24                         || vau.sub.i16 v10 v4 v5
lsu0.ldo.64.l v4 i18 32 || lsu1.ldo.64.h v4 i18 40                         || vau.sub.i16 v11 v6 v7
arithmeticSubU16_loop:

lsu0.ldo.64.l v5 i17 32 || lsu1.ldo.64.h v5 i17 40                         

lsu0.ldo.64.l v6 i18 48 || lsu1.ldo.64.h v6 i18 56 || iau.add i18 i18 64 
lsu0.ldo.64.l v7 i17 48 || lsu1.ldo.64.h v7 i17 56 || iau.add i17 i17 64 
lsu0.sto.64.l  v8 i16 0  || lsu1.sto.64.h  v8 i16 8  
lsu0.sto.64.l  v9 i16 16 || lsu1.sto.64.h  v9 i16 24 
lsu0.sto.64.l v10 i16 32 || lsu1.sto.64.h v10 i16 40  
lsu0.sto.64.l v11 i16 48 || lsu1.sto.64.h v11 i16 56 || iau.add i16 i16 64 


iau.sub i18 i18 64
iau.sub i17 i17 64

cmu.cmz.i32 i3 
peu.pc1c eq || bru.jmp i7
nop 6

arithmeticSubU16_compensate:
lsu0.ldo.64.l v0 i18 0 || lsu1.ldo.64.h v0 i18 8 || iau.add i18 i18 16 
lsu0.ldo.64.l v1 i17 0 || lsu1.ldo.64.h v1 i17 8 || iau.add i17 i17 16 
lsu0.ldo.64.l v2 i18 0 || lsu1.ldo.64.h v2 i18 8 || iau.add i18 i18 16 
lsu0.ldo.64.l v3 i17 0 || lsu1.ldo.64.h v3 i17 8 || iau.add i17 i17 16 
lsu0.ldo.64.l v4 i18 0 || lsu1.ldo.64.h v4 i18 8 || iau.add i18 i18 16 
lsu0.ldo.64.l v5 i17 0 || lsu1.ldo.64.h v5 i17 8 || iau.add i17 i17 16 
lsu0.ldo.64.l v6 i18 0 || lsu1.ldo.64.h v6 i18 8 || iau.add i18 i18 16 
lsu0.ldo.64.l v7 i17 0 || lsu1.ldo.64.h v7 i17 8 || iau.add i17 i17 16 
nop 6

vau.sub.i16  v8 v0 v1
vau.sub.i16  v9 v2 v3
vau.sub.i16 v10 v4 v5
vau.sub.i16 v11 v6 v7
nop
IAU.SUB i3, i3, 8
PEU.PC1I GTE || lsu0.sto.64.l  v8 i16 0  || lsu1.sto.64.h  v8 i16 8  || iau.sub i3 i3 8 
PEU.PC1I GTE || lsu0.sto.64.l  v9 i16 16 || lsu1.sto.64.h  v9 i16 24 || iau.sub i3 i3 8 
PEU.PC1I GTE || lsu0.sto.64.l v10 i16 32 || lsu1.sto.64.h v10 i16 40 || iau.sub i3 i3 8
PEU.PC1I GTE || lsu0.sto.64.l v11 i16 48 || lsu1.sto.64.h v11 i16 56 


arithmeticSubU16_final:

    BRU.jmp i30
	nop 6
.nowarnend
.size mvcvArithmeticSubU16_asm,.-mvcvArithmeticSubU16_asm
.end
