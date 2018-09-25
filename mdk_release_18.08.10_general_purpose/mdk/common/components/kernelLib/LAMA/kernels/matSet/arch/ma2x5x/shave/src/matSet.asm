// /* matrix definition */
.set MAT_M_OFFSET_U32     0
.set MAT_N_OFFSET_U32     4
.set MAT_MAX_M_U32        8
.set MAT_MAX_N_U32       12
.set MAT_MAX_SIZE_U32    16
.set MAT_ME_PTR          20
.set MAT_BASE_PTR        24

.type mvcvMatSet_asm,@function
.version 00.50.02

.code .text.mvcvMatSet_asm
.align 16
//
.nowarn
/////* set mat sub block to scalar */
//MAT	*m_set_sub(Real scalar, MAT *in, int start_row, int start_col, int length_row, int length_col)
// i18: scalar
// i17: pointer to in
// i16: start row
// i15: start column
// i14: length row
// i13: length column
.code
.lalign
mvcvMatSet_asm:
LSU0.LDO.32 i15, i17, MAT_BASE_PTR                                    || LSU1.LDIL i4, 0x0000
LSU0.LDO.32 i13, i17, MAT_N_OFFSET_U32 || SAU.SHL.x32 i1,  i15, 0x02  || LSU1.LDIH i4, 0x0001
IAU.FEXTU   i3,  i13, 0x00, 0x01       || SAU.ADD.i16 i5,  i4,  0x02  || LSU1.LDIL i2, 0x10
IAU.SUBSU   i3,  i2,  i3               || SAU.ADD.i16 i6,  i4,  0x04  || LSU1.LDIL i0, __m_set_sub_loop
IAU.SUBSU   i2,  i13, 0x10             || SAU.ADD.i16 i7,  i4,  0x06  || LSU1.LDIH i0, __m_set_sub_loop
CMU.CPIVR.x32 v0,  i18                 || SAU.ADD.i16 i8,  i4,  0x08  || LSU1.LDIL i0, __m_set_sub_denat_loop || PEU.PCIX.EQ 0x21
IAU.ADDSU   i3,  i13, 0                || SAU.ADD.i16 i9,  i4,  0x0A  || LSU1.LDIH i0, __m_set_sub_denat_loop || PEU.PCIX.EQ 0x21
IAU.ADDSU   i2,  i2,  0x03             || SAU.ADD.i16 i10, i4,  0x0C  || BRU.JMP i0
IAU.SHL     i13, i13, 0x02             || SAU.ADD.i16 i11, i4,  0x0E
IAU.MUL     i16, i16, i13              || SAU.SHR.u32 i2,  i2,  0x01
CMU.VSZM.BYTE  i3,  i3,  [1010]        || SAU.ADD.i32 i18, i17, 0
IAU.ADDSU   i15, i15, i16
IAU.ADDSU   i15, i15, i1
IAU.SUBSU   i14, i14, 0x01             || SAU.ADD.i32 i1,  i15, 0

// i18: pointer to in
// i15: in current row address
// i14: length row
// i13: in.n*4
// i11: 14, 15
// i10: 12, 13
// i9: 10, 11
// i8: 8,9
// i7: 6,7
// i6: 4,5
// i5: 2,3
// i4: 0,1
// i3: limit: (length col>16)?(16-(length col%2)):length col
// i2: repeat: (sat_sub(length col-16)+3)/2
// i1: in current address
// i0: __m_set_sub_loop
.lalign
__m_set_sub_loop:
                                                   PEU.PCIX.EQ 0x1 || IAU.ADDSU i0,  i30, 0
LSU0.STI.64.l v0,  i1                           || BRU.RPI i2
LSU0.STI.64.l v0,  i1                           || BRU.JMP i0
LSU0.STI.64.l v0,  i1
LSU0.STI.64.l v0,  i1                                              || SAU.ADD.i32 i15, i15, i13
LSU0.STI.64.l v0,  i1
LSU0.STI.64.l v0,  i1
LSU0.STI.64.l v0,  i1  || CMU.CMII.i16 i3,  i11                    || SAU.MUL.i32 i1,  i15, 0x01
LSU0.ST.64.l  v0,  i1                           || PEU.PVL032  0x2 || IAU.SUBSU i14, i14, 0x01

.lalign
__m_set_sub_denat_loop:
                          CMU.CMII.i16 i3,  i4  || PEU.PCIX.EQ 0x1 || IAU.ADDSU i0,  i30, 0
LSU0.STI.64.l v0,  i1  || CMU.CMII.i16 i3,  i5  || PEU.PVL032  0x2
LSU0.STI.64.l v0,  i1  || CMU.CMII.i16 i3,  i6  || PEU.PVL032  0x2 || BRU.JMP i0
LSU0.STI.64.l v0,  i1  || CMU.CMII.i16 i3,  i7  || PEU.PVL032  0x2
LSU0.STI.64.l v0,  i1  || CMU.CMII.i16 i3,  i8  || PEU.PVL032  0x2
LSU0.STI.64.l v0,  i1  || CMU.CMII.i16 i3,  i9  || PEU.PVL032  0x2 || SAU.ADD.i32 i15, i15, i13
LSU0.STI.64.l v0,  i1  || CMU.CMII.i16 i3,  i10 || PEU.PVL032  0x2
LSU0.STI.64.l v0,  i1  || CMU.CMII.i16 i3,  i11 || PEU.PVL032  0x2 || SAU.MUL.i32 i1,  i15, 0x01
LSU0.ST.64.l  v0,  i1                           || PEU.PVL032  0x2 || IAU.SUBSU i14, i14, 0x01
.size mvcvMatSet_asm,.-mvcvMatSet_asm
.nowarnend
.end
