// vector matrix multiplication kernel
// Calculates C = C + (A*B') [A*B if B is stored as a transpose matrix]
// Where A is 1*K, B is K*N and C is 1*N//
// A - input vector of size 1*K
// B - input matrix of size K*N
// C - input/output vector of size 1*N
// K - width of A vector
// N - number of columns of matrix B

.type mvcvMatVecMul_asm,@function
.version 00.50.02

.code .text.mvcvMatVecMul_asm
.align 16
//
//void mvcvMatVecMul_asm(float *a, float *b, float *c, int k, int n)
//                           (i18)     (i17)     (i16)  (i15)  (i14)
mvcvMatVecMul_asm:
.nowarn 10
    lsu0.ldil i13 0x10      || lsu1.ldil i11 ___innerLoopK || cmu.cpii i8 i18
    lsu1.ldxvi v0  i18 i13  || lsu0.ldih i11 ___innerLoopK
    lsu0.ldxvi v10 i17 i13  || iau.shr.u32 i12 i15 4
    lsu1.ldxvi v1  i18 i13  || iau.xor i10 i10 i10
    lsu0.ldxvi v11 i17 i13
    lsu1.ldxvi v2  i18 i13  || iau.add i4 i14 i10
    lsu0.ldxvi v12 i17 i13  || iau.add i6 i16 i10
    lsu1.ldxvi v3  i18 i13  || iau.sub i0 i12 1
    lsu0.ldxvi v13 i17 i13
    lsu1.ldi.32 i7 i16
    nop 10
    peu.pc1i gt || bru.bra ___loopKGreatherThen16 || iau.incs i12 -1
    nop 5
    peu.pc1i gt || lsu0.ldil i11 ___innerLoopKGreatherThen16 || lsu1.ldih i11 ___innerLoopKGreatherThen16


    ___loopN:
    vau.mul.f32 v9 v0 v10 || lsu1.ldxvi v0  i18 i13 || bru.rpl i11 i12
    vau.mul.f32 v8 v1 v11 || lsu0.ldxvi v10 i17 i13
    vau.mul.f32 v7 v2 v12 || lsu1.ldxvi v1  i18 i13
    vau.mul.f32 v6 v3 v13 || lsu0.ldxvi v11 i17 i13
    vau.add.f32 v5 v8 v9  || lsu1.ldxvi v2  i18 i13
    lsu0.ldxvi v12 i17 i13
    vau.add.f32 v4 v6 v7  || lsu1.ldxvi v3  i18 i13
    lsu0.ldxvi v13 i17 i13
    nop
    vau.add.f32 v14 v4 v5
    nop
    ___innerLoopK:
    nop
    sau.sumx.f32 i9 v14
    nop 4
    sau.add.f32 i10 i10 i9

    iau.sub i4 i4 1       || cmu.cpii i18 i8
    cmu.cmz.i32 i4        || lsu0.ldxv v0 i18  || iau.add i18 i18 i13
    sau.add.f32 i7 i7 i10 || lsu1.ldxv v1 i18  || iau.add i18 i18 i13
    peu.pc1c gt           || bru.bra ___loopN
    lsu1.ldxv v2 i18      || iau.add i18 i18 i13
    lsu0.ldxv v3 i18      || iau.add i18 i18 i13
    lsu1.sti.32 i7 i6
    lsu1.ldi.32 i7 i16
    iau.xor i10 i10 i10
    nop

    bru.jmp i30
    nop 6


    //// Case where K > 16
    ___loopKGreatherThen16:
    vau.mul.f32 v9 v0 v10 || lsu1.ldxvi v0  i18 i13
    vau.mul.f32 v8 v1 v11 || lsu0.ldxvi v10 i17 i13
    vau.mul.f32 v7 v2 v12 || lsu1.ldxvi v1  i18 i13
    vau.mul.f32 v6 v3 v13 || lsu0.ldxvi v11 i17 i13
    vau.add.f32 v5 v8 v9  || lsu1.ldxvi v2  i18 i13
    lsu0.ldxvi v12 i17 i13
    vau.add.f32 v4 v6 v7  || lsu1.ldxvi v3  i18 i13
    lsu0.ldxvi v13 i17 i13
    nop
    vau.add.f32 v14 v4 v5
    nop 2

    vau.mul.f32 v9 v0 v10  || lsu1.ldxvi v0  i18 i13 || bru.rpl i11 i12
    vau.mul.f32 v8 v1 v11  || lsu0.ldxvi v10 i17 i13
    vau.mul.f32 v7 v2 v12  || lsu1.ldxvi v1  i18 i13
    vau.mul.f32 v6 v3 v13  || lsu0.ldxvi v11 i17 i13
    vau.add.f32 v5 v8 v9   || lsu1.ldxvi v2  i18 i13
    ___innerLoopKGreatherThen16:
    lsu0.ldxvi v12 i17 i13 || sau.sumx.f32 i9 v14
    vau.add.f32 v4 v6 v7   || lsu1.ldxvi v3  i18 i13
    lsu0.ldxvi v13 i17 i13
    nop
    vau.add.f32 v14 v4 v5
    sau.add.f32 i10 i10 i9
    nop


    sau.sumx.f32 i9 v14
    nop 4
    sau.add.f32 i10 i10 i9
    iau.sub i4 i4 1       || cmu.cpii i18 i8
    cmu.cmz.i32 i4        || lsu0.ldxv v0 i18  || iau.add i18 i18 i13
    sau.add.f32 i7 i7 i10 || lsu1.ldxv v1 i18  || iau.add i18 i18 i13
    peu.pc1c gt           || bru.bra ___loopKGreatherThen16
    lsu1.ldxv v2 i18      || iau.add i18 i18 i13
    lsu0.ldxv v3 i18      || iau.add i18 i18 i13
    lsu1.sti.32 i7 i6
    lsu1.ldi.32 i7 i16
    iau.xor i10 i10 i10
    nop

    bru.jmp i30
    nop 6
.nowarnend
.size mvcvMatVecMul_asm,.-mvcvMatVecMul_asm
.end
