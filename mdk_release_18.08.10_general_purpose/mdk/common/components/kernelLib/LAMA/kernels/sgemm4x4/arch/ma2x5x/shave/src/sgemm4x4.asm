// Code for GEMM kernel
// Outline:      C[4][4] = beta * C[4][4] + alpha * A[4][k] * B[k][4]
// Number of instruction cycles: 19 (in loop) +
// Number of stall cycles:        0 (in loop)
// Error from reference output: RMS = 2.55E-8, MAX = 1.19E-7
// Copyright 2014 Movidius

.type mvcvsgemm4x4_asm,@function
.version 00.70.00

.code .text.sgemm4x4_asm
.align 16
//
//void sgemm4x4_asm(s32 k, float* alpha, float* a, float* b, float* beta, float* c, s32 rs_c, s32 cs_c)
//                   (i18)         (i17)     (i16)     (i15)        (i14)     (i13)     (i12)     (i11)
mvcvsgemm4x4_asm:
.nowarn 10 40
  // ------ IRF usage ------
  .set k          i18
  .set addrAlpha  i17
  .set addrA      i16
  .set addrB      i15
  .set addrBeta   i14
  .set addrC      i13
  .set rs_c       i12
  .set cs_c       i11

  .set incr       i0
  .set sAB0_      i1
  .set sAB1_      i2
  .set sAB2_      i3
  .set sAB3_      i4
  .set sAB0       i5
  .set sAB1       i6
  .set sAB2       i7
  .set sAB3       i8
  .set addrC1     i9
  .set addrC2     i10
  .set addrC3     i14
  .set addrCsave  i17

  // ------ VRF usage ------
  .set A0         v0
  .set A1         v1
  .set A2         v2
  .set A3         v3
  .set B0         v4
  .set B1         v5
  .set B2         v6
  .set B3         v7
  .set tB0        v8
  .set tB1        v9
  .set tB2        v10
  .set tB3        v11
  .set vMUL       v12
  .set vADD0      v13
  .set vADD0.0    v13.0
  .set vADD0.2    v13.2
  .set vADD1      v14
  .set vADD1.0    v14.0
  .set vADD1.2    v14.2
  .set vADD2      v15
  .set vADD2.0    v15.0
  .set vADD2.2    v15.2
  .set AB0        v16
  .set AB0.0      v16.0
  .set AB0.2      v16.2
  .set AB0.3      v16.3
  .set AB1        v17
  .set AB1.0      v17.0
  .set AB1.2      v17.2
  .set AB1.3      v17.3
  .set AB2        v18
  .set AB2.0      v18.0
  .set AB2.2      v18.2
  .set AB2.3      v18.3
  .set AB3        v19
  .set AB3.0      v19.0
  .set AB3.2      v19.2
  .set AB3.3      v19.3
  .set alpha      v20
  .set beta       v21

  LSU0.LDIL incr, 16         || CMU.CMZ.i32 k     || LSU1.LD.32r beta, addrBeta
  LSU0.LDXVI A0, addrA, incr || CMU.CPZV AB0, 3   || IAU.SHL cs_c, cs_c, 2
  LSU1.LDXVI B0, addrB, incr || CMU.CPZV vADD1, 2 || IAU.SHL rs_c, rs_c, 2
  LSU0.LDXVI A1, addrA, incr || PEU.PCCX.EQ 0     || BRU.BRA sgemm4x4_k_is_null
  LSU1.LDXVI B1, addrB, incr || IAU.ADDSU addrC1, addrC, cs_c
  LSU0.LDXVI A2, addrA, incr || IAU.ADDSU addrC2, addrC1, cs_c
  LSU1.LDXVI B2, addrB, incr || IAU.ADDSU addrC3, addrC2, cs_c
  LSU0.LDXVI A3, addrA, incr
  LSU1.LDXVI B3, addrB, incr
  CMU.CPZT V_ACC0, 3
  CMU.CPVID sAB0, AB0.0
  CMU.CPVID sAB2, AB0.0
  CMU.CPVID sAB0_, AB0.0
  CMU.CPVID sAB2_, AB0.0
  LSU1.LD.32r alpha, addrAlpha
  LSU1.LDXVI B0, addrB, incr
  NOP
  LSU1.LDXVI B1, addrB, incr || CMU.TP4R tB0, B0

