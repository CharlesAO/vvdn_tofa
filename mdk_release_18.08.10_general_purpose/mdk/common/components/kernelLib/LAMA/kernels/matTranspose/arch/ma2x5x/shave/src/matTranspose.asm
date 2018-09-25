.type mvcvMatTranspose_asm,@function
.version 0.50

// /* matrix definition */
.set MAT_M_OFFSET_U32     0
.set MAT_N_OFFSET_U32     4
.set MAT_MAX_M_U32        8	
.set MAT_MAX_N_U32       12
.set MAT_MAX_SIZE_U32    16
.set MAT_ME_PTR          20
.set MAT_BASE_PTR        24

.code .text.mvcvMatTranspose

.lalign
//void mvcvMatTranspose(half* input, half* output, u32 num_lines, u32 line_len)
//                         i18         i17             i16            i15
mvcvMatTranspose_asm:
.nowarn 40
LSU0.LDO.32 i5,  i17, MAT_N_OFFSET_U32
LSU0.LDO.32 i6,  i17, MAT_M_OFFSET_U32
LSU0.LDO.32 i8,  i17, MAT_BASE_PTR
LSU0.LDO.32 i1,  i18, MAT_BASE_PTR     
CMU.CPII    i18, i17 || LSU1.LDIL i17, 0x10
LSU1.LDIL i2,  0xFFFF
LSU1.LDIH i2,  0xFFFF
IAU.FEXTU   i7,  i5,  0x00, 0x02  || SAU.SUB.i32 i15, i5,  0x01       
IAU.SUBSU   i7,  i7,  0x01        || CMU.VSZM.BYTE i2,  i2,  [ZZZ0]  || PEU.PC1I 0x6
IAU.SUBSU   i7,  i7,  0x01        || CMU.VSZM.BYTE i2,  i2,  [ZZ00]  || PEU.PC1I 0x6
IAU.SUBSU   i7,  i7,  0x01        || CMU.VSZM.BYTE i2,  i2,  [Z000]  || PEU.PC1I 0x6
IAU.FEXTU   i4,  i6,  0x00, 0x02  || SAU.SUB.i32 i16, i6,  0x01                          || LSU0.LDIL i0, __m_transp4x4_pre || LSU1.LDIH i0, __m_transp4x4_pre
IAU.SUBSU   i4,  i4,  0x01                                           || PEU.PC1I 0x6     || LSU0.LDIL i0, __m_transp1x4_pre || LSU1.LDIH i0, __m_transp1x4_pre 
IAU.SUBSU   i4,  i4,  0x01                                           || PEU.PC1I 0x6     || LSU0.LDIL i0, __m_transp2x4_pre || LSU1.LDIH i0, __m_transp2x4_pre 
IAU.SUBSU   i4,  i4,  0x01                                           || PEU.PC1I 0x6     || LSU0.LDIL i0, __m_transp3x4_pre || LSU1.LDIH i0, __m_transp3x4_pre 
IAU.SHR.u32 i16, i16, 0x02        || SAU.SHR.u32 i15, i15, 0x02         
BRU.BRA __m_transp4x4_pre         || CMU.CMZ.i8  i2                  || PEU.PCIX.NEQ 0x0
BRU.JMP i0                        || CMU.CPII    i0,  i30            || PEU.PCIX.EQ  0x4
IAU.SHL     i5,  i5,  0x02        || SAU.SHL.x32 i12, i5,  0x04  
IAU.ADDSU   i9,  i8,  i5          || SAU.SHL.x32 i13, i6,  0x02
IAU.ADDSU   i10, i9,  i5          || SAU.ADD.i32 i2,  i1,  0
IAU.ADDSU   i11, i10, i5          || SAU.ADD.i32 i3,  i1,  i13
IAU.SHL     i13, i13, 0x01        || CMU.CPII    i14, i15
nop

