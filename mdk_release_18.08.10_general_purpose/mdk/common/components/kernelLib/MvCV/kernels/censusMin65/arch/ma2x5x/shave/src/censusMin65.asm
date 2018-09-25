.type mvcvCensusMin65_asm,@function
.version 00.50.00

//////
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
/// 
/// mvcvCensusMatching65 
//     @param[in] in1            - pointer to input lines of the left image
//     @param[in] in2            - pointer to input lines of the right image
//     @param[out] out           - array of disparity cost
//     @param[in] width          - width of the input lines
//     @return    Nothing




.data .rodata.mvcvCensusMin65_asm
.align 16

.code .text.mvcvCensusMin65_asm
.align 16

.lalign
//mvcvCensusMin65(u8 *in, u8 *out, u32 width)
//                  i18      i17        i16
mvcvCensusMin65_asm:

	LSU0.ldil i15, censusmin65_loop || LSU1.ldih i15, censusmin65_loop
	LSU0.ldil i14, 0x41 || LSU1.ldih i14, 0x0

lsu0.ldo.8 i1 i18   0  
lsu0.ldo.8 i4 i18   1 
lsu0.ldo.8 i5 i18   2 
lsu0.ldo.8 i6 i18   3 
lsu0.ldo.8 i7 i18   4 
lsu0.ldo.8 i8 i18   5  
lsu0.ldo.8 i9 i18   6 
lsu0.ldo.8 i10 i18  7  
lsu0.ldo.8 i11 i18  8  

nop 6
IAU.XOR i0 i0 i0  
IAU.XOR i3 i3 i3


