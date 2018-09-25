#ifndef __SLAPLACIAN7x7Fp16ToFp16_H__
#define __SLAPLACIAN7x7Fp16ToFp16_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This filter applies a Laplacian filter with 7x7 size in this case
/// ( see http://en.wikipedia.org/wiki/Discrete_Laplace_operator ) 
/// @param[in] in     - Input lines, 16-bits floating points
/// @param[out] out   - Output line, 16-bits floating points
/// @param[in] width  - Width of input line

MVCV_FUNC(void, mvcvSLaplacian7x7Fp16ToFp16, half** in, half** out, u32 inWidth)
//!@}

#endif //__SLAPLACIAN7x7Fp16ToFp16_H__
