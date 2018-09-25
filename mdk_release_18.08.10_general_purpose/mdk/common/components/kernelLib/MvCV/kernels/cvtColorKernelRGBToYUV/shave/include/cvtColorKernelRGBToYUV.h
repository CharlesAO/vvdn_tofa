#ifndef __CVTCOLORKERNELRGBTOYUV_H__
#define __CVTCOLORKERNELRGBTOYUV_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs color space conversion: RGB to YUV420p 
/// @param[in] in 		- Input RGB channel
/// @param[out] yOut 	- Output Y channel
/// @param[out] uOut 	- Output U channel
/// @param[out] vOut 	- Output V channel
/// @param[in] width 	- Image width in pixels

MVCV_FUNC(void, mvcvCvtColorKernelRGBToYUV, u8* in, u8* yOut, u8* uOut, u8* vOut, u32 width)
//!@}

#endif //__CVTCOLORKERNELRGBTOYUV_H__