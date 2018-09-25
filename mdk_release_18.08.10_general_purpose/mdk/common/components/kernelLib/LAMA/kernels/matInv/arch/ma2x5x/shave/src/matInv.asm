.type mvcvMatInverseLU_asm,@function
.version 00.50.02
// /* matrix definition */
.set MAT_M_OFFSET_U32     0
.set MAT_N_OFFSET_U32     4
.set MAT_MAX_M_U32        8
.set MAT_MAX_N_U32       12
.set MAT_MAX_SIZE_U32    16
.set MAT_ME_PTR          20
.set MAT_BASE_PTR        24

///* matrix inverse LU decomposition */
//MAT	*m_inv_lu(const MAT *A, MAT *out)
// i19: stack
// i18: pointer to in
// i17: pointer to out
.code
.lalign
mvcvMatInverseLU_asm:
.nowarn
CMU.CPIV.x32 v13.0, i20 || LSU0.LDO.32  i15, i17, MAT_N_OFFSET_U32
CMU.CPIV.x32 v13.1, i21 || LSU0.LDO.32  i16, i17, MAT_BASE_PTR
CMU.CPIV.x32 v13.2, i22 || LSU0.LDO.32  i17, i18, MAT_BASE_PTR
CMU.CPIV.x32 v13.3, i23
CMU.CPIV.x32 v15.2, i30 || IAU.FEXTU   i0,  i19, 0x00, 0x03
CMU.CPIV.x32 v15.3, i19 || IAU.SUBSU   i19, i19, i0
CMU.CPII    i18, i17
                           IAU.SHL     i15, i15, 0x04  || SAU.MUL.i32 i0,  i15, i15
						   IAU.SUBSU   i19, i19, i15
CMU.CPII    i23, i19    || IAU.SUBSU   i19, i19, i15
CMU.CPII    i22, i19    || IAU.SUBSU   i19, i19, i15   || SAU.SHL.x32 i0,  i0,  0x02
CMU.CPII    i21, i19    || IAU.SHR.u32 i15, i15, 0x02
                           IAU.SUBSU   i20, i19, i0    || SAU.SUB.i32 i19, i19, i0
// i23: p pivot vector address
// i22: y forward substitution vector address
// i21: x backward substitution vector address
// i20: lu.base
// i19: stack
// i18: out address
// i17: a.base
// i16: out.base
// i15: N*4

//----------------------------
// i14: k=(0//N-1)*4 outer loop
// i13: (N-k) search loop
// i12: P[kmax] address
// i11: P[kmax] value
// i10: P[k] address
// i9:  P[k] value
// i8:  P[i] address
// i7:  P[i] value
// i5:  read address
// i4:  max
// i3:  t=A[P[i]][k]
// i2:  0x7FFFFFFF
// i1:  __m_pivot_lu_copy
// i0: jump pointer
.lalign
__m_inverse_pivot:
CMU.CPZV v0,  0            || IAU.ADDSU   i0,  i23, 0    || LSU1.LDIL i1,  0x01
CMU.CPIV.x32 v0.1, i1      || IAU.SHR.u32 i1,  i15, 0x03 || LSU1.LDIL i14, 0
VAU.ADD.i32 v0,  v0,  0x02 || IAU.ADDSU   i1,  i1,  0x01
VAU.ADD.i32 v0,  v0,  0x04 || LSU0.STI.64.l v0,  i0      || BRU.RPI i1
__m_inverse_pivot_pre:
IAU.ADDSU  i8,  i14, i23   || LSU1.LDIL   i30, __m_inverse_pivot_post
LSU0.LDI.32 i7,  i8        || LSU1.LDIH   i30, __m_inverse_pivot_post
IAU.ADDSU  i10, i14, i23   || LSU1.LDIL   i0,  __m_inverse_pivot_loop
LSU0.LD.32 i9,  i10        || LSU1.LDIH   i0,  __m_inverse_pivot_loop
IAU.XOR    i4,  i4,  i4    || LSU1.LDIL   i1,  __m_inverse_pivot_copy
SAU.SUB.f32  i4,  i4, 1.0  || LSU1.LDIH   i1,  __m_inverse_pivot_copy
IAU.SUBSU i13, i15, i14    || LSU1.LDIL   i2, 0xFFFF
IAU.SHR.u32 i13, i13, 0x02 || LSU1.LDIH   i2, 0x7FFF
.lalign
__m_inverse_pivot_loop:
IAU.MULSU i5,  i7,  i15
nop
IAU.ADDSU i5,  i5,  i17
IAU.ADDSU i5,  i5,  i14
LSU0.LD.32 i3,  i5
IAU.SUBSU i13, i13, 0x01
PEU.PC1I 0x1 || CMU.CPII i0, i1
LSU0.LDI.32 i7,  i8 || BRU.JMP i0
nop
nop
nop
IAU.AND      i3,  i3,  i2
CMU.CMII.f32 i3,  i4
PEU.PC1C 0x2 || CMU.CPII i4,  i3 || IAU.SUBSU i12, i8, 0x08 || SAU.AND i11, i7, i7
.lalign
__m_inverse_pivot_post:
IAU.ADDSU i14, i14, 0x04
CMU.CMII.u32 i14, i15
PEU.PC1C 0x4 || BRU.BRA __m_inverse_pivot_pre
LSU0.ST.32 i9,  i12
LSU0.ST.32 i11, i10
nop
nop
nop
nop

