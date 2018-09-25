///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref scale05Lanc7HV SIPP filter API.
///

#ifndef __SIPP_SCALE05LANC7HV_H__
#define __SIPP_SCALE05LANC7HV_H__ 

#include <sipp.h>

/// @sf_definition scale05Lanc7HV Lanczos Downscale by 2 (7 taps)
/// @sf_description This filter applies a lanczos downscale, with factor 0.5, and 7 taps; Horizontal and vertical directions.
/// @sf_group Arithmetic
/// @sf_preserve numPlanes, dataType
/// @sf_flags SIPP_RESIZE
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 7x7
/// @{

/// @brief Shave function of the @ref scale05Lanc7HV filter.
void SVU_SYM(svuScl05Lanc7)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuScl05Lanc7); 

/// @}

#endif // __SIPP_SCALE05LANC7HV_H__ 
