#ifndef __CONVOLUTIONSEPARABLE9X9FP16TOFP16_H__
#define __CONVOLUTIONSEPARABLE9X9FP16TOFP16_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs a separable convolution on the input image using the given 9x9 matrix
/// @param[in] out        - Output line	
/// @param[in] in         - Input lines
/// @param[in] conv       - Array of Fp16 values from the 9x9 separable convolution matrix
/// @param[in] inWidth    - Width of input line
    
MVCV_FUNC(void, mvcvConvSeparable9x9Fp16ToFp16, half** out, half** in, half conv[5], u32 inWidth)
//!@}


#endif //__CONVOLUTIONSEPARABLE9X9FP16TOFP16_H__
