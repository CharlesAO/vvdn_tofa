///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief   
///

.type bilinearInterpolation_asm,@function
.version 12.34.56

.code .text.bilinearInterpolation_asm
.align 16

//patternAlg( u8 *line1, u8 *line2, u8 *output, u16 *precalcPos, u64 *lut, int stopW, int godValue)
//               (i18)     (i17)       (i16)         (i15)        (i14)       (i13)      (i12)
.lalign
bilinearInterpolation_asm:

////load first 10 elements from precalcPos + save reserved VRF values on stack
    LSU0.LDI.64.L v25 i15    || VAU.XOR v0 v0 v0          || IAU.SUB i19 i19 112
    LSU0.LDI.64.H v25 i15    || LSU1.STO.64.L v24 i19 0   || VAU.XOR v1 v1 v1         || IAU.XOR i3 i3 i3
    LSU0.STO.64.H v24 i19 8  || LSU1.STO.64.L v25 i19 16  || VAU.XOR v2 v2 v2         || IAU.INCS i3 10
    LSU0.STO.64.H v25 i19 24 || LSU1.STO.64.L v26 i19 32  || VAU.XOR v3 v3 v3         || SAU.DIV.U32 i10 i13 i3
    LSU0.STO.64.H v26 i19 40 || LSU1.STO.64.L v27 i19 48  || VAU.XOR v4 v4 v4         || IAU.SUB i18 i18 6
    LSU0.STO.64.H v27 i19 56 || LSU1.STO.64.L v30 i19 64  || VAU.XOR v5 v5 v5         || IAU.XOR i11 i11 i11
    LSU0.STO.64.H v30 i19 72 || LSU1.STO.64.L v28 i19 80  || VAU.XOR v6 v6 v6         || IAU.ADD i13 i13 i16
    LSU0.STO.64.H v28 i19 88 || LSU1.STO.64.L v29 i19 96  || VAU.XOR v7 v7 v7         || CMU.CPIVR.X16 v30 i12
    LSU0.LD.64.L v26 i15     || LSU1.STO.64.H v29 i19 104 || CMU.CPVI.x16 i11.L v25.0 || IAU.SUB i13 i13 2

////prepare datas for calculation
////ushort8 v10=mvuConvert_ushort8(*(uchar8*)(i18+i11))                       i11 = v25.s1
    LSU0.LDX.128.U8.U16 v10 i18 i11 || LSU1.LDX.128.U8.U16 v20 i17 i11 || CMU.CPVI.x16 i11.L v25.1 || VAU.XOR v8 v8 v8
    LSU0.LDX.128.U8.U16 v11 i18 i11 || LSU1.LDX.128.U8.U16 v21 i17 i11 || CMU.CPVI.x16 i11.L v25.2 || VAU.XOR v9 v9 v9
    LSU0.LDX.128.U8.U16 v12 i18 i11 || LSU1.LDX.128.U8.U16 v22 i17 i11 || CMU.CPVI.x16 i11.L v25.3
    LSU0.LDX.128.U8.U16 v13 i18 i11 || LSU1.LDX.128.U8.U16 v23 i17 i11 || CMU.CPVI.x16 i11.L v25.4
    LSU0.LDX.128.U8.U16 v14 i18 i11 || LSU1.LDX.128.U8.U16 v24 i17 i11 || CMU.CPVI.x16 i11.L v25.5
    LSU0.LDX.128.U8.U16 v15 i18 i11 || LSU1.LDX.128.U8.U16 v20 i17 i11 || CMU.CPVI.x16 i11.L v25.6
    LSU0.LDX.128.U8.U16 v16 i18 i11 || LSU1.LDX.128.U8.U16 v21 i17 i11 || CMU.CPVI.x16 i11.L v25.7 || VAU.ADD.I16 v25 v25 v30
    LSU0.LDX.128.U8.U16 v17 i18 i11 || LSU1.LDX.128.U8.U16 v22 i17 i11 || CMU.CPVI.x16 i11.L v26.0
    LSU0.LDX.128.U8.U16 v18 i18 i11 || LSU1.LDX.128.U8.U16 v23 i17 i11 || CMU.CPVI.x16 i11.L v26.1
    LSU0.LDX.128.U8.U16 v19 i18 i11 || LSU1.LDX.128.U8.U16 v24 i17 i11 || CMU.CPVI.x16 i11.L v25.0 || VAU.ADD.I16 v26 v26 v30 || IAU.SUB i10 i10 1
    CMU.ALIGNVEC v10 v10 v20 12     || LSU0.LDO.64.L v0 i14 0          || LSU1.LDO.64.L v1 i14 8
    CMU.ALIGNVEC v11 v11 v21 12     || LSU0.LDO.64.L v2 i14 16         || LSU1.LDO.64.L v3 i14 24
    CMU.ALIGNVEC v12 v12 v22 12     || LSU0.LDO.64.L v4 i14 32         || LSU1.LDO.64.L v5 i14 40
    CMU.ALIGNVEC v13 v13 v23 12     || LSU0.LDO.64.L v6 i14 48         || LSU1.LDO.64.L v7 i14 56
    CMU.ALIGNVEC v14 v14 v24 12     || LSU0.LDO.64.L v8 i14 64         || LSU1.LDO.64.L v9 i14 72
    CMU.ALIGNVEC v15 v15 v20 12     || LSU0.LDIL i15 loopDelaySlot     || LSU1.LDIH i15 loopDelaySlot


