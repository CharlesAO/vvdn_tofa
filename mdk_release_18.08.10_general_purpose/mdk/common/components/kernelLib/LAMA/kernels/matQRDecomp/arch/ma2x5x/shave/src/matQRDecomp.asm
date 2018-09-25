///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

// QR matrix decomposition using the Householder method
.type mvcvMatQRDecomp_asm,@function
.version 00.51.05


.code .text.mvcvMatQRDecomp_asm

//typedef	struct
//{
//	unsigned int	m, n//
//	unsigned int	max_m, max_n, max_size//
//	Real			**me, *base//
//} MAT//
//
//Calling convention
//void mvcvMatQRDecomp(MAT *A, MAT *Q, MAT *R, MAT *q0, MAT *q1, MAT *z0, MAT *z1)//
//                        i18     i17     i16      i15     i14     i13     i12

mvcvMatQRDecomp_asm:
.nowarn
.set loop_height i3
.set q0 i24
.set height i25
.set width i26
.set base_mat i27
.set refl_vec_base i28
.set refl_vecI i29
.set counterK i31
.set iterEnd i30

.set CDMA_CFG_LINK  0x78802000
.set CDMA_CTRL      0x78802030
.set CDMA_DST_SRC   0x78802008
.set CDMA_LEN       0x78802010

iau.sub i19 i19 8
lsu0.st.64 i20 i19  || iau.sub i19 i19 8
lsu0.st.64 i22 i19  || iau.sub i19 i19 8
lsu0.st.64 i24 i19  || iau.sub i19 i19 8
lsu0.st.64 i26 i19  || iau.sub i19 i19 8
lsu0.st.64 i28 i19  || iau.sub i19 i19 8
lsu0.st.64 i30 i19

//v18 is vector with base pointers for matrices q0,q1,z0,z1
//v17 is vector with pointers to matrices q0,q1,z0,z1
lsu0.ldo.32 i0 i12 0x18
lsu0.ldo.32 i1 i13 0x18
lsu0.ldo.32 i2 i14 0x18
lsu0.ldo.32 i3 i15 0x18
cmu.cpivd v17.0 i12          || lsu0.ld.64.l v16 i18
cmu.cpivd v17.2 i14          || lsu0.ldo.32 base_mat i18 0x18
//copy R pointer to V16
cmu.cpiv.x32 v16.3 i16
cmu.shriv.x32 v18 v18 i0
cmu.shriv.x32 v18 v18 i1
cmu.shriv.x32 v18 v18 i2
cmu.shriv.x32 v18 v18 i3

lsu1.cp q0 i3                   || lsu0.ldo.32 i0 i17 0x18 || sau.xor i31 i31 i31

//v16 is vector with pointer to R, A base, width, height
cmu.cpvi.x32 height   v16.0     || lsu0.cp refl_vec_base base_mat
cmu.cpvi.x32 width    v16.1     || iau.incs i31 1
cmu.cpiv.x32 v16.2    base_mat  || iau.sub iterEnd height 1
cmu.min.u32 iterEnd iterEnd width
iau.mul width width 4
sau.sub.i32 iterEnd iterEnd 1

//v19 is vector with pointer to A, A base , Q and Q base
cmu.shriv.x32 v19 v19 i18
cmu.shriv.x32 v19 v19 base_mat
cmu.shriv.x32 v19 v19 i17
cmu.shriv.x32 v19 v19 i0

calc_matrix_minor:

//refl_vec[i] = z0->base[i*width]//
lsu0.ld.32 i8 refl_vec_base  || iau.add refl_vec_base refl_vec_base width || lsu1.ldil i5 matrix_minor_loop_end
lsu0.ld.32 i9 refl_vec_base  || iau.add refl_vec_base refl_vec_base width || lsu1.ldih i5 matrix_minor_loop_end
lsu0.ld.32 i10 refl_vec_base || iau.add refl_vec_base refl_vec_base width
lsu0.ld.32 i11 refl_vec_base || iau.add refl_vec_base refl_vec_base width
lsu0.ldil i7 0x3         || iau.xor i21 i21 i21
sau.and i6 height i7     || iau.shr.u32 i7 height 2
nop
cmu.cpii i22 i8 // element from vector to be increased

