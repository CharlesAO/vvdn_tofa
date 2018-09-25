#ifndef __CVTCOLORKERNELCHROMAYUV420TONV12_H__
#define __CVTCOLORKERNELCHROMAYUV420TONV12_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs format conversion chromaYUV420 to NV21 for one line in an image;
/// naturally is must be called for even lines only as the output format NV12 is sub-sampled at 1/2 vertical resolution
/// the width must be multiple of 8 due to asm optimization reasons
/// @param[in]  inU     - line from input U plane
/// @param[in]  inV     - line from input V plane
/// @param[Out] outUV   - UV plane in the first of the line the output NV12 image
/// @param[in]  width   - Line width in pixels
/// @return     Nothing

MVCV_FUNC(void, mvcvCvtColorKernelChromaYUV420ToNV12, u8* inU, u8* inV, u8* outUV, u32 width)
//!@}

#endif //__CVTCOLORKERNELCHROMAYUV420TONV12_H__
