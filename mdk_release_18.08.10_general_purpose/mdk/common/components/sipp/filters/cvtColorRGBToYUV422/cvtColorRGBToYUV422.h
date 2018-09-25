///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref cvtColorRGBToYUV422 SIPP filter API.
///

#ifndef __SIPP_CVTCOLORRGBTOYUV422_H__
#define __SIPP_CVTCOLORRGBTOYUV422_H__ 

#include <sipp.h>

/// @sf_definition cvtColorRGBToYUV422 RGB to YUV422 conversion
/// @sf_description This filter performs color space conversion from RGB to YUV422.
/// @sf_group ISP
/// @sf_preserve dataType
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/cvtColorKernelRGBToYUV422/arch/$(MV_SOC_REV_DIR)/shave/src/cvtColorKernelRGBToYUV422.asm
/// @{

/// @brief Shave function of the @ref cvtColorRGBToYUV422 filter.
void SVU_SYM(svuCvtColorRGBToYUV422)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuCvtColorRGBToYUV422);

/// @}

#endif // __SIPP_CVTCOLORRGBTOYUV422_H__
