///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref genLuma SIPP filter API.
///

#ifndef __SIPP_GEN_LUMA_H__
#define __SIPP_GEN_LUMA_H__ 

#include <sipp.h>

/// @sf_definition genLuma Luma Blur
/// @sf_description This filter generates Luminance from RGB input.
/// @sf_group ISP
/// @sf_outdatatype UInt8
/// @sf_preserve imgSize, dataType
/// @sf_inputs
///		- datatypes: UInt8; kernels: 1x0, 1x1
/// @{

/// @brief Shave function of the @ref genLuma filter.
void SVU_SYM(svuGenLuma)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuGenLuma);

/// @}

#endif //__SIPP_GEN_LUMA_H__
