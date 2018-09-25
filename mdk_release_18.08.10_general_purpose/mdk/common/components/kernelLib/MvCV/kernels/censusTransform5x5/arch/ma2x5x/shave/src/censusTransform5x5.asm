.type mvcvCensusTransform5x5_asm,@function
.version 00.50.00

/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.data .data.censusTransform5x5
.align 4

.code .text.censusTransform5x5

//void mvcvCensusTransform5x5(u8 **in, u32 *out, u32 width)
//							     i18	   i17		i16
mvcvCensusTransform5x5_asm:
.nowarn 10

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
	
	
	

lsu0.ldil i23,0x1  || lsu1.ldih i23,0x0
lsu0.ldil i21 censusTransform5x5_loop  || lsu1.ldih i21 censusTransform5x5_loop
lsu0.ldo.32 i0,i18,0x0 
lsu0.ldo.32 i1,i18,0x4 
lsu0.ldo.32 i2,i18,0x8 
lsu0.ldo.32 i3,i18,0xc 
lsu0.ldo.32 i4,i18,0x10 
nop 2
  
iau.sub i0 i0 16
iau.sub i1 i1 16
iau.sub i2 i2 16
iau.sub i3 i3 16
iau.sub i4 i4 16
iau.shr.u32 i16 i16 4
cmu.cpivr.x8 v21 i23 
vau.xor v22 v22 v22
vau.xor v23 v23 v23
vau.xor v24 v24 v24
vau.xor v25 v25 v25

lsu0.ldo.64.l v1 i0 0x0  || lsu1.ldo.64.h v1 i0 0x8   || iau.add i0 i0 16 
lsu0.ldo.64.l v2 i1 0x0  || lsu1.ldo.64.h v2 i1 0x8   || iau.add i1 i1 16
lsu0.ldo.64.l v3 i2 0x0  || lsu1.ldo.64.h v3 i2 0x8   || iau.add i2 i2 16
lsu0.ldo.64.l v4 i3 0x0  || lsu1.ldo.64.h v4 i3 0x8   || iau.add i3 i3 16
lsu0.ldo.64.l v5 i4 0x0  || lsu1.ldo.64.h v5 i4 0x8   || iau.add i4 i4 16  

lsu0.ldo.64.l v6 i0 0x0  || lsu1.ldo.64.h v6 i0 0x8   || iau.add i0 i0 16 
lsu0.ldo.64.l v7 i1 0x0  || lsu1.ldo.64.h v7 i1 0x8   || iau.add i1 i1 16 
lsu0.ldo.64.l v8 i2 0x0  || lsu1.ldo.64.h v8 i2 0x8   || iau.add i2 i2 16 
lsu0.ldo.64.l v9 i3 0x0  || lsu1.ldo.64.h v9 i3 0x8   || iau.add i3 i3 16 
lsu0.ldo.64.l v10 i4 0x0 || lsu1.ldo.64.h v10 i4 0x8  || iau.add i4 i4 16   
                                                                                                       
lsu0.ldo.64.l v11 i0 0x0 || lsu1.ldo.64.h v11 i0 0x8  || iau.add i0 i0 16  
lsu0.ldo.64.l v12 i1 0x0 || lsu1.ldo.64.h v12 i1 0x8  || iau.add i1 i1 16
lsu0.ldo.64.l v13 i2 0x0 || lsu1.ldo.64.h v13 i2 0x8  || iau.add i2 i2 16
lsu0.ldo.64.l v14 i3 0x0 || lsu1.ldo.64.h v14 i3 0x8  || iau.add i3 i3 16
lsu0.ldo.64.l v15 i4 0x0 || lsu1.ldo.64.h v15 i4 0x8  || iau.add i4 i4 16 
cmu.cpvv v0 v8 

cmu.alignvec v16 v1 v6 14 || vau.shl.x8 v22 v22 1
cmu.cmvv.u8 v0 v16 
peu.pvv8 GT ||  vau.or v22 v22 v21 
cmu.alignvec v16 v1 v6 15 || vau.shl.x8 v22 v22 1
cmu.cmvv.u8 v0 v16 
peu.pvv8 GT ||  vau.or v22 v22 v21 
vau.shl.x8 v22 v22 1
cmu.cmvv.u8 v0 v6 
peu.pvv8 GT ||  vau.or v22 v22 v21 
cmu.alignvec v16 v6 v11 1 || vau.shl.x8 v22 v22 1
cmu.cmvv.u8 v0 v16 
peu.pvv8 GT ||  vau.or v22 v22 v21 
cmu.alignvec v16 v6 v11 2 || vau.shl.x8 v22 v22 1 
cmu.cmvv.u8 v0 v16 || lsu0.ldo.64.l v11 i0 0x0  || lsu1.ldo.64.h v11 i0 0x8   || iau.add i0 i0 16
peu.pvv8 GT ||  vau.or v22 v22 v21 || cmu.cpvv v1 v6 

