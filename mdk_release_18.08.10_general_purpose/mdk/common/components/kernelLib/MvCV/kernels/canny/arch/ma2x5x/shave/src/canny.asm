///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief   canny edge detection 
///

.type mvcvCanny_asm,@function
.version 00.51.05
//-------------------------------------------------------------------------------
.data .rodata.canny
.align 16

 canny_blur:
		.float16    0.012578, 0.025157, 0.031446, 0.056603, 0.075471, 0.094339
 canny_clampVal:
        .float16 255.0
 canny_tangent:
		.float16 0.414213, 2.414213, -2.414213, -0.414213


.code .text.canny
mvcvCanny_asm:
.lalign
//void canny(u8** srcAddr, u8** dstAddr, u8 ** interim,  u8    threshold1, u8    threshold2, u32 width)//
//                i18          i17             i16               i15              i14             i13

	
	IAU.SUB i19 i19 4
	LSU0.ST.32  i20  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i21  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i22  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i23  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i24  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i25  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i26  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i27  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i28  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i29  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i31  i19 
	
	IAU.SUB i19 i19 8
	LSU0.ST.64.l v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v25  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v25  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v26  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v26  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v27  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v27  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v28  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v28  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v29  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v29  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v30  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v30  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v31  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v31  i19  
	
	LSU0.ldil i0, canny_blur || LSU1.ldih i0, canny_blur
	LSU0.LD.64.l  v0  i0 || IAU.ADD i0 i0 8    
	LSU0.LD.64.h  v0  i0 
	LSU0.ldil i0, canny_clampVal || LSU1.ldih i0, canny_clampVal
	lsu0.ld.16r v31, i0
	
	
	LSU0.LD.32  i0  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i1  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i2  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i3  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i4  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i5  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i6  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i7  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i8  i18 || IAU.ADD i18 i18 4
	LSU0.LD.32  i17 i17 
	


IAU.ADD i20 i16 i13
IAU.ADD i20 i20 8
IAU.ADD i21 i20 i13
IAU.ADD i21 i21 8
IAU.ADD i22 i21 i13
IAU.ADD i22 i22 8
IAU.ADD i23 i22 i13
IAU.ADD i23 i23 8



	LSU0.ldil i24, canny_blurloop || LSU1.ldih i24, canny_blurloop
	IAU.SHR.u32 i25 i13 3
	IAU.ADD i25 i25 1
	
	IAU.SUB i0 i0 8
	IAU.SUB i1 i1 8
	IAU.SUB i2 i2 8
	IAU.SUB i3 i3 8
	IAU.SUB i4 i4 8
	IAU.SUB i5 i5 8
	IAU.SUB i6 i6 8
	IAU.SUB i7 i7 8
	IAU.SUB i8 i8 8
	
	LSU0.LD.128.U8.F16   v1   i0 || IAU.ADD i0 i0 8 
	LSU0.LD.128.U8.F16   v2   i0 
	LSU0.LD.128.U8.F16   v3   i1 || IAU.ADD i1 i1 8
	LSU0.LD.128.U8.F16   v4   i1 
	LSU0.LD.128.U8.F16   v5   i2 || IAU.ADD i2 i2 8
	LSU0.LD.128.U8.F16   v6   i2 					
	LSU0.LD.128.U8.F16   v7   i3 || IAU.ADD i3 i3 8 
	LSU0.LD.128.U8.F16   v8   i3                    	
