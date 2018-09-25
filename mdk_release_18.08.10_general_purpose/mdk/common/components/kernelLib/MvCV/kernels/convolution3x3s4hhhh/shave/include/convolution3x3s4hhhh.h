#ifndef __CONVOLUTION3X3s4hhhh_H__
#define __CONVOLUTION3X3s4hhhh_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs a convolution with stride of 4 on the input image using the given 3x3 matrix
/// stride = 4(s4); input data type = half(h); coefficient data type = half(h); computation/accumulation datatype = half(h); output data type = half(h);
/// @param[in] in         - Input lines
/// @param[in] out        - Output line
/// @param[in] conv       - 9 elements array with half(h) values containing the 3x3 convolution matrix
/// @param[in] inWidth    - Width of input line
	
MVCV_FUNC(void, mvcvConvolution3x3s4hhhh, half** in, half** out, half conv[9], u32 inWidth)
//!@}

#endif 
