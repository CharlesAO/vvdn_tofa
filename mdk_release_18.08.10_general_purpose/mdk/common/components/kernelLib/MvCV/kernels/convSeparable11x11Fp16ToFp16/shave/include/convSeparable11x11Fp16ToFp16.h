#ifndef __CONVOLUTIONSEPARABLE11X11FP16TOFP16_H__
#define __CONVOLUTIONSEPARABLE11X11FP16TOFP16_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs a separable convolution on the input image using the given 11x11 matrix
/// @param[in] out        - Output line	
/// @param[in] in         - Input lines
/// @param[in] conv       - Array of Fp16 values from 11x11 separable convolution matrix
/// @param[in] inWidth    - Width of input line

MVCV_FUNC(void, mvcvConvSeparable11x11Fp16ToFp16, half** out, half** in, half conv[6], u32 inWidth)
//!@}

#endif //__CONVOLUTIONSEPARABLE11X11FP16TOFP16_H__