////the first 4 values are processed and written out + next 10 vectors prepared for the loop
    CMU.ALIGNVEC v16 v16 v21 12
    CMU.ALIGNVEC v17 v17 v22 12
    VAU.MUL.i16 v10 v10 v0      || LSU0.LDX.128.U8.U16 v10 i18 i11 || LSU1.LDX.128.U8.U16 v20 i17 i11 || CMU.CPVI.x16 i11.L v25.1
    VAU.MUL.i16 v11 v11 v1      || LSU0.LDX.128.U8.U16 v11 i18 i11 || LSU1.LDX.128.U8.U16 v21 i17 i11 || CMU.CPVI.x16 i11.L v25.2
    CMU.ALIGNVEC v18 v18 v23 12
    VAU.MUL.i16 v12 v12 v2      || LSU0.LDX.128.U8.U16 v12 i18 i11 || LSU1.LDX.128.U8.U16 v22 i17 i11 || CMU.CPVI.x16 i11.L v25.3
    VAU.MUL.i16 v13 v13 v3      || LSU0.LDX.128.U8.U16 v13 i18 i11 || LSU1.LDX.128.U8.U16 v23 i17 i11 || CMU.CPVI.x16 i11.L v25.4 || SAU.SUMX.U16 i0 v10
    CMU.ALIGNVEC v19 v19 v24 12
    VAU.MUL.i16 v14 v14 v4      || LSU0.LDX.128.U8.U16 v14 i18 i11 || LSU1.LDX.128.U8.U16 v24 i17 i11 || CMU.CPVI.x16 i11.L v25.5 || SAU.SUMX.U16 i1 v11
    VAU.MUL.i16 v15 v15 v5      || LSU0.LDX.128.U8.U16 v15 i18 i11 || LSU1.LDX.128.U8.U16 v20 i17 i11 || CMU.CPVI.x16 i11.L v25.6 || SAU.SUMX.U16 i2 v12 || IAU.SHR.U32 i0 i0 8
    CMU.ALIGNVEC v10 v10 v20 12 || LSU0.STI.8 i0 i16
    VAU.MUL.i16 v16 v16 v6      || LSU0.LDX.128.U8.U16 v16 i18 i11 || LSU1.LDX.128.U8.U16 v21 i17 i11 || CMU.CPVI.x16 i11.L v25.7 || SAU.SUMX.U16 i3 v13 || IAU.SHR.U32 i1 i1 8
    VAU.MUL.i16 v27 v17 v7      || LSU0.LDX.128.U8.U16 v17 i18 i11 || LSU1.LDX.128.U8.U16 v22 i17 i11 || CMU.CPVI.x16 i11.L v26.0 || SAU.SUMX.U16 i4 v14 || IAU.SHR.U32 i2 i2 8
    CMU.ALIGNVEC v11 v11 v21 12 || LSU0.STI.8 i1 i16
    VAU.MUL.i16 v28 v18 v8      || LSU0.LDX.128.U8.U16 v18 i18 i11 || LSU1.LDX.128.U8.U16 v23 i17 i11 || CMU.CPVI.x16 i11.L v26.1 || SAU.SUMX.U16 i5 v15 || IAU.SHR.U32 i3 i3 8
    VAU.MUL.i16 v29 v19 v9      || LSU0.LDX.128.U8.U16 v19 i18 i11 || LSU1.LDX.128.U8.U16 v24 i17 i11 || CMU.CPVI.x16 i11.L v25.0 || SAU.SUMX.U16 i6 v16 || IAU.SHR.U32 i4 i4 8
////////////////////////////////
    CMU.ALIGNVEC v12 v12 v22 12 || LSU0.STI.8 i2 i16
    CMU.ALIGNVEC v13 v13 v23 12 || LSU0.STI.8 i3 i16               || IAU.SHR.U32 i5 i5 8
    CMU.ALIGNVEC v14 v14 v24 12 || VAU.ADD.I16 v25 v25 v30         || IAU.ADD i11 i11 i12
    CMU.ALIGNVEC v15 v15 v20 12 || VAU.ADD.I16 v26 v26 v30         || IAU.SHR.U32 i6 i6 8


