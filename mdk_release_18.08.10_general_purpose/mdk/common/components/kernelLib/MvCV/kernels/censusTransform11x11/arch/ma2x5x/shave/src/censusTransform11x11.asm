.type mvcvCensusTransform11x11_asm,@function
.version 00.50.00

/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.data .data.censusTransform11x11
.align 4

.code .text.censusTransform11x11

//void mvcvCensusTransform11x11(u16 **in, u8 *out, u32 width)
//							      i18	     i17	   i16

mvcvCensusTransform11x11_asm:

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


lsu0.ldil i15 censusTransform11x11_loop  || lsu1.ldih i15 censusTransform11x11_loop	
lsu0.ldil i11 0x1 || lsu1.ldih i11 0x0
lsu0.ldo.32 i0 i18 0x0
lsu0.ldo.32 i1 i18 0x4
lsu0.ldo.32 i2 i18 0x8
lsu0.ldo.32 i3 i18 0xc
lsu0.ldo.32 i4 i18 0x10
lsu0.ldo.32 i5 i18 0x14
lsu0.ldo.32 i6 i18 0x18
lsu0.ldo.32 i7 i18 0x1c
lsu0.ldo.32 i8 i18 0x20
lsu0.ldo.32 i9 i18 0x24
lsu0.ldo.32 i10 i18 0x28
cmu.cpivr.x16 v22 i11
iau.shr.u32 i16 i16 3
iau.sub i0 i0 16
iau.sub i1 i1 16
iau.sub i2 i2 16
iau.sub i3 i3 16
iau.sub i4 i4 16
iau.sub i5 i5 16
iau.sub i6 i6 16
iau.sub i7 i7 16
iau.sub i8 i8 16
iau.sub i9 i9 16
iau.sub i10 i10 16

lsu0.ldo.64.l v0 i0 0   || lsu1.ldo.64.h v0 i0 8    || iau.add i0 i0 16
lsu0.ldo.64.l v1 i1 0   || lsu1.ldo.64.h v1 i1 8    || iau.add i1 i1 16
lsu0.ldo.64.l v2 i2 0   || lsu1.ldo.64.h v2 i2 8    || iau.add i2 i2 16
lsu0.ldo.64.l v3 i3 0   || lsu1.ldo.64.h v3 i3 8    || iau.add i3 i3 16
lsu0.ldo.64.l v4 i4 0   || lsu1.ldo.64.h v4 i4 8    || iau.add i4 i4 16
lsu0.ldo.64.l v5 i5 0   || lsu1.ldo.64.h v5 i5 8    || iau.add i5 i5 16
lsu0.ldo.64.l v6 i6 0   || lsu1.ldo.64.h v6 i6 8    || iau.add i6 i6 16
lsu0.ldo.64.l v7 i7 0   || lsu1.ldo.64.h v7 i7 8    || iau.add i7 i7 16
lsu0.ldo.64.l v8 i8 0   || lsu1.ldo.64.h v8 i8 8    || iau.add i8 i8 16
lsu0.ldo.64.l v9 i9 0   || lsu1.ldo.64.h v9 i9 8    || iau.add i9 i9 16
lsu0.ldo.64.l v10 i10 0 || lsu1.ldo.64.h v10 i10 8  || iau.add i10 i10 16  

