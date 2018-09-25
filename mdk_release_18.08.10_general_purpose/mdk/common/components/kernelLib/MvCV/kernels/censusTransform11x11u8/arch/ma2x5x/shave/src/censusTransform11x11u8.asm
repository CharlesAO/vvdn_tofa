.type mvcvCensusTransform11x11u8_asm,@function
.version 00.50.00

/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.data .data.censusTransform11x11u8
.align 4

.code .text.censusTransform11x11u8

//void mvcvCensusTransform11x11u8(u16 **in, u8 *out, u32 width)
//							      i18	     i17	   i16

mvcvCensusTransform11x11u8_asm:

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
	LSU0.ST.32  i30  i19 || IAU.SUB i19 i19 4
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

lsu0.ldil i15 censusTransform11x11u8_loop  || lsu1.ldih i15 censusTransform11x11u8_loop	
lsu0.ldil i11 0x1 || lsu1.ldih i11 0x0
//load addresses
lsu0.ldo.32 i0 i18 0x0 || cmu.cpivr.x8 v23 i11
lsu0.ldo.32 i1 i18 0x4 || iau.shr.u32 i16 i16 4
lsu0.ldo.32 i2 i18 0x8
lsu0.ldo.32 i3 i18 0xc
lsu0.ldo.32 i4 i18 0x10
lsu0.ldo.32 i5 i18 0x14
lsu0.ldo.32 i6 i18 0x18
lsu0.ldo.32 i7 i18 0x1c    || iau.sub i0 i0 16
lsu0.ldo.32 i8 i18 0x20    || iau.sub i1 i1 16
lsu0.ldo.32 i9 i18 0x24    || iau.sub i2 i2 16
lsu0.ldo.32 i10 i18 0x28   || iau.sub i3 i3 16
//

iau.sub i4 i4 16
iau.sub i5 i5 16
iau.sub i6 i6 16
iau.sub i7 i7 16
iau.sub i8 i8 16
iau.sub i9 i9 16
iau.sub i10 i10 16
cmu.cpzv v0 3  || lsu0.ldo.64.l v16 i0 0  || lsu1.ldo.64.h v16 i0 8   
cmu.cpzv v4 3  || lsu0.ldo.64.l v17 i0 16  || lsu1.ldo.64.h v17 i0 24
cmu.cpzv v8 3  || lsu0.ldo.64.l v18 i0 32  || lsu1.ldo.64.h v18 i0 40 || iau.add i0 i0 16
cmu.cpzv v12 3 || lsu0.ldo.64.l v19 i5 16  || lsu1.ldo.64.h v19 i5 24

nop 6
bru.rpl i15 i16
//first row compare

cmu.alignvec v20 v16 v17 11 || vau.shl.x8 v0 v0 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v0 v0 v23 

cmu.alignvec v20 v16 v17 12 || vau.shl.x8 v0 v0 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v0 v0 v23 
cmu.alignvec v20 v16 v17 13 || vau.shl.x8 v0 v0 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v0 v0 v23 
cmu.alignvec v20 v16 v17 14 || vau.shl.x8 v0 v0 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v0 v0 v23 
cmu.alignvec v20 v16 v17 15 || vau.shl.x8 v0 v0 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v0 v0 v23 
                               vau.shl.x8 v0 v0 1 
cmu.cmvv.u8 v19 v17 || lsu0.ldo.64.l v16 i1 0  || lsu1.ldo.64.h v16 i1 8   
peu.pvv8 GT ||  vau.or v0 v0 v23 
cmu.alignvec v20 v17 v18 1 || vau.shl.x8 v0 v0 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v0 v0 v23 
cmu.alignvec v20 v17 v18 2 || vau.shl.x8 v0 v0 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v0 v0 v23 
cmu.alignvec v20 v17 v18 3 || vau.shl.x8 v1 v1 1 
cmu.cmvv.u8 v19 v20 || lsu0.ldo.64.l v17 i1 16  || lsu1.ldo.64.h v17 i1 24
peu.pvv8 GT ||  vau.or v1 v1 v23 
cmu.alignvec v20 v17 v18 4 || vau.shl.x8 v1 v1 1 || lsu0.ldo.64.l v18 i1 32  || lsu1.ldo.64.h v18 i1 40 || iau.add i1 i1 16
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v1 v1 v23 
cmu.alignvec v20 v17 v18 5 || vau.shl.x8 v1 v1 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v1 v1 v23 

