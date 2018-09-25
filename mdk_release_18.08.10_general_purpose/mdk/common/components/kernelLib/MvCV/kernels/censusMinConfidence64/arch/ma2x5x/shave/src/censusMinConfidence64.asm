.type mvcvCensusMinConfidence64_asm,@function
.version 00.50.00

//////
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
/// 
/// mvcvCensusMatching65 
//     @param[in] in1            - pointer to input lines of the left image
//     @param[in] in2            - pointer to input lines of the right image
//     @param[out] out           - array of disparity cost
//     @param[in] width          - width of the input lines
//     @return    Nothing




.data .rodata.mvcvCensusMinConfidence64
.align 16

mvcvCensusMinConfidence64_data:
		.short     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15
		.short   16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
		.short   32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47
		.short   48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63


.code .text.mvcvCensusMinConfidence64_asm
.align 16

.lalign
//mvcvCensusMinConfidence64(u8 *input, u8 *minimumPosition, u8* minimumValue, u32 width)
//                               i18                i17           i16            i15
mvcvCensusMinConfidence64_asm:
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
nop
lsu0.ldil i1 1 
lsu0.ldil i2 0x00ff || lsu1.ldih i2 0 
cmu.cpivr.x16 v26 i1 || lsu0.ldil i1 0xffff
 cmu.cpivr.x16 v27 i1
