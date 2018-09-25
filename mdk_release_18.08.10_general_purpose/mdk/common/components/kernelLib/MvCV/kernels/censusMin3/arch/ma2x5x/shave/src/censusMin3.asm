.type mvcvCensusMin3_asm,@function
.version 00.50.00

//////
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
/// 
/// mvcvCensusMatching3 
//     @param[in] in1            - pointer to input lines of the left image
//     @param[in] in2            - pointer to input lines of the right image
//     @param[out] out           - array of disparity cost
//     @param[in] width          - width of the input lines
//     @return    Nothing




.data .rodata.mvcvCensusMin3_asm
.align 16

.code .text.mvcvCensusMin3_asm
.align 16

.lalign
//mvcvCensusMin3(u8 *in, u8 *out, u8 *outm, u32 width)
//                  i18      i17     i16        i15
mvcvCensusMin3_asm:

	LSU0.ldil i14, censusmin3_loop || LSU1.ldih i14, censusmin3_loop

lsu0.ldo.8 i1 i18 0     || IAU.XOR i0 i0 i0 
lsu0.ldo.8 i2 i18 1     || IAU.XOR i4 i4 i4 //i4=minPos
lsu0.ldo.8 i3 i18 2     || IAU.ADD i18 i18 3 
nop 3
lsu0.ldo.8 i1 i18 0 
lsu0.ldo.8 i2 i18 1 
lsu0.ldo.8 i3 i18 2     || IAU.ADD i18 i18 3



cmu.cmii.u8 i2 i1  || IAU.ADD i0 i0 1 || lsu1.cp i5 i1  || bru.rpl i14 i15  //|| lsu0.ldo.8 i1 i18 0 
censusmin3_loop:
.nowarn 10
peu.pc1c LT        || IAU.SUB i4 i0 0   || cmu.cpii i1 i2  || lsu1.cp i5 i2 
.nowarnend

cmu.cmii.u8 i3 i1  || IAU.ADD i0 i0 1   //|| lsu0.ldo.8 i2 i18 1  
peu.pc1c LT        || IAU.SUB i4 i0 0 || lsu1.cp i5 i3

lsu1.st.8 i4 i17   || IAU.ADD i17 i17 1 //|| lsu0.ldo.8 i3 i18 2 
lsu0.ldo.8 i1 i18 0     || IAU.XOR i0 i0 i0 || lsu1.st.8 i5 i16 
lsu0.ldo.8 i2 i18 1     || IAU.XOR i4 i4 i4 || sau.add.i32 i16 i16 1//i4=minPos
lsu0.ldo.8 i3 i18 2     || IAU.ADD i18 i18 3      




bru.jmp i30
nop 6
.size mvcvCensusMin3_asm,.-mvcvCensusMin3_asm
.end
