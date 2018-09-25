#ifndef __CONVOLUTION5x5Fp16ToFp16_H__
#define __CONVOLUTION5x5Fp16ToFp16_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel performs a convolution on the fp16 input image using the given 5x5 matrix
/// @param[in] in         - Input lines, 16-bits floating point
/// @param[in] out        - Output line, 16-bits floating point
/// @param[in] conv       - 25 elements array with fp16 values containing the 5x5 convolution matrix
/// @param[in] inWidth    - Width of input line

MVCV_FUNC(void, mvcvConvolution5x5Fp16ToFp16, half** in, half** out, half conv[25], u32 inWidth)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
extern "C" 
{
	void mvcvConvolution5x5Fp16ToFp16_opt(half** in, half** out, half conv[25], u32 inWidth);
}
#endif

#endif //__CONVOLUTION5x5_H__