//second row compare
cmu.alignvec v20 v16 v17 11 || vau.shl.x8 v1 v1 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v1 v1 v23 
cmu.alignvec v20 v16 v17 12 || vau.shl.x8 v1 v1 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v1 v1 v23 
cmu.alignvec v20 v16 v17 13 || vau.shl.x8 v1 v1 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v1 v1 v23 
cmu.alignvec v20 v16 v17 14 || vau.shl.x8 v1 v1 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v1 v1 v23 
cmu.alignvec v20 v16 v17 15 || vau.shl.x8 v1 v1 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v1 v1 v23 
                               vau.shl.x8 v2 v2 1 || cmu.vilv.x8  v0  v1  v1  v0 // 
cmu.cmvv.u8 v19 v17 || lsu0.ldo.64.l v16 i2 0  || lsu1.ldo.64.h v16 i2 8   
peu.pvv8 GT ||  vau.or v2 v2 v23 
cmu.alignvec v20 v17 v18 1 || vau.shl.x8 v2 v2 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v2 v2 v23 
cmu.alignvec v20 v17 v18 2 || vau.shl.x8 v2 v2 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v2 v2 v23 
cmu.alignvec v20 v17 v18 3 || vau.shl.x8 v2 v2 1 
cmu.cmvv.u8 v19 v20 || lsu0.ldo.64.l v17 i2 16  || lsu1.ldo.64.h v17 i2 24
peu.pvv8 GT ||  vau.or v2 v2 v23 
cmu.alignvec v20 v17 v18 4 || vau.shl.x8 v2 v2 1 || lsu0.ldo.64.l v18 i2 32  || lsu1.ldo.64.h v18 i2 40 || iau.add i2 i2 16
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v2 v2 v23 
cmu.alignvec v20 v17 v18 5 || vau.shl.x8 v2 v2 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v2 v2 v23 

//third row compare
cmu.alignvec v20 v16 v17 11 || vau.shl.x8 v2 v2 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v2 v2 v23 
cmu.alignvec v20 v16 v17 12 || vau.shl.x8 v2 v2 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v2 v2 v23 
cmu.alignvec v20 v16 v17 13 || vau.shl.x8 v3 v3 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v3 v3 v23 
cmu.alignvec v20 v16 v17 14 || vau.shl.x8 v3 v3 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v3 v3 v23 
cmu.alignvec v20 v16 v17 15 || vau.shl.x8 v3 v3 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v3 v3 v23 
                               vau.shl.x8 v3 v3 1 
cmu.cmvv.u8 v19 v17 || lsu0.ldo.64.l v16 i3 0   || lsu1.ldo.64.h v16 i3 8  
peu.pvv8 GT ||  vau.or v3 v3 v23 
cmu.alignvec v20 v17 v18 1 || vau.shl.x8 v3 v3 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v3 v3 v23 
cmu.alignvec v20 v17 v18 2 || vau.shl.x8 v3 v3 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v3 v3 v23 
cmu.alignvec v20 v17 v18 3 || vau.shl.x8 v3 v3 1 
cmu.cmvv.u8 v19 v20 || lsu0.ldo.64.l v17 i3 16  || lsu1.ldo.64.h v17 i3 24
peu.pvv8 GT ||  vau.or v3 v3 v23 
cmu.alignvec v20 v17 v18 4 || vau.shl.x8 v3 v3 1 || lsu0.ldo.64.l v18 i3 32  || lsu1.ldo.64.h v18 i3 40 || iau.add i3 i3 16
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v3 v3 v23 
cmu.alignvec v20 v17 v18 5 || vau.shl.x8 v4 v4 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v4 v4 v23 || cmu.vilv.x8  v2  v3  v3  v2 // 


//forth row compare
cmu.alignvec v20 v16 v17 11 || vau.shl.x8 v4 v4 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v4 v4 v23 || cmu.vilv.x16  v0 v2   v2  v0 
cmu.alignvec v20 v16 v17 12 || vau.shl.x8 v4 v4 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v4 v4 v23 || cmu.vilv.x16  v1 v3   v3  v1 
cmu.alignvec v20 v16 v17 13 || vau.shl.x8 v4 v4 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v4 v4 v23 
cmu.alignvec v20 v16 v17 14 || vau.shl.x8 v4 v4 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v4 v4 v23 
cmu.alignvec v20 v16 v17 15 || vau.shl.x8 v4 v4 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v4 v4 v23 
                               vau.shl.x8 v4 v4 1 