cmu.alignvec v17 v2 v7 14 || vau.shl.x8 v22 v22 1
cmu.cmvv.u8 v0 v17 
peu.pvv8 GT ||  vau.or v22 v22 v21 || cmu.cpvv v6 v11 
cmu.alignvec v17 v2 v7 15 || vau.shl.x8 v22 v22 1
cmu.cmvv.u8 v0 v17 
peu.pvv8 GT ||  vau.or v22 v22 v21 
vau.shl.x8 v22 v22 1
cmu.cmvv.u8 v0 v7 
peu.pvv8 GT ||  vau.or v22 v22 v21
cmu.alignvec v17 v7 v12 1 || vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v17 
peu.pvv8 GT ||  vau.or v23 v23 v21 
cmu.alignvec v17 v7 v12 2 || vau.shl.x8 v23 v23 1 
cmu.cmvv.u8 v0 v17 || lsu0.ldo.64.l v12 i1 0x0  || lsu1.ldo.64.h v12 i1 0x8   || iau.add i1 i1 16
peu.pvv8 GT ||  vau.or v23 v23 v21 || cmu.cpvv v2 v7 

cmu.alignvec v18 v3 v8 14 || vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v18 
peu.pvv8 GT ||  vau.or v23 v23 v21 || cmu.cpvv v7 v12 
cmu.alignvec v18 v3 v8 15 || vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v18 
peu.pvv8 GT ||  vau.or v23 v23 v21 
vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v8 
peu.pvv8 GT ||  vau.or v23 v23 v21 
cmu.alignvec v18 v8 v13 1 || vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v18 
peu.pvv8 GT ||  vau.or v23 v23 v21 
cmu.alignvec v18 v8 v13 2 || vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v18 || lsu0.ldo.64.l v13 i2 0x0  || lsu1.ldo.64.h v13 i2 0x8   || iau.add i2 i2 16
peu.pvv8 GT ||  vau.or v23 v23 v21 || cmu.cpvv v3 v8 

cmu.alignvec v19 v4 v9 14 || vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v19
peu.pvv8 GT ||  vau.or v23 v23 v21 || cmu.cpvv v8 v13  
cmu.alignvec v19 v4 v9 15 || vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v19 
peu.pvv8 GT ||  vau.or v24 v24 v21 
vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v9
peu.pvv8 GT ||  vau.or v24 v24 v21 
cmu.alignvec v19 v9 v14 1 || vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v19
peu.pvv8 GT ||  vau.or v24 v24 v21 
cmu.alignvec v19 v9 v14 2 || vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v19 || lsu0.ldo.64.l v14 i3 0x0  || lsu1.ldo.64.h v14 i3 0x8   || iau.add i3 i3 16
peu.pvv8 GT ||  vau.or v24 v24 v21 || cmu.cpvv v4 v9 

cmu.alignvec v20 v5 v10 14 || vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v20
peu.pvv8 GT ||  vau.or v24 v24 v21 || cmu.cpvv v9 v14 
cmu.alignvec v20 v5 v10 15 || vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v20
peu.pvv8 GT ||  vau.or v24 v24 v21 
vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v10
peu.pvv8 GT ||  vau.or v24 v24 v21 
cmu.alignvec v20 v10 v15 1 || vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v20 || lsu0.ldo.64.l v15 i4 0x0  || lsu1.ldo.64.h v15 i4 0x8   || iau.add i4 i4 16 
peu.pvv8 GT ||  vau.or v24 v24 v21 || cmu.cpvv v5 v10 
cmu.alignvec v20 v10 v15 2 || vau.shl.x8 v25 v25 1
cmu.cmvv.u8 v0 v20 
peu.pvv8 GT ||  vau.or v25 v25 v21 || cmu.cpvv v10 v15 

vau.shl.x8 v25, v25, 7 || cmu.cpvv v0 v8 
cmu.vilv.x8 v26 v27 v22 v23
cmu.vilv.x8 v28 v29 v24 v25
cmu.vilv.x16 v30 v31 v26 v28
cmu.vilv.x16 v17 v18 v27 v29
vau.shr.u32 v30 v30 7
vau.shr.u32 v31 v31 7
vau.shr.u32 v17 v17 7
vau.shr.u32 v18 v18 7
nop


cmu.alignvec v16 v1 v6 14 || vau.shl.x8 v22 v22 1 || bru.rpl i21 i16 censusTransform5x5_endCensus5x5_delay
cmu.cmvv.u8 v0 v16 			
peu.pvv8 GT ||  vau.or v22 v22 v21 
cmu.alignvec v16 v1 v6 15 || vau.shl.x8 v22 v22 1
cmu.cmvv.u8 v0 v16 
peu.pvv8 GT ||  vau.or v22 v22 v21 
vau.shl.x8 v22 v22 1      || lsu0.sto.64.l v30 i17 0x0  ||lsu1.sto.64.h v30 i17 0x8
cmu.cmvv.u8 v0 v6         || lsu0.sto.64.l v31 i17 0x10 ||lsu1.sto.64.h v31 i17 0x18
peu.pvv8 GT ||  vau.or v22 v22 v21 
cmu.alignvec v16 v6 v11 1 || vau.shl.x8 v22 v22 1
cmu.cmvv.u8 v0 v16 
peu.pvv8 GT ||  vau.or v22 v22 v21 
cmu.alignvec v16 v6 v11 2 || vau.shl.x8 v22 v22 1 
cmu.cmvv.u8 v0 v16        || lsu0.ldo.64.l v11 i0 0x0  || lsu1.ldo.64.h v11 i0 0x8   || iau.add i0 i0 16
peu.pvv8 GT ||  vau.or v22 v22 v21 || cmu.cpvv v1 v6   || lsu0.sto.64.l v17 i17 0x20 ||lsu1.sto.64.h v17 i17 0x28

