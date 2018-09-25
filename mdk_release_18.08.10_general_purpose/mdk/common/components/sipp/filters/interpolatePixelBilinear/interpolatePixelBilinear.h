///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref interpolatePixelBilinear SIPP filter API.
///    

#ifndef __SIPP_INTERPOLATE_PIXEL_BILINEAR_H__
#define __SIPP_INTERPOLATE_PIXEL_BILINEAR_H__ 

#include <sipp.h>

/// @sf_definition interpolatePixelBilinear interpolatePixelBilinear
/// @sf_description  This filter  makes the bilinear interpolation of four pixels
/// @sf_group CV
/// @sf_outdatatype half
/// @sf_inputs
/// 	- datatypes: half; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/interpolatePixelBilinear/arch/$(MV_SOC_REV_DIR)/shave/src/interpolatePixelBilinear.asm
/// @{
	
/// @brief Parameter structure of the @ref interpolatePixelBilinear filter.
typedef struct
{
	float x; //The x coordinate of the pixel
	float y; //The y coordinate of the pixel
}
InterpolatePixelBilinearParam;

/// @brief Shave function of the @ref interpolatePixelBilinear filter.
void SVU_SYM(svuInterpolatePixelBilinear)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuInterpolatePixelBilinear);

/// @}

#endif //__SIPP_INTERPOLATE_PIXEL_BILINEAR_H__
