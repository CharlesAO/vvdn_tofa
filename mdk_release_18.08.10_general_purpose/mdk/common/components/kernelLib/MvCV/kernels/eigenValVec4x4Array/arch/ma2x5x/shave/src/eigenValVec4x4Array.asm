///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvEigenValVec4x4Array_asm,@function
.version 00.51.04



.data .data.mvcvEigenValVec4x4Array
.align 4

	 mvcvEigenValVec4x4Array_mat:
		.float32    0, 0, 0, 0
		.float32    0, 0, 0, 0
		.float32    0, 0, 0, 0
		.float32    0, 0, 0, 0
	mvcvEigenValVec4x4Array_val:
		.float32    0, 0, 0, 0
		.float32    0, 0, 0, 0
		.float32    0, 0, 0, 0
		.float32    0, 0, 0, 0
	mvcvEigenValVec4x4Array_vec:
		.float32    0, 0, 0, 0
		.float32    0, 0, 0, 0
		.float32    0, 0, 0, 0
		.float32    0, 0, 0, 0


.code .text.mvcvEigenValVec4x4Array_asm


//  void mvcvEigenValVec4x4Array_asm( R** matrix, R* eigvals, R** eigvec, u32 num_matrix, float threshold)
//                                        i18          i17         i16          i15             i14

mvcvEigenValVec4x4Array_asm:
//debug
//LSU0.ldil i26, 0x0 || LSU1.ldih i26, 0x0
//LSU0.ldil i30, 0x2 || LSU1.ldih i30, 0x0


cmu.cpiv.x32 v14.0 i20 
cmu.cpiv.x32 v14.1 i21
cmu.cpiv.x32 v14.2 i22
cmu.cpiv.x32 v14.3 i23
cmu.cpiv.x32 v15.0 i24
cmu.cpiv.x32 v15.1 i26
cmu.cpiv.x32 v15.2 i27
cmu.cpiv.x32 v15.3 i28
cmu.cpiv.x32 v23.0 i29
cmu.cpiv.x32 v23.1 i31
cmu.cpiv.x32 v23.2 i25

LSU0.ldil i0, mvcvEigenValVec4x4Array_mat || LSU1.ldih i0, mvcvEigenValVec4x4Array_mat
LSU0.ldil i1, mvcvEigenValVec4x4Array_val || LSU1.ldih i1, mvcvEigenValVec4x4Array_val
LSU0.ldil i2, mvcvEigenValVec4x4Array_vec || LSU1.ldih i2, mvcvEigenValVec4x4Array_vec

LSU0.ldil i31, mvcvEigenValVec4x4Array_main || LSU1.ldih i31, mvcvEigenValVec4x4Array_main

//LSU0.ldil i29, 0x126f || LSU1.ldih i29, 0x3a03 // threshold for number of loops
cmu.cpii i29 i14

LSU0.ldil i3, 0x0 || LSU1.ldih i3, 0x3f80

//loop start 

mvcvEigenValVec4x4Array_big:
lsu0.ldi.32 i14 i18 
nop 6


vau.xor v0 v0 v0                             || lsu0.ldo.64.l v19 i14 0  || lsu1.ldo.64.h v19 i14 8       //load of input mat
vau.xor v22 v22 v22 || cmu.cpiv.x32 v0.0 i3  || lsu0.ldo.64.l v18 i14 16 || lsu1.ldo.64.h v18 i14 24
vau.xor v21 v21 v21 || cmu.cpiv.x32 v22.1 i3 || lsu0.ldo.64.l v17 i14 32 || lsu1.ldo.64.h v17 i14 40 
vau.xor v20 v20 v20 || cmu.cpiv.x32 v21.2 i3 || lsu0.ldo.64.l v16 i14 48 || lsu1.ldo.64.h v16 i14 56 
                       cmu.cpiv.x32 v20.3 i3 || lsu0.sto.64.l v0 i2 0    || lsu1.sto.64.h v0 i2 8         //start of the initial eigenvec   



lsu0.sto.64.l v22 i2 16 || lsu1.sto.64.h v22 i2 24
lsu0.sto.64.l v21 i2 32 || lsu1.sto.64.h v21 i2 40 
lsu0.sto.64.l v20 i2 48 || lsu1.sto.64.h v20 i2 56 

lsu0.sto.64.l v19 i0 0  || lsu1.sto.64.h v19 i0 8         //copy of input matrix
lsu0.sto.64.l v18 i0 16 || lsu1.sto.64.h v18 i0 24
lsu0.sto.64.l v17 i0 32 || lsu1.sto.64.h v17 i0 40 
lsu0.sto.64.l v16 i0 48 || lsu1.sto.64.h v16 i0 56 



mvcvEigenValVec4x4Array_main:
// compute absolute ov values
.nowarn 10
lsu0.ldo.32 i9 i0 44 || lsu1.ldo.32 i6 i0 12 || IAU.xor i10 i10 i10
.nowarnend
lsu0.ldo.32 i8 i0 28 || lsu1.ldo.32 i5 i0 8  || IAU.xor i11 i11 i11
lsu0.ldo.32 i7 i0 24 || lsu1.ldo.32 i4 i0 4  || IAU.xor i12 i12 i12
nop 6

