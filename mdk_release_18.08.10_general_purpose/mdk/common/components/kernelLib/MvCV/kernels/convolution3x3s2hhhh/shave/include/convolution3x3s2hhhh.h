#ifndef __CONVOLUTION3X3S2hhhh_H__
#define __CONVOLUTION3X3S2hhhh_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel performs a convolution on the fp16 input image using the given 3x3 matrix
/// stride = 2(s2);
/// input data type = half(h);
/// coefficient data type = half(h);
/// computation/accumulation datatype = half(h);
/// output data type = half(h);
/// @param[in] in         - Input lines, 16-bits floating point
/// @param[in] out        - Output line, 16-bits floating point
/// @param[in] conv       - 9 elements array with fp16 values containing the 3x3 convolution matrix
/// @param[in] inWidth    - Width of input line

MVCV_FUNC(void, mvcvConvolution3x3s2hhhh, half** in, half** out, half conv[9], u32 inWidth)
//!@}

#endif //__CONVOLUTION3X3S2hhhh_H__