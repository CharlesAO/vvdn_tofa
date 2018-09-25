#ifndef __CVTCOLORKERNELYUV422TORGB_H__
#define __CVTCOLORKERNELYUV422TORGB_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{	
/// This kernel performs color space conversion YUV422 to RGB for one line
/// @param[in] input - YUV422 interleaved line
/// @param[out] rOut - Output line that contain R values
/// @param[out] gOut - Output line that contain G values
/// @param[out] bOut - Output line that contain B values
/// @param[in] width - Line width

MVCV_FUNC(void, mvcvCvtColorKernelYUV422ToRGB, u8** input, u8** rOut, u8** gOut, u8** bOut, u32 width)
//!@}

#endif //__CVTCOLORKERNELYUV422TORGB_H__
