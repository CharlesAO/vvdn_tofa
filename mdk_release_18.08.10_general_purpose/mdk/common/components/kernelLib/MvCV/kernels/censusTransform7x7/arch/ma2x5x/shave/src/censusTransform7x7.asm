.type mvcvCensusTransform7x7_asm,@function
.version 00.51.04

/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.data .data.censusTransform7x7
.align 4

.code .text.censusTransform7x7

//void mvcvCensusTransform7x7(u8 **in, u64 *out, u32 width)
//                                i18       i17      i16

mvcvCensusTransform7x7_asm:

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


lsu0.ldil i15,0x1  || lsu1.ldih i15,0x0
lsu0.ldil i14 censusTransform7x7_loop  || lsu1.ldih i14 censusTransform7x7_loop
lsu0.ldo.32 i0,i18,0x0 
lsu0.ldo.32 i1,i18,0x4 
lsu0.ldo.32 i2,i18,0x8 
lsu0.ldo.32 i3,i18,0xc 
lsu0.ldo.32 i4,i18,0x10 
lsu0.ldo.32 i5,i18,0x14
lsu0.ldo.32 i6,i18,0x18

iau.sub i0 i0 16
iau.sub i1 i1 16
iau.sub i2 i2 16
iau.sub i3 i3 16
iau.sub i4 i4 16
iau.sub i5 i5 16
iau.sub i6 i6 16
iau.shr.u32 i16 i16 4
cmu.cpivr.x8 v29 i15 


lsu0.ldo.64.l v1 i0 0x0  || lsu1.ldo.64.h v1 i0 0x8  || iau.add i0 i0 16 
lsu0.ldo.64.l v2 i1 0x0  || lsu1.ldo.64.h v2 i1 0x8  || iau.add i1 i1 16
lsu0.ldo.64.l v3 i2 0x0  || lsu1.ldo.64.h v3 i2 0x8  || iau.add i2 i2 16
lsu0.ldo.64.l v4 i3 0x0  || lsu1.ldo.64.h v4 i3 0x8  || iau.add i3 i3 16
lsu0.ldo.64.l v5 i4 0x0  || lsu1.ldo.64.h v5 i4 0x8  || iau.add i4 i4 16  
lsu0.ldo.64.l v6 i5 0x0  || lsu1.ldo.64.h v6 i5 0x8  || iau.add i5 i5 16 
lsu0.ldo.64.l v7 i6 0x0  || lsu1.ldo.64.h v7 i6 0x8  || iau.add i6 i6 16 

lsu0.ldo.64.l v8 i0 0x0  || lsu1.ldo.64.h v8  i0 0x8 || iau.add i0 i0 16
lsu0.ldo.64.l v9 i1 0x0  || lsu1.ldo.64.h v9  i1 0x8 || iau.add i1 i1 16
lsu0.ldo.64.l v10 i2 0x0 || lsu1.ldo.64.h v10 i2 0x8 || iau.add i2 i2 16                                                                                                   
lsu0.ldo.64.l v11 i3 0x0 || lsu1.ldo.64.h v11 i3 0x8 || iau.add i3 i3 16 
lsu0.ldo.64.l v12 i4 0x0 || lsu1.ldo.64.h v12 i4 0x8 || iau.add i4 i4 16
lsu0.ldo.64.l v13 i5 0x0 || lsu1.ldo.64.h v13 i5 0x8 || iau.add i5 i5 16
lsu0.ldo.64.l v14 i6 0x0 || lsu1.ldo.64.h v14 i6 0x8 || iau.add i6 i6 16

lsu0.ldo.64.l v15 i0 0x0 || lsu1.ldo.64.h v15 i0 0x8 || iau.add i0 i0 16 || vau.xor v22 v22 v22 || bru.rpl i14 i16 censusTransform7x7_endCensus7x7_delay
lsu0.ldo.64.l v16 i1 0x0 || lsu1.ldo.64.h v16 i1 0x8 || iau.add i1 i1 16 || vau.xor v23 v23 v23 
lsu0.ldo.64.l v17 i2 0x0 || lsu1.ldo.64.h v17 i2 0x8 || iau.add i2 i2 16 || vau.xor v24 v24 v24                                                                                                   
lsu0.ldo.64.l v18 i3 0x0 || lsu1.ldo.64.h v18 i3 0x8 || iau.add i3 i3 16 || vau.xor v25 v25 v25
lsu0.ldo.64.l v19 i4 0x0 || lsu1.ldo.64.h v19 i4 0x8 || iau.add i4 i4 16 || vau.xor v26 v26 v26
lsu0.ldo.64.l v20 i5 0x0 || lsu1.ldo.64.h v20 i5 0x8 || iau.add i5 i5 16 || vau.xor v27 v27 v27
lsu0.ldo.64.l v21 i6 0x0 || lsu1.ldo.64.h v21 i6 0x8 || iau.add i6 i6 16 || vau.xor v28 v28 v28 || cmu.cpvv v0 v11

