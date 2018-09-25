#ifndef __CONVOLUTIONSEPARABLE5X5FP16TOFP16_H__
#define __CONVOLUTIONSEPARABLE5X5FP16TOFP16_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs a separable convolution on the input image using the given 5x5 matrix
/// @param[in] out        - Output line	
/// @param[in] in         - Input lines
/// @param[in] conv       - Array of Fp16 values from 5x5 separable convolution matrix
/// @param[in] inWidth    - Width of input line

MVCV_FUNC(void, mvcvConvSeparable5x5Fp16ToFp16, half** out, half** in, half conv[3], u32 inWidth)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
extern "C" 
{
	void mvcvConvSeparable5x5Fp16ToFp16_opt(half** out, half** in, half conv[3], u32 inWidth);
}
#endif //MOVICOMPILE_OPTIMIZED


#endif //__CONVOLUTIONSEPARABLE5X5FP16TOFP16_H__
