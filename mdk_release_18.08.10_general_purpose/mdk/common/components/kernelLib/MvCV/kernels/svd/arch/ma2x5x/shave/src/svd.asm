/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///
////      Singular value decomposition
.type mvcvSVD_asm,@function
.version 00.51.04

.data .data.svd
.align 4
//////////////eps = 0.000000000000001 =(float) (1e-15)//
//////////////tol = 0.1f*eps
//////////////e2 = 10.0f*rank*eps*eps
svd_consts:
 .float    0.0000000000000001, 0.00000000000000000000000000001
////////////              tol                  e2/rank
 
.code .text.svd

 
//void svd(float **A, float *S, float **V, int rank)//
//                i18       i17        i16      i15 
.lalign
mvcvSVD_asm:
.nowarn 10,40
    IAU.SUB i19 i19 16 
	LSU0.STo.64.l v24  i19 0  || LSU1.STo.64.h v24  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v25  i19 0  || LSU1.STo.64.h v25  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v26  i19 0  || LSU1.STo.64.h v26  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v27  i19 0  || LSU1.STo.64.h v27  i19 8 || IAU.SUB i19 i19 16 
	LSU0.STo.64.l v28  i19 0  || LSU1.STo.64.h v28  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v29  i19 0  || LSU1.STo.64.h v29  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v30  i19 0  || LSU1.STo.64.h v30  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v31  i19 0  || LSU1.STo.64.h v31  i19 8 || IAU.SUB i19 i19 8
	LSU0.ST.64  i20  i19      || IAU.SUB i19 i19 8
	LSU1.ST.64  i22  i19      || IAU.SUB i19 i19 8
	LSU0.ST.64  i24  i19      || IAU.SUB i19 i19 8
	LSU1.ST.64  i26  i19      || IAU.SUB i19 i19 8
    LSU0.ST.64  i28  i19      || IAU.SUB i19 i19 8
	LSU1.ST.64  i30  i19

iau.sub i0,i15,3
peu.pc1i EQ || bru.bra label_rank3
nop 6
iau.sub i1,i15,9
peu.pc1i EQ || bru.bra label_rank9
nop 6 

label_rank: 
 
iau.xor i0,i0,i0
iau.add i1,i0,1      || cmu.cpii.i32.f32 i0,i0
cmu.cpii.i32.f32 i1,i1
 
iau.xor i2 i2 i2  
 
label_fori:

cmu.cmii.i32 i2,i15  || iau.shl i4,i2,2
peu.pccx.EQ 0x0      || bru.bra label_while  || iau.add i5,i4,i16 
lsu0.ld.32 i5,i5     || iau.xor i3 i3 i3
nop 5
 
label_forj: 

iau.shl i6,i3,2
iau.add i6,i6,i5    || cmu.cmii.i32 i3,i2
peu.pc1c NEQ        || lsu0.st.32 i0 i6      || iau.incs i3,1
peu.pc1c EQ         || lsu0.st.32 i1 i6      || iau.incs i3,1 

cmu.cmii.i32 i3,i15
peu.pc1c LT         || bru.bra label_forj
peu.pc1c EQ         || bru.bra label_fori    || iau.incs i2,1
nop 6
     
label_while:

nop 
lsu0.ldil i0,120    || lsu1.ldih i0,0        || iau.add i3,i15,0//// i0=EstColRank i3 = RotCount
cmu.cmii.i32 i15,i0 || iau.add i0,i15,0 
peu.pc1c LT         || lsu0.ldil i1,30       || lsu1.ldih i1,0  
peu.pc1c GTE        || iau.shr.i32 i1,i15,2            ///////i1= slimit
nop	
  
label_while1:
////while (RotCount != 0 && SweepCount++ <= slimit)
 
cmu.cmz.i32 i3      || iau.incs i1,-1
PEU.PCC0I.OR EQ EQ  || bru.bra label_sqrt_S
nop 6

iau.sub i2,i0,1 ////i2= EstColRank-1
iau.mul i3,i2,i0
nop 
iau.shr.i32 i3,i3,1 || sau.xor i4,i4,i4 //i4=j
 
label_forj2:

iau.add i5,i4,1     //i5=k
 
label_fork2: 

iau.xor i6,i6,i6    || sau.xor i7,i7,i7  //q//p
iau.xor i8,i8,i8    || sau.xor i9,i9,i9  || cmu.cpii i10,i18 //i  //r

lsu0.ldi.32 i23,i10 || iau.shl i11,i4,2  || sau.shl.x32 i12,i5,2
nop 2
lsu0.ldi.32 i23,i10  
nop 3
iau.add i13,i11,i23
iau.add i14,i12,i23 || lsu0.ld.32 i25,i13 ////x0
lsu0.ld.32 i24,i14  || iau.incs i9,1   
iau.add i13,i11,i23 || sau.add.i32 i14,i12,i23
nop 4
 
label_fori2:

sau.mul.f32 i21 i25,i25 || lsu0.ldi.32 i23,i10  || cmu.cmii.i32 i9,i15   || iau.incs i9,1 
sau.mul.f32 i20 i25,i24 || lsu0.ld.32 i25,i13   || peu.pccx.NEQ 0x0      || bru.bra label_fori2  ////x0  
sau.mul.f32 i22,i24,i24 || lsu0.ld.32 i24,i14   
sau.add.f32 i7,i7,i21   
sau.add.f32 i6,i6,i20    
sau.add.f32 i8,i8,i22   
nop 
iau.add i13,i11,i23     || sau.add.i32 i14,i12,i23    
iau.add i11,i11,i17
iau.add i12,i12,i17     || lsu0.st.32 i7 i11     || lsu1.ldil i13,svd_consts
lsu0.st.32 i8 i12       || lsu1.ldih i13,svd_consts  
 
label_qr:

cmu.cmii.f32 i7,i8 
peu.pc1c LT              || bru.bra label_rq

sau.abs.f32 i21,i6       || lsu0.ldo.32 i14,i13,0x0 || lsu1.ldo.32 i13,i13,0x4
lsu0.ldo.32 i20,i17,0x0 
nop 5
sau.mul.f32 i14,i14,i7
sau.mul.f32 i13,i13,i20//i20 = S[0]
nop 2

cmu.cmii.f32 i7 i13   
peu.andacc      || cmu.cmii.f32 i21 i14 
peu.pc1c GT     || bru.bra label_else_qr 
peu.pc1c LTE    || iau.incs i3,-1                   || bru.bra label_rq  
nop 6
    
label_else_qr:

sau.div.f32 i8,i8,i7  || lsu0.ldil i22,0x1          || lsu1.ldih i22,0x0
sau.div.f32 i6,i6,i7  || cmu.cpii.i32.f32 i22,i22
nop 6
////////////i0-i6 ,14-i18
cmu.cpiv   v0.0 i0    || lsu0.cp v0.1  i1           || lsu1.cp v0.2 i2
cmu.cpiv   v0.3 i3    || lsu0.cp v1.0  i4           || lsu1.cp v1.1 i5
lsu0.cp v1.3  i14     || lsu1.cp v2.0 i15
cmu.cpiv   v2.1 i16   || lsu0.cp v2.2  i17          || lsu1.cp v2.3 i18
sau.sub.f32 i8,i22,i8 || lsu0.ldil i29  label_sqrt  || lsu1.ldih i29  label_sqrt 
nop
sau.mul.f32 i23,i6,i6 || lsu0.ldil i22,0x4          || lsu1.ldih i22,0x0 || cmu.cpiv v1.2 i6 
sau.mul.f32 i24,i8,i8 || cmu.cpii.i32.f32 i22,i22   || bru.swp i29
nop
sau.mul.f32 i23,i23,i22 
nop 2
sau.add.f32 i18,i23,i24 
nop 

sau.div.f32 i23,i8,i18 || cmu.cpii i24,i18 || lsu0.ldil i29  label_sqrt   || lsu1.ldih i29  label_sqrt //vt
nop 9
bru.swp i29
nop
sau.add.f32 i23,i23,1.0
nop 2
sau.mul.f32 i18,i23,0.5
nop 
sau.mul.f32 i24,i24,i18 || cmu.cpvi  i6  v1.2 
nop 2
sau.div.f32 i24,i6,i24  || cmu.cpii i25,i18//c0 //s0
////////////////////i0-i6 ,14-i18
cmu.cpvi  i0  v0.0   || lsu0.cp  i1  v0.1 || lsu1.cp  i2    v0.2
cmu.cpvi  i3  v0.3   || lsu0.cp  i4  v1.0 || lsu1.cp  i5    v1.1
lsu0.cp  i14 v1.3    || lsu1.cp  i15   v2.0
cmu.cpvi  i16 v2.1   || lsu0.cp  i17 v2.2 || lsu1.cp  i18   v2.3 || iau.xor i9,i9,i9  //i

iau.shl i10,i9,2 
iau.add i28,i10,i16 
iau.add i10,i10,i18  || lsu0.ld.32 i28,i28
lsu0.ld.32 i10,i10   || iau.shl i11,i4,2
iau.shl i12,i5,2
nop 4
cmu.cpiv   v0.0 i6   || lsu1.cp   v0.1 i7
 
label_fori3:

