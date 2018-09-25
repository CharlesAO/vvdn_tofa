///
/// @file genChromaRGB16.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Extract Chroma differences CrCgCb from (debayerd) RGB image
///

#ifndef __GENCHROMARGB16_H__
#define __GENCHROMARGB16_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// genChromaRGB16 kernel  -this kernel generate chroma from RGB planar input and Luma input
/// @param[in]  inRGB             - array of pointers to input lines, 16bit input values   
/// @param[out] outC              - array of pointers for output lines; 3 planes; 8 bits per pixel 
/// @param[in]  inY               - pointer to Y input   
/// @param[in]  eps               - epsilon, controls removal of chroma noise
/// @param[in]  nBits             - number of bits
/// @param[in]  width             - width of input line

MVCV_FUNC(void, mvispGenChromaRGB16, u16** inRGB, u8 *inY, u8** outC, int width, int eps, int nBits, float *chromaScale)
//!@}

#endif //__GENCHROMARGB16_H__