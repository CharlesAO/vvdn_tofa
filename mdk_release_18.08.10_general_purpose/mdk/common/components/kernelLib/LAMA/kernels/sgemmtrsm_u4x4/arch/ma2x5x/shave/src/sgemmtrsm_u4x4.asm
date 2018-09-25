// Code for GEMMTRSM upper kernel
// Outline:      b11 = alpha * b11 - a12 * b21// */
//               b11 = inv(a11) * b11//
//               c11 = b11//
// Number of instruction cycles:
// Number of stall cycles:
// Error from reference output: RMS = 2.44E-8, MAX = 1.19E-7
// Copyright 2014 Movidius

.type mvcvsgemmtrsm_u4x4_asm,@function
.version 00.70.00

.code .text.sgemmtrsm_u4x4_asm
.align 16
//
//void mvcvsgemmtrsm_u_4x4_asm(int k, float *alpha, float *a12, float *a11, float *bd21, float *b11, float *c11, int rs_c, int cs_c)
//                              (i18)         (i17)       (i16)       (i15)        (i14)       (i13)       (i12)     (i11)    (*i19)

mvcvsgemmtrsm_u4x4_asm:
.nowarn 10 40
  // ------ IRF usage ------
  // sgemm + strsm
  .set k          i18
  .set addrAlpha  i17
  .set addrA12    i16
  .set addrA11    i15
  .set addrBD21   i14
  .set addrB11    i13
  .set addrC11    i12
  .set rs_c       i11
  .set cs_c       i10
  .set incr       i0
  // sgemm only
  .set sAB0_      i1
  .set sAB1_      i2
  .set sAB2_      i3
  .set sAB3_      i4
  .set sAB0       i5
  .set sAB1       i6
  .set sAB2       i7
  .set sAB3       i8
   // strsm only
  .set addrC1     i1
  .set addrC2     i2
  .set addrC3     i3
  .set sC0        i4
  .set sC1        i5
  .set sC2        i6
  .set sC3        i7
  .set incr_1     i8
  .set addrB11save i9
  .set rs_cx3     i7

  // ------ VRF usage ------
  // sgemm + strsm
  .set A0         v0
  .set A1         v1
  .set A2         v2
  .set A3         v3
  .set B0         v4
  .set B1         v5
  .set B2         v6
  .set B3         v7
  // sgemm only
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
  .set AB1        v17
  .set AB2        v18
  .set AB3        v19
  .set alpha      v20
  // strsm only
  .set A0.0       v0.0
  .set A0.1       v0.1
  .set B0.0       v4.0
  .set B0.1       v4.1
  .set B0.2       v4.2
  .set B0.3       v4.3
  .set B1.0       v5.0
  .set B1.1       v5.1
  .set B1.2       v5.2
  .set B1.3       v5.3
  .set B2.0       v6.0
  .set B2.1       v6.1
  .set B2.2       v6.2
  .set B2.3       v6.3
  .set B3.0       v7.0
  .set B3.1       v7.1
  .set B3.2       v7.2
  .set B3.3       v7.3
  .set _B2        v8
  .set _B1        v9
  .set __B1       v10
  .set _B0        v11
  .set __B0       v12
  .set ___B0      v13
  .set _A0        v14
  .set _A1        v15
  .set _A2        v16

  LSU0.LDIL incr, 16            || CMU.CMZ.i32 k     || LSU1.LD.32r alpha, addrAlpha
  LSU0.LDXVI A0, addrA12, incr  || CMU.CPZV AB0, 3
  LSU1.LDXVI B0, addrBD21, incr || CMU.CPZV vADD1, 2
  LSU0.LDXVI A1, addrA12, incr  || PEU.PCCX.EQ 0     || BRU.BRA sgemmtrsm_u4x4_k_is_null
  LSU1.LDXVI B1, addrBD21, incr
  LSU0.LDXVI A2, addrA12, incr
  LSU1.LDXVI B2, addrBD21, incr
  LSU0.LDXVI A3, addrA12, incr
  LSU1.LDXVI B3, addrBD21, incr
  CMU.CPZT V_ACC0, 3
  CMU.CPVID sAB0, AB0.0
  CMU.CPVID sAB2, AB0.0
  CMU.CPVID sAB0_, AB0.0
  CMU.CPVID sAB2_, AB0.0
  NOP
  LSU1.LDXVI B0, addrBD21, incr
  NOP
  LSU1.LDXVI B1, addrBD21, incr || CMU.TP4R tB0, B0