cmu.shriv.x32 v0 v0 i8  || lsu0.ld.32 i8 refl_vec_base  || iau.add refl_vec_base refl_vec_base width || bru.rpl i5 i7
cmu.shriv.x32 v0 v0 i9  || lsu0.ld.32 i9 refl_vec_base  || iau.add refl_vec_base refl_vec_base width
cmu.shriv.x32 v0 v0 i10 || lsu0.ld.32 i10 refl_vec_base || iau.add refl_vec_base refl_vec_base width
cmu.shriv.x32 v0 v0 i11 || lsu0.ld.32 i11 refl_vec_base || iau.add refl_vec_base refl_vec_base width
//vsum += t*t//
vau.mul.f32 v0 v0 v0
nop
matrix_minor_loop_end:
nop
//sum = __builtin_shave_sau_sumx_f32_r( vsum )//
sau.sumx.f32 i20 v0
nop 4
sau.add.f32 i21 i20 i21
nop 2

lsu0.cp i11 i21 || bru.bra sqrtF32
//refl_vec[0]*refl_vec[0]//
sau.mul.f32 i8 i22 i22
lsu0.ldil i29 calc_minor_end || lsu1.ldih i29 calc_minor_end
nop
//sum -= refl_vec[0]*refl_vec[0]
sau.sub.f32 i21 i21 i8
nop 2
calc_minor_end:

//float N1 = sqrt(sum) in i11
//if (m->base[0] < 0)  N1 = -N1//
lsu0.cp i11 i18        || cmu.cmz.f32 i22
peu.pc1c lt            || cmu.vsignf.f32 i11 i11 [n]

//refl_vec[0] += N1//
sau.add.f32 i22 i22 i11
iau.xor i7 i7 i7
nop
//refl_vec[0]*refl_vec[0]
sau.mul.f32 i8 i22 i22
nop 2

//sum += refl_vec[0]*refl_vec[0]//
sau.add.f32 i21 i21 i8
lsu0.ldil i7 0x0 || lsu1.ldih i7 0x4000 || sau.xor i8 i8 i8  // load fp value of 2 in i7
cmu.vsignf.f32 i8 i7 [n]
lsu0.cp i11 i21
//float N2 = -2.0f/sum
sau.div.f32 i11 i8 i21
nop
//!!!!!!!!!!!!!! the first element in the input matrix is changed
lsu0.ld.32 i23 base_mat
nop 6
lsu0.st.32 i22 base_mat

lsu0.cp refl_vecI base_mat
iau.add refl_vecI refl_vecI width
cmu.cpivr.x32 v3 i11

lsu0.cp i21 i11 || cmu.cpii loop_height height
lsu0.cp i0 q0   || iau.shl i4 height 2
lsu0.cp i8 i22  || iau.incs i4 4
compute_householder_matrix:

//  *pbase0 = q0->base//
//  for (i = 0// i < height// i++)
//  {
//    reflV_i = refl_vec[i]//
//    pbase = (float4*)pbase0//
//    reflV_j = (float4*)&(refl_vec[0])//
//
//    for (j = 0// j < height/4// j++)
//    {
//      t  = *reflV_j++//
//      t *= reflV_i//
//      t *= N2//
//      *pbase++ = t//
//    }
//    q0->base[i*height + i] += 1//
//    pbase0 += height//
//  }

