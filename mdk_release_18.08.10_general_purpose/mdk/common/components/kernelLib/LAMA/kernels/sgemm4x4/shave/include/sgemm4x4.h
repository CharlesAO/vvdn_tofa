#ifndef __SGEMM4x4_H__
#define __SGEMM4x4_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// sgemm4x4 kernel
/// @param[in] k     - number of columns of A and rows of B
/// @param[in] alpha - address of a scalar to the A * B product.
/// @param[in] a     - 4 x k "micro-panel" matrix stored in packed (column-stored)
/// @param[in] b     - k x 4 "micro-panel" matrix in packed (row-stored) format
/// @param[in] beta  - address of a scalar to the input value of matrix C
/// @param[out] c    - address of a matrix C of dimension 4 x 4, stored according to rsc and csc.
/// @param[in] rs_c  - row stride of matrix C
/// @param[in] cs_c  - column stride of matrix C

MVCV_FUNC(void, mvcvsgemm4x4, s32 k, float *alpha, float *a, float *b, float *beta, float *c, s32 rs_c, s32 cs_c)
//!@}

#endif //__SGEMM4x4_H__
