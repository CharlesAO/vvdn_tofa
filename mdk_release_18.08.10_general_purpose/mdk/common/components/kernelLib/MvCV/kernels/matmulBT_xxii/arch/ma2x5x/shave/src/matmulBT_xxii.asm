// @file
// @copyright All code copyright Movidius Ltd 2016, all rights reserved
//            For License Warranty see: mdk/common/license.txt
//
// Code for matmul_BT_xxii kernel -> C += A * B'
// Number of instruction cycles: 49 + m/2*( 31 + n/8*( 28 + 16*k/16 ) )
// Number of stall cycles:       0
// Error from reference output: RMS = , MAX =
// Constraints:       m%2  = 0
//                    k%16 = 0
//                    n%8  = 0

.type mvcvMatmulBT_xxii_asm,@function
.version 00.70.00

.code .text.mvcvMatmulBT_xxii_asm
.code .text
.salign 16
//------------------------------------------------------------------------------------------------------------
//void matmulBT_xxii_asm(const half *A, const half *B, half *C, int m, int k, int n, int wA, int wB, int wC)//
//                            (i18)       (i17)        (i16)   (i15)  (i14)   (i13)  (i12)   (i11)   (stack)
//------------------------------------------------------------------------------------------------------------
mvcvMatmulBT_xxii_asm:
.nowarn 9,10,11,12
  // ------ IRF usage ------
  .set A            i18
  .set B            i17
  .set C            i16
  .set m            i15
  .set k            i14
  .set n            i13
  .set wA           i12
  .set wB           i11
  .set wC           i10

  .set k_by_16      k

  .set pA0          i0
  .set pA1          i1
  .set pB0          i2
  .set pB1          i3
  .set pBstart      i4
  .set incr         i5
  .set row_out      i6
  .set col_out      i7
  .set iC_tmp       i8
  .set incr_pBstart i9


  .set pC0           i20
  .set pC1           i21

  .set iC00          i22
  .set iC01          i23
  .set iC02          i24
  .set iC03          i25
  .set iC04          i26
  .set iC05          i27
  .set iC06          i28
  .set iC07          i29

  .set loop_label    i30
  .set temp          i31
  .set incr_pB       temp

  .set iC10          wA
  .set iC11          n
  .set iC12          m
  .set iC13          C
  .set iC14          B
  .set iC15          A
  .set iC16          row_out
  .set iC17          col_out

  // ------ VRF usage ------
  .set vB0           v0
  .set vB1           v1
  .set vB2           v2
  .set vB3           v3
  .set vB4           v4
  .set vB5           v5
  .set vB6           v6
  .set vB7           v7
  .set vA0           v8
  .set vA1           v9
  .set vC0_temp      v10
  .set vB0res        v12
  .set vB1res        v13
  .set vB2res        v14
  .set vB3res        v15
  .set vB4res        v16
  .set vB5res        v17
  .set vB6res        v18
  .set vB7res        v19
  .set vC0           v20
  .set vC0.0         v20.0
  .set vC0.1         v20.1
  .set vC0.2         v20.2
  .set vC0.3         v20.3
  .set vC1           v21
  .set vC1.0         v21.0
  .set vC1.1         v21.1
  .set vC1.2         v21.2
  .set vC1.3         v21.3
  .set vMIN          v22
  .set vMAX          v23

  // save IRF registers to stack
  LSU0.LD.32  wC, i19  || IAU.SUB i19 i19 4
  LSU0.ST.32  i20  i19 || IAU.SUB i19 i19 4
  LSU0.ST.32  i21  i19 || IAU.SUB i19 i19 4
  LSU0.ST.32  i22  i19 || IAU.SUB i19 i19 4
  LSU0.ST.32  i23  i19 || IAU.SUB i19 i19 4
  LSU0.ST.32  i24  i19 || IAU.SUB i19 i19 4
  LSU0.ST.32  i25  i19 || IAU.SUB i19 i19 4
  LSU0.ST.32  i26  i19 || IAU.SUB i19 i19 4
  LSU0.ST.32  i27  i19 || IAU.SUB i19 i19 4
  LSU0.ST.32  i28  i19 || IAU.SUB i19 i19 4
  LSU0.ST.32  i29  i19 || IAU.SUB i19 i19 4
  LSU0.ST.32  i30  i19 || IAU.SUB i19 i19 4
  LSU0.ST.32  i31  i19

  LSU0.LDIL temp, 0x0000 || LSU1.LDIH temp, 0x8000 || IAU.SHL wC, wC, 2
  LSU0.LDIL temp, 0xFFFF || LSU1.LDIH temp, 0x7FFF || CMU.CPIVR.X32 vMIN, temp
  LSU0.LDIL incr_pB, 16  || LSU1.LDIH incr_pB, 0   || CMU.CPIVR.X32 vMAX, temp || IAU.SHR.u32 k_by_16, k, 4

  // pBstart increment = 8*wB
  // pB incremet = 16-8*wB+wB
  IAU.SHL incr_pBstart, wB, 3
  IAU.SUB incr_pB, incr_pB, incr_pBstart
  IAU.ADD incr_pB, incr_pB, wB
  LSU1.LDIH incr_pB, 0

  LSU0.LDIL incr, 16     || LSU1.LDIH incr, 0 || IAU.XOR row_out, row_out, row_out
  CMU.CPII pBstart, B                         || IAU.XOR col_out, col_out, col_out
  CMU.CPII pA0, A                             || IAU.ADD pA1, A, wA
  CMU.CPII pC0, C                             || IAU.ADD pC1, C, wC
  LSU0.LDIL loop_label, __matmulBT_xxii_loop  || LSU1.LDIH loop_label, __matmulBT_xxii_loop
  LSU0.LD.64.L vC0, pC0        || LSU1.LDO.64.H vC0, pC0, 8  || CMU.CPZV vB7res, 0
  LSU0.LD.64.L vC1, pC1        || LSU1.LDO.64.H vC1, pC1, 8  || CMU.CPII pB0, pBstart
  LSU0.STO.32 wA, i19, -4      || LSU1.STO.32  n, i19, -8    || IAU.ADD  pB1, pBstart, 8
  LSU0.LDI.64.L vA0, pA0, incr || LSU1.LDO.64.H vA0, pA0, 8  || IAU.XOR iC_tmp, iC_tmp, iC_tmp
  LSU0.STO.32  B, i19, -12       || LSU1.STO.32  C, i19, -16
  LSU0.STO.32  m, i19, -20       || LSU1.STO.32  row_out, i19, -24

