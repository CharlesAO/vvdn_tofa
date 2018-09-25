#ifndef __CVTCOLORKERNELNV21TORGB_H__
#define __CVTCOLORKERNELNV21TORGB_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{	
/// This kernel performs color space conversion NV21 to RGB for one lines in an image
/// @param[in] yin   - First line from input Y plane
/// @param[in] uvin  - First line from input UV plane interleaved
/// @param[out] outR - R plane in the first line from the output RGB image
/// @param[out] outG - G plane in the first line from the output RGB image
/// @param[out] outB - B plane in the first line from the output RGB image
/// @param[in] width - Line width in pixels
/// @return    Nothing

MVCV_FUNC(void, mvcvCvtColorNV21toRGB,u8** yin, u8** uvin, u8** outR, u8** outG, u8** outB, u32 width)
//!@}

#endif //__CVTCOLORKERNELNV21TORGB_H__
