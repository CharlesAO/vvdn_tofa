///
/// @file genChromaFp16.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
///

#ifndef __hdrLtmFp16ToFp16_MVCV_H__
#define __hdrLtmFp16ToFp16_MVCV_H__

#include <mv_types.h>
#include <mvcv_macro.h>


//!@{ mvispHdrLtmFp16ToFp16  
/// 
/// @param[out] output          - the output of the local tone mapping
/// @param[in]  inLumaFusion    - Short and Long exposures are adaptively blended
/// @param[in]  inGaussUp       - the LumaFusion after downscale, convolution and upscale
/// @param[in]  params          - thr, thr2, thr3 – tone map threholds
/// @param[in]  width           - width of the input lines
/// @return     Nothing


typedef struct
{
    float thr;
    float thr2;
    float thr3;
    float eps;
}
HdrLtmParam;

MVCV_FUNC(void, mvispHdrLtmFp16ToFp16, half* inLumaFusion, half* inGaussUp, half* output, u32 width, HdrLtmParam* params)
//!@}
	
#endif //__hdrLtmFp16ToFp16_MVCV_H__
