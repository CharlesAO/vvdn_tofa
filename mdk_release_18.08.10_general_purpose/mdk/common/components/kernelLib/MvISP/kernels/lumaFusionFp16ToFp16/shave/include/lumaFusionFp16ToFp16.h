///
/// @file LumaFusionFp16ToFp16
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     
///

#ifndef __lumaFusionFp16ToFp16_H__
#define __lumaFusionFp16ToFp16_H__
#include <mv_types.h>
#include <mvcv_macro.h>

typedef struct
{
    float expRat;
    float eps;
    float lslope1;
    float lslope2;
}
LumaFusionParam;

//!@{	
/// mvispLumaFusionFp16ToFp16 kernel  
/// @param[in]  inLumaShort 	    - short exposure of the image 
/// @param[in]  inLumaLong   	    - long exposure of the image 
/// @param[Out] outputAlpha  	    - to avoid motion blur, take the short exposure, except when the local area (measured in the short exposure) is dark
/// @param[Out] outputLumaFusion  	- short exposure is boosted in accordance with the exposure ratio. Short and long exposures are adaptively blended
/// @param[in] params  	            - expRat  - exposure ratio;  eps - epsilon; lslope1/lslope2 - use a programmable slope to control transition from dark to bright. 
/// @param[in]  width               - width in pixels
/// @return     Nothing

MVCV_FUNC(void, mvispLumaFusionFp16ToFp16, half* inLumaShort, half* inLumaLong, half* outputAlpha, half* outputLumaFusion, u32 width, LumaFusionParam *params) 
//!@}

#endif //__gtmFp16ToFp16_H__