sgemmtrsm_u4x4_loop:
  // Normally, the instruction below should be: VAU.MACP.f32 A3, B0 || LSU0.SWZMV4.WORD [0000] [UUUU]
  // The change is necessary because of: CMU.TP4R A0, A0
  VAU.MACP.f32 A0, B0                                     || CMU.TP4R A0, A0          || LSU0.SWZMV4.WORD [3333] [UUUU]
  VAU.MUL.f32 vMUL, A0, tB0                                                           || LSU1.LDXVI A0, addrA12, incr
  VAU.MUL.f32 vMUL, A0, tB1                               || CMU.CPIVD vADD1.0, sAB0
  VAU.MUL.f32 vMUL, A0, tB2                                                           || LSU1.LDXVI B2, addrBD21, incr
  VAU.MACP.f32 A3, B1         || SAU.SUMX.f32 sAB0, vMUL  || CMU.CPIVD vADD1.2, sAB2  || LSU0.SWZMV4.WORD [1111] [UUUU]
  VAU.MUL.f32 vMUL, A0, tB3   || SAU.SUMX.f32 sAB1, vMUL                              || LSU1.LDXVI A1, addrA12, incr
  VAU.MUL.f32 vMUL, A1, tB0   || SAU.SUMX.f32 sAB2, vMUL  || CMU.CPIVD vADD2.0, sAB0_
  VAU.MUL.f32 vMUL, A1, tB1                                                           || LSU1.LDXVI B3, addrBD21, incr
  VAU.MACP.f32 A3, B2         || SAU.SUMX.f32 sAB3, vMUL  || CMU.CPIVD vADD2.2, sAB2_ || LSU0.SWZMV4.WORD [2222] [UUUU]
  VAU.MUL.f32 vMUL, A1, tB2   || SAU.SUMX.f32 sAB0, vMUL                              || LSU0.LDXVI A2, addrA12, incr
  VAU.MUL.f32 vMUL, A1, tB3   || SAU.SUMX.f32 sAB1, vMUL  || CMU.CPIVD vADD0.0, sAB0
  VAU.MUL.f32 vMUL, A2, tB0   || IAU.SUBSU k, k, 4                                    || LSU1.LDXVI A3, addrA12, incr
  VAU.MACP.f32 A3, B3         || SAU.SUMX.f32 sAB2, vMUL  || PEU.PCIX.NEQ 0           || LSU0.SWZMV4.WORD [3333] [UUUU] || BRU.BRA sgemmtrsm_u4x4_loop
  VAU.MUL.f32 vMUL, A2, tB1   || SAU.SUMX.f32 sAB3, vMUL  || CMU.CPIVD vADD0.2, sAB2
  VAU.MUL.f32 vMUL, A2, tB2   || SAU.SUMX.f32 sAB0_, vMUL
  VAU.MUL.f32 vMUL, A2, tB3                               || CMU.TP4R tB0, B0
  VAU.ADD.f32 AB0, AB0, vADD0 || SAU.SUMX.f32 sAB1_, vMUL                             || LSU1.LDXVI B0, addrBD21, incr
  VAU.ADD.f32 AB2, AB2, vADD2 || SAU.SUMX.f32 sAB2_, vMUL
  VAU.ADD.f32 AB1, AB1, vADD1 || SAU.SUMX.f32 sAB3_, vMUL                             || LSU1.LDXVI B1, addrBD21, incr
