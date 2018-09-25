.type mvcvCensusTransformAverageRefMask7x7_asm,@function
.version 00.51.04

/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.data .data.censusTransformAverageRefMask7x7
.align 4

 .LboxFilter7x7_multiply:
        .float16  0.020408163F16, 255.0F16

.code .text.censusTransformAverageRefMask7x7

//void mvcvCensusTransformAverageRefMask7x7(u8 **in, u64 *out, u32 width)
//                                i18       i17      i16

mvcvCensusTransformAverageRefMask7x7_asm:
.nowarn 10 




//1 - step - boxfilter7x7
        iau.sub i19 i19 i16 //make room on stack   
        iau.sub i10 i16 0  //copy width of elements
        iau.sub  i9 i19 0    //copy 1 of stack address
        iau.sub i11 i9 0    //copy 2 of stack address

	IAU.SHR.u32 i16 i16 3
 	lsu0.ldil i7, .LboxFilter7x7_multiply     ||  lsu1.ldih i7, .LboxFilter7x7_multiply
	lsu0.ldil i8, .LboxFilter7x7_loop     ||  lsu1.ldih i8, .LboxFilter7x7_loop
	LSU0.LD.16R v0, i7  || IAU.ADD i7 i7 2
	LSU0.LDo.32  i0  i18 0  
	LSU0.LDo.32  i1  i18 4  
	LSU0.LDo.32  i2  i18 8  
	LSU0.LDo.32  i3  i18 12 
	LSU0.LDo.32  i4  i18 16 
	LSU0.LDo.32  i5  i18 20 
	LSU0.LDo.32  i6  i18 24 
                                IAU.SUB i0 i0 8
	LSU0.LD.16R v17, i7  || IAU.SUB i1 i1 8

IAU.SUB i2 i2 8
IAU.SUB i3 i3 8
IAU.SUB i4 i4 8
IAU.SUB i5 i5 8
IAU.SUB i6 i6 8

LSU0.LD.128.u8.u16  v1 i0 || IAU.ADD i0 i0 8
LSU0.LD.128.u8.u16  v2 i1 || IAU.ADD i1 i1 8
LSU0.LD.128.u8.u16  v3 i2 || IAU.ADD i2 i2 8
LSU0.LD.128.u8.u16  v4 i3 || IAU.ADD i3 i3 8
LSU0.LD.128.u8.u16  v5 i4 || IAU.ADD i4 i4 8
LSU0.LD.128.u8.u16  v6 i5 || IAU.ADD i5 i5 8
LSU0.LD.128.u8.u16  v7 i6 || IAU.ADD i6 i6 8
VAU.ACCPZ.u16     v1 || LSU0.LD.128.u8.u16  v8  i0 || IAU.ADD i0 i0 8
VAU.ACCP.u16      v2 || LSU0.LD.128.u8.u16  v9  i1 || IAU.ADD i1 i1 8
VAU.ACCP.u16      v3 || LSU0.LD.128.u8.u16  v10 i2 || IAU.ADD i2 i2 8
VAU.ACCP.u16      v4 || LSU0.LD.128.u8.u16  v11 i3 || IAU.ADD i3 i3 8
VAU.ACCP.u16      v5 || LSU0.LD.128.u8.u16  v12 i4 || IAU.ADD i4 i4 8
VAU.ACCP.u16      v6 || LSU0.LD.128.u8.u16  v13 i5 || IAU.ADD i5 i5 8
VAU.ACCPW.u16 v21 v7 || LSU0.LD.128.u8.u16  v14 i6 || IAU.ADD i6 i6 8
VAU.ACCPZ.u16     v8   || LSU0.LD.128.u8.u16  v1  i0 || IAU.ADD i0 i0 8    
VAU.ACCP.u16      v9   || LSU0.LD.128.u8.u16  v2  i1 || IAU.ADD i1 i1 8
VAU.ACCP.u16      v10  || LSU0.LD.128.u8.u16  v3  i2 || IAU.ADD i2 i2 8
VAU.ACCP.u16      v11  || LSU0.LD.128.u8.u16  v4  i3 || IAU.ADD i3 i3 8
VAU.ACCP.u16      v12  || LSU0.LD.128.u8.u16  v5  i4 || IAU.ADD i4 i4 8
VAU.ACCP.u16      v13  || LSU0.LD.128.u8.u16  v6  i5 || IAU.ADD i5 i5 8
VAU.ACCPW.u16 v22 v14  || LSU0.LD.128.u8.u16  v7  i6 || IAU.ADD i6 i6 8
VAU.ACCPZ.u16     v1                                 || LSU0.LD.128.u8.u16  v1  i0 || IAU.ADD i0 i0 8 
VAU.ACCP.u16      v2                                 || LSU0.LD.128.u8.u16  v2  i1 || IAU.ADD i1 i1 8
VAU.ACCP.u16      v3                                 || LSU0.LD.128.u8.u16  v3  i2 || IAU.ADD i2 i2 8
VAU.ACCP.u16      v4                                 || LSU0.LD.128.u8.u16  v4  i3 || IAU.ADD i3 i3 8
VAU.ACCP.u16      v5                                 || LSU0.LD.128.u8.u16  v5  i4 || IAU.ADD i4 i4 8
VAU.ACCP.u16      v6                                 || LSU0.LD.128.u8.u16  v6  i5 || IAU.ADD i5 i5 8
VAU.ACCPW.u16 v23 v7  || cmu.alignvec  v8 v21 v22 10 || LSU0.LD.128.u8.u16  v7  i6 || IAU.ADD i6 i6 8
VAU.ACCPZ.u16     v8  || cmu.alignvec  v9 v21 v22 12 || bru.rpl i8 i16
VAU.ACCP.u16      v9  || cmu.alignvec v10 v21 v22 14
VAU.ACCP.u16      v10 || cmu.alignvec v11 v22 v23  2
VAU.ACCP.u16      v11 || cmu.alignvec v12 v22 v23  4
VAU.ACCP.u16      v12 || cmu.alignvec v13 v22 v23  6
VAU.ACCP.u16      v13 || cmu.cpvv v21 v22
VAU.ACCPW.u16 v14 v22 || cmu.cpvv v22 v23
VAU.ACCPZ.u16     v1 
VAU.ACCP.u16      v2  || CMU.CPVV.i16.f16 v15 v14     || LSU0.LD.128.u8.u16  v1  i0 || IAU.ADD i0 i0 8
 .LboxFilter7x7_loop: 
