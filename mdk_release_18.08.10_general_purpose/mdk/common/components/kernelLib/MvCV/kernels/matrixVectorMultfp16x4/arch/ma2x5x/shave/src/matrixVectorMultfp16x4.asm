// Code for matrixVectorMultfp16x4 kernel
// Number of instruction cycles: 10 (in loop) +
// Number of stall cycles:        0 (in loop)
// Copyright 2015 Movidius

.type mvcvmatrixVectorMultfp16x4_asm,@function
.version 00.70.00

.code .text.matrixVectorMultfp16x4_asm

.align 16

//
//void mvcvmatrixVectorMultfp16x4_asm(fp16 **inArrayA, fp16 *inVecB, u32 num_elements, fp16 *outVec)
//                                              (i18)         (i17)             (i16)         (i15)
mvcvmatrixVectorMultfp16x4_asm:

  // ------ IRF usage ------
  .set inArrayA     i18
  .set inVecB       i17
  .set num_elements i16
  .set outVec       i15

  .set incr         i0
  .set inArrayA0    i2
  .set inArrayA1    i3
  .set inArrayA2    i4
  .set inArrayA3    i5
  .set iA00         i6
  .set iA01         i7
  .set iA02         i8
  .set iA03         i9
  .set iA10         i10
  .set iA11         i11
  .set iA12         i12
  .set iA13         i13

  // ------ VRF usage ------
  .set vB0           v0
  .set vB1           v1
  .set vA00          v2
  .set vA01          v3
  .set vA02          v4
  .set vA03          v5
  .set vA10          v6
  .set vA11          v7
  .set vA12          v8
  .set vA13          v9
  .set vDot0         v10
  .set vDot0.0       v10.0
  .set vDot0.2       v10.2
  .set vDot1         v11
  .set vDot1.0       v11.0
  .set vDot1.2       v11.2

  LSU1.LD.64 inArrayA0, inArrayA     || CMU.CPZV vDot0, 1
  LSU1.LDO.64 inArrayA2, inArrayA, 8 || CMU.CPZV vA10, 3
  LSU0.LDIL incr, 16
  CMU.CPVID iA00, vDot0.0
  CMU.CPVID iA02, vDot0.0
  CMU.CPVID iA10, vDot0.0
  VAU.ACCPZ.u16 vDot0
  LSU1.LDXVI vB0, inVecB, incr
  LSU0.LDXVI vA00, inArrayA0, incr
  LSU1.LDXVI vA01, inArrayA1, incr
  LSU0.LDXVI vA02, inArrayA2, incr
  LSU1.LDXVI vA03, inArrayA3, incr
  LSU0.LDXVI vB1, inVecB, incr
  LSU1.LDXVI vA10, inArrayA0, incr
  LSU0.LDXVI vA11, inArrayA1, incr
  LSU1.LDXVI vA12, inArrayA2, incr
  LSU0.LDXVI vA13, inArrayA3, incr
.lalign
__matrixVectorMultfp16x4_loop:
.nowarn 10, 40
  VAU.MUL.f16 vA00, vA00, vB0 || LSU1.LDXVI vB0, inVecB, incr     || SAU.SUMX.f16 iA12, vA12
  VAU.MUL.f16 vA01, vA01, vB0 || LSU0.LDXVI vA00, inArrayA0, incr || SAU.SUMX.f16 iA13, vA13 || CMU.CPIVD vDot0.2, iA02
  VAU.MUL.f16 vA02, vA02, vB0 || LSU1.LDXVI vA01, inArrayA1, incr                            || IAU.SUBSU num_elements, num_elements, 16
  VAU.MUL.f16 vA03, vA03, vB0 || LSU0.LDXVI vA02, inArrayA2, incr || SAU.SUMX.f16 iA00, vA00 || PEU.PCIX.NEQ 0x00 || BRU.BRA __matrixVectorMultfp16x4_loop
  VAU.ACCP.f16 vDot0          || LSU1.LDXVI vA03, inArrayA3, incr || SAU.SUMX.f16 iA01, vA01
  VAU.MUL.f16 vA10, vA10, vB1 || LSU0.LDXVI vB1, inVecB, incr     || SAU.SUMX.f16 iA02, vA02 || CMU.CPIVD vDot1.0, iA10
  VAU.MUL.f16 vA11, vA11, vB1 || LSU1.LDXVI vA10, inArrayA0, incr || SAU.SUMX.f16 iA03, vA03
  VAU.MUL.f16 vA12, vA12, vB1 || LSU0.LDXVI vA11, inArrayA1, incr                            || CMU.CPIVD vDot1.2, iA12
  VAU.MUL.f16 vA13, vA13, vB1 || LSU1.LDXVI vA12, inArrayA2, incr || SAU.SUMX.f16 iA10, vA10
  VAU.ACCP.f16 vDot1          || LSU0.LDXVI vA13, inArrayA3, incr || SAU.SUMX.f16 iA11, vA11 || CMU.CPIVD vDot0.0, iA00
  //~__matrixVectorMultfp16x4_loop
.nowarnend
  SAU.SUMX.f16 iA12, vA12
  SAU.SUMX.f16 iA13, vA13 || CMU.CPIVD vDot0.2, iA02
  VAU.ACCP.f16 vDot0
  NOP
  CMU.CPIVD vDot1.0, iA10
  NOP
  CMU.CPIVD vDot1.2, iA12
  VAU.ACCPW.f16 vDot0, vDot1
  BRU.JMP i30
  NOP
  CMU.VDILV.x16 vA00, vDot0, vDot0, vDot0
  LSU0.ST.64.l vDot0, outVec
  NOP 3

 BRU.JMP i30
 NOP 6
.size mvcvmatrixVectorMultfp16x4_asm,.-mvcvmatrixVectorMultfp16x4_asm
 .end
