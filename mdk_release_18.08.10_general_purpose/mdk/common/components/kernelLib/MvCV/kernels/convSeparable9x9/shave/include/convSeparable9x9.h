#ifndef __CONVOLUTIONSEPARABLE9X9_H__
#define __CONVOLUTIONSEPARABLE9X9_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs a separable convolution on the input image using the given 9x9 matrix
/// @param[in] out        - Output line	
/// @param[in] in         - Input lines
/// @param[in] conv       - Array of values from the 9x9 separable convolution matrix
/// @param[in] inWidth    - Width of input line
    
MVCV_FUNC(void, mvcvConvSeparable9x9, u8** out, u8** in, float conv[5], u32 inWidth)
//!@}


#endif //__CONVOLUTIONSEPARABLE9X9_H__