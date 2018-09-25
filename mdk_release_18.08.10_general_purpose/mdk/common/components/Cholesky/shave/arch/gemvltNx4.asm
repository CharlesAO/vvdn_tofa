  .version 00.50.01

// Transposed matrix by vector multiplication - chunk compute from a column and the vector

  .code .text.gemvltNx4
  .nowarn

// void my_gemvNx4( float *c, const float *A, const float *b, int n, int w )//

.lalign
  .type gemvltNx4, @function
gemvltNx4:
  // input arguments
  .set   c i18
  .set   A i17
  .set   b i16
  .set   n i15
  .set  wA i14
.ifdef ma2480
  lsu1.ldi.128      v0,  A wA  || lsu0.ldi.64   i0, b
  nop
  lsu1.ldi.128      v1,  A wA  || iau.incs       n, -4
  nop
  lsu1.ldi.128      v2,  A wA  || lsu0.ldi.64   i2, b
  nop
  lsu1.ldi.128      v3,  A wA  || bru.bra  .Lfinal       || peu.pcix eq 0
    cmu.cp.128.32.r v4, i0
    vau.macpz.f32     v0 v4 V_ACC0
    nop
    cmu.cp.128.32.r v5, i1     || lsu1.ldi.64   i0,  b
    vau.macp.f32      v1 v5  V_ACC0 || lsu1.ldi.128    v0,  A wA
    nop
.Lmain:
  cmu.cp.128.32.r   v6, i2     || iau.incs       n, -4
  vau.macp.f32        v2 v6  V_ACC0 || lsu1.ldi.128    v1,  A wA
  nop
  cmu.cp.128.32.r   v7, i3    || lsu1.ldi.64   i2, b
  vau.macp.f32        v3 v7  V_ACC0 || lsu1.ldi.128    v2,  A wA
  bru.bra         .Lmain     || peu.pc1i     neq
    cmu.cp.128.32.r v4, i0
    vau.macp.f32      v0 v4  V_ACC0 || lsu1.ldi.128    v3,  A wA
    nop
    cmu.cp.128.32.r v5, i1     || lsu1.ldi.64   i0,  b
    vau.macp.f32      v1 v5 V_ACC0 || lsu1.ldi.128    v0,  A wA
    nop
.Lfinal:
  cmu.cp.128.32.r   v6, i2
  vau.macp.f32        v2 v6 V_ACC0 || cmu.cp.128.32.r v7, i3
  nop 2
    vau.macpw.f32 v0, v3 v7 V_ACC0 || bru.jmp i30 
    nop 4 
    lsu1.st.128 v0, c 
    nop
 .else
 lsu1.ldxvi      v0,  A wA  || lsu0.ldi.64   i0, b
  nop
  lsu1.ldxvi      v1,  A wA  || iau.incs       n, -4
  nop
  lsu1.ldxvi      v2,  A wA  || lsu0.ldi.64   i2, b
  nop
  lsu1.ldxvi      v3,  A wA  || bru.bra  .Lfinal        || peu.pcix.eq 0
    cmu.cpivr.x32 v4, i0
    vau.macpz.f32     v0 v4
    nop
    cmu.cpivr.x32 v5, i1     || lsu1.ldi.64   i0,  b
    vau.macp.f32      v1 v5  || lsu1.ldxvi    v0,  A wA
    nop
.Lmain:
  cmu.cpivr.x32   v6, i2     || iau.incs       n, -4
  vau.macp.f32        v2 v6  || lsu1.ldxvi    v1,  A wA
  nop
  cmu.cpivr.x32   v7, i3     || lsu1.ldi.64   i2, b
  vau.macp.f32        v3 v7  || lsu1.ldxvi    v2,  A wA
  bru.bra         .Lmain     || peu.pc1i     neq
    cmu.cpivr.x32 v4, i0
    vau.macp.f32      v0 v4  || lsu1.ldxvi    v3,  A wA
    nop
    cmu.cpivr.x32 v5, i1     || lsu1.ldi.64   i0,  b
    vau.macp.f32      v1 v5  || lsu1.ldxvi    v0,  A wA
    nop
.Lfinal:
  cmu.cpivr.x32   v6, i2
  vau.macp.f32        v2 v6  || cmu.cpivr.x32 v7, i3
  nop 2
  vau.macpw.f32   v0, v3 v7  || bru.jmp      i30
    nop 4
    lsu1.stxv     v0, c
    nop
.endif
.nowarnend
