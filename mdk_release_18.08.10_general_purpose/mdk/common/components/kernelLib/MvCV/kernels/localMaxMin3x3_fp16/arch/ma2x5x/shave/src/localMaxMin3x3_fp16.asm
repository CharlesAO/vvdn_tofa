///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvLocalMaxMin3x3_fp16_asm,@function
.version 00.51.05
//-------------------------------------------------------------------------------
/////void mvcvLocalMaxMin3x3_fp16_asm(fp16** inBuffer, u32 width, u32* minLocationList[], u32* maxLocationList[], u32* minCount, u32* maxCount)//
/////                                i18             i17             i16                i15                     i14             i13
.code .text.mvcvLocalMaxMin3x3_fp16_asm
.align 16

.lalign
    mvcvLocalMaxMin3x3_fp16_asm:
    IAU.SUB i19 i19 8
	LSU0.ST.64.l v27  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v27  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v28  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v28  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v29  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v29  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v30  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v30  i19 
LSU0.LDO.32 i10 i18 4 || LSU1.LDO.32 i18 i18 0
LSU0.LDO.32 i9 i18 8

LSU0.LDIL i1 localMaxMin3x3_fp16_loop_end || LSU1.LDIH i1 localMaxMin3x3_fp16_loop_end
IAU.SHR.u32 i17 i17 3 // repetition count
SAU.XOR i3 i3 i3 || IAU.XOR i6 i6 i6 //index1 || max count
IAU.ADD i4 i3 1 || SAU.XOR i5 i5 i5//index2 || min count
IAU.ADD i12 i3 4
iau.xor i8 i8 i8
iau.add i8 i8 i17
//load the first elements of the three lines in v0-2
LSU0.LDI.64.l v0 i18 || LSU1.LD.64.l v1 i10
LSU0.LDI.64.h v0 i18 || LSU1.LDI.64.l v2 i9
LSU0.LDO.64.h v1 i10 8 || LSU1.LDI.64.h v2 i9

