.type mvcvMeshExpand_asm,@function
.version 0.50.00

// i18: out pointer
// i17: in  pointer list
// i16: width
// i15: mesh pointer list ->meshBot
// i14: my -> meshTop
// i13: contribBot
// i12: contribTop
// v10: prev dy
// v11: prev dx
// v12: iy
// v13: ix
// v14: dy
// v15: dx
// v16: meshY0
// v17: meshX0
// v18: meshY1
// v19: meshX1
// v20: contribLeft
// v21: contribRight
// v22: 1(one)
// v23: backup

.code .text.meshExp //text
.align 16

mvcvMeshExpand_asm:

.nowarn

// arrange variables in registers
LSU0.LDI.32 i15, i15            || LSU1.LDIL i1,  0x0    //1.0f32
LSU0.LDI.32 i14, i15            || LSU1.LDIH i1,  0x3f80 // 1.0f32
SAU.SUB.f32 i0,  i1,  i14       || LSU1.LDIL i4,  0x3c00 //1.0f16
CMU.CPII.f32.f16 i1,  i1        || LSU1.LDIH i4,  0x3b00 //0.875f16
CMU.CPII.f32.f16 i13, i14       || LSU1.LDIL i5,  0x3a00 //0.75f16
CMU.CPII.f32.f16 i12, i0        || LSU1.LDIH i5,  0x3900 //0.625f16
CMU.CPIVR.x16 v22, i1           || LSU1.LDIL i6,  0x3800 //0.5f16
LSU0.LDI.32     i21, i15        || LSU1.LDIH i6,  0x3600 //0.375f16
LSU0.LDI.32     i20, i14        || LSU1.LDIL i7,  0x3400 //0.25f16
CMU.CPIV  v23.0, i20            || LSU1.LDIH i7,  0x3000 //0.125f16
CMU.CPIV  v23.1, i21            || LSU1.LDIL i8,  0x0    //0.0f16
CMU.CPIV  v23.2, i22            || LSU1.LDIH i8,  0x3000 //0.125f16
CMU.VSZM.BYTE  i12, i12, [1010] || LSU1.LDIL i9,  0x3400 //0.25f16
CMU.VSZM.BYTE  i13, i13, [1010] || LSU1.LDIH i9,  0x3600 //0.375f16
CMU.VSIGNF.f16 i12, i12, [NP]   || LSU1.LDIL i10, 0x3800 //0.5f16
CMU.VSIGNF.f16 i13, i13, [NP]   || LSU1.LDIH i10, 0x3900 //0.625f16
SAU.MUL.f16 i20, i20, i12       || LSU1.LDIL i11, 0x3a00 //0.75f16
SAU.MUL.f16 i21, i21, i13       || LSU1.LDIH i11, 0x3b00 //0.875f16
CMU.CPIVD v20.0, i4             || LSU1.LDIL i22, 0
CMU.CPIVD v20.2, i6
SAU.ADD.f16 i21, i21, i20
CMU.CPIVD v21.0, i8
CMU.CPIVD v21.2, i10
CMU.CPIVR.x16     v17, i21     || IAU.SHR.u32 i20, i21, 0x10
CMU.CPIVR.x16     v16, i20
VAU.MUL.f16 v14, v16, v20
VAU.MUL.f16 v15, v17, v20
VAU.MUL.f16 v0,  v16, v21
VAU.MUL.f16 v1,  v17, v21
nop
VAU.ADD.f16 v14, v14, v0
VAU.ADD.f16 v15, v15, v1
nop
CMU.CPVV.f16.i16s v12, v14
CMU.CPVV.f16.i16s v13, v15
CMU.CMZ.f16       v14
VAU.SUB.i16 v12, v12, 0x01 || PEU.PVV16 0x4
CMU.CMZ.f16       v15
VAU.SUB.i16 v13, v13, 0x01 || PEU.PVV16 0x4
CMU.CPVV.i16.f16  v0,  v12 || VAU.SUB.i16 v12, v12, 0x01
CMU.CPVV.i16.f16  v1,  v13
VAU.SHL.x16 v12, v12, 0x02
VAU.SUB.f16 v14, v14, v0
VAU.SUB.f16 v15, v15, v1

