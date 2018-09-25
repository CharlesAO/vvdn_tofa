#ifndef __CVTCOLORKERNELCHROMAYUV444TONV12_H__
#define __CVTCOLORKERNELCHROMAYUV444TONV12_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs format conversion chromaYUV444 to NV21 for one line in the output image;
/// is must be called every two input lines as the output format NV12 is sub-sampled at 1/2 vertical resolution
/// the width must be multiple of 8 due to asm optimization reasons
/// @param[in]  inU     - 2 lines from input U plane
/// @param[in]  inV     - 2 lines from input V plane
/// @param[Out] outUV   - UV interlaced line in the output UV plane
/// @param[in]  width   - line width in pixels
/// @return     Nothing

MVCV_FUNC(void, mvcvCvtColorKernelChromaYUV444ToNV12, u8* inU[2], u8* inV[2], u8* outUV, u32 width)
//!@}

void mvcvCvtColorKernelChromaYUV444ToNV12_SHV(u8* inU[2], u8* inV[2], u8* outUV, u32 width);

#endif //__CVTCOLORKERNELCHROMAYUV444TONV12_H__