.lalign
_matmulBT_xxii_row_out:
//    for (row_out = 0// row_out < m// row_out+=2)
  LSU0.LDO.64.L vC0, pC0, 16     || LSU1.LDO.64.H vC0, pC0, 24
  LSU0.LDO.64.L vC1, pC1, 16     || LSU1.LDO.64.H vC1, pC1, 24
  LSU0.LDI.64.L vB0, pB0, wB     || LSU1.LD.64.H vB0, pB1 || IAU.ADD pB1, pB1, wB
  LSU0.LDI.64.L vB1, pB0, wB     || LSU1.LD.64.H vB1, pB1 || IAU.ADD pB1, pB1, wB || CMU.CPVID iC00, vC0.0
  LSU0.LDI.64.L vB2, pB0, wB     || LSU1.LD.64.H vB2, pB1 || IAU.ADD pB1, pB1, wB || CMU.CPVID iC02, vC0.2
                                                                                     CMU.CPVID iC10, vC1.0
  LSU0.STO.32  A, i19, -28       || LSU1.STO.32  col_out, i19, -32                || CMU.CPVID iC12, vC1.2
  LSU0.LDI.64.L vB3, pB0, wB   || LSU1.LD.64.H vB3, pB1 || IAU.ADD pB1, pB1, wB   || CMU.CPVID iC04, vC0.0
  LSU0.LDI.64.L vB4, pB0, wB   || LSU1.LD.64.H vB4, pB1 || IAU.ADD pB1, pB1, wB   || CMU.CPVID iC06, vC0.2
  LSU0.LDI.64.L vB5, pB0, wB   || LSU1.LD.64.H vB5, pB1 || IAU.ADD pB1, pB1, wB   || CMU.CPVID iC14, vC1.0
  LSU0.LDI.64.L vB6, pB0, wB   || LSU1.LD.64.H vB6, pB1 || IAU.ADD pB1, pB1, wB   || CMU.CPVID iC16, vC1.2
  LSU0.LDI.64.L vB7, pB0, incr_pB || LSU1.LDI.64.H vB7, pB1, incr_pB                                     || VAU.MUL.u8f vB0res, vB0, vA0
  LSU0.LD.64.L vA1, pA1        || LSU1.LDO.64.H vA1, pA1, 8 || IAU.ADD pA1, pA1, incr                    || VAU.MUL.u8f vB1res, vB1, vA0