//--------------------
// i14: k=(0//N-2)*4 outer loop
// i13: i=(k+1//N-1)*4 column loop count
// i12: j=(N-k)/4 row loop count
// i11: address for LU[k][k]
// i10: address for LU[i][k]
// i9:  address for LU[i][j]
// i8:  address for LU[k][j]
// i7:  increment, 16
// i6:  write LU[i][j]
// i5:  limit
// v0:  LU[k][k]
// v1:  LU[i][k]
// v2:  LU[k][j]
// v3:  LU[i][j]
// v4:  LU[i][j] -= LU[i][k]*LU[k][j]
// v5:
.lalign
__m_inverse_LUdecompose_pre:
CMU.CPII  i11, i20       || LSU1.LDIL i14, 0
__m_inverse_LUdecompose_loop:
__m_inverse_LUdecompose_col_pre:
IAU.ADDSU i13, i14, 0x04 || LSU0.LD.32R v0,  i11
IAU.ADDSU i10, i11, i15
.lalign
__m_inverse_LUdecompose_col_loop:
__m_inverse_LUdecompose_row_pre:
LSU0.LD.32  i1,  i10
LSU1.LDIL i7, 0x10
IAU.ADDSU   i9,  i10, 0x04
IAU.ADDSU   i8,  i11, 0x04
CMU.CPII     i6,  i9   || LSU0.LDXVI  v3,  i9,  i7
CMU.CPVI.x32 i0,  v0.0
LSU0.LDXVI  v2,  i8,  i7
SAU.DIV.f32  i1,  i1,  i0
IAU.SUBSU    i12, i15, i14
IAU.SHR.u32  i12, i12, 0x02
IAU.SUBSU    i5,  i12, 0x01
IAU.SUBSU    i12, i12, 0x02
IAU.SHR.u32  i12, i12, 0x02
PEU.PC1I 0x1 || BRU.BRA __m_inverse_LUdecompose_row_post || IAU.ADDSU i13, i13, 0x04
IAU.FEXTU    i5,  i5, 0x00, 0x02
PEU.PC1I 0x1 || LSU1.LDIL i5, 0x04
CMU.VSZM.BYTE i5,  i5, [0000]
LSU1.LDIL i4, 0x0100
LSU1.LDIH i4, 0x0302
CMU.CPIVR.x32 v1,  i1     || LSU0.ST.32 i1,  i10
.lalign
__m_inverse_LUdecompose_row_loop:
VAU.MUL.f32 v4,  v2,  v1  || LSU0.LDXVI  v2,  i8,  i7 || IAU.SUBSU i12, i12, 0x01
PEU.PC1I 0x2 || BRU.BRA __m_inverse_LUdecompose_row_loop
PEU.PC1I 0x1 || IAU.ADDSU i13, i13, 0x04
VAU.SUB.f32 v4,  v3,  v4  || LSU0.LDXVI  v3,  i9,  i7
nop
nop
                             LSU0.STXVI  v4,  i6,  i7
