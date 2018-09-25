  .version 00.50.01

// In-place symmetric left rank update - cell compute from two columns

  .code .text.syrktnNx4
  .nowarn

// void syrktnNx4Nx4( float* C, const float *A, const float *B,
//                    int n, int wC, int wAA )//

  .lalign
  .type syrktnNx4, @function
syrktnNx4:
  // input arguments
  .set   C i18
  .set   A i17
  .set   B i16
  .set   n i15
  .set  wC i14
  .set wAB i13
  // locals
  .set  A0 v0
  .set  A1 v1
  .set  A2 v2
  .set  A3 v3
  .set  C0 v4
  .set  C1 v5
  .set  C2 v6
  .set  C3 v7
  .set  B0 v8
  .set  B1 v9
  .set  B2 v10
  .set  B3 v11
.ifdef ma2480
  lsu0.ldi.128     A0, A wAB  || lsu1.ldi.128  B0, B wAB
                               nop
                               lsu1.ldi.128  B1, B wAB
                               nop
  iau.incs        n, -4     || lsu1.ldi.128  A1, A wAB
  bru.bra    .Lfinal        || PEU.PC1I  0
    nop 2
    vau.mul.f32  C0, B0 A0                           || lsu0.swzmv4.word [0000] [UUUU]
    vau.macpz.f32    B0 A0 V_ACC0  || lsu1.ldi.128  B2, B wAB || lsu0.swzmv4.word [1111] [UUUU]
    vau.mul.f32  C2, B0 A0                           || lsu0.swzmv4.word [2222] [UUUU]
    vau.mul.f32  C3, B0 A0  || lsu1.ldi.128  A2, A wAB || lsu0.swzmv4.word [3333] [UUUU]
.Lmain:
  vau.mul.f32   v12, B1 A1                           || lsu0.swzmv4.word [0000] [UUUU]
  vau.macp.f32       B1 A1  V_ACC0                       || lsu0.swzmv4.word [1111] [UUUU]
  vau.mul.f32   v13, B1 A1                           || lsu0.swzmv4.word [2222] [UUUU]
  vau.mul.f32   v14, B1 A1                           || lsu0.swzmv4.word [3333] [UUUU]
  vau.add.f32    C0, C0 v12 || lsu1.ldi.128  B3, B wAB
  vau.add.f32    C2, C2 v13
  vau.add.f32    C3, C3 v14 || lsu1.ldi.128  A3, A wAB
  vau.mul.f32   v12, B2 A2                           || lsu0.swzmv4.word [0000] [UUUU]
  vau.macp.f32       B2 A2  V_ACC0                         || lsu0.swzmv4.word [1111] [UUUU]
  vau.mul.f32   v13, B2 A2                           || lsu0.swzmv4.word [2222] [UUUU]
  vau.mul.f32   v14, B2 A2  || lsu1.ldi.128  B0, B wAB || lsu0.swzmv4.word [3333] [UUUU]
  vau.add.f32    C0, C0 v12
  vau.add.f32    C2, C2 v13 || lsu1.ldi.128  A0, A wAB
  vau.add.f32    C3, C3 v14
  vau.mul.f32   v12, B3 A3                           || lsu0.swzmv4.word [0000] [UUUU]
  vau.macp.f32       B3 A3   V_ACC0                        || lsu0.swzmv4.word [1111] [UUUU]
  vau.mul.f32   v13, B3 A3                           || lsu0.swzmv4.word [2222] [UUUU]
  vau.mul.f32   v14, B3 A3  || lsu1.ldi.128  B1, B wAB || lsu0.swzmv4.word [3333] [UUUU]
  vau.add.f32    C0, C0 v12 || iau.incs     n, -4
  vau.add.f32    C2, C2 v13 || lsu1.ldi.128  A1, A wAB
  vau.mul.f32   v12, B0 A0                           || lsu0.swzmv4.word [0000] [UUUU]
  vau.add.f32    C3, C3 v14 || bru.bra .Lmain        || peu.pcix neq     0
    vau.macp.f32     B0 A0 V_ACC0                          || lsu0.swzmv4.word [1111] [UUUU]
    vau.mul.f32 v13, B0 A0                           || lsu0.swzmv4.word [2222] [UUUU]
    vau.mul.f32 v14, B0 A0                           || lsu0.swzmv4.word [3333] [UUUU]
    vau.add.f32  C0, C0 v12 || lsu1.ldi.128  B2, B wAB
    vau.add.f32  C2, C2 v13
    vau.add.f32  C3, C3 v14 || lsu1.ldi.128  A2, A wAB