//// v0-v10 factors
//// loop stopW/10 - 1 times// because we already calculated 10 values, and unroll count is 10

.lalign
bru_location:
bru.rpl i15 i10 loopDelaySlot
////multiply the 2 vectors         # load the next values from line1   # and line 2             # load the offset for the next load   #sum up vector elements   #normalize the sum
    CMU.ALIGNVEC v16 v16 v21 12 || LSU0.STI.8 i4 i16
    CMU.ALIGNVEC v17 v17 v22 12 || LSU0.STI.8 i5 i16
    VAU.MUL.i16 v10 v10 v0      || LSU0.LDX.128.U8.U16 v10 i18 i11 || LSU1.LDX.128.U8.U16 v20 i17 i11 || CMU.CPVI.x16 i11.L v25.1 || SAU.SUMX.U16 i7 v27
    VAU.MUL.i16 v11 v11 v1      || LSU0.LDX.128.U8.U16 v11 i18 i11 || LSU1.LDX.128.U8.U16 v21 i17 i11 || CMU.CPVI.x16 i11.L v25.2 || SAU.SUMX.U16 i8 v28
    CMU.ALIGNVEC v18 v18 v23 12 || LSU0.STI.8 i6 i16
    VAU.MUL.i16 v12 v12 v2      || LSU0.LDX.128.U8.U16 v12 i18 i11 || LSU1.LDX.128.U8.U16 v22 i17 i11 || CMU.CPVI.x16 i11.L v25.3 || SAU.SUMX.U16 i9 v29 || IAU.SHR.U32 i7 i7 8
    VAU.MUL.i16 v13 v13 v3      || LSU0.LDX.128.U8.U16 v13 i18 i11 || LSU1.LDX.128.U8.U16 v23 i17 i11 || CMU.CPVI.x16 i11.L v25.4 || SAU.SUMX.U16 i0 v10 || IAU.SHR.U32 i8 i8 8
    CMU.ALIGNVEC v19 v19 v24 12 || LSU0.STI.8 i7 i16
    VAU.MUL.i16 v14 v14 v4      || LSU0.LDX.128.U8.U16 v14 i18 i11 || LSU1.LDX.128.U8.U16 v24 i17 i11 || CMU.CPVI.x16 i11.L v25.5 || SAU.SUMX.U16 i1 v11 || IAU.SHR.U32 i9 i9 8
    VAU.MUL.i16 v15 v15 v5      || LSU0.LDX.128.U8.U16 v15 i18 i11 || LSU1.LDX.128.U8.U16 v20 i17 i11 || CMU.CPVI.x16 i11.L v25.6 || SAU.SUMX.U16 i2 v12 || IAU.SHR.U32 i0 i0 8
    CMU.ALIGNVEC v10 v10 v20 12 || LSU0.STI.8 i8 i16
    VAU.MUL.i16 v16 v16 v6      || LSU0.LDX.128.U8.U16 v16 i18 i11 || LSU1.LDX.128.U8.U16 v21 i17 i11 || CMU.CPVI.x16 i11.L v25.7 || SAU.SUMX.U16 i3 v13 || IAU.SHR.U32 i1 i1 8
    VAU.MUL.i16 v27 v17 v7      || LSU0.LDX.128.U8.U16 v17 i18 i11 || LSU1.LDX.128.U8.U16 v22 i17 i11 || CMU.CPVI.x16 i11.L v26.0 || SAU.SUMX.U16 i4 v14 || IAU.SHR.U32 i2 i2 8

loopDelaySlot:
    CMU.ALIGNVEC v11 v11 v21 12 || LSU0.STI.8 i9 i16
    VAU.MUL.i16 v28 v18 v8      || LSU0.LDX.128.U8.U16 v18 i18 i11 || LSU1.LDX.128.U8.U16 v23 i17 i11 || CMU.CPVI.x16 i11.L v26.1 || SAU.SUMX.U16 i5 v15 || IAU.SHR.U32 i3 i3 8
    VAU.MUL.i16 v29 v19 v9      || LSU0.LDX.128.U8.U16 v19 i18 i11 || LSU1.LDX.128.U8.U16 v24 i17 i11 || CMU.CPVI.x16 i11.L v25.0 || SAU.SUMX.U16 i6 v16 || IAU.SHR.U32 i4 i4 8
    CMU.ALIGNVEC v12 v12 v22 12 || LSU0.STI.8 i0 i16
    CMU.ALIGNVEC v13 v13 v23 12 || LSU0.STI.8 i1 i16                                                  || IAU.SHR.U32 i5 i5 8
    CMU.ALIGNVEC v14 v14 v24 12 || LSU0.STI.8 i2 i16               || VAU.ADD.I16 v25 v25 v30         || IAU.SHR.U32 i6 i6 8
    CMU.ALIGNVEC v15 v15 v20 12 || LSU0.STI.8 i3 i16               || VAU.ADD.I16 v26 v26 v30         || IAU.ADD i11 i11 i12


    CMU.ALIGNVEC v16 v16 v21 12
    CMU.ALIGNVEC v17 v17 v22 12
    CMU.ALIGNVEC v18 v18 v23 12

