//
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref convertPU16Fp16 SIPP filter API.
///

#ifndef __SIPP_CONVERTPU16FP16_H__
#define __SIPP_CONVERTPU16FP16_H__ 

#include <sipp.h>

/// @sf_definition convertPU16Fp16 Convert U16 to Fp16
/// @sf_description This filter performs U16 to Fp16 conversion.
/// @sf_group Arithmetic
/// @sf_preserve numPlanes, imgSize
/// @sf_outdatatype half
/// @sf_inputs
/// 	- datatypes: UInt16; kernels: 1x1
/// @{

/// @brief Shave function of the @ref convertPU16Fp16 filter.
void SVU_SYM(svuConvertPU16Fp16)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConvertPU16Fp16);

/// @}

#endif // __SIPP_CONVERTPU16FP16_H__
