  .version 00.50.01

// In-place symmetric rank update - reverse row copy operator

  .code .text.rcpy4xN
  .nowarn

// void rcpy4xN( float *B, const float *A, int n, int wB, int wA )//

  .lalign
  .type rcpy4xN, @function
rcpy4xN:
  // input arguments
  .set  B i18
  .set  A i17
  .set  n i16
  .set wB i15
  .set wA i14
  // locals
  .set  B1 i13
  .set  A1 i12
  .set wB2 i11
  .set wA2 i10
  iau.shl        wA, wA 1   || sau.add.i32  A1,  A wA
// initial load step
.ifdef ma2480
  lsu1.ldi.128    v0,  A wA  || iau.incs      n, -4
  lsu0.ldi.128     v1, A1 wA  || bru.bra  .Lfinal        || peu.pcix eq   0
    lsu1.ld.128    v2,  A     || iau.add      B1,  B wB  || sau.add.i32 wA2, wA 16
    lsu0.ld.128    v3, A1     || iau.shl      wB, wB 1
.else
  lsu1.ldxvi     v0,  A wA  || iau.incs      n, -4
  lsu0.ldxvi     v1, A1 wA  || bru.bra  .Lfinal        || peu.pcix.eq   0
    lsu1.ldxv    v2,  A     || iau.add      B1,  B wB  || sau.add.i32 wA2, wA 16
    lsu0.ldxv    v3, A1     || iau.shl      wB, wB 1
.endif
    iau.sub       A,  A wA2 || sau.add.i32 wB2, wB 16
    iau.sub      A1, A1 wA2
    nop 3
// store-load loop
.Lmain:
.ifdef ma2480
  lsu1.ldi.128     v0,  A wA  || iau.incs      n, -4
  lsu0.sti.128     v0,  B wB  || bru.bra  .Lmain         || peu.pcix neq  0
    lsu1.ldi.128   v1, A1 wA
    lsu0.sti.128   v1, B1 wB
    lsu1.ld.128    v2,  A     || iau.sub       A,  A wA2
    lsu0.st.128    v2,  B     || iau.sub       B,  B wB2
    lsu1.ld.128    v3, A1     || iau.sub      A1, A1 wA2
    lsu0.st.128    v3, B1     || iau.sub      B1, B1 wB2
  nop 3 //for the last iteration - make sure v3 arrives before we write it
// last store step
.Lfinal:
  lsu1.sti.128     v0,  B wB  || bru.jmp     i30
    lsu0.sti.128   v1, B1 wB
    lsu1.st.128    v2,  B
    lsu0.st.128    v3, B1
.else
  lsu1.ldxvi     v0,  A wA  || iau.incs      n, -4
  lsu0.stxvi     v0,  B wB  || bru.bra  .Lmain         || peu.pcix.neq  0
    lsu1.ldxvi   v1, A1 wA
    lsu0.stxvi   v1, B1 wB
    lsu1.ldxv    v2,  A     || iau.sub       A,  A wA2
    lsu0.stxv    v2,  B     || iau.sub       B,  B wB2
    lsu1.ldxv    v3, A1     || iau.sub      A1, A1 wA2
    lsu0.stxv    v3, B1     || iau.sub      B1, B1 wB2
  nop 3 //for the last iteration - make sure v3 arrives before we write it
// last store step
.Lfinal:
  lsu1.stxvi     v0,  B wB  || bru.jmp     i30
    lsu0.stxvi   v1, B1 wB
    lsu1.stxv    v2,  B
    lsu0.stxv    v3, B1
.endif 
    nop 3
  .size rcpy4xN, . - rcpy4xN
  .nowarnend
  .end