.Lfinal:
  vau.mul.f32   v12, B1 A1                           || lsu0.swzmv4.word [0000] [UUUU]
  vau.macp.f32       B1 A1 V_ACC0                          || lsu0.swzmv4.word [1111] [UUUU]
  vau.mul.f32   v13, B1 A1                           || lsu0.swzmv4.word [2222] [UUUU]
  vau.mul.f32   v14, B1 A1                           || lsu0.swzmv4.word [3333] [UUUU]
  vau.add.f32    C0, C0 v12 || lsu1.ldi.128  B3, B wAB
  vau.add.f32    C2, C2 v13
  vau.add.f32    C3, C3 v14 || lsu1.ldi.128  A3, A wAB
  vau.mul.f32   v12, B2 A2                           || lsu0.swzmv4.word [0000] [UUUU]
  vau.macp.f32       B2 A2 V_ACC0                          || lsu0.swzmv4.word [1111] [UUUU]
  vau.mul.f32   v13, B2 A2                           || lsu0.swzmv4.word [2222] [UUUU]
  vau.mul.f32   v14, B2 A2                           || lsu0.swzmv4.word [3333] [UUUU]
  vau.add.f32    C0, C0 v12
  vau.add.f32    C2, C2 v13
  vau.add.f32    C3, C3 v14 || iau.add     i0,  C wC
  vau.mul.f32   v12, B3 A3                           || lsu0.swzmv4.word [0000] [UUUU]
  vau.macpw.f32  C1, B3 A3 V_ACC0                          || lsu0.swzmv4.word [1111] [UUUU]
  vau.mul.f32   v13, B3 A3                           || lsu0.swzmv4.word [2222] [UUUU]
  nop                       // macpw writeback gets triggered here
  vau.mul.f32   v14, B3 A3                           || lsu0.swzmv4.word [3333] [UUUU]
  vau.add.f32    C0, C0 v12 || iau.shl     wC, wC 1
  vau.add.f32    C2, C2 v13 || bru.jmp    i30
    vau.add.f32  C3, C3 v14
    lsu1.sti.128   C0,  C wC
    lsu0.sti.128   C1, i0 wC
    lsu1.st.128    C2  C
    lsu0.st.128    C3 i0
 .else 
  lsu0.ldxvi     A0, A wAB  || lsu1.ldxvi  B0, B wAB
                               nop
                               lsu1.ldxvi  B1, B wAB
                               nop
  iau.incs        n, -4     || lsu1.ldxvi  A1, A wAB
  bru.bra    .Lfinal        || peu.pcix.eq  0
    nop 2
    vau.mul.f32  C0, B0 A0                           || lsu0.swzmv4.word [0000] [UUUU]
    vau.macpz.f32    B0 A0  || lsu1.ldxvi  B2, B wAB || lsu0.swzmv4.word [1111] [UUUU]
    vau.mul.f32  C2, B0 A0                           || lsu0.swzmv4.word [2222] [UUUU]
    vau.mul.f32  C3, B0 A0  || lsu1.ldxvi  A2, A wAB || lsu0.swzmv4.word [3333] [UUUU]
