#ifndef __CONVOLUTIONSEPARABLE7x7_H__
#define __CONVOLUTIONSEPARABLE7x7_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs a separable convolution on the input image using the given 7x7 matrix
/// @param[in] out        - Output line	
/// @param[in] in         - Input lines
/// @param[in] conv       - Array of values from 7x7 separable convolution matrix
/// @param[in] inWidth    - Width of input line
    
MVCV_FUNC(void, mvcvConvSeparable7x7, u8** out, u8** in, float conv[4], u32 inWidth)
//!@}


#endif //__CONVOLUTIONSEPARABLE7x7_H__