lsu0.cp refl_vec_base base_mat || sau.mul.f32 i1 i22 i22
lsu0.ld.32 i8 refl_vec_base    || iau.add refl_vec_base refl_vec_base width || lsu1.ldil i5 householder_loop_end
lsu0.ld.32 i9 refl_vec_base    || iau.add refl_vec_base refl_vec_base width || lsu1.ldih i5 householder_loop_end
lsu0.ld.32 i10 refl_vec_base   || iau.add refl_vec_base refl_vec_base width || sau.mul.f32 i2 i1 i21
lsu0.ld.32 i11 refl_vec_base   || iau.add refl_vec_base refl_vec_base width
lsu0.ldil i7 0x0               || lsu1.ldih i7 0x3f80
//    reflV = refl_vec[i]
cmu.cpivr.x32 v1 i22           || sau.add.f32 i2 i2 i7
sau.and i6 height i7           || iau.shr.u32 i7 height 2

// t  = *reflV_j++//
cmu.shriv.x32 v0 v0 i8     || lsu0.ld.32 i8 refl_vec_base           || iau.add refl_vec_base refl_vec_base width  || bru.rpl i5 i7
cmu.shriv.x32 v0 v0 i9     || lsu0.ld.32 i9 refl_vec_base           || iau.add refl_vec_base refl_vec_base width
cmu.shriv.x32 v0 v0 i10    || lsu0.ld.32 i10 refl_vec_base          || iau.add refl_vec_base refl_vec_base width
cmu.shriv.x32 v0 v0 i11    || lsu0.ld.32 i11 refl_vec_base          || iau.add refl_vec_base refl_vec_base width
householder_loop_end:
// t *= reflV_i
vau.mul.f32 v2 v0 v1
nop 2
//t *= N2
vau.mul.f32 v4 v2 v3
nop 2
lsu0.st.64.l v4 q0 || lsu1.sto.64.h v4 q0 8 || iau.incs q0 16

///////////////////// end inner loop

// reflV_i = refl_vec[i] in i22

iau.deccmp loop_height 1
lsu0.ld.32 i22 refl_vecI || iau.add refl_vecI refl_vecI width || peu.pcix.neq 0 || bru.bra compute_householder_matrix
//q0->base[i*height + i] += 1//
lsu0.st.32 i2 i0
iau.add i0 i0 i4
nop 4

//get pointer to input Mat
cmu.cpvi.x32 i18 v19.0
lsu0.st.32 i23 base_mat

//m_mlt(q0,z0, z1)//
bru.bra mvcvMatMul_asm
lsu0.cp i17 i18
lsu0.cp i18 i15
lsu0.cp i16 i12
lsu0.cp i9 i30
lsu0.ldil i30 QR_first_iter_decomp_end
lsu0.ldih i30 QR_first_iter_decomp_end
QR_first_iter_decomp_end:

lsu0.cp i30 i9
cmu.cpvi.x32 i15 v18.3 //src
cmu.cpvi.x32 i16 v19.3 //dst

lsu0.ldil i1 CDMA_CFG_LINK || lsu1.ldih i1 CDMA_CFG_LINK
lsu0.ldil i2 CDMA_CTRL     || lsu1.ldih i2 CDMA_CTRL
lsu0.ldil i3 CDMA_DST_SRC  || lsu1.ldih i3 CDMA_DST_SRC
lsu0.ldil i4 CDMA_LEN      || lsu1.ldih i4 CDMA_LEN

iau.mul i5 height height
lsu0.ldil i0 0x574 || lsu1.ldih i0 0x0010
lsu0.st.32 i0 i1   || iau.shl i5 i5 2
lsu0.st.64 i15 i3
lsu1.st.32 i5 i4
lsu0.ldil i0 0x5   || lsu1.ldih i0 0x0
lsu0.sto.32 i0 i2 0

////////////////////////////end-of-first-iteration/////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////Begin-QR-Decomp-Main-Loop\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

cmu.cpvi.x32 i12 v18.0
cmu.cpvi.x32 i15 v18.3      || lsu0.cp base_mat i12
cmu.cpii     q0 i15         || lsu0.cp refl_vec_base i12


