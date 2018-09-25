#ifndef __CONVOLUTIONSEPARABLE5X5_H__
#define __CONVOLUTIONSEPARABLE5X5_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs a separable convolution on the input image using the given 5x5 matrix
/// @param[in] out        - Output line	
/// @param[in] in         - Input lines
/// @param[in] conv       - Array of values from 5x5 separable convolution matrix
/// @param[in] inWidth    - Width of input line

MVCV_FUNC(void, mvcvConvSeparable5x5, u8** out, u8** in, float conv[3], u32 inWidth)
//!@}


#endif //__CONVOLUTIONSEPARABLE5X5_H__