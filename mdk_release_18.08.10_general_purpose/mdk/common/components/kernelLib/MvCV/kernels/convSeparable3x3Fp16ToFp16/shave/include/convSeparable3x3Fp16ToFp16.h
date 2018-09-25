#ifndef __CONVOLUTIONSEPARABLE3X3FP16TOFP16_H__
#define __CONVOLUTIONSEPARABLE3X3FP16TOFP16_H__
#include <mv_types.h>
#include <mvcv_macro.h>
//#include <half.h>

//!@{
/// This kernel performs a separable convolution on the input image using the given 3x3 matrix
/// @param[in] out        - Output line	
/// @param[in] in         - Input lines
/// @param[in] conv       - Array of Fp16 values from 3x3 separable convolution matrix
/// @param[in] inWidth    - Width of input line
    
MVCV_FUNC(void, mvcvConvSeparable3x3Fp16ToFp16, half* out, half** in, half conv[2], u32 inWidth)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
	void mvcvConvSeparable3x3Fp16ToFp16_opt(half* out, half** in, half conv[2], u32 inWidth);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__CONVOLUTIONSEPARABLE3X3FP16TOFP16_H__