lsu0.ldil i8 mvcvCensusMinConfidence64_data || lsu1.ldih i8 mvcvCensusMinConfidence64_data 
lsu0.ldil i6 mvcvCensusMinConfidence64_loop || lsu1.ldih i6 mvcvCensusMinConfidence64_loop 
lsu0.ldi.128.u8.u16 v7 i18 
lsu0.ldi.128.u8.u16 v8 i18 
lsu0.ldi.128.u8.u16 v9 i18 
lsu0.ldi.128.u8.u16 v10 i18 
lsu0.ldi.128.u8.u16 v24 i18 
lsu0.ldi.128.u8.u16 v25 i18 
lsu0.ldi.128.u8.u16 v4 i18 
lsu0.ldi.128.u8.u16 v5 i18 
vau.shl.x16 v11 v7  8  || lsu0.ldo.64.l v0 i8 0  || lsu1.ldo.64.h v0 i8 8   
vau.shl.x16 v12 v8  8  || lsu0.ldo.64.l v1 i8 16 || lsu1.ldo.64.h v1 i8 24
vau.shl.x16 v13 v9  8  || lsu0.ldo.64.l v2 i8 32 || lsu1.ldo.64.h v2 i8 40
vau.shl.x16 v14 v10 8  || lsu0.ldo.64.l v3 i8 48 || lsu1.ldo.64.h v3 i8 56
vau.shl.x16 v15 v24 8  || lsu0.ldo.64.l v0 i8 64  || lsu1.ldo.64.h v0 i8 72 
vau.shl.x16 v16 v25 8  || lsu0.ldo.64.l v1 i8 80  || lsu1.ldo.64.h v1 i8 88  
vau.shl.x16 v17 v4  8  || lsu0.ldo.64.l v2 i8 96  || lsu1.ldo.64.h v2 i8 104
vau.shl.x16 v18 v5  8  || lsu0.ldo.64.l v3 i8 112 || lsu1.ldo.64.h v3 i8 120  
nop
vau.or v11 v11 v0 
vau.or v12 v12 v1 
vau.or v13 v13 v2 
vau.or v14 v14 v3 
vau.or v15 v15 v0 
vau.or v16 v16 v1
vau.or v17 v17 v2
vau.or v18 v18 v3
cmu.min.u16 v12 v11 v12 
cmu.min.u16 v14 v13 v14
cmu.min.u16 v16 v15 v16
cmu.min.u16 v18 v17 v18
nop
cmu.min.u16 v19 v12 v14 
cmu.min.u16 v20 v16 v18
nop
cmu.min.u16 v21 v19 v20 
nop 
cmu.alignvec v22 v21 v21 8 
cmu.min.u16 v23 v21 v22 
bru.rpl i6 i15
cmu.alignvec v22 v23 v23 4
cmu.min.u16 v23 v23 v22
nop
cmu.alignvec v22 v23 v23 2
cmu.min.u16 v23 v23 v22
nop
cmu.cpvi.x16 i0.l v23.0 // lower part = position
iau.shr.u32 i1 i0 8// minval
iau.and i1 i1 i2
cmu.cpivr.x16 v31 i1 || lsu0.sti.8 i0 i17 
//find second min
//#1
cmu.cpvi.x16 i7.l v7.1 || iau.mul i3 i2 i1 || lsu1.ldo.64.l v0 i8 0  
cmu.cpzv v28 2                             || lsu1.ldo.64.l v1 i8 16 
cmu.alignvec v11 v7 v7 14                  || lsu1.ldo.64.l v2 i8 32 
cmu.alignvec v12 v7 v8 2                   || lsu1.ldo.64.l v3 i8 48 
cmu.cpiv.x16 v11.0  i7.l                                           || lsu1.ldo.64.h v0 i8 8     
cmu.cmvv.u16 v7 v11                                                || lsu1.ldo.64.h v1 i8 24
peu.pvl016 lt  || lsu0.cp v28 v26 || cmu.cmvv.u16 v7 v12           || lsu1.ldo.64.h v2 i8 40
peu.pvl016 lt  || lsu0.cp v29 v26 || cmu.cmvv.u16 v7 v31           || lsu1.ldo.64.h v3 i8 56
peu.pvl016 neq || lsu0.cp v30 v26 || cmu.alignvec v11 v7 v8 14
vau.and v29 v28 v29 || cmu.alignvec v12 v8 v9 2
vau.and v13 v29 v30 || cmu.cmvv.u16 v8 v11
//#2
cmu.cpzv v28 2
peu.pvl016 lt  || lsu0.cp v28 v26 || cmu.cmvv.u16 v8 v12
peu.pvl016 lt  || lsu0.cp v29 v26 || cmu.cmvv.u16 v8 v31
peu.pvl016 neq || lsu0.cp v30 v26  || cmu.alignvec v11 v8 v9 14
vau.and v29 v28 v29 || cmu.alignvec v12 v9 v10 2
vau.and v14 v29 v30 || cmu.cmvv.u16 v9 v11
//#3
cmu.cpzv v28 2
peu.pvl016 lt  || lsu0.cp v28 v26 || cmu.cmvv.u16 v9 v12
peu.pvl016 lt  || lsu0.cp v29 v26 || cmu.cmvv.u16 v9 v31
peu.pvl016 neq || lsu0.cp v30 v26 || cmu.alignvec v11 v9 v10 14
vau.and v29 v28 v29 || cmu.alignvec v12 v10 v24 2
vau.and v15 v29 v30 || cmu.cmvv.u16 v10 v11
//#4
cmu.cpzv v28 2
peu.pvl016 lt  || lsu0.cp v28 v26 || cmu.cmvv.u16 v10 v12
peu.pvl016 lt  || lsu0.cp v29 v26 || cmu.cmvv.u16 v10 v31
peu.pvl016 neq || lsu0.cp v30 v26 || cmu.alignvec v11 v10 v24 14
vau.and v29 v28 v29 || cmu.alignvec v12 v24 v25 2
vau.and v16 v29 v30 || cmu.cmvv.u16 v24 v11
//#5
cmu.cpzv v28 2
peu.pvl016 lt  || lsu0.cp v28 v26 || cmu.cmvv.u16 v24 v12
peu.pvl016 lt  || lsu0.cp v29 v26 || cmu.cmvv.u16 v24 v31
peu.pvl016 neq || lsu0.cp v30 v26 || cmu.alignvec v11 v24 v25 14
vau.and v29 v28 v29 || cmu.alignvec v12 v25 v4 2
vau.and v17 v29 v30 || cmu.cmvv.u16 v25 v11
//#6
cmu.cpzv v28 2
peu.pvl016 lt  || lsu0.cp v28 v26 || cmu.cmvv.u16 v25 v12
peu.pvl016 lt  || lsu0.cp v29 v26 || cmu.cmvv.u16 v25 v31
peu.pvl016 neq || lsu0.cp v30 v26 || cmu.alignvec v11 v25 v4 14
vau.and v29 v28 v29 || cmu.alignvec v12 v4 v5 2
vau.and v18 v29 v30 || cmu.cmvv.u16 v4 v11
//#7
cmu.cpzv v28 2
peu.pvl016 lt  || lsu0.cp v28 v26 || cmu.cmvv.u16 v4 v12
peu.pvl016 lt  || lsu0.cp v29 v26 || cmu.cmvv.u16 v4 v31
peu.pvl016 neq || lsu0.cp v30 v26 || cmu.cpvi.x16 i7.l v5.6
vau.and v29 v28 v29 || cmu.alignvec v11 v4 v5 14
vau.and v19 v29 v30 || cmu.alignvec v12 v5 v5 2
//#8
cmu.cpiv.x16 v12.7  i7.l
cmu.cpzv v28 2
cmu.cmvv.u16 v5 v11
peu.pvl016 lt  || lsu0.cp v28 v26 || cmu.cmvv.u16 v5 v12
peu.pvl016 lt  || lsu0.cp v29 v26 || cmu.cmvv.u16 v5 v31
peu.pvl016 neq || lsu0.cp v30 v26 || cmu.cpivr.x16 v11 i2
vau.and v29 v28 v29 || cmu.cpivr.x16 v12 i2
vau.and v20 v29 v30 || cmu.cpivr.x16 v27 i2
cmu.cpivr.x16  v6 i2 || lsu1.ldi.128.u8.u16 v7 i18 
cmu.cpivr.x16 v21 i2 || lsu1.ldi.128.u8.u16 v8 i18 
cmu.cpivr.x16 v22 i2 || lsu1.ldi.128.u8.u16 v9 i18 
cmu.cpivr.x16 v23 i2 || lsu1.ldi.128.u8.u16 v10 i18 
cmu.cpivr.x16 v28 i2 || lsu1.ldi.128.u8.u16 v24 i18 
                                      cmu.cmz.i16 v13 || lsu1.ldi.128.u8.u16 v25 i18 
