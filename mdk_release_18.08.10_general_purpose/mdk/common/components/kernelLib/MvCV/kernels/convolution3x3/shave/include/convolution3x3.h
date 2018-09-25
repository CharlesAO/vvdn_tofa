#ifndef __CONVOLUTION3X3_H__
#define __CONVOLUTION3X3_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel performs a convolution on the input image using the given 3x3 matrix
/// @param[in] in         - Input lines
/// @param[in] out        - Output line
/// @param[in] conv       - 9 elements array with fp16 values containing the 3x3 convolution matrix
/// @param[in] inWidth    - Width of input line
	
MVCV_FUNC(void, mvcvConvolution3x3, u8** in, u8** out, half conv[9], u32 inWidth)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
extern "C" 
{
	void mvcvConvolution3x3_opt(u8** in, u8** out, half conv[9], u32 inWidth);
}
#endif

#endif //__CONVOLUTION3X3_H__