cmu.alignvec v30 v1 v8 13 || vau.shl.x8 v22 v22 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v1 v8 14 || peu.pvv8 GT ||  vau.or v22 v22 v29 
vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v1 v8 15 || peu.pvv8 GT ||  vau.or v23 v23 v29
vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v23 v23 v29 
vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v8
cmu.alignvec v30 v8 v15 1 || peu.pvv8 GT ||  vau.or v23 v23 v29
vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v8 v15 2 || peu.pvv8 GT ||  vau.or v23 v23 v29
vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v8 v15 3 || peu.pvv8 GT ||  vau.or v23 v23 v29
vau.shl.x8 v23 v23 1      || cmu.cpvv v1 v8
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v23 v23 v29 || cmu.cpvv v8 v15

cmu.alignvec v30 v2 v9 13 || vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v2 v9 14 || peu.pvv8 GT ||  vau.or v23 v23 v29
vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v2 v9 15 || peu.pvv8 GT ||  vau.or v23 v23 v29
vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v24 v24 v29 
vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v9
cmu.alignvec v30 v9 v16 1 || peu.pvv8 GT ||  vau.or v24 v24 v29
vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v9 v16 2 || peu.pvv8 GT ||  vau.or v24 v24 v29
vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v9 v16 3 || peu.pvv8 GT ||  vau.or v24 v24 v29
vau.shl.x8 v24 v24 1      || cmu.cpvv v2 v9
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v24 v24 v29 || cmu.cpvv v9 v16

cmu.alignvec v30 v3 v10 13 || vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v3 v10 14 || peu.pvv8 GT ||  vau.or v24 v24 v29
vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v3 v10 15 || peu.pvv8 GT ||  vau.or v24 v24 v29
vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v24 v24 v29 
vau.shl.x8 v25 v25 1
cmu.cmvv.u8 v0 v10
cmu.alignvec v30 v10 v17 1 || peu.pvv8 GT ||  vau.or v25 v25 v29
vau.shl.x8 v25 v25 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v10 v17 2 || peu.pvv8 GT ||  vau.or v25 v25 v29
vau.shl.x8 v25 v25 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v10 v17 3 || peu.pvv8 GT ||  vau.or v25 v25 v29
vau.shl.x8 v25 v25 1       || cmu.cpvv v3 v10
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v25 v25 v29 || cmu.cpvv v10 v17

cmu.alignvec v30 v4 v11 13 || vau.shl.x8 v25 v25 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v4 v11 14 || peu.pvv8 GT ||  vau.or v25 v25 v29
vau.shl.x8 v25 v25 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v4 v11 15 || peu.pvv8 GT ||  vau.or v25 v25 v29
vau.shl.x8 v25 v25 1
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v25 v25 v29 
vau.shl.x8 v25 v25 1
cmu.cmvv.u8 v0 v11
cmu.alignvec v30 v11 v18 1 || peu.pvv8 GT ||  vau.or v25 v25 v29
vau.shl.x8 v26 v26 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v11 v18 2 || peu.pvv8 GT ||  vau.or v26 v26 v29
vau.shl.x8 v26 v26 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v11 v18 3 || peu.pvv8 GT ||  vau.or v26 v26 v29
vau.shl.x8 v26 v26 1       || cmu.cpvv v4 v11
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v26 v26 v29 || cmu.cpvv v11 v18