.lalign
_matmulBT_xxii_col_out:
//for (col_out = 0// col_out < n// col_out +=8)

// for (common_dim = 0 .. k/8)

.lalign
//__matmulBT_xxii_loop:
  VAU.MUL.u8f vB2res, vB2, vA0  || SAU.SUMX.u8 iC_tmp, vB7res || IAU.ADDSI iC15, iC15, iC_tmp   || LSU0.LDI.64.L vA0, pA0, incr || LSU1.LDO.64.H vA0, pA0, 8 || BRU.RPL loop_label, k_by_16
  VAU.MUL.u8f vB3res, vB3, vA0  || SAU.SUMX.u8 iC_tmp, vB0res || IAU.ADDSI iC16, iC16, iC_tmp   || LSU0.LDI.64.L vB0, pB0, wB   || LSU1.LDI.64.H vB0, pB1, wB
  VAU.MUL.u8f vB4res, vB4, vA0  || SAU.SUMX.u8 iC_tmp, vB1res || IAU.ADDSI iC17, iC17, iC_tmp   || LSU0.LDI.64.L vB1, pB0, wB   || LSU1.LDI.64.H vB1, pB1, wB
  VAU.MUL.u8f vB5res, vB5, vA0  || SAU.SUMX.u8 iC_tmp, vB2res || IAU.ADDSI iC00, iC00, iC_tmp   || LSU0.LDI.64.L vB2, pB0, wB   || LSU1.LDI.64.H vB2, pB1, wB
  VAU.MUL.u8f vB6res, vB6, vA0  || SAU.SUMX.u8 iC_tmp, vB3res || IAU.ADDSI iC01, iC01, iC_tmp   || LSU0.LDI.64.L vB3, pB0, wB   || LSU1.LDI.64.H vB3, pB1, wB
  VAU.MUL.u8f vB7res, vB7, vA0  || SAU.SUMX.u8 iC_tmp, vB4res || IAU.ADDSI iC02, iC02, iC_tmp   || LSU0.LDI.64.L vB4, pB0, wB   || LSU1.LDI.64.H vB4, pB1, wB
  VAU.MUL.u8f vB0res, vB0, vA1  || SAU.SUMX.u8 iC_tmp, vB5res || IAU.ADDSI iC03, iC03, iC_tmp   || LSU0.LDI.64.L vB5, pB0, wB   || LSU1.LDI.64.H vB5, pB1, wB
  VAU.MUL.u8f vB1res, vB1, vA1  || SAU.SUMX.u8 iC_tmp, vB6res || IAU.ADDSI iC04, iC04, iC_tmp   || LSU0.LDI.64.L vB6, pB0, wB   || LSU1.LDI.64.H vB6, pB1, wB
  VAU.MUL.u8f vB2res, vB2, vA1  || SAU.SUMX.u8 iC_tmp, vB7res || IAU.ADDSI iC05, iC05, iC_tmp   || LSU0.LDI.64.L vB7, pB0, incr_pB || LSU1.LDI.64.H vB7, pB1, incr_pB
__matmulBT_xxii_loop:
  VAU.MUL.u8f vB3res, vB3, vA1  || SAU.SUMX.u8 iC_tmp, vB0res || IAU.ADDSI iC06, iC06, iC_tmp   || LSU0.LDI.64.L vA1, pA1, incr    || LSU1.LDO.64.H vA1, pA1, 8
  VAU.MUL.u8f vB4res, vB4, vA1  || SAU.SUMX.u8 iC_tmp, vB1res || IAU.ADDSI iC07, iC07, iC_tmp
  VAU.MUL.u8f vB5res, vB5, vA1  || SAU.SUMX.u8 iC_tmp, vB2res || IAU.ADDSI iC10, iC10, iC_tmp
  VAU.MUL.u8f vB6res, vB6, vA1  || SAU.SUMX.u8 iC_tmp, vB3res || IAU.ADDSI iC11, iC11, iC_tmp
  VAU.MUL.u8f vB7res, vB7, vA1  || SAU.SUMX.u8 iC_tmp, vB4res || IAU.ADDSI iC12, iC12, iC_tmp
  VAU.MUL.u8f vB0res, vB0, vA0  || SAU.SUMX.u8 iC_tmp, vB5res || IAU.ADDSI iC13, iC13, iC_tmp
  VAU.MUL.u8f vB1res, vB1, vA0  || SAU.SUMX.u8 iC_tmp, vB6res || IAU.ADDSI iC14, iC14, iC_tmp