//first line	
	LSU0.LD.128.U8.F16   v9   i4 || IAU.ADD i4 i4 8 || cmu.alignvec v11,  v1,    v2,    8	|| bru.rpl i24 i25
	LSU0.LD.128.U8.F16  v10   i4                    || cmu.alignvec v12,  v1,    v2,   10   ||	VAU.MACPZ.f16      v0  v11  || lsu1.swzv8 [00000000] 
	                                                   cmu.alignvec v13,  v1,    v2,   12   ||  VAU.MACP.f16       v0  v12  || LSU1.SWZV8 [11111111] 	 
	                                                   cmu.alignvec v14,  v1,    v2,   14	||  VAU.MACP.f16       v0  v13  || LSU1.SWZV8 [22222222] 	 
	                                                   cmu.alignvec v15,  v3,    v4,    8	||  VAU.MACP.f16       v0  v14  || LSU1.SWZV8 [11111111] 	 
	                                                   cmu.alignvec v16,  v3,    v4,   10	||  VAU.MACP.f16       v0   v2  || LSU1.SWZV8 [00000000] 	 
													   cmu.alignvec v17,  v3,    v4,   12	||  VAU.MACP.f16       v0  v15  || LSU1.SWZV8 [11111111] 	 
	                                                   cmu.alignvec v18,  v3,    v4,   14	||  VAU.MACP.f16       v0  v16  || LSU1.SWZV8 [33333333] 	 
	                                                   cmu.alignvec v19,  v5,    v6,    8   ||  VAU.MACP.f16       v0  v17  || LSU1.SWZV8 [44444444] 	 
	                                                   cmu.alignvec v20,  v5,    v6,   10   ||  VAU.MACP.f16       v0  v18  || LSU1.SWZV8 [33333333] 	 
	                                                   cmu.alignvec v21,  v5,    v6,   12   ||  VAU.MACP.f16       v0   v4  || LSU1.SWZV8 [11111111] 	 
	                                                   cmu.alignvec v22,  v5,    v6,   14   ||  VAU.MACP.f16       v0  v19  || LSU1.SWZV8 [22222222] 	 
													   cmu.alignvec v23,  v7,    v8,    8	||  VAU.MACP.f16       v0  v20  || LSU1.SWZV8 [44444444] 	 
													   cmu.alignvec v24,  v7,    v8,   10   ||  VAU.MACP.f16       v0  v21  || LSU1.SWZV8 [55555555] 	 
													   cmu.alignvec v25,  v7,    v8,   12   ||  VAU.MACP.f16       v0  v22  || LSU1.SWZV8 [44444444] 	 
													   cmu.alignvec v26,  v7,    v8,   14	||  VAU.MACP.f16       v0   v6  || LSU1.SWZV8 [22222222] 	 
                                                       cmu.alignvec v27,  v9,   v10,    8   ||  VAU.MACP.f16       v0  v23  || LSU1.SWZV8 [11111111] 	
                                                       cmu.alignvec v28,  v9,   v10,   10   ||  VAU.MACP.f16       v0  v24  || LSU1.SWZV8 [33333333] 	
                                                       cmu.alignvec v29,  v9,   v10,   12   ||  VAU.MACP.f16       v0  v25  || LSU1.SWZV8 [44444444] 	
                                                       cmu.alignvec v30,  v9,   v10,   14   ||  VAU.MACP.f16       v0  v26  || LSU1.SWZV8 [33333333] 	
                                                                                                VAU.MACP.f16       v0   v8  || LSU1.SWZV8 [11111111] 	
                                                                                                VAU.MACP.f16       v0  v27  || LSU1.SWZV8 [00000000] 	
                                                                                                VAU.MACP.f16       v0  v28  || LSU1.SWZV8 [11111111] 	
                                                                                                VAU.MACP.f16       v0  v29  || LSU1.SWZV8 [22222222] 	
                                                                                                VAU.MACP.f16       v0  v30  || LSU1.SWZV8 [11111111] || LSU0.LD.128.U8.F16   v1   i5 || IAU.ADD i5 i5 8	
                                                                                                VAU.MACPW.f16 v11  v0  v10  || LSU1.SWZV8 [00000000] || LSU0.LD.128.U8.F16   v2   i5	
	nop 9
	CMU.CLAMP0.f16 v11 v11 v31
	nop
	LSU0.ST.128.f16.u8 v11 i16 || IAU.ADD i16 i16 8
	//second line
	cmu.alignvec v11,  v1,    v2,    8
	cmu.alignvec v12,  v1,    v2,   10
        cmu.alignvec v13,  v1,    v2,   12	
	cmu.alignvec v14,  v1,    v2,   14
	VAU.MACPZ.f16      v0  v15  || lsu1.swzv8 [00000000]
	VAU.MACP.f16       v0  v16  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v17  || LSU1.SWZV8 [22222222]
	VAU.MACP.f16       v0  v18  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0   v4  || LSU1.SWZV8 [00000000]
	VAU.MACP.f16       v0  v19  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v20  || LSU1.SWZV8 [33333333]
	VAU.MACP.f16       v0  v21  || LSU1.SWZV8 [44444444]
	VAU.MACP.f16       v0  v22  || LSU1.SWZV8 [33333333]
	VAU.MACP.f16       v0   v6  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v23  || LSU1.SWZV8 [22222222]
	VAU.MACP.f16       v0  v24  || LSU1.SWZV8 [44444444]
	VAU.MACP.f16       v0  v25  || LSU1.SWZV8 [55555555]
	VAU.MACP.f16       v0  v26  || LSU1.SWZV8 [44444444]
	VAU.MACP.f16       v0   v8  || LSU1.SWZV8 [22222222]
	VAU.MACP.f16       v0  v27  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v28  || LSU1.SWZV8 [33333333]
	VAU.MACP.f16       v0  v29  || LSU1.SWZV8 [44444444]
	VAU.MACP.f16       v0  v30  || LSU1.SWZV8 [33333333]
	VAU.MACP.f16       v0  v10  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v11  || LSU1.SWZV8 [00000000]
	VAU.MACP.f16       v0  v12  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v13  || LSU1.SWZV8 [22222222]
	VAU.MACP.f16       v0  v14  || LSU1.SWZV8 [11111111] || LSU0.LD.128.U8.F16   v3   i6 || IAU.ADD i6 i6 8
	VAU.MACPW.f16 v15  v0   v2  || LSU1.SWZV8 [00000000] || LSU0.LD.128.U8.F16   v4   i6
	nop 9
	CMU.CLAMP0.f16 v15 v15 v31
	nop
	LSU0.ST.128.f16.u8 v15 i20 || IAU.ADD i20 i20 8	
	//third line
	cmu.alignvec v15,  v3,    v4,    8
	cmu.alignvec v16,  v3,    v4,   10
        cmu.alignvec v17,  v3,    v4,   12
	cmu.alignvec v18,  v3,    v4,   14
	VAU.MACPZ.f16      v0  v19  || lsu1.swzv8 [00000000]
	VAU.MACP.f16       v0  v20  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v21  || LSU1.SWZV8 [22222222]
	VAU.MACP.f16       v0  v22  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0   v6  || LSU1.SWZV8 [00000000]
	VAU.MACP.f16       v0  v23  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v24  || LSU1.SWZV8 [33333333]
	VAU.MACP.f16       v0  v25  || LSU1.SWZV8 [44444444]
	VAU.MACP.f16       v0  v26  || LSU1.SWZV8 [33333333]
	VAU.MACP.f16       v0   v8  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v27  || LSU1.SWZV8 [22222222]
	VAU.MACP.f16       v0  v28  || LSU1.SWZV8 [44444444]
	VAU.MACP.f16       v0  v29  || LSU1.SWZV8 [55555555]
	VAU.MACP.f16       v0  v30  || LSU1.SWZV8 [44444444]
	VAU.MACP.f16       v0  v10  || LSU1.SWZV8 [22222222]
	VAU.MACP.f16       v0  v11  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v12  || LSU1.SWZV8 [33333333]
	VAU.MACP.f16       v0  v13  || LSU1.SWZV8 [44444444]
	VAU.MACP.f16       v0  v14  || LSU1.SWZV8 [33333333]
	VAU.MACP.f16       v0   v2  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v15  || LSU1.SWZV8 [00000000]
	VAU.MACP.f16       v0  v16  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v17  || LSU1.SWZV8 [22222222]
	VAU.MACP.f16       v0  v18  || LSU1.SWZV8 [11111111] || LSU0.LD.128.U8.F16   v5   i7 || IAU.ADD i7 i7 8
	VAU.MACPW.f16 v19  v0   v4  || LSU1.SWZV8 [00000000] || LSU0.LD.128.U8.F16   v6   i7
	nop 9
	CMU.CLAMP0.f16 v19 v19 v31
	nop
	LSU0.ST.128.f16.u8 v19 i21 || IAU.ADD i21 i21 8
