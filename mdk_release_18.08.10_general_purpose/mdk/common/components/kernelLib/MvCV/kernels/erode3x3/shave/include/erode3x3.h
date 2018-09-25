#ifndef __ERODE3x3_H__
#define __ERODE3x3_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel erodes the source image using the specified structuring element.
/// This is determined by the shape of a pixel neighborhood over which the minimum is taken.
/// @param[in]  src      - Input lines of the input image
/// @param[out] dst      - Output line
/// @param[in]  kernel   - Kernel filled with 1s and 0s which determines the image area to erode on
/// @param[in]  width    - Width  of the input line

MVCV_FUNC(void, mvcvErode3x3, u8** src, u8** dst, u8** kernel, u32 width)
//!@}

#endif //__ERODE3x3_H__
