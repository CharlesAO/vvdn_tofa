.type mvcvMatAdd_asm,@function
.version 00.50.02
// /* matrix definition */
.set MAT_M_OFFSET_U32     0
.set MAT_N_OFFSET_U32     4
.set MAT_MAX_M_U32        8
.set MAT_MAX_N_U32       12
.set MAT_MAX_SIZE_U32    16
.set MAT_ME_PTR          20
.set MAT_BASE_PTR        24

// /* matrix add matrix */
// MAT *m_add(const MAT *A, const MAT *B, MAT *out)
// i18: pointer to A
// i17: pointer to B
// i16: pointer to out
.code
.lalign
mvcvMatAdd_asm:
.nowarn 10 40
LSU0.LDO.32 i11, i16, MAT_N_OFFSET_U32 || LSU1.LDIL i4, 0x0100
LSU0.LDO.32 i12, i16, MAT_M_OFFSET_U32 || LSU1.LDIH i4, 0x0302
LSU0.LDO.32 i15, i18, MAT_BASE_PTR     || LSU1.LDIL i7, 0x0404
LSU0.LDO.32 i14, i17, MAT_BASE_PTR     || LSU1.LDIH i7, 0x0404
LSU0.LDO.32 i13, i16, MAT_BASE_PTR
IAU.ADDSU i5,  i4,  i7                || LSU1.LDIL i0, __m_add_loop
IAU.ADDSU i6,  i5,  i7                || LSU1.LDIH i0, __m_add_loop
IAU.ADDSU i7,  i6,  i7                || LSU1.LDIL i1, __m_add_post
IAU.MULSU i10, i11, i12               || LSU1.LDIH i1, __m_add_post
LSU1.LDIL i2, 0x10
IAU.SUBSU   i10, i10, 0x01
// enter loop
__m_add_pre:
LSU0.LDXVI v0,  i15, i2  || IAU.SHR.u32 i9,  i10, 0x04
LSU1.LDXVI v4,  i14, i2  || IAU.ADDSU i0,  i1,  0     || PEU.PCIX.EQ 0x1
LSU0.LDXVI v1,  i15, i2
LSU1.LDXVI v5,  i14, i2  || IAU.FEXTU   i8,  i10, 0x00, 0x04 || BRU.JMP i0
LSU0.LDXVI v2,  i15, i2  || SAU.SWZ     i8,  i8,  [0000]
LSU1.LDXVI v6,  i14, i2
LSU0.LDXVI v3,  i15, i2
LSU1.LDXVI v7,  i14, i2
nop
VAU.ADD.f32 v8,  v4,  v0
// i15: a current address
// i14: b current address
// i13: out current address
// i12: out.m
// i11: out.n
// i10: out.m*out.n-1
// i9:  (out.m*out.n-1)/16
// i8:  (out.m*out.n-1)%16
// i7:  12,13,14,15
// i6:  8,9,10,11
// i5:  4,5,6,7
// i4:  0,1,2,3
// i2: 16
// i1: __m_add_post
// i0: __m_add_loop
.lalign
__m_add_loop:
LSU0.LDXVI v0,  i15, i2
LSU1.LDXVI v4,  i14, i2
LSU0.STXVI v8,  i13, i2  || VAU.ADD.f32 v9,  v5,  v1
LSU1.LDXVI v1,  i15, i2  || IAU.SUBSU i9,  i9,  0x01
LSU0.LDXVI v5,  i14, i2  || IAU.ADDSU i0,  i1,  0     || PEU.PCIX.EQ 0x1
LSU1.STXVI v9,  i13, i2  || VAU.ADD.f32 v10, v6,  v2  || BRU.JMP i0
LSU0.LDXVI v2,  i15, i2
LSU1.LDXVI v6,  i14, i2
LSU0.STXVI v10, i13, i2  || VAU.ADD.f32 v11, v7,  v3
LSU1.LDXVI v3,  i15, i2
LSU0.LDXVI v7,  i14, i2
LSU1.STXVI v11, i13, i2  || VAU.ADD.f32 v8,  v4,  v0
.lalign
__m_add_post:
CMU.CMII.i8 i8,  i4
VAU.ADD.f32 v9,  v5,  v1
LSU0.STXVI v8,  i13, i2  || PEU.PVL032 0x3
CMU.CMII.i8 i8,  i5
VAU.ADD.f32 v10, v6,  v2 || BRU.JMP i30
LSU0.STXVI v9,  i13, i2  || PEU.PVL032 0x3
CMU.CMII.i8 i8,  i6      || VAU.ADD.f32 v11, v7,  v3
LSU0.STXVI v10, i13, i2  || PEU.PVL032 0x3
CMU.CMII.i8 i8,  i7
LSU0.STXVI v11, i13, i2  || PEU.PVL032 0x3
IAU.ADDSU  i18, i16, 0
.nowarnend
.size mvcvMatAdd_asm,.-mvcvMatAdd_asm
.end
