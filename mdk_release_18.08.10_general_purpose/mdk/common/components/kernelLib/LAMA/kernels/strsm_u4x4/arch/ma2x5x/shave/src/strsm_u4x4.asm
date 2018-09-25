// Code for TRSM upper kernel
// Outline: C[4][4] = A[4][4]^-1 * B[4][4], with A upper triangular
// Number of instruction cycles: 36
// Number of stall cycles: 9
// Error from reference output: RMS = 2.02E-8, MAX = 1.19E-7
// Copyright 2014 Movidius

.type lamaStrsm_u4x4_asm,@function
.version 00.70.00

.code .text
.align 16
// NOTE: Not fully parallelized!!!
//
// void lamaStrsm_u4x4_asm(float* a, float* b, float* c, int rs_c, int cs_c)//
//                             (i18)     (i17)     (i16)     (i15)      (i14)
lamaStrsm_u4x4_asm:
  // ------ IRF usage ------
  .set addrA      i18
  .set addrB      i17
  .set addrC      i16
  .set rs_c       i15
  .set cs_c       i14
  .set incr       i0
  .set addrC1     i1
  .set addrC2     i2
  .set addrC3     i3

  .set sC0        i4
  .set sC1        i5
  .set sC2        i6
  .set sC3        i7
  .set rs_cx3     i8
  .set minus_one  i9

  // ------ VRF usage ------
  .set A0         v0
  .set A0.0       v0.0
  .set A0.1       v0.1
  .set A1         v1
  .set A2         v2
  .set A3         v3
  .set B0         v4
  .set B0.0       v4.0
  .set B0.1       v4.1
  .set B0.2       v4.2
  .set B0.3       v4.3
  .set B1         v5
  .set B1.0       v5.0
  .set B1.1       v5.1
  .set B1.2       v5.2
  .set B1.3       v5.3
  .set B2         v6
  .set B2.0       v6.0
  .set B2.1       v6.1
  .set B2.2       v6.2
  .set B2.3       v6.3
  .set B3         v7
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

  LSU0.LDO.64.l A3, addrA, 48 || LSU1.LDO.64.h A3, addrA, 56 || IAU.SHL cs_c, cs_c, 2
  LSU0.LDO.64.l B3, addrB, 48 || LSU1.LDO.64.h B3, addrB, 56 || IAU.SHL rs_c, rs_c, 2
  LSU0.LDO.64.l A2, addrA, 32 || LSU1.LDO.64.h A2, addrA, 40 || IAU.MULSU rs_cx3, rs_c, 3
  LSU0.LDO.64.l B2, addrB, 32 || LSU1.LDO.64.h B2, addrB, 40
                                 LSU1.LDO.64.l A1, addrA, 16 //|| IAU.ADDSU addrC, addrC, rs_cx3
  LSU0.LDO.64.l B1, addrB, 16 || LSU1.LDO.64.h B1, addrB, 24 //|| SAU.MUL.i16 cs_c, cs_c, -1
  LSU0.LD.64.l B0, addrB      || LSU1.LDO.64.h B0, addrB,  8
                                 LSU1.LD.64.l A0, addrA