VAU.ACCP.u16      v3                                  || LSU0.LD.128.u8.u16  v2  i1 || IAU.ADD i1 i1 8
vau.mul.f16 v16 v15 v0                                || LSU0.LD.128.u8.u16  v3  i2 || IAU.ADD i2 i2 8                                  
VAU.ACCP.u16      v4                                 || LSU0.LD.128.u8.u16  v4  i3 || IAU.ADD i3 i3 8 
VAU.ACCP.u16      v5                                 || LSU0.LD.128.u8.u16  v5  i4 || IAU.ADD i4 i4 8 
VAU.ACCP.u16      v6  || CMU.CLAMP0.f16 v18 v16 v17  || LSU0.LD.128.u8.u16  v6  i5 || IAU.ADD i5 i5 8 
LSU0.LD.128.u8.u16  v7  i6 || IAU.ADD i6 i6 8
VAU.ACCPW.u16 v23 v7  || cmu.alignvec  v8 v21 v22 10 || LSU0.ST.128.f16.u8 v18 i9 || IAU.ADD i9 i9 8

nop 10

// second part censustransformmodified

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
lsu0.ldil i14 .LcensusTransform7x7_loop  || lsu1.ldih i14 .LcensusTransform7x7_loop
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
iau.shr.u32 i16 i16 1
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

lsu0.ldo.64.l v15 i0 0x0 || lsu1.ldo.64.h v15 i0 0x8 || iau.add i0 i0 16 || vau.xor v22 v22 v22 || bru.rpl i14 i16 .LcensusTransform7x7_endCensus7x7_delay
lsu0.ldo.64.l v0 i11 0x0 || lsu1.ldo.64.h v0 i11 0x8 || iau.add i11 i11 16
lsu0.ldo.64.l v16 i1 0x0 || lsu1.ldo.64.h v16 i1 0x8 || iau.add i1 i1 16 || vau.xor v23 v23 v23 
lsu0.ldo.64.l v17 i2 0x0 || lsu1.ldo.64.h v17 i2 0x8 || iau.add i2 i2 16 || vau.xor v24 v24 v24                                                                                                   
lsu0.ldo.64.l v18 i3 0x0 || lsu1.ldo.64.h v18 i3 0x8 || iau.add i3 i3 16 || vau.xor v25 v25 v25
lsu0.ldo.64.l v19 i4 0x0 || lsu1.ldo.64.h v19 i4 0x8 || iau.add i4 i4 16 || vau.xor v26 v26 v26
lsu0.ldo.64.l v20 i5 0x0 || lsu1.ldo.64.h v20 i5 0x8 || iau.add i5 i5 16 || vau.xor v27 v27 v27
lsu0.ldo.64.l v21 i6 0x0 || lsu1.ldo.64.h v21 i6 0x8 || iau.add i6 i6 16 || vau.xor v28 v28 v28 //|| cmu.cpvv v0 v11
vau.sub.i8 v0 v0 1

