.type mvcvMatrixInv4x4_fp32_asm,@function
.version 00.50.00

.data .data.matrixInv4x4_fp32_asm:
.align 16
    _signMask:
      .word 0x80000000

.code .text.matrixInv4x4_fp32_asm

//bool mvcvMatrixInv4x4_fp32(fp32 *in, fp32 *out)
//                                i18          i17
mvcvMatrixInv4x4_fp32_asm:
        lsu1.ldi.64.l  v0, i18   || cmu.cpzv    v4, 3
        lsu1.ldi.64.h  v0, i18   || iau.xor     i0, i0 i0
        lsu1.ldi.64.l  v1, i18   || lsu0.ldih   i0, 0x3f80 // i0 = 1.0f
        lsu1.ldi.64.h  v1, i18   || cmu.cpiv  v4.0, i0
        lsu1.ldi.64.l  v2, i18   || cmu.cpiv  v5.1, i0
        lsu1.ldi.64.h  v2, i18   || cmu.cpiv  v6.2, i0
        lsu1.ld.64.l   v3, i18   || cmu.cpiv  v7.3, i0
        lsu1.ldo.64.h  v3, i18 8 || cmu.cpvi    i1, v0.0

        iau.shl        i2, i1  1
        cmu.cmz.f32    v1
        vau.add.f32    v0, v0 v1 || peu.pcc0i.and neq eq
        vau.add.f32    v4, v4 v5 || peu.pcc0i.and neq eq || iau.incs i2, 1

        cmu.cmz.f32    v2
        vau.add.f32    v0, v0 v2 || peu.pcc0i.and neq eq
        vau.add.f32    v4, v4 v6 || peu.pcc0i.and neq eq || iau.incs i2, 1

        vau.add.f32    v0, v0 v3 || peu.pc1i eq
        vau.add.f32    v4, v4 v7 || peu.pc1i eq
        nop


        cmu.cpvi       i1, v0.0    // i1 = 1/a00
        sau.div.f32    i1, i0 i1
        cmu.cpvi       i2, v1.0
        cmu.cpivr.x32  v9, i2      //  v9 = { a10, a10, a10, a10 }
        cmu.cpvi       i2, v2.0
        cmu.cpivr.x32 v10, i2      // v10 = { a20, a20, a20, a20 }
        cmu.cpvi       i2, v3.0
        cmu.cpivr.x32 v11, i2      // v11 = { a30, a30, a30, a30 }
        nop 5

        cmu.cpivr.x32  v8, i1      //  v8 = { 1/a00, 1/a00, 1/a00, 1/a00 }
        vau.mul.f32    v0, v0 v8   //  v0 = { 1.0, a01/a00, a02/a00, a03/a00 }
        vau.mul.f32    v4, v4 v8
        nop
        vau.mul.f32    v9, v0 v9   //  v9 = a10 *{ 1.0, a01/a00, a02/a00, a03/a00 }
        vau.mul.f32   v13, v4 v9
        vau.mul.f32   v10, v0 v10  // v10 = a20 *{ 1.0, a01/a00, a02/a00, a03/a00 }
        vau.mul.f32   v14, v4 v10
        vau.mul.f32   v11, v0 v11  // v11 = a20 *{ 1.0, a01/a00, a02/a00, a03/a00 }
        vau.mul.f32   v15, v4 v11
        vau.sub.f32    v1, v1 v9
        vau.sub.f32    v5, v5 v13
        vau.sub.f32    v2, v2 v10
// v1 is now { 0.0, x, x, x } - ready to zero out column 1

        vau.sub.f32    v6, v6 v14 || cmu.cpvi    i1, v1.1
        vau.sub.f32    v3, v3 v11 || iau.shl     i2, i1  1
        vau.sub.f32    v7, v7 v15 || cmu.cmz.f32 v2

        vau.add.f32    v1, v1 v2 || peu.pcc1i.and neq eq
        vau.add.f32    v5, v5 v6 || peu.pcc1i.and neq eq || iau.incs i2, 1

        vau.add.f32    v1, v1 v3 || peu.pc1i eq
        vau.add.f32    v5, v5 v7 || peu.pc1i eq
        nop

        cmu.cpvi       i1, v1.1
        sau.div.f32    i1, i0 i1  //  i1 = 1/a11'
        cmu.cpvi       i2, v2.1
        cmu.cpivr.x32 v10, i2     // v10 = { a21', a21', a21', a21' }
        cmu.cpvi       i2, v3.1
        cmu.cpivr.x32 v11, i2     // v11 = { a31', a31', a31', a31' }
        cmu.cpvi       i2, v0.1
        cmu.cpivr.x32  v8, i2     //  v8 = { a01', a01', a01', a01' }
        nop 5

        cmu.cpivr.x32  v9, i1     //  v9 = { 1/a11', 1/a11', 1/a11', 1/a11' }
        vau.mul.f32    v1, v1 v9  //  v1 = { 0.0, 1.0, a12'/a11', a13'/a11' }
        vau.mul.f32    v5, v5 v9
        nop
        vau.mul.f32   v10, v1 v10 // v10 = a21' *{ 0.0, 1.0, a12'/a11', a13'/a11' }
        vau.mul.f32   v14, v5 v10
        vau.mul.f32   v11, v1 v11 // v11 = a31' *{ 0.0, 1.0, a12'/a11', a13'/a11' }
        vau.mul.f32   v15, v5 v11
        vau.mul.f32    v8, v1 v8  //  v8 = a01' *{ 0.0, 1.0, a12'/a11', a13'/a11' }
        vau.mul.f32   v12, v5 v8
        vau.sub.f32    v2, v2 v10
        vau.sub.f32    v6, v6 v14
        vau.sub.f32    v3, v3 v11
