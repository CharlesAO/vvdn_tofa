///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvErode5x5_asm,@function
.version 00.51.04

.data .data.erode5x5
.align 4
.code .text.erode5x5
//  void Erode5x5(u8** src, u8** dst, u8** kernel, u32 width)
//                   i18       i17        i16        i15    
mvcvErode5x5_asm:
.nowarn
		
	IAU.SUB i19 i19 8		
	LSU0.ST.64.L v24 i19	|| IAU.SUB i19 i19 8
	LSU0.ST.64.H v24 i19	|| IAU.SUB i19 i19 8
	LSU0.ST.64.L v25 i19	|| IAU.SUB i19 i19 8
	LSU0.ST.64.H v25 i19	|| IAU.SUB i19 i19 8
	LSU0.ST.64.L v26 i19	|| IAU.SUB i19 i19 8
	LSU0.ST.64.H v26 i19	|| IAU.SUB i19 i19 8
	LSU0.ST.64.L v27 i19	|| IAU.SUB i19 i19 8
	LSU0.ST.64.H v27 i19    || IAU.SUB i19 i19 8
	LSU0.ST.64.L v28 i19    || IAU.SUB i19 i19 8
	LSU0.ST.64.H v28 i19    || IAU.SUB i19 i19 8
	LSU0.ST.64.L v29 i19    || IAU.SUB i19 i19 8
	LSU0.ST.64.H v29 i19    || IAU.SUB i19 i19 8
	LSU0.ST.64.L v30 i19    || IAU.SUB i19 i19 8
	LSU0.ST.64.H v30 i19    || IAU.SUB i19 i19 8
	LSU0.ST.64.L v31 i19    || IAU.SUB i19 i19 8
	LSU0.ST.64.H v31 i19

	
	
LSU0.LDIl i11 erode5x5_loop         || LSU1.LDIh i11 erode5x5_loop
LSU0.LDIl i12 erode5x5_compensate   || LSU1.LDIh i12 erode5x5_compensate
LSU0.LDIL i14 0x0F || LSU1.LDIH i14 0x00 
LSU0.LDIL i10 0xFF 	
nop 6
lsu0.ld.32 i17 i17    
	
lsu0.ldo.32 i0 i18 0  || lsu1.ldo.32 i5 i16 0  || IAU.AND i13 i15 i14
lsu0.ldo.32 i1 i18 4  || lsu1.ldo.32 i6 i16 4  
lsu0.ldo.32 i2 i18 8  || lsu1.ldo.32 i7 i16 8  
lsu0.ldo.32 i3 i18 12 || lsu1.ldo.32 i8 i16 12 || iau.shr.u32 i15 i15 4 
lsu0.ldo.32 i4 i18 16 || lsu1.ldo.32 i9 i16 16 
nop 4

cmu.cmz.i8 i15
peu.pc1c eq || bru.jmp i12
nop 6

lsu0.ldo.64.l v5 i5 0 || lsu1.ldo.64.l v6 i6 0 
lsu0.ldo.64.l v7 i7 0 || lsu1.ldo.64.l v8 i8 0 
lsu0.ldo.64.l v9 i9 0 || vau.sub.i8 v31 v31 v31 
nop 4

cmu.vilv.x8 v5 v10 v5 v5 || iau.sub i0 i0 16 
cmu.vilv.x8 v6 v10 v6 v6 || iau.sub i1 i1 16 
cmu.vilv.x8 v7 v10 v7 v7 || iau.sub i2 i2 16 
cmu.vilv.x8 v8 v10 v8 v8 || iau.sub i3 i3 16 
cmu.vilv.x8 v9 v10 v9 v9 || iau.sub i4 i4 16 