//~__matmulBT_xxii_loop

  LSU0.CP vC1.2, iC12           || LSU1.CP vC1.3, iC13            || CMU.CPIVD vC1.0, iC10  || SAU.SUMX.u8 iC_tmp, vB7res                 || IAU.ADDSI iC15, iC15, iC_tmp
  LSU0.LDO.32  A, i19, -28      || LSU1.LDO.32  col_out, i19, -32                   || SAU.ADD.I32 pBstart, pBstart, incr_pBstart || IAU.ADDSI iC16, iC16, iC_tmp
  LSU0.CP vC0.2, iC02           || LSU1.CP vC0.3, iC03            || CMU.CLAMPAB.I32 vC1, vMIN, vMAX
  LSU0.LDO.32  wA, i19, -4      || LSU1.LDO.32  n, i19, -8                                           || IAU.ADD  pB1, pBstart, 8
  LSU0.STI.64.L vC1, pC1, incr  || LSU1.STO.64.H vC1, pC1, 8      || CMU.CPZV vB7res, 0              || IAU.ADDSI iC17, iC17, iC_tmp
  LSU0.CP vC1.0, iC14           || LSU1.CP vC1.1, iC15            || CMU.CPIVD vC1.2, iC16           || IAU.XOR iC_tmp, iC_tmp, iC_tmp
                                                                     CMU.CLAMPAB.I32 vC1, vMIN, vMAX
  LSU0.CP vC0.0, iC00           || LSU1.CP vC0.1, iC01            || CMU.CPII pB0, pBstart
  LSU0.STI.64.L vC1, pC1, incr  || LSU1.STO.64.H vC1, pC1, 8
  LSU0.LD.64.L vC1, pC1         || LSU1.LDO.64.H vC1, pC1, 24     || CMU.CLAMPAB.I32 vC0, vMIN, vMAX || IAU.ADD col_out, col_out, 8
  LSU0.STO.32  A, i19, -28      || LSU1.STO.32  col_out, i19, -32
  LSU0.STI.64.L vC0, pC0, incr  || LSU1.STO.64.H vC0, pC0, 8      || CMU.CPII pA0, A                 || IAU.ADD pA1, A, wA
  LSU0.CP vC0.0, iC04 || LSU1.CP vC0.1, iC05                      || CMU.CPIVD vC0.2, iC06           || IAU.SUB n, n, col_out
  LSU0.LDI.64.L vA0, pA0, incr  || LSU1.LDO.64.H vA0, pA0, 8      || CMU.CLAMPAB.I32 vC0, vMIN, vMAX
  LSU0.LDO.64.L vC0, pC0, 16    || LSU1.LDO.64.H vC0, pC0, 24     || CMU.CMZ.I32 n                   || IAU.ADD n, n, col_out
  LSU0.STO.32 wA, i19, -4       || LSU1.STO.32  n, i19, -8
  LSU0.LDO.64.L vC1, pC1, 16    || LSU1.LDO.64.H vC1, pC1, 24
  LSU0.STI.64.L vC0, pC0, incr  || LSU1.STO.64.H vC0, pC0, 8
  LSU0.LDO.64.L vC0, pC0, 16    || LSU1.LDO.64.H vC0, pC0, 40
  LSU0.LDI.64.L vB0, pB0, wB    || LSU1.LD.64.H vB0, pB1 || IAU.ADD pB1, pB1, wB || CMU.CPVID iC10, vC1.0
  LSU0.LDI.64.L vB1, pB0, wB    || LSU1.LD.64.H vB1, pB1 || IAU.ADD pB1, pB1, wB || CMU.CPVID iC12, vC1.2
  LSU0.LDI.64.L vB2, pB0, wB    || LSU1.LD.64.H vB2, pB1 || IAU.ADD pB1, pB1, wB || CMU.CPVID iC00, vC0.0 || PEU.PCCX.NEQ 0x00 || BRU.BRA _matmulBT_xxii_col_out
  LSU0.LDI.64.L vB3, pB0, wB    || LSU1.LD.64.H vB3, pB1 || IAU.ADD pB1, pB1, wB || CMU.CPVID iC02, vC0.2
  LSU0.LDI.64.L vB4, pB0, wB    || LSU1.LD.64.H vB4, pB1 || IAU.ADD pB1, pB1, wB || CMU.CPVID iC14, vC1.0
  LSU0.LDI.64.L vB5, pB0, wB    || LSU1.LD.64.H vB5, pB1 || IAU.ADD pB1, pB1, wB || CMU.CPVID iC16, vC1.2
  LSU0.LDI.64.L vB6, pB0, wB    || LSU1.LD.64.H vB6, pB1 || IAU.ADD pB1, pB1, wB || CMU.CPVID iC04, vC0.0
  LSU0.LDI.64.L vB7, pB0, incr_pB || LSU1.LDI.64.H vB7, pB1, incr_pB             || CMU.CPVID iC06, vC0.2  || VAU.MUL.u8f vB0res, vB0, vA0
  LSU0.LDI.64.L vA1, pA1, incr    || LSU1.LDO.64.H vA1, pA1, 8                                             || VAU.MUL.u8f vB1res, vB1, vA0