LSU0.LDI.64.l v3 i18 || LSU1.LDO.64.l v4 i10 16
LSU0.LDO.64.h v4 i10 24 || LSU1.LDI.64.l v5 i9
LSU0.LDI.64.h v3 i18 || LSU1.LDI.64.h v5 i9 || IAU.ADD i10 i10 32
nop 
IAU.ADD i2 i3 5
 localMaxMin3x3_fp16_loop_start:
    // process four pixels
    // copy the 0th column in a vector || index1 increment 
	.nowarn 10
    CMU.CPVCR.x16 v23.l v0.0          || IAU.ADD i3 i3 1 || BRU.RPL i1 i17
    .nowarnend
	// copy the 4th column in a vector || index2 increment
    CMU.CPVCR.x16 v22.l v0.4          || IAU.ADD i4 i4 1 
    // copy the 1th column in a vector || index3 increment  || candidates in v16/h
    CMU.CPVCR.x16 v21.l v0.1          || IAU.ADD i12 i12 1 || LSU0.SWZV8 [32761054] || VAU.CMB.WORD v16 v1 [EEEE]
   
    // copy the 5th column in a vector || index4 increment
    CMU.CPVCR.x16 v20.l v0.5          || IAU.ADD i2 i2 1

    CMU.ALIGNVEC v2 v2 v5 2 // shift line2 to the right (the first column is already copied)
    CMU.ALIGNVEC v0 v0 v3 2 // shift line0 to the right (the first column is already copied)

    // copy 4 elements in v10 (candidate 1,2)and v11 (candidate 3,4)|| shift line1 to the right
    CMU.VILV.x16 v10 v11 v2 v0                                     
    CMU.ALIGNVEC v1 v1 v4 2
    // v15 has seven elements (candidate1) || candidates in v17.l
    CMU.VILV.x16 v13 v9 v10 v23           || LSU0.SWZV8 [32761054] || VAU.CMB.WORD v17 v1 [EEEE]
    CMU.CPVRC.x16 v16.0 v17.l //candidates at position 0 in v16-19
    CMU.VILV.x16 v15 v9 v11 v22 || LSU0.SWZV8 [10765432] || VAU.CMB.WORD v10 v10 [EEEE]// v15 has seven elements (candidate3)
    CMU.VILV.x16 v12 v9 v10 v21 || LSU0.SWZV8 [10765432] || VAU.CMB.WORD v11 v11 [EEEE]// v12 has seven elements (candidate2)

    //v12 has seven elements (candidate4) || fill v17 with candidate  (candidate1)
    CMU.VILV.x16 v14 v9 v11 v20          || LSU0.SWZV8 [00000000] || VAU.CMB.WORD v17 v17 [EEEE]
    // copy the 8th (candidate 1-4) elements in v12-15. We have all 8 elements in vectors
    CMU.CPVRC.x16 v12.0 v16.h             || LSU0.SWZV8 [00000000] || VAU.CMB.WORD v18 v18 [EEEE]// vector filled with candidate4
    // compare candidate1  || vector filled with candidate2
    CMU.CPVV V27 V15
    CMU.CPVV V28 V14
    CMU.CMVV.f16  v17 v13 || LSU0.SWZV8 [00000000] || VAU.CMB.WORD v16 v16 [EEEE]
    PEU.PC8C.AND 0x2 || IAU.ADD i6 i6 1 || LSU0.STI.32 i3 i15 //it is a maxima store it
    PEU.PC8C.AND 0x4 || IAU.ADD i5 i5 1 || LSU0.STI.32 i3 i16 //it is a minima store it
    // compare candidate2  || vector filled with candidate3
    CMU.CMVV.f16  v16 v12 ||  LSU0.SWZV8 [00000000] || VAU.CMB.WORD v19 v19 [EEEE]
    PEU.PC8C.AND 0x2 || IAU.ADD i6 i6 1 || LSU0.STI.32 i4 i15 //it is a maxima store it
    PEU.PC8C.AND 0x4 || IAU.ADD i5 i5 1 || LSU0.STI.32 i4 i16 //it is a minima store it
    // compare candidate3  || shift v3 in v9
    LSU0.SWZV8 [07654321] || VAU.CMB.WORD v9 v3 [EEEE]
    // compare candidate 4 with elements || shift line0
    CMU.ALIGNVEC v0 v0 v9 2
    CMU.CPVV V29 V18
    CMU.CPVV V30 V19
    // shift line1 and line 2 // load new elements in v6-8
    CMU.ALIGNVEC v9 v4 v4 2 || LSU0.LDI.64.l v6 i18 || LSU1.LDI.64.l v7 i10
    CMU.ALIGNVEC v1 v1 v9 2 || LSU0.LDI.64.h v6 i18 || LSU1.LDI.64.h v7 i10
    CMU.ALIGNVEC v9 v5 v5 2 || LSU1.LDI.64.l v8 i9
    CMU.ALIGNVEC v2 v2 v9 2 || LSU1.LDI.64.h v8 i9
    // Start processing the next 4 candidates
    // copy the 0th column in a vector || index1 increment || candidates in v16/h
    CMU.CPVCR.x16 v23.l v0.0          || IAU.ADD i3 i3 2 || LSU0.SWZV8 [32761054] || VAU.CMB.WORD v16 v1 [EEEE]
    // copy the 4th column in a vector || index2 increment
    CMU.CPVCR.x16 v22.l v0.4          || IAU.ADD i4 i4 2
    // copy the 1th column in a vector || index3 increment
    CMU.CPVCR.x16 v21.l v0.1          
    // copy the 5th column in a vector || index4 increment
    CMU.CPVCR.x16 v20.l v0.5          
    CMU.ALIGNVEC v2 v2 v5 2 // shift line2 to the right (the first column is already copied)
    CMU.ALIGNVEC v0 v0 v3 2 // shift line0 to the right (the first column is already copied)
    // copy 4 elements in v10 (candidate 1,2)and v11 (candidate 3,4)
    CMU.VILV.x16 v10 v11 v2 v0
    CMU.ALIGNVEC v1 v1 v4 2//shift line1 to the right

    // v15 has seven elements (candidate1) || candidates in v17.l
    CMU.VILV.x16 v13 v9 v10 v23           || LSU0.SWZV8 [32761054] || VAU.CMB.WORD v17 v1 [EEEE]
    // candidates at position 0 in v16-19
    CMU.CPVRC.x16 v16.0 v17.l
    CMU.VILV.x16 v15 v9 v11 v22 || LSU0.SWZV8 [10765432] || VAU.CMB.WORD v10 v10 [EEEE]// v15 has seven elements (candidate3)
    CMU.VILV.x16 v12 v9 v10 v21 || LSU0.SWZV8 [10765432] || VAU.CMB.WORD v11 v11 [EEEE]// v12 has seven elements (candidate2)
    //v12 has seven elements (candidate4) || fill v17 with candidate  (candidate1)
    CMU.VILV.x16 v14 v9 v11 v20          || LSU0.SWZV8 [00000000] || VAU.CMB.WORD v17 v17 [EEEE]
    // copy the 8th (candidate 1-4) elements in v12-15. We have all 8 elements in vectors
    CMU.CPVRC.x16 v12.0 v16.h || LSU0.SWZV8 [00000000] || VAU.CMB.WORD v18 v18 [EEEE]// vector filled with candidate4
    // compare candidate1  || vector filled with candidate2
    CMU.CMVV.f16  v17 v13 || LSU0.SWZV8 [00000000] || VAU.CMB.WORD v16 v16 [EEEE]// vector filled with candidate2 compare candidate 1 with elements
    PEU.PC8C.AND 0x2 || IAU.ADD i6 i6 1 || LSU0.STI.32 i3 i15 //it is a maxima store it
    PEU.PC8C.AND 0x4 || IAU.ADD i5 i5 1 || LSU0.STI.32 i3 i16 //it is a minima store it
    // compare candidate2  || vector filled with candidate3
    CMU.CMVV.f16  v16 v12 ||  LSU0.SWZV8 [00000000] || VAU.CMB.WORD v19 v19 [EEEE]// vector filled with candidate3// compare candidate 2 with elements
    PEU.PC8C.AND 0x2 || IAU.ADD i6 i6 1 || LSU0.STI.32 i4 i15 //it is a maxima store it
    PEU.PC8C.AND 0x4 || IAU.ADD i5 i5 1 || LSU0.STI.32 i4 i16 //it is a minima store it
    //store elements 5, 6 after elements 3, 4 
    CMU.CMVV.f16  v30 v27 
    PEU.PC8C.AND 0x2 || IAU.ADD i6 i6 1 || LSU0.STI.32 i12 i15 //it is a maxima store it
    PEU.PC8C.AND 0x4 || IAU.ADD i5 i5 1 || LSU0.STI.32 i12 i16 //it is a minima store it
    CMU.CMVV.f16  v29 v28
    PEU.PC8C.AND 0x2 || IAU.ADD i6 i6 1 || LSU0.STI.32 i2 i15 //it is a maxima store it
    PEU.PC8C.AND 0x4 || IAU.ADD i5 i5 1 || LSU0.STI.32 i2 i16 //it is a minima store it
    IAU.ADD i12 i12 2
    IAU.ADD i2 i2 2
    //compare candidate3
    CMU.CMVV.f16  v19 v15
    PEU.PC8C.AND 0x2 || IAU.ADD i6 i6 1 || LSU0.STI.32 i12 i15 //it is a maxima store it
    PEU.PC8C.AND 0x4 || IAU.ADD i5 i5 1 || LSU0.STI.32 i12 i16 //it is a minima store it
    iau.incs i8 -1
    cmu.cmz.i32 i8
    peu.pc1c eq || bru.bra  label_forLastElem
    nop 6
    //compare candidate4
    CMU.CMVV.f16  v18 v14
    PEU.PC8C.AND 0x2 || IAU.ADD i6 i6 1 || LSU0.STI.32 i2 i15 //it is a maxima store it
    PEU.PC8C.AND 0x4 || IAU.ADD i5 i5 1 || LSU0.STI.32 i2 i16 //it is a minima store it
    label_forLastElem:
    .nowarn 9

    //copy the next elements in v0-5 from v3-8
    CMU.CPVV v0 v3 || LSU0.CP v1 v4 || LSU1.CP v2 v5 || IAU.ADD i3 i3 5 || SAU.ADD.i32 i2 i2 5
    CMU.CPVV v3 v6 || LSU0.CP v4 v7 || LSU1.CP v5 v8 || IAU.ADD i4 i4 5 || SAU.ADD.i32 i12 i12 5
    .nowarnend
    localMaxMin3x3_fp16_loop_end:
	nop 7
    
//store minCount and maxCount
LSU0.ST.32 i6 i13 || LSU1.ST.32 i5 i14
    LSU0.LD.64.h  v30  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v30  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v29  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v29  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v28  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v28  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v27  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v27  i19 || IAU.ADD i19 i19 8


BRU.JMP i30
NOP 6

.size mvcvLocalMaxMin3x3_fp16_asm,.-mvcvLocalMaxMin3x3_fp16_asm
.end
