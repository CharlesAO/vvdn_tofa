#ifndef __CVTCOLORKERNELYUVTORGB_H__
#define __CVTCOLORKERNELYUVTORGB_H__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs color space conversion: YUV420p to RGB 
/// @param[in] yIn 	 - Input Y channel
/// @param[in] uIn	 - Input U channel
/// @param[in] vIn	 - Input V channel
/// @param[out] out	 - Output RGB channel
/// @param[in] width - Image width in pixels
   
MVCV_FUNC(void, mvcvCvtColorKernelYUVToRGB, u8* yIn, u8* uIn, u8* vIn, u8* out, u32 width)
//!@}

#endif //__CVTCOLORKERNELYUVTORGB_H__