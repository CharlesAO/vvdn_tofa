#ifndef __CVTCOLORKERNELRGBTOYUV422_H__
#define __CVTCOLORKERNELRGBTOYUV422_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs color space conversion RGB to YUV422 for one line
/// @param[in] rIn 		- Input line that contain R values from RGB
/// @param[in] gIn 		- Input line that contain G values from RGB
/// @param[in] bIn 		- Input line that contain B values from RGB
/// @param[out] output  - Output line - YUV422 interleaved
/// @param[in] width 	- Line width
/// @return    Nothing
	
MVCV_FUNC(void, mvcvCvtColorKernelRGBToYUV422, u8** rIn, u8** gIn, u8** bIn, u8** output, u32 width)
//!@}

#endif //__CVTCOLORKERNELRGBTOYUV422_H__
