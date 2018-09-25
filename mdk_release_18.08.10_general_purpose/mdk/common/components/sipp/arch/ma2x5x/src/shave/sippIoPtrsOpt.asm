.version 00.51.05

.include "myriad2SippDefs.inc"

.code .text.sippIoPtrsOpt
.align 16

//#############################################################################################
// C header: "u32 getInPtr (SippFilter *fptr, u32 parNo, u32 iterNo, u32 lineNo, u32 planeNo)//"
//#############################################################################################
.lalign
getInPtr:

lsu0.ldil i6 F_O_LINES_I        || cmu.cpti i12 P_SVID  || iau.shl i1 i17 7
lsu0.ldo.32 i0 i18 F_O_EXENO    || iau.add i1 i1 i18    || sau.shl.x32 i16 i16 2
lsu0.ldo.64 i2 i18 F_O_PAR_INFO || iau.add i1 i1 i6
iau.add i1 i1 i16
lsu0.ld.32 i4 i1                || iau.add i1 i1 0x40
lsu0.ld.32 i5 i1
sau.shl.x32 i14 i14 2
nop
sau.shl.x32 i12 i12 2
nop
iau.fextu i0 i0 0 1             || sau.shl.x32 i3 i15 2
iau.mul i1 i2 i12               || sau.add.i32 i14 i3 i14
cmu.cpii i4 i5                  || peu.pc1i NEQ
iau.add i4 i4 i3
iau.add i1 i1 i14               || lsu0.ld.32 i4 i4
lsu0.ld.32 i1 i1
bru.jmp i30
nop 5
iau.add i18 i4 i1


//#####################################################################################################
// C header: "void getIn3PlanePtr(SippFilter *fptr, u32 parNo, u32 iterNo, u32 lineNo, void *out3Ptr)//"
//#####################################################################################################
.lalign
getIn3PlanePtr:

lsu0.ldil i4 F_O_LINES_I        || iau.shl i0 i17 7    || sau.shl.x32 i16 i16 2
lsu0.ldo.32 i1 i18 F_O_EXENO    || iau.add i0 i0 i18
lsu0.ldo.64 i2 i18 F_O_PAR_INFO || iau.add i0 i0 i4
iau.add i0 i0 i16
lsu0.ld.32 i0 i0                || iau.add i0 i0 0x40
lsu0.ld.32 i1 i0
cmu.cpti i13 P_SVID
nop
sau.shl.x32 i13 i13 2
nop
iau.fextu i1 i1 0 1             || sau.shl.x32 i4 i15 2
iau.mul i1 i2 i13
cmu.cpii i0 i1                  || peu.pc1i NEQ
iau.add i0 i0 i4
iau.add i1 i1 i3                || lsu0.ld.32 i0 i0
lsu0.ld.64 i1 i1
lsu0.ldo.32 i3 i1 0x8
nop 2
bru.jmp i30
nop 2
iau.add i1 i1 i0
iau.add i2 i2 i0
iau.add i3 i3 i0                || lsu0.st.64 i1 i14
lsu0.sto.32 i3 i14 0x8


//################################################################################################################
// C header: "u32 getInPtrAbs(SippFilter *fptr, u32 parNo, u32 iterNo, u32 lineNo, u32 planeNo, u32 targetSlice)//"
//################################################################################################################
.lalign
getInPtrAbs:

lsu0.ldil i5 F_O_LINES_I        || iau.shl i0 i17 7   || sau.shl.x32 i16 i16 2
lsu0.ldo.32 i1 i18 F_O_EXENO    || iau.add i0 i0 i18
lsu0.ldo.64 i2 i18 F_O_PAR_INFO || iau.add i0 i0 i5
iau.add i0 i0 i16
lsu0.ld.32 i0 i0                || iau.add i0 i0 0x40
lsu0.ld.32 i1 i0
sau.shl.x32 i14 i14 2
nop
sau.shl.x32 i13 i13 2
nop
iau.fextu i1 i1 0 1             || sau.shl.x32 i3 i15 2
iau.mul i1 i2 i13               || sau.add.i32 i14 i3 i14
cmu.cpii i0 i1                  || peu.pc1i NEQ
iau.add i0 i0 i3
iau.add i1 i1 i14               || lsu0.ld.32 i0 i0
lsu0.ld.32 i1 i1
bru.jmp i30
nop 5
iau.add i18 i0 i1


//#######################################################################
// C header: "u32 getOutPtr (SippFilter *fptr, u32 iterNo, u32 planeNo)//"
//#######################################################################
.lalign
getOutPtr:

lsu0.ldo.64 i0 i18 F_O_EXENO     || lsu1.ldil i6 F_O_LINES_O  || sau.shl.x32 i17 i17 2
lsu0.ldo.32 i13 i18 F_O_OUT_OFFS || iau.add i18 i18 i6
iau.add i18 i18 i17
lsu0.ld.32 i2 i18                || iau.add i18 i18 0x40
lsu0.ld.32 i3 i18
cmu.cpti i14 P_SVID
sau.shl.x32 i14 i14 2
sau.shl.x32 i16 i16 2
nop
iau.fextu i0 i0 0 1
sau.add.i32 i13 i13 i16          || iau.mul i1 i1 i14
cmu.cpii i2 i3                   || peu.pc1i NEQ
iau.add i13 i13 i1
lsu0.ld.32  i13 i13
bru.jmp i30
nop 5
iau.add i18 i2 i13


//####################################################################################################
// C header: "u32 getPlaneIoPtrs(SippFilter *fptr, u32 parNo, u32 iterNo, u32 planeNo, void *inPtrs)//"
//####################################################################################################
.lalign
getPlaneIoPtrs:

lsu0.ldo.64 i0 i18 F_O_EXENO     || lsu1.ldil i8 F_O_LINES_I || iau.shl i2 i17 7      || sau.shl.x32 i16 i16 2
lsu0.ldo.32 i10 i18 F_O_OUT_OFFS || lsu1.ldil i9 F_O_LINES_O || iau.add i2 i2 i18
lsu0.ldo.64 i2 i2 F_O_PAR_INFO   || iau.add i8 i8 i2         || sau.add.i32 i9 i9 i18
iau.add i8 i8 i16
lsu0.ld.32 i4 i8                 || iau.shl i2 i17 2         || sau.add.i32 i9 i9 i16
iau.add i8 i8 0x40
lsu0.ld.32 i5 i8                 || lsu1.ld.32 i6 i9         || iau.add i9 i9 0x40
lsu0.ld.32 i7 i9                 || iau.add i2 i2 i18
lsu0.ldo.32 i7 i2 F_O_LINES_USED
cmu.cpti i11 P_SVID
sau.shl.x32 i11 i11 2
sau.shl.x32 i12 i15 2
iau.fextu i0 i0 0 1       || sau.mul.i32 i2 i2 i11
cmu.cpii i4 i5            || peu.pcix.NEQ 0x04        || iau.mul i5 i1 i11
cmu.cpii i6 i7            || peu.pcix.NEQ 0x04
iau.add i2 i2 i12         || sau.add.i32 i5 i5 i12
iau.add i2 i2 i3
lsu1.ld.32R v0 i2         || iau.add i5 i5 i10
lsu1.ld.32 i5 i5
lsu1.ldi.64.l v1 i4       || iau.subsu   i0 i7  2     || cmu.cpii i1 i14
lsu1.ldi.64.l v1 i4       || iau.subsu   i0 i0  2     || peu.pcix.NEQ 0x20
lsu1.ldi.64.l v1 i4       || iau.subsu   i0 i0  2     || peu.pcix.NEQ 0x20
lsu1.ldi.64.l v1 i4       || iau.subsu   i0 i0  2     || peu.pcix.NEQ 0x20
lsu1.ldi.64.l v1 i4       || iau.subsu   i0 i0  2     || peu.pcix.NEQ 0x20     || sau.shr.u32 i3 i7 1
lsu1.ldi.64.l v1 i4       || iau.subsu   i0 i0  2     || peu.pcix.NEQ 0x20
lsu1.ldi.64.l v1 i4       || iau.subsu   i0 i0  2     || peu.pcix.NEQ 0x20     || sau.add.i32 i10 i5 i6
lsu1.ldi.64.l v1 i4       || iau.subsu   i0 i0  2     || peu.pcix.NEQ 0x20     || vau.add.i32 v2 v0 v1
lsu1.ldi.64.l v1 i4       || vau.add.i32 v2 v0 v1     || peu.pcix.NEQ 0x20     || iau.or i3 i3 i3
lsu1.ldi.64.l v1 i4       || vau.add.i32 v2 v0 v1     || lsu0.sti.64.l v2 i1   || bru.rpi i3         || peu.pc1i GT
lsu0.cp i5 v2.0           || iau.fextu i7 i7 0 1      || bru.jmp i30
lsu0.st.32 i5 i1          || peu.pc1i NEQ
cmu.cpii i18 i10
nop 4


.end