iau.add i13,i11,i10
iau.add i14,i12,i10  || lsu0.ld.32 i26,i13 ////d1
lsu0.ld.32 i27,i14   || iau.add i11,i11,i28 ////d2
iau.add i12,i12,i28  || lsu0.ld.32 i6,i11 ////d1//
lsu0.ld.32 i7,i12    || iau.incs i9,1 ////d2
nop 3
sau.mul.f32 i20 i26,i25 || iau.shl i10,i9,2 
sau.mul.f32 i21 i27,i24 || iau.add i28,i10,i16 
sau.mul.f32 i22,i26,i24 || iau.add i10,i10,i18  || lsu0.ld.32 i28,i28
sau.mul.f32 i23,i27,i25 || lsu0.ld.32 i10,i10
sau.add.f32 i26,i20,i21
sau.mul.f32 i20 i6,i25 
sau.sub.f32 i27,i23,i22
sau.mul.f32 i21 i7,i24
sau.mul.f32 i22,i6,i24 || lsu0.st.32 i26 i13    ////// A[i][j]
sau.mul.f32 i23,i7,i25 || lsu0.st.32 i27 i14    ////// A[i][k]
sau.add.f32 i6,i20,i21
nop 
sau.sub.f32 i7,i23,i22
nop 
lsu0.st.32 i6 i11      || iau.shl i11,i4,2 //////  V[i][j]
lsu0.st.32 i7 i12      || iau.shl i12,i5,2 //////  V[i][k]

cmu.cmii.i32 i9,i15
peu.pc1c LT    || bru.bra label_fori3
peu.pc1c GTE   || bru.bra label_while2    || cmu.cpvi  i6 v0.0  ||  lsu0.cp i7 v0.1
nop 6    
 
label_rq:

sau.div.f32 i6,i6,i8
sau.div.f32 i7,i7,i8
nop 11
sau.sub.f32 i7,i7,1.0
nop 2

sau.mul.f32 i23,i6,i6  || lsu0.ldil i22,0x4   || lsu1.ldih i22,0x0
sau.mul.f32 i24,i7,i7  || cmu.cpii.i32.f32 i22,i22
nop 2
sau.mul.f32 i23,i23,i22
nop 2
sau.add.f32 i23,i23,i24
nop 2 
//////i0-i6 ,14-i18
cmu.cpiv   v0.0 i0     || lsu0.cp v0.1  i1    || lsu1.cp v0.2 i2
cmu.cpiv   v0.3 i3     || lsu0.cp v1.0  i4    || lsu1.cp v1.1 i5
cmu.cpiv   v1.2 i6     || lsu0.cp v1.3  i14   || lsu1.cp v2.0 i15
cmu.cpiv   v2.1 i16    || lsu0.cp v2.2  i17   || lsu1.cp v2.3 i18

cmu.cpii i18,i23
lsu0.ldil i29  label_sqrt || lsu1.ldih i29  label_sqrt 
bru.swp i29
nop 6
cmu.cpii i25,i18//vt

sau.div.f32 i23,i7,i25    || lsu0.ldil i22,0x1 || lsu1.ldih i22,0x0
cmu.cpii.i32.f32 i22,i22
nop 11
sau.sub.f32 i24,i22,i23
nop 2 
sau.mul.f32 i24,i24,0.5
nop 2 
cmu.cpii i18,i24
lsu0.ldil i29  label_sqrt || lsu1.ldih i29  label_sqrt 
bru.swp i29
nop 6
cmu.cpii i24,i18          || lsu0.ldil i22,0x0 || lsu1.ldih i22,0x0//s0
cmu.cpii.i32.f32 i22,i22
nop

//i0-i6 ,14-i18
cmu.cpvi  i0  v0.0  || lsu0.cp  i1  v0.1 || lsu1.cp  i2    v0.2
cmu.cpvi  i3  v0.3  || lsu0.cp  i4  v1.0 || lsu1.cp  i5    v1.1
cmu.cpvi  i6  v1.2  || lsu0.cp  i14 v1.3 || lsu1.cp  i15   v2.0
cmu.cpvi  i16 v2.1  || lsu0.cp  i17 v2.2 || lsu1.cp  i18   v2.3

cmu.cmz.f32 i6
peu.pc1c LT              || sau.sub.f32 i24,i22,i24 //s0
nop 2 
sau.mul.f32 i25,i25,i24
nop 2 
sau.div.f32 i25,i6,i25 
nop 11

iau.xor i9,i9,i9  //i

iau.shl i10,i9,2 
iau.add i28,i10,i16 
iau.add i10,i10,i18  || lsu0.ld.32 i28,i28
lsu0.ld.32 i10,i10   || iau.shl i11,i4,2
iau.shl i12,i5,2
nop 4
cmu.cpiv   v0.0 i6   || lsu1.cp   v0.1 i7
 
label_fori4:

iau.add i13,i11,i10
iau.add i14,i12,i10  || lsu0.ld.32 i26,i13 ////d1
lsu0.ld.32 i27,i14   || iau.add i11,i11,i28 ////d2
iau.add i12,i12,i28  || lsu0.ld.32 i6,i11 ////d1//
lsu0.ld.32 i7,i12    || iau.incs i9,1 ////d2
nop 3
sau.mul.f32 i20 i26,i25 || iau.shl i10,i9,2 
sau.mul.f32 i21 i27,i24 || iau.add i28,i10,i16 
sau.mul.f32 i22,i26,i24 || iau.add i10,i10,i18  || lsu0.ld.32 i28,i28
sau.mul.f32 i23,i27,i25 || lsu0.ld.32 i10,i10
sau.add.f32 i26,i20,i21
sau.mul.f32 i20 i6,i25 
sau.sub.f32 i27,i23,i22
sau.mul.f32 i21 i7,i24
sau.mul.f32 i22,i6,i24 || lsu0.st.32 i26 i13    ////// A[i][j]
sau.mul.f32 i23,i7,i25 || lsu0.st.32 i27 i14    ////// A[i][k]
sau.add.f32 i6,i20,i21
nop 
sau.sub.f32 i7,i23,i22
nop 
lsu0.st.32 i6 i11   || iau.shl i11,i4,2 //////  V[i][j]
lsu0.st.32 i7 i12   || iau.shl i12,i5,2//////  V[i][k]

cmu.cmii.i32 i9,i15
peu.pc1c LT    || bru.bra label_fori4
peu.pc1c GTE   || bru.bra label_while2    || cmu.cpvi  i6 v0.0  ||  lsu0.cp i7 v0.1
nop 6    
 
label_while2:
iau.incs i5,1
cmu.cmii.i32 i5,i0
peu.pc1c LT   || bru.bra label_fork2
nop 6

iau.incs i4,1
cmu.cmii.i32 i4,i2
peu.pc1c LT   || bru.bra label_forj2
nop 6

//////	while (EstColRank>2 && S[EstColRank-1]<=S[0]*tol+tol*tol) EstColRank--//
		
label_while3:	
	
lsu0.ldil i13,0x2   || lsu1.ldih i13,0x0	
cmu.cmii.i32 i0,i13 
peu.pc1c LTE        || bru.bra end_while3
nop 6
	
iau.sub i11,i0,1    || lsu0.ldil i13,svd_consts || lsu1.ldih i13,svd_consts  
iau.shl i11,i11,2 
iau.add i11,i11,i17
lsu0.ld.32 i7 i17   || lsu1.ldo.32 i13,i13,0x0
lsu0.ld.32 i8 i11      //S[EstColRank-1]
nop 5	
sau.mul.f32 i7,i7,i13  	
sau.mul.f32 i13,i13,i13  	
nop 2
sau.add.f32 i7,i7,i13  
nop 2

cmu.cmii.f32 i8, i7
peu.pc1c LTE        || iau.incs i0,-1 || bru.bra label_while3
nop 6
 
end_while3:

bru.bra label_while1
nop 6

label_sqrt_S: 
 // for (i = 0// i < rank// i++)
 // {
  // S[i] = sqrt(S[i])//
 // } 
iau.xor i9,i9,i9    || lsu0.cp v2.0 i15          || lsu1.cp v2.1 i17 
  
label_fori5:
iau.shl i11,i9,2    || lsu0.ldil i29  label_sqrt || lsu1.ldih i29  label_sqrt || cmu.cpvi  i17 v2.1
iau.add i11,i11,i17          
lsu0.ld.32 i18 i11  || bru.swp i29               || iau.incs i9,1
nop 6
lsu0.st.32 i18 i11  || cmu.cpvi  i15 v2.0   
cmu.cmii.i32 i9,i15 
peu.pc1c LT         || bru.bra label_fori5
nop  6    
  
bru.bra  label_exit
nop 6  

////#####################         SVD for rank=3    ###############

label_rank3:


iau.xor i0,i0,i0
iau.add i1,i0,1      || cmu.cpii.i32.f32 i0,i0
cmu.cpii.i32.f32 i1,i1
 
iau.xor i2 i2 i2  
 
label_fori_r3:

cmu.cmii.i32 i2,i15  || iau.shl i4,i2,2
peu.pccx.EQ 0x0      || bru.bra label_while_r3  || iau.add i5,i4,i16 
lsu0.ld.32 i5,i5     || iau.xor i3 i3 i3
nop 5
 
label_forj_r3: 

iau.shl i6,i3,2
iau.add i6,i6,i5    || cmu.cmii.i32 i3,i2
peu.pc1c NEQ        || lsu0.st.32 i0 i6      || iau.incs i3,1
peu.pc1c EQ         || lsu0.st.32 i1 i6      || iau.incs i3,1 

cmu.cmii.i32 i3,i15
peu.pc1c LT         || bru.bra label_forj_r3
peu.pc1c EQ         || bru.bra label_fori_r3 || iau.incs i2,1
nop 6
     
label_while_r3:

