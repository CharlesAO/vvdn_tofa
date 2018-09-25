///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref scale2xLancHV SIPP filter API.
///

#ifndef __SIPP_SCALE2xLANCHV_H__
#define __SIPP_SCALE2xLANCHV_H__ 

#include <sipp.h>

/// @sf_definition scale2xLancHV Lanczos Upscale by 2
/// @sf_description This filter applies a lanczos upscale, with factor 2; Horizontal and Vertical direction.
/// @sf_group Arithmetic
/// @sf_preserve numPlanes, dataType
/// @sf_flags SIPP_RESIZE
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 4x4
/// @{

/// @brief Shave function of the @ref scale2xLancHV filter.
void SVU_SYM(svuScl2xLancHV)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuScl2xLancHV); 

/// @}

#endif // __SIPP_SCALE2xLANCHV_H__ 
