#ifndef __CONVOLUTION11x11S1HHHH_H__  
#define __CONVOLUTION11x11S1HHHH_H__   
#include <mv_types.h>
#include <mvcv_macro.h>
#include <stdio.h>
#include <stdlib.h>


//!@{
/// This kernel performs a convolution on the fp16 input image  using the given 11x11 matrix
/// stride = 1(s1); input data type = half(h); coefficient data type = half(h); computation/accumulation datatype = half(h); output data type = half(h);
/// @param[in] in         - Input lines
/// @param[in] out        - Output line
/// @param[in] conv       - 121 elements array with fp16 values containing the 11x11 convolution matrix
/// @param[in] inWidth    - Width of input line

MVCV_FUNC(void, mvcvConvolution11x11s1hhhh, half** in, half** out, half conv[121], u32 inWidth)
//!@}


#endif //__CONVOLUTION11x11S1HHHH_H__