lsu0.ldo.64.l v11 i0 0  || lsu1.ldo.64.h v11 i0 8   || iau.add i0 i0 16
lsu0.ldo.64.l v12 i1 0  || lsu1.ldo.64.h v12 i1 8   || iau.add i1 i1 16
lsu0.ldo.64.l v13 i2 0  || lsu1.ldo.64.h v13 i2 8   || iau.add i2 i2 16
lsu0.ldo.64.l v14 i3 0  || lsu1.ldo.64.h v14 i3 8   || iau.add i3 i3 16
lsu0.ldo.64.l v15 i4 0  || lsu1.ldo.64.h v15 i4 8   || iau.add i4 i4 16
lsu0.ldo.64.l v16 i5 0  || lsu1.ldo.64.h v16 i5 8   || iau.add i5 i5 16
lsu0.ldo.64.l v17 i6 0  || lsu1.ldo.64.h v17 i6 8   || iau.add i6 i6 16
lsu0.ldo.64.l v18 i7 0  || lsu1.ldo.64.h v18 i7 8   || iau.add i7 i7 16
lsu0.ldo.64.l v19 i8 0  || lsu1.ldo.64.h v19 i8 8   || iau.add i8 i8 16
lsu0.ldo.64.l v20 i9 0  || lsu1.ldo.64.h v20 i9 8   || iau.add i9 i9 16 
lsu0.ldo.64.l v21 i10 0 || lsu1.ldo.64.h v21 i10 8  || iau.add i10 i10 16 
nop
//Disable peu.pvv warning since we use this on purpose
.nowarn 22
cmu.cpvi.x16 i16.l v16.0 || bru.rpl i15 i16 
cmu.cpivr.x16 v25 i16
cmu.alignvec v23 v0 v11 6 || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23 
peu.pvv16 GT || vau.or v24 v24 v22                 || cmu.alignvec v26 v0 v11 12
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]        || cmu.alignvec v27 v1 v12 6
cmu.vnz.x16 i12 v28 0                              || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 10
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                 || cmu.alignvec v26 v1 v12 12
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]        || cmu.alignvec v27 v2 v13 6
cmu.vnz.x16 i12 v28 1                              || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 4
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                 || cmu.alignvec v23 v2 v13 10
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i12 v28 2                              || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                 || cmu.alignvec v26 v2 v13 12
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]        || cmu.alignvec v27 v3 v14 6
cmu.vnz.x16 i12 v28 3                              || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 14                        || lsu0.sti.32 i12 i17
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                 || cmu.alignvec v26 v3 v14 12
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]        || cmu.alignvec v27 v4 v15 6
cmu.vnz.x16 i13 v28 0                              || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 8
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                 || cmu.alignvec v26 v4 v15 12
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]        || cmu.alignvec v27 v5 v16 6
cmu.vnz.x16 i13 v28 1                              || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 2
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                 || cmu.alignvec v23 v5 v16 8
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i13 v28 2                              || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                 || cmu.alignvec v26 v5 v16 12
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]        || cmu.alignvec v27 v6 v17 6
cmu.vnz.x16 i13 v28 3                              || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 12                        || lsu0.sti.32 i13 i17
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                 || cmu.alignvec v26 v6 v17 12
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]        || cmu.alignvec v27 v7 v18 6
cmu.vnz.x16 i14 v28 0                              || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 6
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                 || cmu.alignvec v23 v7 v18 12
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i14 v28 1                              || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                 || cmu.alignvec v23 v8 v19 6
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i14 v28 2                              || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                 || cmu.alignvec v26 v8 v19 12
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i14 v28 3                              || vau.xor v24 v24 v24
cmu.alignvec v27 v9 v20 6                          || lsu0.sti.32 i14 i17
cmu.alignvec v23 v26 v27 10
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                 || cmu.alignvec v26 v9 v20 12
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]        || cmu.alignvec v27 v10 v21 6
cmu.vnz.x16 i14 v28 0                              || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 4
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                 || cmu.alignvec v23 v10 v21 10
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i14 v28 1                              || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                 || cmu.cpvi.x16 i12.l v21.5
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i14 v28 2                              || vau.xor v24 v24 v24
cmu.cmii.u16 i16 i12   
peu.pc1c GT || cmu.shliv.x16 v24 v24 i11  
cmu.vnz.x16 i14 v24 3