cmu.cmvv.u8 v19 v17 || lsu0.ldo.64.l v16 i4 0   || lsu1.ldo.64.h v16 i4 8  
peu.pvv8 GT ||  vau.or v4 v4 v23 
cmu.alignvec v20 v17 v18 1 || vau.shl.x8 v4 v4 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v4 v4 v23 
cmu.alignvec v20 v17 v18 2 || vau.shl.x8 v5 v5 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v5 v5 v23 
cmu.alignvec v20 v17 v18 3 || vau.shl.x8 v5 v5 1 
cmu.cmvv.u8 v19 v20 || lsu0.ldo.64.l v17 i4 16  || lsu1.ldo.64.h v17 i4 24
peu.pvv8 GT ||  vau.or v5 v5 v23 
cmu.alignvec v20 v17 v18 4 || vau.shl.x8 v5 v5 1 || lsu0.ldo.64.l v18 i4 32  || lsu1.ldo.64.h v18 i4 40 || iau.add i4 i4 16
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v5 v5 v23 
cmu.alignvec v20 v17 v18 5 || vau.shl.x8 v5 v5 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v5 v5 v23 


//fifth row compare
cmu.alignvec v20 v16 v17 11 || vau.shl.x8 v5 v5 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v5 v5 v23 
cmu.alignvec v20 v16 v17 12 || vau.shl.x8 v5 v5 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v5 v5 v23 
cmu.alignvec v20 v16 v17 13 || vau.shl.x8 v5 v5 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v5 v5 v23 
cmu.alignvec v20 v16 v17 14 || vau.shl.x8 v5 v5 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v5 v5 v23 
cmu.alignvec v20 v16 v17 15 || vau.shl.x8 v6 v6 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v6 v6 v23 
                               vau.shl.x8 v6 v6 1 || cmu.vilv.x8  v4  v5  v5  v4 // 
cmu.cmvv.u8 v19 v17 || lsu0.ldo.64.l v16 i5 0   || lsu1.ldo.64.h v16 i5 8  
peu.pvv8 GT ||  vau.or v6 v6 v23 
cmu.alignvec v20 v17 v18 1 || vau.shl.x8 v6 v6 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v6 v6 v23 
cmu.alignvec v20 v17 v18 2 || vau.shl.x8 v6 v6 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v6 v6 v23 
cmu.alignvec v20 v17 v18 3 || vau.shl.x8 v6 v6 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v6 v6 v23 
cmu.alignvec v20 v17 v18 4 || vau.shl.x8 v6 v6 1 
cmu.cmvv.u8 v19 v20 || lsu0.ldo.64.l v18 i5 32  || lsu1.ldo.64.h v18 i5 40 || iau.add i5 i5 16
peu.pvv8 GT ||  vau.or v6 v6 v23 
cmu.alignvec v20 v17 v18 5 || vau.shl.x8 v6 v6 1 
cmu.cmvv.u8 v19 v20 || lsu0.cp v17 v19 //lsu0.ldo.64.l v17 i4 16  || lsu1.ldo.64.h v17 i4 24
peu.pvv8 GT ||  vau.or v6 v6 v23 

//sixth row compare
cmu.alignvec v20 v16 v17 11 || vau.shl.x8 v6 v6 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v6 v6 v23 
cmu.alignvec v20 v16 v17 12 || vau.shl.x8 v7 v7 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v7 v7 v23 
cmu.alignvec v20 v16 v17 13 || vau.shl.x8 v7 v7 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v7 v7 v23 
cmu.alignvec v20 v16 v17 14 || vau.shl.x8 v7 v7 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v7 v7 v23 
cmu.alignvec v20 v16 v17 15 || vau.shl.x8 v7 v7 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v7 v7 v23 
                               vau.shl.x8 v7 v7 1 