// take off
CMU.CPVV.i16.i32  v0,  v12         || LSU0.LDI.32     i20, i14
CMU.VSZM.WORD     v1,  v12, [1032] || LSU0.LDI.32     i21, i15      || LSU1.CP i0,  v0.0
CMU.CPVV.i16.i32  v1,  v1          || LSU0.LDX.64.l   v0,  i17, i0  || LSU1.CP i1,  v0.1
CMU.CPVV.i16.i32  v2,  v13         || LSU0.LDX.64.h   v0,  i17, i1  || LSU1.CP i0,  v0.2
CMU.VSZM.WORD     v3,  v13, [1032] || LSU0.LDX.64.l   v1,  i17, i0  || LSU1.CP i1,  v0.3
CMU.CPVV.i16.i32  v3,  v3          || LSU0.LDX.64.h   v1,  i17, i1  || LSU1.CP i0,  v1.0
CMU.CPVI          i4,  v2.0        || LSU0.LDX.64.l   v2,  i17, i0  || LSU1.CP i1,  v1.1
CMU.CPVI          i5,  v2.1        || LSU0.LDX.64.h   v2,  i17, i1  || LSU1.CP i0,  v1.2  || SAU.MUL.f16 i20, i20, i12
CMU.CPVI          i6,  v2.2        || LSU0.LDX.64.l   v3,  i17, i0  || LSU1.CP i1,  v1.3  || SAU.MUL.f16 i21, i21, i13
CMU.CPVI          i7,  v2.3        || LSU0.LDX.64.h   v3,  i17, i1  || LSU1.CP i2,  v0.0  || VAU.ADD.f16 v10, v14, 0.5    || IAU.ADD  i4,  i4,  i22
CMU.CPVI          i8,  v3.0        || LSU0.LDX.16     i4,  i2,  i4  || LSU1.CP i3,  v0.1  || VAU.ADD.f16 v11, v15, 0.5    || IAU.ADD  i22, i22, 0x01
CMU.CPVI          i9,  v3.1        || LSU0.LDX.16     i5,  i3,  i4  || LSU1.CP i2,  v0.2  || SAU.ADD.f16 i21, i21, i20    || IAU.ADD  i5,  i5,  i22
CMU.CPVI          i10, v3.2        || LSU0.LDX.16     i0,  i2,  i5  || LSU1.CP i3,  v0.3  || VAU.MUL.f16 v14, v16, v20    || IAU.ADD  i22, i22, 0x01
CMU.CPVI          i11, v3.3        || LSU0.LDX.16     i1,  i3,  i5  || LSU1.CP i2,  v1.0  || VAU.MUL.f16 v15, v17, v20    || IAU.ADD  i6,  i6,  i22
CMU.CPIVR.x16     v19, i21         || LSU0.LDX.16     i6,  i2,  i6  || LSU1.CP i3,  v1.1  || SAU.SWZ     i20, i21, [1032] || IAU.ADD  i22, i22, 0x01
CMU.CPIVR.x16     v18, i20         || LSU0.LDX.16     i7,  i3,  i6  || LSU1.CP i2,  v1.2  || VAU.MUL.f16 v1,  v19, v21    || IAU.ADD  i7,  i7,  i22
                                      LSU0.LDX.16     i0,  i2,  i7  || LSU1.CP i3,  v1.3  || VAU.MUL.f16 v0,  v18, v21    || IAU.ADD  i22, i22, 0x01
                                      LSU0.LDX.16     i1,  i3,  i7  || LSU1.CP i2,  v2.0                                  || IAU.ADD  i8,  i8,  i22
                                      LSU0.LDX.16     i8,  i2,  i8  || LSU1.CP i3,  v2.1  || VAU.ADD.f16 v15, v15, v1     || IAU.ADD  i22, i22, 0x01