nop
__m_inverse_LUdecompose_row_post:
VAU.MUL.f32 v4,  v2,  v1  || CMU.CMII.u32 i13, i15
PEU.PC1C 0x4 || BRU.BRA __m_inverse_LUdecompose_col_loop
IAU.ADDSU i10, i10, i15
VAU.SUB.f32 v4,  v3,  v4
nop
CMU.CMII.u8 i4, i5
LSU0.STXV   v4,  i6  || PEU.PVL032 0x4
nop
__m_inverse_LUdecompose_col_post:
__m_inverse_LUdecompose_post:
IAU.ADDSU i14, i14, 0x08
CMU.CMII.u32 i14, i15 || IAU.SUBSU i14, i14, 0x04
PEU.PC1C 0x4 || BRU.BRA __m_inverse_LUdecompose_loop
IAU.ADDSU i11, i11, i15
IAU.ADDSU i11, i11, 0x04
nop
nop
nop
nop

// i14: k=(0//N-1)*4 outer loop
// i13: LU current address
// i12: LU reciprocal of diagonal value
// i11: 1.0f
// i10: out.base+(k*N+k)*4 - diagonal address
// i9:  LUDinv address
.lalign
__m_inverse_init_pre:
// i3:  1.0f
CMU.CPII    i13, i20      || IAU.ADDSU   i9,  i23, i15        || SAU.MUL.i32 i2,  i15, i15
CMU.CPII    i10, i16      || IAU.ADDSU   i9,  i9,  i15        || LSU1.LDIL i11, 0x01
CMU.CPII    i0,  i16      || LSU0.LDI.32 i12,  i13            || LSU1.LDIL i4,  0x0100
CMU.CPII.i32.f32 i11, i11 || IAU.FEXTU   i3,  i2,  0x04, 0x02 || LSU1.LDIH i4,  0x0302
PEU.PC1I 0x1   || LSU1.LDIL i3,  0x04
CMU.VSZM.BYTE i3,  i3,  [0000] || IAU.SUBSU   i2,  i2,  0x10
CMU.CMII.u8   i4,  i3          || IAU.SHR.u32 i2,  i2,  0x06  || VAU.XOR v0,  v0,  v0
PEU.PC1I 0x2   || BRU.RPI i2   || IAU.ADDSU i0,  i0,  0x10 || LSU0.ST.64.l v0,  i0 || LSU1.STO.64.h v0,  i0,  0x08
PEU.PVL032 0x4 || LSU0.STXV v0,  i0 || LSU1.LDIL i14, 0
.lalign
__m_inverse_init_loop:
SAU.DIV.f32 i12, i11, i12
LSU0.STI.32 i11, i10
IAU.ADDSU   i14, i14, 0x04
IAU.ADDSU   i13, i13, i15
IAU.ADDSU   i10, i10, i15
CMU.CMII.u32 i14, i15
PEU.PC1C 0x4 || BRU.BRA __m_inverse_init_loop || LSU0.LDI.32 i12, i13
nop
nop
nop
nop
nop
IAU.ADDSU   i9,  i9,  0x04 || LSU0.ST.32 i12, i9
__m_inverse_init_post:
__m_inverse_LUinverse_pre:
// i14: i*4
// i13: n*4
// i12: address LU[n][0]
// i11: current LU[n][m]
// i10: address Y4[n*4]
// i9:  current address Y4[m*4]
// i8:  address P[n]
// i7:  pivot value
// i6:  offset
// v4:  Y4[n*4+idx]
// v3:  t[idx]
// v2:  LU[n][m]*Y4[m*4+idx]
// v1:  LU[n][m]
// v0:  Y4[m*4+idx]
CMU.CPII i12, i20 || IAU.ADDSU i8,  i23, 0  || LSU1.LDIL i14, 0
CMU.CPII i13, i15 || IAU.ADDSU i10, i22, 0
__m_inverse_LUinverse_loop:
__m_inverse_LUinverse_forward_pre:
CMU.CMII.u32 i22, i10 || IAU.ADDSU i11, i12, 0  || LSU0.LDI.32 i7,  i8
PEU.PC1C 0x3 || BRU.BRA __m_inverse_LUinverse_forward_post
PEU.PC1C 0x4 || LSU0.LDI.32R v1,  i11
PEU.PC1C 0x4 || LSU0.LDXV v0,  i22
VAU.XOR  v3,  v3,  v3
nop
IAU.ADDSU  i6,  i16, i14
IAU.ADDSU  i9,  i22, 0x10
CMU.CMII.u32 i9, i10
.lalign
__m_inverse_LUinverse_forward_loop:
PEU.PC1C 0x4 || LSU0.LDXV v0,  i9
PEU.PC1C 0x4 || BRU.BRA __m_inverse_LUinverse_forward_loop
VAU.MUL.f32 v2,  v1,  v0 || LSU0.LDI.32R v1,  i11
nop
nop
VAU.ADD.f32 v3,  v3,  v2
IAU.ADDSU  i9,  i9,  0x10
CMU.CMII.u32 i9, i10
__m_inverse_LUinverse_forward_post:
IAU.MULSU  i7,  i7,  i15
nop
IAU.ADDSU  i7,  i7,  i6
// damn slow due to pivoting row redirection
LSU0.LDXV  v4,  i7
nop
nop
nop
IAU.ADDSU i12, i12, i15
IAU.SUBSU i13, i13, 0x04
PEU.PC1I 0x2 || BRU.BRA __m_inverse_LUinverse_forward_pre
nop
VAU.SUB.f32 v4,  v4,  v3
nop
nop
LSU0.STXV   v4,  i10
IAU.ADDSU   i10, i10, 0x10
// i14: i*4
// i13: n*4
// i12: address LU[n][0] ok
// i11: current LU[n][m]
// i10: address Y4[n*4] ok
// i9:  current address Y4[m*4]
// i8:  address LUDinv
// i7:  LUDinv value
// i6:  offset
// i5:  address X4[n*4]
// i4:  current address X4[m*4]
CMU.CPII i13, i15 || IAU.SHL   i0,  i15, 0x02
                     IAU.ADDSU i5,  i21, i0
					 IAU.SUBSU i5,  i5,  0x10
