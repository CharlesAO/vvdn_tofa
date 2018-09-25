#ifndef __CONVOLUTION7x1Fp16ToFp16_H__
#define __CONVOLUTION7x1Fp16ToFp16_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel performs a convolution on the fp16 input image using the given 7x1 matrix
/// @param[in] in         - Input lines
/// @param[in] out        - Output line
/// @param[in] conv       - 7 elements array with fp16 values containing the 7x1 convolution matrix
/// @param[in] inWidth    - Width of input line

MVCV_FUNC(void, mvcvConvolution7x1Fp16ToFp16, half** in, half* out, half* conv, u32 inWidth)
//!@}

#endif //__CONVOLUTION7x1Fp16ToFp16_H__
