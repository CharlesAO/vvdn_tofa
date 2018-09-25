/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref harrisResponse SIPP filter API.
///

#ifndef __SIPP_HARRIS_RESPONSE_H__
#define __SIPP_HARRIS_RESPONSE_H__

#include <sipp.h>

/// @sf_definition harrisResponse harrisResponse
/// @sf_description This filter computes Harris response over a patch of the image with a radius of 3
/// @sf_group CV
/// @sf_preserve numPlanes, imgSize
/// @sf_outdatatype float
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 8x8
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/harrisResponse/arch/$(MV_SOC_REV_DIR)/shave/src/harrisResponse.asm
/// @{

/// @brief Parameter structure of the @ref harrisResponse filter.
typedef struct
{
	float k;
}
HarrisSwParam;

/// @brief Shave function of the @ref harrisResponse filter
void SVU_SYM(svuHarrisResponse)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuHarrisResponse);

/// @}

#endif // __SIPP_HARRIS_RESPONSE_H__