CMU.VSZM.BYTE     i4,  i0,  [10DD] || LSU0.LDX.16     i9,  i3,  i8  || LSU1.CP i2,  v2.2  || VAU.ADD.f16 v14, v14, v0     || IAU.ADD  i9,  i9,  i22
CMU.VSZM.BYTE     i5,  i1,  [10DD] || LSU0.LDX.16     i0,  i2,  i9  || LSU1.CP i3,  v2.3                                  || IAU.ADD  i22, i22, 0x01
CMU.CPVV.f16.i16s v13, v15         || LSU0.LDX.16     i1,  i3,  i9  || LSU1.CP i2,  v3.0                                  || IAU.ADD  i10, i10, i22
CMU.CPVV.f16.i16s v12, v14         || LSU0.LDX.16     i10, i2,  i10 || LSU1.CP i3,  v3.1                                  || IAU.ADD  i22, i22, 0x01
CMU.VSZM.BYTE     i6,  i0,  [10DD] || LSU0.LDX.16     i11, i3,  i10 || LSU1.CP i2,  v3.2                                  || IAU.ADD  i11, i11, i22
CMU.VSZM.BYTE     i7,  i1,  [10DD] || LSU0.LDX.16     i0,  i2,  i11 || LSU1.CP i3,  v3.3                                  || IAU.ADD  i22, i22, 0x01
CMU.CMZ.f16       v14              || LSU0.LDX.16     i1,  i3,  i11
CMU.CMZ.f16       v15                                               || LSU1.CP v4.0,  i4  || VAU.SUB.i16 v12, v12, 0x01   || PEU.PVV16 0x4
CMU.VSZM.BYTE     i8,  i0,  [10DD]                                  || LSU1.CP v6.0,  i5  || VAU.SUB.i16 v13, v13, 0x01   || PEU.PVV16 0x4
CMU.VSZM.BYTE     i9,  i1,  [10DD]                                  || LSU1.CP v4.1,  i6
CMU.CPVV.i16.f16  v0,  v12                                          || LSU1.CP v6.1,  i7  || VAU.SUB.i16 v12, v12, 0x01
CMU.CPVV.i16.f16  v1,  v13                                          || LSU1.CP v5.0,  i8
CMU.VSZM.BYTE     i10, i0,  [10DD]                                  || LSU1.CP v7.0,  i9  || VAU.SHL.x16 v12, v12, 0x02
CMU.VSZM.BYTE     i11, i1,  [10DD]                                  || LSU1.CP v5.1,  i10 || VAU.SUB.f16 v14, v14, v0
                                                                       LSU1.CP v7.1,  i11 || VAU.SUB.f16 v15, v15, v1     || IAU.SUBSU i16, i16, 0x08
CMU.CPVV.u8f.f16  v4,  v4                                                                 || BRU.BRA MeshExpandLoop
CMU.CPVV.u8f.f16  v5,  v5                                                                 || VAU.SUB.f16 v0,  v22, v10
CMU.CPVV.u8f.f16  v6,  v6                                           || LSU1.CP v16, v18   || VAU.SUB.f16 v1,  v22, v11
CMU.CPVV.u8f.f16  v7,  v7                                           || LSU1.CP v17, v19   || VAU.MUL.f16 v11, v10, v11
                                                                                             VAU.MUL.f16 v9,  v0,  v11
CMU.VDILV.x16 v5,  v4,  v4,  v5                                                           || VAU.MUL.f16 v10, v10, v1
CMU.VDILV.x16 v7,  v6,  v6,  v7                                                           || VAU.MUL.f16 v8,  v0,  v1

