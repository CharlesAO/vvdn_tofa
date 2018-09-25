#ifndef __CONVOLUTION7x7S8HHHH_H__
#define __CONVOLUTION7x7S8HHHH_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs a convolution on the fp16 input image using the given 7x7 matrix
/// stride = 8(s8); input data type = half(h); coefficient data type = half(h); computation/accumulation datatype = half(h); output data type = half(h);
/// @param[in] in         - Input lines, 16-bits floating point
/// @param[in] out        - Output line, 16-bits floating point
/// @param[in] conv       - 49 elements array with fp16 values containing the 7x7 convolution matrix
/// @param[in] inWidth    - Width of input line

MVCV_FUNC(void, mvcvConvolution7x7s8hhhh, half** in, half** out, half *conv, unsigned int inWidth)
//!@}

#endif //__CONVOLUTION7x7S8HHHH_H__