//fourth line
	cmu.alignvec v19,  v5,    v6,    8
	cmu.alignvec v20,  v5,    v6,   10
        cmu.alignvec v21,  v5,    v6,   12
	cmu.alignvec v22,  v5,    v6,   14
	VAU.MACPZ.f16      v0  v23  || lsu1.swzv8 [00000000]
	VAU.MACP.f16       v0  v24  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v25  || LSU1.SWZV8 [22222222]
	VAU.MACP.f16       v0  v26  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0   v8  || LSU1.SWZV8 [00000000]
	VAU.MACP.f16       v0  v27  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v28  || LSU1.SWZV8 [33333333]
	VAU.MACP.f16       v0  v29  || LSU1.SWZV8 [44444444]
	VAU.MACP.f16       v0  v30  || LSU1.SWZV8 [33333333]
	VAU.MACP.f16       v0  v10  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v11  || LSU1.SWZV8 [22222222]
	VAU.MACP.f16       v0  v12  || LSU1.SWZV8 [44444444]
	VAU.MACP.f16       v0  v13  || LSU1.SWZV8 [55555555]
	VAU.MACP.f16       v0  v14  || LSU1.SWZV8 [44444444]
	VAU.MACP.f16       v0   v2  || LSU1.SWZV8 [22222222]
	VAU.MACP.f16       v0  v15  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v16  || LSU1.SWZV8 [33333333]
	VAU.MACP.f16       v0  v17  || LSU1.SWZV8 [44444444]
	VAU.MACP.f16       v0  v18  || LSU1.SWZV8 [33333333]
	VAU.MACP.f16       v0   v4  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v19  || LSU1.SWZV8 [00000000]
	VAU.MACP.f16       v0  v20  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v21  || LSU1.SWZV8 [22222222]
	VAU.MACP.f16       v0  v22  || LSU1.SWZV8 [11111111] || LSU0.LD.128.U8.F16   v7   i8 || IAU.ADD i8 i8 8
	VAU.MACPW.f16 v23  v0   v6  || LSU1.SWZV8 [00000000] || LSU0.LD.128.U8.F16   v8   i8
	nop 9
	CMU.CLAMP0.f16 v23 v23 v31
	nop
	LSU0.ST.128.f16.u8 v23 i22 || IAU.ADD i22 i22 8
	//fifth line
	cmu.alignvec v23,  v7,    v8,    8
	cmu.alignvec v24,  v7,    v8,   10
        cmu.alignvec v25,  v7,    v8,   12
	cmu.alignvec v26,  v7,    v8,   14
	VAU.MACPZ.f16      v0  v27  || lsu1.swzv8 [00000000]
	VAU.MACP.f16       v0  v28  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v29  || LSU1.SWZV8 [22222222]
	VAU.MACP.f16       v0  v30  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v10  || LSU1.SWZV8 [00000000]
	VAU.MACP.f16       v0  v11  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v12  || LSU1.SWZV8 [33333333]
	VAU.MACP.f16       v0  v13  || LSU1.SWZV8 [44444444]
	VAU.MACP.f16       v0  v14  || LSU1.SWZV8 [33333333]
	VAU.MACP.f16       v0   v2  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v15  || LSU1.SWZV8 [22222222]
	VAU.MACP.f16       v0  v16  || LSU1.SWZV8 [44444444]
	VAU.MACP.f16       v0  v17  || LSU1.SWZV8 [55555555]
	VAU.MACP.f16       v0  v18  || LSU1.SWZV8 [44444444]
	VAU.MACP.f16       v0   v4  || LSU1.SWZV8 [22222222]
	VAU.MACP.f16       v0  v19  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v20  || LSU1.SWZV8 [33333333]
	VAU.MACP.f16       v0  v21  || LSU1.SWZV8 [44444444]
	VAU.MACP.f16       v0  v22  || LSU1.SWZV8 [33333333]
	VAU.MACP.f16       v0   v6  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v23  || LSU1.SWZV8 [00000000]
	VAU.MACP.f16       v0  v24  || LSU1.SWZV8 [11111111]
	VAU.MACP.f16       v0  v25  || LSU1.SWZV8 [22222222]
	VAU.MACP.f16       v0  v26  || LSU1.SWZV8 [11111111]
	VAU.MACPW.f16 v27  v0   v8  || LSU1.SWZV8 [00000000]
	LSU0.LD.128.U8.F16   v1   i0 || IAU.ADD i0 i0 8 
	LSU0.LD.128.U8.F16   v2   i0 
	LSU0.LD.128.U8.F16   v3   i1 || IAU.ADD i1 i1 8
	LSU0.LD.128.U8.F16   v4   i1 
	LSU0.LD.128.U8.F16   v5   i2 || IAU.ADD i2 i2 8
	LSU0.LD.128.U8.F16   v6   i2 				
 canny_blurloop:
 .nowarn 10
	LSU0.LD.128.U8.F16   v7   i3 || IAU.ADD i3 i3 8 