cmu.cmiiabs.f32 i9 i10                                               
peu.pc1c gt || SAU.ABS.f32 i10 i9 || LSU0.ldil i11, 2 || LSU1.ldil i12, 3

cmu.cmiiabs.f32 i8 i10                                               
peu.pc1c gt || SAU.ABS.f32 i10 i8 || LSU0.ldil i11, 1 || LSU1.ldil i12, 3

cmu.cmiiabs.f32 i7 i10                                               
peu.pc1c gt || SAU.ABS.f32 i10 i7 || LSU0.ldil i11, 1 || LSU1.ldil i12, 2

cmu.cmiiabs.f32 i6 i10                                               
peu.pc1c gt || SAU.ABS.f32 i10 i6 || LSU0.ldil i11, 0 || LSU1.ldil i12, 3

cmu.cmiiabs.f32 i5 i10                                               
peu.pc1c gt || SAU.ABS.f32 i10 i5 || LSU0.ldil i11, 0 || LSU1.ldil i12, 2

cmu.cmiiabs.f32 i4 i10
peu.pc1c gt || SAU.ABS.f32 i10 i4 || LSU0.ldil i11, 0 || LSU1.ldil i12, 1
                        
cmu.cmii.f32 i10 i29
peu.pc1c lt || bru.bra mvcvEigenValVec4x4Array_exit


IAU.SHL i13 i11 4 // i* 16
IAU.SHL i14 i12 2 // j* 4
IAU.SHL i20 i11 2 // i * 4
IAU.SHL i21 i12 4 // j*16

IAU.ADD i22 i13 i14 //ij
IAU.ADD i23 i13 i20 //ii
iau.add i24 i14 i21 //jj

IAU.ADD i22  i22 i0 || LSU0.ldil i4, 0x59df || LSU1.ldih i4, 0x5f37
IAU.ADD i23  i23 i0 || lsu0.ld.32 i22 i22
IAU.ADD i24  i24 i0 || lsu0.ld.32 i23 i23
                       lsu0.ld.32 i24 i24
nop 6
sau.sub.f32 i25 i23 i24
nop 2
sau.div.f32 i23 i22 i25
nop 11
sau.mul.f32 i23 i23 2.0
nop 2
sau.mul.f32 i22 i23 i23 
nop 2
sau.add.f32 i22 i22 1.0 
nop 2

sau.mul.f32 i24 i22 0.5 || IAU.SHR.u32 i5 i22 1
IAU.SUB i6 i4 i5
sau.mul.f32 i7 i6 i6 || LSU0.ldil i4, 0x0 || LSU1.ldih i4, 0x3fc0
nop 2
sau.mul.f32 i8 i7 i24
nop 2
sau.sub.f32 i8 i4 i8
nop 2
sau.mul.f32 i8 i8 i6
nop 2	
sau.mul.f32 i9 i8 i8
nop 2
sau.mul.f32 i9 i9 i24
nop 2
sau.sub.f32 i9 i4 i9
nop 2
sau.mul.f32 i9 i9 i8
nop 2	
sau.mul.f32 i24 i9 i22
nop 2	
sau.sub.f32 i24 i24 1.0
nop 2
sau.div.f32 i22 i24 i23
nop 9
cmu.cpii.f32.f16 i27 i23
LSU0.ldil i24, 0x0 || LSU1.ldih i24, 0x3f80 
cmu.cpii.f32.f16 i22 i22

cmu.cmz.f32 i25 || LSU0.ldil i23, 0x3C00 || LSU1.ldih i23, 0x3f80 
peu.pc1c eq || cmu.cpii i27 i23 || lsu0.cp i22 i23

sau.atn i28 i22.l
nop 3


CMU.CMiiabs.f16 i27 i23
peu.pc1c lt || sau.atn i28 i27.l
nop 4
peu.pc1c lt || sau.mul.f16 i28 i28 0.5
nop 

CMU.CMZ.f16 i27
peu.pc1c eq || iau.xor i28 i28 i28



sau.sin i28 i28.l  //s
sau.cos i4 i28.l  //c
nop 3

cmu.cpii.f16l.f32 i28 i28 //s
cmu.cpii.f16l.f32 i4 i4 //c

cmu.cpivr.x32 v2 i28 || IAU.ADD i22 i0  i14 
cmu.cpivr.x32 v3 i4  || IAU.ADD i23 i0  i20 


lsu0.ldo.32 i4 i22  0 ||  lsu1.ldo.32 i8 i23   0
lsu0.ldo.32 i5 i22 16 ||  lsu1.ldo.32 i9 i23  16
lsu0.ldo.32 i6 i22 32 ||  lsu1.ldo.32 i10 i23 32
lsu0.ldo.32 i7 i22 48 ||  lsu1.ldo.32 i24 i23 48 
nop 3