CMU.CPII i6,  i5  || IAU.ADDSU i8,  i23, i0
                     IAU.SUBSU i8,  i8,  i15
                     IAU.SUBSU i9,  i10, 0x10

__m_inverse_LUinverse_backward_pre:
CMU.CMII.u32 i5,  i6 || IAU.SUBSU i11, i12, 0x04 || LSU0.LDXV v4,  i9
PEU.PC1C 0x3 || BRU.BRA __m_inverse_LUinverse_backward_post
PEU.PC1C 0x4 || LSU0.LD.32R v1,  i11
PEU.PC1C 0x4 || LSU0.LDXV v0,  i6
IAU.SUBSU i8,  i8, 0x04 || VAU.XOR  v3,  v3,  v3
IAU.SUBSU i9,  i9, 0x10 || LSU0.LD.32R v5,  i8
IAU.SUBSU i4,  i6,  0x10
IAU.SUBSU i11, i11, 0x04
CMU.CMII.u32 i5,  i4
.lalign
__m_inverse_LUinverse_backward_loop:
PEU.PC1C 0x4 || LSU0.LDXV v0,  i4
PEU.PC1C 0x4 || BRU.BRA __m_inverse_LUinverse_backward_loop
VAU.MUL.f32 v2,  v1,  v0 || LSU0.LD.32R v1,  i11
nop
IAU.SUBSU  i4,  i4,  0x10
VAU.ADD.f32 v3,  v3,  v2
IAU.SUBSU i11, i11, 0x04
CMU.CMII.u32 i5, i4
__m_inverse_LUinverse_backward_post:
IAU.SUBSU i12, i12, i15 || VAU.SUB.f32 v4,  v4,  v3
IAU.SUBSU i13, i13, 0x04
PEU.PC1I 0x2 || BRU.BRA __m_inverse_LUinverse_backward_pre
nop
VAU.MUL.f32 v4,  v4,  v5
nop
nop
LSU0.STXV   v4,  i5
IAU.SUBSU   i5,  i5,  0x10