// warp
MeshExpandLoop:
CMU.CPVV.i16.i32  v0,  v12         || LSU0.LDI.32     i20, i14                            || VAU.MUL.f16 v11, v11, v7
CMU.VSZM.WORD     v1,  v12, [1032] || LSU0.LDI.32     i21, i15      || LSU1.CP i0,  v0.0  || VAU.MUL.f16 v10, v10, v6
CMU.CPVV.i16.i32  v1,  v1          || LSU0.LDX.64.l   v4,  i17, i0  || LSU1.CP i1,  v0.1  || VAU.MUL.f16 v9,  v9,  v5
CMU.CPVV.i16.i32  v2,  v13         || LSU0.LDX.64.h   v4,  i17, i1  || LSU1.CP i0,  v0.2  || VAU.MUL.f16 v8,  v8,  v4
CMU.VSZM.WORD     v3,  v13, [1032] || LSU0.LDX.64.l   v5,  i17, i0  || LSU1.CP i1,  v0.3
CMU.CPVV.i16.i32  v3,  v3          || LSU0.LDX.64.h   v5,  i17, i1  || LSU1.CP i0,  v1.0  || VAU.ADD.f16 v9,  v9,  v11
CMU.CPVI          i4,  v2.0        || LSU0.LDX.64.l   v6,  i17, i0  || LSU1.CP i1,  v1.1  || VAU.ADD.f16 v8,  v8,  v10
CMU.CPVI          i5,  v2.1        || LSU0.LDX.64.h   v6,  i17, i1  || LSU1.CP i0,  v1.2  || SAU.MUL.f16 i20, i20, i12
CMU.CPVI          i6,  v2.2        || LSU0.LDX.64.l   v7,  i17, i0  || LSU1.CP i1,  v1.3  || SAU.MUL.f16 i21, i21, i13
CMU.CPVI          i7,  v2.3        || LSU0.LDX.64.h   v7,  i17, i1  || LSU1.CP i2,  v4.0  || VAU.ADD.f16 v10, v14, 0.5    || IAU.ADD  i4,  i4,  i22
CMU.CPVI          i8,  v3.0        || LSU0.LDX.16     i4,  i2,  i4  || LSU1.CP i3,  v4.1  || VAU.ADD.f16 v11, v15, 0.5    || IAU.ADD  i22, i22, 0x01
CMU.CPVI          i9,  v3.1        || LSU0.LDX.16     i5,  i3,  i4  || LSU1.CP i2,  v4.2  || SAU.ADD.f16 i21, i21, i20    || IAU.ADD  i5,  i5,  i22
CMU.CPVI          i10, v3.2        || LSU0.LDX.16     i0,  i2,  i5  || LSU1.CP i3,  v4.3  || VAU.MUL.f16 v14, v16, v20    || IAU.ADD  i22, i22, 0x01
CMU.CPVI          i11, v3.3        || LSU0.LDX.16     i1,  i3,  i5  || LSU1.CP i2,  v5.0  || VAU.MUL.f16 v15, v17, v20    || IAU.ADD  i6,  i6,  i22
CMU.CPIVR.x16     v19, i21         || LSU0.LDX.16     i6,  i2,  i6  || LSU1.CP i3,  v5.1  || SAU.SWZ     i20, i21, [1032] || IAU.ADD  i22, i22, 0x01
CMU.CPIVR.x16     v18, i20         || LSU0.LDX.16     i7,  i3,  i6  || LSU1.CP i2,  v5.2  || VAU.MUL.f16 v1,  v19, v21    || IAU.ADD  i7,  i7,  i22
                                      LSU0.LDX.16     i0,  i2,  i7  || LSU1.CP i3,  v5.3  || VAU.MUL.f16 v0,  v18, v21    || IAU.ADD  i22, i22, 0x01
                                      LSU0.LDX.16     i1,  i3,  i7  || LSU1.CP i2,  v6.0  || VAU.ADD.f16 v8,  v8,  v9     || IAU.ADD  i8,  i8,  i22
                                      LSU0.LDX.16     i8,  i2,  i8  || LSU1.CP i3,  v6.1  || VAU.ADD.f16 v15, v15, v1     || IAU.ADD  i22, i22, 0x01
