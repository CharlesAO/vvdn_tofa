#ifndef __CONVOLUTION7x7_H__
#define __CONVOLUTION7x7_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel performs a convolution on the input image using the given 7x7 matrix
/// @param[in] in         - Input lines
/// @param[in] out        - Output line
/// @param[in] conv       - 49 elements array with fp16 values containing the 7x7 convolution matrix
/// @param[in] inWidth    - Width of input line
MVCV_FUNC(void, mvcvConvolution7x7, u8** in, u8** out, half conv[49], unsigned int inWidth)
//!@}

#endif //__CONVOLUTION7x7_H__
