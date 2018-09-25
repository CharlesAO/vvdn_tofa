#ifndef __GAUSS5x1_U32IN_U16OUT_H__
#define __GAUSS5x1_U32IN_U16OUT_H__
#include <mv_types.h>
#include <mvcv_macro.h>
    

//!@{
/// This kernel performs a convolution on the input image using the 1 4 6 4 1 matrix
/// @param[in] in         - Input lines
/// @param[in] out        - Output line
/// @param[in] inWidth    - Width of input line

MVCV_FUNC(void, mvcvGauss5x1_u32in_u16out, u32** in, u16** out, u32 inWidth)
//!@}

#endif //__GAUSS5x1_U32IN_U16OUT_H__
