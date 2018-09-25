#ifndef __DILATE5x5_H__
#define __DILATE5x5_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel dilates the source image using the specified structuring element.
/// This is determined by the shape of a pixel neighborhood over which the maximum is taken.
/// @param[in]  src      - Input lines of the input image
/// @param[out] dst      - Output lines
/// @param[in]  kernel   - Kernel filled with 1s and 0s which determines the image area to dilate on
/// @param[in]  width    - Width  of the input line

MVCV_FUNC(void, mvcvDilate5x5, u8** src, u8** dst, u8** kernel, u32 width)
//!@}

#endif //__DILATE5x5_H__