.Lmain:
  vau.mul.f32   v12, B1 A1                           || lsu0.swzmv4.word [0000] [UUUU]
  vau.macp.f32       B1 A1                           || lsu0.swzmv4.word [1111] [UUUU]
  vau.mul.f32   v13, B1 A1                           || lsu0.swzmv4.word [2222] [UUUU]
  vau.mul.f32   v14, B1 A1                           || lsu0.swzmv4.word [3333] [UUUU]
  vau.add.f32    C0, C0 v12 || lsu1.ldxvi  B3, B wAB
  vau.add.f32    C2, C2 v13
  vau.add.f32    C3, C3 v14 || lsu1.ldxvi  A3, A wAB
  vau.mul.f32   v12, B2 A2                           || lsu0.swzmv4.word [0000] [UUUU]
  vau.macp.f32       B2 A2                           || lsu0.swzmv4.word [1111] [UUUU]
  vau.mul.f32   v13, B2 A2                           || lsu0.swzmv4.word [2222] [UUUU]
  vau.mul.f32   v14, B2 A2  || lsu1.ldxvi  B0, B wAB || lsu0.swzmv4.word [3333] [UUUU]
  vau.add.f32    C0, C0 v12
  vau.add.f32    C2, C2 v13 || lsu1.ldxvi  A0, A wAB
  vau.add.f32    C3, C3 v14
  vau.mul.f32   v12, B3 A3                           || lsu0.swzmv4.word [0000] [UUUU]
  vau.macp.f32       B3 A3                           || lsu0.swzmv4.word [1111] [UUUU]
  vau.mul.f32   v13, B3 A3                           || lsu0.swzmv4.word [2222] [UUUU]
  vau.mul.f32   v14, B3 A3  || lsu1.ldxvi  B1, B wAB || lsu0.swzmv4.word [3333] [UUUU]
  vau.add.f32    C0, C0 v12 || iau.incs     n, -4
  vau.add.f32    C2, C2 v13 || lsu1.ldxvi  A1, A wAB
  vau.mul.f32   v12, B0 A0                           || lsu0.swzmv4.word [0000] [UUUU]
  vau.add.f32    C3, C3 v14 || bru.bra .Lmain        || peu.pcix.neq     0
    vau.macp.f32     B0 A0                           || lsu0.swzmv4.word [1111] [UUUU]
    vau.mul.f32 v13, B0 A0                           || lsu0.swzmv4.word [2222] [UUUU]
    vau.mul.f32 v14, B0 A0                           || lsu0.swzmv4.word [3333] [UUUU]
    vau.add.f32  C0, C0 v12 || lsu1.ldxvi  B2, B wAB
    vau.add.f32  C2, C2 v13
    vau.add.f32  C3, C3 v14 || lsu1.ldxvi  A2, A wAB
.Lfinal:
  vau.mul.f32   v12, B1 A1                           || lsu0.swzmv4.word [0000] [UUUU]
  vau.macp.f32       B1 A1                           || lsu0.swzmv4.word [1111] [UUUU]
  vau.mul.f32   v13, B1 A1                           || lsu0.swzmv4.word [2222] [UUUU]
  vau.mul.f32   v14, B1 A1                           || lsu0.swzmv4.word [3333] [UUUU]
  vau.add.f32    C0, C0 v12 || lsu1.ldxvi  B3, B wAB
  vau.add.f32    C2, C2 v13
  vau.add.f32    C3, C3 v14 || lsu1.ldxvi  A3, A wAB
  vau.mul.f32   v12, B2 A2                           || lsu0.swzmv4.word [0000] [UUUU]
  vau.macp.f32       B2 A2                           || lsu0.swzmv4.word [1111] [UUUU]
  vau.mul.f32   v13, B2 A2                           || lsu0.swzmv4.word [2222] [UUUU]
  vau.mul.f32   v14, B2 A2                           || lsu0.swzmv4.word [3333] [UUUU]
  vau.add.f32    C0, C0 v12
  vau.add.f32    C2, C2 v13
  vau.add.f32    C3, C3 v14 || iau.add     i0,  C wC
  vau.mul.f32   v12, B3 A3                           || lsu0.swzmv4.word [0000] [UUUU]
  vau.macpw.f32  C1, B3 A3                           || lsu0.swzmv4.word [1111] [UUUU]
  vau.mul.f32   v13, B3 A3                           || lsu0.swzmv4.word [2222] [UUUU]
  nop                       // macpw writeback gets triggered here
  vau.mul.f32   v14, B3 A3                           || lsu0.swzmv4.word [3333] [UUUU]
  vau.add.f32    C0, C0 v12 || iau.shl     wC, wC 1
  vau.add.f32    C2, C2 v13 || bru.jmp    i30
    vau.add.f32  C3, C3 v14
    lsu1.stxvi   C0,  C wC
    lsu0.stxvi   C1, i0 wC
    lsu1.stxv    C2,  C
    lsu0.stxv    C3, i0
    nop
.endif 
    
  .size syrktnNx4, . - syrktnNx4
  .nowarnend
  .end
