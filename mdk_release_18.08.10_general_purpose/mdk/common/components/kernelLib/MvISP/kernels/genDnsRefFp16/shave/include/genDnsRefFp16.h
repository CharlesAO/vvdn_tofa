///
/// @file genDnsRefFp16.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Generate Luma denoise reference plane as input for LumaDenoise kernel. 
///            The reference plane is used for calculating weights to be assigned to 
///            pixels in the neighbourhood.
///

#ifndef __genDnsRefFp16_MVCV_H__
#define __genDnsRefFp16_MVCV_H__

#include <mv_types.h>
#include <mvcv_macro.h>


typedef struct
{
    unsigned char   *lutGamma;
    unsigned char   *lutDist;
    int      shift;
}
YDnsRefFp16Param;

//!@{
/// Generate denoise reference input image
/// @param[out] outRef  - output reference line
/// @param[in]  inY     - array of pointers to input line of the image, 1 planes, Luma
/// @param[in]  yc      - vertical position
/// @param[in]  x0      - horizontal position
/// @param[in]  param   - effect specific parameters
/// @param[in]  width   - width of the input lines
/// @return     Nothing

MVCV_FUNC(void, mvispGenDnsRefFp16, half *inY, unsigned int yc, int x0,  YDnsRefFp16Param* param, unsigned char* outRef, unsigned int width)
//!@}
	
#endif //__genDnsRefFp16_MVCV_H__
