#ifndef __CONVOLUTION1x5_H__
#define __CONVOLUTION1x5_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel performs a convolution on the input image using the given 1x5 matrix
/// @param[in] in         - Input lines
/// @param[in] out        - Output line
/// @param[in] conv       - 5 elements array with fp16 values containing the 1x5 convolution matrix
/// @param[in] inWidth    - Width of input line

MVCV_FUNC(void, mvcvConvolution1x5, u8** in, u8** out, half conv[5], u32 inWidth)
//!@}

#endif //__CONVOLUTION1x5_H__
