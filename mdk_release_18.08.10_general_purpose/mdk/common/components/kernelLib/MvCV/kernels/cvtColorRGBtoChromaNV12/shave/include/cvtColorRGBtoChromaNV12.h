#ifndef __CVTCOLORKERNELRGBTOCHROMANV12_H__
#define __CVTCOLORKERNELRGBTOCHROMANV12_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{	
/// This kernel performs color space conversion RGB to chromaNV12 for one line in an image
/// @param[in]  inR     - First line from input R plane 
/// @param[in]  inG     - First line from input G plane
/// @param[in]  in      - First line from input B plane 	
/// @param[Out] uvOut   - Pointer to the UV interleaved plane 
/// @param[in]  width   - Line width in pixels
/// @param[in]  offset  - Offset parameters
/// @param[in]  mat     - Conversion matrix
/// @return     Nothing

MVCV_FUNC(void, mvcvCvtColorRGBtoChromaNV12, u8* in1RGB[3], u8* in2RGB[3], u8* uvOut, float mat[3*3], float offset[1*3], u32 width)
//!@}

#endif //__CVTCOLORKERNELRGBTOCHROMANV12_H__