cmu.alignvec v30 v1 v8 13 || vau.shl.x8 v22 v22 1
cmu.cmvv.u8 v0 v30                                                      //0
cmu.alignvec v30 v1 v8 15 || peu.pvv8 GT ||  vau.or v22 v22 v29 
vau.shl.x8 v22 v22 1
cmu.cmvv.u8 v0 v30                                                      //2 
peu.pvv8 GT ||  vau.or v22 v22 v29 
cmu.alignvec v30 v8 v15 1
vau.shl.x8 v22 v22 1
cmu.cmvv.u8 v0 v30                                                      //4
cmu.alignvec v30 v8 v15 3 || peu.pvv8 GT ||  vau.or v22 v22 v29
vau.shl.x8 v22 v22 1      || cmu.cpvv v1 v8
cmu.cmvv.u8 v0 v30                                                      //6 
peu.pvv8 GT ||  vau.or v22 v22 v29 || cmu.cpvv v8 v15


cmu.alignvec v30 v2 v9 13 || vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v2 v9 15 || peu.pvv8 GT ||  vau.or v23 v23 v29
vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v23 v23 v29 
cmu.alignvec v30 v9 v16 1
vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v9 v16 3 || peu.pvv8 GT ||  vau.or v23 v23 v29
vau.shl.x8 v23 v23 1      || cmu.cpvv v2 v9
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v23 v23 v29 || cmu.cpvv v9 v16



cmu.alignvec v30 v3 v10 13 || vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v3 v10 15 || peu.pvv8 GT ||  vau.or v23 v23 v29
vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v23 v23 v29 
cmu.alignvec v30 v10 v17 1 
vau.shl.x8 v23 v23 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v10 v17 3 || peu.pvv8 GT ||  vau.or v23 v23 v29
vau.shl.x8 v23 v23 1       || cmu.cpvv v3 v10
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v23 v23 v29 || cmu.cpvv v10 v17



cmu.alignvec v30 v4 v11 13 || vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v4 v11 15 || peu.pvv8 GT ||  vau.or v24 v24 v29
vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v24 v24 v29 
cmu.alignvec v30 v11 v18 1
vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v11 v18 3 || peu.pvv8 GT ||  vau.or v24 v24 v29 
vau.shl.x8 v24 v24 1       || cmu.cpvv v4 v11
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v24 v24 v29 || cmu.cpvv v11 v18



cmu.alignvec v30 v5 v12 13 || vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v5 v12 15 || peu.pvv8 GT ||  vau.or v24 v24 v29
vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v24 v24 v29 
cmu.alignvec v30 v12 v19 1
vau.shl.x8 v24 v24 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v12 v19 3 || peu.pvv8 GT ||  vau.or v24 v24 v29
vau.shl.x8 v24 v24 1       || cmu.cpvv v5 v12
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v24 v24 v29 || cmu.cpvv v12 v19



cmu.alignvec v30 v6 v13 13 || vau.shl.x8 v25 v25 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v6 v13 15 || peu.pvv8 GT ||  vau.or v25 v25 v29
vau.shl.x8 v25 v25 1
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v25 v25 v29 
cmu.alignvec v30 v13 v20 1
vau.shl.x8 v25 v25 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v13 v20 3 || peu.pvv8 GT ||  vau.or v25 v25 v29
vau.shl.x8 v25 v25 1       || cmu.cpvv v6 v13
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v25 v25 v29 || cmu.cpvv v13 v20



cmu.alignvec v30 v7 v14 13 || vau.shl.x8 v25 v25 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v7 v14 15 || peu.pvv8 GT ||  vau.or v25 v25 v29
vau.shl.x8 v25 v25 1
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v25 v25 v29 
cmu.alignvec v30 v14 v21 1
vau.shl.x8 v25 v25 1
cmu.cmvv.u8 v0 v30
cmu.alignvec v30 v14 v21 3 || peu.pvv8 GT ||  vau.or v25 v25 v29
vau.shl.x8 v25 v25 1       || cmu.cpvv v7 v14
cmu.cmvv.u8 v0 v30
peu.pvv8 GT ||  vau.or v25 v25 v29 || cmu.cpvv v14 v21


cmu.vilv.x8 v27 v26 v22 v23
 .LcensusTransform7x7_loop:    
cmu.vilv.x8 v30 v28 v24 v25
cmu.vilv.x16 v28 v26 v26 v28
cmu.vilv.x16 v30 v27 v27 v30
lsu0.sto.64.l v30 i17 0x0  || lsu1.sto.64.h v30 i17 0x8
lsu0.sto.64.l v27 i17 0x10 || lsu1.sto.64.h v27 i17 0x18
lsu0.sto.64.l v28 i17 0x20 || lsu1.sto.64.h v28 i17 0x28
lsu0.sto.64.l v26 i17 0x30 || lsu1.sto.64.h v26 i17 0x38 || iau.add i17 i17 0x40
 .LcensusTransform7x7_endCensus7x7_delay:

 

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
iau.add i19 i19 i10
.nowarnend    
bru.jmp i30
nop 6
.size mvcvCensusTransformAverageRefMask7x7_asm,.-mvcvCensusTransformAverageRefMask7x7_asm
.end
