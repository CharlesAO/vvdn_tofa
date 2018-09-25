.version 00.51.05

.include "myriad2SippDefs.inc"

.code .text.sippFilterAccesorsOpt
.align 16

//##############################################################################################
// C header: "u32 sippFilterGetNumOutPlanes (SippFilter * fptr, u32 outBufferIdx)//"
//##############################################################################################
.lalign
sippFilterGetNumOutPlanes:
lsu0.ldil i0 1
iau.shl i0 i0 2
iau.mul i17 i17 i0
nop
iau.add i3 i18 i0
iau.add i3 i3 i17
lsu0.ld.32 i18 i3   || bru.jmp i30
nop 6


//##############################################################################################
// C header: "u32 sippFilterGetOutputBpp (SippFilter * fptr)//"
//##############################################################################################
.lalign
sippFilterGetOutputBpp:
lsu0.ldil i0 F_O_BYTESPP
iau.add i18 i18 i0
lsu0.ld.32 i18 i18 || bru.jmp i30
nop 6


//##############################################################################################
// C header: "u32 sippFilterGetParentInputLines (SippFilter * fptr, u32 parentIdx)//"
//##############################################################################################
.lalign
sippFilterGetParentInputLines:
lsu0.ldil i0 F_O_PAR_OBUF_IDX
iau.add i0 i0 i18  || lsu0.ldil i1 1
iau.shl i1 i1 2
iau.mul i2 i17 i1
nop
iau.add i4 i18 F_O_PARENTS
iau.add i0 i0 i2
lsu0.ld.32 i3 i0   || iau.add i4 i4 i2
lsu0.ld.32 i5 i4
lsu0.ldil i6 F_O_N_LINES
nop 5
iau.add i5 i5 i6
iau.add i5 i5 i3
lsu0.ld.32 i18 i5  || bru.jmp i30
nop 6


//##############################################################################################
// C header: "u32 sippFilterGetParentPlaneStride (SippFilter * fptr, u32 parentIdx)//"
//##############################################################################################
.lalign
sippFilterGetParentPlaneStride:
lsu0.ldil i0 F_O_PAR_OBUF_IDX || lsu1.ldih i0 0x0
iau.add i0 i0 i18             || lsu0.ldil i1 1
iau.shl i1 i1 2
iau.mul i2 i17 i1
nop
iau.add i4 i18 F_O_PARENTS
iau.add i0 i0 i2
lsu0.ld.32 i3 i0    || iau.add i4 i4 i2
lsu0.ld.32 i5 i4
nop 6
lsu0.ldil i6 F_O_PL_STRIDE || lsu1.ldih i6 0x0
iau.add i5 i5 i6
iau.add i5 i5 i3
lsu0.ld.32 i18 i5   || bru.jmp i30
nop 6


//##############################################################################################
// C header: "u32 sippFilterGetPlaneStride (SippFilter * fptr, u32 outBufferIdx)//"
//##############################################################################################
.lalign
sippFilterGetPlaneStride:
lsu0.ldil i1 F_O_PL_STRIDE || lsu1.ldih i1 0x0
iau.add i18 i18 i1         || lsu0.ldil i0 1
iau.shl i0 i0 2
iau.mul i17 i17 i0
nop
iau.add i17 i17 i18
lsu0.ld.32 i18 i17  || bru.jmp i30
nop 6


//##############################################################################################
// C header: "u32 sippFilterGetLinesThisIter (SippFilter * fptr)//"
//##############################################################################################
.lalign
sippFilterGetLinesThisIter:
lsu0.ldo.32 i2 i18 F_O_EXENO
lsu0.ldo.64 i0 i18 F_O_LNS_THIS_ITER
nop 2
bru.jmp i30
nop 3
cmu.cpii i18 i0
iau.fextu i2 i2 0 1
cmu.cpii i18 i1     || peu.pc1i NEQ


//##############################################################################################
// C header: "u32 sippFilterGetLinesPerIter (SippFilter * fptr)//"
//##############################################################################################
.lalign
sippFilterGetLinesPerIter:
lsu0.ldo.32 i18 i18 F_O_LNS_PER_ITER || bru.jmp i30
nop 6


//##############################################################################################
// C header: "u32 sippFilterGetParentSliceWidth (SippFilter * fptr, u32 parentIdx)//"
//##############################################################################################
.lalign
sippFilterGetParentSliceWidth:
lsu0.ldil i1 1   || iau.add i3 i18 F_O_PARENTS
iau.shl i1 i1 2
iau.mul i2 i17 i1
lsu0.ldil i6 F_O_SLICE_WIDTH || lsu1.ldih i6 0x0
iau.add i3 i3 i2
lsu0.ld.32 i5 i3
nop 6
iau.add i5 i5 i6
lsu0.ld.32 i18 i5   || bru.jmp i30
nop 6


//##############################################################################################
// C header: "u32 sippFilterGetParentOutputWidth (SippFilter * fptr, u32 parentIdx)//"
//##############################################################################################
.lalign
sippFilterGetParentOutputWidth:
lsu0.ldil i1 1   || iau.add i3 i18 F_O_PARENTS
iau.shl i1 i1 2
iau.mul i2 i17 i1
lsu0.ldil i6 F_O_OUTPUT_WIDTH || lsu1.ldih i6 0x0
iau.add i3 i3 i2
lsu0.ld.32 i5 i3
nop 6
iau.add i5 i5 i6
lsu0.ld.32 i18 i5   || bru.jmp i30
nop 6


//##############################################################################################
// C header: "u32 sippFilterGetParentOutputHeight (SippFilter * fptr, u32 parentIdx)//"
//##############################################################################################
.lalign
sippFilterGetParentOutputHeight:
lsu0.ldil i1 1   || iau.add i3 i18 F_O_PARENTS
iau.shl i1 i1 2
iau.mul i2 i17 i1
lsu0.ldil i6 F_O_OUTPUT_HEIGHT || lsu1.ldih i6 0x0
iau.add i3 i3 i2
lsu0.ld.32 i5 i3
nop 6
iau.add i5 i5 i6
lsu0.ld.32 i18 i5   || bru.jmp i30
nop 6


.end