cmu.cpvi.x16 i16.l v16.1                           || lsu0.sti.32 i14 i17
cmu.cpivr.x16 v25 i16
cmu.alignvec v23 v0 v11 8 || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23 
peu.pvv16 GT || vau.or v24 v24 v22                 || cmu.alignvec v26 v0 v11 14
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]        || cmu.alignvec v27 v1 v12 8
cmu.vnz.x16 i12 v28 0                              || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 10
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                 || cmu.alignvec v26 v1 v12 14
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]        || cmu.alignvec v27 v2 v13 8
cmu.vnz.x16 i12 v28 1                              || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 4
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                 || cmu.alignvec v23 v2 v13 12
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i12 v28 2                              || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                 || cmu.alignvec v26 v2 v13 14
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]        || cmu.alignvec v27 v3 v14 8
cmu.vnz.x16 i12 v28 3                              || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 14                        || lsu0.sti.32 i12 i17 
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v3 v14 14
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v4 v15 8
cmu.vnz.x16 i13 v28 0                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 8
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v4 v15 14
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v5 v16 8
cmu.vnz.x16 i13 v28 1                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 2
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v23 v5 v16 10
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i13 v28 2                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v5 v16 14
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v6 v17 8
cmu.vnz.x16 i13 v28 3                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 12                       || lsu0.sti.32 i13 i17 
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v6 v17 14
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v7 v18 8
cmu.vnz.x16 i14 v28 0                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 6
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v23 v7 v18 14
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i14 v28 1                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v23 v8 v19 8
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i14 v28 2                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v8 v19 14
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v9 v20 8
cmu.vnz.x16 i14 v28 3                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 10                       || lsu0.sti.32 i14 i17 
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v9 v20 14
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v10 v21 8
cmu.vnz.x16 i14 v28 0                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 4
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v23 v10 v21 12
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i14 v28 1                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.cpvi.x16 i12.l v21.6
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i14 v28 2                             || vau.xor v24 v24 v24
cmu.cmii.u16 i16 i12   
peu.pc1c GT || cmu.shliv.x16 v24 v24 i11          
cmu.vnz.x16 i14 v24 3


cmu.cpvi.x16 i16.l v16.2                          || lsu0.sti.32 i14 i17 
cmu.cpivr.x16 v25 i16                            
cmu.alignvec v23 v0 v11 10 || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23 
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v27 v1 v12 10
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v23 v11 v27 10
cmu.vnz.x16 i12 v28 0                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v27 v2 v13 10
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v23 v12 v27 4
cmu.vnz.x16 i12 v28 1                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v23 v2 v13 14
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i12 v28 2                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v27 v3 v14 10
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v23 v13 v27 14
cmu.vnz.x16 i12 v28 3                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23                              || lsu0.sti.32 i12 i17 
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v27 v4 v15 10
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v23 v14 v27 8
cmu.vnz.x16 i13 v28 0                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v27 v5 v16 10
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v23 v15 v27 2
cmu.vnz.x16 i13 v28 1                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22   
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v23 v5 v16 12
cmu.vnz.x16 i13 v28 2                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v27 v6 v17 10
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v23 v16 v27 12
cmu.vnz.x16 i13 v28 3                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23                              || lsu0.sti.32 i13 i17                    
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v27 v7 v18 10
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v23 v17 v27 6
cmu.vnz.x16 i14 v28 0                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23                              || lsu0.ldo.64.l v29 i0 0   || lsu1.ldo.64.h v29 i0 8 
peu.pvv16 GT || vau.or v24 v24 v22                || lsu0.ldo.64.l v30 i1 0   || lsu1.ldo.64.h v30 i1 8 
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i14 v28 1                             ||  vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v18                              || lsu0.ldo.64.l v31 i2 0   || lsu1.ldo.64.h v31 i2 8   
peu.pvv16 GT || vau.or v24 v24 v22   
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v23 v8 v19 10
cmu.vnz.x16 i14 v28 2                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v27 v9 v20 10
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v23 v19 v27 10
cmu.vnz.x16 i14 v28 3                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23                              || lsu0.sti.32 i14 i17
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v27 v10 v21 10
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v23 v20 v27 4
cmu.vnz.x16 i14 v28 0                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v23 v10 v21 14
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i14 v28 1                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.cpvi.x16 i12.l v21.7
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i14 v28 2                             || vau.xor v24 v24 v24
cmu.cmii.u16 i16 i12   
peu.pc1c GT || cmu.shliv.x16 v24 v24 i11  
cmu.vnz.x16 i14 v24 3