// v2 is now { 0.0, 0.0, x, x } - ready to zero out column 2

        vau.sub.f32    v7, v7 v15 || cmu.cpvi i1, v2.2
        iau.shl        i2, i1  1

        vau.add.f32    v2, v2 v3  || peu.pc1i eq
        vau.add.f32    v6, v6 v7  || peu.pc1i eq
        nop


        cmu.cpvi       i1, v2.2
        sau.div.f32    i1, i0 i1  //  i1 = 1/a22"
        vau.sub.f32    v0, v0 v8
        vau.sub.f32    v4, v4 v12

        cmu.cpvi       i2, v3.2
        cmu.cpivr.x32 v11, i2     // v11 = { a32", a32", a32", a32" }
        cmu.cpvi       i2, v0.2
        cmu.cpivr.x32  v8, i2     //  v8 = { a02", a02", a02", a02" }
        cmu.cpvi       i2, v1.2
        cmu.cpivr.x32  v9, i2     //  v9 = { a12", a12", a12", a12" }
        nop 3

        cmu.cpivr.x32 v10, i1     // v10 = { 1/a22", 1/a22", 1/a22", 1/a22" }
        vau.mul.f32    v2, v2 v10 //  v2 = { 0.0, 0.0, 1.0, a23"/a22" }
        vau.mul.f32    v6, v6 v10
        nop
        vau.mul.f32   v11, v2 v11 // v11 = a32" *{ 0.0, 0.0, 1.0, a23"/a22" }
        vau.mul.f32   v15, v6 v11
        vau.mul.f32    v8, v2 v8  //  v8 = a02" *{ 0.0, 0.0, 1.0, a23"/a22" }
        vau.mul.f32   v12, v6 v8
        vau.mul.f32    v9, v2 v9  //  v9 = a12" *{ 0.0, 0.0, 1.0, a23"/a22" }
        vau.mul.f32   v13, v6 v9
        vau.sub.f32    v3, v3 v11
        vau.sub.f32    v7, v7 v15
        vau.sub.f32    v0, v0 v8
// v3 is now { 0.0, 0.0, 0.0, x } - ready to zero out column 3

        cmu.cpvi       i1, v3.3
        sau.div.f32    i1, i0 i1  //  i1 = 1/a33"'
        vau.sub.f32    v4, v4 v12
        vau.sub.f32    v1, v1 v9
        vau.sub.f32    v5, v5 v13

        cmu.cpvi       i2, v0.3
        cmu.cpivr.x32  v8, i2     //  v8 = { a03"', a03"', a03"', a03"' }
        cmu.cpvi       i2, v1.3
        cmu.cpivr.x32  v9, i2     //  v9 = { a13"', a13"', a13"', a13"' }
        cmu.cpvi       i2, v2.3
        cmu.cpivr.x32 v10, i2     // v10 = { a23"', a23"', a23"', a23"' }
        nop 2

        cmu.cpivr.x32 v11, i1     // v11 = { 1/a33"', 1/a33"', 1/a33"', 1/a33"' }
        vau.mul.f32    v7, v7 v11
        nop 2
        vau.mul.f32    v12, v7 v8  || lsu1.sto.64.l v7, i17 48
        vau.mul.f32    v13, v7 v9  || lsu1.sto.64.h v7, i17 56
        vau.mul.f32    v14, v7 v10 || bru.jmp i30
        vau.sub.f32   v4, v4 v12
        vau.sub.f32   v5, v5 v13
        vau.sub.f32   v6, v6 v14
        lsu1.st.64.l  v4, i17    || lsu0.sto.64.h v4, i17 8
        lsu1.sto.64.l v5, i17 16 || lsu0.sto.64.h v5, i17 24
        lsu1.sto.64.l v6, i17 32 || lsu0.sto.64.h v6, i17 40

.size mvcvMatrixInv4x4_fp32_asm,.-mvcvMatrixInv4x4_fp32_asm
.end