.nowarnend
	LSU0.LD.128.U8.F16   v8   i3                  
	nop
	CMU.CLAMP0.f16 v27 v27 v31
	nop
	LSU0.ST.128.f16.u8 v27 i23 || IAU.ADD i23 i23 8
	nop		

//sobel step	
	IAU.SUB i16 i16 i13
	IAU.SUB i16 i16 8
	IAU.SUB i20 i20 i13
	IAU.SUB i20 i20 8
	IAU.SUB i21 i21 i13
	IAU.SUB i21 i21 8
	IAU.SUB i22 i22 i13
	IAU.SUB i22 i22 8
	IAU.SUB i23 i23 i13
	IAU.SUB i23 i23 8



	IAU.ADD i1 i16 i13
	IAU.ADD i1  i1 i13
	IAU.ADD i1  i1 i13
	IAU.ADD i1  i1 i13
	IAU.ADD i1  i1 i13
	IAU.ADD i1  i1 0x28
	IAU.ADD i2  i1 i13
	IAU.ADD i2  i2 8
	IAU.ADD i3  i2 i13
	IAU.ADD i3  i3 8
	IAU.ADD i29  i3 i13
	IAU.ADD i29  i29 8
	IAU.ADD i4  i29 i13
	IAU.ADD i4  i4 8

	
	LSU0.ldil i24, canny_sobelloop || LSU1.ldih i24, canny_sobelloop
	
	LSU0.LD.128.U8.F16   v1   i16 || IAU.ADD i16 i16 8 
	LSU0.LD.128.U8.F16   v2   i16 
	LSU0.LD.128.U8.F16   v3   i20 || IAU.ADD i20 i20 8 || VAU.XOR v21 v21 v21
	LSU0.LD.128.U8.F16   v4   i20                      || VAU.XOR v22 v22 v22
	LSU0.LD.128.U8.F16   v5   i21 || IAU.ADD i21 i21 8 || VAU.XOR v23 v23 v23
	LSU0.LD.128.U8.F16   v6   i21                      || VAU.XOR v24 v24 v24
	LSU0.LD.128.U8.F16   v7   i22 || IAU.ADD i22 i22 8 || VAU.XOR v25 v25 v25
	LSU0.LD.128.U8.F16   v8   i22                      || VAU.XOR v26 v26 v26
	LSU0.LD.128.U8.F16   v9   i23 || IAU.ADD i23 i23 8  
	LSU0.LD.128.U8.F16  v10   i23 
	cmu.alignvec v11,  v1,    v2,    2 
	cmu.alignvec v12,  v1,    v2,    4  
	cmu.alignvec v13,  v3,    v4,    2 
	cmu.alignvec v14,  v3,    v4,    4 || bru.rpl i24 i25 
	cmu.alignvec v15,  v5,    v6,    2 
	cmu.alignvec v16,  v5,    v6,    4  
	cmu.alignvec v17,  v7,    v8,    2 
	cmu.alignvec v18,  v7,    v8,    4  
	cmu.alignvec v19,  v9,    v10,    2 
	cmu.alignvec v20,  v9,    v10,    4  
	
