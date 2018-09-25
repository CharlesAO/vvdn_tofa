#ifndef __CONVOLUTIONSEPARABLE11X11_H__
#define __CONVOLUTIONSEPARABLE11X11_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs a separable convolution on the input image using the given 11x11 matrix
/// @param[out] out       - Output line	
/// @param[in] in         - Input lines
/// @param[in] conv       - Array with 6 fp32 values that represent the separable values for 11x11 matrix
/// @param[in] inWidth    - Width of input line

MVCV_FUNC(void, mvcvConvSeparable11x11, u8** out, u8** in, float conv[6], u32 inWidth)
//!@}

#endif //__CONVOLUTIONSEPARABLE11X11_H__