///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvMaxTest3x3_s16_asm,@function
.version 00.51.05
//-------------------------------------------------------------------------------
//void mvcvMaxTest3x3_s16_asm(s16* inBufferCandidates, s16** inBuffer, u32 maxLocationsIn[], u32 maxLocationsOut[], u32 maxCountIn, u32* maxCountOut)
//                                    i18                       i17             i16                     i15                 i14               i13 

.code .text.mvcvMaxTest3x3_s16_asm
.align 16
.nowarn
mvcvMaxTest3x3_s16_asm:
LSU0.LDI.64 i0 i16 || LSU1.LDIL i4 1// i0, i1 two candidates location
LSU0.LDO.32 i10 i17 4 || LSU1.LDO.32 i17 i17 0
LSU0.LDO.32 i9 i17 8 || IAU.SHR.u32 i8 i14 1
LSU0.LDIL i11 maxTest3x3_s16_maxTest_lastCandidate || LSU1.LDIH i11 maxTest3x3_s16_maxTest_lastCandidate
IAU.SUB i12 i14 2
PEU.PC1I LT || BRU.JMP i11 // if the maxCount is smaller then two, skip the loop
nop
IAU.SHL i2 i0 1
// reset i11 - output max count || we don't need the width anymore. Load the loop_end into it
IAU.XOR i11 i11 i11
IAU.SUB i3 i2 2 || LSU0.LDIL i12 maxTest3x3_s16_maxTest_loop_end || LSU1.LDIH i12 maxTest3x3_s16_maxTest_loop_end
IAU.ADD i6 i3 i17 || VAU.XOR v1 v1 v1
IAU.ADD i7 i3 i10

//cycle start // process two candidates in a cycle
    LSU0.LD.64.l v1 i6 || BRU.RPL i12 i8
    IAU.ADD i6 i3 i9  || LSU1.LD.64.h v0 i7 
    IAU.ADD i3 i2 i18 || LSU0.LD.64.l v0 i6
    LSU1.LD.16r v10 i3 || IAU.SHL i2 i1 1
    IAU.SUB i3 i2 2
    nop
    IAU.ADD i6 i3 i17 || VAU.XOR v3 v3 v3
    IAU.ADD i7 i3 i10 || LSU1.LD.64.l v3 i6 || LSU0.SWZV8 [22222210] || VAU.ALIGNBYTE v1 v1 v1 0
    IAU.ADD i6 i3 i9  || LSU1.LD.64.h v2 i7 || LSU0.LDI.64 i0 i16
    IAU.ADD i3 i2 i18 || LSU1.LD.64.l v2 i6 || LSU0.SWZV8 [66654210] || VAU.ALIGNBYTE v0 v0 v0 0
    CMU.max.i16 v1 v1 v0 || LSU1.LD.16r v11 i3
    CMU.CPII i12 i0
    CMU.CPII i8 i1
    nop
 maxTest3x3_s16_maxTest_loop_end:

    CMU.CMVV.i16 v1 v10 || LSU1.SWZV8 [22222210] || VAU.ALIGNBYTE v3 v3 v3 0
    PEU.PC8C.AND 0x4 || IAU.ADD i11 i11 1 || LSU1.STI.32 i12 i15 //it is a maxima store it
    LSU1.SWZV8 [66654210] || IAU.SHL i2 i0 1 || VAU.ALIGNBYTE v2 v2 v2 0
    CMU.max.i16 v2 v2 v3 || IAU.SUB i3 i2 2
    IAU.ADD i6 i3 i17 || VAU.XOR v1 v1 v1
    CMU.CMVV.i16 v2 v11 || IAU.ADD i7 i3 i10
    PEU.PC8C.AND 0x4 || IAU.ADD i11 i11 1 || LSU1.STI.32 i8 i15 //it is a maxima store it

 maxTest3x3_s16_maxTest_lastCandidate:
IAU.AND i14 i14 i4 || LSU0.LDIL i1 maxTest3x3_s16_maxTest_end || LSU1.LDIH i1 maxTest3x3_s16_maxTest_end

// if the maxCount is smaller then two, skip the last element processing
// in this case we must ensure that all registers (which are accessed) has valid addresses
PEU.PC1I EQ || BRU.JMP i1 || SAU.AND i2 i2 i14 || IAU.XOR i3 i3 i3 || CMU.CPII i7 i18 || LSU0.LDIH i6 0x700B
IAU.ADD i6 i3 i9 || SAU.ADD.i32 i3 i2 i18 || LSU0.LD.64.l v1 i6
LSU1.LD.64.h v0 i7 || LSU0.LD.64.l v0 i6 
nop 2
LSU1.LD.16r v10 i3
nop 2
LSU0.SWZV8 [22222210] || VAU.ALIGNBYTE v1 v1 v1 0
LSU0.SWZV8 [66654210] || VAU.ALIGNBYTE v0 v0 v0 0
CMU.max.i16 v1 v1 v0
nop 1
CMU.CMVV.i16 v1 v10
PEU.PC8C.AND 0x4 || IAU.ADD i11 i11 1 || LSU1.STI.32 i0 i15 //it is a maxima store it


 maxTest3x3_s16_maxTest_end:
LSU0.ST.32 i11 i13 || BRU.JMP i30
nop 6
.size mvcvMaxTest3x3_s16_asm,.-mvcvMaxTest3x3_s16_asm
.nowarnend
.end