// Gx	
VAU.add.f16 v21 v21 v12
VAU.add.f16 v22 v22 v14
VAU.add.f16 v23 v23 v16
VAU.SUB.f16 v21 v21 v1
VAU.SUB.f16 v22 v22 v3
VAU.SUB.f16 v23 v23 v5
VAU.add.f16 v21 v21 v14
VAU.add.f16 v22 v22 v16
VAU.add.f16 v23 v23 v18
VAU.add.f16 v21 v21 v14
VAU.add.f16 v22 v22 v16
VAU.add.f16 v23 v23 v18
VAU.SUB.f16 v21 v21 v3	
VAU.SUB.f16 v22 v22 v5
VAU.SUB.f16 v23 v23 v7
VAU.SUB.f16 v21 v21 v3	
VAU.SUB.f16 v22 v22 v5
VAU.SUB.f16 v23 v23 v7
VAU.ADD.f16 v21 v21 v16
VAU.ADD.f16 v22 v22 v18
VAU.ADD.f16 v23 v23 v20
VAU.SUB.f16 v21 v21 v5
VAU.SUB.f16 v22 v22 v7	
VAU.SUB.f16 v23 v23 v9

VAU.mul.f16 v27 v21 v21  
VAU.mul.f16 v28 v22 v22 || CMU.CPVI.x16  i5.l v22.0
VAU.mul.f16 v29 v23 v23 || CMU.CPVI.x16  i5.h v22.1
//Gy
VAU.ADD.f16 v24 v24 v1 || CMU.CPVI.x16  i6.l v22.2
VAU.ADD.f16 v25 v25 v3 || CMU.CPVI.x16  i6.h v22.3
VAU.ADD.f16 v26 v26 v5 || CMU.CPVI.x16  i7.l v22.4
VAU.sub.f16 v24 v24 v5 || CMU.CPVI.x16  i7.h v22.5
VAU.sub.f16 v25 v25 v7 || CMU.CPVI.x16  i8.l v22.6
VAU.sub.f16 v26 v26 v9 || CMU.CPVI.x16  i8.h v22.7
VAU.ADD.f16 v24 v24 v11
VAU.ADD.f16 v25 v25 v13
VAU.ADD.f16 v26 v26 v15
VAU.ADD.f16 v24 v24 v11
VAU.ADD.f16 v25 v25 v13
VAU.ADD.f16 v26 v26 v15
VAU.SUB.f16 v24 v24 v15
VAU.SUB.f16 v25 v25 v17
VAU.SUB.f16 v26 v26 v19
VAU.SUB.f16 v24 v24 v15
VAU.SUB.f16 v25 v25 v17
VAU.SUB.f16 v26 v26 v19
VAU.ADD.f16 v24 v24 v12
VAU.ADD.f16 v25 v25 v14
VAU.ADD.f16 v26 v26 v16
VAU.SUB.f16 v24 v24 v16
VAU.SUB.f16 v25 v25 v18
VAU.SUB.f16 v26 v26 v20

