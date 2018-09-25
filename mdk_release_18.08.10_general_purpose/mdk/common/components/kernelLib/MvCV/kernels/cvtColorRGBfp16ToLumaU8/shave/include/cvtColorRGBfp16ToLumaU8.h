#ifndef __CVTCOLORKERNELRGBFP16TOLUMAU8_H__
#define __CVTCOLORKERNELRGBFP16TOLUMAU8_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel performs color space conversion RGBfp16 to LumaU8 for one line in an image
/// @param[in]  inRGB   - First lines from input RGB planes
/// @param[Out] yOut    - Luma plane
/// @param[in]  width   - Line width in pixels
/// @return     Nothing

MVCV_FUNC(void, mvcvCvtColorRGBfp16ToLumaU8, half* inRGB[3], u8* yOut, u32 width)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
    void mvcvCvtColorRGBfp16ToLumaU8_opt(short float * __restrict inRGB[3], unsigned char * __restrict yOut, unsigned int width);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__CVTCOLORKERNELRGBFP16TOLUMAU8_H__
