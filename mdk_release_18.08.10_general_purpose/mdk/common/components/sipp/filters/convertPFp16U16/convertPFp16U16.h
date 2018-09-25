///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref convertPFp16U16 SIPP filter API.
///

#ifndef __SIPP_CONVERTPFP16U16_H__
#define __SIPP_CONVERTPFP16U16_H__ 

#include <sipp.h>

/// @sf_definition convertPFp16U16 Convert Fp16 to U16
/// @sf_description This filter performs FP16 to U16 conversion.
/// @sf_group Arithmetic
/// @sf_preserve numPlanes, imgSize
/// @sf_outdatatype UInt16
/// @sf_inputs
/// 	- datatypes: half; kernels: 1x1
/// @{

/// @brief Shave function of the @ref convertPFp16U16 filter.
void SVU_SYM(svuConvertPFp16U16)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConvertPFp16U16);

/// @}

#endif // __SIPP_CONVERTPFP16U16_H__