// i18: out  
// i17: 0x10
// i16: (out.m-1)/4 @
// i15: (out.n-1)/4 @
// i14: current loop index @ 
// i13: 2*length of an input row
// i12: 4*length of an output row
// i8..11: output start of row address for 4 rows @
// i4..7:  output read address for 4 rows @
// i3:  next row input read address
// i2:  current input read address
// i1:  in column start address
// i0:  pointer for last 4rows handling
.lalign
__m_transp4x4_pre:
LSU0.LDXVI v0,  i2,  i13 || IAU.SUBSU i14, i14, 0
LSU1.LDXVI v1,  i3,  i13 || BRU.BRA __m_transp4x4_post || PEU.PCIX.EQ 0
LSU0.LDXVI v2,  i2,  i13 
LSU1.LDXVI v3,  i3,  i13 
SAU.ADD.i32  i4,  i8,  0 || IAU.ADDSU  i8,  i8,  i12 
SAU.ADD.i32  i5,  i9,  0 || IAU.ADDSU  i9,  i9,  i12
SAU.ADD.i32  i6,  i10, 0 || IAU.ADDSU  i10, i10, i12
SAU.ADD.i32  i7,  i11, 0 || IAU.ADDSU  i11, i11, i12
__m_transp4x4_loop:
LSU0.LDXVI v0,  i2,  i13 || IAU.SUBSU i14, i14, 0x01
LSU1.LDXVI v1,  i3,  i13 || BRU.BRA __m_transp4x4_loop || PEU.PCIX.NEQ 0
LSU0.LDXVI v2,  i2,  i13  
LSU1.LDXVI v3,  i3,  i13 || CMU.TP4R v4,  v0
LSU0.STXVI v4,  i4,  i17
LSU1.STXVI v5,  i5,  i17
LSU0.STXVI v6,  i6,  i17
LSU1.STXVI v7,  i7,  i17 
__m_transp4x4_post:
                            IAU.SUBSU i16, i16, 0x01
CMU.CPII i14, i15        ||	BRU.BRA __m_transp4x4_pre  || PEU.PCIX.NEQ 0
CMU.CPII i0,  i30        ||	BRU.JMP i0                 || PEU.PCIX.EQ  0x4
CMU.TP4R v4,  v0
LSU0.STXVI v4,  i4,  i17 || IAU.SHR.u32 i3,  i13, 0x01 || PEU.PVL032 0x6
LSU1.STXVI v5,  i5,  i17 || IAU.ADDSU   i1,  i1,  0x10 || PEU.PVL132 0x6
LSU0.STXVI v6,  i6,  i17 || IAU.ADDSU   i2,  i1,  0    || PEU.PVL032 0x6
LSU1.STXVI v7,  i7,  i17 || IAU.ADDSU   i3,  i3,  i2   || PEU.PVL132 0x6
nop

.lalign
__m_transp3x4_pre:
LSU0.LDXVI v0,  i2,  i13 || IAU.SUBSU i14, i14, 0
LSU1.LDXVI v1,  i3,  i13 || BRU.BRA __m_transp3x4_post || PEU.PCIX.EQ 0
LSU0.LDXVI v2,  i2,  i13 
LSU1.LDXVI v3,  i3,  i13 
SAU.ADD.i32  i4,  i8,  0 || IAU.ADDSU  i8,  i8,  i12
SAU.ADD.i32  i5,  i9,  0 || IAU.ADDSU  i9,  i9,  i12
SAU.ADD.i32  i6,  i10, 0 || IAU.ADDSU  i10, i10, i12
SAU.ADD.i32  i7,  i11, 0 || IAU.ADDSU  i11, i11, i12
__m_transp3x4_loop:
LSU0.LDXVI v0,  i2,  i13 || IAU.SUBSU i14, i14, 0x01
LSU1.LDXVI v1,  i3,  i13 || BRU.BRA __m_transp3x4_loop || PEU.PCIX.NEQ 0
LSU0.LDXVI v2,  i2,  i13  
LSU1.LDXVI v3,  i3,  i13 || CMU.TP4R v4,  v0
LSU0.STXVI v4,  i4,  i17
LSU1.STXVI v5,  i5,  i17
LSU0.STXVI v6,  i6,  i17
nop 
__m_transp3x4_post:
nop
BRU.JMP i30 
nop               
CMU.TP4R v4,  v0
LSU0.STXVI v4,  i4,  i17                               || PEU.PVL032 0x6
LSU1.STXVI v5,  i5,  i17                               || PEU.PVL132 0x6
LSU0.STXVI v6,  i6,  i17                               || PEU.PVL032 0x6
nop  