.lalign
__m_inverse_LUinverse_writeback_pre:
CMU.CPII i1,  i21 || IAU.ADDSU i0,  i14, i16 || LSU1.LDIL i3, 0x0100
CMU.CPII i13, i15 || LSU0.LDXV v1,  i1       || LSU1.LDIH i3, 0x0302
IAU.SUBSU i2,  i15, i14
IAU.SHR.u32 i2,  i2,  0x02
CMU.VSZM.BYTE i2,  i2, [0000] || IAU.ADDSU i1,  i1,  0x10
CMU.CMII.u8   i3,  i2         || IAU.SUBSU i13, i13, 0x04
.lalign
__m_inverse_LUinverse_writeback_loop:
PEU.PC1I 0x2 || BRU.BRA __m_inverse_LUinverse_writeback_loop
LSU0.LDXV v1,  i1
IAU.ADDSU i1,  i1,  0x10
LSU0.STXV v1,  i0  || PEU.PVL032 0x4
IAU.ADDSU i0,  i0,  i15
nop
IAU.SUBSU i13, i13, 0x04
.lalign
__m_inverse_LUinverse_writeback_post:
__m_inverse_LUinverse_post:
IAU.ADDSU i14, i14, 0x10
CMU.CMII.u32 i14, i15
PEU.PC1C 0x4 || BRU.BRA __m_inverse_LUinverse_loop
CMU.CPII i12, i20 || IAU.ADDSU i8,  i23, 0
CMU.CPII i13, i15 || IAU.ADDSU i10, i22, 0
nop
nop
nop
nop

// exit function
CMU.CPVI.x32 i30, v15.2
CMU.CPVI.x32 i19, v15.3 || BRU.JMP i30
CMU.CPVI.x32 i20, v13.0
CMU.CPVI.x32 i21, v13.1
CMU.CPVI.x32 i22, v13.2
CMU.CPVI.x32 i23, v13.3
nop
nop


// i15: N*4
// i6:  16
// i5:  LU destination address
// i4:  A source address
// i3:  {4,4,4,4}
// i2:  {3,2,1,0}
// i1:  (N%16)?(N%)
// i0:  (N-1)/16
// a lazy copy
.lalign
__m_inverse_pivot_copy:
CMU.CPII i4,  i17     || IAU.SHR.u32 i0,  i14, 0x02 || SAU.MUL.i32 i2,  i11, i15
CMU.CPII i5,  i20     || IAU.SHR.u32 i0,  i15, 0x02 || SAU.MUL.i32 i3,  i0,  i15
                         IAU.SUBSU   i0,  i0,  0x01 || LSU1.LDIL i6,  0x10
                         IAU.ADDSU   i4,  i4,  i2
                         IAU.ADDSU   i5,  i5,  i3
__m_pivot_lu_copy_pre:
LSU0.LDXVI v0,  i4,  i6  || IAU.SHR.u32 i0,  i0,  0x04
LSU1.LDIL i2,  0x0100    || BRU.BRA __m_pivot_lu_copy_post || PEU.PCIX.EQ  0x0
LSU0.LDXVI v1,  i4,  i6
LSU1.LDIH i2,  0x0302    || IAU.FEXTU     i1,  i15,  0x02, 0x04
LSU0.LDXVI v2,  i4,  i6  || IAU.ADDSU     i1,  i1,  0x10   || PEU.PCIX.EQ  0x1
LSU1.LDIL i3,  0x0404    || CMU.VSZM.BYTE i1,  i1, [0000]
LSU0.LDXVI v3,  i4,  i6
LSU1.LDIH i3,  0x0404
.lalign
__m_pivot_lu_copy_loop:
LSU0.LDXVI v0,  i4,  i6  || IAU.SUBSU i0,  i0,  0x01
LSU1.STXVI v0,  i5,  i6  || BRU.BRA __m_pivot_lu_copy_loop || PEU.PCIX.NEQ 0x0
LSU0.LDXVI v1,  i4,  i6
LSU1.STXVI v1,  i5,  i6
LSU0.LDXVI v2,  i4,  i6
LSU1.STXVI v2,  i5,  i6
LSU0.LDXVI v3,  i4,  i6
LSU1.STXVI v3,  i5,  i6
__m_pivot_lu_copy_post:
CMU.CMII.i8 i2,  i1      || IAU.ADDSU i2,  i2,  i3
LSU1.STXVI v0,  i5,  i6  || PEU.PVL132 0x4
CMU.CMII.i8 i2,  i1      || IAU.ADDSU i2,  i2,  i3  || BRU.JMP i30
LSU1.STXVI v1,  i5,  i6  || PEU.PVL132 0x4
CMU.CMII.i8 i2,  i1      || IAU.ADDSU i2,  i2,  i3
LSU1.STXVI v2,  i5,  i6  || PEU.PVL132 0x4
CMU.CMII.i8 i2,  i1
LSU1.STXV  v3,  i5       || PEU.PVL132 0x4
nop
.nowarnend
.size mvcvMatInverseLU_asm,.-mvcvMatInverseLU_asm
.end
