/// @file
/// @copyright Three code copyright Movidius Ltd 2015, Three rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///
    
.type mvcvCensusMinConfidence64Subpixel_asm,@function
.version 00.51.04

.data .data.CensusMinConfidence64Subpixel
.align 4

.code .text.CensusMinConfidence64Subpixel

.lalign
//void mvcvCensusMinConfidence64Subpixel(u8 *in, half *out, u8 *outm, u32 width)
//                                          i18    i17        i16        i15
mvcvCensusMinConfidence64Subpixel_asm:

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

lsu0.ldil i0 0xffff
cmu.cpivr.x16 v4 i0
cmu.cpivr.x16 v13 i0

lsu0.ldil i14 .LmvcvCensusMinConfidence64Subpixel_loop || lsu1.ldih i14 .LmvcvCensusMinConfidence64Subpixel_loop
lsu0.ldil i1 0x0100 || lsu1.ldih i1 0x0302
lsu0.ldil i2 0x0504 || lsu1.ldih i2 0x0706 
lsu0.ldil i3 0x0908 || lsu1.ldih i3 0x0B0A
lsu0.ldil i4 0x0D0C || lsu1.ldih i4 0x0F0E
lsu0.ldil i5 0x10


cmu.cpivr.x8 v23 i5

cmu.cpiv.x32 v24.0 i1
cmu.cpiv.x32 v24.1 i2
cmu.cpiv.x32 v24.2 i3
cmu.cpiv.x32 v24.3 i4

vau.add.i8 v25 v24 v23
nop 2
vau.add.i8 v26 v25 v23
nop 2
vau.add.i8 v27 v26 v23
nop 2
lsu0.ldil i0 0xff
lsu0.ldil i1 0x3f || lsu1.ldih i1 0x0




lsu0.ldo.64.l v0 i18 0 || lsu1.ldo.64.h v0 i18 8 
lsu0.ldo.64.l v1 i18 16 || lsu1.ldo.64.h v1 i18 24
lsu0.ldo.64.l v2 i18 32 || lsu1.ldo.64.h v2 i18 40 || cmu.cpzv v14 3
lsu0.ldo.64.l v3 i18 48 || lsu1.ldo.64.h v3 i18 56  || iau.add i18 i18 64
nop 5


cmu.cmvv.u8 v0 v1 
peu.pvl08 lte || lsu0.cp v14 v0 
peu.pvl08  gt || lsu0.cp v14 v1  || cmu.alignvec v5 v4 v0 15
peu.pvl08 lte || lsu0.cp v15 v5  || cmu.alignvec v7 v0 v1 15
peu.pvl08  gt || lsu0.cp v15 v7  || cmu.alignvec v6 v0 v1 1
peu.pvl08 lte || lsu0.cp v16 v6  || cmu.alignvec v8 v1 v2 1
peu.pvl08  gt || lsu0.cp v16 v8
peu.pvl08 lte || lsu0.cp v17 v24
peu.pvl08  gt || lsu0.cp v17 v25 || cmu.cmvv.u8 v2 v3
peu.pvl08 lte || lsu0.cp v18 v2 
peu.pvl08  gt || lsu0.cp v18 v3  || cmu.alignvec  v9 v1 v2 15
peu.pvl08 lte || lsu0.cp v19 v9  || cmu.alignvec v11 v2 v3 15
peu.pvl08  gt || lsu0.cp v19 v11 || cmu.alignvec v10 v2 v3 1
peu.pvl08 lte || lsu0.cp v20 v10 || cmu.alignvec v12 v3 v4 1
peu.pvl08  gt || lsu0.cp v20 v12 
peu.pvl08 lte || lsu0.cp v21 v26
peu.pvl08  gt || lsu0.cp v21 v27 || cmu.cmvv.u8 v14 v18
peu.pvl08 lte || lsu0.cp v28 v14 
peu.pvl08  gt || lsu0.cp v28 v18 || cmu.cpvi.x16 i12.l v0.0 
peu.pvl08 lte || lsu0.cp v29 v15 || cmu.cpvi.x16 i13.l v3.7 
peu.pvl08  gt || lsu0.cp v29 v19 || iau.shr.u32 i13 i13 8
peu.pvl08 lte || lsu0.cp v30 v16 || iau.and i12 i12 i0
peu.pvl08  gt || lsu0.cp v30 v20 || iau.and i13 i13 i0
peu.pvl08 lte || lsu0.cp v31 v17 
peu.pvl08  gt || lsu0.cp v31 v21 