nop 
lsu0.ldil i0,120    || lsu1.ldih i0,0      || iau.add i3,i15,0//// i0=EstColRank i3 = RotCount
cmu.cmii.i32 i15,i0 || iau.add i0,i15,0 
peu.pc1c LT         || lsu0.ldil i1,30     || lsu1.ldih i1,0  
peu.pc1c GTE        || iau.shr.i32 i1,i15,2            ///////i1= slimit
nop	

label_while1_r3:
////while (RotCount != 0 && SweepCount++ <= slimit)
cmu.cmz.i32 i3      || iau.incs i1,-1
PEU.PCC0I.OR EQ EQ  || bru.bra label_sqrt_S_r3
nop 6
iau.sub i2,i0,1 ////i2= EstColRank-1
iau.mul i3,i2,i0
nop 
iau.shr.i32 i3,i3,1 || sau.xor i4,i4,i4 //i4=j
  
lsu0.ldo.32 i11,i18,0x0 
lsu0.ldo.32 i12,i18,0x4 
lsu0.ldo.32 i13,i18,0x8 
lsu0.ldo.32 i11,i16,0x0 
lsu0.ldo.32 i12,i16,0x4 
lsu0.ldo.32 i13,i16,0x8 
nop
lsu0.ldxv v0,i11 
lsu1.ldxv v1,i12 
lsu0.ldxv v2,i13 
lsu1.ldxv v4,i11 
lsu0.ldxv v5,i12 
lsu1.ldxv v6,i13 
nop 7
cmu.tp4r v0,v0 
cmu.tp4r v4,v4

label_forj2_r3:

iau.add i5,i4,1     //i5=k
 
label_fork2_r3:
  
iau.xor i6,i6,i6            || sau.xor i7,i7,i7  //p//q
iau.xor i8,i8,i8 //r

vau.mul.f32 v20,v0,v0       || lsu0.swzmv4.word [3210] [ZUUU] || iau.shl i24,i4,2 
vau.mul.f32 v21,v1,v1       || lsu0.swzmv4.word [3210] [ZUUU] || iau.shl i25,i5,2 
vau.mul.f32 v22,v0,v1       || lsu0.swzmv4.word [3210] [ZUUU] || iau.add i24,i24,i17    
sau.sumx.f32 i7,v20         || iau.add i25,i25,i17 
sau.sumx.f32 i8,v21           
sau.sumx.f32 i6,v22           
nop 2
lsu0.st.32 i7,i24            
lsu0.st.32 i8,i25           
nop
label_qr_r3: 

cmu.cmii.f32 i7,i8          || lsu0.ldo.32 i20,i17,0x0  
peu.pc1c LT                 || bru.bra label_rq_r3
sau.abs.f32 i21,i6          || cmu.cpvi i14 v15.0
nop 3
sau.mul.f32 i14,i14,i7      || cmu.cpvi i13 v15.1
sau.mul.f32 i13,i13,i20//i20 = S[0]
nop 

cmu.cmii.f32 i21 i14  
peu.andacc             || cmu.cmii.f32 i7 i13
peu.pc1c GT            || bru.bra label_else_qr_r3 
peu.pc1c LTE           || iau.incs i3,-1      || bru.bra label_while2_r3  
nop 6
    
label_else_qr_r3:

sau.div.f32 i8,i8,i7   || lsu0.ldil i22,0x1    || lsu1.ldih i22,0x0
sau.div.f32 i6,i6,i7   || cmu.cpii.i32.f32 i22,i22
nop 6
////////////i0-i6 ,14-i18
cmu.cpiv   v12.0 i0    || lsu0.cp v12.1  i1    || lsu1.cp v12.2 i2
cmu.cpiv   v12.3 i3    || lsu0.cp v13.0  i4    || lsu1.cp v13.1 i5
lsu0.cp v13.3  i14     || lsu1.cp v14.0 i15
cmu.cpiv v14.1 i16     || lsu0.cp v14.2  i17   || lsu1.cp v14.3 i18
sau.sub.f32 i8,i22,i8  || lsu0.ldil i29  label_sqrt                || lsu1.ldih i29  label_sqrt 
nop
sau.mul.f32 i23,i6,i6  || lsu0.ldil i22,0x4    || lsu1.ldih i22,0x0|| cmu.cpiv v13.2 i6 
sau.mul.f32 i24,i8,i8  || cmu.cpii.i32.f32 i22,i22 || bru.swp i29
nop
sau.mul.f32 i23,i23,i22 
nop 2
sau.add.f32 i18,i23,i24 
nop 
sau.div.f32 i23,i8,i18 || cmu.cpii i24,i18 || lsu0.ldil i29  label_sqrt || lsu1.ldih i29  label_sqrt //vt
nop 9
bru.swp i29
nop
sau.add.f32 i23,i23,1.0
nop 2
sau.mul.f32 i18,i23,0.5
nop 
sau.mul.f32 i24,i24,i18 || cmu.cpvi  i6  v13.2 
nop 2
sau.div.f32 i24,i6,i24  || cmu.cpii i25,i18//c0 //s0
////////////////////i0-i6 ,14-i18
cmu.cpvi  i0  v12.0     || lsu0.cp  i1  v12.1   || lsu1.cp  i2    v12.2
cmu.cpvi  i3  v12.3     || lsu0.cp  i4  v13.0   || lsu1.cp  i5    v13.1
lsu0.cp  i14 v13.3      || lsu1.cp  i15   v14.0
cmu.cpvi  i16 v14.1     || lsu0.cp  i17 v14.2   || lsu1.cp  i18   v14.3 
nop 6
cmu.cpivr.x32 v7,i25 //c0
vau.mul.f32 v8,v0,v7    || cmu.cpivr.x32 v3,i24 //s0
vau.mul.f32 v9,v1,v3
vau.mul.f32 v10,v0,v3 //d1*s0
vau.mul.f32 v11,v1,v7 //d2*c0
vau.add.f32 v0,v8,v9
vau.mul.f32 v8,v4,v7
vau.sub.f32 v1,v11,v10  || bru.bra label_while2_r3 
vau.mul.f32 v9,v5,v3
vau.mul.f32 v10,v4,v3
vau.mul.f32 v11,v5,v7
vau.add.f32 v4,v8,v9
nop
vau.sub.f32 v5,v11,v10
 
label_rq_r3:

sau.div.f32 i6,i6,i8
sau.div.f32 i7,i7,i8
nop 11
sau.sub.f32 i7,i7,1.0
nop 2

sau.mul.f32 i23,i6,i6  || lsu0.ldil i22,0x4   || lsu1.ldih i22,0x0
sau.mul.f32 i24,i7,i7  || cmu.cpii.i32.f32 i22,i22
nop 2
sau.mul.f32 i23,i23,i22
nop 2
sau.add.f32 i23,i23,i24
nop 2 
//////i0-i6 ,14-i18
cmu.cpiv   v12.0 i0    || lsu0.cp v12.1  i1    || lsu1.cp v12.2 i2
cmu.cpiv   v12.3 i3    || lsu0.cp v13.0  i4    || lsu1.cp v13.1 i5
cmu.cpiv   v13.2 i6    || lsu0.cp v13.3  i14   || lsu1.cp v14.0 i15
cmu.cpiv   v14.1 i16   || lsu0.cp v14.2  i17   || lsu1.cp v14.3 i18

cmu.cpii i18,i23
lsu0.ldil i29  label_sqrt || lsu1.ldih i29  label_sqrt 
bru.swp i29
nop 6
cmu.cpii i25,i18//vt

sau.div.f32 i23,i7,i25 || lsu0.ldil i22,0x1    || lsu1.ldih i22,0x0
cmu.cpii.i32.f32 i22,i22
nop 11
sau.sub.f32 i24,i22,i23
nop 2 
sau.mul.f32 i24,i24,0.5
nop 2 
cmu.cpii i18,i24
lsu0.ldil i29  label_sqrt || lsu1.ldih i29  label_sqrt 
bru.swp i29
nop 6
cmu.cpii i24,i18          || lsu0.ldil i22,0x0 || lsu1.ldih i22,0x0//s0
cmu.cpii.i32.f32 i22,i22
nop

//i0-i6 ,14-i18
cmu.cpvi  i0  v12.0   || lsu0.cp  i1  v12.1 || lsu1.cp  i2    v12.2
cmu.cpvi  i3  v12.3   || lsu0.cp  i4  v13.0 || lsu1.cp  i5    v13.1
cmu.cpvi  i6  v13.2   || lsu0.cp  i14 v13.3 || lsu1.cp  i15   v14.0
cmu.cpvi  i16 v14.1   || lsu0.cp  i17 v14.2 || lsu1.cp  i18   v14.3

cmu.cmz.f32 i6
peu.pc1c LT           || sau.sub.f32 i24,i22,i24 //s0
nop 2 
sau.mul.f32 i25,i25,i24
nop 2 
sau.div.f32 i25,i6,i25 
nop 11

cmu.cpivr.x32 v7,i25 //c0
vau.mul.f32 v8,v0,v7  || cmu.cpivr.x32 v3,i24 //s0
vau.mul.f32 v9,v1,v3
vau.mul.f32 v10,v0,v3 //d1*s0
vau.mul.f32 v11,v1,v7 //d2*c0
vau.add.f32 v0,v8,v9
vau.mul.f32 v8,v4,v7
vau.sub.f32 v1,v11,v10 
vau.mul.f32 v9,v5,v3
vau.mul.f32 v10,v4,v3
vau.mul.f32 v11,v5,v7
vau.add.f32 v4,v8,v9
nop
vau.sub.f32 v5,v11,v10
  
label_while2_r3:

