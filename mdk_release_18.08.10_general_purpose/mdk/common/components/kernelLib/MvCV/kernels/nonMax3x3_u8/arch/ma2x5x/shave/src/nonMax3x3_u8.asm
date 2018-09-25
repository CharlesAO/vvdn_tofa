.type mvcvNonMax3x3_u8_asm,@function
.version 0.50.00

.code .text.nonMax3x3_u8
.align 16

// NONMAX, maximal reduction on a 3x3 area

//void mvcvNonMax3x3_u8(
//u32 width      	- i18
//u8* corners1       - i17
//u8* corners2       - i16
//u8* corners3       - i15
//u16* candOutPos    - i14
//u32 *candOutCount  - i13
//)

.lalign
mvcvNonMax3x3_u8_asm:

lsu0.ldil i12  ___loopNonMax3x3_u8 || lsu1.ldih i12  ___loopNonMax3x3_u8
lsu0.ldil i2 0xffff || lsu1.ldih i2 0x0000  || iau.xor i10 i10 i10
lsu0.ldil i3 0xffff || lsu1.ldih i3 0xffff	|| iau.xor i1 i1 i1 
cmu.cpiv v12.0 i2   || iau.sub i17 i17 1
cmu.cpiv v12.1 i3   || iau.sub i16 i16 1
cmu.cpiv v12.2 i3   || iau.sub i15 i15 1
cmu.cpiv v12.3 i3
cmu.cpii i8 i18

lsu0.ld.128.u8.u16 v0 i17 || lsu1.ld.128.u8.u16 v1 i16
lsu0.ld.128.u8.u16 v2 i15 || iau.sub i7 i8 i18
nop 2
iau.add i17 i17 1
iau.add i16 i16 1
iau.add i15 i15 1
cmu.cpvi.x32 i1 v1.0   

iau.shr.u32 i1 i1 16|| cmu.max.u16 v3 v0 v2  || lsu0.ld.128.u8.u16 v0 i17 || lsu1.ld.128.u8.u16 v1 i16 || bru.rpl i12 i18
___loopNonMax3x3_u8: 
.nowarn 10
vau.and v1 v1 v12   || lsu0.ld.128.u8.u16 v2 i15 || iau.sub i7 i8 i18
.nowarnend
cmu.cpivr.x16 v4 i1 || iau.add i15 i15 1
cmu.max.u16 v5 v3 v1||iau.add i17 i17 1
iau.sub i18 i18 1
cmu.cmvv.u16 v5 v4 	|| lsu0.swzmc8.l [0122] [UUUU] || iau.add i16 i16 1
PEU.PC4C.AND LT		|| lsu0.sti.16 i7 i14	|| iau.incs i10 1	
lsu0.st.32 i10 i13  || cmu.cpvi.x32 i1 v1.0 


bru.jmp i30
nop 6
.size mvcvNonMax3x3_u8_asm,.-mvcvNonMax3x3_u8_asm
.end