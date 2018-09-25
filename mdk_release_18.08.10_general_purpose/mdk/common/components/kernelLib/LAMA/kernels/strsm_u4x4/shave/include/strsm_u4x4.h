#ifndef __STRSM_U4x4_H__
#define __STRSM_U4x4_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// strsm_u 4x4 kernel
/// @param[in]  a     - address of input matrix (upper triangular)
/// @param[in]  b     - address of input matrix
/// @param[out] c     - address of the output matrix
/// @param[in]  rs_c  - row stride
/// @param[in]  cs_c  - column stride

MVCV_FUNC(void, lamaStrsm_u4x4, float *a, float *b, float *c, int rs_c, int cs_c)

//!@}
#endif //__STRSM_U4x4_H__
