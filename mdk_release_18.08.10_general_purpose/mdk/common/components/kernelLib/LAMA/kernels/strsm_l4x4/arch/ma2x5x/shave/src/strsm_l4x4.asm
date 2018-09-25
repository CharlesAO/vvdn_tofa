// Code for TRSM lower kernel
// Outline: C[4][4] = A[4][4]^-1 * B[4][4], with A lower triangular
// Number of instruction cycles: 36
// Number of stall cycles: 9
// Error from reference output: RMS = 1.47E-8, MAX = 1.19E-7
// Copyright 2014 Movidius
.type lamaStrsm_l4x4_asm,@function
.version 00.70.00

.code .text
.align 16
// NOTE: Not fully parallelized!!!
//
// void lamaStrsm_l4x4_asm(float *a, float *b, float *c, inc_t rs_c, inc_t cs_c)//
//                             (i18)     (i17)     (i16)       (i15)       (i14)
lamaStrsm_l4x4_asm:

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

  // ------ VRF usage ------
  .set A0         v0
  .set A0.2       v0.2
  .set A0.3       v0.3
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
  .set _B1        v8
  .set _B2        v9
  .set __B2       v10
  .set _B3        v11
  .set __B3       v12
  .set ___B3      v13
  .set _A1        v14
  .set _A2        v15
  .set _A3        v16

  LSU0.LD.64.l A0, addrA      || LSU1.LDO.64.h A0, addrA,  8 || IAU.SHL cs_c, cs_c, 2
  LSU0.LD.64.l B0, addrB      || LSU1.LDO.64.h B0, addrB,  8 || IAU.SHL rs_c, rs_c, 2
  LSU0.LDO.64.l A1, addrA, 16 || LSU1.LDO.64.h A1, addrA, 24
  LSU0.LDO.64.l B1, addrB, 16 || LSU1.LDO.64.h B1, addrB, 24
                                 LSU1.LDO.64.h A2, addrA, 40
  LSU0.LDO.64.l B2, addrB, 32 || LSU1.LDO.64.h B2, addrB, 40
  LSU0.LDO.64.l B3, addrB, 48 || LSU1.LDO.64.h B3, addrB, 56
  LSU1.LDO.64.h A3, addrA, 56
// 8 cycles
  NOP 6
  // Iteration 0
  //  b00 = a00 * b00//
  //  b01 = a00 * b01//
  //  b02 = a00 * b02//
  //  b03 = a00 * b03//
  VAU.MUL.f32 B0, A0, B0     || LSU0.SWZMV4.WORD [0000] [UUUU]
  //  => a11*a10
  VAU.MUL.f32 _A1, A1, A0                                      || CMU.VSZM.WORD A1, A0 [DDD3]
  //  B1.x = a11*b1x//
  VAU.MUL.f32 B1, A1, B1     || LSU0.SWZMV4.WORD [1111] [UUUU] || CMU.CPVCR.x32 _A2, A0.2
  //  => a22*a20 // a22*a21 // a22*a22 // - //
  VAU.MUL.f32 _A2, A2, _A2   || LSU0.SWZMV4.WORD [2222] [UUUU]
  //  _B1.x = a11*a10*b0x//
  VAU.MUL.f32 _B1, _A1, B0   || LSU0.SWZMV4.WORD [1111] [UUUU]
  //  B2.x = a22*b2x
  VAU.MUL.f32 B2, A2, B2     || LSU0.SWZMV4.WORD [2222] [UUUU]
  //  _B2.x = a22*a20*b0x//
  VAU.MUL.f32 _B2, _A2, B0   || LSU0.SWZMV4.WORD [3333] [UUUU]
  // Iteration 1
  //  b10 = a11*b10 - a11*a10*b00//
  //  b11 = a11*b11 - a11*a10*b01//
  //  b12 = a11*b12 - a11*a10*b02//
  //  b13 = a11*b13 - a11*a10*b03//
  VAU.SUB.f32 B1, B1, _B1    || CMU.CPVCR.x32 _A3, A0.3
  //  => a33*a30 // a33*a31 // a33*a32 // a33*a33
  VAU.MUL.f32 _A3, A3, _A3   || LSU0.SWZMV4.WORD [3333] [UUUU]
  //  B2.x = a22*b2x - a22*a20*b0x//
  VAU.SUB.f32 B2, B2, _B2
  //  __B2.x = a22*a21*b1x//
  VAU.MUL.f32 __B2, _A2, B1  || LSU0.SWZMV4.WORD [2222] [UUUU]
  //  B3.x = a33*b3x//
  VAU.MUL.f32 B3, A3, B3     || LSU0.SWZMV4.WORD [3333] [UUUU]
  //  _B3.x = a33*a30*b0x//
  VAU.MUL.f32 _B3, _A3, B0   || LSU0.SWZMV4.WORD [3333] [UUUU]
  // Iteration 2
  //  b20 = a22*b20 - a22*a20*b00 - a22*a21*b10//
  //  b21 = a22*b21 - a22*a20*b01 - a22*a21*b11//
  //  b22 = a22*b22 - a22*a20*b02 - a22*a21*b12//
  //  b23 = a22*b23 - a22*a20*b03 - a22*a21*b13//
  VAU.SUB.f32 B2, B2, __B2
  //  __B3.x = a33*a31*b1x//
  VAU.MUL.f32 __B3, _A3, B1  || LSU0.SWZMV4.WORD [2222] [UUUU]
  //  B3.x = a33*b3x - a33*a30*b0x//
  VAU.SUB.f32 B3, B3, _B3
  //  ___B3.x = a33*a32*b2x//
  VAU.MUL.f32 ___B3, _A3, B2 || LSU0.SWZMV4.WORD [1111] [UUUU]
  NOP
  //  B3.x = a33*b3x - a33*a30*b0x - a33*a31*b1x//
  VAU.SUB.f32 B3, B3, __B3
  NOP
  NOP
  // Iteration 3
  //  b30 = a33*b30 - a33*a30*b00 - a33*a31*b10 - a33*a32*b20//
  //  b31 = a33*b31 - a33*a30*b01 - a33*a31*b11 - a33*a32*b21//
  //  b32 = a33*b32 - a33*a30*b02 - a33*a31*b12 - a33*a32*b22//
  //  b33 = a33*b33 - a33*a30*b03 - a33*a31*b13 - a33*a32*b23//
  VAU.SUB.f32 B3, B3, ___B3

  NOP
  NOP
  LSU1.LDIL incr, 16
  LSU1.STXVI B0, addrB, incr
  NOP
  LSU1.STXVI B1, addrB, incr
  NOP
  LSU1.STXVI B2, addrB, incr
  NOP
  LSU1.STXV  B3, addrB
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
  .size lamaStrsm_l4x4_asm,.-lamaStrsm_l4x4_asm
  .end