iau.incs i5,1
cmu.cmii.i32 i5,i0
peu.pc1c LT     || bru.bra label_fork2_r3
peu.pc1c LT     || cmu.cpvv v16,v1
peu.pc1c LT     || cmu.cpvv v1,v2
peu.pc1c LT     || cmu.cpvv v18,v5
peu.pc1c LT     || cmu.cpvv v5,v6
nop 2

cmu.cpvv v2,v1  || vau.and v6,v5,v5
cmu.cpvv v1,v16 || vau.and v5,v18,v18 || iau.incs i4,1

cmu.cmii.i32 i4,i2
peu.pc1c LT     || bru.bra label_forj2_r3
peu.pc1c LT     || cmu.cpvv v17 v0
peu.pc1c LT     || cmu.cpvv v0,v1
peu.pc1c LT     || cmu.cpvv v1,v2
peu.pc1c LT     || cmu.cpvv v19,v4
peu.pc1c LT     || cmu.cpvv v4,v5
peu.pc1c LT     || cmu.cpvv v5,v6
                
cmu.cpvv v1,v0  || vau.and v0,v17,v17
cmu.cpvv v5,v4  || vau.and v4,v19,v19

//////	while (EstColRank>2 && S[EstColRank-1]<=S[0]*tol+tol*tol) EstColRank--//
 		
cmu.tp4r v0,v0		
cmu.tp4r v4,v4 		
lsu0.ldo.32 i11,i18,0x0 
lsu0.ldo.32 i12,i18,0x4 
lsu0.ldo.32 i13,i18,0x8
lsu0.ldo.32 i11,i16,0x0 
lsu0.ldo.32 i12,i16,0x4 
lsu0.ldo.32 i13,i16,0x8
nop
peu.pl0en4.word 0x7            || lsu0.stxv v0,i11 
peu.pl1en4.word 0x7            || lsu1.stxv v1,i12 
peu.pl0en4.word 0x7            || lsu0.stxv v2,i13 		
peu.pl1en4.word 0x7            || lsu1.stxv v4,i11 
peu.pl0en4.word 0x7            || lsu0.stxv v5,i12 
peu.pl1en4.word 0x7            || lsu1.stxv v6,i13 		
nop 	
		
label_while3_r3:	
	
lsu0.ldil i13,0x2   || lsu1.ldih i13,0x0	
cmu.cmii.i32 i0,i13 
peu.pc1c LTE        || bru.bra end_while3_r3

iau.sub i11,i0,1    || cmu.cpvi i13,v15.0 || lsu0.ld.32 i7 i17   
iau.shl i11,i11,2 
iau.add i11,i11,i17
lsu0.ld.32 i8 i11      //S[EstColRank-1]
nop 2	
sau.mul.f32 i13,i13,i13  
sau.mul.f32 i7,i7,i13  	
nop 2
sau.add.f32 i7,i7,i13  
nop 2

cmu.cmii.f32 i8, i7
peu.pc1c LTE        || iau.incs i0,-1 || bru.bra label_while3_r3
nop 6
 
end_while3_r3:

bru.bra label_while1_r3
nop 6

label_sqrt_S_r3: 
 // for (i = 0// i < rank// i++)
 // {
  // S[i] = sqrt(S[i])//
 // } 
lsu0.ldil i29  label_sqrt_4val || lsu1.ldih i29  label_sqrt_4val 
lsu0.ldxv v23 i17              || bru.swp i29 
nop 6
peu.pl0en4.word 0x7            || lsu0.stxv v23 i17   

bru.bra  label_exit
nop 6

////#####################         SVD for rank=9    ###############

label_rank9:

iau.xor i0,i0,i0
iau.add i1,i0,1      || cmu.cpii.i32.f32 i0,i0
cmu.cpii.i32.f32 i1,i1
 
iau.xor i2 i2 i2  
 
label_fori_r9:

cmu.cmii.i32 i2,i15  || iau.shl i4,i2,2
peu.pccx.EQ 0x0      || bru.bra label_while_r9  || iau.add i5,i4,i16 
lsu0.ld.32 i5,i5     || iau.xor i3 i3 i3
nop 5
 
label_forj_r9: 

iau.shl i6,i3,2
iau.add i6,i6,i5     || cmu.cmii.i32 i3,i2
peu.pc1c NEQ         || lsu0.st.32 i0 i6        || iau.incs i3,1
peu.pc1c EQ          || lsu0.st.32 i1 i6        || iau.incs i3,1 

cmu.cmii.i32 i3,i15
peu.pc1c LT          || bru.bra label_forj_r9
peu.pc1c EQ          || bru.bra label_fori_r9   || iau.incs i2,1
nop 6
     
label_while_r9:

nop 
lsu0.ldil i0,120     || lsu1.ldih i0,0          || iau.add i3,i15,0//// i0=EstColRank i3 = RotCount
cmu.cmii.i32 i15,i0  || iau.add i0,i15,0 
peu.pc1c LT          || lsu0.ldil i1,30         || lsu1.ldih i1,0  
peu.pc1c GTE         || iau.shr.i32 i1,i15,2            ///////i1= slimit
nop	
 
label_while1_r9:
////while (RotCount != 0 && SweepCount++ <= slimit)
 
cmu.cmz.i32 i3       || iau.incs i1,-1
PEU.PCC0I.OR EQ EQ   || bru.bra label_sqrt_S_r9
nop 6 

iau.sub i2,i0,1 ////i2= EstColRank-1
iau.mul i3,i2,i0
nop 
iau.shr.i32 i3,i3,1  || sau.xor i4,i4,i4 //i4=j
 
label_forj2_r9:

iau.add i5,i4,1     //i5=k
 
label_fork2_r9: 
iau.xor i6,i6,i6     || sau.xor i7,i7,i7                              ////q  ////p
iau.xor i8,i8,i8     || sau.xor i9,i9,i9          || cmu.cpii i10,i18 ////i  ////r
                     
lsu1.ldi.32 i23,i10  || iau.shl i11,i4,2          || sau.shl.x32 i12,i5,2
lsu1.ldi.32 i23,i10
lsu1.ldi.32 i23,i10
lsu1.ldi.32 i23,i10
lsu1.ldi.32 i23,i10
lsu1.ldi.32 i23,i10
lsu1.ldi.32 i23,i10
lsu1.ldi.32 i23,i10  || iau.add i13,i11,i23 || sau.add.i32 i14,i12,i23
lsu1.ldi.32 i23,i10  || iau.add i20,i11,i23 || sau.add.i32 i21,i12,i23
                     
label_fori2_r9:      
                     
lsu0.ld.64.l v0 i13  || lsu1.ld.64.h v0,i14 || iau.add i13,i11,i23 || sau.add.i32 i14,i12,i23
lsu0.ld.64.l v1 i20  || lsu1.ld.64.h v1,i21 || iau.add i20,i11,i23 || sau.add.i32 i21,i12,i23
lsu0.ld.64.l v2 i13  || lsu1.ld.64.h v2,i14 || iau.add i13,i11,i23 || sau.add.i32 i14,i12,i23
lsu0.ld.64.l v3 i20  || lsu1.ld.64.h v3,i21 || iau.add i20,i11,i23 || sau.add.i32 i21,i12,i23
lsu0.ld.64.l v4 i13  || lsu1.ld.64.h v4,i14 || iau.add i13,i11,i23 || sau.add.i32 i14,i12,i23
lsu0.ld.64.l v5 i20  || lsu1.ld.64.h v5,i21 || iau.add i20,i11,i23 || sau.add.i32 i21,i12,i23
lsu0.ld.64.l v6 i13  || lsu1.ld.64.h v6,i14 || iau.add i13,i11,i23 || sau.add.i32 i14,i12,i23
lsu0.ld.64.l v7 i20  || lsu1.ld.64.h v7,i21 
lsu0.ld.64.l v8 i13  || lsu1.ld.64.h v8,i14 
                                               cmu.vszm.word v0,v0,[2020]
                                               cmu.vszm.word v1,v1,[2020] || lsu1.SWZMV4.word [2310] [ZUUU] || vau.mul.f32 v9,v0,v0
                                               cmu.vszm.word v2,v2,[2020] || lsu1.SWZMV4.word [2310] [ZUUU] || vau.mul.f32 v10,v1,v1
                                               cmu.vszm.word v3,v3,[2020] || lsu1.SWZMV4.word [2310] [ZUUU] || vau.mul.f32 v11,v2,v2
                                               cmu.vszm.word v4,v4,[2020] || lsu1.SWZMV4.word [2310] [ZUUU] || vau.mul.f32 v12,v3,v3
                                               cmu.vszm.word v5,v5,[2020] || lsu1.SWZMV4.word [2310] [ZUUU] || vau.mul.f32 v13,v4,v4
                                               cmu.vszm.word v6,v6,[2020] || lsu1.SWZMV4.word [2310] [ZUUU] || vau.mul.f32 v14,v5,v5
                                               cmu.vszm.word v7,v7,[2020] || lsu1.SWZMV4.word [2310] [ZUUU] || vau.mul.f32 v15,v6,v6
                                               cmu.vszm.word v8,v8,[2020] || lsu1.SWZMV4.word [2310] [ZUUU] || vau.mul.f32 v16,v7,v7
                                                                            lsu1.SWZMV4.word [2310] [ZUUU] || vau.mul.f32 v17,v8,v8

