#ifndef __CONVOLUTION5x5s3hhhh_H__
#define __CONVOLUTION5x5s3hhhh_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel performs a convolution with stride 3 on the input image using the given 5x5 matrix
/// stride = 3(s3);
/// input data type = half(h);
/// coefficient data type = half(h);
/// computation/accumulation datatype = half(h);
/// output data type = half(h);
/// @param[in] in         - Input lines
/// @param[in] out        - Output line
/// @param[in] conv       - Array of Fp16 values from 5x5 convolution matrix
/// @param[in] inWidth    - Width of input line

MVCV_FUNC(void, mvcvConvolution5x5s3hhhh, half** in, half** out, half conv[25], u32 inWidth)
//!@}

#endif //__CONVOLUTION5x5s3hhhh_H__
