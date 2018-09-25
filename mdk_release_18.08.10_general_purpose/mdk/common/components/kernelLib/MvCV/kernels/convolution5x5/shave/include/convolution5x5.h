#ifndef __CONVOLUTION5x5_H__
#define __CONVOLUTION5x5_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel performs a convolution on the input image using the given 5x5 matrix
/// @param[in] in         - Input lines
/// @param[in] out        - Output line
/// @param[in] conv       - 25 elements array with fp16 values containing the 5x5 convolution matrix
/// @param[in] inWidth    - Width of input line

MVCV_FUNC(void, mvcvConvolution5x5, u8** in, u8** out, half conv[25], u32 inWidth)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
extern "C" 
{
	void mvcvConvolution5x5_opt(u8** in, u8** out, half conv[25], u32 inWidth);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__CONVOLUTION5x5_H__
