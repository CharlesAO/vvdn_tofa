#ifndef __SLaplacian3x3Fp16ToFp16_H__
#define __SLaplacian3x3Fp16ToFp16_H__
#include <mv_types.h>
#include <mvcv_macro.h>
//#include "half.h"

//!@{
/// This filter applies a Laplacian filter with 3x3 size in this case
/// ( see http://en.wikipedia.org/wiki/Discrete_Laplace_operator ) 
/// @param[in] in     - Input lines, 16-bits floating point
/// @param[out] out   - Output line, 16-bits floating point
/// @param[in] width  - Width of input line

MVCV_FUNC(void, mvcvSLaplacian3x3Fp16ToFp16, half** in, half** out, u32 inWidth)
//!@}

#endif //__SLaplacian3x3Fp16ToFp16_H__