vau.add.f32 v10,v10,v11		
vau.add.f32 v12,v12,v13		
vau.add.f32 v14,v14,v15	
vau.add.f32 v16,v16,v17	
vau.add.f32 v10,v10,v12	
nop
vau.add.f32 v14,v14,v16																						
vau.add.f32 v9,v9,v10																			
nop 2
vau.add.f32 v9,v9,v14																
lsu1.ldil i13,svd_consts || iau.add i11,i11,i17
lsu1.ldih i13,svd_consts || iau.add i12,i12,i17 
cmu.cpvi.x32 i6 v9.2     || lsu0.cp i7 v9.0          || lsu1.cp i8 v9.1  
lsu0.st.32 i7 i11        || lsu1.st.32 i8 i12    
 
label_qr_r9:

cmu.cmii.f32 i7,i8 
peu.pc1c LT              || bru.bra label_rq_r9

sau.abs.f32 i21,i6       || lsu0.ldo.32 i14,i13,0x0  || lsu1.ldo.32 i13,i13,0x4
lsu0.ldo.32 i20,i17,0x0 
nop 5
sau.mul.f32 i14,i14,i7
sau.mul.f32 i13,i13,i20//i20 = S[0]
nop 2
cmu.cmii.f32 i7 i13   
peu.andacc               || cmu.cmii.f32 i21 i14 
peu.pc1c GT              || bru.bra label_else_qr_r9
peu.pc1c LTE             || iau.incs i3,-1       || bru.bra label_while2_r9  
nop 6
    
label_else_qr_r9:

sau.div.f32 i8,i8,i7 || lsu0.ldil i22,0x1        || lsu1.ldih i22,0x0
sau.div.f32 i6,i6,i7 || cmu.cpii.i32.f32 i22,i22
nop 6
////////////i0-i6 ,i14-i18
cmu.cpiv   v10.0 i0  || lsu0.cp v10.1  i1        || lsu1.cp v10.2 i2
cmu.cpiv   v10.3 i3  || lsu0.cp v11.0  i4        || lsu1.cp v11.1 i5
lsu0.cp v11.3  i14   || lsu1.cp v12.0 i15
cmu.cpiv   v12.1 i16 || lsu0.cp v12.2  i17       || lsu1.cp v12.3 i18
sau.sub.f32 i8,i22,i8|| lsu0.ldil i29  label_sqrt|| lsu1.ldih i29  label_sqrt 
nop
sau.mul.f32 i23,i6,i6|| lsu0.ldil i22,0x4        || lsu1.ldih i22,0x0        || cmu.cpiv v11.2 i6 
sau.mul.f32 i24,i8,i8|| cmu.cpii.i32.f32 i22,i22 || bru.swp i29
nop
sau.mul.f32 i23,i23,i22 
nop 2
sau.add.f32 i18,i23,i24 
nop 
sau.div.f32 i23,i8,i18|| cmu.cpii i24,i18        || lsu0.ldil i29  label_sqrt || lsu1.ldih i29  label_sqrt //vt
nop 9
bru.swp i29
nop
sau.add.f32 i23,i23,1.0
nop 2
sau.mul.f32 i18,i23,0.5
nop 
sau.mul.f32 i24,i24,i18 || cmu.cpvi  i6  v11.2 
nop 2
sau.div.f32 i24,i6,i24  || cmu.cpii i25,i18// i25=c0 //i24=s0
nop 2
cmu.cpiv   v13.0 i6     || lsu0.cp v13.1  i7

label_fori3_r9:

cmu.cpvi i10,v12.3   || lsu0.cp  i4  v11.0 || lsu1.cp  i5    v11.1   
lsu1.ldi.32 i23,i10  || iau.shl i11,i4,2   || sau.shl.x32 i12,i5,2
lsu1.ldi.32 i23,i10  || cmu.cpiv v9.0 i25
lsu1.ldi.32 i23,i10  
lsu1.ldi.32 i23,i10  
lsu1.ldi.32 i23,i10
lsu1.ldi.32 i23,i10
lsu1.ldi.32 i23,i10
lsu1.ldi.32 i23,i10 || iau.add i13,i11,i23  || sau.add.i32 i14,i12,i23
lsu1.ldi.32 i23,i10 || iau.add i20,i11,i23  || sau.add.i32 i21,i12,i23         
 
lsu0.ld.64.l v0 i13 || lsu1.ld.64.h v0,i14  || iau.add i22,i11,i23        || sau.add.i32 i1,i12,i23
lsu0.ld.64.l v1 i20 || lsu1.ld.64.h v1,i21  || iau.add i25,i11,i23        || sau.add.i32 i2,i12,i23
lsu0.ld.64.l v2 i22 || lsu1.ld.64.h v2,i1   || iau.add i26,i11,i23        || sau.add.i32 i3,i12,i23
lsu0.ld.64.l v3 i25 || lsu1.ld.64.h v3,i2   || iau.add i27,i11,i23        || sau.add.i32 i6,i12,i23
lsu0.ld.64.l v4 i26 || lsu1.ld.64.h v4,i3   || iau.add i28,i11,i23        || sau.add.i32 i7,i12,i23
lsu0.ld.64.l v5 i27 || lsu1.ld.64.h v5,i6   || iau.add i29,i11,i23        || sau.add.i32 i8,i12,i23
lsu0.ld.64.l v6 i28 || lsu1.ld.64.h v6,i7   || iau.add i0,i11,i23         || sau.add.i32 i9,i12,i23
lsu0.ld.64.l v7 i29 || lsu1.ld.64.h v7,i8   || cmu.cpiv v9.1 i24 
lsu0.ld.64.l v8 i0 || lsu1.ld.64.h v8,i9    || cmu.vszm.word v9,v9,[0110]
                                               cmu.vszm.word v0,v0,[2020]
                                               cmu.vszm.word v1,v1,[2020] || vau.mul.f32 v0,v0,v9
                                               cmu.vszm.word v2,v2,[2020] || vau.mul.f32 v1,v1,v9
                                               cmu.vszm.word v3,v3,[2020] || vau.mul.f32 v2,v2,v9
                                               cmu.vszm.word v4,v4,[2020] || vau.mul.f32 v3,v3,v9 
                                               cmu.vszm.word v5,v5,[2020] || vau.mul.f32 v4,v4,v9
                                               cmu.vszm.word v6,v6,[2020] || vau.mul.f32 v5,v5,v9
                                               cmu.vszm.word v7,v7,[2020] || vau.mul.f32 v6,v6,v9
                                               cmu.vszm.word v8,v8,[2020] || vau.mul.f32 v7,v7,v9
                                                                             vau.mul.f32 v8,v8,v9

lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v0,v0,v0 || cmu.cpvi  i10 v12.1  
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v1,v1,v1
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v2,v2,v2
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v3,v3,v3 || cmu.cpvi  i4 v0.0
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v4,v4,v4 || cmu.cpvi  i5 v0.3 || lsu0.st.32 i4,i13
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v5,v5,v5 || cmu.cpvi  i4 v1.0 || lsu0.st.32 i5,i14
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v6,v6,v6 || cmu.cpvi  i5 v1.3 || lsu0.st.32 i4,i20
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v7,v7,v7 || cmu.cpvi  i4 v2.0 || lsu0.st.32 i5,i21
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v8,v8,v8 || cmu.cpvi  i5 v2.3 || lsu0.st.32 i4,i22
cmu.cpvi  i4 v3.0              || lsu0.st.32 i5,i1 
cmu.cpvi  i5 v3.3              || lsu0.st.32 i4,i25
cmu.cpvi  i4 v4.0              || lsu0.st.32 i5,i2
cmu.cpvi  i5 v4.3              || lsu0.st.32 i4,i26
cmu.cpvi  i4 v5.0              || lsu0.st.32 i5,i3
cmu.cpvi  i5 v5.3              || lsu0.st.32 i4,i27
cmu.cpvi  i4 v6.0              || lsu0.st.32 i5,i6
cmu.cpvi  i5 v6.3              || lsu0.st.32 i4,i28
cmu.cpvi  i4 v7.0              || lsu0.st.32 i5,i7
cmu.cpvi  i5 v7.3              || lsu0.st.32 i4,i29
cmu.cpvi  i4 v8.0              || lsu0.st.32 i5,i8 
cmu.cpvi  i5 v8.3              || lsu0.st.32 i4,i0
lsu0.st.32 i5,i9


lsu1.ldi.32 i23,i10  
lsu1.ldi.32 i23,i10 
lsu1.ldi.32 i23,i10  
lsu1.ldi.32 i23,i10  
lsu1.ldi.32 i23,i10
lsu1.ldi.32 i23,i10
lsu1.ldi.32 i23,i10
lsu1.ldi.32 i23,i10 || iau.add i13,i11,i23  || sau.add.i32 i14,i12,i23
lsu1.ldi.32 i23,i10 || iau.add i20,i11,i23  || sau.add.i32 i21,i12,i23         
 
