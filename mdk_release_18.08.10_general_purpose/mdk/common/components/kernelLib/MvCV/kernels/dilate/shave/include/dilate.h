#ifndef __DILATE_H__
#define __DILATE_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel dilates the source image using the specified structuring element.
/// This is determined by the shape of a pixel neighborhood over which the maximum is taken.
/// @param[in]  src      - Input lines of the input image
/// @param[out] dst      - Output line
/// @param[in]  kernel   - Kernel filled with 1s and 0s which determines the image area to dilate on
/// @param[in]  width    - Width  of the input line
/// @param[in]  height   - Height of the input line (defaulted to one line)
/// @param[in]  k        - Kernel size

MVCV_FUNC(void, mvcvDilate, u8** src, u8** dst, u8** kernel, u32 width, u32 height, u32 k)
//!@}

#endif //__DILATE_H__