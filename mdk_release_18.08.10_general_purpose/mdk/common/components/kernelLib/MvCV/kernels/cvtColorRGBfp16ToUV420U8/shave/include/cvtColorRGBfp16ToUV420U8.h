#ifndef __CVTCOLORKERNELRGBFP16TOUV420U8_H__
#define __CVTCOLORKERNELRGBFP16TOUV420U8_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{	
/// This kernel performs color space conversion RGBfp16 to UV420U8 for one line in an image
/// @param[in]  inRGB   - First lines from input RGB planes 
/// @param[Out] uOut    - U plane 
/// @param[Out] vOut    - V plane 
/// @param[in]  width   - Line width in pixels
/// @return     Nothing

MVCV_FUNC(void, mvcvCvtColorRGBfp16ToUV420U8, half* inRGB[3], u8* uOut, u8* vOut, u32 width)
//!@}

#endif //__CVTCOLORKERNELRGBFP16TOUV420U8_H__