cmu.cpiv.x32 v1.0 i4 || lsu0.cp v0.0 i8 //i
cmu.cpiv.x32 v1.1 i5 || lsu0.cp v0.1 i9 
cmu.cpiv.x32 v1.2 i6 || lsu0.cp v0.2 i10
cmu.cpiv.x32 v1.3 i7 || lsu0.cp v0.3 i24

vau.mul.f32 v4 v0 v2 //s i
vau.mul.f32 v5 v0 v3 //c i 
vau.mul.f32 v6 v1 v2 //s j
vau.mul.f32 v7 v1 v3 //c j
nop 

vau.add.f32 v8 v5 v6 // ci+sj
vau.sub.f32 v9 v7 v4 // cj-si
nop 2


cmu.cpvi.x32  i4 v9.0 || lsu0.cp  i8  v8.0//i
cmu.cpvi.x32  i5 v9.1 || lsu0.cp  i9  v8.1
cmu.cpvi.x32  i6 v9.2 || lsu0.cp  i10 v8.2
cmu.cpvi.x32  i7 v9.3 || lsu0.cp  i24 v8.3

lsu0.sto.32 i4 i22  0 ||  lsu1.sto.32 i8 i23   0
lsu0.sto.32 i5 i22 16 ||  lsu1.sto.32 i9 i23  16 || IAU.ADD i4 i0 i13 //mat
lsu0.sto.32 i6 i22 32 ||  lsu1.sto.32 i10 i23 32 || IAU.ADD i5 i0 i21
lsu0.sto.32 i7 i22 48 ||  lsu1.sto.32 i24 i23 48 || IAU.ADD i6 i2 i13 //vec


IAU.ADD i7 i2 i21


lsu0.ldo.64.l v0 i4 0 || lsu1.ldo.64.h v0 i4 8 //i mat
lsu0.ldo.64.l v1 i5 0 || lsu1.ldo.64.h v1 i5 8 //j
lsu0.ldo.64.l v4 i6 0 || lsu1.ldo.64.h v4 i6 8 //i vec
lsu0.ldo.64.l v5 i7 0 || lsu1.ldo.64.h v5 i7 8 //j
nop 3
//
vau.mul.f32 v6 v0 v3 //i *c //mat
vau.mul.f32 v7 v1 v2 //j *s //mat
vau.mul.f32 v8 v4 v3 //i *c vec
vau.mul.f32 v9 v5 v2 //j *s vec

vau.add.f32 v7 v6 v7 // c * i + s * j
vau.mul.f32 v10 v0 v2 //i *s
vau.mul.f32 v11 v1 v3 //j *c
vau.mul.f32 v12 v4 v2 //i *s
vau.mul.f32 v13 v5 v3 //j *c

bru.bra mvcvEigenValVec4x4Array_main
vau.add.f32 v9 v8 v9 //w
vau.sub.f32 v11 v11  v10  //mat
vau.sub.f32 v13 v13  v12  || lsu0.sto.64.l v7  i4 0 || lsu1.sto.64.h v7  i4 8
lsu0.sto.64.l v9 i6 0 || lsu1.sto.64.h v9 i6 8
lsu0.sto.64.l v11  i5 0 || lsu1.sto.64.h v11  i5 8
lsu0.sto.64.l v13 i7 0 || lsu1.sto.64.h v13 i7 8




mvcvEigenValVec4x4Array_exit:
lsu0.ldo.32 i4 i0 0 
lsu0.ldo.32 i5 i0 20 
lsu0.ldi.32 i13 i16 
lsu0.ldo.32 i6 i0 40 
lsu0.ldo.32 i7 i0 60 
nop 4

IAU.XOR i8 i8 i8
cmu.cmii.f32 i5 i4
peu.pc1c lt || cmu.cpii i4 i5 || LSU0.ldil i8, 0x1 

cmu.cmii.f32 i6 i4
peu.pc1c lt || cmu.cpii i4 i6 || LSU0.ldil i8, 0x2 

cmu.cmii.f32 i7 i4
peu.pc1c lt || cmu.cpii i4 i7 || LSU0.ldil i8, 0x3 

IAU.SHL i8 i8 4
IAU.ADD i9 i2 i8 
lsu0.ldo.64.l v0 i9 0 || lsu1.ldo.64.h v0 i9 8 || iau.sub i15 i15 1
peu.pc1i neq || bru.bra mvcvEigenValVec4x4Array_big
nop 4

lsu0.sti.32 i4 i17 
lsu0.sto.64.l v0 i13 0 || lsu1.sto.64.h v0 i13 8 





cmu.cpvi.x32  i20 v14.0
cmu.cpvi.x32  i21 v14.1
cmu.cpvi.x32  i22 v14.2
cmu.cpvi.x32  i23 v14.3
cmu.cpvi.x32  i24 v15.0
cmu.cpvi.x32  i26 v15.1
cmu.cpvi.x32  i27 v15.2
cmu.cpvi.x32  i28 v15.3
cmu.cpvi.x32  i29 v23.0
cmu.cpvi.x32  i31 v23.1
cmu.cpvi.x32  i25 v23.2


BRU.JMP i30
NOP 6



.size mvcvEigenValVec4x4Array_asm,.-mvcvEigenValVec4x4Array_asm
.end