// Do not use ".lalign"!!!
sgemm4x4_loop:
  // Normally, the instruction below should be: VAU.MACP.f32 A3, B0 || LSU0.SWZMV4.WORD [0000] [UUUU]
  // The change is necessary because of: CMU.TP4R A0, A0
  VAU.MACP.f32 A0, B0                                     || CMU.TP4R A0, A0          || LSU0.SWZMV4.WORD [3333] [UUUU]
  VAU.MUL.f32 vMUL, A0, tB0                                                           || LSU1.LDXVI A0, addrA, incr
  VAU.MUL.f32 vMUL, A0, tB1                               || CMU.CPIVD vADD1.0, sAB0
  VAU.MUL.f32 vMUL, A0, tB2                                                           || LSU1.LDXVI B2, addrB, incr
  VAU.MACP.f32 A3, B1         || SAU.SUMX.f32 sAB0, vMUL  || CMU.CPIVD vADD1.2, sAB2  || LSU0.SWZMV4.WORD [1111] [UUUU]
  VAU.MUL.f32 vMUL, A0, tB3   || SAU.SUMX.f32 sAB1, vMUL                              || LSU1.LDXVI A1, addrA, incr
  VAU.MUL.f32 vMUL, A1, tB0   || SAU.SUMX.f32 sAB2, vMUL  || CMU.CPIVD vADD2.0, sAB0_
  VAU.MUL.f32 vMUL, A1, tB1                                                           || LSU1.LDXVI B3, addrB, incr
  VAU.MACP.f32 A3, B2         || SAU.SUMX.f32 sAB3, vMUL  || CMU.CPIVD vADD2.2, sAB2_ || LSU0.SWZMV4.WORD [2222] [UUUU]
  VAU.MUL.f32 vMUL, A1, tB2   || SAU.SUMX.f32 sAB0, vMUL                              || LSU0.LDXVI A2, addrA, incr
  VAU.MUL.f32 vMUL, A1, tB3   || SAU.SUMX.f32 sAB1, vMUL  || CMU.CPIVD vADD0.0, sAB0
  VAU.MUL.f32 vMUL, A2, tB0   || IAU.SUBSU k, k, 4                                    || LSU1.LDXVI A3, addrA, incr
  VAU.MACP.f32 A3, B3         || SAU.SUMX.f32 sAB2, vMUL           || LSU0.SWZMV4.WORD [3333] [UUUU]
  VAU.MUL.f32 vMUL, A2, tB1   || SAU.SUMX.f32 sAB3, vMUL  || CMU.CPIVD vADD0.2, sAB2 || BRU.BRA sgemm4x4_loop || PEU.PCIX.NEQ 0
  VAU.MUL.f32 vMUL, A2, tB2   || SAU.SUMX.f32 sAB0_, vMUL
  VAU.MUL.f32 vMUL, A2, tB3
  VAU.ADD.f32 AB0, AB0, vADD0 || SAU.SUMX.f32 sAB1_, vMUL
  VAU.ADD.f32 AB2, AB2, vADD2 || SAU.SUMX.f32 sAB2_, vMUL  || LSU1.LDXVI B0, addrB, incr
  VAU.ADD.f32 AB1, AB1, vADD1 || SAU.SUMX.f32 sAB3_, vMUL
   CMU.TP4R tB0, B0  || LSU1.LDXVI B1, addrB, incr
// ~bli_sgemm_4x4_loop

  // Scale A*B by alpha
                                 CMU.CPIVD vADD1.0, sAB0
  VAU.ACCPW.i32 AB3, AB3      || CMU.CPIVD vADD1.2, sAB2
  VAU.MUL.f32 AB0, AB0, alpha || CMU.CPIVD vADD2.0, sAB0_
  VAU.ADD.f32 AB1, AB1, vADD1
  VAU.MUL.f32 AB3, AB3, alpha || CMU.CPIVD vADD2.2, sAB2_
