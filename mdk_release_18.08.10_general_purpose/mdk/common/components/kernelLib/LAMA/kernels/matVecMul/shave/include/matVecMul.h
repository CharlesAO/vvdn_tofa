#ifndef __MAT_VEC_MUL_H__
#define __MAT_VEC_MUL_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// vector matrix multiplication kernel
/// @param[in]  A - input vector of size 1*K
/// @param[in]  B - input matrix of size K*N
/// @param[out] C - input/output vector of size 1*N
/// @param[in]  K - width of A vector
/// @param[in]  N - number of columns of matrix B

MVCV_FUNC(void, mvcvMatVecMul, float *a, float *b, float *c, int k, int n)
//!@}

#endif //__MAT_VEC_MUL_H__
