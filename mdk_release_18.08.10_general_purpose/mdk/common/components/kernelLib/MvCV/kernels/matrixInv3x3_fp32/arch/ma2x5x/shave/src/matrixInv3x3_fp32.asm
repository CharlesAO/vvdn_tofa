.type mvcvMatrixInv3x3_fp32_asm,@function
.version 00.50.00
.code .text.matrixInv3x3_fp32

//void mvcvMatrixInv3x3_fp32(fp32 **in, fp32 **out)
//                                i18          i17
mvcvMatrixInv3x3_fp32_asm:

lsu0.ld.32 i0 i18         || lsu1.ldo.32 i1 i18 0x4
lsu0.ldo.32 i2 i18 0x8    || lsu1.ld.32 i3 i17 
lsu0.ldo.32 i4 i17 0x4    || lsu1.ldo.32 i5 i17 0x8
nop 4
lsu1.ld.64.l v1 i1 || lsu0.ldo.64.h v1 i1 0x08 //line 1
lsu1.ld.64.l v2 i2 || lsu0.ldo.64.h v2 i2 0x08 //line 2
lsu1.ld.64.l v0 i0 || lsu0.ldo.64.h v0 i0 0x08 //line 0
iau.xor i0 i0 i0 
iau.incs i0 1
cmu.cpii.i32.f32 i0 i0 
nop 2
vau.mul.f32 v3 v1 v2  || lsu0.swzmv4.word [0102] [ZUUU] //v3 {in[2][0]*in[1][2], in[2][1]*in[1][0], in[2][2]*in[1][1], x}
vau.mul.f32 v4 v2 v1  || lsu0.swzmv4.word [0102] [ZUUU] //v4 {in[1][0]*in[2][2], in[1][1]*in[2][0], in[1][2]*in[2][1], x}
vau.mul.f32 v7 v1 v0  || lsu0.swzmv4.word [0021] [ZUUU] //v7 {in[0][0]*in[1][1], in[0][1]*in[1][2], in[0][2]*in[1][0], x}
vau.mul.f32 v8 v0 v1  || lsu0.swzmv4.word [0021] [ZUUU] //v8 {in[1][0]*in[0][1], in[1][1]*in[0][2], in[1][2]*in[0][0], x}
vau.sub.f32 v5 v3 v4                                       //v5 {a2, a3, a1, x}
vau.mul.f32 v11 v2 v0 || lsu0.swzmv4.word [0102] [ZUUU] //v11 {in[0][0]*in[2][2], in[0][1]*in[2][0], in[0][2]*in[2][1], x}
vau.sub.f32 v9 v7 v8
vau.mul.f32 v6 v5 v0  || lsu0.swzmv4.word [0102] [ZUUU]
vau.mul.f32 v12 v0 v2 || lsu0.swzmv4.word [0102] [ZUUU] //v12 {in[2][0]*in[0][2], in[2][1]*in[0][0], in[2][2]*in[0][1], x}
nop
sau.sumx.f32 i16 v6 //i16 determinant
vau.sub.f32 v13 v11 v12
nop 3
sau.div.f32 i15 i0 i16 //i15 invdet
nop 11
cmu.cpivr.x32 v15 i15
vau.mul.f32 v16 v5 v15  || lsu0.swzmv4.word [0102] [ZUUU] || BRU.JMP i30
vau.mul.f32 v10 v9 v15  || lsu0.swzmv4.word [0021] [ZUUU]
vau.mul.f32 v14 v13 v15 || lsu0.swzmv4.word [0102] [ZUUU]
lsu0.st.64.l v16 i3     || cmu.cpvi i6 v16.2
lsu0.st.64.l v10 i5     || cmu.cpvi i7 v10.2   || lsu1.sto.32 i6 i3 0x8
lsu0.st.64.l v14 i4     || cmu.cpvi i8 v14.2   || lsu1.sto.32 i7 i5 0x8
lsu1.sto.32 i8 i4 0x8
	
.size mvcvMatrixInv3x3_fp32_asm,.-mvcvMatrixInv3x3_fp32_asm
.end	
	