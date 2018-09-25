#ifndef __MINMAXVALUE_H__
#define __MINMAXVALUE_H__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel computes the minimum and the maximum value of a given input image
/// @param[in] in         - Input lines
/// @param[in] width      - Line`s width(length)
/// @param[in] height     - Height of image (defaulted to one line)
/// @param[in] minVal     - Stores the minimum value on the line
/// @param[in] maxVal     - Stores the maximum value on the line
/// @param[in] maskAddr   - Mask filled with 1s and 0s which determines the image area to compute minimum and maximum
/// @return               - Nothing

MVCV_FUNC(void, mvcvMinMaxKernel, u8** in, u32 width, u32 height, u8* minVal, u8* maxVal, u8* maskAddr)
//!@}

#endif //__MINMAXVALUE_H__