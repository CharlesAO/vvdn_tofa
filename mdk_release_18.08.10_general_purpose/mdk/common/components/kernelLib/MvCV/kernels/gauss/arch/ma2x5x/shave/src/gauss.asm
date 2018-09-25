///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvGauss_asm,@function
.version 00.50.05

.data .rodata.gauss 
.align 16
 gauss_vect:
		.float16    1.4F16, 3.0F16, 3.8F16, 0.00880503144654088050314465408805F16, 0.01886792452830188679245283018868F16, 0.02389937106918238993710691823899F16


.code .text.gauss //text
.align 16

mvcvGauss_asm:
//void gauss(u8** inLine, u8** out, u32 width)//
//                 i18      i17        i16      
	LSU0.ldil i1, gauss_vect || LSU1.ldih i1, gauss_vect
	lsu0.ld.64.l v23 i1 || IAU.ADD i1 i1 8
	lsu0.ld.64.h v23 i1
	LSU0.LDi.64 i0 i18            
	LSU0.LDi.64 i2 i18            
	LSU0.LD.32 i4 i18            
	LSU0.LD.32 i17 i17
	nop 3

IAU.SHR.u32  i16 i16 3
iau.sub i0 i0 8
iau.sub i1 i1 8
iau.sub i2 i2 8
iau.sub i3 i3 8
iau.sub i4 i4 8


LSU0.ldil i15, gauss_loop || LSU1.ldih i15, gauss_loop


LSU0.LD.128.u8f.f16  v0  i0 || IAU.ADD i0 i0 8
LSU0.LD.128.u8f.f16  v3  i1 || IAU.ADD i1 i1 8
LSU0.LD.128.u8f.f16  v6  i2 || IAU.ADD i2 i2 8
LSU0.LD.128.u8f.f16  v9  i3 || IAU.ADD i3 i3 8
LSU0.LD.128.u8f.f16 v12  i4 || IAU.ADD i4 i4 8              
LSU0.LD.128.u8f.f16  v1  i0 || IAU.ADD i0 i0 8
LSU0.LD.128.u8f.f16  v4  i1 || IAU.ADD i1 i1 8
LSU0.LD.128.u8f.f16  v7  i2 || IAU.ADD i2 i2 8
LSU0.LD.128.u8f.f16 v10  i3 || IAU.ADD i3 i3 8
LSU0.LD.128.u8f.f16 v13  i4 || IAU.ADD i4 i4 8
nop 


vau.add.f16 v15 v0 v12
vau.add.f16 v16 v3  v9
vau.mul.f16 v17 v23 v6  || lsu0.swzv8 [22222222]  
vau.mul.f16 v19 v23 v15 || lsu0.swzv8 [00000000]
vau.mul.f16 v20 v23 v16 || lsu0.swzv8 [11111111]
vau.add.f16 v15 v1 v13
vau.add.f16 v19 v17 v19
vau.add.f16 v16 v4  v10
vau.mul.f16 v17 v23 v7  || lsu0.swzv8 [22222222]  
vau.add.f16 v20 v20 v19
vau.mul.f16 v19 v23 v15 || lsu0.swzv8 [00000000]
vau.mul.f16 v21 v23 v16 || lsu0.swzv8 [11111111]
nop
vau.add.f16 v19 v17 v19
nop 2
vau.add.f16 v21 v21 v19



LSU0.LD.128.u8f.f16  v2  i0 || IAU.ADD i0 i0 8 
LSU0.LD.128.u8f.f16 v14  i4 || IAU.ADD i4 i4 8
LSU0.LD.128.u8f.f16  v5  i1 || IAU.ADD i1 i1 8
LSU0.LD.128.u8f.f16 v11  i3 || IAU.ADD i3 i3 8
LSU0.LD.128.u8f.f16  v8  i2 || IAU.ADD i2 i2 8
nop 4


vau.add.f16 v15 v2 v14
vau.add.f16 v16 v5  v11
vau.mul.f16 v17 v23 v8  || lsu0.swzv8 [22222222]  
vau.mul.f16 v19 v23 v15 || lsu0.swzv8 [00000000]
vau.mul.f16 v22 v23 v16 || lsu0.swzv8 [11111111]
nop
vau.add.f16 v19 v17 v19 || bru.rpl i15 i16
nop 2
vau.add.f16 v22 v22 v19       || LSU0.LD.128.u8f.f16  v2  i0 || IAU.ADD i0 i0 8
cmu.alignvec v0 v20 v21 12    || LSU0.LD.128.u8f.f16 v14  i4 || IAU.ADD i4 i4 8
cmu.alignvec v1 v20 v21 14    || LSU0.LD.128.u8f.f16  v5  i1 || IAU.ADD i1 i1 8
cmu.alignvec v3 v21 v22 2     || LSU0.LD.128.u8f.f16 v11  i3 || IAU.ADD i3 i3 8
cmu.alignvec v4 v21 v22 4     || LSU0.LD.128.u8f.f16  v8  i2 || IAU.ADD i2 i2 8
VAU.ADD.f16  v6 v0 v4
VAU.ADD.f16  v9 v1 v3
VAU.mul.f16 v10 v23 v21 || lsu0.swzv8 [55555555]  
VAU.mul.f16 v12 v23  v6 || lsu0.swzv8 [33333333]  || cmu.cpvv v20 v21 
VAU.mul.f16  v7 v23  v9 || lsu0.swzv8 [44444444]  || cmu.cpvv v21 v22
vau.add.f16 v15 v2 v14
 gauss_loop:
 .nowarn 10
vau.add.f16 v12 v10 v12
.nowarnend
vau.add.f16 v16 v5  v11
vau.mul.f16 v17 v23 v8  || lsu0.swzv8 [22222222]  
vau.add.f16 v7   v7 v12
vau.mul.f16 v19 v23 v15 || lsu0.swzv8 [00000000]
vau.mul.f16 v22 v23 v16 || lsu0.swzv8 [11111111]
LSU0.ST.128.f16.u8f v7 i17 || IAU.ADD i17 i17 8


	



	bru.jmp i30
nop 6

	
.size mvcvGauss_asm,.-mvcvGauss_asm
.end
