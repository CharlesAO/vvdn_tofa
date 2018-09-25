#ifndef __SGEMMTRSM_L4x4_H__
#define __SGEMMTRSM_L4x4_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// sgemmtrsm_l 4x4 kernel
/// @param[in]  k     - the number of columns of a10 and rows of b01
/// @param[in]  alpha - the address of a scalar to be applied to b11
/// @param[in]  a10   - 4 x k submatrix of the packed (column-stored) micro-panel of A, situated to the left of the 4 x 4 triangular submatrix a11
/// @param[in]  a11   - 4 x 4 lower triangular submatrix within the packed (column-stored) micro-panel of matrix A, situated to the right of a10
/// @param[in]  b01   - k x 4 submatrix of the packed (row-stored) micro-panel of B, situated above the 4 x 4 block b11
/// @param[in]  b11   - 4 x 4 submatrix of the packed (row-stored) micro-panel of B, situated below b01
/// @param[out] c11   - 4 x 4 submatrix of matrix C, stored according to rsc and csc
/// @param[in]  rs_c  - row stride of c11
/// @param[in]  cs_c  - column stride of c11
MVCV_FUNC(void, mvcvsgemmtrsm_l4x4, s32 k, float *alpha, float *a10, float *a11, float *b01, float *b11, float *c11, s32 rs_c, s32 cs_c)
//!@}

#endif //__SGEMMTRSM_L4x4_H__
