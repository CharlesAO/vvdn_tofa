/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref lowLvlCorrMultiplePlanes SIPP filter API.
///

#ifndef __SIPP_FILTER_LOW_LEVEL_CORRECTIONS_MULTIPLE_PLANES_H__
#define __SIPP_FILTER_LOW_LEVEL_CORRECTIONS_MULTIPLE_PLANES_H__

#include <sipp.h>

/// @sf_definition lowLvlCorrMultiplePlanes Low Level Correction on Multiple Planes
/// @sf_description This filter performs low level pixel value correction on multiple planes. Contains black level correction and correct bad pixels.
/// @sf_group ISP
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 3x3
/// @{

/// @brief Parameter structure of the @ref lowLvlCorrMultiplePlanes filter.
typedef struct 
{
	/// @sf_desc black level
    UInt8 blackLevel;
	/// @sf_desc level of correction
    float alphaBadPixel;
}LowLvlCorrNPlParam;

/// @brief Shave function of the @ref lowLvlCorrMultiplePlanes filter.
void SVU_SYM(svulowLvlCorrMultiplePlanes)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svulowLvlCorrMultiplePlanes);

/// @}

#endif //__SIPP_FILTER_LOW_LEVEL_CORRECTIONS_MULTIPLE_PLANES_H__
