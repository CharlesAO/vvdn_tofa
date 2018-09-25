#ifndef __CVTCOLORKERNELRGBTONV21_H__
#define __CVTCOLORKERNELRGBTONV21_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{	
/// This kernel performs color space conversion RGB to NV21 for one line in an image
/// @param[in]  inR     - First line from input R plane 
/// @param[in]  inG     - First line from input G plane
/// @param[in]  in      - First line from input B plane 	
/// @param[Out] yOut    - y plane in the first of the line the output NV21 image
/// @param[out]  uvOut  - uv plane in the first of line from the output NV21 image
/// @param[in]   width  - Line width in pixels
/// @return     Nothing

MVCV_FUNC(void, mvcvCvtColorRGBtoNV21, u8** inR, u8** inG, u8** inB, u8** yOut, u8** uvOut, u32 width, u32 k)
//!@}

#endif //__CVTCOLORKERNELRGBTONV21_H__
