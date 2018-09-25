///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref scale2xLancV SIPP filter API.
///

#ifndef __SIPP_SCALE2xLANCV_H__
#define __SIPP_SCALE2xLANCV_H__ 

#include <sipp.h>

/// @sf_definition scale2xLancV Lanczos Vertical Upscale by 2
/// @sf_description This filter applies a lanczos upscale, with factor 2; Vertical direction.
/// @sf_group Arithmetic
/// @sf_preserve numPlanes, dataType
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 4x1
/// @{

/// @brief Shave function of the @ref scale2xLancV filter.
void SVU_SYM(svuScl2xLancV)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuScl2xLancV); 

/// @}

#endif // __SIPP_SCALE2xLANCV_H__ 
