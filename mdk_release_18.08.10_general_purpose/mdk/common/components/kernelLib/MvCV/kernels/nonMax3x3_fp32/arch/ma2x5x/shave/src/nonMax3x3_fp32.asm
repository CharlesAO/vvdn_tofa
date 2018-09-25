.type mvcvNonMax3x3_fp32_asm,@function
.version 0.50.01

.code .text.nonMax3x3_fp32
.align 16

// NONMAX, maximal reduction on a 3x3 area

//void mvcvNonMax3x3_fp32(
//u16* candPos       - i18
//u32 candCount      - i17
//fp32* corners1     - i16
//fp32* corners2     - i15
//fp32* corners3     - i14
//u16* candOutPos    - i13
//u32 *candOutCount  - i12
//)


//i11 outCount
//i10 0xffffffff
//i9 -Inf
//i8 POS
//i7
//i6 corners1[pos]
//i5 corners2[pos]
//i4 corners2[pos]
//i2 candidate
//i1 candidate pos
//i0 pos * 4 (fp32 --- 32biti ---- 4bytes)

//v0 corners1[pos-1] corners1[pos] corners1[pos+1] (corners1[pos+2])
//v1 corners2[pos-1] corners2[pos] corners2[pos+1] (corners2[pos+2])
//v2 corners3[pos-1] corners3[pos] corners3[pos+1] (corners3[pos+2])
//v3 max (v0, v1)
//v4 candidate
//v5
//v6 max (v0, v1, v2)
//v7
//v8
//v9 i30


.lalign
mvcvNonMax3x3_fp32_asm:
.nowarn 10,40
lsu0.ldi.16 i8 i18  || cmu.cpiv.x32 v9.0 i30    || iau.xor i8 i8 i8     
cmu.cmz.i32 i17     || iau.xor i11 i11 i11      
peu.pc1c eq             || bru.jmp i30              || lsu0.st.32 i11 i12
lsu0.ldil i10 0xffff    || lsu1.ldil i30 ___loopNonMax3x3_fp32  
lsu0.ldih i10 0xffff    || lsu1.ldih i30 ___loopNonMax3x3_fp32
iau.xor i7 i7 i7        

iau.sub i14 i14 4           
iau.sub i15 i15 4       || lsu0.ldil i9 0x0000      || lsu1.ldil i2 ___endNonMax3x3_fp32
iau.sub i16 i16 4       || lsu0.ldih i9 0xff80      || lsu1.ldih i2 ___endNonMax3x3_fp32



iau.shl i0 i8 2     || cmu.cpiv.x32 v9.1 i2
iau.add i6 i16 i0       || sau.add.i32 i5 i15 i0
iau.add i4 i14 i0       || lsu1.ldxv v0 i6      || lsu0.ldi.16 i7 i18
lsu0.ldxv v1 i5     || iau.add i1 i5 4  
lsu1.ldxv v2 i4     || iau.sub i17 i17 1
lsu0.ld.32 i2 i1
peu.pc1i eq             || cmu.cpvi.x32 i30 v9.1    
nop 2
iau.shl i0 i7 2         
nop
cmu.cpiv.x32 v1.1 i9    || iau.add i4 i14 i0
cmu.max.f32 v3 v0 v1    
nop
cmu.cpivr.x32 v4 i2     || lsu1.ldxv v2 i4    
iau.add i6 i16 i0       || sau.add.i32 i5 i15 i0 
cmu.max.f32 v6 v3 v2    || lsu1.ldxv v0 i6  || bru.jmp i30
nop
cmu.cmvv.f32 v6 v4  || lsu0.swzmc4.word [0122] [UUUU]   
PEU.PC4C.AND LT     || lsu0.sti.16 i8 i13                   ||  iau.incs i11 1      
lsu0.st.32 i11 i12  || lsu1.ldi.16 i7 i18   || iau.add i1 i5 4
sau.sub.i16 i8 i7 0 || lsu1.ldxv v1 i5   
nop 2

___loopNonMax3x3_fp32:
lsu0.ld.32 i2 i1        || iau.sub i17 i17 1
peu.pc1i eq             || cmu.cpvi.x32 i30 v9.1    
nop 2
iau.shl i0 i7 2         || lsu0.st.32 i11 i12
cmu.cpiv.x32 v1.1 i9    || iau.add i4 i14 i0
cmu.max.f32 v3 v0 v1    || iau.add i5 i15 i0  
bru.jmp i30             || lsu1.ldxv v1 i5
cmu.cpivr.x32 v4 i2 
iau.add i6 i16 i0       || lsu1.ldxv v2 i4    
cmu.max.f32 v6 v3 v2    || lsu0.ldxv v0 i6 
lsu1.ldi.16 i7 i18      || iau.add i1 i5 4     
cmu.cmvv.f32 v6 v4      || lsu0.swzmc4.word [0122] [UUUU]       || sau.sub.i16 i8 i7 0
PEU.PC4C.AND LT         || lsu0.sti.16 i8 i13                   ||  iau.incs i11 1      


___endNonMax3x3_fp32:

cmu.cpvi.x32 i30 v9.0
bru.jmp i30
lsu0.st.32 i11 i12
nop 5
.nowarnend
.size mvcvNonMax3x3_fp32_asm,.-mvcvNonMax3x3_fp32_asm
.end