cmu.cpvi.x16 i16.l v16.3                          || lsu0.sti.32 i14 i17
cmu.cpivr.x16 v25 i16
cmu.alignvec v23 v0 v11 12 || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v11 v29 2
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v1 v12 12
cmu.vnz.x16 i12 v28 0                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 10
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v12 v30 2 
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v2 v13 12
cmu.vnz.x16 i12 v28 1                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 4                        || lsu0.ldo.64.l v29 i3 0   || lsu1.ldo.64.h v29 i3 8  
cmu.cmvv.u16 v25 v23                              || lsu0.ldo.64.l v30 i4 0   || lsu1.ldo.64.h v30 i4 8 
peu.pvv16 GT || vau.or v24 v24 v22
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i12 v28 2
cmu.cmvv.u16 v25 v13  || vau.xor v24 v24 v24
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v13 v31 2
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v3 v14 12 
cmu.vnz.x16 i12 v28 3                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 14                       || lsu0.ldo.64.l v31 i5 0   || lsu1.ldo.64.h v31 i5 8    
cmu.cmvv.u16 v25 v23                              || lsu0.sti.32 i12 i17 
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v14 v29 2
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v4 v15 12
cmu.vnz.x16 i13 v28 0                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 8                        || lsu0.ldo.64.l v29 i6 0   || lsu1.ldo.64.h v29 i6 8    
cmu.cmvv.u16 v25 v23                              || lsu0.ldo.64.l v30 i7 0   || lsu1.ldo.64.h v30 i7 8 
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v15 v30 2
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v5 v16 12
cmu.vnz.x16 i13 v28 1                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 2
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v23 v5 v16 14
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i13 v28 2                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23                              || lsu0.ldo.64.l v31 i8 0   || lsu1.ldo.64.h v31 i8 8 
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v16 v31 2
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v6 v17 12
cmu.vnz.x16 i13 v28 3                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 12                       || lsu0.sti.32 i13 i17 
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v17 v29 2
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v7 v18 12
cmu.vnz.x16 i14 v28 0                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 6
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v23 v18 v30 2
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i14 v28 1                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23                              || lsu0.ldo.64.l v29 i9 0    || lsu1.ldo.64.h v29 i9 8  
peu.pvv16 GT || vau.or v24 v24 v22                || lsu0.ldo.64.l v30 i10 0   || lsu1.ldo.64.h v30 i10 8  
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v23 v8 v19 12
cmu.vnz.x16 i14 v28 2                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v19 v31 2
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v9 v20 12
cmu.vnz.x16 i14 v28 3                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 10                       || lsu0.sti.32 i14 i17 
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v20 v29 2
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v10 v21 12
cmu.vnz.x16 i14 v28 0                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 4                        || lsu0.ldo.64.l v29 i0 0   || lsu1.ldo.64.h v29 i0 8  
cmu.cmvv.u16 v25 v23                              || lsu0.ldo.64.l v30 i1 0   || lsu1.ldo.64.h v30 i1 8 
peu.pvv16 GT || vau.or v24 v24 v22                || lsu0.ldo.64.l v31 i2 0   || lsu1.ldo.64.h v31 i2 8    
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i14 v28 1
cmu.cmvv.u16 v25 v21|| vau.xor v24 v24 v24
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.cpvi.x16 i12.l v30.0
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i14 v28 2                             || vau.xor v24 v24 v24
cmu.cmii.u16 i16 i12   
peu.pc1c GT || cmu.shliv.x16 v24 v24 i11   //16
cmu.vnz.x16 i14 v24 3