.lalign
__m_transp2x4_pre:
LSU0.LDXVI v0,  i2,  i13 || IAU.SUBSU i14, i14, 0
LSU1.LDXVI v1,  i3,  i13 || BRU.BRA __m_transp2x4_post || PEU.PCIX.EQ 0
LSU0.LDXVI v2,  i2,  i13 
LSU1.LDXVI v3,  i3,  i13 
SAU.ADD.i32  i4,  i8,  0 || IAU.ADDSU  i8,  i8,  i12
SAU.ADD.i32  i5,  i9,  0 || IAU.ADDSU  i9,  i9,  i12
SAU.ADD.i32  i6,  i10, 0 || IAU.ADDSU  i10, i10, i12
SAU.ADD.i32  i7,  i11, 0 || IAU.ADDSU  i11, i11, i12
__m_transp2x4_loop:
LSU0.LDXVI v0,  i2,  i13 || IAU.SUBSU i14, i14, 0x01
LSU1.LDXVI v1,  i3,  i13 || BRU.BRA __m_transp2x4_loop || PEU.PCIX.NEQ 0
LSU0.LDXVI v2,  i2,  i13  
LSU1.LDXVI v3,  i3,  i13 || CMU.TP4R v4,  v0
LSU0.STXVI v4,  i4,  i17
LSU1.STXVI v5,  i5,  i17
nop
nop 
__m_transp2x4_post:
BRU.JMP i30 
nop
nop               
CMU.TP4R v4,  v0
LSU0.STXVI v4,  i4,  i17                                 || PEU.PVL032 0x6
LSU1.STXVI v5,  i5,  i17                                 || PEU.PVL132 0x6
nop
 
.lalign
__m_transp1x4_pre:
LSU0.LDXVI v0,  i2,  i13 || IAU.SUBSU i14, i14, 0
LSU1.LDXVI v1,  i3,  i13 || BRU.BRA __m_transp1x4_post || PEU.PCIX.EQ 0
LSU0.LDXVI v2,  i2,  i13 
LSU1.LDXVI v3,  i3,  i13 
SAU.ADD.i32  i4,  i8,  0 || IAU.ADDSU  i8,  i8,  i12
SAU.ADD.i32  i5,  i9,  0 || IAU.ADDSU  i9,  i9,  i12
SAU.ADD.i32  i6,  i10, 0 || IAU.ADDSU  i10, i10, i12
SAU.ADD.i32  i7,  i11, 0 || IAU.ADDSU  i11, i11, i12
__m_transp1x4_loop:
LSU0.LDXVI v0,  i2,  i13 || IAU.SUBSU i14, i14, 0x01
LSU1.LDXVI v1,  i3,  i13 || BRU.BRA __m_transp1x4_loop || PEU.PCIX.NEQ 0
LSU0.LDXVI v2,  i2,  i13  
LSU1.LDXVI v3,  i3,  i13 || CMU.TP4R v4,  v0
LSU0.STXVI v4,  i4,  i17
nop 
nop
nop 
__m_transp1x4_post:
BRU.JMP i30 
nop
nop               
CMU.TP4R v4,  v0
LSU0.STXVI v4,  i4,  i17                                || PEU.PVL032 0x6
nop
nop
.nowarnend
.size mvcvMatTranspose_asm,.-mvcvMatTranspose_asm
.end
