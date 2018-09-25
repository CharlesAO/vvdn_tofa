.type mvcvCensusMin7_asm,@function
.version 00.50.00

//////
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
/// 
/// mvcvCensusMatching7 
//     @param[in] in1            - pointer to input lines of the left image
//     @param[in] in2            - pointer to input lines of the right image
//     @param[out] out           - array of disparity cost
//     @param[in] width          - width of the input lines
//     @return    Nothing




.data .rodata.mvcvCensusMin7_asm
.align 16

.code .text.mvcvCensusMin7_asm
.align 16

.lalign
//mvcvCensusMin7(u8 *in, u8 *out, u32 width)
//                  i18      i17        i16
mvcvCensusMin7_asm:

	LSU0.ldil i15, censusmin7_loop || LSU1.ldih i15, censusmin7_loop

lsu0.ldo.8 i1 i18 0  
lsu0.ldo.8 i4 i18 1 
lsu0.ldo.8 i5 i18 2 
lsu0.ldo.8 i6 i18 3 
lsu0.ldo.8 i7 i18 4  
lsu0.ldo.8 i8 i18 5  || IAU.XOR i0 i0 i0 
lsu0.ldo.8 i9 i18 6  || IAU.ADD i18 i18 7  
SAU.XOR i3 i3 i3


nop 6




cmu.cmii.u8 i4 i1 || IAU.ADD i0 i0 1 || lsu0.ldo.8 i4 i18 1  || bru.rpl i15 i16 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i4
cmu.cmii.u8 i5 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i5 i18 2 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i5
cmu.cmii.u8 i6 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i6 i18 3 
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i6
cmu.cmii.u8 i7 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i1 i18 0 
censusmin7_loop:
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i7
cmu.cmii.u8 i8 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i7 i18 4  
peu.pc1c LT || IAU.SUB i3 i0 0 || cmu.cpii i1 i8
cmu.cmii.u8 i9 i1  || IAU.ADD i0 i0 1 || lsu0.ldo.8 i8 i18 5  
peu.pc1c LT || IAU.SUB i3 i0 0 
lsu0.ldo.8 i9 i18 6  || IAU.ADD i18 i18 7  || SAU.XOR i0 i0 i0
lsu0.st.8 i3 i17 || IAU.ADD i17 i17 1 || SAU.XOR i3 i3 i3 





	

bru.jmp i30
nop 6
.size mvcvCensusMin7_asm,.-mvcvCensusMin7_asm
.end