cmu.cmvv.u8 v19 v17 || lsu0.ldo.64.l v16 i6 0   || lsu1.ldo.64.h v16 i6 8 
peu.pvv8 GT ||  vau.or v7 v7 v23 
cmu.alignvec v20 v17 v18 1 || vau.shl.x8 v7 v7 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v7 v7 v23 
cmu.alignvec v20 v17 v18 2 || vau.shl.x8 v7 v7 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v7 v7 v23 
cmu.alignvec v20 v17 v18 3 || vau.shl.x8 v7 v7 1 
cmu.cmvv.u8 v19 v20  || lsu0.ldo.64.l v17 i6 16  || lsu1.ldo.64.h v17 i6 24
peu.pvv8 GT ||  vau.or v7 v7 v23 
cmu.alignvec v20 v17 v18 4 || vau.shl.x8 v8 v8 1 || lsu0.ldo.64.l v18 i6 32  || lsu1.ldo.64.h v18 i6 40 || iau.add i6 i6 16
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v8 v8 v23 || cmu.vilv.x8  v6  v7  v7  v6 // 
cmu.alignvec v20 v17 v18 5 || vau.shl.x8 v8 v8 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v8 v8 v23 || cmu.vilv.x16  v4 v6   v6  v4 


//seventh row compare
cmu.alignvec v20 v16 v17 11 || vau.shl.x8 v8 v8 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v8 v8 v23 || cmu.vilv.x16  v5 v7   v7  v5 
cmu.alignvec v20 v16 v17 12 || vau.shl.x8 v8 v8 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v8 v8 v23 || cmu.vilv.x32 v0 v4 v4 v0
cmu.alignvec v20 v16 v17 13 || vau.shl.x8 v8 v8 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v8 v8 v23 || cmu.vilv.x32 v2 v6 v6 v2
cmu.alignvec v20 v16 v17 14 || vau.shl.x8 v8 v8 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v8 v8 v23 || cmu.vilv.x32 v1 v5 v5 v1
cmu.alignvec v20 v16 v17 15 || vau.shl.x8 v8 v8 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v8 v8 v23 || cmu.vilv.x32 v3 v7 v7 v3
                               vau.shl.x8 v8 v8 1 
cmu.cmvv.u8 v19 v17 || lsu0.ldo.64.l v16 i7 0   || lsu1.ldo.64.h v16 i7 8
peu.pvv8 GT ||  vau.or v8 v8 v23 
cmu.alignvec v20 v17 v18 1 || vau.shl.x8 v9 v9 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v9 v9 v23 
cmu.alignvec v20 v17 v18 2 || vau.shl.x8 v9 v9 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v9 v9 v23 
cmu.alignvec v20 v17 v18 3 || vau.shl.x8 v9 v9 1 
cmu.cmvv.u8 v19 v20 || lsu0.ldo.64.l v17 i7 16  || lsu1.ldo.64.h v17 i7 24
peu.pvv8 GT ||  vau.or v9 v9 v23 
cmu.alignvec v20 v17 v18 4 || vau.shl.x8 v9 v9 1 || lsu0.ldo.64.l v18 i7 32  || lsu1.ldo.64.h v18 i7 40 || iau.add i7 i7 16
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v9 v9 v23 
cmu.alignvec v20 v17 v18 5 || vau.shl.x8 v9 v9 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v9 v9 v23 


//eight row compare
cmu.alignvec v20 v16 v17 11 || vau.shl.x8 v9 v9 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v9 v9 v23 
cmu.alignvec v20 v16 v17 12 || vau.shl.x8 v9 v9 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v9 v9 v23 
cmu.alignvec v20 v16 v17 13 || vau.shl.x8 v9 v9 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v9 v9 v23 
cmu.alignvec v20 v16 v17 14 || vau.shl.x8 v10 v10 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v10 v10 v23 || cmu.vilv.x8  v8  v9  v9  v8 // 
cmu.alignvec v20 v16 v17 15 || vau.shl.x8 v10 v10 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v10 v10 v23 
                               vau.shl.x8 v10 v10 1 