cmu.cmii.u8 i4 i1 || IAU.ADD i0 i0 1 || lsu0.ldo.8 i4 i18   9  || bru.rpl i15 i16 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i4
cmu.cmii.u8 i5 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i5 i18   10 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i5
cmu.cmii.u8 i6 i1 || IAU.ADD i0 i0 1 || lsu0.ldo.8 i6 i18   11 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i6
cmu.cmii.u8 i7 i1 || IAU.ADD i0 i0 1 || lsu0.ldo.8 i7 i18   12 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i7
cmu.cmii.u8 i8 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i8 i18   13 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i8
cmu.cmii.u8 i9 i1 || IAU.ADD i0 i0 1 || lsu0.ldo.8 i9 i18   14 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i9
cmu.cmii.u8 i10 i1 || IAU.ADD i0 i0 1 || lsu0.ldo.8 i10 i18  15 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i10
cmu.cmii.u8 i11 i1 || IAU.ADD i0 i0 1 || lsu0.ldo.8 i11 i18  16 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i11
cmu.cmii.u8 i4 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i4 i18   17 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i4
cmu.cmii.u8 i5 i1 || IAU.ADD i0 i0 1 || lsu0.ldo.8 i5 i18   18 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i5
cmu.cmii.u8 i6 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i6 i18   19 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i6
cmu.cmii.u8 i7 i1 || IAU.ADD i0 i0 1 || lsu0.ldo.8 i7 i18   20 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i7
cmu.cmii.u8 i8 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i8 i18   21 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i8
cmu.cmii.u8 i9 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i9 i18   22 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i9
cmu.cmii.u8 i10 i1 || IAU.ADD i0 i0 1 || lsu0.ldo.8 i10 i18  23 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i10
cmu.cmii.u8 i11 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i11 i18  24 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i11
cmu.cmii.u8 i4 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i4 i18   25 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i4
cmu.cmii.u8 i5 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i5 i18   26 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i5
cmu.cmii.u8 i6 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i6 i18   27 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i6
cmu.cmii.u8 i7 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i7 i18   28 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i7
cmu.cmii.u8 i8 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i8 i18   29 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i8
cmu.cmii.u8 i9 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i9 i18   30 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i9
cmu.cmii.u8 i10 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i10 i18  31 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i10
cmu.cmii.u8 i11 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i11 i18  32 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i11
cmu.cmii.u8 i4 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i4 i18   33 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i4
cmu.cmii.u8 i5 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i5 i18   34  
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i5
cmu.cmii.u8 i6 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i6 i18   35 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i6
cmu.cmii.u8 i7 i1 || IAU.ADD i0 i0 1 || lsu0.ldo.8 i7 i18   36 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i7
cmu.cmii.u8 i8 i1 || IAU.ADD i0 i0 1 || lsu0.ldo.8 i8 i18   37 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i8
cmu.cmii.u8 i9 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i9 i18   38 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i9
cmu.cmii.u8 i10 i1 || IAU.ADD i0 i0 1 || lsu0.ldo.8 i10 i18  39 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i10
cmu.cmii.u8 i11 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i11 i18  40 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i11
cmu.cmii.u8 i4 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i4 i18   41 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i4
cmu.cmii.u8 i5 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i5 i18   42 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i5
cmu.cmii.u8 i6 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i6 i18   43 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i6
cmu.cmii.u8 i7 i1 || IAU.ADD i0 i0 1 || lsu0.ldo.8 i7 i18   44 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i7
cmu.cmii.u8 i8 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i8 i18   45 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i8
cmu.cmii.u8 i9 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i9 i18   46 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i9
cmu.cmii.u8 i10 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i10 i18  47 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i10
cmu.cmii.u8 i11 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i11 i18  48 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i11
cmu.cmii.u8 i4 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i4 i18   49 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i4
cmu.cmii.u8 i5 i1 || IAU.ADD i0 i0 1 || lsu0.ldo.8 i5 i18   50 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i5
cmu.cmii.u8 i6 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i6 i18   51 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i6
cmu.cmii.u8 i7 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i7 i18   52 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i7
cmu.cmii.u8 i8 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i8 i18   53 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i8
cmu.cmii.u8 i9 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i9 i18   54 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i9
cmu.cmii.u8 i10 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i10 i18  55 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i10
cmu.cmii.u8 i11 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i11 i18  56 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i11
cmu.cmii.u8 i4 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i4 i18   57 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i4
cmu.cmii.u8 i5 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i5 i18   58 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i5
cmu.cmii.u8 i6 i1  ||IAU.ADD i0 i0 1 || lsu0.ldo.8 i6 i18   59 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i6
cmu.cmii.u8 i7 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i7 i18   60 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i7
cmu.cmii.u8 i8 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i8 i18   61 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i8
cmu.cmii.u8 i9 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i9 i18   62 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i9
cmu.cmii.u8 i10 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i10 i18  63 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i10
cmu.cmii.u8 i11 i1 || IAU.ADD i0 i0 1 || lsu0.ldo.8 i11 i18  64 || SAU.ADD.i32 i18 i18 i14
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i11
cmu.cmii.u8 i4 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i4 i18   1 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i4
cmu.cmii.u8 i5 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i5 i18   2 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i5
cmu.cmii.u8 i6 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i6 i18   3 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i6
cmu.cmii.u8 i7 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i7 i18   4 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i7
cmu.cmii.u8 i8 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i1 i18   0  
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i8
cmu.cmii.u8 i9 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i8 i18   5  
censusmin65_loop:
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i9
cmu.cmii.u8 i10 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i9 i18   6 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i10
cmu.cmii.u8 i11 i1  || IAU.ADD i12 i0 1 || lsu0.ldo.8 i10 i18  7  || SAU.XOR i0 i0 i0  
peu.pc1c LT || IAU.SUB i3 i12 0 
lsu0.st.8 i3 i17 || SAU.ADD.i32 i17 i17 1 || IAU.XOR i3 i3 i3
lsu1.ldo.8 i11 i18  8  




bru.jmp i30
nop 6
.size mvcvCensusMin65_asm,.-mvcvCensusMin65_asm
.end
