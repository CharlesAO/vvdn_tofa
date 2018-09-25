#ifndef __CONVOLUTION9x9S2HHHH_H__
#define __CONVOLUTION9x9S2HHHH_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel performs a convolution on the input image using the given 9x9 matrix
/// stride = 2(s2); input data type = half(h); coefficient data type = half(h); computation/accumulation datatype = half(h); output data type = half(h);
/// @param[in] in         - Input lines
/// @param[in] out        - Output line
/// @param[in] conv       - Array of Fp16 values from 9x9 convolution matrix
/// @param[in] inWidth    - Width of input line

MVCV_FUNC(void, mvcvConvolution9x9s2hhhh, half** in, half** out, half conv[81], u32 inWidth)
//!@}

#endif //__CONVOLUTION9x9S2HHHH_H__