cmu.cpvi.x16 i16.l v16.4                          || lsu0.sti.32 i14 i17 
cmu.cpivr.x16 v25 i16
cmu.alignvec v23 v0 v11 14 || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v11 v29 4
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v1 v12 14
cmu.vnz.x16 i12 v28 0                             || vau.xor v24 v24 v24        || lsu1.cp v0 v11
cmu.alignvec v23 v26 v27 10                       || lsu0.ldo.64.l v11 i0 0     || lsu1.ldo.64.h v11 i0 8 || iau.add i0 i0 16
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v12 v30 4
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v2 v13 14 
cmu.vnz.x16 i12 v28 1                             || vau.xor v24 v24 v24        || lsu1.cp v1 v12
cmu.alignvec v23 v26 v27 4                        || lsu0.ldo.64.l v29 i3 0     || lsu1.ldo.64.h v29 i3 8
cmu.cmvv.u16 v25 v23                              || lsu0.ldo.64.l v30 i4 0     || lsu1.ldo.64.h v30 i4 8
peu.pvv16 GT || vau.or v24 v24 v22                || lsu0.ldo.64.l v31 i5 0     || lsu1.ldo.64.h v31 i5 8 
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v23 v13 v31 2
cmu.vnz.x16 i12 v28 2                             || lsu0.ldo.64.l v12 i1 0     || lsu1.ldo.64.h v12 i1 8 || iau.add i1 i1 16
cmu.cmvv.u16 v25 v23  || vau.xor v24 v24 v24
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v13 v31 4
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v3 v14 14 
cmu.vnz.x16 i12 v28 3                             || vau.xor v24 v24 v24       
cmu.alignvec v23 v26 v27 14                       || lsu0.sti.32 i12 i17 
cmu.cmvv.u16 v25 v23 
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v14 v29 4
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v4 v15 14 || lsu1.cp v2 v13
cmu.vnz.x16 i13 v28 0                             || vau.xor v24 v24 v24        || lsu1.cp v3 v14
cmu.alignvec v23 v26 v27 8                        || lsu0.ldo.64.l v13 i2 0     || lsu1.ldo.64.h v13 i2 8 || iau.add i2 i2 16
cmu.cmvv.u16 v25 v23                              || lsu0.ldo.64.l v14 i3 0     || lsu1.ldo.64.h v14 i3 8 || iau.add i3 i3 16
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v15 v30 4 || lsu1.cp v4 v15
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v5 v16 14 
cmu.vnz.x16 i13 v28 1                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 2                        || lsu0.ldo.64.l v29 i6 0     || lsu1.ldo.64.h v29 i6 8 
cmu.cmvv.u16 v25 v23                              || lsu0.ldo.64.l v30 i7 0     || lsu1.ldo.64.h v30 i7 8 
peu.pvv16 GT || vau.or v24 v24 v22                || lsu0.ldo.64.l v31 i8 0     || lsu1.ldo.64.h v31 i8 8  
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || lsu1.ldo.64.l v15 i4 0  
cmu.vnz.x16 i13 v28 2                             || vau.xor v24 v24 v24        || lsu1.ldo.64.h v15 i4 8 || iau.add i4 i4 16
cmu.cmvv.u16 v25 v16
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v16 v31 4 
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v6 v17 14
cmu.vnz.x16 i13 v28 3                             || vau.xor v24 v24 v24       
cmu.alignvec v23 v26 v27 12                       || lsu0.sti.32 i13 i17
cmu.cmvv.u16 v25 v23                              || lsu0.ldo.64.l v16 i5 0     || lsu1.ldo.64.h v16 i5 8 || iau.add i5 i5 16
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v17 v29 4 || lsu1.cp v6 v17
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v7 v18 14 || lsu1.cp v5 v16
cmu.vnz.x16 i14 v28 0                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 6                        || lsu0.ldo.64.l v29 i9 0     || lsu1.ldo.64.h v29 i9 8
cmu.cmvv.u16 v25 v23                              || lsu0.ldo.64.l v30 i10 0    || lsu1.ldo.64.h v30 i10 8
peu.pvv16 GT || vau.or v24 v24 v22                || lsu0.ldo.64.l v17 i6 0     || lsu1.ldo.64.h v17 i6 8 || iau.add i6 i6 16
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v23 v18 v30 4 || lsu1.cp v7 v18
cmu.vnz.x16 i14 v28 1                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23                              || lsu0.ldo.64.l v18 i7 0     || lsu1.ldo.64.h v18 i7 8 || iau.add i7 i7 16
peu.pvv16 GT || vau.or v24 v24 v22
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v23 v8 v19 14
cmu.vnz.x16 i14 v28 2                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23                              
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v19 v31 4  || lsu1.cp v8 v19
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v9 v20 14
cmu.vnz.x16 i14 v28 3                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 10                       || lsu0.sti.32 i14 i17 
cmu.cmvv.u16 v25 v23                              || lsu0.ldo.64.l v19 i8 0      || lsu1.ldo.64.h v19 i8 8 || iau.add i8 i8 16
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v20 v29 4  || lsu1.cp v9 v20
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v10 v21 14
cmu.vnz.x16 i14 v28 0                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 4                        || lsu0.ldo.64.l v20 i9 0      || lsu1.ldo.64.h v20 i9 8 || iau.add i9 i9 16 
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v23 v21 v30 2  || lsu1.cp v10 v21
cmu.vnz.x16 i14 v28 1                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23                              || lsu0.ldo.64.l v21 i10 0     || lsu1.ldo.64.h v21 i10 8|| iau.add i10 i10 16 
peu.pvv16 GT || vau.or v24 v24 v22
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.cpvi.x16 i12.l v30.1
cmu.vnz.x16 i14 v28 2                             || vau.xor v24 v24 v24        
cmu.cmii.u16 i16 i12                              
peu.pc1c GT || cmu.shliv.x16 v24 v24 i11         
cmu.vnz.x16 i14 v24 3