VAU.mul.f16 v15 v24 v24 
VAU.mul.f16 v16 v25 v25  || CMU.CPVI.x16  i9.l v25.0
VAU.mul.f16 v17 v26 v26  || CMU.CPVI.x16  i9.h v25.1
CMU.CPVI.x16  i10.l v25.2 || VAU.ADD.f16  v27 v27 v15 || SAU.DIV.f16 i9 i9 i5
nop
CMU.CPVI.x16  i10.h v25.3 || VAU.ADD.f16  v28 v28 v16
CMU.CPVI.x16  i11.l v25.4 || VAU.ADD.f16  v29 v29 v17 || SAU.DIV.f16 i10 i10 i6
nop
CMU.CPVI.x16  i11.h v25.5 || SAU.SQT i5 v27.0
CMU.CPVI.x16  i12.l v25.6 || SAU.DIV.f16 i11 i11 i7
nop
CMU.CPVI.x16  i12.h v25.7 || SAU.SQT i6 v27.1
SAU.DIV.f16 i12 i12 i8  
nop 3
SAU.SQT i7 v27.2
SAU.SQT i8 v27.3  || CMU.CPIV.x32  v22.0  i9
SAU.SQT i9 v27.4  ||  IAU.SHL i6 i6 16 || lsu0.ldih i5 0x0
CMU.CPIV.x32  v22.1  i10 || IAU.OR i5 i6 i5 
SAU.SQT i10 v27.5 || CMU.CPIV.x32  v27.0  i5 
CMU.CPIV.x32  v22.2  i11
SAU.SQT i11 v27.6 ||  IAU.SHL i8 i8 16 || lsu0.ldih i7 0x0
CMU.CPIV.x32  v22.3  i12 || IAU.OR i7 i8 i7 
SAU.SQT i12 v27.7 || CMU.CPIV.x32  v27.1  i7   || LSU0.LD.128.U8.F16   v1   i16 || IAU.ADD i16 i16 8 
SAU.SQT  i5 v28.0                              	 || LSU0.LD.128.U8.F16   v2   i16 ||  IAU.SHL i10 i10 16 || lsu1.ldih i9 0x0
SAU.SQT  i6 v28.1                                || LSU0.LD.128.U8.F16   v3   i20 || IAU.ADD i20 i20 8 || VAU.XOR v21 v21 v21
SAU.SQT  i7 v28.2                                || LSU0.LD.128.U8.F16   v4   i20 || IAU.OR i9 i9 i10                     
SAU.SQT  i8 v28.3  || CMU.CPIV.x32  v27.2  i9    || LSU0.LD.128.U8.F16   v5   i21 || IAU.ADD i21 i21 8 || VAU.XOR v23 v23 v23
SAU.SQT  i9  v28.4                               || LSU0.LD.128.U8.F16   v6   i21                      || VAU.XOR v24 v24 v24	||  IAU.SHL i12 i12 16 || lsu1.ldih i11 0x0
SAU.SQT  i10 v28.5                               || LSU0.LD.128.U8.F16   v7   i22 || IAU.ADD i22 i22 8 || VAU.XOR v25 v25 v25	
SAU.SQT  i11 v28.6                               || LSU0.LD.128.U8.F16   v8   i22                      || VAU.XOR v26 v26 v26	|| IAU.OR i12 i11 i12
SAU.SQT  i12 v28.7                               || LSU0.LD.128.U8.F16   v9   i23 || IAU.ADD i23 i23 8  || lsu1.cp v27.3 i12
SAU.SQT   i5 v29.0                               || LSU0.LD.128.U8.F16  v10   i23 ||  IAU.SHL i6 i6 16 || lsu1.ldih i5 0x0
SAU.SQT   i6 v29.1                               || IAU.OR i5 i6 i5 
SAU.SQT   i7 v29.2                                || cmu.cpiv.x32  v28.0  i5 ||  IAU.SHL i8 i8 16 || lsu1.ldih i7 0x0
SAU.SQT   i8 v29.3                                || IAU.OR i7 i8 i7 
SAU.SQT   i9 v29.4                                || cmu.cpiv.x32  v28.1  i7	||  IAU.SHL i10 i10 16 || lsu1.ldih i9 0x0
SAU.SQT   i10 v29.5                               || IAU.OR i9 i9 i10   
SAU.SQT   i11 v29.6                               || lsu1.cp  v28.2  i9 ||  IAU.SHL i12 i12 16 || lsu0.ldih i11 0x0
SAU.SQT   i12 v29.7                               || IAU.OR i12 i11 i12	
                                cmu.cpiv.x32  v28.3  i12 ||  IAU.SHL i6 i6 16 || lsu1.ldih i5 0x0
                               IAU.OR i5 i6 i5 
                               cmu.cpiv.x32  v29.0  i5 ||  IAU.SHL i8 i8 16 || lsu1.ldih i7 0x0
IAU.OR i7 i8 i7 

cmu.cpiv.x32  v29.1  i7 ||  IAU.SHL i10 i10 16 || lsu1.ldih i9 0x0

CMU.CLAMP0.f16 v27 v27 v31 || LSU0.ST.64.L v22 i4  || IAU.ADD i4 i4 8 || SAU.OR i9 i9 i10   
CMU.CLAMP0.f16 v28 v28 v31 || LSU0.ST.64.H v22 i4  || IAU.ADD i4 i4 8 || lsu1.cp  v29.2  i9
IAU.SHL i12 i12 16 || lsu0.ldih i11 0x0
 canny_sobelloop:
