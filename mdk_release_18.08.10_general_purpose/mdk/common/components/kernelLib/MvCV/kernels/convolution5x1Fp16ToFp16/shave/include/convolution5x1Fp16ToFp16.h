#ifndef __CONVOLUTION5x1Fp16ToFp16_H__
#define __CONVOLUTION5x1Fp16ToFp16_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel performs a convolution on the f16 input image using the given 5x1 matrix
/// @param[in] in         - Input lines, 16-bits floating point
/// @param[in] out        - Output line, 16-bits floating point
/// @param[in] conv       - 5 elements array with fp16 values containing the 5x1 convolution matrix
/// @param[in] inWidth    - Width of input line

MVCV_FUNC(void, mvcvConvolution5x1Fp16ToFp16, half** in, half** out, half* conv, u32 inWidth)
//!@}

#endif //__CONVOLUTION5x1Fp16ToFp16_H__