cmu.cpvi.x16 i16.l v5.5                           || lsu0.sti.32 i14 i17 
cmu.cpivr.x16 v25 i16                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v0
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v0 v11 6
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v23 v26 v1 10
cmu.vnz.x16 i12 v28 0                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v1 v12 6
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v23 v26 v2 4
cmu.vnz.x16 i12 v28 1                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v23 v2 v13 4
cmu.vnz.x16 i12 v28 2                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23 
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v2 v13 6
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v23 v26 v3 14
cmu.vnz.x16 i12 v28 3                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23                              || lsu0.sti.32 i12 i17 
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v3 v14 6
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v23 v26 v4 8
cmu.vnz.x16 i13 v28 0                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v4 v15 6
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v23 v26 v5 2
cmu.vnz.x16 i13 v28 1                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v23 v5 v16 2
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i13 v28 2                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v5 v16 6
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v23 v26 v6 12
cmu.vnz.x16 i13 v28 3                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23                              || lsu0.sti.32 i13 i17 
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v6 v17 6
vau.and v28 v24 v22 || lsu1.swzv8 [01234567]      || cmu.alignvec v23 v26 v7 6
cmu.vnz.x16 i14 v28 0                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v23 v7 v18 6
vau.and v28 v24 v22 || lsu1.swzv8 [01234567]
cmu.vnz.x16 i14 v28 1                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22
vau.and v28 v24 v22 || lsu1.swzv8 [01234567]
cmu.vnz.x16 i14 v28 2
cmu.cmvv.u16 v25 v8||vau.xor v24 v24 v24
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v8 v19 6
vau.and v28 v24 v22 || lsu1.swzv8 [01234567]      || cmu.alignvec v23 v26 v9 10
cmu.vnz.x16 i14 v28 3                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23                              || lsu0.sti.32 i14 i17
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v9 v20 6
vau.and v28 v24 v22 || lsu1.swzv8 [01234567]      || cmu.alignvec v23 v26 v10 4
cmu.vnz.x16 i14 v28 0                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v23 v10 v21 4
vau.and v28 v24 v22 || lsu1.swzv8 [01234567]
cmu.vnz.x16 i14 v28 1                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22
vau.and v28 v24 v22 || lsu1.swzv8 [01234567]      || cmu.cpvi.x16 i12.l v21.2 
cmu.vnz.x16 i14 v28 2                             || vau.xor v24 v24 v24
cmu.cmii.u16 i16 i12   
peu.pc1c GT || cmu.shliv.x16 v24 v24 i11  
cmu.vnz.x16 i14 v24 3


cmu.cpvi.x16 i16.l v5.6                           || lsu0.sti.32 i14 i17
cmu.cpivr.x16 v25 i16
cmu.alignvec v23 v0 v11 2 || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v0 v11 8
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v1 v12 2
cmu.vnz.x16 i12 v28 0                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 10
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v1 v12 8
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v2 v13 2
cmu.vnz.x16 i12 v28 1                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 4
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v23 v2 v13 6
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i12 v28 2                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v2 v13 8
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v3 v14 2
cmu.vnz.x16 i12 v28 3                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 14                       || lsu0.sti.32 i12 i17
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v3 v14 8
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v4 v15 2
cmu.vnz.x16 i13 v28 0                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 8
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v4 v15 8
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v5 v16 2
cmu.vnz.x16 i13 v28 1                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 2
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v23 v5 v16 4
cmu.vnz.x16 i13 v28 2                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v5 v16 8
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v6 v17 2
cmu.vnz.x16 i13 v28 3                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 12                       || lsu0.sti.32 i13 i17
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v6 v17 8
vau.and v28 v24 v22 || lsu1.swzv8 [01234567]      || cmu.alignvec v27 v7 v18 2
cmu.vnz.x16 i14 v28 0                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 6
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v23 v7 v18 8
vau.and v28 v24 v22 || lsu1.swzv8 [01234567]
cmu.vnz.x16 i14 v28 1                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v23 v8 v19 2
vau.and v28 v24 v22 || lsu1.swzv8 [01234567]
cmu.vnz.x16 i14 v28 2                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v8 v19 8
vau.and v28 v24 v22 || lsu1.swzv8 [01234567]      || cmu.alignvec v27 v9 v20 2
cmu.vnz.x16 i14 v28 3                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 10                       || lsu0.sti.32 i14 i17 
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v9 v20 8
vau.and v28 v24 v22 || lsu1.swzv8 [01234567]      || cmu.alignvec v27 v10 v21 2
cmu.vnz.x16 i14 v28 0                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 4
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v23 v10 v21 6
vau.and v28 v24 v22 || lsu1.swzv8 [01234567]
cmu.vnz.x16 i14 v28 1                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22
vau.and v28 v24 v22 || lsu1.swzv8 [01234567]      || cmu.cpvi.x16 i12.l v21.3
cmu.vnz.x16 i14 v28 2                             || vau.xor v24 v24 v24 
cmu.cmii.u16 i16 i12   
peu.pc1c GT || cmu.shliv.x16 v24 v24 i11   
cmu.vnz.x16 i14 v24 3


