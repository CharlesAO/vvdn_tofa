#ifndef __CONVOLUTION1x7_H__
#define __CONVOLUTION1x7_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel performs a convolution on the input image using the given 1x7 matrix
/// @param[in] in         - Input lines
/// @param[in] out        - Output line
/// @param[in] conv       - 7 elements array with fp16 values containing the 1x7 convolution matrix
/// @param[in] inWidth    - Width of input line

MVCV_FUNC(void, mvcvConvolution1x7, u8** in, u8** out, half conv[7], u32 inWidth)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
	void mvcvConvolution1x7_opt(u8** in, u8** out, half conv[7], u32 inWidth);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__CONVOLUTION1x7_H__