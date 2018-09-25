#ifndef __CONVOLUTION9x9_H__
#define __CONVOLUTION9x9_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel performs a convolution on the input image using the given 9x9 matrix
/// @param[in] in         - Input lines
/// @param[in] out        - Output line
/// @param[in] conv       - 81 elements array with fp16 values containing the 9x9 convolution matrix
/// @param[in] inWidth    - Width of input line

MVCV_FUNC(void, mvcvConvolution9x9, u8** in, u8** out, half conv[81], u32 inWidth)
//!@}

#endif //__CONVOLUTION9x9_H__
