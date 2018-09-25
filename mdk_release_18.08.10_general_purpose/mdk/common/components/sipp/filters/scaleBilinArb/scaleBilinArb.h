///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref scaleBilinArb SIPP filter API.
///

#ifndef __SIPP_SCALE05BILINHV_H__
#define __SIPP_SCALE05BILINHV_H__ 

#include <sipp.h>

/// @sf_definition scaleBilinArb Arbitrary Downscale 
/// @sf_description This filter performs bilinear scale, arbitrary X and Y scale factors.
/// @sf_group Arithmetic
/// @sf_preserve numPlanes, dataType
/// @sf_flags SIPP_RESIZE
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 2x2
/// @{

/// @brief Shave function of the @ref scaleBilinArb filter.
void SVU_SYM(svuSclBilinArb)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuSclBilinArb);

/// @}

#endif // __SIPP_SCALE05BILINHV_H__ 
