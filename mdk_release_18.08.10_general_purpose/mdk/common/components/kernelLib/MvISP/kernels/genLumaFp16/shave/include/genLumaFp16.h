///
/// @file genLumaFp16.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Extract Luma channel from RGB input data
///

#ifndef __genLumaFp16_MVCV_H__
#define __genLumaFp16_MVCV_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// Calculate luma from rgb input lines obtained from debayer
/// @param[out] outY    - Luma line output, fp16 format
/// @param[in]  inR      - array of pointers to input line of the image, 1 planes, R
/// @param[in]  inG      - array of pointers to input line of the image, 1 planes, G
/// @param[in]  inB      - array of pointers to input line of the image, 1 planes, B
/// @param[in]  normVal  - normalization float factor, depend by number of bytes used 
/// @param[in]  width    - width of the input lines
/// @return     Nothing

MVCV_FUNC(void, mvispGenLumaFp16, half *outY, unsigned short* inR, unsigned short *inG, unsigned short *inB, float normVal, int width)
//!@}
	
#endif //__genLumaFp16_MVCV_H__
