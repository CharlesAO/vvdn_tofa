///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief   
///

.type mvcvBilateral5x5_asm,@function
.version 00.50.01
//-------------------------------------------------------------------------------    
.code .text.bilateral5x5
//   void bilateral5x5(u16* input, u16* sigma, u32 width, u16* output)//
//                           i18         i17         i16         i15 
mvcvBilateral5x5_asm:

    IAU.SUB i19 i19 8
    LSU0.ST.64.l v24  i19   || IAU.SUB i19 i19 8
    LSU0.ST.64.h v24  i19   || IAU.SUB i19 i19 8
    LSU0.ST.64.l v25  i19   || IAU.SUB i19 i19 8
    LSU0.ST.64.h v25  i19   || IAU.SUB i19 i19 8
    LSU0.ST.64.l v26  i19   || IAU.SUB i19 i19 8
    LSU0.ST.64.h v26  i19   || IAU.SUB i19 i19 8
    LSU0.ST.64.l v27  i19   || IAU.SUB i19 i19 8
    LSU0.ST.64.h v27  i19   || IAU.SUB i19 i19 8
    LSU0.ST.64.l v28  i19   || IAU.SUB i19 i19 8
    LSU0.ST.64.h v28  i19
    
    
    lsu0.ldxv v3 i18    || iau.mul i0 i16 2
    nop
    iau.sub i2 i18 i0   || cmu.cpii i3 i18  || lsu1.ldil i6 0x10
    lsu0.ldxvi v6 i17 i6
    nop
    lsu1.ldxv v7 i17
    lsu0.ldxv v2 i2     || iau.sub i1 i2 i0
    lsu1.ldxv v1 i1     || iau.add i4 i3 i0
    lsu0.ldxv v4 i4     || iau.add i5 i4 i0
    lsu1.ldxv v5 i5     || iau.incs i1 0x10
    
    lsu0.ldxvi v11 i1 i6    || iau.incs i2 0x10
    lsu1.ldxvi v12 i2 i6    || iau.incs i3 0x10
    lsu0.ldxvi v13 i3 i6    || iau.incs i4 0x10
    lsu1.ldxvi v14 i4 i6    || iau.incs i5 0x10
    lsu0.ldxvi v15 i5 i6    || iau.shr.i32 i12 i16 3
    

    cmu.cpvi.x32 i0 v3.0
    lsu1.sti.32 i0 i15      || cmu.alignvec v6 v6 v7 4
    cmu.cpvv.i16.f16 v1 v1
    cmu.cpvv.i16.f16 v2 v2 || iau.shl i14 i16 1
    cmu.cpvv.i16.f16 v3 v3
    cmu.cpvv.i16.f16 v4 v4 || iau.add i13 i15 i14
    cmu.cpvv.i16.f16 v5 v5
    cmu.cpvv.i16.f16 v11 v11 || lsu0.ld.32 i14 i13
    cmu.cpvv.i16.f16 v12 v12
    cmu.cpvv.i16.f16 v13 v13
    cmu.cpvv.i16.f16 v14 v14
    cmu.cpvv.i16.f16 v15 v15
       
    lsu1.ldil i18 0x0001
    cmu.cpivr.x16 v24 i18   || lsu1.ldil i0 0xb800  ////1 -> v24
    cmu.cpivr.x16 v8 i0     || lsu1.ldil i0 0x3DC5  //// -0.5 -> v8
    cmu.cpivr.x16 v9 i0     || lsu1.ldil i0 0x7fff  //// 1.44269504 -> v9
    cmu.cpivr.x16 v16 i0    //// 0x7fff -> v16
    lsu0.ldil i0 bilateral5x5_loopBilateral5x5  || lsu1.ldih i0 bilateral5x5_loopBilateral5x5
    
    
    vau.xor v20 v20 v20     || cmu.min.u16 v6 v6 v16 //v20 sumDepth
    vau.xor v21 v21 v21         //v21 sumWeights
    cmu.max.u16 v6 v6 v24
    nop 
    cmu.cpvv.i16.f16 v6 v6       //// v6 sigma
    nop 
    sau.rcp i7 v6.0
    sau.rcp i8 v6.1
    sau.rcp i9 v6.2
    sau.rcp i10 v6.3
    sau.rcp i11 v6.4
    sau.rcp i7 v6.5     || cmu.cpiv.x16 v6.0 i7.0
    sau.rcp i8 v6.6     || cmu.cpiv.x16 v6.1 i8.0
    sau.rcp i9 v6.7     || cmu.cpiv.x16 v6.2 i9.0
    cmu.cpiv.x16 v6.3 i10.0
    cmu.cpiv.x16 v6.4 i11.0
    cmu.cpiv.x16 v6.5 i7.0
    cmu.cpiv.x16 v6.6 i8.0
    cmu.cpiv.x16 v6.7 i9.0
    cmu.alignvec v0 v3 v13 4     //v0 central value

    vau.mul.f16 v6 v6 v8 ////sigmaFactor -> v6
    vau.adiff.f16 v10 v1 v0
    cmu.alignvec v19 v1 v11 2
    vau.mul.f16 v6 v6 v9 ////sigmaFactor * log2(e)
    nop 2

    //step1:
    vau.mul.f16 v17 v10 v6  ||  cmu.alignvec v28 v3 v13 2  || bru.rpl i0 i12    //sigmaFactor*dist -> v17
    cmu.alignvec v27 v2 v12 2
    vau.adiff.f16 v10 v2 v0 //dist -> v10
    sau.exp2 i7 v17.0
    sau.exp2 i8 v17.1
    sau.exp2 i9 v17.2           || vau.mul.f16 v25 v10 v6 //sigmaFactor*dist -> v25
    sau.exp2 i10 v17.3          || vau.adiff.f16 v10 v3 v0//dist -> v10
    sau.exp2 i11 v17.4           
    sau.exp2 i7 v17.5           || cmu.cpiv.x16 v17.0 i7.0
    sau.exp2 i8 v17.6           || cmu.cpiv.x16 v17.1 i8.0  || vau.mul.f16 v26 v10 v6 //sigmaFactor*dist -> v26
    sau.exp2 i9 v17.7           || cmu.cpiv.x16 v17.2 i9.0
    cmu.cpiv.x16 v17.3 i10.0    || sau.exp2 i7 v25.0
    cmu.cpiv.x16 v17.4 i11.0    || sau.exp2 i8 v25.1
    cmu.cpiv.x16 v17.5 i7.0     || sau.exp2 i9 v25.2
    cmu.cpiv.x16 v17.6 i8.0     || sau.exp2 i10 v25.3
    cmu.cpiv.x16 v17.7 i9.0     || sau.exp2 i11 v25.4
    vau.mul.f16 v18 v17 v1      || sau.exp2 i7 v25.5        || cmu.cpiv.x16 v25.0 i7.0//weights*dCurrent - > v18
                                   sau.exp2 i8 v25.6        || cmu.cpiv.x16 v25.1 i8.0
                                   sau.exp2 i9 v25.7        || cmu.cpiv.x16 v25.2 i9.0
    vau.mul.f16 v18 v18 0.5     || cmu.cpiv.x16 v25.3 i10.0 || sau.exp2 i10 v26.0
    cmu.cpiv.x16 v25.4 i11.0    || sau.exp2 i11 v26.1       || vau.adiff.f16 v10 v4 v0//dist -> v10
    vau.add.f16 v21 v21 v17     || cmu.cpiv.x16 v25.5 i7.0  || sau.exp2 i7 v26.2
    vau.add.f16 v20 v20 v18     || cmu.cpiv.x16 v25.6 i8.0  || sau.exp2 i8 v26.3

    cmu.cpiv.x16 v25.7 i9.0     || sau.exp2 i9 v26.4        ||  vau.mul.f16 v17 v10 v6 //sigmaFactor*dist -> v17
    vau.mul.f16 v18 v25 v2      || sau.exp2 i10 v26.5       || cmu.cpiv.x16 v26.0 i10.0//weights*dCurrent - > v18
                                    sau.exp2 i11 v26.6      || cmu.cpiv.x16 v26.1 i11.0 || vau.adiff.f16 v10 v5 v0//dist -> v10
                                    sau.exp2 i7 v26.7       || cmu.cpiv.x16 v26.2 i7.0
    vau.mul.f16 v18 v18 0.5     || cmu.cpiv.x16 v26.3 i8.0  || sau.exp2 i7 v17.0
    cmu.cpiv.x16 v26.4 i9.0     || sau.exp2 i8 v17.1        
    cmu.cpiv.x16 v26.5 i10.0    || sau.exp2 i9 v17.2        || vau.add.f16 v21 v21 v25
    vau.add.f16 v20 v20 v18     || cmu.cpiv.x16 v26.6 i11.0 || sau.exp2 i10 v17.3
    cmu.cpiv.x16 v26.7 i7.0     || sau.exp2 i11 v17.4       || vau.mul.f16 v25 v10 v6 //sigmaFactor*dist -> v25

    vau.mul.f16 v18 v26 v3      || sau.exp2 i7  v17.5       || cmu.cpiv.x16 v17.0 i7.0//weights*dCurrent - > v18
                                    sau.exp2 i8 v17.6       || cmu.cpiv.x16 v17.1 i8.0  || vau.adiff.f16 v10 v19 v0 //dist -> v10
                                    sau.exp2 i9 v17.7       || cmu.cpiv.x16 v17.2 i9.0
    vau.mul.f16 v18 v18 0.5     || cmu.cpiv.x16 v17.3 i10.0 || sau.exp2 i7 v25.0
    cmu.cpiv.x16 v17.4 i11.0    || sau.exp2 i8 v25.1        || vau.add.f16 v21 v21 v26
    cmu.cpiv.x16 v17.5 i7.0     || sau.exp2 i9 v25.2        || vau.mul.f16 v26 v10 v6 //sigmaFactor*dist -> v26
    vau.add.f16 v20 v20 v18     || cmu.cpiv.x16 v17.6 i8.0  || sau.exp2 i10 v25.3
                                    cmu.cpiv.x16 v17.7 i9.0 || sau.exp2 i11 v25.4

    vau.mul.f16 v18 v17 v4      || sau.exp2 i7  v25.5       || cmu.cpiv.x16 v25.0 i7.0 //weights*dCurrent - > v18
                                    sau.exp2 i8 v25.6       || cmu.cpiv.x16 v25.1 i8.0  || vau.adiff.f16 v10 v27 v0 //dist -> v10
                                    sau.exp2 i9 v25.7       || cmu.cpiv.x16 v25.2 i9.0
    vau.mul.f16 v18 v18 0.5     || cmu.cpiv.x16 v25.3 i10.0 || sau.exp2 i7 v26.0
    cmu.cpiv.x16 v25.4 i11.0    || sau.exp2 i8 v26.1        || vau.add.f16 v21 v21 v17
    cmu.cpiv.x16 v25.5 i7.0     || sau.exp2 i9 v26.2
    vau.add.f16 v20 v20 v18     || cmu.cpiv.x16 v25.6 i8.0  || sau.exp2 i10 v26.3
                                    cmu.cpiv.x16 v25.7 i9.0 || sau.exp2 i11 v26.4   ||  vau.mul.f16 v17 v10 v6 //sigmaFactor*dist -> v17

    vau.mul.f16 v18 v25 v5      || sau.exp2 i7  v26.5       || cmu.cpiv.x16 v26.0 i7.0   //weights*dCurrent - > v18
                                    sau.exp2 i8 v26.6       || cmu.cpiv.x16 v26.1 i8.0  || vau.adiff.f16 v10 v28 v0 //dist -> v10
                                    sau.exp2 i9 v26.7       || cmu.cpiv.x16 v26.2 i9.0
    vau.mul.f16 v18 v18 0.5     || cmu.cpiv.x16 v26.3 i10.0 || sau.exp2 i7 v17.0
    vau.add.f16 v21 v21 v25     || cmu.cpiv.x16 v26.4 i11.0 || sau.exp2 i8 v17.1
                                    cmu.cpiv.x16 v26.5 i7.0 || sau.exp2 i9 v17.2        || vau.mul.f16 v25 v10 v6 //sigmaFactor*dist -> v25
    vau.add.f16 v20 v20 v18     || cmu.cpiv.x16 v26.6 i8.0  || sau.exp2 i10 v17.3
                                    cmu.cpiv.x16 v26.7 i9.0 || sau.exp2 i11 v17.4
    
    vau.mul.f16 v18 v26 v19     || sau.exp2 i7  v17.5   || cmu.cpiv.x16 v17.0 i7.0 //weights*dCurrent - > v18
                                    sau.exp2 i8 v17.6   || cmu.cpiv.x16 v17.1 i8.0
                                    sau.exp2 i9 v17.7   || cmu.cpiv.x16 v17.2 i9.0
    cmu.alignvec v19 v4 v14 2
    vau.mul.f16 v18 v18 0.5     || cmu.cpiv.x16 v17.3 i10.0     || sau.exp2 i7 v25.0
                                    cmu.cpiv.x16 v17.4 i11.0    || sau.exp2 i8 v25.1    || vau.adiff.f16 v10 v19 v0//dist -> v10
    vau.add.f16 v21 v21 v26     || cmu.cpiv.x16 v17.5 i7.0      || sau.exp2 i9 v25.2
    vau.add.f16 v20 v20 v18     || cmu.cpiv.x16 v17.6 i8.0      || sau.exp2 i10 v25.3
                                    cmu.cpiv.x16 v17.7 i9.0     || sau.exp2 i11 v25.4   || vau.mul.f16 v26 v10 v6 //sigmaFactor*dist -> v16

    vau.mul.f16 v18 v17 v27     || sau.exp2 i7  v25.5   || cmu.cpiv.x16 v25.0 i7.0 //weights*dCurrent - > v18
                                    sau.exp2 i8 v25.6   || cmu.cpiv.x16 v25.1 i8.0
                                    sau.exp2 i9 v25.7   || cmu.cpiv.x16 v25.2 i9.0
    cmu.alignvec v27 v5 v15 2                               
    vau.mul.f16 v18 v18 0.5     || cmu.cpiv.x16 v25.3 i10.0     || sau.exp2 i7 v26.0
                                    cmu.cpiv.x16 v25.4 i11.0    || sau.exp2 i8 v26.1    || vau.adiff.f16 v10 v27 v0//dist -> v10
    vau.add.f16 v21 v21 v17     || cmu.cpiv.x16 v25.5 i7.0      || sau.exp2 i9 v26.2
    vau.add.f16 v20 v20 v18     || cmu.cpiv.x16 v25.6 i8.0      || sau.exp2 i10 v26.3
                                    cmu.cpiv.x16 v25.7 i9.0     || sau.exp2 i11 v26.4   || vau.mul.f16 v17 v10 v6 //sigmaFactor*dist -> v17

    vau.mul.f16 v18 v25 v28     || sau.exp2 i7  v26.5   || cmu.cpiv.x16 v26.0 i7.0 //weights*dCurrent - > v18
                                    sau.exp2 i8 v26.6   || cmu.cpiv.x16 v26.1 i8.0
                                    sau.exp2 i9 v26.7   || cmu.cpiv.x16 v26.2 i9.0
    cmu.alignvec v28 v1 v11 4                               
    vau.mul.f16 v18 v18 0.5     || cmu.cpiv.x16 v26.3 i10.0 || sau.exp2 i7 v17.0
                                cmu.cpiv.x16 v26.4 i11.0    || sau.exp2 i8 v17.1    || vau.adiff.f16 v10 v28 v0 //dist -> v10
    vau.add.f16 v21 v21 v25     ||  cmu.cpiv.x16 v26.5 i7.0 || sau.exp2 i9 v17.2    
    vau.add.f16 v20 v20 v18     || cmu.cpiv.x16 v26.6 i8.0  || sau.exp2 i10 v17.3
                                cmu.cpiv.x16 v26.7 i9.0     || sau.exp2 i11 v17.4   || vau.mul.f16 v25 v10 v6 //sigmaFactor*dist -> v25

    vau.mul.f16 v18 v26 v19     || sau.exp2 i7  v17.5   || cmu.cpiv.x16 v17.0 i7.0 //weights*dCurrent - > v18
                                    sau.exp2 i8 v17.6   || cmu.cpiv.x16 v17.1 i8.0
                                    sau.exp2 i9 v17.7   || cmu.cpiv.x16 v17.2 i9.0
    cmu.alignvec v19 v2 v12 4                               
    vau.mul.f16 v18 v18 0.5     || cmu.cpiv.x16 v17.3 i10.0 || sau.exp2 i7 v25.0
                                cmu.cpiv.x16 v17.4 i11.0    || sau.exp2 i8 v25.1    || vau.adiff.f16 v10 v19 v0//dist -> v10
    vau.add.f16 v21 v21 v26     || cmu.cpiv.x16 v17.5 i7.0  || sau.exp2 i9 v25.2
    vau.add.f16 v20 v20 v18     || cmu.cpiv.x16 v17.6 i8.0  || sau.exp2 i10 v25.3
                                    cmu.cpiv.x16 v17.7 i9.0 || sau.exp2 i11 v25.4   || vau.mul.f16 v26 v10 v6 //sigmaFactor*dist -> v26

    vau.mul.f16 v18 v17 v27     || sau.exp2 i7  v25.5   || cmu.cpiv.x16 v25.0 i7.0 //weights*dCurrent - > v18
                                    sau.exp2 i8 v25.6   || cmu.cpiv.x16 v25.1 i8.0
                                    sau.exp2 i9 v25.7   || cmu.cpiv.x16 v25.2 i9.0
    cmu.alignvec v27 v3 v13 4
    vau.mul.f16 v18 v18 0.5 || cmu.cpiv.x16 v25.3 i10.0 || sau.exp2 i7 v26.0
                            cmu.cpiv.x16 v25.4 i11.0    || sau.exp2 i8 v26.1    || vau.adiff.f16 v10 v27 v0 //dist -> v10
    vau.add.f16 v21 v21 v17 || cmu.cpiv.x16 v25.5 i7.0  || sau.exp2 i9 v26.2
    vau.add.f16 v20 v20 v18 || cmu.cpiv.x16 v25.6 i8.0  || sau.exp2 i10 v26.3
                                cmu.cpiv.x16 v25.7 i9.0 || sau.exp2 i11 v26.4   || vau.mul.f16 v17 v10 v6 //sigmaFactor*dist -> v17
    
    
    vau.mul.f16 v18 v25 v28 || sau.exp2 i7  v26.5   || cmu.cpiv.x16 v26.0 i7.0 //weights*dCurrent - > v18
                                sau.exp2 i8 v26.6   || cmu.cpiv.x16 v26.1 i8.0
                                sau.exp2 i9 v26.7   || cmu.cpiv.x16 v26.2 i9.0
    cmu.alignvec v28 v4 v14 4
    vau.mul.f16 v18 v18 0.5 || cmu.cpiv.x16 v26.3 i10.0 || sau.exp2 i7 v17.0
                            cmu.cpiv.x16 v26.4 i11.0    || sau.exp2 i8 v17.1    || vau.adiff.f16 v10 v28 v0 //dist -> v10
    vau.add.f16 v21 v21 v25 || cmu.cpiv.x16 v26.5 i7.0  || sau.exp2 i9 v17.2
    vau.add.f16 v20 v20 v18 || cmu.cpiv.x16 v26.6 i8.0  || sau.exp2 i10 v17.3
                                cmu.cpiv.x16 v26.7 i9.0 || sau.exp2 i11 v17.4   || vau.mul.f16 v25 v10 v6 //sigmaFactor*dist -> v25

    vau.mul.f16 v18 v26 v19 || sau.exp2 i7  v17.5   || cmu.cpiv.x16 v17.0 i7.0 //weights*dCurrent - > v18
                                sau.exp2 i8 v17.6   || cmu.cpiv.x16 v17.1 i8.0
                                sau.exp2 i9 v17.7   || cmu.cpiv.x16 v17.2 i9.0
    cmu.alignvec v19 v5 v15 4
    vau.mul.f16 v18 v18 0.5 || cmu.cpiv.x16 v17.3 i10.0 || sau.exp2 i7 v25.0
                            cmu.cpiv.x16 v17.4 i11.0    || sau.exp2 i8 v25.1    || vau.adiff.f16 v10 v19 v0 //dist -> v10
    vau.add.f16 v21 v21 v26 || cmu.cpiv.x16 v17.5 i7.0  || sau.exp2 i9 v25.2
    vau.add.f16 v20 v20 v18 || cmu.cpiv.x16 v17.6 i8.0  || sau.exp2 i10 v25.3
                                cmu.cpiv.x16 v17.7 i9.0 || sau.exp2 i11 v25.4   || vau.mul.f16 v26 v10 v6 //sigmaFactor*dist -> v26

    vau.mul.f16 v18 v17 v27 || sau.exp2 i7  v25.5   || cmu.cpiv.x16 v25.0 i7.0 //weights*dCurrent - > v18
                                sau.exp2 i8 v25.6   || cmu.cpiv.x16 v25.1 i8.0
                                sau.exp2 i9 v25.7   || cmu.cpiv.x16 v25.2 i9.0
    cmu.alignvec v27 v1 v11 6
    vau.mul.f16 v18 v18 0.5 || cmu.cpiv.x16 v25.3 i10.0 || sau.exp2 i7 v26.0
                            cmu.cpiv.x16 v25.4 i11.0    || sau.exp2 i8 v26.1    || vau.adiff.f16 v10 v27 v0 //dist -> v10
    vau.add.f16 v21 v21 v17 || cmu.cpiv.x16 v25.5 i7.0  || sau.exp2 i9 v26.2
    vau.add.f16 v20 v20 v18 || cmu.cpiv.x16 v25.6 i8.0  || sau.exp2 i10 v26.3
                                cmu.cpiv.x16 v25.7 i9.0 || sau.exp2 i11 v26.4   || vau.mul.f16 v17 v10 v6 //sigmaFactor*dist -> v17

    vau.mul.f16 v18 v25 v28 || sau.exp2 i7  v26.5   || cmu.cpiv.x16 v26.0 i7.0 //weights*dCurrent - > v18
                                sau.exp2 i8 v26.6   || cmu.cpiv.x16 v26.1 i8.0
                                sau.exp2 i9 v26.7   || cmu.cpiv.x16 v26.2 i9.0
                                
    cmu.alignvec v28 v2 v12 6   
    vau.mul.f16 v18 v18 0.5 || cmu.cpiv.x16 v26.3 i10.0 || sau.exp2 i7 v17.0
                            cmu.cpiv.x16 v26.4 i11.0    || sau.exp2 i8 v17.1    || vau.adiff.f16 v10 v28 v0 //dist -> v10
    vau.add.f16 v21 v21 v25 || cmu.cpiv.x16 v26.5 i7.0  || sau.exp2 i9 v17.2
    vau.add.f16 v20 v20 v18 || cmu.cpiv.x16 v26.6 i8.0  || sau.exp2 i10 v17.3
                                cmu.cpiv.x16 v26.7 i9.0 || sau.exp2 i11 v17.4   || vau.mul.f16 v25 v10 v6 //sigmaFactor*dist -> v25

    vau.mul.f16 v18 v26 v19 || sau.exp2 i7  v17.5   || cmu.cpiv.x16 v17.0 i7.0 //weights*dCurrent - > v18
                                sau.exp2 i8 v17.6   || cmu.cpiv.x16 v17.1 i8.0
                                sau.exp2 i9 v17.7   || cmu.cpiv.x16 v17.2 i9.0
                        
    cmu.alignvec v19 v3 v13 6   
    vau.mul.f16 v18 v18 0.5 || cmu.cpiv.x16 v17.3 i10.0 || sau.exp2 i7 v25.0
                            cmu.cpiv.x16 v17.4 i11.0    || sau.exp2 i8 v25.1    || vau.adiff.f16 v10 v19 v0 //dist -> v10
    vau.add.f16 v21 v21 v26 || cmu.cpiv.x16 v17.5 i7.0  || sau.exp2 i9 v25.2
    vau.add.f16 v20 v20 v18 || cmu.cpiv.x16 v17.6 i8.0  || sau.exp2 i10 v25.3
                                cmu.cpiv.x16 v17.7 i9.0 || sau.exp2 i11 v25.4   || vau.mul.f16 v26 v10 v6 //sigmaFactor*dist -> v26

    vau.mul.f16 v18 v17 v27 || sau.exp2 i7  v25.5   || cmu.cpiv.x16 v25.0 i7.0 //weights*dCurrent - > v18
                                sau.exp2 i8 v25.6   || cmu.cpiv.x16 v25.1 i8.0  
                                sau.exp2 i9 v25.7   || cmu.cpiv.x16 v25.2 i9.0
    cmu.alignvec v27 v4 v14 6
    vau.mul.f16 v18 v18 0.5 || cmu.cpiv.x16 v25.3 i10.0 || sau.exp2 i7 v26.0
                            cmu.cpiv.x16 v25.4 i11.0    || sau.exp2 i8 v26.1    || vau.adiff.f16 v10 v27 v0//dist -> v10
    vau.add.f16 v21 v21 v17 || cmu.cpiv.x16 v25.5 i7.0  || sau.exp2 i9 v26.2
    vau.add.f16 v20 v20 v18 || cmu.cpiv.x16 v25.6 i8.0  || sau.exp2 i10 v26.3   
                                cmu.cpiv.x16 v25.7 i9.0 || sau.exp2 i11 v26.4   || vau.mul.f16 v17 v10 v6 //sigmaFactor*dist -> v17

    vau.mul.f16 v18 v25 v28 || sau.exp2 i7  v26.5   || cmu.cpiv.x16 v26.0 i7.0 //weights*dCurrent - > v18
                                sau.exp2 i8 v26.6   || cmu.cpiv.x16 v26.1 i8.0
                                sau.exp2 i9 v26.7   || cmu.cpiv.x16 v26.2 i9.0
    cmu.alignvec v28 v5 v15 6   
    vau.mul.f16 v18 v18 0.5 || cmu.cpiv.x16 v26.3 i10.0 || sau.exp2 i7 v17.0
                            cmu.cpiv.x16 v26.4 i11.0    || sau.exp2 i8 v17.1    || vau.adiff.f16 v10 v28 v0 //dist -> v10
    vau.add.f16 v21 v21 v25 || cmu.cpiv.x16 v26.5 i7.0  || sau.exp2 i9 v17.2
    vau.add.f16 v20 v20 v18 || cmu.cpiv.x16 v26.6 i8.0  || sau.exp2 i10 v17.3
                                cmu.cpiv.x16 v26.7 i9.0 || sau.exp2 i11 v17.4   || vau.mul.f16 v25 v10 v6 //sigmaFactor*dist -> v25

    vau.mul.f16 v18 v26 v19 || sau.exp2 i7  v17.5   || cmu.cpiv.x16 v17.0 i7.0 //weights*dCurrent - > v18
                                sau.exp2 i8 v17.6   || cmu.cpiv.x16 v17.1 i8.0
                                sau.exp2 i9 v17.7   || cmu.cpiv.x16 v17.2 i9.0
    cmu.alignvec v19 v1 v11 8
    vau.mul.f16 v18 v18 0.5 || cmu.cpiv.x16 v17.3 i10.0 || sau.exp2 i7 v25.0
                            cmu.cpiv.x16 v17.4 i11.0    || sau.exp2 i8 v25.1    || vau.adiff.f16 v10 v19 v0 //dist -> v10
    vau.add.f16 v21 v21 v26 || cmu.cpiv.x16 v17.5 i7.0  || sau.exp2 i9 v25.2
    vau.add.f16 v20 v20 v18 || cmu.cpiv.x16 v17.6 i8.0  || sau.exp2 i10 v25.3
                                cmu.cpiv.x16 v17.7 i9.0 || sau.exp2 i11 v25.4   || vau.mul.f16 v26 v10 v6 //sigmaFactor*dist -> v26

    vau.mul.f16 v18 v17 v27 || sau.exp2 i7  v25.5   || cmu.cpiv.x16 v25.0 i7.0 //weights*dCurrent - > v18
                                sau.exp2 i8 v25.6   || cmu.cpiv.x16 v25.1 i8.0
                                sau.exp2 i9 v25.7   || cmu.cpiv.x16 v25.2 i9.0
                                
    cmu.alignvec v27 v2 v12 8
    vau.mul.f16 v18 v18 0.5 || cmu.cpiv.x16 v25.3 i10.0 || sau.exp2 i7 v26.0
                            cmu.cpiv.x16 v25.4 i11.0    || sau.exp2 i8 v26.1    || vau.adiff.f16 v10 v27 v0 //dist -> v10
    vau.add.f16 v21 v21 v17 || cmu.cpiv.x16 v25.5 i7.0  || sau.exp2 i9 v26.2
    vau.add.f16 v20 v20 v18 || cmu.cpiv.x16 v25.6 i8.0  || sau.exp2 i10 v26.3
                                cmu.cpiv.x16 v25.7 i9.0 || sau.exp2 i11 v26.4   || vau.mul.f16 v17 v10 v6 //sigmaFactor*dist -> v17

    vau.mul.f16 v18 v25 v28 || sau.exp2 i7  v26.5   || cmu.cpiv.x16 v26.0 i7.0 //weights*dCurrent - > v18
                                sau.exp2 i8 v26.6   || cmu.cpiv.x16 v26.1 i8.0
                                sau.exp2 i9 v26.7   || cmu.cpiv.x16 v26.2 i9.0
    cmu.alignvec v28 v3 v13 8
    vau.mul.f16 v18 v18 0.5 || cmu.cpiv.x16 v26.3 i10.0 || sau.exp2 i7 v17.0
                            cmu.cpiv.x16 v26.4 i11.0    || sau.exp2 i8 v17.1    || vau.adiff.f16 v10 v28 v0//dist -> v10
    vau.add.f16 v21 v21 v25 || cmu.cpiv.x16 v26.5 i7.0  || sau.exp2 i9 v17.2
    vau.add.f16 v20 v20 v18 || cmu.cpiv.x16 v26.6 i8.0  || sau.exp2 i10 v17.3
                                cmu.cpiv.x16 v26.7 i9.0 || sau.exp2 i11 v17.4   || vau.mul.f16 v25 v10 v6 //sigmaFactor*dist -> v25
    
    vau.mul.f16 v18 v26 v19 || sau.exp2 i7  v17.5   || cmu.cpiv.x16 v17.0 i7.0 //weights*dCurrent - > v18
                                sau.exp2 i8 v17.6   || cmu.cpiv.x16 v17.1 i8.0
                                sau.exp2 i9 v17.7   || cmu.cpiv.x16 v17.2 i9.0
                            
    cmu.alignvec v19 v4 v14 8
    vau.mul.f16 v18 v18 0.5 || cmu.cpiv.x16 v17.3 i10.0 || sau.exp2 i7 v25.0
                            cmu.cpiv.x16 v17.4 i11.0    || sau.exp2 i8 v25.1    || vau.adiff.f16 v10 v19 v0 //dist -> v10
    vau.add.f16 v21 v21 v26 || cmu.cpiv.x16 v17.5 i7.0  || sau.exp2 i9 v25.2
    vau.add.f16 v20 v20 v18 || cmu.cpiv.x16 v17.6 i8.0  || sau.exp2 i10 v25.3
                                cmu.cpiv.x16 v17.7 i9.0 || sau.exp2 i11 v25.4   || vau.mul.f16 v26 v10 v6 //sigmaFactor*dist -> v26

    vau.mul.f16 v18 v17 v27 || sau.exp2 i7  v25.5   || cmu.cpiv.x16 v25.0 i7.0 //weights*dCurrent - > v18
                                sau.exp2 i8 v25.6   || cmu.cpiv.x16 v25.1 i8.0
                                sau.exp2 i9 v25.7   || cmu.cpiv.x16 v25.2 i9.0
                                
    cmu.alignvec v27 v5 v15 8
    vau.mul.f16 v18 v18 0.5 || cmu.cpiv.x16 v25.3 i10.0 || sau.exp2 i7 v26.0
                            cmu.cpiv.x16 v25.4 i11.0    || sau.exp2 i8 v26.1    || vau.adiff.f16 v10 v27 v0 //dist -> v10
    vau.add.f16 v21 v21 v17 || cmu.cpiv.x16 v25.5 i7.0  || sau.exp2 i9 v26.2    || lsu0.ldo.64.l v7 i17 0x10
    vau.add.f16 v20 v20 v18 || cmu.cpiv.x16 v25.6 i8.0  || sau.exp2 i10 v26.3   || lsu0.ldo.64.h v7 i17 0x18
                                cmu.cpiv.x16 v25.7 i9.0 || sau.exp2 i11 v26.4   ||  vau.mul.f16 v17 v10 v6 //sigmaFactor*dist -> v17

    vau.mul.f16 v18 v25 v28 || sau.exp2 i7  v26.5   || cmu.cpiv.x16 v26.0 i7.0 //weights*dCurrent - > v18
                                sau.exp2 i8 v26.6   || cmu.cpiv.x16 v26.1 i8.0
                                sau.exp2 i9 v26.7   || cmu.cpiv.x16 v26.2 i9.0
    vau.mul.f16 v18 v18 0.5 || cmu.cpiv.x16 v26.3 i10.0 || sau.exp2 i7 v17.0    
                            cmu.cpiv.x16 v26.4 i11.0    || sau.exp2 i8 v17.1    || lsu0.ld.64.l v6 i17
                            cmu.cpiv.x16 v26.5 i7.0     || sau.exp2 i9 v17.2    || lsu0.ldo.64.h v6 i17 0x08
    vau.add.f16 v20 v20 v18 || cmu.cpiv.x16 v26.6 i8.0  || sau.exp2 i10 v17.3
    vau.add.f16 v21 v21 v25 || cmu.cpiv.x16 v26.7 i9.0  || sau.exp2 i11 v17.4
    
    vau.mul.f16 v18 v26 v19 || sau.exp2 i7  v17.5   || cmu.cpiv.x16 v17.0 i7.0 //weights*dCurrent - > v18
                                sau.exp2 i8 v17.6   || cmu.cpiv.x16 v17.1 i8.0  || iau.incs i17 0x10
                                sau.exp2 i9 v17.7   || cmu.cpiv.x16 v17.2 i9.0
    vau.mul.f16 v18 v18 0.5 || cmu.cpiv.x16 v17.3 i10.0
    cmu.cpiv.x16 v17.4 i11.0    ||  vau.add.f16 v21 v21 v26
    cmu.cpiv.x16 v17.5 i7.0 || lsu1.ld.64.l v13 i3
    vau.add.f16 v20 v20 v18 || cmu.cpiv.x16 v17.6 i8.0
    cmu.cpiv.x16 v17.7 i9.0 || lsu1.ldo.64.h v13 i3 0x08
    
    cmu.cpvv v3 v13 || iau.incs i3 0x10     || vau.add.f16 v21 v21 v17
    cmu.cpvv v2 v12 || lsu0.ldxvi v12 i2 i6 || vau.mul.f16 v18 v17 v27      //weights*dCurrent - > v18
    cmu.cpvv v1 v11 || lsu1.ldxvi v11 i1 i6
    cmu.cpvv v4 v14 || lsu0.ldxvi v14 i4 i6 || sau.rcp i7 v21.0
    cmu.cpvv v5 v15 || lsu1.ldxvi v15 i5 i6 || sau.rcp i8 v21.1

    cmu.alignvec v6 v6 v7 4
    cmu.clampab.u16 v6 v24 v16
    sau.rcp i9 v21.2    || cmu.cpvv.i16.f16 v13 v13 
    sau.rcp i10 v21.3   || vau.mul.f16 v18 v18 0.5 || cmu.cpvv.i16.f16 v6 v6
    sau.rcp i11 v21.4   
    sau.rcp i7 v21.5    || cmu.cpiv.x16 v21.0 i7.0
    sau.rcp i8 v21.6    || cmu.cpiv.x16 v21.1 i8.0
    sau.rcp i9 v21.7    || cmu.cpiv.x16 v21.2 i9.0
    cmu.cpiv.x16 v21.3 i10.0    || sau.rcp i7 v6.0
    cmu.cpiv.x16 v21.4 i11.0    || sau.rcp i8 v6.1
    cmu.cpiv.x16 v21.5 i7.0 || vau.add.f16 v20 v20 v18  || sau.rcp i9 v6.2
    cmu.cpiv.x16 v21.6 i8.0 || sau.rcp i10 v6.3
    cmu.cpiv.x16 v21.7 i9.0 || sau.rcp i11 v6.4
    vau.mul.f16 v23 v20 v21 || sau.rcp i7 v6.5          || cmu.cpiv.x16 v6.0 i7.0

                                sau.rcp i8 v6.6         || cmu.cpiv.x16 v6.1 i8.0
                                sau.rcp i9 v6.7         || cmu.cpiv.x16 v6.2 i9.0
    vau.mul.f16 v23 v23 2.0  || cmu.cpiv.x16 v6.3 i10.0
    vau.xor v20 v20 v20    || cmu.cpiv.x16 v6.4 i11.0
    cmu.cpiv.x16 v6.5 i7.0
    cmu.cpiv.x16 v6.6 i8.0 || vau.sub.f16 v23 v23 v8
    cmu.cpiv.x16 v6.7 i9.0
    vau.mul.f16 v6 v6 v8   || cmu.alignvec v0 v3 v13 4 

    bilateral5x5_loopBilateral5x5:
    vau.adiff.f16 v10 v1 v0  || cmu.cpvv.f16.i16s v23 v23
    cmu.cpvv.i16.f16 v15 v15
    vau.mul.f16 v6 v6 v9     || lsu1.stxvi v23 i15 i6 || cmu.cpvv.i16.f16 v14 v14
    cmu.cpvv.i16.f16 v11 v11 || vau.xor v21 v21 v21  
    cmu.cpvv.i16.f16 v12 v12    
    nop
    cmu.alignvec v19 v1 v11 2



    iau.sub i3 i3 0x24
    lsu1.ld.32 i0 i3
    nop 5
    iau.sub i15 i15 8
    lsu1.sti.32 i0 i15
     lsu1.sti.32 i14 i15
 
    LSU0.LD.64.h  v28  i19 || IAU.ADD i19 i19 8
    LSU0.LD.64.l  v28  i19 || IAU.ADD i19 i19 8
    LSU0.LD.64.h  v27  i19 || IAU.ADD i19 i19 8
    LSU0.LD.64.l  v27  i19 || IAU.ADD i19 i19 8
    LSU0.LD.64.h  v26  i19 || IAU.ADD i19 i19 8
    LSU0.LD.64.l  v26  i19 || IAU.ADD i19 i19 8
    LSU0.LD.64.h  v25  i19 || IAU.ADD i19 i19 8
    LSU0.LD.64.l  v25  i19 || IAU.ADD i19 i19 8
    LSU0.LD.64.h  v24  i19 || IAU.ADD i19 i19 8
    LSU0.LD.64.l  v24  i19 || IAU.ADD i19 i19 8

    bru.jmp i30
    nop 6
    
.size mvcvBilateral5x5_asm,.-mvcvBilateral5x5_asm
.end