QR_Decomp_Loop:

iau.shl i0 height 2
iau.incs width 4                       || sau.mul.i32 i0 i0 counterK
iau.mul i1 counterK width
sau.sub.i32 width width 4
iau.shl i2 counterK 2                  || sau.add.i32 q0 q0 i0
iau.add refl_vec_base refl_vec_base i2
iau.add base_mat base_mat i1           || lsu0.cp i13 refl_vec_base

//m_copy(z1,z0)//   // no copy here, just switch pointers
//pbase = (float4*)&(z0->base[(k-1)*width + k])//
//*pbase++ = 0.0//
//z0->base[(k-1)*width + k-1] = 1//
iau.sub i12 base_mat width
iau.sub i12 i12 4

lsu0.ldih i0 0x3f80        || lsu1.ldil i0 0           || vau.xor v0 v0 v0
cmu.shliv.x32 v0 v0 i0     || iau.xor i1 i1 i1
iau.shr.u32 i0 width 4
lsu0.st.64.l v0 i12        || lsu1.sto.64.h v0 i12 8   || iau.incs i12 4          || vau.xor v0 v0 v0
bru.rpi i0                 || lsu0.st.64.l v0 i12      || lsu1.sto.64.h v0 i12 8  || iau.incs i12 16
iau.sub i12 i12 4          || lsu0.cp i0 height

//z0[i][k-1] = 0
iau.sub i0 i0 counterK
bru.rpi i0                 || lsu0.st.32 i1 i12        || iau.add i12 i12 width
//////////calc-reflection-vector-norm
//refl_vec[i] = z0->base[i*width]//
lsu0.ld.32 i8 refl_vec_base  || iau.add refl_vec_base refl_vec_base width || lsu1.ldil i5 inner_matrix_minor_loop_end
lsu0.ld.32 i9 refl_vec_base  || iau.add refl_vec_base refl_vec_base width || lsu1.ldih i5 inner_matrix_minor_loop_end
lsu0.ld.32 i10 refl_vec_base || iau.add refl_vec_base refl_vec_base width
lsu0.ld.32 i11 refl_vec_base || iau.add refl_vec_base refl_vec_base width
                                iau.xor i21 i21 i21
                                iau.shr.u32 i7 height 2
//element from vector to be increased
lsu0.ld.32 i22 base_mat

bru.rpl i5 i7
cmu.shriv.x32 v0 v0 i8  || lsu0.ld.32 i8 refl_vec_base  || iau.add refl_vec_base refl_vec_base width
cmu.shriv.x32 v0 v0 i9  || lsu0.ld.32 i9 refl_vec_base  || iau.add refl_vec_base refl_vec_base width
cmu.shriv.x32 v0 v0 i10 || lsu0.ld.32 i10 refl_vec_base || iau.add refl_vec_base refl_vec_base width
cmu.shriv.x32 v0 v0 i11 || lsu0.ld.32 i11 refl_vec_base || iau.add refl_vec_base refl_vec_base width
//vsum += t*t//
vau.mul.f32 v0 v0 v0
nop
inner_matrix_minor_loop_end:
nop
//sum = __builtin_shave_sau_sumx_f32_r( vsum )//
sau.sumx.f32 i20 v0
nop 4
sau.add.f32 i21 i20 i21
nop 2
/////////////////calc-reflection-vector-norm-end

/////////////////////compute-N2-begin
lsu0.cp i11 i21
//refl_vec[0]*refl_vec[0]//
sau.mul.f32 i8 i22 i22  || iau.shl i2 counterK 2
lsu0.cp i0 v19.1        || bru.bra sqrtF32
iau.mul i1 counterK width
//sum -= refl_vec[0]*refl_vec[0]
sau.sub.f32 i21 i21 i8
iau.add i1 i1 i2
iau.add i0 i0 i1
//m->base[k*width+k]
lsu0.ld.32 i23 i0
lsu0.ldil i29 calc_minor_end_iter || lsu1.ldih i29 calc_minor_end_iter
calc_minor_end_iter:

