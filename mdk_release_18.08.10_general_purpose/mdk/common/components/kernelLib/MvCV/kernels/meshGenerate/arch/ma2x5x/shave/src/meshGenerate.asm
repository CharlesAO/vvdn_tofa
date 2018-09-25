.type mvcvMeshGenerate_asm,@function
.version 0.50.00

// i18: out pointer
// i17: warp matrix pointer
// i16: width
// i15: posx
// i14: posy
// v8-13: warp
// v21: increment
// v22: posx
// v23: posy
.nowarn
.code .text.MeshGenerate_asm //text
.align 16
mvcvMeshGenerate_asm:
                               IAU.SHL i15, i15, 0x03     || LSU0.LDI.32R v8,  i17  || LSU1.LDIL i0, 4
CMU.CPIVR.x32    v21,   i0  || IAU.SHL i14, i14, 0x03     || LSU0.LDI.32R v10, i17
CMU.CPIV         v22.0, i15 || IAU.ADD i15, i15, 0x01     || LSU0.LDI.32R v12, i17
CMU.CPIV         v22.1, i15 || IAU.ADD i15, i15, 0x01     || LSU0.LDI.32R v9,  i17
CMU.CPIV         v22.2, i15 || IAU.ADD i15, i15, 0x01     || LSU0.LDI.32R v11, i17
CMU.CPIV         v22.3, i15                               || LSU0.LDI.32R v13, i17
CMU.CPIVR.x32    v23,   i14
CMU.CPVV.i32.f32 v21,   v21
CMU.CPVV.i32.f32 v22,   v22 || LSU1.LDIL i0, MeshGenerateLoop
CMU.CPVV.i32.f32 v23,   v23 || LSU1.LDIH i0, MeshGenerateLoop
MeshGenerateLoop:
VAU.MUL.f32 v2,  v22, v8
VAU.MUL.f32 v3,  v22, v9
VAU.MUL.f32 v0,  v23, v10
VAU.MUL.f32 v1,  v23, v11
VAU.ADD.f32 v2,  v2,  v12
VAU.ADD.f32 v3,  v3,  v13
nop
VAU.ADD.f32 v2,  v2,  v0
VAU.ADD.f32 v3,  v3,  v1   || IAU.SUBSU i16, i16, 0x04
CMU.CPII i0,  i30          || PEU.PC1I 0x1
VAU.SUB.f32 v4,  v2,  v22  || BRU.JMP i0
VAU.SUB.f32 v5,  v23,  v3 //change sign
VAU.ADD.f32 v22, v22, v21
nop
CMU.VILV.x32 v4,  v5,  v5,  v4
IAU.ADD     i18, i18, 0x08 || LSU0.ST.128.f32.f16  v4,  i18
IAU.ADD     i18, i18, 0x08 || LSU0.ST.128.f32.f16  v5,  i18

MeshGenerateExit:

BRU.SWIH 0x1F
nop
nop
nop
nop
nop
nop
.nowarnend
.size mvcvMeshGenerate_asm,.-mvcvMeshGenerate_asm
.end