lsu0.ld.64.l v0 i13 || lsu1.ld.64.h v0,i14  || iau.add i22,i11,i23 || sau.add.i32 i1,i12,i23
lsu0.ld.64.l v1 i20 || lsu1.ld.64.h v1,i21  || iau.add i25,i11,i23 || sau.add.i32 i2,i12,i23
lsu0.ld.64.l v2 i22 || lsu1.ld.64.h v2,i1   || iau.add i26,i11,i23 || sau.add.i32 i3,i12,i23
lsu0.ld.64.l v3 i25 || lsu1.ld.64.h v3,i2   || iau.add i27,i11,i23 || sau.add.i32 i6,i12,i23
lsu0.ld.64.l v4 i26 || lsu1.ld.64.h v4,i3   || iau.add i28,i11,i23 || sau.add.i32 i7,i12,i23
lsu0.ld.64.l v5 i27 || lsu1.ld.64.h v5,i6   || iau.add i29,i11,i23 || sau.add.i32 i8,i12,i23
lsu0.ld.64.l v6 i28 || lsu1.ld.64.h v6,i7   || iau.add i0,i11,i23  || sau.add.i32 i9,i12,i23
lsu0.ld.64.l v7 i29 || lsu1.ld.64.h v7,i8    
lsu0.ld.64.l v8 i0  || lsu1.ld.64.h v8,i9    
                                               cmu.vszm.word v0,v0,[2020]
                                               cmu.vszm.word v1,v1,[2020] || vau.mul.f32 v0,v0,v9
                                               cmu.vszm.word v2,v2,[2020] || vau.mul.f32 v1,v1,v9
                                               cmu.vszm.word v3,v3,[2020] || vau.mul.f32 v2,v2,v9
                                               cmu.vszm.word v4,v4,[2020] || vau.mul.f32 v3,v3,v9 
                                               cmu.vszm.word v5,v5,[2020] || vau.mul.f32 v4,v4,v9
                                               cmu.vszm.word v6,v6,[2020] || vau.mul.f32 v5,v5,v9
                                               cmu.vszm.word v7,v7,[2020] || vau.mul.f32 v6,v6,v9
                                               cmu.vszm.word v8,v8,[2020] || vau.mul.f32 v7,v7,v9
                                                                             vau.mul.f32 v8,v8,v9

lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v0,v0,v0
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v1,v1,v1
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v2,v2,v2
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v3,v3,v3 || cmu.cpvi  i4 v0.0
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v4,v4,v4 || cmu.cpvi  i5 v0.3 || lsu0.st.32 i4,i13
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v5,v5,v5 || cmu.cpvi  i4 v1.0 || lsu0.st.32 i5,i14
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v6,v6,v6 || cmu.cpvi  i5 v1.3 || lsu0.st.32 i4,i20
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v7,v7,v7 || cmu.cpvi  i4 v2.0 || lsu0.st.32 i5,i21
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v8,v8,v8 || cmu.cpvi  i5 v2.3 || lsu0.st.32 i4,i22
cmu.cpvi  i4 v3.0    || lsu0.st.32 i5,i1 
cmu.cpvi  i5 v3.3    || lsu0.st.32 i4,i25
cmu.cpvi  i4 v4.0    || lsu0.st.32 i5,i2
cmu.cpvi  i5 v4.3    || lsu0.st.32 i4,i26
cmu.cpvi  i4 v5.0    || lsu0.st.32 i5,i3
cmu.cpvi  i5 v5.3    || lsu0.st.32 i4,i27
cmu.cpvi  i4 v6.0    || lsu0.st.32 i5,i6
cmu.cpvi  i5 v6.3    || lsu0.st.32 i4,i28
cmu.cpvi  i4 v7.0    || lsu0.st.32 i5,i7
cmu.cpvi  i5 v7.3    || lsu0.st.32 i4,i29
cmu.cpvi  i4 v8.0    || lsu0.st.32 i5,i8 
cmu.cpvi  i5 v8.3    || lsu0.st.32 i4,i0
lsu0.st.32 i5,i9     || bru.bra label_while2_r9
nop 
////////////////////i0-i6 ,14-i18
cmu.cpvi  i0  v10.0  || lsu1.cp  i2    v10.2
cmu.cpvi  i3  v10.3  || lsu0.cp  i4 v11.0    || lsu1.cp  i5   v11.1 
cmu.cpvi  i1  v10.1  || lsu0.cp   i14 v11.3  || lsu1.cp  i15   v12.0
cmu.cpvi  i16  v12.1 || lsu0.cp  i17 v12.2   || lsu1.cp  i18   v12.3 
cmu.cpvi i6 v13.0    || lsu0.cp i7 v13.1     

label_rq_r9:

sau.div.f32 i6,i6,i8
sau.div.f32 i7,i7,i8
nop 11
sau.sub.f32 i7,i7,1.0
nop 2
sau.mul.f32 i23,i6,i6  || lsu0.ldil i22,0x4            || lsu1.ldih i22,0x0
sau.mul.f32 i24,i7,i7  || cmu.cpii.i32.f32 i22,i22
nop 2
sau.mul.f32 i23,i23,i22
nop 2
sau.add.f32 i23,i23,i24|| lsu0.ldil i29  label_sqrt    || lsu1.ldih i29  label_sqrt 
bru.swp i29
nop 
//////i0-i6 ,14-i18
cmu.cpiv   v10.0 i0    || lsu0.cp v10.1  i1            || lsu1.cp v10.2 i2
cmu.cpiv   v10.3 i3    || lsu0.cp v11.0  i4            || lsu1.cp v11.1 i5
cmu.cpiv   v11.2 i6    || lsu0.cp v11.3  i14           || lsu1.cp v12.0 i15
cmu.cpiv   v12.1 i16   || lsu0.cp v12.2  i17           || lsu1.cp v12.3 i18
cmu.cpii i18,i23
cmu.cpii i25,i18//vt
sau.div.f32 i23,i7,i25    || lsu0.ldil i22,0x1         || lsu1.ldih i22,0x0
cmu.cpii.i32.f32 i22,i22  || lsu0.ldil i29  label_sqrt || lsu1.ldih i29  label_sqrt
nop 11
sau.sub.f32 i24,i22,i23   || bru.swp i29               || lsu0.ldil i13,0x0        || lsu1.ldih i13,0x0//s0
nop 2 
sau.mul.f32 i24,i24,0.5   || cmu.cpii.i32.f32 i13,i13
nop 2 
cmu.cpii i18,i24
cmu.cpii i24,i18          
nop
cmu.cpvi  i6  v11.2 
cmu.cmz.f32 i6
peu.pc1c LT                || sau.sub.f32 i24,i13,i24 //s0
nop 2 
sau.mul.f32 i25,i25,i24
nop 2 
sau.div.f32 i25,i6,i25 
nop 11
cmu.cpiv   v13.0 i6        || lsu0.cp v13.1  i7
 
label_fori4_r9:

cmu.cpvi i10,v12.3         || lsu0.cp  i4  v11.0       || lsu1.cp  i5    v11.1   
lsu1.ldi.32 i23,i10        || iau.shl i11,i4,2         || sau.shl.x32 i12,i5,2
lsu1.ldi.32 i23,i10        || cmu.cpiv v9.1 i24
lsu1.ldi.32 i23,i10        || cmu.cpiv v9.0 i25 
lsu1.ldi.32 i23,i10  
lsu1.ldi.32 i23,i10
lsu1.ldi.32 i23,i10
lsu1.ldi.32 i23,i10
lsu1.ldi.32 i23,i10 || iau.add i13,i11,i23  || sau.add.i32 i14,i12,i23
lsu1.ldi.32 i23,i10 || iau.add i20,i11,i23  || sau.add.i32 i21,i12,i23         
 
lsu0.ld.64.l v0 i13 || lsu1.ld.64.h v0,i14  || iau.add i22,i11,i23        || sau.add.i32 i1,i12,i23
lsu0.ld.64.l v1 i20 || lsu1.ld.64.h v1,i21  || iau.add i25,i11,i23        || sau.add.i32 i2,i12,i23
lsu0.ld.64.l v2 i22 || lsu1.ld.64.h v2,i1   || iau.add i26,i11,i23        || sau.add.i32 i3,i12,i23
lsu0.ld.64.l v3 i25 || lsu1.ld.64.h v3,i2   || iau.add i27,i11,i23        || sau.add.i32 i6,i12,i23
lsu0.ld.64.l v4 i26 || lsu1.ld.64.h v4,i3   || iau.add i28,i11,i23        || sau.add.i32 i7,i12,i23
lsu0.ld.64.l v5 i27 || lsu1.ld.64.h v5,i6   || iau.add i29,i11,i23        || sau.add.i32 i8,i12,i23
lsu0.ld.64.l v6 i28 || lsu1.ld.64.h v6,i7   || iau.add i0,i11,i23         || sau.add.i32 i9,i12,i23
lsu0.ld.64.l v7 i29 || lsu1.ld.64.h v7,i8   
lsu0.ld.64.l v8 i0  || lsu1.ld.64.h v8,i9   || cmu.vszm.word v9,v9,[0110]
                                               cmu.vszm.word v0,v0,[2020]
                                               cmu.vszm.word v1,v1,[2020] || vau.mul.f32 v0,v0,v9
                                               cmu.vszm.word v2,v2,[2020] || vau.mul.f32 v1,v1,v9
                                               cmu.vszm.word v3,v3,[2020] || vau.mul.f32 v2,v2,v9
                                               cmu.vszm.word v4,v4,[2020] || vau.mul.f32 v3,v3,v9 
                                               cmu.vszm.word v5,v5,[2020] || vau.mul.f32 v4,v4,v9
                                               cmu.vszm.word v6,v6,[2020] || vau.mul.f32 v5,v5,v9
                                               cmu.vszm.word v7,v7,[2020] || vau.mul.f32 v6,v6,v9
                                               cmu.vszm.word v8,v8,[2020] || vau.mul.f32 v7,v7,v9
                                                                             vau.mul.f32 v8,v8,v9

lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v0,v0,v0 || cmu.cpvi  i10 v12.1  
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v1,v1,v1 
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v2,v2,v2 
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v3,v3,v3 || cmu.cpvi  i4 v0.0
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v4,v4,v4 || cmu.cpvi  i5 v0.3 || lsu0.st.32 i4,i13
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v5,v5,v5 || cmu.cpvi  i4 v1.0 || lsu0.st.32 i5,i14
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v6,v6,v6 || cmu.cpvi  i5 v1.3 || lsu0.st.32 i4,i20
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v7,v7,v7 || cmu.cpvi  i4 v2.0 || lsu0.st.32 i5,i21
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v8,v8,v8 || cmu.cpvi  i5 v2.3 || lsu0.st.32 i4,i22

