///
/// @file genChromaSS.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Extract Chroma differences CrCgCb from (debayerd) RGB 
///            image and sub-sample (re-size) the output image by half 
///            in each dimension
///

#ifndef __genChromaSS_MVCV_H__
#define __genChromaSS_MVCV_H__

#include <mv_types.h>
#include <mvcv_macro.h>


#ifdef __PC__
#include "half.h"
#endif



//!@{
/// Generate Chroma from RGB input data and Luma
/// @param[out] outC     - Chroma line output, 3 planes
/// @param[in]  inRGB    - array of pointers to input line of the image, 3 planes, RGB
/// @param[in]  inY      - array of pointers to input line of the image, 1 planes, Luma
/// @param[in]  eps      - epsilon, controls removal of chroma noise
/// @param[in]  normVal  - normalization float factor, depend by number of bytes used 
/// @param[in]  width    - width of the input lines
/// @return    Nothing
MVCV_FUNC(void, mvispGenChromaSS, unsigned char* outC[3], unsigned char** inRGB, int eps, float chromaScale[3], unsigned int width)
//!@}
	
#endif //__genChromaSS_MVCV_H__