IAU.OR i12 i11 i12	
lsu1.cp  v29.3 i12 
CMU.CLAMP0.f16 v29 v29 v31 || VAU.XOR v22 v22 v22	
nop
LSU0.ST.128.f16.u8 v27 i1 || IAU.ADD i1 i1 8 || cmu.alignvec v11,  v1,    v2,    2
LSU0.ST.128.f16.u8 v28 i2 || IAU.ADD i2 i2 8 || cmu.alignvec v12,  v1,    v2,    4
LSU0.ST.128.f16.u8 v29 i3 || IAU.ADD i3 i3 8 || cmu.alignvec v13,  v3,    v4,    2



IAU.SUB i1 i1 i13
IAU.SUB i1 i1 8
IAU.SUB i2 i2 i13
IAU.SUB i2 i2 8
IAU.SUB i3 i3 i13
IAU.SUB i3 i3 8
IAU.SUB i4 i4 i13
IAU.SUB i4 i4 8
IAU.SUB i4 i4 i13
IAU.SUB i4 i4 8
IAU.ADD i4 i4 2
LSU0.LD.8 i31 i2
nop


LSU0.ldil i5, canny_tangent || LSU1.ldih i5, canny_tangent 
LSU0.LD.16  i6 i5 || IAU.ADD i5 i5 2 //1
LSU0.LD.16  i7 i5 || IAU.ADD i5 i5 2 //2
LSU0.LD.16  i8 i5 || IAU.ADD i5 i5 2 //3
LSU0.LD.16  i9 i5 || IAU.ADD i5 i5 2 //4
LSU0.ldil i5, canny_suppresion || LSU1.ldih i5, canny_suppresion 
LSU0.STi.8 i31 i29


LSU0.LDo.8          i10  i1 0  
LSU0.LDo.8          i11  i1 1  
LSU0.LDo.8          i12  i1 2  
LSU1.LDo.8          i20  i2 0 
LSU1.LDo.8          i22  i2 2 
LSU0.LDo.8          i23  i3 0  
LSU0.LDo.8          i24  i3 1  
LSU0.LDo.8          i25  i3 2  
LSU1.LDo.8          i21  i2 1 || bru.rpl i5  i13 
LSU0.LDi.16         i26  i4 
nop 3
IAU.ADD i1 i1 1
IAU.ADD i2 i2 1
IAU.ADD i3 i3 1

	//first comparison
	CMU.CMII.f16  i26 i9
	PEU.ANDACC  || CMU.CMII.f16  i6 i26
	PEU.ANDACC  || CMU.CMII.U8  i11 i21
	PEU.PC1C GTE || IAU.SUB    i21  i21 i21

	
	CMU.CMII.f16  i26 i9 || LSU0.LDo.8          i11  i1 1  
	PEU.ANDACC  || CMU.CMII.f16  i6 i26
	PEU.ANDACC  || CMU.CMII.U8   i24 i21
	PEU.PC1C GTE || IAU.SUB    i21  i21 i21

	
	//second comparison
	CMU.CMII.f16  i26 i6 || LSU0.LDo.8          i24  i3 1  
	PEU.ANDACC  || CMU.CMII.f16  i7 i26
	PEU.ANDACC  || CMU.CMII.U8  i12 i21
	PEU.PC1C GTE || IAU.SUB    i21  i21 i21

	CMU.CMII.f16  i26 i6 || LSU0.LDo.8          i12  i1 2 
	PEU.ANDACC  || CMU.CMII.f16  i7 i26
	PEU.ANDACC  || CMU.CMII.U8  i23 i21
	PEU.PC1C GTE || IAU.SUB    i21  i21 i21
	
	
	
	//third comparison
	CMU.CMII.f16  i26 i7 || LSU0.LDo.8          i23  i3 0  
	PEU.ANDACC  || CMU.CMII.U8  i22 i21
	PEU.PC1C GTE || IAU.SUB    i21  i21 i21

	CMU.CMII.f16  i26 i7
	PEU.ANDACC  || CMU.CMII.U8  i20 i21
	PEU.PC1C GTE || IAU.SUB    i21  i21 i21

	CMU.CMII.f16  i8 i26 
	PEU.ANDACC  || CMU.CMII.U8  i22 i21
	PEU.PC1C GTE || IAU.SUB    i21  i21 i21

	CMU.CMII.f16  i8 i26 || LSU0.LDo.8          i22  i2 2 
	PEU.ANDACC  || CMU.CMII.U8  i20 i21
	PEU.PC1C GTE || IAU.SUB    i21  i21 i21

	
	//fourth comparison
	CMU.CMII.f16  i26 i8 || LSU0.LDo.8          i20  i2 0 
	PEU.ANDACC  || CMU.CMII.f16  i9 i26
 canny_suppresion:
	PEU.ANDACC  || CMU.CMII.U8  i10 i21
	
	PEU.PC1C GTE || IAU.SUB    i21  i21 i21
	
	
	CMU.CMII.f16  i26 i8 || LSU0.LDo.8          i10  i1 0
	PEU.ANDACC  || CMU.CMII.f16  i9 i26
	PEU.ANDACC  || CMU.CMII.U8  i25 i21
	PEU.PC1C GTE || IAU.SUB    i21  i21 i21
	
	LSU0.ST.8 i21 i29 || IAU.ADD i29 i29 1 || LSU1.LDo.8          i25  i3 2
	

