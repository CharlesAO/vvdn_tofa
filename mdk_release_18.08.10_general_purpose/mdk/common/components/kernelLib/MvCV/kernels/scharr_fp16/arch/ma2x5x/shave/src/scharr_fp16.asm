///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvScharr_fp16_asm,@function
.version 00.50.05
.data .rodata.scharr
.align 16

.code .text.scharr //text
.align 16

mvcvScharr_fp16_asm:
//void mvcvScharr_fp16(half** in, half** out, u32 width)
//                           i18        i17       i16

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


lsu0.ldi.64 i0 i18
lsu0.ld.32 i2 i18
lsu0.ld.32 i17 i17
lsu0.ldil i13 ___loop || lsu1.ldih i13 ___loop
lsu0.ldil i3 0x03     || lsu1.ldih i3 0x00
lsu0.ldil i4 0x0a     || lsu1.ldih i4 0x00
cmu.cpivr.i16.f16 v10 i3 //full of 3
cmu.cpivr.i16.f16 v11 i4 //full of 10
iau.shr.u32 i16 i16 3

iau.sub i0 i0 0x10
iau.sub i1 i1 0x10
iau.sub i2 i2 0x10

lsu0.ld.64.l v0 i0 || lsu1.ldo.64.h v0 i0 0x08                        
lsu0.ld.64.l v1 i1 || lsu1.ldo.64.h v1 i1 0x08 || iau.add i0 i0 0x10
lsu0.ld.64.l v2 i2 || lsu1.ldo.64.h v2 i2 0x08 || iau.add i1 i1 0x10
lsu0.ld.64.l v3 i0 || lsu1.ldo.64.h v3 i0 0x08 || iau.add i2 i2 0x10
lsu0.ld.64.l v4 i1 || lsu1.ldo.64.h v4 i1 0x08 || iau.add i0 i0 0x10
lsu0.ld.64.l v5 i2 || lsu1.ldo.64.h v5 i2 0x08 || iau.add i1 i1 0x10
lsu0.ld.64.l v6 i0 || lsu1.ldo.64.h v6 i0 0x08 || iau.add i2 i2 0x10
lsu0.ld.64.l v7 i1 || lsu1.ldo.64.h v7 i1 0x08 || iau.add i0 i0 0x10
lsu0.ld.64.l v8 i2 || lsu1.ldo.64.h v8 i2 0x08 || iau.add i1 i1 0x10
iau.add i2 i2 0x10
nop 2


vau.mul.f16 v23 v3 v11  || cmu.alignvec v9 v0 v3 14  
vau.mul.f16 v12 v9 v10  || cmu.alignvec v9 v3 v6 2 
vau.mul.f16 v13 v9 v10  || cmu.alignvec v9 v1 v4 14
vau.mul.f16 v14 v9 v11  || cmu.alignvec v9 v4 v7 2
vau.mul.f16 v15 v9 v11  || cmu.alignvec v9 v2 v5 14
vau.mul.f16 v16 v9 v10  || cmu.alignvec v9 v5 v8 2
vau.mul.f16 v24 v5 v11  || cmu.cpvv v0 v3 || lsu0.ld.64.l v6 i0 || lsu1.ldo.64.h v6 i0 0x08 || iau.add i0 i0 0x10
vau.mul.f16 v17 v9 v10  || cmu.cpvv v1 v4 || lsu0.ld.64.l v7 i1 || lsu1.ldo.64.h v7 i1 0x08 || iau.add i1 i1 0x10
vau.sub.f16 v18 v13 v12 || cmu.cpvv v2 v5 || lsu0.ld.64.l v8 i2 || lsu1.ldo.64.h v8 i2 0x08 || iau.add i2 i2 0x10
vau.sub.f16 v25 v16 v12 || cmu.cpvv v3 v6
vau.sub.f16 v19 v15 v14 || cmu.cpvv v4 v7
vau.sub.f16 v26 v17 v13 || cmu.cpvv v5 v8
vau.sub.f16 v20 v17 v16
vau.add.f16 v21 v18 v19
vau.add.f16 v28 v25 v26
vau.sub.f16 v27 v24 v23
vau.add.f16 v22 v21 v20
nop
vau.add.f16 v29 v28 v27
vau.mul.f16 v15 v22 v22
nop
vau.mul.f16 v16 v29 v29
nop 2
vau.add.f16 v31 v15 v16


vau.mul.f16 v23 v3 v11  || cmu.alignvec v9 v0 v3 14   || bru.rpl i13 i16
vau.mul.f16 v12 v9 v10  || cmu.alignvec v9 v3 v6 2 
vau.mul.f16 v13 v9 v10  || cmu.alignvec v9 v1 v4 14
vau.mul.f16 v14 v9 v11  || cmu.alignvec v9 v4 v7 2
vau.mul.f16 v15 v9 v11  || cmu.alignvec v9 v2 v5 14
vau.mul.f16 v16 v9 v10  || cmu.alignvec v9 v5 v8 2 
vau.mul.f16 v24 v5 v11  || cmu.cpvv v0 v3         || sau.sqt i5 v31.0  
vau.mul.f16 v17 v9 v10  || cmu.cpvv v1 v4         || sau.sqt i6 v31.1 
vau.sub.f16 v18 v13 v12 || cmu.cpvv v2 v5         || sau.sqt i7 v31.2
vau.sub.f16 v25 v16 v12 || cmu.cpvv v3 v6         || sau.sqt i8 v31.3
vau.sub.f16 v19 v15 v14 || cmu.cpvv v4 v7         || sau.sqt i9 v31.4 
vau.sub.f16 v26 v17 v13 || cmu.cpvv v5 v8         || sau.sqt i10 v31.5    || iau.align i5 i5 i5 2
vau.sub.f16 v20 v17 v16 || sau.sqt i11 v31.6      || lsu0.ld.64.l v6 i0   || lsu1.ldo.64.h v6 i0 0x08 || iau.add i0 i0 0x10
vau.add.f16 v21 v18 v19 || sau.sqt i12 v31.7      || lsu0.ld.64.l v7 i1   || lsu1.ldo.64.h v7 i1 0x08 || iau.add i1 i1 0x10
vau.add.f16 v28 v25 v26 || lsu0.ld.64.l v8 i2     || lsu1.ldo.64.h v8 i2 0x08 || iau.add i2 i2 0x10
vau.sub.f16 v27 v24 v23 || iau.align i6 i5 i6 2     
vau.add.f16 v22 v21 v20 || iau.align i7 i7 i7 2   || cmu.cpiv.x32 v30.0 i6
iau.align i8 i7 i8 2 
___loop:
.nowarn 10
vau.add.f16 v29 v28 v27 || iau.align i9 i9 i9 2   || cmu.cpiv.x32 v30.1 i8
.nowarnend
vau.mul.f16 v15 v22 v22 || iau.align i10 i9 i10 2  
iau.align i11 i11 i11 2 || cmu.cpiv.x32 v30.2 i10
vau.mul.f16 v16 v29 v29 || iau.align i12 i11 i12 2
nop 
cmu.cpiv.x32 v30.3 i12
vau.add.f16 v31 v15 v16 || lsu0.st.64.l v30 i17   || lsu1.sto.64.h v30 i17 0x08 || iau.add i17 i17 0x10



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
.size mvcvScharr_fp16_asm,.-mvcvScharr_fp16_asm
.end