//float N1 = sqrt(sum) in i11
//m->base[k*width+k] < 0
lsu0.cp i11 i18        || iau.xor i10 i10 i10        || cmu.cmz.f32 i23
peu.pc1c lt            || cmu.vsignf.f32 i11 i11 [n]

//refl_vec[0] += N1//
sau.add.f32 i22 i22 i11
nop 2
//refl_vec[0]*refl_vec[0]
sau.mul.f32 i8 i22 i22
nop 2

//sum += refl_vec[0]*refl_vec[0]//
sau.add.f32 i21 i21 i8
lsu0.ldil i7 0x0 || lsu1.ldih i7 0x4000 || sau.xor i8 i8 i8  // load fp value of 2 in i7
cmu.vsignf.f32 i8 i7 [n]
//float N2 = -2.0f/sum
sau.div.f32 i11 i8 i21
/////////////////////compute-N2-end

//pbase0 = &q0->base[(k-1)*height]//
//    pbase = (float4*)pbase0//
//
//    for (j = 0// j < height/4// j++)
//    {
//      *pbase++ = 0.0//
//    }
//  q0->base[(k-1)*height + (k-1)] += 1//
//  pbase0 += height//

iau.sub i1 counterK 1      || lsu0.ldih i0 0x3f80        || lsu1.ldil i0 0           || vau.xor v0 v0 v0
iau.shl i1 i1 2            || cmu.shliv.x32 v0 v0 i0
iau.shl i0 height 2
iau.sub i15 q0 i0
iau.add i15 i15 i1
iau.shr.u32 i0 height 2
lsu0.st.64.l v0 i15        || lsu1.sto.64.h v0 i15 8    || iau.incs i15 4          || vau.xor v0 v0 v0
bru.rpi i0                 || lsu0.st.64.l  v0 i15      || lsu1.sto.64.h v0 i15 8  || iau.incs i15 16

//!!!!!!!!!!!!!! the first element in the input matrix is changed, fix later
lsu0.ld.32 i23 base_mat
nop 6
lsu0.st.32 i22 base_mat

lsu0.cp refl_vecI base_mat
iau.add refl_vecI refl_vecI width
cmu.cpivr.x32 v3 i11

//preserve the value of N2 in i21
lsu0.cp i21 i11                 || iau.sub loop_height height counterK
//the element on the diagonal that will be stored separately at i0, and calculated in i2
//q0 points to the first element of the q0 matrix, on the k-th line
iau.shl i1 counterK 2           || sau.shl.x32 i4 height 2
iau.add i0 q0 i1
lsu0.cp i8 i22                  || iau.incs i4 4


compute_householder_matrix_loop:

//  for (i = k// i < height// i++)
//  {
//    reflV_i = refl_vec[i]//
//    pbase = (float4*)pbase0//
//    reflV_j = (float4*)&(refl_vec[0])//
//
//    for (j = 0// j < height/4// j++)
//    {
//      t  = *reflV_j++//
//      t *= reflV_i//
//      t *= N2//
//      *pbase++ = t//
//    }
//    q0->base[i*height + i] += 1//
//    pbase0 += height//
//  }
lsu0.cp refl_vec_base i13      || sau.mul.f32 i1 i22 i22
lsu0.ld.32 i8 refl_vec_base    || iau.add refl_vec_base refl_vec_base width || lsu1.ldil i5 householder_main_loop_end
lsu0.ld.32 i9 refl_vec_base    || iau.add refl_vec_base refl_vec_base width || lsu1.ldih i5 householder_main_loop_end
lsu0.ld.32 i10 refl_vec_base   || iau.add refl_vec_base refl_vec_base width || sau.mul.f32 i2 i1 i21
lsu0.ld.32 i11 refl_vec_base   || iau.add refl_vec_base refl_vec_base width
lsu0.ldil i7 0x0               || lsu1.ldih i7 0x3f80

