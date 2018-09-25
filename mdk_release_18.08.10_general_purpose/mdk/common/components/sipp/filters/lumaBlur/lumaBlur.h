///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref lumaBlur SIPP filter API.
///

#ifndef __SIPP_LUMA_BLUR_H__
#define __SIPP_LUMA_BLUR_H__ 

#include <sipp.h>

/// @sf_definition lumaBlur Luma Blur
/// @sf_description This filter applies the blur operator on Luma channel.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
///		- datatypes: UInt8; kernels: 3x3
/// @{

/// @brief Shave function of the @ref lumaBlur filter.
void SVU_SYM(svuLumaBlur)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuLumaBlur);

/// @}

#endif // __SIPP_LUMA_BLUR_H__ 
