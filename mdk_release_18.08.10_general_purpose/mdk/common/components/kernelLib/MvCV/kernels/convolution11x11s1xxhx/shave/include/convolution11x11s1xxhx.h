#ifndef __CONVOLUTION11x11S1XXHX_H__
#define __CONVOLUTION11x11S1XXHX_H__
#include <mv_types.h>
#include <mvcv_macro.h>
#include <stdio.h>
#include <stdlib.h>


//!@{
/// This kernel performs a convolution on the input image using the given 11x11 matrix
/// @param[in] in         - Input lines
/// @param[in] out        - Output line
/// @param[in] conv       - 121 elements array with u8 values containing the 11x11 convolution matrix
/// @param[in] inWidth    - Width of input line

MVCV_FUNC(void, mvcvConvolution11x11s1xxhx, u8** in, u8** out, u8 conv[121], u32 inWidth)
//!@}


#endif //__CONVOLUTION11x11S1XXHX_H__