// reflV = refl_vec[i]
cmu.cpivr.x32 v1 i22           || sau.add.f32 i2 i2 i7
iau.shr.u32 i7 height 2

bru.rpl i5 i7
// t  = *reflV_j++//
cmu.shriv.x32 v0 v0 i8     || lsu0.ld.32 i8 refl_vec_base           || iau.add refl_vec_base refl_vec_base width
cmu.shriv.x32 v0 v0 i9     || lsu0.ld.32 i9 refl_vec_base           || iau.add refl_vec_base refl_vec_base width
cmu.shriv.x32 v0 v0 i10    || lsu0.ld.32 i10 refl_vec_base          || iau.add refl_vec_base refl_vec_base width
cmu.shriv.x32 v0 v0 i11    || lsu0.ld.32 i11 refl_vec_base          || iau.add refl_vec_base refl_vec_base width
householder_main_loop_end:
// t *= reflV_i
vau.mul.f32 v2 v0 v1
nop 2
//t *= N2
vau.mul.f32 v4 v2 v3
nop 2
lsu0.st.64.l v4 q0 || lsu1.sto.64.h v4 q0 8 || iau.incs q0 16

iau.deccmp loop_height 1
lsu0.ld.32 i22 refl_vecI || iau.add refl_vecI refl_vecI width || peu.pcix.neq 0 || bru.bra compute_householder_matrix_loop
//q0->base[i*height + i] += 1//
lsu0.st.32 i2 i0
iau.add i0 i0 i4
nop 4
////////////////end-of-householder-matrix-computation

//m_mlt(q0,z1,z0)//
lsu0.st.32 i23 base_mat || bru.bra mvcvMatMul_asm
cmu.cpvi.x32 i18 v17.3
cmu.cpvi.x32 i17 v17.0
cmu.cpvi.x32 i16 v17.1
lsu0.cp i9 i30
lsu0.ldil i30 Q_intermed_computing
lsu0.ldih i30 Q_intermed_computing
Q_intermed_computing:


//m_mlt(q0,Q,q1)// // multiply Q with q0
bru.bra mvcvMatMul_asm
cmu.cpvi.x32 i18 v17.3
cmu.cpvi.x32 i17 v19.2
cmu.cpvi.x32 i16 v17.2
lsu0.ldil i30 Q_main_loop_computing
lsu0.ldih i30 Q_main_loop_computing
nop
Q_main_loop_computing:

lsu0.cp i30 i9

//m_copy(q1,Q)//  // copy q1 into Q
cmu.cpvi.x32 i15 v18.2 //src
cmu.cpvi.x32 i16 v19.3 //dst

lsu0.ldil i1 CDMA_CFG_LINK || lsu1.ldih i1 CDMA_CFG_LINK
lsu0.ldil i2 CDMA_CTRL     || lsu1.ldih i2 CDMA_CTRL
lsu0.ldil i3 CDMA_DST_SRC  || lsu1.ldih i3 CDMA_DST_SRC
lsu0.ldil i4 CDMA_LEN      || lsu1.ldih i4 CDMA_LEN

iau.mul i5 height height
lsu0.ldil i0 0x574 || lsu1.ldih i0 0x0010
lsu0.st.32 i0 i1   || iau.shl i5 i5 2
lsu0.st.64 i15 i3
lsu0.st.32 i5 i4
lsu0.ldil i0 0x5   || lsu1.ldih i0 0x0
lsu0.st.32 i0 i2

iau.deccmp iterEnd 1
peu.pc1i gt        || bru.bra QR_Decomp_Loop
cmu.vszm.word v18 v18 [3201] // switch z0 with z1
cmu.vszm.word v17 v17 [3201] // switch z0 with z1
cmu.cpvi.x32 base_mat v18.0
lsu0.cp refl_vec_base base_mat
cmu.cpvi.x32 q0 v18.3
iau.incs counterK 1

