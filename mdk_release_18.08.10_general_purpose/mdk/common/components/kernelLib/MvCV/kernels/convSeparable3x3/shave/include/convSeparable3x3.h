#ifndef __CONVOLUTIONSEPARABLE3X3_H__
#define __CONVOLUTIONSEPARABLE3X3_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs a separable convolution on the input image using the given 3x3 matrix
/// @param[in] out        - Output line	
/// @param[in] in         - Input lines
/// @param[in] conv       - Array of values from 3x3 separable convolution matrix
/// @param[in] inWidth    - Width of input line
    
MVCV_FUNC(void, mvcvConvSeparable3x3, u8* out, u8** in, float conv[2], u32 inWidth)
//!@}


#endif //__CONVOLUTIONSEPARABLE3X3_H__