//_matmulBT_xxii_col_out
  LSU0.LDO.32  m, i19, -20 || LSU1.LDO.32  row_out, i19, -24
  LSU0.LDO.32  A, i19, -28 || LSU1.LDO.32  wA, i19, -4
  LSU0.LDO.32  C, i19, -16 || LSU1.LDO.32  B, i19, -12
  LSU0.LDO.32  n, i19, -8  || LSU1.LDO.32  col_out, i19, -32
  nop 3
  IAU.ADD row_out, row_out, 2
  IAU.ADD A, A, wA
  IAU.ADD A, A, wA
  IAU.ADD C, C, wC                                                                                 || CMU.CMII.I32 row_out, m
  IAU.ADD C, C, wC              || PEU.PC1C LT  || BRU.BRA _matmulBT_xxii_row_out
  LSU0.STO.32 wA, i19, -4      || LSU1.STO.32  n, i19, -8                                          || CMU.CPII pC0, C
  LSU0.LD.64.L vC0, pC0        || LSU1.LDO.64.H vC0, pC0, 8  || IAU.ADD pC1, C, wC
  LSU0.LD.64.L vC1, pC1        || LSU1.LDO.64.H vC1, pC1, 8  || IAU.XOR col_out, col_out, col_out  || CMU.CPII pBstart, B
  LSU0.STO.32  m, i19, -20     || LSU1.STO.32  row_out, i19, -24 || IAU.ADD  pB1, pBstart, 8       || CMU.CPII pA0, A
  LSU0.LDI.64.L vA0, pA0, incr || LSU1.LDO.64.H vA0, pA0, 8  || IAU.XOR iC_tmp, iC_tmp, iC_tmp     || CMU.CPII pB0, pBstart
  LSU0.STO.32  B, i19, -12     || LSU1.STO.32  C, i19, -16   || IAU.ADD pA1, A, wA                 || CMU.CPZV vB7res, 0

// _matmulBT_xxii_row_out


// restore used IRF registers from stack
  LSU0.LD.32  i31  i19 || IAU.ADD i19 i19 4
  LSU0.LD.32  i30  i19 || IAU.ADD i19 i19 4
  LSU0.LD.32  i29  i19 || IAU.ADD i19 i19 4
  LSU0.LD.32  i28  i19 || IAU.ADD i19 i19 4
  LSU0.LD.32  i27  i19 || IAU.ADD i19 i19 4
  LSU0.LD.32  i26  i19 || IAU.ADD i19 i19 4
  LSU0.LD.32  i25  i19 || IAU.ADD i19 i19 4
  LSU0.LD.32  i24  i19 || IAU.ADD i19 i19 4
  LSU0.LD.32  i23  i19 || IAU.ADD i19 i19 4
  LSU0.LD.32  i22  i19 || IAU.ADD i19 i19 4
  LSU0.LD.32  i21  i19 || IAU.ADD i19 i19 4
  LSU0.LD.32  i20  i19 || IAU.ADD i19 i19 4
  BRU.JMP i30
.nowarnend
  NOP 6
  .size mvcvMatmulBT_xxii_asm,.-mvcvMatmulBT_xxii_asm
  .end
