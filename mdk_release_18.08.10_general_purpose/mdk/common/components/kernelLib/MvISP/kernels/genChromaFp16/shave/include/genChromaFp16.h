///
/// @file genChromaFp16.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Extract Chroma differences CrCgCb from (debayerd) RGB image
///

#ifndef __genChromaFp16_MVCV_H__
#define __genChromaFp16_MVCV_H__

#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// Generate Chroma from RGB input data and Luma
/// @param[out] outY     - Chroma output line, 3 planes, fp16 format
/// @param[in]  inRGB    - array of pointers to input line of the image, 3 planes, RGB
/// @param[in]  inY      - array of pointers to input line of the image, 1 planes, Luma
/// @param[in]  eps      - epsilon, controls removal of chroma noise
/// @param[in]  normVal  - normalization float factor, depend by number of bytes used 
/// @param[in]  width    - width of the input lines
/// @return     Nothing

MVCV_FUNC(void, mvispGenChromaFp16, unsigned char* outC[3], unsigned short* inRGB[3], half *inY, float eps, float normVal, unsigned int width)
//!@}
	
#endif //__genChromaFp16_MVCV_H__