lsu0.ldo.64.l v0 i0 0 || lsu1.ldo.64.h v0 i0 8 || iau.add i0 i0 16 || CMU.CPIVR.x8 v31, i10
lsu0.ldo.64.l v1 i1 0 || lsu1.ldo.64.h v1 i1 8 || iau.add i1 i1 16
lsu0.ldo.64.l v2 i2 0 || lsu1.ldo.64.h v2 i2 8 || iau.add i2 i2 16
lsu0.ldo.64.l v3 i3 0 || lsu1.ldo.64.h v3 i3 8 || iau.add i3 i3 16
lsu0.ldo.64.l v4 i4 0 || lsu1.ldo.64.h v4 i4 8 || iau.add i4 i4 16
                                               
lsu0.ldo.64.l v10 i0 0 || lsu1.ldo.64.h v10 i0 8 || iau.add i0 i0 16 
lsu0.ldo.64.l v11 i1 0 || lsu1.ldo.64.h v11 i1 8 || iau.add i1 i1 16
lsu0.ldo.64.l v12 i2 0 || lsu1.ldo.64.h v12 i2 8 || iau.add i2 i2 16
lsu0.ldo.64.l v13 i3 0 || lsu1.ldo.64.h v13 i3 8 || iau.add i3 i3 16
lsu0.ldo.64.l v14 i4 0 || lsu1.ldo.64.h v14 i4 8 || iau.add i4 i4 16

lsu0.ldo.64.l v15 i0 0 || lsu1.ldo.64.h v15 i0 8 || iau.sub i0 i0 16  
lsu0.ldo.64.l v16 i1 0 || lsu1.ldo.64.h v16 i1 8 || iau.sub i1 i1 16 
lsu0.ldo.64.l v17 i2 0 || lsu1.ldo.64.h v17 i2 8 || iau.sub i2 i2 16 
lsu0.ldo.64.l v18 i3 0 || lsu1.ldo.64.h v18 i3 8 || iau.sub i3 i3 16 
lsu0.ldo.64.l v19 i4 0 || lsu1.ldo.64.h v19 i4 8 || iau.sub i4 i4 16 


LSU0.LDIl i18 erode5x5_final        || LSU1.LDIh i18 erode5x5_final