cmu.cpvi  i4 v3.0              || lsu0.st.32 i5,i1 
cmu.cpvi  i5 v3.3              || lsu0.st.32 i4,i25
cmu.cpvi  i4 v4.0              || lsu0.st.32 i5,i2
cmu.cpvi  i5 v4.3              || lsu0.st.32 i4,i26
cmu.cpvi  i4 v5.0              || lsu0.st.32 i5,i3
cmu.cpvi  i5 v5.3              || lsu0.st.32 i4,i27
cmu.cpvi  i4 v6.0              || lsu0.st.32 i5,i6
cmu.cpvi  i5 v6.3              || lsu0.st.32 i4,i28
cmu.cpvi  i4 v7.0              || lsu0.st.32 i5,i7
cmu.cpvi  i5 v7.3              || lsu0.st.32 i4,i29
cmu.cpvi  i4 v8.0              || lsu0.st.32 i5,i8 
cmu.cpvi  i5 v8.3              || lsu0.st.32 i4,i0
lsu0.st.32 i5,i9

lsu1.ldi.32 i23,i10  
lsu1.ldi.32 i23,i10 
lsu1.ldi.32 i23,i10  
lsu1.ldi.32 i23,i10  
lsu1.ldi.32 i23,i10
lsu1.ldi.32 i23,i10
lsu1.ldi.32 i23,i10
lsu1.ldi.32 i23,i10 || iau.add i13,i11,i23  || sau.add.i32 i14,i12,i23
lsu1.ldi.32 i23,i10 || iau.add i20,i11,i23  || sau.add.i32 i21,i12,i23         
 
lsu0.ld.64.l v0 i13 || lsu1.ld.64.h v0,i14  || iau.add i22,i11,i23 || sau.add.i32 i1,i12,i23
lsu0.ld.64.l v1 i20 || lsu1.ld.64.h v1,i21  || iau.add i25,i11,i23 || sau.add.i32 i2,i12,i23
lsu0.ld.64.l v2 i22 || lsu1.ld.64.h v2,i1   || iau.add i26,i11,i23 || sau.add.i32 i3,i12,i23
lsu0.ld.64.l v3 i25 || lsu1.ld.64.h v3,i2   || iau.add i27,i11,i23 || sau.add.i32 i6,i12,i23
lsu0.ld.64.l v4 i26 || lsu1.ld.64.h v4,i3   || iau.add i28,i11,i23 || sau.add.i32 i7,i12,i23
lsu0.ld.64.l v5 i27 || lsu1.ld.64.h v5,i6   || iau.add i29,i11,i23 || sau.add.i32 i8,i12,i23
lsu0.ld.64.l v6 i28 || lsu1.ld.64.h v6,i7   || iau.add i0,i11,i23  || sau.add.i32 i9,i12,i23
lsu0.ld.64.l v7 i29 || lsu1.ld.64.h v7,i8    
lsu0.ld.64.l v8 i0  || lsu1.ld.64.h v8,i9    
                                               cmu.vszm.word v0,v0,[2020]
                                               cmu.vszm.word v1,v1,[2020] || vau.mul.f32 v0,v0,v9
                                               cmu.vszm.word v2,v2,[2020] || vau.mul.f32 v1,v1,v9
                                               cmu.vszm.word v3,v3,[2020] || vau.mul.f32 v2,v2,v9
                                               cmu.vszm.word v4,v4,[2020] || vau.mul.f32 v3,v3,v9 
                                               cmu.vszm.word v5,v5,[2020] || vau.mul.f32 v4,v4,v9
                                               cmu.vszm.word v6,v6,[2020] || vau.mul.f32 v5,v5,v9
                                               cmu.vszm.word v7,v7,[2020] || vau.mul.f32 v6,v6,v9
                                               cmu.vszm.word v8,v8,[2020] || vau.mul.f32 v7,v7,v9
                                                                             vau.mul.f32 v8,v8,v9

lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v0,v0,v0
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v1,v1,v1
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v2,v2,v2
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v3,v3,v3 || cmu.cpvi  i4 v0.0
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v4,v4,v4 || cmu.cpvi  i5 v0.3 || lsu0.st.32 i4,i13
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v5,v5,v5 || cmu.cpvi  i4 v1.0 || lsu0.st.32 i5,i14
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v6,v6,v6 || cmu.cpvi  i5 v1.3 || lsu0.st.32 i4,i20
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v7,v7,v7 || cmu.cpvi  i4 v2.0 || lsu0.st.32 i5,i21
lsu1.SWZMV4.word [2001] [VUUU] || vau.add.f32 v8,v8,v8 || cmu.cpvi  i5 v2.3 || lsu0.st.32 i4,i22
cmu.cpvi  i4 v3.0              || lsu0.st.32 i5,i1 
cmu.cpvi  i5 v3.3              || lsu0.st.32 i4,i25
cmu.cpvi  i4 v4.0              || lsu0.st.32 i5,i2
cmu.cpvi  i5 v4.3              || lsu0.st.32 i4,i26
cmu.cpvi  i4 v5.0              || lsu0.st.32 i5,i3
cmu.cpvi  i5 v5.3              || lsu0.st.32 i4,i27
cmu.cpvi  i4 v6.0              || lsu0.st.32 i5,i6
cmu.cpvi  i5 v6.3              || lsu0.st.32 i4,i28
cmu.cpvi  i4 v7.0              || lsu0.st.32 i5,i7
cmu.cpvi  i5 v7.3              || lsu0.st.32 i4,i29
cmu.cpvi  i4 v8.0              || lsu0.st.32 i5,i8 
cmu.cpvi  i5 v8.3              || lsu0.st.32 i4,i0
lsu0.st.32 i5,i9
nop 
////////////////////i0-i6 ,14-i18
cmu.cpvi  i0  v10.0            || lsu1.cp  i2 v10.2
cmu.cpvi  i3  v10.3            || lsu0.cp  i4 v11.0    || lsu1.cp  i5   v11.1 
cmu.cpvi  i1  v10.1            || lsu0.cp   i14 v11.3  || lsu1.cp  i15  v12.0
cmu.cpvi  i16  v12.1           || lsu0.cp  i17 v12.2   || lsu1.cp  i18  v12.3 
cmu.cpvi  i6 v13.0             || lsu0.cp  i7 v13.1

label_while2_r9:
iau.incs i5,1
cmu.cmii.i32 i5,i0
peu.pc1c LT                   || bru.bra label_fork2_r9
nop 6

iau.incs i4,1
cmu.cmii.i32 i4,i2
peu.pc1c LT                   || bru.bra label_forj2_r9
nop 6

//////	while (EstColRank>2 && S[EstColRank-1]<=S[0]*tol+tol*tol) EstColRank--//
		
label_while3_r9:	
	
lsu0.ldil i13,0x2             || lsu1.ldih i13,0x0	
cmu.cmii.i32 i0,i13           
peu.pc1c LTE                  || bru.bra end_while3_r9
nop 6                         
iau.sub i11,i0,1              || lsu0.ldil i13,svd_consts || lsu1.ldih i13,svd_consts  
iau.shl i11,i11,2             
iau.add i11,i11,i17           
lsu0.ld.32 i7 i17             || lsu1.ldo.32 i13,i13,0x0
lsu0.ld.32 i8 i11      //S[EstColRank-1]
nop 5	
sau.mul.f32 i7,i7,i13  	
sau.mul.f32 i13,i13,i13  	
nop 2
sau.add.f32 i7,i7,i13  
nop 2
cmu.cmii.f32 i8, i7
peu.pc1c LTE                  || iau.incs i0,-1           || bru.bra label_while3_r9
nop 6
 
end_while3_r9:

bru.bra label_while1_r9
nop 6
label_sqrt_S_r9: 
 // for (i = 0// i < rank// i++)
 // {
  // S[i] = sqrt(S[i])//
 // } 
iau.xor i9,i9,i9              || lsu0.cp v2.0 i15          || lsu1.cp v2.1 i17 
                              
label_fori5_r9:               
                              
iau.shl i11,i9,2              || lsu0.ldil i29  label_sqrt || lsu1.ldih i29  label_sqrt || cmu.cpvi  i17 v2.1
iau.add i11,i11,i17                    
lsu0.ld.32 i18 i11            || bru.swp i29               || iau.incs i9,1
nop 6                         
lsu0.st.32 i18 i11            || cmu.cpvi  i15 v2.0   
cmu.cmii.i32 i9,i15           
peu.pc1c LT                   || bru.bra label_fori5_r9
nop  6     
 
