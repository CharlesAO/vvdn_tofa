///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief apply a convolution 7x7 filter, in the case of symmetric kernel.
///

.type mvcvSsdPointLine7x7U8U32_asm,@function
.version 00.51.04

.code .text.mvcvSsdPointLine7x7U8U32

//void mvcvSsdPointLine7x7U8U32(u8** in1, u8** in2, u32** out, u32 width)
//                                 i18      i17        i16         i15

 // for (i = 0// i < width// i++){
        // sum = 0//
        // for (x = 0// x < 7// x++)
        // {
            // for (y = 0// y < 7// y++)
            // {
                // diff = lines1[x][y - 3] - lines2[x][y - 3]//
				// diff = diff * diff//
                // sum += diff//
            // }         
            // lines2[x]++//
        // }

        // out[0][i] = sum//
    // }

mvcvSsdPointLine7x7U8U32_asm: 

	IAU.SUB i19 i19 8
	LSU0.ST.64.L v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v25  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v25  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v26  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v26  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v27  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v27  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v28  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v28  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v29  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v29  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v30  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v30  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v31  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v31  i19 

	lsu0.ldil i14 ssdPointLine_loop || lsu1.ldih i14 ssdPointLine_loop
	lsu0.ld.32 i0 i18 || iau.add i18 i18 4
	lsu0.ld.32 i1 i18 || iau.add i18 i18 4
	lsu0.ld.32 i2 i18 || iau.add i18 i18 4
	lsu0.ld.32 i3 i18 || iau.add i18 i18 4
	lsu0.ld.32 i4 i18 || iau.add i18 i18 4
	lsu0.ld.32 i5 i18 || iau.add i18 i18 4
	lsu0.ld.32 i6 i18 || iau.shr.u32 i15 i15 3 || lsu1.ld.32 i16 i16

	lsu0.ld.32 i7 i17  || iau.add i17 i17 4  || sau.sub.i32 i0 i0 8
	lsu0.ld.32 i8 i17  || iau.add i17 i17 4  || sau.sub.i32 i1 i1 8
	lsu0.ld.32 i9 i17  || iau.add i17 i17 4  || sau.sub.i32 i2 i2 8
	lsu0.ld.32 i10 i17 || iau.add i17 i17 4  || sau.sub.i32 i3 i3 8
	lsu0.ld.32 i11 i17 || iau.add i17 i17 4  || sau.sub.i32 i4 i4 8
	lsu0.ld.32 i12 i17 || iau.add i17 i17 4  || sau.sub.i32 i5 i5 8
	lsu0.ld.32 i13 i17                       || sau.sub.i32 i6 i6 8
	
	lsu0.ldo.64.l v0 i0 5 || iau.sub i7  i7  8 
	lsu0.ldo.64.l v1 i1 5 || iau.sub i8  i8  8
	lsu0.ldo.64.l v2 i2 5 || iau.sub i9  i9  8
	lsu0.ldo.64.l v3 i3 5 || iau.sub i10 i10 8
	lsu0.ldo.64.l v4 i4 5 || iau.sub i11 i11 8
	lsu0.ldo.64.l v5 i5 5 || iau.sub i12 i12 8
	lsu0.ldo.64.l v6 i6 5 || iau.sub i13 i13 8  
	cmu.cpvv.u8.u16 v0 v0
	cmu.cpvv.u8.u16 v1 v1
	cmu.cpvv.u8.u16 v2 v2
	cmu.cpvv.u8.u16 v3 v3
	cmu.cpvv.u8.u16 v4 v4
	cmu.cpvv.u8.u16 v5 v5
	cmu.cpvv.u8.u16 v6 v6
	
	lsu0.ldi.128.u8.u16 v7 i7 
	lsu0.ldi.128.u8.u16 v8 i7 
	lsu0.ld.128.u8.u16 v9 i7   
	nop 6
	cmu.alignvec v17 v7 v8 10 || iau.sub i7  i7  8 
	vau.sub.i16 v10 v0 v17 ||lsu0.swzv8 [00000000] || cmu.alignvec v17 v7 v8 12 
	vau.sub.i16 v11 v0 v17 ||lsu0.swzv8 [11111111] || cmu.alignvec v17 v7 v8 14
	vau.sub.i16 v12 v0 v17 ||lsu0.swzv8 [22222222] || cmu.alignvec v17 v8 v9 2 
	vau.sub.i16 v13 v0 v8  ||lsu0.swzv8 [33333333] || cmu.alignvec v18 v10 v10 8
	vau.sub.i16 v14 v0 v17 ||lsu0.swzv8 [44444444] || cmu.alignvec v17 v8 v9 4
	vau.sub.i16 v15 v0 v17 ||lsu0.swzv8 [55555555] || cmu.alignvec v17 v8 v9 6 
	vau.sub.i16 v31 v0 v17 ||lsu0.swzv8 [66666666] || cmu.alignvec v19 v11 v11 8

	cmu.alignvec v20 v12 v12 8  
	cmu.alignvec v21 v13 v13 8
	cmu.alignvec v22 v14 v14 8
	cmu.alignvec v23 v15 v15 8
	cmu.alignvec v24 v31 v31 8
	cmu.cpvv.i16.i32 v10 v10
	cmu.cpvv.i16.i32 v18 v18     || vau.mul.i32 v10 v10 v10 || lsu0.ldi.128.u8.u16 v7 i8 
	cmu.cpvv.i16.i32 v11 v11     || vau.mul.i32 v18 v18 v18 || lsu0.ldi.128.u8.u16 v8 i8
	cmu.cpvv.i16.i32 v19 v19     || vau.mul.i32 v11 v11 v11 || lsu0.ld.128.u8.u16 v9 i8 
	cmu.cpvv.i16.i32 v12 v12     || vau.mul.i32 v19 v19 v19

	cmu.cpvv.i16.i32 v20 v20     || vau.mul.i32 v12 v12 v12 || bru.rpl i14 i15
	cmu.cpvv.i16.i32 v13 v13     || vau.mul.i32 v20 v20 v20
	cmu.cpvv.i16.i32 v21 v21     || vau.mul.i32 v13 v13 v13
	cmu.cpvv.i16.i32 v14 v14     || vau.mul.i32 v21 v21 v21
	cmu.cpvv.i16.i32 v22 v22     || vau.mul.i32 v14 v14 v14
	cmu.cpvv.i16.i32 v15 v15     || vau.mul.i32 v22 v22 v22
	cmu.cpvv.i16.i32 v23 v23     || vau.mul.i32 v15 v15 v15
	cmu.cpvv.i16.i32 v16 v31     || vau.mul.i32 v23 v23 v23
	cmu.cpvv.i16.i32 v24 v24     || vau.mul.i32 v16 v16 v16
									vau.mul.i32 v24 v24 v24
	nop
	vau.add.i32 v25 v10 v11 
	vau.add.i32 v27 v12 v13 
	vau.add.i32 v26 v18 v19 
	vau.add.i32 v28 v20 v21 
	vau.add.i32 v25 v14 v25 
	vau.add.i32 v27 v15 v27 || cmu.alignvec v17 v7 v8 10  || iau.sub i8  i8  8 
	vau.sub.i16 v10 v1 v17 || lsu0.swzv8 [00000000] || cmu.alignvec v17 v7 v8 12
	vau.sub.i16 v11 v1 v17 || lsu0.swzv8 [11111111] || cmu.alignvec v17 v7 v8 14
	vau.sub.i16 v12 v1 v17 || lsu0.swzv8 [22222222] || cmu.alignvec v17 v8 v9 2
	vau.sub.i16 v13 v1 v8  || lsu0.swzv8 [33333333] || cmu.alignvec v18 v10 v10 8
	vau.sub.i16 v14 v1 v17 || lsu0.swzv8 [44444444] || cmu.alignvec v17 v8 v9 4
	vau.sub.i16 v15 v1 v17 || lsu0.swzv8 [55555555] || cmu.alignvec v17 v8 v9 6
	vau.sub.i16 v31 v1 v17 || lsu0.swzv8 [66666666] || cmu.alignvec v19 v11 v11 8
	vau.add.i32 v26 v22 v26 || cmu.alignvec v20 v12 v12 8
	vau.add.i32 v28 v23 v28 || cmu.alignvec v21 v13 v13 8
	vau.add.i32 v25 v16 v25 || cmu.alignvec v22 v14 v14 8
	vau.add.i32 v26 v24 v26 || cmu.alignvec v23 v15 v15 8
	vau.add.i32 v29 v27 v25 || cmu.alignvec v24 v31 v31 8
	vau.add.i32 v30 v28 v26 || cmu.cpvv.i16.i32 v10 v10
	cmu.cpvv.i16.i32 v18 v18     || vau.mul.i32 v10 v10 v10 || lsu0.ldi.128.u8.u16 v7 i9
	cmu.cpvv.i16.i32 v11 v11     || vau.mul.i32 v18 v18 v18 || lsu0.ldi.128.u8.u16 v8 i9
	cmu.cpvv.i16.i32 v19 v19     || vau.mul.i32 v11 v11 v11 || lsu0.ld.128.u8.u16 v9 i9
	cmu.cpvv.i16.i32 v12 v12     || vau.mul.i32 v19 v19 v19
	cmu.cpvv.i16.i32 v20 v20     || vau.mul.i32 v12 v12 v12
	cmu.cpvv.i16.i32 v13 v13     || vau.mul.i32 v20 v20 v20
	cmu.cpvv.i16.i32 v21 v21     || vau.mul.i32 v13 v13 v13
	cmu.cpvv.i16.i32 v14 v14     || vau.mul.i32 v21 v21 v21
	cmu.cpvv.i16.i32 v22 v22     || vau.mul.i32 v14 v14 v14
	cmu.cpvv.i16.i32 v15 v15     || vau.mul.i32 v22 v22 v22
	cmu.cpvv.i16.i32 v23 v23     || vau.mul.i32 v15 v15 v15
	cmu.cpvv.i16.i32 v16 v31     || vau.mul.i32 v23 v23 v23
	cmu.cpvv.i16.i32 v24 v24     || vau.mul.i32 v16 v16 v16
									vau.mul.i32 v24 v24 v24
	nop
	vau.add.i32 v25 v10 v11 
	vau.add.i32 v27 v12 v13 
	vau.add.i32 v26 v18 v19 
	vau.add.i32 v28 v20 v21 
	vau.add.i32 v25 v14 v25 
	vau.add.i32 v27 v15 v27 
	vau.add.i32 v26 v22 v26 
	vau.add.i32 v28 v23 v28 || cmu.alignvec v17 v7 v8 10 || iau.sub i9  i9  8 
	vau.sub.i16 v10 v2 v17 || lsu0.swzv8 [00000000] || cmu.alignvec v17 v7 v8 12
	vau.sub.i16 v11 v2 v17 || lsu0.swzv8 [11111111] || cmu.alignvec v17 v7 v8 14
	vau.sub.i16 v12 v2 v17 || lsu0.swzv8 [22222222] || cmu.alignvec v17 v8 v9 2
	vau.sub.i16 v13 v2 v8  || lsu0.swzv8 [33333333] || cmu.alignvec v18 v10 v10 8
	vau.sub.i16 v14 v2 v17 || lsu0.swzv8 [44444444] || cmu.alignvec v17 v8 v9 4
	vau.sub.i16 v15 v2 v17 || lsu0.swzv8 [55555555] || cmu.alignvec v17 v8 v9 6
	vau.sub.i16 v31 v2 v17 || lsu0.swzv8 [66666666] || cmu.alignvec v19 v11 v11 8
	vau.add.i32 v25 v16 v25 || cmu.alignvec v20 v12 v12 8
	vau.add.i32 v26 v24 v26 || cmu.alignvec v21 v13 v13 8
	vau.add.i32 v27 v27 v25 || cmu.alignvec v22 v14 v14 8
	vau.add.i32 v28 v28 v26 || cmu.alignvec v23 v15 v15 8
	vau.add.i32 v29 v29 v27 || cmu.alignvec v24 v31 v31 8
	vau.add.i32 v30 v30 v28 || cmu.cpvv.i16.i32 v10 v10
	cmu.cpvv.i16.i32 v18 v18     || vau.mul.i32 v10 v10 v10 || lsu0.ldi.128.u8.u16 v7 i10
	cmu.cpvv.i16.i32 v11 v11     || vau.mul.i32 v18 v18 v18 || lsu0.ldi.128.u8.u16 v8 i10
	cmu.cpvv.i16.i32 v19 v19     || vau.mul.i32 v11 v11 v11 || lsu0.ld.128.u8.u16 v9 i10 
	cmu.cpvv.i16.i32 v12 v12     || vau.mul.i32 v19 v19 v19
	cmu.cpvv.i16.i32 v20 v20     || vau.mul.i32 v12 v12 v12
	cmu.cpvv.i16.i32 v13 v13     || vau.mul.i32 v20 v20 v20
	cmu.cpvv.i16.i32 v21 v21     || vau.mul.i32 v13 v13 v13
	cmu.cpvv.i16.i32 v14 v14     || vau.mul.i32 v21 v21 v21
	cmu.cpvv.i16.i32 v22 v22     || vau.mul.i32 v14 v14 v14
	cmu.cpvv.i16.i32 v15 v15     || vau.mul.i32 v22 v22 v22
	cmu.cpvv.i16.i32 v23 v23     || vau.mul.i32 v15 v15 v15
	cmu.cpvv.i16.i32 v16 v31     || vau.mul.i32 v23 v23 v23
	cmu.cpvv.i16.i32 v24 v24     || vau.mul.i32 v16 v16 v16
									vau.mul.i32 v24 v24 v24
	nop
	vau.add.i32 v25 v10 v11 
	vau.add.i32 v27 v12 v13 
	vau.add.i32 v26 v18 v19 
	vau.add.i32 v28 v20 v21 
	vau.add.i32 v25 v14 v25 
	vau.add.i32 v27 v15 v27
	vau.add.i32 v26 v22 v26 
	vau.add.i32 v28 v23 v28 || cmu.alignvec v17 v7 v8 10  || iau.sub i10  i10  8 
	vau.sub.i16 v10 v3 v17 || lsu0.swzv8 [00000000] || cmu.alignvec v17 v7 v8 12
	vau.sub.i16 v11 v3 v17 || lsu0.swzv8 [11111111] || cmu.alignvec v17 v7 v8 14
	vau.sub.i16 v12 v3 v17 || lsu0.swzv8 [22222222] || cmu.alignvec v17 v8 v9 2
	vau.sub.i16 v13 v3 v8  || lsu0.swzv8 [33333333] || cmu.alignvec v18 v10 v10 8
	vau.sub.i16 v14 v3 v17 || lsu0.swzv8 [44444444] || cmu.alignvec v17 v8 v9 4
	vau.sub.i16 v15 v3 v17 || lsu0.swzv8 [55555555] || cmu.alignvec v17 v8 v9 6
	vau.sub.i16 v31 v3 v17 || lsu0.swzv8 [66666666] || cmu.alignvec v19 v11 v11 8
	vau.add.i32 v25 v16 v25 || cmu.alignvec v20 v12 v12 8
	vau.add.i32 v26 v24 v26 || cmu.alignvec v21 v13 v13 8
	vau.add.i32 v27 v27 v25 || cmu.alignvec v22 v14 v14 8
	vau.add.i32 v28 v28 v26 || cmu.alignvec v23 v15 v15 8
	vau.add.i32 v29 v29 v27 || cmu.alignvec v24 v31 v31 8
	vau.add.i32 v30 v30 v28 || cmu.cpvv.i16.i32 v10 v10
	cmu.cpvv.i16.i32 v18 v18     || vau.mul.i32 v10 v10 v10 || lsu0.ldi.128.u8.u16 v7 i11 
	cmu.cpvv.i16.i32 v11 v11     || vau.mul.i32 v18 v18 v18 || lsu0.ldi.128.u8.u16 v8 i11 
	cmu.cpvv.i16.i32 v19 v19     || vau.mul.i32 v11 v11 v11 || lsu0.ld.128.u8.u16 v9 i11 
	cmu.cpvv.i16.i32 v12 v12     || vau.mul.i32 v19 v19 v19
	cmu.cpvv.i16.i32 v20 v20     || vau.mul.i32 v12 v12 v12
	cmu.cpvv.i16.i32 v13 v13     || vau.mul.i32 v20 v20 v20
	cmu.cpvv.i16.i32 v21 v21     || vau.mul.i32 v13 v13 v13
	cmu.cpvv.i16.i32 v14 v14     || vau.mul.i32 v21 v21 v21
	cmu.cpvv.i16.i32 v22 v22     || vau.mul.i32 v14 v14 v14
	cmu.cpvv.i16.i32 v15 v15     || vau.mul.i32 v22 v22 v22
	cmu.cpvv.i16.i32 v23 v23     || vau.mul.i32 v15 v15 v15
	cmu.cpvv.i16.i32 v16 v31     || vau.mul.i32 v23 v23 v23
	cmu.cpvv.i16.i32 v24 v24     || vau.mul.i32 v16 v16 v16
									vau.mul.i32 v24 v24 v24
	nop
	vau.add.i32 v25 v10 v11 
	vau.add.i32 v27 v12 v13 
	vau.add.i32 v26 v18 v19 
	vau.add.i32 v28 v20 v21 
	vau.add.i32 v25 v14 v25
	vau.add.i32 v27 v15 v27 	
	vau.add.i32 v26 v22 v26 
	vau.add.i32 v28 v23 v28  || cmu.alignvec v17 v7 v8 10 || iau.sub i11  i11  8
	vau.sub.i16 v10 v4 v17 || lsu0.swzv8 [00000000] || cmu.alignvec v17 v7 v8 12
	vau.sub.i16 v11 v4 v17 || lsu0.swzv8 [11111111] || cmu.alignvec v17 v7 v8 14
	vau.sub.i16 v12 v4 v17 || lsu0.swzv8 [22222222] || cmu.alignvec v17 v8 v9 2
	vau.sub.i16 v13 v4 v8  || lsu0.swzv8 [33333333] || cmu.alignvec v18 v10 v10 8 
	vau.sub.i16 v14 v4 v17 || lsu0.swzv8 [44444444] || cmu.alignvec v17 v8 v9 4
	vau.sub.i16 v15 v4 v17 || lsu0.swzv8 [55555555] || cmu.alignvec v17 v8 v9 6
	vau.sub.i16 v31 v4 v17 || lsu0.swzv8 [66666666] || cmu.alignvec v19 v11 v11 8
	cmu.alignvec v20 v12 v12 8 || vau.add.i32 v25 v16 v25  
	cmu.alignvec v21 v13 v13 8 || vau.add.i32 v26 v24 v26 
	cmu.alignvec v22 v14 v14 8 || vau.add.i32 v27 v27 v25 
	cmu.alignvec v23 v15 v15 8 || vau.add.i32 v28 v28 v26 
	cmu.alignvec v24 v31 v31 8 || vau.add.i32 v29 v29 v27 
	cmu.cpvv.i16.i32 v10 v10   || vau.add.i32 v30 v30 v28 
	cmu.cpvv.i16.i32 v18 v18     || vau.mul.i32 v10 v10 v10 || lsu0.ldi.128.u8.u16 v7 i12 
	cmu.cpvv.i16.i32 v11 v11     || vau.mul.i32 v18 v18 v18 || lsu0.ldi.128.u8.u16 v8 i12 
	cmu.cpvv.i16.i32 v19 v19     || vau.mul.i32 v11 v11 v11 || lsu0.ld.128.u8.u16 v9 i12
	cmu.cpvv.i16.i32 v12 v12     || vau.mul.i32 v19 v19 v19
	cmu.cpvv.i16.i32 v20 v20     || vau.mul.i32 v12 v12 v12
	cmu.cpvv.i16.i32 v13 v13     || vau.mul.i32 v20 v20 v20
	cmu.cpvv.i16.i32 v21 v21     || vau.mul.i32 v13 v13 v13
	cmu.cpvv.i16.i32 v14 v14     || vau.mul.i32 v21 v21 v21
	cmu.cpvv.i16.i32 v22 v22     || vau.mul.i32 v14 v14 v14
	cmu.cpvv.i16.i32 v15 v15     || vau.mul.i32 v22 v22 v22
	cmu.cpvv.i16.i32 v23 v23     || vau.mul.i32 v15 v15 v15
	cmu.cpvv.i16.i32 v16 v31     || vau.mul.i32 v23 v23 v23
	cmu.cpvv.i16.i32 v24 v24     || vau.mul.i32 v16 v16 v16
									vau.mul.i32 v24 v24 v24
	nop
	vau.add.i32 v25 v10 v11 
	vau.add.i32 v27 v12 v13 
	vau.add.i32 v26 v18 v19 
	vau.add.i32 v28 v20 v21 
	vau.add.i32 v25 v14 v25 
	vau.add.i32 v27 v15 v27
	vau.add.i32 v26 v22 v26 
	vau.add.i32 v28 v23 v28  || cmu.alignvec v17 v7 v8 10  || iau.sub i12  i12  8
 	vau.sub.i16 v10 v5 v17 || lsu0.swzv8 [00000000] || cmu.alignvec v17 v7 v8 12
	vau.sub.i16 v11 v5 v17 || lsu0.swzv8 [11111111] || cmu.alignvec v17 v7 v8 14
	vau.sub.i16 v12 v5 v17 || lsu0.swzv8 [22222222] || cmu.alignvec v17 v8 v9 2
	vau.sub.i16 v13 v5 v8  || lsu0.swzv8 [33333333] || cmu.alignvec v18 v10 v10 8
	vau.sub.i16 v14 v5 v17 || lsu0.swzv8 [44444444] || cmu.alignvec v17 v8 v9 4
	vau.sub.i16 v15 v5 v17 || lsu0.swzv8 [55555555] || cmu.alignvec v17 v8 v9 6
	vau.sub.i16 v31 v5 v17 || lsu0.swzv8 [66666666] || cmu.alignvec v19 v11 v11 8
	cmu.alignvec v20 v12 v12 8 || vau.add.i32 v25 v16 v25  
	cmu.alignvec v21 v13 v13 8 || vau.add.i32 v26 v24 v26 
	cmu.alignvec v22 v14 v14 8 || vau.add.i32 v27 v27 v25 
	cmu.alignvec v23 v15 v15 8 || vau.add.i32 v28 v28 v26 
	cmu.alignvec v24 v31 v31 8 || vau.add.i32 v29 v29 v27 
	cmu.cpvv.i16.i32 v10 v10   || vau.add.i32 v30 v30 v28 
	cmu.cpvv.i16.i32 v18 v18     || vau.mul.i32 v10 v10 v10 || lsu0.ldi.128.u8.u16 v7 i13
	cmu.cpvv.i16.i32 v11 v11     || vau.mul.i32 v18 v18 v18 || lsu0.ldi.128.u8.u16 v8 i13
	cmu.cpvv.i16.i32 v19 v19     || vau.mul.i32 v11 v11 v11 || lsu0.ld.128.u8.u16 v9 i13 
	cmu.cpvv.i16.i32 v12 v12     || vau.mul.i32 v19 v19 v19
	cmu.cpvv.i16.i32 v20 v20     || vau.mul.i32 v12 v12 v12
	cmu.cpvv.i16.i32 v13 v13     || vau.mul.i32 v20 v20 v20
	cmu.cpvv.i16.i32 v21 v21     || vau.mul.i32 v13 v13 v13
	cmu.cpvv.i16.i32 v14 v14     || vau.mul.i32 v21 v21 v21
	cmu.cpvv.i16.i32 v22 v22     || vau.mul.i32 v14 v14 v14
	cmu.cpvv.i16.i32 v15 v15     || vau.mul.i32 v22 v22 v22
	cmu.cpvv.i16.i32 v23 v23     || vau.mul.i32 v15 v15 v15
	cmu.cpvv.i16.i32 v16 v31     || vau.mul.i32 v23 v23 v23
	cmu.cpvv.i16.i32 v24 v24     || vau.mul.i32 v16 v16 v16
									vau.mul.i32 v24 v24 v24
	nop
	vau.add.i32 v25 v10 v11 
	vau.add.i32 v27 v12 v13 
	vau.add.i32 v26 v18 v19 
	vau.add.i32 v28 v20 v21 
	vau.add.i32 v25 v14 v25 
	vau.add.i32 v27 v15 v27
	vau.add.i32 v26 v22 v26 
	vau.add.i32 v28 v23 v28 || cmu.alignvec v17 v7 v8 10 || iau.sub i13  i13  8 
	vau.sub.i16 v10 v6 v17 || lsu0.swzv8 [00000000] || cmu.alignvec v17 v7 v8 12
	vau.sub.i16 v11 v6 v17 || lsu0.swzv8 [11111111] || cmu.alignvec v17 v7 v8 14
	vau.sub.i16 v12 v6 v17 || lsu0.swzv8 [22222222] || cmu.alignvec v17 v8 v9 2
	vau.sub.i16 v13 v6 v8  || lsu0.swzv8 [33333333] || cmu.alignvec v18 v10 v10 8 
	vau.sub.i16 v14 v6 v17 || lsu0.swzv8 [44444444] || cmu.alignvec v17 v8 v9 4
	vau.sub.i16 v15 v6 v17 || lsu0.swzv8 [55555555] || cmu.alignvec v17 v8 v9 6
	vau.sub.i16 v31 v6 v17 || lsu0.swzv8 [66666666] || cmu.alignvec v19 v11 v11 8
	cmu.alignvec v20 v12 v12 8 || vau.add.i32 v25 v16 v25 
	cmu.alignvec v21 v13 v13 8 || vau.add.i32 v26 v24 v26 
	cmu.alignvec v22 v14 v14 8 || vau.add.i32 v27 v27 v25 
	cmu.alignvec v23 v15 v15 8 || vau.add.i32 v28 v28 v26 
	cmu.alignvec v24 v31 v31 8 || vau.add.i32 v29 v29 v27 
	cmu.cpvv.i16.i32 v10 v10   || vau.add.i32 v30 v30 v28 
	cmu.cpvv.i16.i32 v18 v18     || vau.mul.i32 v10 v10 v10 || lsu0.ldi.128.u8.u16 v7 i7
	cmu.cpvv.i16.i32 v11 v11     || vau.mul.i32 v18 v18 v18 || lsu0.ldi.128.u8.u16 v8 i7
	cmu.cpvv.i16.i32 v19 v19     || vau.mul.i32 v11 v11 v11 || lsu0.ld.128.u8.u16 v9 i7 
	cmu.cpvv.i16.i32 v12 v12     || vau.mul.i32 v19 v19 v19
	cmu.cpvv.i16.i32 v20 v20     || vau.mul.i32 v12 v12 v12
	cmu.cpvv.i16.i32 v13 v13     || vau.mul.i32 v20 v20 v20
	cmu.cpvv.i16.i32 v21 v21     || vau.mul.i32 v13 v13 v13
	cmu.cpvv.i16.i32 v14 v14     || vau.mul.i32 v21 v21 v21
	cmu.cpvv.i16.i32 v22 v22     || vau.mul.i32 v14 v14 v14
	cmu.cpvv.i16.i32 v15 v15     || vau.mul.i32 v22 v22 v22
	cmu.cpvv.i16.i32 v23 v23     || vau.mul.i32 v15 v15 v15
	cmu.cpvv.i16.i32 v16 v31     || vau.mul.i32 v23 v23 v23
	cmu.cpvv.i16.i32 v24 v24     || vau.mul.i32 v16 v16 v16
									vau.mul.i32 v24 v24 v24
	nop
	vau.add.i32 v25 v10 v11  
	vau.add.i32 v27 v12 v13  
	vau.add.i32 v26 v18 v19  
	vau.add.i32 v28 v20 v21 
	vau.add.i32 v25 v14 v25
	vau.add.i32 v27 v15 v27 
	vau.add.i32 v26 v22 v26 
	vau.add.i32 v28 v23 v28 || cmu.alignvec v17 v7 v8 10 || iau.sub i7  i7  8 
	vau.sub.i16 v10 v0 v17 || lsu0.swzv8 [00000000] || cmu.alignvec v17 v7 v8 12   
	vau.sub.i16 v11 v0 v17 || lsu0.swzv8 [11111111] || cmu.alignvec v17 v7 v8 14   
	vau.sub.i16 v12 v0 v17 || lsu0.swzv8 [22222222] || cmu.alignvec v17 v8 v9 2    
	vau.sub.i16 v13 v0 v8  || lsu0.swzv8 [33333333] || cmu.alignvec v18 v10 v10 8  
	vau.sub.i16 v14 v0 v17 || lsu0.swzv8 [44444444] || cmu.alignvec v17 v8 v9 4 
	vau.sub.i16 v15 v0 v17 || lsu0.swzv8 [55555555] || cmu.alignvec v17 v8 v9 6 
	vau.sub.i16 v31 v0 v17 || lsu0.swzv8 [66666666] || cmu.alignvec v19 v11 v11 8
	vau.add.i32 v25 v16 v25 || cmu.alignvec v20 v12 v12 8
	vau.add.i32 v26 v24 v26 || cmu.alignvec v21 v13 v13 8
	vau.add.i32 v27 v27 v25 || cmu.alignvec v22 v14 v14 8
	ssdPointLine_loop:
	.nowarn 10
	vau.add.i32 v28 v28 v26 || cmu.alignvec v23 v15 v15 8
	.nowarnend
	vau.add.i32 v29 v29 v27 || cmu.alignvec v24 v31 v31 8
	vau.add.i32 v30 v30 v28 || cmu.cpvv.i16.i32 v10 v10	
	cmu.cpvv.i16.i32 v18 v18 || vau.mul.i32 v10 v10 v10 || lsu0.ldi.128.u8.u16 v7 i8 || lsu1.sto.64.l v29 i16 0    
	cmu.cpvv.i16.i32 v11 v11 || vau.mul.i32 v18 v18 v18 || lsu0.ldi.128.u8.u16 v8 i8 || lsu1.sto.64.h v29 i16 8 
	cmu.cpvv.i16.i32 v19 v19 || vau.mul.i32 v11 v11 v11 || lsu0.ld.128.u8.u16 v9 i8  || lsu1.sto.64.l v30 i16 16
	cmu.cpvv.i16.i32 v12 v12 || vau.mul.i32 v19 v19 v19                              || lsu1.sto.64.h v30 i16 24 || iau.add i16 i16 32
	

	LSU0.LD.64.H  v31  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v31  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v30  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v30  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v29  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v29  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v28  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v28  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v27  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v27  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v26  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v26  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v25  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v25  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v24  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v24  i19 || IAU.ADD i19 i19 8	  
	
	BRU.jmp i30
	nop 6

.size mvcvSsdPointLine7x7U8U32_asm,.-mvcvSsdPointLine7x7U8U32_asm
.end
 
 