.nowarn low
    LSU0.STI.8 i4 i16 || VAU.MUL.i16 v10 v10 v0 || SAU.SUMX.U16 i7 v27
    LSU0.STI.8 i5 i16 || VAU.MUL.i16 v11 v11 v1 || SAU.SUMX.U16 i8 v28
    LSU0.STI.8 i6 i16 || VAU.MUL.i16 v12 v12 v2 || SAU.SUMX.U16 i9 v29 || IAU.SHR.U32 i7 i7 8
    LSU0.STI.8 i7 i16 || VAU.MUL.i16 v13 v13 v3 || SAU.SUMX.U16 i0 v10 || IAU.SHR.U32 i8 i8 8
    LSU0.STI.8 i8 i16 || VAU.MUL.i16 v14 v14 v4 || SAU.SUMX.U16 i1 v11 || IAU.SHR.U32 i9 i9 8
    LSU0.STI.8 i9 i16 || VAU.MUL.i16 v15 v15 v5 || SAU.SUMX.U16 i2 v12 || IAU.SHR.U32 i0 i0 8 || CMU.CMII.U32 i13 i16 
    LSU0.STI.8 i0 i16 || VAU.MUL.i16 v16 v16 v6 || SAU.SUMX.U16 i3 v13 || IAU.SHR.U32 i1 i1 8 || CMU.CMII.U32 i13 i16 || peu.pc1c GT
    LSU0.STI.8 i1 i16 || VAU.MUL.i16 v27 v17 v7 || SAU.SUMX.U16 i4 v14 || IAU.SHR.U32 i2 i2 8 || CMU.CMII.U32 i13 i16 || peu.pc1c GT
    LSU0.STI.8 i2 i16 || VAU.MUL.i16 v28 v18 v8 || SAU.SUMX.U16 i5 v15 || IAU.SHR.U32 i3 i3 8 || CMU.CMII.U32 i13 i16 || peu.pc1c GT
    LSU0.STI.8 i3 i16                           || SAU.SUMX.U16 i6 v16 || IAU.SHR.U32 i4 i4 8 || CMU.CMII.U32 i13 i16 || peu.pc1c GT
    LSU0.STI.8 i4 i16                           || SAU.SUMX.U16 i7 v27 || IAU.SHR.U32 i5 i5 8 || CMU.CMII.U32 i13 i16 || peu.pc1c GT
    LSU0.STI.8 i5 i16                           || SAU.SUMX.U16 i8 v28 || IAU.SHR.U32 i6 i6 8 || CMU.CMII.U32 i13 i16 || peu.pc1c GT
    LSU0.STI.8 i6 i16                                                  || IAU.SHR.U32 i7 i7 8 || CMU.CMII.U32 i13 i16 || peu.pc1c GT
    LSU0.STI.8 i7 i16                                                  || IAU.SHR.U32 i8 i8 8 || CMU.CMII.U32 i13 i16 || peu.pc1c GT
    LSU0.STI.8 i8 i16                                                                         || CMU.CMII.U32 i13 i16 || peu.pc1c GT
.nowarnend

//// copy back from stack reserved VRFs' values
    LSU0.LDO.64.L v24 i19 0  || LSU1.LDO.64.H v24 i19 8
    LSU0.LDO.64.L v25 i19 16 || LSU1.LDO.64.H v25 i19 24
    LSU0.LDO.64.L v26 i19 32 || LSU1.LDO.64.H v26 i19 40
    LSU0.LDO.64.L v27 i19 48 || LSU1.LDO.64.H v27 i19 56
    LSU0.LDO.64.L v30 i19 64 || LSU1.LDO.64.H v30 i19 72
    LSU0.LDO.64.L v28 i19 80 || LSU1.LDO.64.H v28 i19 88
    LSU0.LDO.64.L v29 i19 96 || LSU1.LDO.64.H v29 i19 104 || IAU.ADD i19 i19 112

    BRU.JMP i30
    nop 6
.size bilinearInterpolation_asm,.-bilinearInterpolation_asm
.end
