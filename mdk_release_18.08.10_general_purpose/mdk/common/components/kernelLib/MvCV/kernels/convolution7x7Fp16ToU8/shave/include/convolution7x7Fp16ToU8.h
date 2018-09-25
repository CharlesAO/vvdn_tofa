#ifndef __CONVOLUTION7x7FP16TOU8_H__
#define __CONVOLUTION7x7FP16TOU8_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel performs a convolution on the fp16 input image using the given 7x7 matrix
/// @param[in] in         - Input lines, 16-bits floating point
/// @param[in] out        - Output line, 8-bits unsigned char
/// @param[in] conv       - 49 elements array with fp16 values containing the 7x7 convolution matrix
/// @param[in] inWidth    - Width of input line

MVCV_FUNC(void, mvcvConvolution7x7Fp16ToU8, half** in, u8** out, half *conv, unsigned int inWidth)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
	void mvcvConvolution7x7Fp16ToU8_opt(half** in, u8** out, half *conv, unsigned int inWidth);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__CONVOLUTION7x7FP16TOU8_H__
