///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvCalcEpipolarDistance_asm,@function
.version 00.50.00

.data .data.calcEpipolarDistance
.align 4
.code .text.calcEpipolarDistance

//float mvcvCalcEpipolarDistance(float *p_1, float *p_2, int nPoints, float RANSAC_dist_threshold, float *fm, int *inliers, int *nInliers)
//                                      i18         i17          i16                          i15        i14           i13            i12
mvcvCalcEpipolarDistance_asm:
.nowarn
    IAU.SUB i19 i19 16        || lsu0.ldil i1 1           || lsu1.ldih i1 0
    LSU0.STo.64.l v24  i19 0  || LSU1.STo.64.h v24  i19 8 || IAU.SUB i19 i19 16 || cmu.cpivr.x32 v18 i1
    LSU0.STo.64.l v25  i19 0  || LSU1.STo.64.h v25  i19 8 || IAU.SUB i19 i19 16 || cmu.cpivr.x32 v21 i15
    LSU0.STo.64.l v26  i19 0  || LSU1.STo.64.h v26  i19 8 || IAU.SUB i19 i19 16 || vau.mul.f32 v21 v21 v21 //RANSAC_dist_threshold^2
    LSU0.STo.64.l v27  i19 0  || LSU1.STo.64.h v27  i19 8 || IAU.SUB i19 i19 16
    LSU0.STo.64.l v28  i19 0  || LSU1.STo.64.h v28  i19 8 || IAU.SUB i19 i19 16
    LSU0.STo.64.l v29  i19 0  || LSU1.STo.64.h v29  i19 8 || IAU.SUB i19 i19 16
    LSU0.STo.64.l v30  i19 0  || LSU1.STo.64.h v30  i19 8 || IAU.SUB i19 i19 16
    LSU0.STo.64.l v31  i19 0  || LSU1.STo.64.h v31  i19 8 || IAU.SUB i19 i19 8
    LSU0.ST.64  i20  i19      || IAU.SUB i19 i19 8
    LSU1.ST.64  i22  i19      || IAU.SUB i19 i19 8
    LSU0.ST.64  i24  i19      || IAU.SUB i19 i19 8
    LSU1.ST.64  i26  i19      || iau.shr.u32 i21 i16 2
    iau.xor i9 i9 i9
    iau.xor i11 i11 i11
    iau.xor i8 i8 i8

    //load p1 and p2
    lsu0.ld.64.l v0 i18       || lsu1.ldo.64.h v0 i18 0x08
    lsu0.ldo.64.l v1 i18 0x10 || lsu1.ldo.64.h v1 i18 0x18 || iau.incs i18 0x20
    lsu0.ld.64.l v2 i17       || lsu1.ldo.64.h v2 i17 0x08
    lsu0.ldo.64.l v3 i17 0x10 || lsu1.ldo.64.h v3 i17 0x18 || iau.incs i17 0x20
    vau.xor v19 v19 v19       || lsu0.ld.32r v31 i14       || iau.incs i14 4
                                 lsu0.ld.32r v30 i14       || iau.incs i14 4
    vau.xor v20 v20 v20       || lsu0.ld.32r v29 i14       || iau.incs i14 4
                                 lsu0.ld.32r v28 i14       || iau.incs i14 4
    cmu.vdilv.x32 v5 v4 v0 v1 || lsu0.ld.32r v27 i14       || iau.incs i14 4 //v4 even [0, 2, 4, 6] v5 odd [1, 3, 5, 7] pos
                                 lsu0.ld.32r v26 i14       || iau.incs i14 4
    cmu.vdilv.x32 v7 v6 v2 v3 || lsu0.ld.32r v25 i14       || iau.incs i14 4 //v6 even v7 odd pos
    vau.mul.f32 v8 v4 v31     || lsu0.ld.32r v24 i14       || iau.incs i14 4 ////p_1[2*i]*fm[0]
    vau.mul.f32 v9 v5 v30 //// p_1[2*i + 1]*fm[1]
                                 lsu0.ld.32r v23 i14       || iau.xor i10 i10 i10
    vau.mul.f32 v10 v4 v28 ////p_1[2*i]*fm[3]
    vau.mul.f32 v11 v5 v27 ////p_1[2*i + 1]*fm[4]
    vau.add.f32 v8 v8 v9
    vau.mul.f32 v12 v4 v25 ////p_1[2*i]*fm[6]
    vau.mul.f32 v13 v5 v24 ////p_1[2*i + 1]*fm[7]
    vau.add.f32 v10 v10 v11
    vau.add.f32 v8 v8 v29   ////l[0]
    vau.add.f32 v12 v12 v13
    vau.add.f32 v10 v10 v26 ////l[1]
    vau.mul.f32 v13 v8 v8 //// l[0]*l[0]
    vau.add.f32 v12 v12 v23 ////l[2]
    vau.mul.f32 v14 v10 v10 ////l[1]*l[1]
    vau.mul.f32 v8 v8 v6 ////l[0] * p_2[2*i]
    vau.mul.f32 v10 v10 v7 ////l[1] * p_2[2*i + 1]
    vau.add.f32 v15 v13 v14 ////l[0]*l[0] + l[1]*l[1]
    vau.add.f32 v16 v8 v12 ////l[0] * p_2[2 * i] + l[2]
    lsu0.ldil i20 __calcEpipolarDistLoop
    lsu1.ldih i20 __calcEpipolarDistLoop



    vau.add.f32 v17 v16 v10 || lsu0.ld.64.l v0 i18       || lsu1.ldo.64.h v0 i18 0x08 || bru.rpl i20 i21 __calcEpipolarDistLoopEnd ////v17 - tmp
    cmu.cpvi i4 v15.0       || lsu0.ldo.64.l v1 i18 0x10 || lsu1.ldo.64.h v1 i18 0x18 || vau.xor v19 v19 v19
    cmu.cpvi i5 v15.1       || lsu0.ld.64.l v2 i17       || lsu1.ldo.64.h v2 i17 0x08 || iau.incs i18 0x20
    vau.mul.f32 v17 v17 v17 || lsu0.ldo.64.l v3 i17 0x10 || lsu1.ldo.64.h v3 i17 0x18 || iau.incs i17 0x20 ////v17 - tmp^2
    cmu.cpvi i6 v15.2
    cmu.cpvi i7 v15.3
    cmu.cpvi i0 v17.0
    cmu.cpvi i1 v17.1    || sau.div.f32 i0 i0 i4
    cmu.cpvi i2 v17.2    || sau.div.f32 i1 i1 i5
    cmu.cpvi i3 v17.3    || sau.div.f32 i2 i2 i6
    sau.div.f32 i3 i3 i7 || cmu.vdilv.x32 v5 v4 v0 v1 //v4 even [0, 2, 4, 6] v5 odd [1, 3, 5, 7] pos
    cmu.vdilv.x32 v7 v6 v2 v3 //v6 even v7 odd pos
    vau.mul.f32 v8 v4 v31 ////p_1[2*i]*fm[0]
    vau.mul.f32 v9 v5 v30 //// p_1[2*i + 1]*fm[1]
    nop
    vau.mul.f32 v10 v4 v28 ////p_1[2*i]*fm[3]
    vau.mul.f32 v11 v5 v27 ////p_1[2*i + 1]*fm[4]
    vau.add.f32 v8 v8 v9
    vau.mul.f32 v12 v4 v25 ////p_1[2*i]*fm[6]
    cmu.cpiv v17.0 i0 || vau.mul.f32 v13 v5 v24 ////p_1[2*i + 1]*fm[7]
    cmu.cpiv v17.1 i1 || vau.add.f32 v10 v10 v11
    cmu.cpiv v17.2 i2 || vau.add.f32 v8 v8 v29   ////l[0]
    cmu.cpiv v17.3 i3 || vau.add.f32 v12 v12 v13// v17 distsqr
    cmu.cmvv.f32 v17 v21
    peu.pvv32 LT || vau.add.i32 v19 v19 v18
    vau.add.f32 v10 v10 v26 ////l[1]
    peu.pvv32 GTE || vau.mul.f32 v17 v17 v20
    sau.sumx.u32 i8 v19 ||  vau.mul.f32 v13 v8 v8 //// l[0]*l[0]
    vau.add.f32 v12 v12 v23 ////l[2]

     __calcEpipolarDistLoop:
    sau.sumx.f32 i9 v17 || vau.mul.f32 v14 v10 v10 ////l[1]*l[1]
    iau.add i11 i11 i8  || vau.mul.f32 v8 v8 v6 ////l[0] * p_2[2*i]
    vau.mul.f32 v10 v10 v7 ////l[1] * p_2[2*i + 1]
    vau.add.f32 v15 v13 v14 ////l[0]*l[0] + l[1]*l[1]
    vau.add.f32 v16 v8 v12 ////l[0] * p_2[2 * i] + l[2]
    lsu0.sti.64.l v19 i13 || sau.add.f32 i10 i10 i9
    lsu0.sti.64.h v19 i13
    __calcEpipolarDistLoopEnd:
    lsu0.st.32 i11 i12        || cmu.cpii.i32.f32 i11 i11 || bru.bra sqrtF32
    nop 2
    cmu.cpii i18 i10
    nop 3
    __calcEpipolarDistEnd:
    sau.div.f32 i18 i18 i11
    LSU0.LD.64  i26  i19      || IAU.ADD i19 i19 8
    LSU0.LD.64  i24  i19      || IAU.ADD i19 i19 8
    LSU0.LD.64  i22  i19      || IAU.ADD i19 i19 8
    LSU0.LD.64  i20  i19      || IAU.ADD i19 i19 8
    LSU0.LDo.64.l  v31  i19 0 || LSU1.LDo.64.h  v31  i19 8 ||  IAU.ADD i19 i19 16
    LSU0.LDo.64.l  v30  i19 0 || LSU1.LDo.64.h  v30  i19 8 ||  IAU.ADD i19 i19 16
    LSU0.LDo.64.l  v29  i19 0 || LSU1.LDo.64.h  v29  i19 8 ||  IAU.ADD i19 i19 16
    LSU0.LDo.64.l  v28  i19 0 || LSU1.LDo.64.h  v28  i19 8 ||  IAU.ADD i19 i19 16
    LSU0.LDo.64.l  v27  i19 0 || LSU1.LDo.64.h  v27  i19 8 ||  IAU.ADD i19 i19 16
    LSU0.LDo.64.l  v26  i19 0 || LSU1.LDo.64.h  v26  i19 8 ||  IAU.ADD i19 i19 16
    LSU0.LDo.64.l  v25  i19 0 || LSU1.LDo.64.h  v25  i19 8 ||  IAU.ADD i19 i19 16
    LSU0.LDo.64.l  v24  i19 0 || LSU1.LDo.64.h  v24  i19 8 ||  IAU.ADD i19 i19 16 || BRU.jmp i30
    nop 6