LSU0.ldil i0, canny_final || LSU1.ldih i0, canny_final
IAU.SUB i1  i1 i13
IAU.SUB i2  i2 i13
IAU.SUB i3  i3 i13
IAU.SUB i29 i29 i13
IAU.SUB i29 i29 1
LSU0.ldil i4, 0xff || LSU1.ldih i4, 0x0
LSU0.ldil i5, 0x0  || LSU1.ldih i5, 0x0


LSU0.LDo.8          i7    i1 0 
LSU0.LDo.8          i8    i1 1
LSU0.LDo.8          i9    i1 2
LSU0.LDo.8          i10  i29 0
LSU0.LDo.8          i11  i29 1 || bru.rpl i0 i13
LSU0.LDo.8          i12  i29 2
LSU0.LDo.8          i20   i3 0
LSU0.LDo.8          i21   i3 1
LSU0.LDo.8          i22   i3 2 || IAU.add  i1  i1 1

IAU.add i29 i29 1
IAU.add  i3  i3 1

	CMU.CMII.U8  i11 i15
	peu.pc1c lt || lsu0.st.8 i5 i17
	
	CMU.CMII.U8  i11 i14
	peu.pc1c gt || lsu0.st.8 i4 i17
	
	CMU.CMII.U8  i14 i11
	PEU.ANDACC  || CMU.CMII.U8  i11 i15
	PEU.ANDACC  || CMU.CMII.U8  i7 i14
	peu.pc1c gte || lsu0.st.8 i4 i17
	peu.pc1c lt || lsu0.st.8 i5 i17
	
	CMU.CMII.U8  i14 i11 || LSU0.LDo.8          i7    i1 0
	PEU.ANDACC  || CMU.CMII.U8  i11 i15
	PEU.ANDACC  || CMU.CMII.U8  i8 i14
	peu.pc1c gte || lsu0.st.8 i4 i17
	peu.pc1c lt || lsu0.st.8 i5 i17
	
	CMU.CMII.U8  i14 i11 || LSU0.LDo.8          i8    i1 1
	PEU.ANDACC  || CMU.CMII.U8  i11 i15
	PEU.ANDACC  || CMU.CMII.U8  i9 i14
	peu.pc1c gte || lsu0.st.8 i4 i17
	peu.pc1c lt || lsu0.st.8 i5 i17
	
	CMU.CMII.U8  i14 i11 || LSU0.LDo.8          i9    i1 2
	PEU.ANDACC  || CMU.CMII.U8  i11 i15
	PEU.ANDACC  || CMU.CMII.U8  i10 i14
	peu.pc1c gte || lsu0.st.8 i4 i17
	peu.pc1c lt || lsu0.st.8 i5 i17
	
	CMU.CMII.U8  i14 i11 || LSU0.LDo.8          i10  i29 0
	PEU.ANDACC  || CMU.CMII.U8  i11 i15
	PEU.ANDACC  || CMU.CMII.U8  i12 i14
	peu.pc1c gte || lsu0.st.8 i4 i17
	peu.pc1c lt || lsu0.st.8 i5 i17
	
	CMU.CMII.U8  i14 i11 
	PEU.ANDACC  || CMU.CMII.U8  i11 i15
	PEU.ANDACC  || CMU.CMII.U8  i20 i14
	peu.pc1c gte || lsu0.st.8 i4 i17
	peu.pc1c lt || lsu0.st.8 i5 i17
	
	CMU.CMII.U8  i14 i11
	PEU.ANDACC  || CMU.CMII.U8  i11 i15
	PEU.ANDACC  || CMU.CMII.U8  i21 i14
	peu.pc1c gte || lsu0.st.8 i4 i17
 canny_final:
	peu.pc1c lt || lsu0.st.8 i5 i17

	CMU.CMII.U8  i14 i11
	PEU.ANDACC  || CMU.CMII.U8  i11 i15
	PEU.ANDACC  || CMU.CMII.U8  i22 i14
	peu.pc1c gte || lsu0.st.8 i4 i17
	peu.pc1c lt || lsu0.st.8 i5 i17
	IAU.ADD i17 i17 1 


	
	LSU0.LD.64.h  v31  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v31  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v30  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v30  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v29  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v29  i19 || IAU.ADD i19 i19 8
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

	LSU0.LD.32  i31  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i29  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i28  i19 || IAU.ADD i19 i19 4	
	LSU0.LD.32  i27  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i26  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i25  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i24  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i23  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i22  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i21  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i20  i19 || IAU.ADD i19 i19 4
		nop 6
	
	bru.jmp i30
	nop 6
.size mvcvCanny_asm,.-mvcvCanny_asm
.end
