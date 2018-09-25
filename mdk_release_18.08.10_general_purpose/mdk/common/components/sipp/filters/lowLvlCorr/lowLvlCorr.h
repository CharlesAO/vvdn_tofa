/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref lowLvlCorr SIPP filter API.
///

#ifndef __SIPP_FILTER_LOW_LEVEL_CORRECTIONS_H__
#define __SIPP_FILTER_LOW_LEVEL_CORRECTIONS_H__

#include <sipp.h>

/// @sf_definition lowLvlCorr Low Level Correction
/// @sf_description This filter performs low level pixel value correction on a single plane. Contains black level correction and correct bad pixels.
/// @sf_group ISP
/// @sf_outdatatype UInt16
/// @sf_inputs
/// 	- datatypes: UInt16; kernels: 3x3
/// @{

/// @brief Parameter structure of the @ref lowLvlCorr filter.
typedef struct 
{
	/// @sf_desc black level
    UInt8 blackLevel;
	/// @sf_desc level of correction
    float alphaBadPixel;
}LowLvlCorrParam;

/// @brief Shave function of the @ref lowLvlCorrMultiplePlanes filter.
void SVU_SYM(svuLowLvlCorr)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuLowLvlCorr);

#endif //__SIPP_FILTER_LOW_LEVEL_CORRECTIONS_H__