sgemm4x4_k_is_null:
  // Load C and scale by beta and store the result
  VAU.ADD.f32 AB2, AB2, vADD2    || LSU0.LDI.32r v3,  addrC,  rs_c || LSU1.LDI.32r v2,  addrC1, rs_c || IAU.ADDSU addrCsave, addrC, 0
  VAU.MUL.f32 AB1, AB1, alpha    || LSU0.LDI.32r v1,  addrC2, rs_c || LSU1.LDI.32r v0,  addrC3, rs_c
                                    LSU0.LDI.32r v7,  addrC, rs_c  || LSU1.LDI.32r v6,  addrC1, rs_c
  VAU.MUL.f32 AB2, AB2, alpha    || LSU0.LDI.32r v5,  addrC2, rs_c || LSU1.LDI.32r v4,  addrC3, rs_c
                                    LSU0.LDI.32r v11, addrC, rs_c  || LSU1.LDI.32r v10, addrC1, rs_c
                                    LSU0.LDI.32r v9,  addrC2, rs_c || LSU1.LDI.32r  v8, addrC3, rs_c
  IAU.ADDSU addrC, addrCsave, 0  || LSU0.LD.32r v15, addrC         || LSU1.LD.32r  v14, addrC1
  IAU.ADDSU addrC1, addrC, cs_c  || LSU0.LD.32r v13, addrC2        || LSU1.LD.32r  v12, addrC3
  IAU.ADDSU addrC2, addrC1, cs_c || CMU.CPVCR.x32 v0, v0.0
  VAU.MUL.f32 v0, v0, beta       || IAU.ADDSU addrC3, addrC2, cs_c
                                    CMU.CPVCR.x32 v1, v4.0
  VAU.MUL.f32 v1, v1, beta
  VAU.ADD.f32 AB0, AB0, v0       || CMU.CPVCR.x32 v2, v8.0
  VAU.MUL.f32 v2, v2, beta
  VAU.ADD.f32 AB1, AB1, v1       || CMU.CPVCR.x32 v3, v12.0
  VAU.MUL.f32 v3, v3, beta       || CMU.CPVID sAB0, AB0.0          || LSU0.CP sAB2, AB0.2 || LSU1.CP sAB3, AB0.3
  VAU.ADD.f32 AB2, AB2, v2       || LSU0.STI.32 sAB0, addrC, rs_c  || LSU1.STI.32 sAB1, addrC1, rs_c
                                    LSU0.STI.32 sAB2, addrC2, rs_c || LSU1.STI.32 sAB3, addrC3, rs_c
  VAU.ADD.f32 AB3, AB3, v3
                                    CMU.CPVID sAB0, AB1.0          || LSU0.CP sAB2, AB1.2 || LSU1.CP sAB3, AB1.3
  LSU0.STI.32 sAB0, addrC, rs_c  || LSU1.STI.32 sAB1, addrC1, rs_c
  LSU0.STI.32 sAB2, addrC2, rs_c || LSU1.STI.32 sAB3, addrC3, rs_c
  NOP
                                    CMU.CPVID sAB0, AB2.0          || LSU0.CP sAB2, AB2.2 || LSU1.CP sAB3, AB2.3
  LSU0.STI.32 sAB0, addrC, rs_c  || LSU1.STI.32 sAB1, addrC1, rs_c
  LSU0.STI.32 sAB2, addrC2, rs_c || LSU1.STI.32 sAB3, addrC3, rs_c
  BRU.JMP i30
                                    CMU.CPVID sAB0, AB3.0          || LSU0.CP sAB2, AB3.2 || LSU1.CP sAB3, AB3.3
  LSU0.ST.32 sAB0, addrC         || LSU1.ST.32 sAB1, addrC1
  LSU0.ST.32 sAB2, addrC2        || LSU1.ST.32 sAB3, addrC3
  NOP 3

.nowarnend
.size mvcvsgemm4x4_asm,.-mvcvsgemm4x4_asm
.end