cmu.alignvec v14 v28 v28 8 || iau.xor i4 i4 i4 
cmu.min.u8 v15 v14 v28     || iau.xor i5 i5 i5
nop
cmu.alignvec v14 v15 v15 4 || iau.xor i6 i6 i6
cmu.min.u8 v16 v14 v15 || bru.rpl i14 i15 
nop
cmu.cpvv.u8.u16 v14 v16 || lsu0.cp i7 v16.0
cmu.cpvi.x16 i4.l v14.1 || iau.and i8 i7 i0
cmu.cpvi.x16 i5.l v14.2 || iau.minu i4 i4 i8
cmu.cpvi.x16 i6.l v14.3 || iau.minu i5 i4 i5
                           iau.minu i2 i5 i6 
//minimum found !!!!!!
cmu.cpivr.x8 v17 i2 //lots of minimums
cmu.cmvv.u8 v17 v28
peu.pvl08 neq || lsu0.cp v31 v13  

//minimum positions
//find minimum position
cmu.alignvec v14 v31 v31 8
cmu.min.u8 v15 v14 v31 
iau.shl i7 i2 1   //f1 * 2 //second mincost
cmu.alignvec v14 v15 v15 4
cmu.min.u8 v16 v14 v15
nop
cmu.alignvec v14 v16 v16 2
cmu.min.u8 v15 v14 v16
nop
cmu.alignvec v14 v15 v15 1
cmu.min.u8 v16 v14 v15
nop
cmu.cpvi.x16 i3.l v16.0 //minimum position


cmu.cmvv.u8 v5 v0 || iau.and i3 i3 i0
peu.pvl08 lte || lsu0.cp v0 v13 || cmu.cmvv.u8 v6 v0
peu.pvl08 lte || lsu0.cp v0 v13 || cmu.cmvv.u8 v0 v17
peu.pvl08 lte || lsu0.cp v0 v13 || cmu.cmvv.u8 v7 v1 
peu.pvl08 lte || lsu0.cp v1 v13 || cmu.cmvv.u8 v8 v1
peu.pvl08 lte || lsu0.cp v1 v13 || cmu.cmvv.u8 v1 v17
peu.pvl08 lte || lsu0.cp v1 v13 || cmu.cmvv.u8  v9 v2
peu.pvl08 lte || lsu0.cp v2 v13 || cmu.cmvv.u8 v10 v2
peu.pvl08 lte || lsu0.cp v2 v13 || cmu.cmvv.u8 v2 v17
peu.pvl08 lte || lsu0.cp v2 v13 || cmu.cmvv.u8 v11 v3
peu.pvl08 lte || lsu0.cp v3 v13 || cmu.cmvv.u8 v12 v3
peu.pvl08 lte || lsu0.cp v3 v13 || cmu.cmvv.u8 v3 v17
peu.pvl08 lte || lsu0.cp v3 v13 || cmu.min.u8 v0 v0 v1

cmu.min.u8 v2 v2 v3
nop 
cmu.min.u8 v0 v0 v2
nop 


cmu.cmvv.u8 v16 v31
peu.pvv8 neq || vau.xor v29 v29 v29
peu.pvv8 neq || vau.xor v30 v30 v30 || sau.sumx.u8 i4 v29 // precost
sau.sumx.u8 i5 v30 // postcost
cmu.cmz.i32 i3
peu.pc1c eq || cmu.cpii i4 i12
cmu.cmii.i32 i3 i1 
peu.pc1c eq || cmu.cpii i5 i13
//find second mincost
cmu.alignvec v14 v0 v0 8 || iau.add i8 i4 i5 || lsu0.ldo.64.l v0 i18 0 || lsu1.ldo.64.h v0 i18 8  // f0 + f2
cmu.min.u8 v15 v14 v0    || iau.sub i8 i8 i7 || lsu0.ldo.64.l v1 i18 16 || lsu1.ldo.64.h v1 i18 24   //check this if it's 0
iau.shl i9 i8 1    || lsu0.ldo.64.l v2 i18 32 || lsu1.ldo.64.h v2 i18 40//denom
cmu.alignvec v14 v15 v15 4 || iau.sub i4 i4 i5 || lsu0.ldo.64.l v3 i18 48 || lsu1.ldo.64.h v3 i18 56
cmu.min.u8 v16 v14 v15 || iau.add i18 i18 64
nop
cmu.cpvv.u8.u16 v14 v16 || lsu0.cp i10 v16.0
cmu.cpvi.x16 i6.l v14.1 || iau.and i10 i10 i0
cmu.cpvi.x16 i6.l v14.2 || iau.minu i10 i10 i6
cmu.cpvi.x16 i6.l v14.3 || iau.minu i10 i10 i6
                           iau.minu i6 i10 i6  || cmu.cpii.i32.f32 i9 i9//number