cmu.cmvv.u8 v19 v17 || lsu0.ldo.64.l v16 i8 0   || lsu1.ldo.64.h v16 i8 8  
peu.pvv8 GT ||  vau.or v10 v10 v23 
cmu.alignvec v20 v17 v18 1 || vau.shl.x8 v10 v10 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v10 v10 v23 
cmu.alignvec v20 v17 v18 2 || vau.shl.x8 v10 v10 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v10 v10 v23 
cmu.alignvec v20 v17 v18 3 || vau.shl.x8 v10 v10 1 
cmu.cmvv.u8 v19 v20 || lsu0.ldo.64.l v17 i8 16  || lsu1.ldo.64.h v17 i8 24
peu.pvv8 GT ||  vau.or v10 v10 v23 
cmu.alignvec v20 v17 v18 4 || vau.shl.x8 v10 v10 1 || lsu0.ldo.64.l v18 i8 32  || lsu1.ldo.64.h v18 i8 40 || iau.add i8 i8 16
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v10 v10 v23 
cmu.alignvec v20 v17 v18 5 || vau.shl.x8 v10 v10 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v10 v10 v23 


//nineth row compare
cmu.alignvec v20 v16 v17 11 || vau.shl.x8 v11 v11 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v11 v11 v23 
cmu.alignvec v20 v16 v17 12 || vau.shl.x8 v11 v11 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v11 v11 v23 
cmu.alignvec v20 v16 v17 13 || vau.shl.x8 v11 v11 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v11 v11 v23 
cmu.alignvec v20 v16 v17 14 || vau.shl.x8 v11 v11 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v11 v11 v23 
cmu.alignvec v20 v16 v17 15 || vau.shl.x8 v11 v11 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v11 v11 v23 
                               vau.shl.x8 v11 v11 1 
cmu.cmvv.u8 v19 v17 || lsu0.ldo.64.l v16 i9 0   || lsu1.ldo.64.h v16 i9 8 
peu.pvv8 GT ||  vau.or v11 v11 v23 
cmu.alignvec v20 v17 v18 1 || vau.shl.x8 v11 v11 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v11 v11 v23 
cmu.alignvec v20 v17 v18 2 || vau.shl.x8 v11 v11 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v11 v11 v23 
cmu.alignvec v20 v17 v18 3 || vau.shl.x8 v12 v12 1 
cmu.cmvv.u8 v19 v20 || lsu0.ldo.64.l v17 i9 16  || lsu1.ldo.64.h v17 i9 24
peu.pvv8 GT ||  vau.or v12 v12 v23 || cmu.vilv.x8 v10 v11 v11 v10 // 
cmu.alignvec v20 v17 v18 4 || vau.shl.x8 v12 v12 1 || lsu0.ldo.64.l v18 i9 32  || lsu1.ldo.64.h v18 i9 40 || iau.add i9 i9 16
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v12 v12 v23 || cmu.vilv.x16  v8 v10  v10 v8 
cmu.alignvec v20 v17 v18 5 || vau.shl.x8 v12 v12 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v12 v12 v23 || cmu.vilv.x16  v9 v11  v11 v9 

//tenth row compare
cmu.alignvec v20 v16 v17 11 || vau.shl.x8 v12 v12 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v12 v12 v23 
cmu.alignvec v20 v16 v17 12 || vau.shl.x8 v12 v12 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v12 v12 v23 
cmu.alignvec v20 v16 v17 13 || vau.shl.x8 v12 v12 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v12 v12 v23 
cmu.alignvec v20 v16 v17 14 || vau.shl.x8 v12 v12 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v12 v12 v23 
cmu.alignvec v20 v16 v17 15 || vau.shl.x8 v12 v12 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v12 v12 v23 
                               vau.shl.x8 v13 v13 1 
cmu.cmvv.u8 v19 v17 || lsu0.ldo.64.l v16 i10 0   || lsu1.ldo.64.h v16 i10 8   
peu.pvv8 GT ||  vau.or v13 v13 v23 
cmu.alignvec v20 v17 v18 1 || vau.shl.x8 v13 v13 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v13 v13 v23 
cmu.alignvec v20 v17 v18 2 || vau.shl.x8 v13 v13 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v13 v13 v23 
cmu.alignvec v20 v17 v18 3 || vau.shl.x8 v13 v13 1 
cmu.cmvv.u8 v19 v20 || lsu0.ldo.64.l v17 i10 16  || lsu1.ldo.64.h v17 i10 24
peu.pvv8 GT ||  vau.or v13 v13 v23 
cmu.alignvec v20 v17 v18 4 || vau.shl.x8 v13 v13 1 || lsu0.ldo.64.l v18 i10 32  || lsu1.ldo.64.h v18 i10 40 || iau.add i10 i10 16
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v13 v13 v23 
cmu.alignvec v20 v17 v18 5 || vau.shl.x8 v13 v13 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v13 v13 v23 


