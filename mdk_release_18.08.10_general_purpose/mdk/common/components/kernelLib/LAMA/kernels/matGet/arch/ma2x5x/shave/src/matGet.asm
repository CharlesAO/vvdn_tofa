// /* matrix definition */
.set MAT_M_OFFSET_U32     0
.set MAT_N_OFFSET_U32     4
.set MAT_MAX_M_U32        8
.set MAT_MAX_N_U32       12
.set MAT_MAX_SIZE_U32    16
.set MAT_ME_PTR          20
.set MAT_BASE_PTR        24

.type mvcvMatGet_asm,@function
.version 00.50.02

///* get mat sub block */
//MAT	*m_get_sub(const MAT *in, MAT *out, int start_row, int start_col)
// i18: pointer to in
// i17: pointer to out
// i16: start row
// i15: start column
.code
.lalign
mvcvMatGet_asm:
LSU0.LDO.32 i14, i18, MAT_N_OFFSET_U32 || LSU1.LDIL i5, 0x0002
LSU0.LDO.32 i13, i17, MAT_N_OFFSET_U32 || LSU1.LDIH i5, 0x0003
LSU0.LDO.32 i12, i17, MAT_M_OFFSET_U32 || SAU.ADD.i16 i6,  i5,  0x02
LSU0.LDO.32 i16, i18, MAT_BASE_PTR     || SAU.ADD.i16 i7,  i5,  0x04
LSU0.LDO.32 i15, i17, MAT_BASE_PTR     || SAU.ADD.i16 i8,  i5,  0x06
IAU.ADDSU   i18, i17, 0                || SAU.ADD.i16 i9,  i5,  0x08 || LSU1.LDIL i0,  __m_get_sub_loop
IAU.SHL     i2,  i15, 0x02             || SAU.ADD.i16 i10, i5,  0x0A || LSU1.LDIH i0,  __m_get_sub_loop
IAU.SHL     i14, i14, 0x02             || SAU.ADD.i16 i11, i5,  0x0C
IAU.FEXTU   i4,  i13, 0x00, 0x01       || SAU.MUL.i32 i1,  i14, i16  || LSU1.LDIL i3, 0x10
IAU.SUBSU   i4,  i3,  i4
IAU.SUBSU   i3,  i13, 0x10             || SAU.SHL.x32 i13, i13, 0x02
IAU.ADDSU   i4,  i13, 0                || SAU.ADD.i32 i16, i16, i1   || PEU.PCIX.EQ 0x1
IAU.ADDSU   i3,  i3,  0x03             || CMU.VSZM.BYTE i4,  i4, [1010]
IAU.ADDSU   i16, i16, i2               || SAU.SHR.u32 i3,  i3,  0x01
IAU.ADDSU   i2,  i16, 0                || CMU.CMII.i16 i4,  i5
// i18: pointer to out
// i16: in current address
// i15: out current address
// i14: in.n*4
// i13: out.n*4
// i12: out.m
// i11: 14, 15
// i10: 12, 13
// i9: 10, 11
// i8: 8,9
// i7: 6,7
// i6: 4,5
// i5: 2,3
// i4: limit: (out.n>16)?(16-(out.n%2)):out.n
// i3: repeat: (sat_sub(out.n-16)+3)/2
// i2: in current address
// i1: out current address
// i0: __m_get_sub_loop
.lalign
__m_get_sub_loop:
LSU1.LDI.64.l v0,  i2                                                                                             || IAU.SUBSU i12, i12, 0x01
LSU1.LDI.64.l v0,  i2                                                                         || PEU.PCIX.EQ 0x1  || IAU.ADDSU i0,  i30, 0
LSU1.LDI.64.l v0,  i2
LSU1.LDI.64.l v0,  i2
LSU1.LDI.64.l v0,  i2
LSU1.LDI.64.l v0,  i2                                                                                             || IAU.ADDSU i1,  i15, 0
LSU1.LDI.64.l v0,  i2                                                                                             || IAU.ADDSU i15, i15, i13
LSU1.LDI.64.l v0,  i2  || LSU0.STI.64.l v0,  i1                                               || BRU.RPI i3
                          LSU0.STI.64.l v0,  i1  || CMU.CMII.i16 i4,  i6  || PEU.PVL032 0x2   || BRU.JMP i0
                          LSU0.STI.64.l v0,  i1  || CMU.CMII.i16 i4,  i7  || PEU.PVL032 0x2
                          LSU0.STI.64.l v0,  i1  || CMU.CMII.i16 i4,  i8  || PEU.PVL032 0x2
                          LSU0.STI.64.l v0,  i1  || CMU.CMII.i16 i4,  i9  || PEU.PVL032 0x2
                          LSU0.STI.64.l v0,  i1  || CMU.CMII.i16 i4,  i10 || PEU.PVL032 0x2
                          LSU0.STI.64.l v0,  i1  || CMU.CMII.i16 i4,  i11 || PEU.PVL032 0x2                       || IAU.ADDSU i16, i16, i14
                          LSU0.ST.64.l  v0,  i1  || CMU.CMII.i16 i4,  i5  || PEU.PVL032 0x2                       || IAU.ADDSU i2,  i16, 0
.size mvcvMatGet_asm,.-mvcvMatGet_asm
.end
