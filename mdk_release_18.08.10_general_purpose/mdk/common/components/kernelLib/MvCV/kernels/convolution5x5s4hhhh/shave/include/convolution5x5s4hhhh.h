#ifndef __CONVOLUTION5x5S4HHHH_H__
#define __CONVOLUTION5x5S4HHHH_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel performs a convolution on the fp16 input image using the given 5x5 matrix
/// stride = 4(s4); input data type = half(h); coefficient data type = half(h); computation/accumulation datatype = half(h); output data type = half(h);
/// @param[in] in         - Input lines, 16-bits floating point
/// @param[in] out        - Output line, 16-bits floating point
/// @param[in] conv       - 25 elements array with fp16 values containing the 5x5 convolution matrix
/// @param[in] inWidth    - Width of input line

MVCV_FUNC(void, mvcvConvolution5x5s4hhhh, half** in, half** out, half conv[25], u32 inWidth)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
	void mvcvConvolution5x5s4hhhh_opt(half** in, half** out, half conv[25], u32 inWidth);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__CONVOLUTION5x5S4HHHH_H__