//eleventh row compare
cmu.alignvec v20 v16 v17 11 || vau.shl.x8 v13 v13 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v13 v13 v23 
cmu.alignvec v20 v16 v17 12 || vau.shl.x8 v13 v13 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v13 v13 v23 
cmu.alignvec v20 v16 v17 13 || vau.shl.x8 v14 v14 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v14 v14 v23 || cmu.vilv.x8 v12 v13 v13 v12 // 
cmu.alignvec v20 v16 v17 14 || vau.shl.x8 v14 v14 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v14 v14 v23 
cmu.alignvec v20 v16 v17 15 || vau.shl.x8 v14 v14 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v14 v14 v23 
                               vau.shl.x8 v14 v14 1 
cmu.cmvv.u8 v19 v17
peu.pvv8 GT ||  vau.or v14 v14 v23 
cmu.alignvec v20 v17 v18 1 || vau.shl.x8 v14 v14 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v14 v14 v23 
cmu.alignvec v20 v17 v18 2 || vau.shl.x8 v14 v14 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v14 v14 v23 
cmu.alignvec v20 v17 v18 3 || vau.shl.x8 v14 v14 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v14 v14 v23 
cmu.alignvec v20 v17 v18 4 || vau.shl.x8 v14 v14 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v14 v14 v23 

cmu.alignvec v20 v17 v18 5 || vau.shl.x8 v15 v15 1 
cmu.cmvv.u8 v19 v20 
peu.pvv8 GT ||  vau.or v15 v15 v23 
		
//first
cmu.vilv.x8 v14 v24 v15 v14 // 
//second 
cmu.vilv.x16 v12 v14  v14 v12
cmu.vilv.x16 v13 v24  v24 v13
//third
cmu.vilv.x32 v8 v12  v12  v8 
cmu.vilv.x32 v10 v14 v14  v10
cmu.vilv.x32 v9 v13  v13  v9 
cmu.vilv.x32 v11 v24 v24  v11


//first two	

lsu0.cp i20 v8.0 || lsu1.cp i21 v8.1 || cmu.cpvv v25 v0
cmu.cpiv.x32 v25.2 i20 || lsu0.cp v25.3 i21 || lsu1.cp i20 v0.2
lsu0.cp i21 v0.3 || cmu.cpvv v26 v8 || lsu1.cp i22 v12.0
cmu.cpiv.x32 v26.0 i20 || lsu0.cp v26.1 i21 || lsu1.cp i23 v12.1 
lsu0.sto.64.l v25 i17 0  || lsu1.sto.64.h v25 i17 8 || iau.add i17 i17 16 || cmu.cpvv v27 v4
lsu0.sto.64.l v26 i17 0  || lsu1.sto.64.h v26 i17 8 || iau.add i17 i17 16 || cmu.cpiv.x32 v27.2 i22 

//second two
lsu0.cp v27.3 i23 || lsu1.cp i22 v4.2  || cmu.cpvv v29 v2
lsu0.cp i23 v4.3 || cmu.cpvv v28 v12 || lsu1.cp i24 v10.0 
cmu.cpiv.x32 v28.0 i22 || lsu0.cp v28.1 i23 || lsu1.cp i25 v10.1 
lsu0.sto.64.l v27 i17 0  || lsu1.sto.64.h v27 i17 8 || iau.add i17 i17 16 || cmu.cpiv.x32 v29.2 i24 
lsu0.sto.64.l v28 i17 0  || lsu1.sto.64.h v28 i17 8 || iau.add i17 i17 16 || cmu.cpiv v29.3 i25 

//third set
lsu0.cp i25 v2.3 || cmu.cpvv v30 v10 || lsu1.cp i24 v2.2
cmu.cpiv.x32 v30.0 i24  || lsu0.cp v30.1 i25 || lsu1.cp v25 v6
lsu0.sto.64.l v29 i17 0  || lsu1.sto.64.h v29 i17 8 || iau.add i17 i17 16 || cmu.cpvi i26 v14.0
lsu0.sto.64.l v30 i17 0  || lsu1.sto.64.h v30 i17 8 || iau.add i17 i17 16 || cmu.cpvi i27 v14.1 