CMU.VSZM.BYTE     i4,  i0,  [10DD] || LSU0.LDX.16     i9,  i3,  i8  || LSU1.CP i2,  v6.2  || VAU.ADD.f16 v14, v14, v0     || IAU.ADD  i9,  i9,  i22
CMU.VSZM.BYTE     i5,  i1,  [10DD] || LSU0.LDX.16     i0,  i2,  i9  || LSU1.CP i3,  v6.3                                  || IAU.ADD  i22, i22, 0x01
CMU.CPVV.f16.i16s v13, v15         || LSU0.LDX.16     i1,  i3,  i9  || LSU1.CP i2,  v7.0                                  || IAU.ADD  i10, i10, i22
CMU.CPVV.f16.i16s v12, v14         || LSU0.LDX.16     i10, i2,  i10 || LSU1.CP i3,  v7.1                                  || IAU.ADD  i22, i22, 0x01
CMU.VSZM.BYTE     i6,  i0,  [10DD] || LSU0.LDX.16     i11, i3,  i10 || LSU1.CP i2,  v7.2                                  || IAU.ADD  i11, i11, i22
CMU.VSZM.BYTE     i7,  i1,  [10DD] || LSU0.LDX.16     i0,  i2,  i11 || LSU1.CP i3,  v7.3                                  || IAU.ADD  i22, i22, 0x01
CMU.CMZ.f16       v14              || LSU0.LDX.16     i1,  i3,  i11
CMU.CMZ.f16       v15                                               || LSU1.CP v4.0,  i4  || VAU.SUB.i16 v12, v12, 0x01   || PEU.PVV16 0x4
CMU.VSZM.BYTE     i8,  i0,  [10DD]                                  || LSU1.CP v6.0,  i5  || VAU.SUB.i16 v13, v13, 0x01   || PEU.PVV16 0x4
CMU.VSZM.BYTE     i9,  i1,  [10DD] || LSU0.ST.128.f16.u8f v8,  i18  || LSU1.CP v4.1,  i6
CMU.CPVV.i16.f16  v0,  v12                                          || LSU1.CP v6.1,  i7  || VAU.SUB.i16 v12, v12, 0x01
CMU.CPVV.i16.f16  v1,  v13                                          || LSU1.CP v5.0,  i8                                  || IAU.ADD  i18, i18, 0x08
CMU.VSZM.BYTE     i10, i0,  [10DD]                                  || LSU1.CP v7.0,  i9  || VAU.SHL.x16 v12, v12, 0x02
CMU.VSZM.BYTE     i11, i1,  [10DD]                                  || LSU1.CP v5.1,  i10 || VAU.SUB.f16 v14, v14, v0
                                                                       LSU1.CP v7.1,  i11 || VAU.SUB.f16 v15, v15, v1     || IAU.SUBSU i16, i16, 0x08
CMU.CPVV.u8f.f16  v4,  v4                                                                 || BRU.BRA MeshExpandLoop       || PEU.PCIX.NEQ 0
CMU.CPVV.u8f.f16  v5,  v5                                                                 || VAU.SUB.f16 v0,  v22, v10
CMU.CPVV.u8f.f16  v6,  v6                                           || LSU1.CP v16, v18   || VAU.SUB.f16 v1,  v22, v11
CMU.CPVV.u8f.f16  v7,  v7                                           || LSU1.CP v17, v19   || VAU.MUL.f16 v11, v10, v11
                                                                                             VAU.MUL.f16 v9,  v0,  v11
CMU.VDILV.x16 v5,  v4,  v4,  v5                                                           || VAU.MUL.f16 v10, v10, v1
CMU.VDILV.x16 v7,  v6,  v6,  v7                                                           || VAU.MUL.f16 v8,  v0,  v1

// landing
VAU.MUL.f16 v11, v11, v7
VAU.MUL.f16 v10, v10, v6
VAU.MUL.f16 v9,  v9,  v5
VAU.MUL.f16 v8,  v8,  v4
nop
VAU.ADD.f16 v9,  v9,  v11
VAU.ADD.f16 v8,  v8,  v10 || BRU.JMP i30
nop
CMU.CPVI i20, v23.0
VAU.ADD.f16 v8,  v8,  v9
CMU.CPVI i21, v23.1
CMU.CPVI i22, v23.2
LSU0.ST.128.f16.u8f v8,  i18
MeshExpandExit:
BRU.SWIH 0x1F
nop
nop
nop
nop
nop
nop
.size mvcvMeshExpand_asm,.-mvcvMeshExpand_asm
.nowarnend
.end