cmu.alignvec v17 v2 v7 14 || vau.shl.x8 v22 v22 1
cmu.cmvv.u8 v0 v17 
peu.pvv8 GT ||  vau.or v22 v22 v21 || cmu.cpvv v6 v11
cmu.alignvec v17 v2 v7 15 || vau.shl.x8 v22 v22 1
cmu.cmvv.u8 v0 v17 
peu.pvv8 GT ||  vau.or v22 v22 v21 
vau.shl.x8 v22 v22 1
cmu.cmvv.u8 v0 v7 
peu.pvv8 GT ||  vau.or v22 v22 v21 
cmu.alignvec v17 v7 v12 1 || vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v17 
peu.pvv8 GT ||  vau.or v23 v23 v21 || lsu0.sto.64.l v18 i17 0x30  ||lsu1.sto.64.h v18 i17 0x38 || iau.add i17 i17 0x40
cmu.alignvec v17 v7 v12 2 || vau.shl.x8 v23 v23 1 
cmu.cmvv.u8 v0 v17 || lsu0.ldo.64.l v12 i1 0x0  || lsu1.ldo.64.h v12 i1 0x8   || iau.add i1 i1 16
peu.pvv8 GT ||  vau.or v23 v23 v21 || cmu.cpvv v2 v7

cmu.alignvec v18 v3 v8 14 || vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v18 
peu.pvv8 GT ||  vau.or v23 v23 v21 || cmu.cpvv v7 v12 
cmu.alignvec v18 v3 v8 15 || vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v18 
peu.pvv8 GT ||  vau.or v23 v23 v21 
vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v8 
peu.pvv8 GT ||  vau.or v23 v23 v21 
cmu.alignvec v18 v8 v13 1 || vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v18 
peu.pvv8 GT ||  vau.or v23 v23 v21 
cmu.alignvec v18 v8 v13 2 || vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v18 || lsu0.ldo.64.l v13 i2 0x0  || lsu1.ldo.64.h v13 i2 0x8   || iau.add i2 i2 16
peu.pvv8 GT ||  vau.or v23 v23 v21 || cmu.cpvv v3 v8 

cmu.alignvec v19 v4 v9 14 || vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v19
peu.pvv8 GT ||  vau.or v23 v23 v21 || cmu.cpvv v8 v13  
cmu.alignvec v19 v4 v9 15 || vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v19 
peu.pvv8 GT ||  vau.or v24 v24 v21 
vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v9
peu.pvv8 GT ||  vau.or v24 v24 v21 
cmu.alignvec v19 v9 v14 1 || vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v19
peu.pvv8 GT ||  vau.or v24 v24 v21 
cmu.alignvec v19 v9 v14 2 || vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v19 || lsu0.ldo.64.l v14 i3 0x0  || lsu1.ldo.64.h v14 i3 0x8   || iau.add i3 i3 16
peu.pvv8 GT || vau.or v24 v24 v21 || cmu.cpvv v4 v9 

cmu.alignvec v20 v5 v10 14 || vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v20
peu.pvv8 GT ||  vau.or v24 v24 v21 || cmu.cpvv v9 v14 
cmu.alignvec v20 v5 v10 15 || vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v20
peu.pvv8 GT ||  vau.or v24 v24 v21 
vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v10
peu.pvv8 GT ||  vau.or v24 v24 v21 
cmu.alignvec v20 v10 v15 1 || vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v20 || lsu0.ldo.64.l v15 i4 0x0  || lsu1.ldo.64.h v15 i4 0x8   || iau.add i4 i4 16 
peu.pvv8 GT ||  vau.or v24 v24 v21 || cmu.cpvv v5 v10 
cmu.alignvec v20 v10 v15 2 || vau.shl.x8 v25 v25 1
cmu.cmvv.u8 v0 v20 		
peu.pvv8 GT ||  vau.or v25 v25 v21 || cmu.cpvv v10 v15 

vau.shl.x8 v25, v25, 7 || cmu.cpvv v0 v8
 censusTransform5x5_loop:
cmu.vilv.x8 v26 v27 v22 v23	
cmu.vilv.x8 v28 v29 v24 v25	
cmu.vilv.x16 v30 v31 v26 v28	
cmu.vilv.x16 v17 v18 v27 v29 || vau.shr.u32 v30 v30 7
vau.shr.u32 v31 v31 7
vau.shr.u32 v17 v17 7
vau.shr.u32 v18 v18 7
 censusTransform5x5_endCensus5x5_delay:


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
	.nowarnend
bru.jmp i30
nop 6
.size mvcvCensusTransform5x5_asm,.-mvcvCensusTransform5x5_asm
.end