peu.pvl016 neq  || lsu0.cp v11 v7  || cmu.cmz.i16 v14 || lsu1.ldi.128.u8.u16 v4 i18 
peu.pvl016 neq  || lsu0.cp v12 v8  || cmu.cmz.i16 v15 || lsu1.ldi.128.u8.u16 v5 i18 
peu.pvl016 neq  || lsu0.cp v27 v9  || cmu.cmz.i16 v16 
peu.pvl016 neq  || lsu0.cp  v6 v10 || cmu.cmz.i16 v17 || vau.shl.x16 v13 v9  8 
peu.pvl016 neq  || lsu0.cp v21 v24 || cmu.cmz.i16 v18 || vau.shl.x16 v14 v10 8 
peu.pvl016 neq  || lsu0.cp v22 v25 || cmu.cmz.i16 v19 || vau.shl.x16 v15 v24 8 
peu.pvl016 neq  || lsu0.cp v23 v4  || cmu.cmz.i16 v20 || vau.shl.x16 v16 v25 8  
peu.pvl016 neq  || lsu0.cp v28 v5  || cmu.min.u16 v11 v11 v12 || vau.shl.x16 v17 v4  8 
cmu.min.u16 v27 v27 v6             || vau.shl.x16 v12 v8  8   || lsu0.ldo.64.l v0 i8 64  || lsu1.ldo.64.h v0 i8 72 
cmu.min.u16 v21 v21 v22                                       || lsu0.ldo.64.l v1 i8 80  || lsu1.ldo.64.h v1 i8 88 
cmu.min.u16 v23 v23 v28            || vau.or v12 v12 v1       || lsu0.ldo.64.l v2 i8 96  || lsu1.ldo.64.h v2 i8 104                     
cmu.min.u16 v18 v11 v27            || vau.shl.x16 v11 v7  8   || lsu0.ldo.64.l v3 i8 112 || lsu1.ldo.64.h v3 i8 120
cmu.min.u16 v20 v21 v23                                    
vau.or v11 v11 v0                            
cmu.min.u16 v20 v18 v20 || vau.shl.x16 v18 v5  8                                   
nop                                                                   
cmu.min.u16 v21 v20 v20 || LSU0.SWZC8 [32107654]   || vau.or v13 v13 v2 
vau.or v14 v14 v3 
cmu.min.u16 v22 v21 v21 || LSU0.SWZC8 [54761032]   || vau.or v15 v15 v0 
vau.or v16 v16 v1
cmu.min.u16 v20 v22 v22 || LSU0.SWZC8 [67452301] || iau.xor i4 i4 i4 || vau.or v17 v17 v2
vau.or v18 v18 v3
cmu.cpvi.x16 i4.l v20.0
                         cmu.min.u16 v12 v11 v12 
sau.div.u16 i5 i3 i4     || cmu.min.u16 v14 v13 v14                     // minimumValue
                         cmu.min.u16 v16 v15 v16
                         cmu.min.u16 v18 v17 v18
mvcvCensusMinConfidence64_loop:
						 cmu.min.u16 v19 v12 v14 
cmu.min.u16 v20 v16 v18
nop
cmu.min.u16 v21 v19 v20 
nop 
cmu.alignvec v22 v21 v21 8 
lsu1.sti.8 i5 i16 || cmu.min.u16 v23 v21 v22 


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

bru.jmp i30
nop 6
.size mvcvCensusMinConfidence64_asm,.-mvcvCensusMinConfidence64_asm
.end