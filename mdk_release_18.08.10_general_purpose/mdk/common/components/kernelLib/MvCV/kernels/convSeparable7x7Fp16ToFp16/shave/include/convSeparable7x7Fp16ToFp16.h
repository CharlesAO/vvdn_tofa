#ifndef __CONVOLUTIONSEPARABLE7x7FP16TOFP16_H__
#define __CONVOLUTIONSEPARABLE7x7FP16TOFP16_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs a convolution on the input image using the given 7x7 matrix
/// @param[in] out        - Output line	
/// @param[in] in         - Input lines
/// @param[in] conv       - Array of Fp16 values from 7x7 convolution matrix
/// @param[in] inWidth    - Width of input line
    
MVCV_FUNC(void, mvcvConvSeparable7x7Fp16ToFp16, half** out, half** in, half conv[4], u32 inWidth)
//!@}


#endif //__CONVOLUTIONSEPARABLE7x7FP16TOFP16_H__