//--------------------------------------------------------------------------
cmu.alignvec v20 v0 v10 14 
cmu.alignvec v21 v0 v10 15 
cmu.alignvec v23 v10 v15 1 || lsu0.ldo.64.l v0 i0 0 || lsu1.ldo.64.h v0 i0 8 || iau.add i0 i0 16 
cmu.alignvec v24 v10 v15 2 
vau.sub.i8 v20 v31 v20 
vau.sub.i8 v21 v31 v21 
vau.sub.i8 v22 v31 v10    
vau.sub.i8 v23 v31 v23  
vau.sub.i8 v24 v31 v24     || lsu0.ldo.64.l v10 i0 0 || lsu1.ldo.64.h v10 i0 8 || iau.add i0 i0 16 
nop
vau.mul.i8 v20 v5 v20  || lsu0.swzv8 [00000000] || cmu.alignvec v26 v1 v11 14 
vau.mul.i8 v21 v5 v21  || lsu0.swzv8 [11111111] || cmu.alignvec v27 v1 v11 15
vau.mul.i8 v22 v5 v22  || lsu0.swzv8 [22222222] || cmu.alignvec v29 v11 v16 1 
vau.mul.i8 v23 v5 v23  || lsu0.swzv8 [33333333] || cmu.alignvec v30 v11 v16 2 
vau.mul.i8 v24 v5 v24  || lsu0.swzv8 [44444444] || cmu.max.u8 v20 v21 v20 
nop 
vau.sub.i8 v26 v31 v26 || cmu.max.u8 v22 v23 v22 || lsu0.ldo.64.l v1 i1 0  || lsu1.ldo.64.h v1 i1 8 || iau.add i1 i1 16 
vau.sub.i8 v27 v31 v27 || cmu.max.u8 v21 v24 v20 || lsu0.ldo.64.l v11 i1 0 || lsu1.ldo.64.h v11 i1 8 || iau.add i1 i1 16
vau.sub.i8 v28 v31 v11 
vau.sub.i8 v29 v31 v29 || cmu.max.u8 v15 v21 v22
vau.sub.i8 v30 v31 v30  
vau.mul.i8 v26 v6 v26  || lsu0.swzv8 [00000000] || cmu.alignvec v20 v2 v12 14 || bru.rpl i11 i15 _end_delay_loop_erode5x5
vau.mul.i8 v27 v6 v27  || lsu0.swzv8 [11111111] || cmu.alignvec v21 v2 v12 15 
vau.mul.i8 v28 v6 v28  || lsu0.swzv8 [22222222] || cmu.alignvec v23 v12 v17 1 
vau.mul.i8 v29 v6 v29  || lsu0.swzv8 [33333333] || cmu.alignvec v24 v12 v17 2 
vau.mul.i8 v30 v6 v30  || lsu0.swzv8 [44444444] || cmu.max.u8 v26 v26 v27
nop
vau.sub.i8 v20 v31 v20 || cmu.max.u8 v29 v28 v29 || lsu0.ldo.64.l v2 i2 0 || lsu1.ldo.64.h v2 i2 8 || iau.add i2 i2 16
vau.sub.i8 v21 v31 v21 || cmu.max.u8 v27 v30 v26 || lsu0.ldo.64.l v12 i2 0 || lsu1.ldo.64.h v12 i2 8 || iau.add i2 i2 16
vau.sub.i8 v22 v31 v12 
vau.sub.i8 v23 v31 v23 || cmu.max.u8 v16 v27 v29 
vau.sub.i8 v24 v31 v24  
vau.mul.i8 v20 v7 v20  || lsu0.swzv8 [00000000] || cmu.alignvec v26 v3 v13 14 
vau.mul.i8 v21 v7 v21  || lsu0.swzv8 [11111111] || cmu.alignvec v27 v3 v13 15 
vau.mul.i8 v22 v7 v22  || lsu0.swzv8 [22222222] || cmu.alignvec v28 v13 v18 1 
vau.mul.i8 v23 v7 v23  || lsu0.swzv8 [33333333] || cmu.alignvec v29 v13 v18 2 
vau.mul.i8 v24 v7 v24  || lsu0.swzv8 [44444444] || cmu.max.u8 v20 v20 v21
nop
vau.sub.i8 v26 v31 v26 || cmu.max.u8 v22 v23 v22 || lsu0.ldo.64.l v3 i3 0  || lsu1.ldo.64.h v3 i3 8  || iau.add i3 i3 16
vau.sub.i8 v27 v31 v27 || cmu.max.u8 v21 v24 v20 || lsu0.ldo.64.l v13 i3 0  || lsu1.ldo.64.h v13 i3 8  || iau.add i3 i3 16
vau.sub.i8 v28 v31 v13 || cmu.max.u8 v25 v15 v16 
vau.sub.i8 v29 v31 v28 || cmu.max.u8 v17 v21 v22  || lsu0.ldo.64.l v15 i0 0 || lsu1.ldo.64.h v15 i0 8 || iau.sub i0 i0 16 
vau.sub.i8 v30 v31 v29 
vau.mul.i8 v26 v8 v26  || lsu0.swzv8 [00000000] || cmu.alignvec v20 v4 v14 14 
vau.mul.i8 v27 v8 v27  || lsu0.swzv8 [11111111] || cmu.alignvec v21 v4 v14 15
vau.mul.i8 v28 v8 v28  || lsu0.swzv8 [22222222] || cmu.alignvec v23 v14 v19 1
vau.mul.i8 v29 v8 v29  || lsu0.swzv8 [33333333] || cmu.alignvec v24 v14 v19 2
vau.mul.i8 v30 v8 v30  || lsu0.swzv8 [44444444] || cmu.max.u8 v26 v26 v27 
nop
vau.sub.i8 v20 v31 v20 || cmu.max.u8 v28 v28 v29 || lsu0.ldo.64.l v4 i4 0  || lsu1.ldo.64.h v4 i4 8  || iau.add i4 i4 16
vau.sub.i8 v21 v31 v21 || cmu.max.u8 v27 v30 v26 || lsu0.ldo.64.l v14 i4 0 || lsu1.ldo.64.h v14 i4 8 || iau.add i4 i4 16
vau.sub.i8 v22 v31 v14 || lsu0.ldo.64.l v0 i0 0  || lsu1.ldo.64.h v0 i0 8  || iau.add i0 i0 16 
vau.sub.i8 v29 v31 v23 || cmu.max.u8 v18 v27 v28
vau.sub.i8 v30 v31 v24  
vau.mul.i8 v26 v9 v20  || lsu0.swzv8 [00000000] || cmu.alignvec v20 v0 v10 14 
vau.mul.i8 v27 v9 v21  || lsu0.swzv8 [11111111] || cmu.alignvec v21 v0 v10 15 
vau.mul.i8 v28 v9 v22  || lsu0.swzv8 [22222222] || cmu.alignvec v23 v10 v15 1 
vau.mul.i8 v29 v9 v29  || lsu0.swzv8 [33333333] || cmu.alignvec v24 v10 v15 2 
vau.mul.i8 v30 v9 v30  || lsu0.swzv8 [44444444] || cmu.max.u8 v26 v26 v27 
nop   
vau.sub.i8 v20 v31 v20 || cmu.max.u8 v28 v28 v29  || lsu0.ldo.64.l v16 i1 0 || lsu1.ldo.64.h v16 i1 8 || iau.sub i1 i1 16                      
vau.sub.i8 v21 v31 v21 || cmu.max.u8 v27 v26 v30  || lsu0.ldo.64.l v18 i3 0 || lsu1.ldo.64.h v18 i3 8 || iau.sub i3 i3 16    
vau.sub.i8 v22 v31 v10 || cmu.max.u8 v17 v17 v18                                                                    
vau.sub.i8 v23 v31 v23 || cmu.max.u8 v19 v27 v28 
vau.sub.i8 v24 v31 v24 							  || lsu0.ldo.64.l v10 i0 0 || lsu1.ldo.64.h v10 i0 8 || iau.add i0 i0 16 
vau.mul.i8 v20 v5 v20  || lsu0.swzv8 [00000000] || cmu.alignvec v26 v1 v11 14
vau.mul.i8 v21 v5 v21  || lsu0.swzv8 [11111111] || cmu.alignvec v27 v1 v11 15 
vau.mul.i8 v22 v5 v22  || lsu0.swzv8 [22222222] || cmu.alignvec v29 v11 v16 1
vau.mul.i8 v23 v5 v23  || lsu0.swzv8 [33333333] || cmu.alignvec v30 v11 v16 2
vau.mul.i8 v24 v5 v24  || lsu0.swzv8 [44444444] || cmu.max.u8 v25 v19 v25 
 