label_exit: 
 
    LSU0.LD.64  i30  i19      || IAU.ADD i19 i19 8
	LSU0.LD.64  i28  i19      || IAU.ADD i19 i19 8
	LSU0.LD.64  i26  i19      || IAU.ADD i19 i19 8
	LSU0.LD.64  i24  i19      || IAU.ADD i19 i19 8
	LSU0.LD.64  i22  i19      || IAU.ADD i19 i19 8
	LSU0.LD.64  i20  i19      || IAU.ADD i19 i19 8
	LSU0.LDo.64.h  v31  i19 8 || LSU1.LDo.64.l  v31  i19 0 ||  IAU.ADD i19 i19 16
	LSU0.LDo.64.h  v30  i19 8 || LSU1.LDo.64.l  v30  i19 0 ||  IAU.ADD i19 i19 16
	LSU0.LDo.64.h  v29  i19 8 || LSU1.LDo.64.l  v29  i19 0 ||  IAU.ADD i19 i19 16
	LSU0.LDo.64.h  v28  i19 8 || LSU1.LDo.64.l  v28  i19 0 ||  IAU.ADD i19 i19 16
	LSU0.LDo.64.h  v27  i19 8 || LSU1.LDo.64.l  v27  i19 0 ||  IAU.ADD i19 i19 16
	LSU0.LDo.64.h  v26  i19 8 || LSU1.LDo.64.l  v26  i19 0 ||  IAU.ADD i19 i19 16
	LSU0.LDo.64.h  v25  i19 8 || LSU1.LDo.64.l  v25  i19 0 ||  IAU.ADD i19 i19 16
	LSU0.LDo.64.h  v24  i19 8 || LSU1.LDo.64.l  v24  i19 0 ||  IAU.ADD i19 i19 16 || BRU.jmp i30
    nop 6
 
label_sqrt:
  .set   x i18
  .set   f i17
  .set   y i16
  .set exp i15
  .set nex i14 // new exponent
  // {f, exp+126} = frexp_v4f(x)//
  // f *= 0.5
  // y = f * 1.18032 + 0.41731
  // optimizations: assume positive, don't check for 0/Inf/NaN input (handled at the end)
                             lsu0.ldil i0, 0xFFFF     || lsu1.ldih i0, 0x807F
  iau.and       f, x  i0  || lsu0.ldil i1, 0          || lsu1.ldih i1, 0x3E80 // i1=0x3e800000
  iau.or        f, f  i1  || lsu0.ldil i2, 0x14ba || lsu1.ldih i2, 0x3f97
  sau.mul.f32   y, f i2                               // y=f*1.18032
  iau.not      i0, i0     || lsu0.ldil i2, 0xa9a8 // i0=0x7f800000
  iau.and     exp, x i0   || lsu0.ldih i2, 0x3ed5
  sau.add.f32   y, y i2                               // y=f*1.18032+0.41731 i1=0x00800000
  lsu1.cp     nex, exp    || lsu0.ldil i2, .Lhlast
                             lsu0.ldih i2, .Lhlast    
  sau.div.f32  i6, f y    || bru.rps   i2 2 // i6=f/y
  sau.mul.f32   y, y 0.5       // y*=0.5
  lsu0.ldil    i5, 0x04f3
  lsu0.ldih    i5, 0x3f35
  lsu0.ldih    i1, 0x0080      // i1=0x00800000
  iau.shl      i6, i1 8        // i6=0x80000000
  iau.not      i2, i6          // i2=0x7fffffff
  lsu0.ldil    i3, 0x0000
.Lhlast:
    lsu0.ldih  i3, 0x3f00      // i3=0x3f000000
    lsu0.ldil  i4, 0x0000
    lsu0.ldih  i4, 0x7fc0      // i4=qNaN
    iau.or     i4, x i4
    sau.add.f32 y, y i6 // y=y*0.5+f/y
    nop
    iau.and    i6, exp i1
  iau.add     nex, exp i1 || sau.mul.f32 y, y i5 || peu.pc1i neq // if(exp&1)nex=exp+1,y*=sqrt(.5)
  iau.sub     nex, nex i3 
  iau.shr.i32 nex, nex 1  || bru.jmp      i29
    iau.and   nex, nex i0 || cmu.cmii.i32 exp  i0
    iau.add     y, y nex  || sau.or       exp, exp exp 
  // if( exp==0xff ) y=x//
    lsu0.cp     y, x      || peu.pc1c eq
  // if( !exp ) y=0.0//
    lsu0.ldil   y, 0      || peu.pc1s eq
    sau.and   i18, y i2   || iau.add       i0, x 0   
  // if( x<0 ) y=qNaN//
    lsu0.cp   i18, i4     || peu.pc1i lt

  .unset x
  .unset f
  .unset y
  .unset exp
  .unset nex

label_sqrt_4val:  

  .set   x v23
  .set   f v22
  .set exp v21
  .set   y v20
  .set nex v19 // new exponent
  .set  xs v18 // inital x's sign

  // {f, exp+126} = frexp_v4f(x)//
  // y = f * 0.59016 + 0.41731
  // optimizations: assume positive, don't check for 0/Inf/NaN input (handled at the end)
                                                         lsu0.ldil i0, 0xFFFF     || lsu1.ldih i0, 0x807F
                              cmu.cpivr.x32 v0, i0    || lsu0.ldil i1, 0          || lsu1.ldih i1, 0x3E80     // v0=<4 x ~(0xff<<23)>
  vau.and       f, x v0    || cmu.cpivr.x32 v1, i1    || lsu0.ldil i0, 0x14ba || lsu1.ldih i0, 0x3f97 // v1=<4 x (0x7d<<23)>
  vau.or        f, f v1    || cmu.cpivr.x32 v1, i0                                // f=x&v0|v1, v1=<4 x 1.18032>
  vau.mul.f32   y, f v1                               || lsu0.ldil i0, 0xa9a8 // y=f*1.18032
  vau.not      v0, v0      || lsu1.cp       i2, v22.0 || lsu0.ldih i0, 0x3ed5 // v0=<4 x (0xff<<23)>
                              cmu.cpivr.x32 v1, i0                                // v1=<4 x 0.41731>
  vau.add.f32   y, y v1    || lsu0.cp       i3, v22.1 // y=f*1.18032+0.41731
  vau.and     exp, x v0    || lsu0.cp       i4, v22.2 // exp=x&v0
                              lsu0.cp       i5, v22.3
  // repeat(2) y=y*0.5+f/y//
  // here: i<2:5> ::=f[0:3]
  // use: i<6:9> ::=y[0:3]
                              lsu0.cp       i6, v20.0
  // 4x(i10,i11) = f/y//
  sau.div.f32  i10, i2 i6  || lsu0.cp       i7, v20.1
  sau.div.f32  i11, i3 i7  || lsu0.cp       i8, v20.2
  sau.div.f32  i10, i4 i8  || lsu0.cp       i9, v20.3
  sau.div.f32  i11, i5 i9  || vau.mul.f32    y, y 0.5
  lsu0.ldih     i1, 0x0080
  cmu.cpivr.x32 v1, i1     // v1=<4 x (1<<23)>
  // i6:7 = y*0.5
  vau.and       v2, exp v1 || lsu0.cp       i6, v20.0
  lsu0.cp       i7, v20.1
  lsu0.cp       i8, v20.2
  lsu0.cp       i9, v20.3
  cmu.cmz.i32   v2
  vau.or       nex, exp exp                 // nex=exp
  vau.add.i32  nex, exp v1 || peu.pvv32 neq // where( exp[i]&1 ) ++nex[i]//
  // i6:i9 = 0.5*y + f/y//
  sau.add.f32   i6, i6 i10
  sau.add.f32   i7, i7 i11
  sau.add.f32   i8, i8 i10
  // y = i6:i9 = 0.5*y + f/y//
  sau.add.f32   i9, i9 i11 || lsu0.cp    v20.0, i6
  // 2x(i10:i11) = f/y//                 
  sau.div.f32  i10, i2 i6  || lsu0.cp    v20.1, i7
  sau.div.f32  i11, i3 i7  || lsu0.cp    v20.2, i8
  sau.div.f32  i10, i4 i8  || lsu0.cp    v20.3, i9
  sau.div.f32  i11, i5 i9
  lsu0.ldih     i1, 0xC100
  cmu.cpivr.x32 v2, i1 
  vau.add.i32  nex, nex v2 // nex[i]-=<4 x 126>//
  vau.shl.x32   v1, v1 8   // v1=<4 x (1<<31)>
  vau.shr.u32  nex, nex 1
  lsu0.ldil     i0, 0x04f3
  lsu0.ldih     i0, 0x3f35
  // f = 2x(i10:i11) = f/y//
  vau.mul.f32    y, y 0.5
  // f = 2x(i10:i11) = f/y//
  lsu1.cp   v22.0, i10     || lsu0.ldih i1, 0x7FC0 // f.0
  lsu0.cp   v22.1, i11
  lsu0.cp   v22.2, i10
  lsu0.cp   v22.3, i11
  vau.add.f32    y, y f    || cmu.cpvv f, x
  vau.and       xs, x v1
  cmu.cpivr.x32 v2, i0                                 // v2=<4 x SQRT_HALF>
  vau.mul.f32    y, y v2   || peu.pvv32 neq            // where( exp[i]&1 ) y[i]*=sqrt(0.5)//
  vau.not       v3, v1     
  cmu.cpivr.x32 v2, i1        // v2=<4 x qNaN>
  vau.add.i32    y, y nex  || bru.jmp i29
    cmu.cmz.i32 exp
    vau.and    v23, y v3
  // where( !exp_in[i] ) y[i]=0//
    vau.xor    v23, f f    || peu.pvv32 eq || cmu.cmvv.i32 exp, v0
  // where( exp_in[i]==0xff ) y[i]=x[i]//
    vau.or     v23, f f    || peu.pvv32 eq || cmu.cmz.i32  f
  // where( x[i]<0 ) y[i]=qNaN//
    vau.or     v23, v2 v2  || peu.pvv32 lt

  .unset x
  .unset f
  .unset exp
  .unset y
  .unset nex
  .unset xs
.nowarnend
.size mvcvSVD_asm,.-mvcvSVD_asm
  .end  
  
  
  
  

 
 
 
 
 
 
 
 
 