// 8 cycles
NOP 6
  // Iteration 0
  //  b30 = a33*b30//
  //  b31 = a33*b31//
  //  b32 = a33*b32//
  //  b33 = a33*b33//
  VAU.MUL.f32 B3, A3, B3     || LSU0.SWZMV4.WORD [3333] [UUUU]
  //  => a22*a23
  VAU.MUL.f32 _A2, A2, A3
  //  B2.x = a22*b2x//
  VAU.MUL.f32 B2, A2, B2     || LSU0.SWZMV4.WORD [2222] [UUUU] || CMU.CPVCR.x32 _A1, A0.1
  //  => -- // a11*a11 // a11*a12 // a11*a13 //
  VAU.MUL.f32 _A1, A1, _A1   || LSU0.SWZMV4.WORD [1111] [UUUU]
  //  _B2.x = a22*a23*b3x//
  VAU.MUL.f32 _B2, _A2, B3   || LSU0.SWZMV4.WORD [2222] [UUUU]
  //  B1.x = a11*b1x
  VAU.MUL.f32 B1, A1, B1     || LSU0.SWZMV4.WORD [1111] [UUUU]
  //  _B1.x = a11*a13*b3x//
  VAU.MUL.f32 _B1, _A1, B3   || LSU0.SWZMV4.WORD [0000] [UUUU]
  // Iteration 1
  //  b20 = a22*b20 - a22*a23*b30//
  //  b21 = a22*b21 - a22*a23*b31//
  //  b22 = a22*b22 - a22*a23*b32//
  //  b23 = a22*b23 - a22*a23*b33//
  VAU.SUB.f32 B2, B2, _B2                                      || CMU.CPVCR.x32 _A0, A0.0
  //  => a00*a00 // a00*a01 // a00*a02 // a00*a03
  VAU.MUL.f32 _A0, A0, _A0   || LSU0.SWZMV4.WORD [0000] [UUUU]
  //  B1.x = a11*b1x - a11*a13*b3x//
  VAU.SUB.f32 B1, B1, _B1
  //  __B1.x = a11*a12*b3x//
  VAU.MUL.f32 __B1, _A1, B2  || LSU0.SWZMV4.WORD [1111] [UUUU]
  //  B0.x = a00*b0x//
  VAU.MUL.f32 B0, A0, B0     || LSU0.SWZMV4.WORD [0000] [UUUU]
  //  _B0.x = a00*a03*b3x//
  VAU.MUL.f32 _B0, _A0, B3   || LSU0.SWZMV4.WORD [0000] [UUUU]
  // Iteration 2
  //  b10 = a11*b10 - a11*a13*b30 - a11*a12*b20//
  //  b11 = a11*b11 - a11*a13*b31 - a11*a12*b21//
  //  b12 = a11*b12 - a11*a13*b32 - a11*a12*b22//
  //  b13 = a11*b13 - a11*a13*b33 - a11*a12*b23//
  VAU.SUB.f32 B1, B1, __B1
  //  __B0.x = a00*a02*b2x//
  VAU.MUL.f32 __B0, _A0, B2  || LSU0.SWZMV4.WORD [1111] [UUUU]
  //  B0.x = a00*b0x - a00*a01*b1x//
  VAU.SUB.f32 B0, B0, _B0
  //  ___B0.x = a00*a01*b1x//
  VAU.MUL.f32 ___B0, _A0, B1 || LSU0.SWZMV4.WORD [2222] [UUUU]
  NOP
  //  B0.x = a00*b0x - a00*a03*b3x - a00*a02*b2x//
  VAU.SUB.f32 B0, B0, __B0
  NOP
  NOP
  // Iteration 3
  //  b00 = a00*b00 - a00*a03*b30 - a00*a02*b20 - a00*a01*b10//
  //  b01 = a00*b01 - a00*a03*b31 - a00*a02*b21 - a00*a01*b11//
  //  b02 = a00*b02 - a00*a03*b32 - a00*a02*b22 - a00*a01*b12//
  //  b03 = a00*b03 - a00*a03*b33 - a00*a02*b23 - a00*a01*b13//
  VAU.SUB.f32 B0, B0, ___B0
  NOP
  NOP

  IAU.ADDSU addrB, addrB, 48
  LSU1.LDIL incr, -16
  LSU1.STXVI B3, addrB, incr
  NOP
  LSU1.STXVI B2, addrB, incr
  NOP
  LSU1.STXVI B1, addrB, incr
  NOP
  LSU1.STXV  B0, addrB
  IAU.ADDSU addrC1, addrC, cs_c
  IAU.ADDSU addrC2, addrC1, cs_c
  IAU.ADDSU addrC3, addrC2, cs_c
  CMU.CPVID sC0, B0.0
  CMU.CPVID sC2, B0.2
  LSU0.STI.32 sC0, addrC, rs_c  || LSU1.STI.32 sC1, addrC1, rs_c
  LSU0.STI.32 sC2, addrC2, rs_c || LSU1.STI.32 sC3, addrC3, rs_c
  NOP
  CMU.CPVID sC0, B1.0
  CMU.CPVID sC2, B1.2
  LSU0.STI.32 sC0, addrC, rs_c  || LSU1.STI.32 sC1, addrC1, rs_c
  LSU0.STI.32 sC2, addrC2, rs_c || LSU1.STI.32 sC3, addrC3, rs_c
  NOP
  CMU.CPVID sC0, B2.0
  CMU.CPVID sC2, B2.2
  LSU0.STI.32 sC0, addrC, rs_c  || LSU1.STI.32 sC1, addrC1, rs_c
  LSU0.STI.32 sC2, addrC2, rs_c || LSU1.STI.32 sC3, addrC3, rs_c
  NOP
  CMU.CPVID sC0, B3.0
  CMU.CPVID sC2, B3.2
  LSU0.STI.32 sC0, addrC, rs_c  || LSU1.STI.32 sC1, addrC1, rs_c
  LSU0.STI.32 sC2, addrC2, rs_c || LSU1.STI.32 sC3, addrC3, rs_c


  BRU.JMP i30
  NOP 6
  .size lamaStrsm_u4x4_asm,.-lamaStrsm_u4x4_asm
  .end