nop
erode5x5_loop:

vau.sub.i8 v26 v31 v26 || cmu.max.u8 v17 v25 v17 || lsu0.ldo.64.l v17 i2 0  || lsu1.ldo.64.h v17 i2 8  || iau.sub i2 i2 16
vau.sub.i8 v27 v31 v27 || cmu.max.u8 v20 v21 v20 
vau.sub.i8 v28 v31 v11 || cmu.max.u8 v22 v23 v22 || lsu0.ldo.64.l v1 i1 0   || lsu1.ldo.64.h v1 i1 8   || iau.add i1 i1 16      
vau.sub.i8 v25 v31 v17 || cmu.max.u8 v21 v24 v20 || lsu0.ldo.64.l v11 i1 0  || lsu1.ldo.64.h v11 i1 8  || iau.add i1 i1 16
vau.sub.i8 v29 v31 v29  						 || lsu0.ldo.64.l v19 i4 0  || lsu1.ldo.64.h v19 i4 8  || iau.sub i4 i4 16 
vau.sub.i8 v30 v31 v30 || cmu.max.u8 v15 v21 v22 || lsu0.sto.64.l v25 i17 0 || lsu1.sto.64.h v25 i17 8 || iau.add i17 i17 16 

nop
_end_delay_loop_erode5x5:

nop

cmu.cmz.i32 i13 
peu.pc1c eq || bru.jmp i18
nop 6

iau.sub i0 i0 32	
iau.sub i1 i1 32
 erode5x5_compensate:
iau.sub i0 i0 8
iau.sub i1 i1 8
iau.sub i2 i2 8
iau.sub i3 i3 8
iau.sub i4 i4 8 

lsu0.ldi.128.u8.u16 v0 i0
lsu0.ldi.128.u8.u16 v1 i1
lsu0.ldi.128.u8.u16 v2 i2
lsu0.ldi.128.u8.u16 v3 i3
lsu0.ldi.128.u8.u16 v4 i4

lsu0.ldi.128.u8.u16 v10 i0 || lsu1.ld.128.u8.u16 v5 i5 || CMU.CPIVR.x16 v31, i10
lsu0.ldi.128.u8.u16 v11 i1 || lsu1.ld.128.u8.u16 v6 i6
lsu0.ldi.128.u8.u16 v12 i2 || lsu1.ld.128.u8.u16 v7 i7
lsu0.ldi.128.u8.u16 v13 i3 || lsu1.ld.128.u8.u16 v8 i8
lsu0.ldi.128.u8.u16 v14 i4 || lsu1.ld.128.u8.u16 v9 i9
lsu0.ld.128.u8.u16 v15 i0 
lsu0.ld.128.u8.u16 v16 i1 
lsu0.ld.128.u8.u16 v17 i2 
lsu0.ld.128.u8.u16 v18 i3 
lsu0.ld.128.u8.u16 v19 i4 
cmu.alignvec v20 v0 v10 12 
cmu.alignvec v21 v0 v10 14 
cmu.alignvec v23 v10 v15 2 
cmu.alignvec v24 v10 v15 4 
vau.sub.i16 v20 v31 v20 
vau.sub.i16 v21 v31 v21 
vau.sub.i16 v22 v31 v10 
vau.sub.i16 v23 v31 v23 
vau.sub.i16 v24 v31 v24 || cmu.alignvec v26 v1 v11 12
vau.mul.i16 v20 v5 v20 || lsu0.swzv8 [00000000] || cmu.alignvec v27 v1 v11 14 
vau.mul.i16 v21 v5 v21 || lsu0.swzv8 [11111111] || cmu.alignvec v29 v11 v16 2
vau.mul.i16 v22 v5 v22 || lsu0.swzv8 [22222222] || cmu.alignvec v30 v11 v16 4
vau.mul.i16 v23 v5 v23 || lsu0.swzv8 [33333333] 
vau.mul.i16 v24 v5 v24 || lsu0.swzv8 [44444444] || cmu.max.u16 v20 v21 v20 
nop
vau.sub.i16 v26 v31 v26 || cmu.max.u16 v22 v23 v22
vau.sub.i16 v27 v31 v27 || cmu.max.u16 v21 v24 v20
vau.sub.i16 v28 v31 v11 
vau.sub.i16 v29 v31 v29 || cmu.max.u16 v10 v21 v22 
vau.sub.i16 v30 v31 v30 
vau.mul.i16 v26 v6 v26 || lsu0.swzv8 [00000000] || cmu.alignvec v20 v2 v12 12 
vau.mul.i16 v27 v6 v27 || lsu0.swzv8 [11111111] || cmu.alignvec v21 v2 v12 14  
vau.mul.i16 v28 v6 v28 || lsu0.swzv8 [22222222] || cmu.alignvec v23 v12 v17 2 
vau.mul.i16 v29 v6 v29 || lsu0.swzv8 [33333333] || cmu.alignvec v24 v12 v17 4 
vau.mul.i16 v30 v6 v30 || lsu0.swzv8 [44444444] || cmu.max.u16 v26 v27 v26 
nop
vau.sub.i16 v20 v31 v20 || cmu.max.u16 v29 v28 v29
vau.sub.i16 v21 v31 v21 || cmu.max.u16 v27 v26 v30
vau.sub.i16 v22 v31 v12 
vau.sub.i16 v23 v31 v23 || cmu.max.u16 v11 v27 v29
vau.sub.i16 v24 v31 v24 
vau.mul.i16 v20 v7 v20 || lsu0.swzv8 [00000000] || cmu.alignvec v26 v3 v13 12 
vau.mul.i16 v21 v7 v21 || lsu0.swzv8 [11111111] || cmu.alignvec v27 v3 v13 14  
vau.mul.i16 v22 v7 v22 || lsu0.swzv8 [22222222] || cmu.alignvec v29 v13 v18 2 
vau.mul.i16 v23 v7 v23 || lsu0.swzv8 [33333333] || cmu.alignvec v30 v13 v18 4 
vau.mul.i16 v24 v7 v24 || lsu0.swzv8 [44444444] || cmu.max.u16 v20 v21 v20
nop
vau.sub.i16 v26 v31 v26 || cmu.max.u16 v22 v23 v22
vau.sub.i16 v27 v31 v27 || cmu.max.u16 v21 v24 v20
vau.sub.i16 v28 v31 v13 
vau.sub.i16 v29 v31 v29 || cmu.max.u16 v12 v21 v22
vau.sub.i16 v30 v31 v30 
vau.mul.i16 v26 v8 v26 || lsu0.swzv8 [00000000] || cmu.alignvec v20 v4 v14 12
vau.mul.i16 v27 v8 v27 || lsu0.swzv8 [11111111] || cmu.alignvec v21 v4 v14 14 
vau.mul.i16 v28 v8 v28 || lsu0.swzv8 [22222222] || cmu.alignvec v23 v14 v19 2
vau.mul.i16 v29 v8 v29 || lsu0.swzv8 [33333333] || cmu.alignvec v24 v14 v19 4
vau.mul.i16 v30 v8 v30 || lsu0.swzv8 [44444444] 
nop
vau.sub.i16 v20 v31 v20 || cmu.max.u16 v26 v27 v26 
vau.sub.i16 v21 v31 v21 || cmu.max.u16 v29 v28 v29
vau.sub.i16 v22 v31 v14 || cmu.max.u16 v27 v26 v30
vau.sub.i16 v23 v31 v23 
vau.sub.i16 v24 v31 v24 || cmu.max.u16 v13 v27 v29 
vau.mul.i16 v20 v9 v20 || lsu0.swzv8 [00000000]
vau.mul.i16 v21 v9 v21 || lsu0.swzv8 [11111111]  
vau.mul.i16 v22 v9 v22 || lsu0.swzv8 [22222222] 
vau.mul.i16 v23 v9 v23 || lsu0.swzv8 [33333333] 
vau.mul.i16 v24 v9 v24 || lsu0.swzv8 [44444444] 
nop 3
cmu.max.u16 v20 v21 v20 
cmu.max.u16 v22 v23 v22
cmu.max.u16 v21 v24 v20
cmu.max.u16 v10 v10 v11
cmu.max.u16 v14 v21 v22 
cmu.max.u16 v12 v12 v13
cmu.max.u16 v14 v14 v10  
nop
cmu.max.u16 v11 v14 v12 
nop
vau.sub.i16 v13 v31 v11
nop
lsu0.st.128.u16.u8 v13 i17 

 erode5x5_final:
	LSU0.LD.64.H  v31 i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v31 i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v30 i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.L  v30 i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v29 i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v29 i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v28 i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v28 i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v27 i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v27 i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v26 i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v26 i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v25 i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v25 i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v24 i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v24 i19 || IAU.ADD i19 i19 8 	

	
BRU.JMP i30
NOP 6
.nowarnend
.size mvcvErode5x5_asm,.-mvcvErode5x5_asm
.end