//forth set
 
cmu.cpiv.x32 v25.2 i26 || lsu0.cp v25.3 i27 || lsu1.cp i26 v6.2
lsu0.cp i27 v6.3 || cmu.cpvv v26 v14 || lsu1.cp i29 v9.1
cmu.cpiv.x32 v26.0 i26 || lsu0.cp v26.1 i27 || lsu1.cp i28 v9.0 
lsu0.sto.64.l v25 i17 0  || lsu1.sto.64.h v25 i17 8 || iau.add i17 i17 16  || cmu.cpvv v27 v1 
lsu0.sto.64.l v26 i17 0  || lsu1.sto.64.h v26 i17 8 || iau.add i17 i17 16  || cmu.cpiv v27.3 i29 

//fifth set
lsu0.cp v27.2 i28  || lsu1.cp i31 v13.1 || cmu.cpvv v29 v5
lsu0.cp i28 v1.2 || lsu1.cp i29 v1.3 || cmu.cpvv v28 v9
cmu.cpiv.x32 v28.0 i28 || lsu0.cp v28.1 i29 || lsu1.cp i30 v13.0
lsu0.sto.64.l v27 i17 0  || lsu1.sto.64.h v27 i17 8 || iau.add i17 i17 16 || cmu.cpiv.x32 v29.2 i30
lsu0.sto.64.l v28 i17 0  || lsu1.sto.64.h v28 i17 8 || iau.add i17 i17 16 || cmu.cpiv.x32 v29.3 i31

//sixth set
lsu1.cp i30 v5.2 || lsu0.cp i20 v11.0  || cmu.cpvv v25 v3
lsu0.cp i31 v5.3 || cmu.cpvv v30 v13 || lsu1.cp i21 v11.1
cmu.cpiv.x32 v30.0 i30 || lsu1.cp v30.1 i31 || lsu0.cp v25.3 i21
lsu0.sto.64.l v29 i17 0  || lsu1.sto.64.h v29 i17 8 || iau.add i17 i17 16 || cmu.cpiv.x32 v25.2 i20 
lsu0.sto.64.l v30 i17 0  || lsu1.sto.64.h v30 i17 8 || iau.add i17 i17 16 || cmu.cpvi.x32 i20 v3.2

//seventh set
lsu0.cp i21 v3.3 || cmu.cpvv v26 v11  || lsu1.cp i23 v24.1 
cmu.cpiv.x32 v26.0 i20 || lsu0.cp v26.1 i21 || lsu1.cp i22 v24.0
lsu0.ldo.64.l v16 i0 0  || lsu1.ldo.64.h v16 i0 8   || cmu.cpvv v27 v7
lsu0.ldo.64.l v17 i0 16  || lsu1.ldo.64.h v17 i0 24 || cmu.cpiv.x32 v27.2 i22 
lsu0.ldo.64.l v18 i0 32  || lsu1.ldo.64.h v18 i0 40 || iau.add i0 i0 16 || cmu.cpiv.x32 v27.3 i23
lsu0.ldo.64.l v19 i5 16  || lsu1.ldo.64.h v19 i5 24 || cmu.cpvi.x32 i22 v7.2
lsu0.sto.64.l v25 i17 0  || lsu1.sto.64.h v25 i17 8 || iau.add i17 i17 16 || cmu.cpvi.x32 i23 v7.3
lsu0.sto.64.l v26 i17 0  || lsu1.sto.64.h v26 i17 8 || iau.add i17 i17 16 || cmu.cpvv v28 v24

//eight set
cmu.cpiv.x32 v28.0 i22 
censusTransform11x11u8_loop:
cmu.cpiv.x32 v28.1 i23 
lsu0.sto.64.l v27 i17 0  || lsu1.sto.64.h v27 i17 8 || iau.add i17 i17 16
lsu0.sto.64.l v28 i17 0  || lsu1.sto.64.h v28 i17 8 || iau.add i17 i17 16 
cmu.cpzv v0 3
cmu.cpzv v4 3
cmu.cpzv v8 3
cmu.cpzv v12 3



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
	LSU0.LD.32  i30  i19 || IAU.ADD i19 i19 4
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
.size mvcvCensusTransform11x11u8_asm,.-mvcvCensusTransform11x11u8_asm
.end