// ~sgemmtrsm_u4x4_loop

                                 CMU.CPIVD vADD1.0, sAB0
  VAU.ACCPW.i32 AB3, AB3      || CMU.CPIVD vADD1.2, sAB2
                                 CMU.CPIVD vADD2.0, sAB0_
  VAU.ADD.f32 AB1, AB1, vADD1
                                 CMU.CPIVD vADD2.2, sAB2_
sgemmtrsm_u4x4_k_is_null:
  // Load C and scale by alpha and store the result
  IAU.ADDSU addrB11, addrB11, 48  || LSU0.LDIL incr_1, -16       || LSU1.LD.32 cs_c, i19
  LSU1.LDXVI B3, addrB11, incr_1  || IAU.ADDSU addrB11save, addrB11, 0
  LSU0.LDI.64.l A0, addrA11, incr || VAU.ADD.f32 AB2, AB2, vADD2
  LSU1.LDXVI B2, addrB11, incr_1
  LSU0.LDI.64.l A1, addrA11, incr
  LSU1.LDXVI B1, addrB11, incr_1  || IAU.SHL rs_c, rs_c, 2
  LSU0.LDXVI A2, addrA11, incr    || IAU.MUL rs_cx3, rs_c, 3
  LSU1.LDXV B0, addrB11
  LSU0.LDXV A3, addrA11           || IAU.ADDSU addrC11, addrC11, rs_cx3
  VAU.MUL.f32 B3, B3, alpha       || IAU.SHL cs_c, cs_c, 2
  SAU.MUL.i16 rs_c, rs_c, -1
  VAU.MUL.f32 B2, B2, alpha       || IAU.ADDSU addrC1, addrC11, cs_c
  VAU.SUB.f32 B3, B3, AB3         || IAU.ADDSU addrC2, addrC1, cs_c
  VAU.MUL.f32 B1, B1, alpha       || IAU.ADDSU addrC3, addrC2, cs_c
  VAU.SUB.f32 B2, B2, AB2         || LSU1.CP addrB11, addrB11save
  VAU.MUL.f32 B0, B0, alpha
  VAU.SUB.f32 B1, B1, AB1
  // Iteration 0
  //  b30 = a33*b30//
  //  b31 = a33*b31//
  //  b32 = a33*b32//
  //  b33 = a33*b33//
  VAU.MUL.f32 B3, A3, B3     || LSU0.SWZMV4.WORD [3333] [UUUU]
  VAU.SUB.f32 B0, B0, AB0
  //  => a22*a23
  VAU.MUL.f32 _A2, A2, A3
  //  B2.x = a22*b2x//
  VAU.MUL.f32 B2, A2, B2     || LSU0.SWZMV4.WORD [2222] [UUUU] || CMU.CPVCR.x32 _A1, A0.1
  //  => -- // a11*a11 // a11*a12 // a11*a13 //
  VAU.MUL.f32 _A1, A1, _A1   || LSU0.SWZMV4.WORD [1111] [UUUU] || LSU1.STXVI B3, addrB11, incr_1 || CMU.CPVID sC0, B3.0
  //  _B2.x = a22*a23*b3x//
  VAU.MUL.f32 _B2, _A2, B3   || LSU0.SWZMV4.WORD [2222] [UUUU]
  //  B1.x = a11*b1x
  VAU.MUL.f32 B1, A1, B1     || LSU0.SWZMV4.WORD [1111] [UUUU] || LSU1.STI.32 sC0, addrC11, rs_c || CMU.CPVID sC2, B3.2
  //  _B1.x = a11*a13*b3x//
  VAU.MUL.f32 _B1, _A1, B3   || LSU0.SWZMV4.WORD [0000] [UUUU] || LSU1.STI.32 sC1, addrC1, rs_c
  // Iteration 1
  //  b20 = a22*b20 - a22*a23*b30//
  //  b21 = a22*b21 - a22*a23*b31//
  //  b22 = a22*b22 - a22*a23*b32//
  //  b23 = a22*b23 - a22*a23*b33//
  VAU.SUB.f32 B2, B2, _B2    || CMU.CPVCR.x32 _A0, A0.0        || LSU1.STI.32 sC2, addrC2, rs_c
  //  => a00*a00 // a00*a01 // a00*a02 // a00*a03
  VAU.MUL.f32 _A0, A0, _A0   || LSU0.SWZMV4.WORD [0000] [UUUU] || LSU1.STI.32 sC3, addrC3, rs_c
  //  B1.x = a11*b1x - a11*a13*b3x//
  VAU.SUB.f32 B1, B1, _B1
  //  __B1.x = a11*a12*b3x//
  VAU.MUL.f32 __B1, _A1, B2  || LSU0.SWZMV4.WORD [1111] [UUUU] || LSU1.STXVI B2, addrB11, incr_1 || CMU.CPVID sC0, B2.0
  //  B0.x = a00*b0x//
  VAU.MUL.f32 B0, A0, B0     || LSU0.SWZMV4.WORD [0000] [UUUU]
  //  _B0.x = a00*a03*b3x//
  VAU.MUL.f32 _B0, _A0, B3   || LSU0.SWZMV4.WORD [0000] [UUUU] || LSU1.STI.32 sC0, addrC11, rs_c || CMU.CPVID sC2, B2.2
  // Iteration 2
  //  b10 = a11*b10 - a11*a13*b30 - a11*a12*b20//
  //  b11 = a11*b11 - a11*a13*b31 - a11*a12*b21//
  //  b12 = a11*b12 - a11*a13*b32 - a11*a12*b22//
  //  b13 = a11*b13 - a11*a13*b33 - a11*a12*b23//
  VAU.SUB.f32 B1, B1, __B1                                     || LSU1.STI.32 sC1, addrC1, rs_c
  //  __B0.x = a00*a02*b2x//
  VAU.MUL.f32 __B0, _A0, B2  || LSU0.SWZMV4.WORD [1111] [UUUU] || LSU1.STI.32 sC2, addrC2, rs_c
  //  B0.x = a00*b0x - a00*a01*b1x//
  VAU.SUB.f32 B0, B0, _B0                                      || LSU1.STI.32 sC3, addrC3, rs_c
  //  ___B0.x = a00*a01*b1x//
  VAU.MUL.f32 ___B0, _A0, B1 || LSU0.SWZMV4.WORD [2222] [UUUU] || LSU1.STXVI B1, addrB11, incr_1
  NOP
  //  B0.x = a00*b0x - a00*a03*b3x - a00*a02*b2x//
  VAU.SUB.f32 B0, B0, __B0                                     || CMU.CPVID sC0, B1.0
  LSU1.STI.32 sC0, addrC11, rs_c                               || CMU.CPVID sC2, B1.2
  LSU1.STI.32 sC1, addrC1, rs_c
  // Iteration 3
  //  b00 = a00*b00 - a00*a03*b30 - a00*a02*b20 - a00*a01*b10//
  //  b01 = a00*b01 - a00*a03*b31 - a00*a02*b21 - a00*a01*b11//
  //  b02 = a00*b02 - a00*a03*b32 - a00*a02*b22 - a00*a01*b12//
  //  b03 = a00*b03 - a00*a03*b33 - a00*a02*b23 - a00*a01*b13//
  VAU.SUB.f32 B0, B0, ___B0  || LSU1.STI.32 sC2, addrC2, rs_c
  LSU1.STI.32 sC3, addrC3, rs_c
  BRU.JMP i30
  LSU0.ST.64.l B0, addrB11   || LSU1.STO.64.h B0, addrB11, 8   || CMU.CPVID sC0, B0.0
  LSU0.ST.32 sC0, addrC11    || LSU1.ST.32 sC1, addrC1         || CMU.CPVID sC2, B0.2
  LSU0.ST.32 sC2, addrC2     || LSU1.ST.32 sC3, addrC3
  NOP 3

.nowarnend
.size mvcvsgemmtrsm_u4x4_asm,.-mvcvsgemmtrsm_u4x4_asm
.end
