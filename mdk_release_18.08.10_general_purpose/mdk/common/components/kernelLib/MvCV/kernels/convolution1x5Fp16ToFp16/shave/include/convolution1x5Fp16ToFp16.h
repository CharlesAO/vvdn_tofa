#ifndef __CONVOLUTION1x5FP16TOFP16_H__
#define __CONVOLUTION1x5FP16TOFP16_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel performs a convolution on the fp16 input image using the given 1x5 matrix
/// @param[in] in         - Input lines, 16-bits floating point 
/// @param[in] out        - Output line, 16-bits floating point
/// @param[in] conv       - 15 elements array with fp16 values containing the 1x5 convolution matrix
/// @param[in] inWidth    - Width of input line

MVCV_FUNC(void, mvcvConvolution1x5Fp16ToFp16, half** in, half** out, half *conv, u32 inWidth)
//!@}

#endif //__CONVOLUTION1x5FP16TOFP16_H__