cmu.cpii.i32.f32 i4 i4
nop
sau.div.f32 i9 i4 i9 || cmu.cpzv v14 3//x
cmu.cmvv.u8 v0 v1 
peu.pvl08 lte || lsu0.cp v14 v0 
peu.pvl08  gt || lsu0.cp v14 v1  || cmu.alignvec v5 v4 v0 15
peu.pvl08 lte || lsu0.cp v15 v5  || cmu.alignvec v7 v0 v1 15
peu.pvl08  gt || lsu0.cp v15 v7  || cmu.alignvec v6 v0 v1 1
peu.pvl08 lte || lsu0.cp v16 v6  || cmu.alignvec v8 v1 v2 1
peu.pvl08  gt || lsu0.cp v16 v8
peu.pvl08 lte || lsu0.cp v17 v24
peu.pvl08  gt || lsu0.cp v17 v25 || cmu.cmvv.u8 v2 v3
lsu0.ldil i4 0x0000 || lsu1.ldih i4 0xbf00 
lsu0.ldil i5 0x0000 || lsu1.ldih i5 0x3f00
cmu.clampab.f32 i9 i4 i5 || peu.pvl08 lte || lsu0.cp v18 v2 
nop 1
cmu.cmz.i32 i8
peu.pc1c eq || iau.xor i9 i9 i9
cmu.cmvv.u8 v2 v3
cmu.cpii.i32.f32 i3 i3
nop 1
sau.add.f32 i3 i3 i9 || peu.pvl08  gt || lsu0.cp v18 v3  || cmu.alignvec  v9 v1 v2 15
peu.pvl08 lte || lsu0.cp v19 v9  || cmu.alignvec v11 v2 v3 15
peu.pvl08  gt || lsu0.cp v19 v11 || cmu.alignvec v10 v2 v3 1
cmu.cpii.f32.f16 i3 i3 //minpos
nop
lsu0.ldil i9 0x0000 || lsu1.ldih i9 0x437f || cmu.cpii.i32.f32 i2 i2//255
cmu.cpii.i32.f32 i6 i6 || lsu0.sti.16 i3 i17
sau.mul.f32 i2 i2 i9

peu.pvl08 lte || lsu0.cp v20 v10 || cmu.alignvec v12 v3 v4 1
peu.pvl08  gt || lsu0.cp v20 v12
sau.div.f32 i9 i2 i6 || peu.pvl08 lte || lsu0.cp v21 v26
peu.pvl08  gt || lsu0.cp v21 v27 || cmu.cmvv.u8 v14 v18
peu.pvl08 lte || lsu0.cp v28 v14 
peu.pvl08  gt || lsu0.cp v28 v18 || cmu.cpvi.x16 i12.l v0.0 
peu.pvl08 lte || lsu0.cp v29 v15 || cmu.cpvi.x16 i13.l v3.7 
peu.pvl08  gt || lsu0.cp v29 v19 || iau.shr.u32 i13 i13 8
peu.pvl08 lte || lsu0.cp v30 v16 || iau.and i12 i12 i0
peu.pvl08  gt || lsu0.cp v30 v20 || iau.and i13 i13 i0
peu.pvl08 lte || lsu0.cp v31 v17 
.LmvcvCensusMinConfidence64Subpixel_loop:
peu.pvl08  gt || lsu0.cp v31 v21 
cmu.alignvec v14 v28 v28 8 || iau.xor i4 i4 i4
cmu.min.u8 v15 v14 v28     || iau.xor i5 i5 i5
cmu.cpii.f32.i32s i9 i9 //minval
nop 
lsu0.sti.8 i9 i16 || cmu.alignvec v12 v3 v4 1
cmu.alignvec v14 v15 v15 4 || iau.xor i6 i6 i6


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
.size mvcvCensusMinConfidence64Subpixel_asm,.-mvcvCensusMinConfidence64Subpixel_asm
.end