//////////////////////////////////////////////////////////////////////////////////////////////End-QR-Decomp-Main-Loop\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

//m_mlt(Q,m,R)
bru.bra mvcvMatMul_asm
cmu.cpvi.x32 i18 v19.2
cmu.cpvi.x32 i17 v19.0
cmu.cpvi.x32 i16 v16.3
lsu0.ldil i30 matrix_R_computing
lsu0.ldih i30 matrix_R_computing
nop
matrix_R_computing:

//m_transp(Q, q0)//
bru.bra mvcvMatTranspose_asm
cmu.cpvi.x32 i18 v19.2
cmu.cpvi.x32 i17 v17.3
lsu0.ldil i30 Final_Q_Transpose
lsu0.ldih i30 Final_Q_Transpose
nop 2
Final_Q_Transpose:

//m_copy(q0,Q)//
cmu.cpvi.x32 i15 v18.3 //src
cmu.cpvi.x32 i16 v19.3 //dst

lsu0.ldil i1 CDMA_CFG_LINK || lsu1.ldih i1 CDMA_CFG_LINK
lsu0.ldil i2 CDMA_CTRL     || lsu1.ldih i2 CDMA_CTRL
lsu0.ldil i3 CDMA_DST_SRC  || lsu1.ldih i3 CDMA_DST_SRC
lsu0.ldil i4 CDMA_LEN      || lsu1.ldih i4 CDMA_LEN

iau.mul i5 height height
lsu0.ldil i0 0x574 || lsu1.ldih i0 0x0010
lsu0.st.32 i0 i1   || iau.shl i5 i5 2
lsu0.st.64 i15 i3
lsu1.st.32 i5 i4
lsu0.ldil i0 0x5   || lsu1.ldih i0 0x0
lsu0.sto.32 i0 i2 0


lsu0.ld.64 i30 i19 || iau.add i19 i19 8
lsu0.ld.64 i28 i19 || iau.add i19 i19 8
lsu0.ld.64 i26 i19 || iau.add i19 i19 8
lsu0.ld.64 i24 i19 || iau.add i19 i19 8
lsu0.ld.64 i22 i19 || iau.add i19 i19 8
lsu0.ld.64 i20 i19 || iau.add i19 i19 8
nop
bru.jmp i30
nop 6

// 48 instructions, 48instr/value

// float sqrtf( float x )
// {
//   int exp//
//   float f = frexp( x, &exp )//
//   f *= 0.5//
//   float y = 0.41731 + 1.18032*f//
//   y = y * 0.5 + f / y//
//   y = y * 0.5 + f / y//
//   if( exp & 1 ) {
//     y *= __SQRT_HALF//
//     ++exp//
//   }
//   exp >>= 1//
//   y = ldexpf( y, exp )//
//   if( isinf( x )|| isnan( x ))
//     y = x//
//   if( !x )
//     y = 0//
//   if( x < 0 )
//     y = NAN//
//   return y//
// }

.size mvcvMatQRDecomp_asm,.-mvcvMatQRDecomp_asm
  .code .text.sqrtF32
  .salign 16
  sqrtF32:
  .set   x i11
  .set   f i10
  .set   y i9
  .set exp i8
  .set nex i7 // new exponent

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
  iau.shr.i32 nex, nex 1  || bru.jmp i29
  iau.and   nex, nex i0   || cmu.cmii.i32 exp  i0
    iau.add     y, y nex  || sau.or       exp, exp exp
  // if( exp==0xff ) y=x//
    lsu0.cp     y, x      || peu.pc1c eq
  // if( !exp ) y=0.0//
    lsu0.ldil   y, 0      || peu.pc1s eq
    sau.and   i18, y i2   || iau.add i0, x 0
  // if( x<0 ) y=qNaN//
    lsu0.cp   i18, i4     || peu.pc1i lt

.nowarnend
.end