cmu.cpvi.x16 i16.l v5.7                           || lsu0.sti.32 i14 i17 
cmu.cpivr.x16 v25 i16
cmu.alignvec v23 v0 v11 4 || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v0 v11 10
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v1 v12 4
cmu.vnz.x16 i12 v28 0                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 10
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v1 v12 10
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v2 v13 4
cmu.vnz.x16 i12 v28 1                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 4
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v23 v2 v13 8
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i12 v28 2                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v2 v13 10
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v3 v14 4
cmu.vnz.x16 i12 v28 3                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 14                       || lsu0.sti.32 i12 i17 
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v3 v14 10
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v4 v15 4
cmu.vnz.x16 i13 v28 0                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 8
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v4 v15 10
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v5 v16 4
cmu.vnz.x16 i13 v28 1                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 2
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v23 v5 v16 6
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]
cmu.vnz.x16 i13 v28 2                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v5 v16 10
vau.and v28 v24 v22|| lsu0.swzv8 [01234567]       || cmu.alignvec v27 v6 v17 4
cmu.vnz.x16 i13 v28 3                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 12                       || lsu0.sti.32 i13 i17 
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v6 v17 10
vau.and v28 v24 v22 || lsu1.swzv8 [01234567]      || cmu.alignvec v27 v7 v18 4
cmu.vnz.x16 i14 v28 0                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 6
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v23 v7 v18 10
vau.and v28 v24 v22 || lsu1.swzv8 [01234567]
cmu.vnz.x16 i14 v28 1                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22
vau.and v28 v24 v22 || lsu1.swzv8 [01234567]      || cmu.alignvec v23 v8 v19 4 
cmu.vnz.x16 i14 v28 2                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v8 v19 10
vau.and v28 v24 v22 || lsu1.swzv8 [01234567]      || cmu.alignvec v27 v9 v20 4
cmu.vnz.x16 i14 v28 3                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 10                       || lsu0.sti.32 i14 i17 
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v26 v9 v20 10
vau.and v28 v24 v22 || lsu1.swzv8 [01234567]      || cmu.alignvec v27 v10 v21 4
cmu.vnz.x16 i14 v28 0                             || vau.xor v24 v24 v24
cmu.alignvec v23 v26 v27 4
cmu.cmvv.u16 v25 v23
peu.pvv16 GT || vau.or v24 v24 v22                || cmu.alignvec v23 v10 v21 8
vau.and v28 v24 v22 || lsu1.swzv8 [01234567]
cmu.vnz.x16 i14 v28 1                             || vau.xor v24 v24 v24
cmu.cmvv.u16 v25 v23

censusTransform11x11_loop:
peu.pvv16 GT || vau.or v24 v24 v22
vau.and v28 v24 v22 || lsu1.swzv8 [01234567]     || cmu.cpvi.x16 i12.l v21.4
cmu.vnz.x16 i14 v28 2                            || vau.xor v24 v24 v24
cmu.cmii.u16 i16 i12   
peu.pc1c GT || cmu.shliv.x16 v24 v24 i11   
cmu.vnz.x16 i14 v24 3
lsu0.sti.32 i14 i17 

.nowarnend

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
	nop 6

bru.jmp i30
nop 6
.size mvcvCensusTransform11x11_asm,.-mvcvCensusTransform11x11_asm
.end