cmu.alignvec v30 v5 v12 13 || vau.shl.x8 v26 v26 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v5 v12 14 || peu.pvv8 GT ||  vau.or v26 v26 v29
vau.shl.x8 v26 v26 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v5 v12 15 || peu.pvv8 GT ||  vau.or v26 v26 v29
vau.shl.x8 v26 v26 1
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v26 v26 v29 
vau.shl.x8 v26 v26 1
cmu.cmvv.u8 v0 v12
cmu.alignvec v30 v12 v19 1 || peu.pvv8 GT ||  vau.or v26 v26 v29 
vau.shl.x8 v26 v26 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v12 v19 2 || peu.pvv8 GT ||  vau.or v26 v26 v29
vau.shl.x8 v27 v27 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v12 v19 3 || peu.pvv8 GT ||  vau.or v27 v27 v29
vau.shl.x8 v27 v27 1       || cmu.cpvv v5 v12
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v27 v27 v29 || cmu.cpvv v12 v19

cmu.alignvec v30 v6 v13 13 || vau.shl.x8 v27 v27 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v6 v13 14 || peu.pvv8 GT ||  vau.or v27 v27 v29
vau.shl.x8 v27 v27 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v6 v13 15 || peu.pvv8 GT ||  vau.or v27 v27 v29
vau.shl.x8 v27 v27 1
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v27 v27 v29 
vau.shl.x8 v27 v27 1
cmu.cmvv.u8 v0 v13
cmu.alignvec v30 v13 v20 1 || peu.pvv8 GT ||  vau.or v27 v27 v29
vau.shl.x8 v27 v27 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v13 v20 2 || peu.pvv8 GT ||  vau.or v27 v27 v29
vau.shl.x8 v27 v27 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v13 v20 3 || peu.pvv8 GT ||  vau.or v27 v27 v29 
vau.shl.x8 v28 v28 1       || cmu.cpvv v6 v13
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v28 v28 v29 || cmu.cpvv v13 v20

cmu.alignvec v30 v7 v14 13 || vau.shl.x8 v28 v28 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v7 v14 14 || peu.pvv8 GT ||  vau.or v28 v28 v29
vau.shl.x8 v28 v28 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v7 v14 15 || peu.pvv8 GT ||  vau.or v28 v28 v29
vau.shl.x8 v28 v28 1
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v28 v28 v29 
vau.shl.x8 v28 v28 1
cmu.cmvv.u8 v0 v14
cmu.alignvec v30 v14 v21 1 || peu.pvv8 GT ||  vau.or v28 v28 v29 
vau.shl.x8 v28 v28 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v14 v21 2 || peu.pvv8 GT ||  vau.or v28 v28 v29
vau.shl.x8 v28 v28 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v14 v21 3 || peu.pvv8 GT ||  vau.or v28 v28 v29
vau.shl.x8 v28 v28 1       || cmu.cpvv v7 v14
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v28 v28 v29 || cmu.cpvv v14 v21

vau.xor v30 v30 v30
cmu.vilv.x8 v31 v0 v30 v22
cmu.vilv.x8 v22 v30 v23 v24 
cmu.vilv.x16 v23 v24 v31 v22
cmu.vilv.x16 v31 v22 v0 v30
cmu.vilv.x8 v0 v30 v25 v26 
cmu.vilv.x8 v25 v26 v27 v28
cmu.vilv.x16 v27 v28 v0 v25
cmu.vilv.x16 v25 v0 v30 v26
cmu.vilv.x32 v30 v26 v23 v27
cmu.vilv.x32 v27 v23 v24 v28
cmu.vilv.x32 v24 v28 v31 v25
cmu.vilv.x32 v25 v31 v22 v0
lsu0.sto.64.l v30 i17 0x0  || lsu1.sto.64.h v30 i17 0x8
 censusTransform7x7_loop:                      
lsu0.sto.64.l v26 i17 0x10 || lsu1.sto.64.h v26 i17 0x18
lsu0.sto.64.l v27 i17 0x20 || lsu1.sto.64.h v27 i17 0x28
lsu0.sto.64.l v23 i17 0x30 || lsu1.sto.64.h v23 i17 0x38 
lsu0.sto.64.l v24 i17 0x40 || lsu1.sto.64.h v24 i17 0x48 
lsu0.sto.64.l v28 i17 0x50 || lsu1.sto.64.h v28 i17 0x58 
lsu0.sto.64.l v25 i17 0x60 || lsu1.sto.64.h v25 i17 0x68 
lsu0.sto.64.l v31 i17 0x70 || lsu1.sto.64.h v31 i17 0x78 || iau.add i17 i17 0x80
 censusTransform7x7_endCensus7x7_delay:

 

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
.size mvcvCensusTransform7x7_asm,.-mvcvCensusTransform7x7_asm
.end