// 48 instructions, 48instr/value

// float sqrtf( float x )
// {
//   int exp//
//   float f = frexp( x, &exp )//
//   f *= 0.5//
//   float y = 0.41731 + 1.18032*f//
//   y = y * 0.5 + f / y//
//   y = y * 0.5 + f / y//
//   if( exp & 1 ) {
//     y *= __SQRT_HALF//
//     ++exp//
//   }
//   exp >>= 1//
//   y = ldexpf( y, exp )//
//   if( isinf( x )|| isnan( x ))
//     y = x//
//   if( !x )
//     y = 0//
//   if( x < 0 )
//     y = NAN//
//   return y//
// }

.size mvcvCalcEpipolarDistance_asm,.-mvcvCalcEpipolarDistance_asm
  .code .text.sqrtF32
  .salign 16
  sqrtF32:
  .set   x i18
  .set   f i17
  .set   y i16
  .set exp i15
  .set nex i14 // new exponent

  // {f, exp+126} = frexp_v4f(x)//
  // f *= 0.5
  // y = f * 1.18032 + 0.41731
  // optimizations: assume positive, don't check for 0/Inf/NaN input (handled at the end)
                             lsu0.ldil i0, 0xFFFF     || lsu1.ldih i0, 0x807F
  iau.and       f, x  i0  || lsu0.ldil i1, 0          || lsu1.ldih i1, 0x3E80 // i1=0x3e800000
  iau.or        f, f  i1  || lsu0.ldil i2, 0x14ba || lsu1.ldih i2, 0x3f97
  sau.mul.f32   y, f i2                               // y=f*1.18032
  iau.not      i0, i0     || lsu0.ldil i2, 0xa9a8 // i0=0x7f800000
  iau.and     exp, x i0   || lsu0.ldih i2, 0x3ed5
  sau.add.f32   y, y i2                               // y=f*1.18032+0.41731 i1=0x00800000
  lsu1.cp     nex, exp    || lsu0.ldil i2, .Lhlast
                             lsu0.ldih i2, .Lhlast
  bru.rps   i2 2
  sau.div.f32  i6, f y    // i6=f/y
  sau.mul.f32   y, y 0.5       // y*=0.5
  lsu0.ldil    i5, 0x04f3
  lsu0.ldih    i5, 0x3f35
  lsu0.ldih    i1, 0x0080      // i1=0x00800000
  iau.shl      i6, i1 8        // i6=0x80000000
  iau.not      i2, i6          // i2=0x7fffffff
  lsu0.ldil    i3, 0x0000
.Lhlast:
    lsu0.ldih  i3, 0x3f00      // i3=0x3f000000
    lsu0.ldil  i4, 0x0000
    lsu0.ldih  i4, 0x7fc0      // i4=qNaN
    iau.or     i4, x i4
    sau.add.f32 y, y i6 // y=y*0.5+f/y
    nop
    iau.and    i6, exp i1
  iau.add     nex, exp i1 || sau.mul.f32 y, y i5 || peu.pc1i neq // if(exp&1)nex=exp+1,y*=sqrt(.5)
  iau.sub     nex, nex i3
  iau.shr.i32 nex, nex 1  || bru.bra __calcEpipolarDistEnd
    iau.and   nex, nex i0 || cmu.cmii.i32 exp  i0
    iau.add     y, y nex  || sau.or       exp, exp exp
  // if( exp==0xff ) y=x//
    lsu0.cp     y, x      || peu.pc1c eq
  // if( !exp ) y=0.0//
    lsu0.ldil   y, 0      || peu.pc1s eq
    sau.and   i18, y i2   || iau.add i0, x 0
  // if( x<0 ) y=qNaN//
    lsu0.cp   i18, i4     || peu.pc1i lt
.nowarnend
.end
