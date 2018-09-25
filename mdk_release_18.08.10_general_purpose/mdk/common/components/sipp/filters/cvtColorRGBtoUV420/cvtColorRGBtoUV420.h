///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref cvtColorRGBtoUV420 SIPP filter API.
///

#ifndef __SIPP_CVTCOLORRGBTOUV420_H__
#define __SIPP_CVTCOLORRGBTOUV420_H__ 

#include <sipp.h>

/// @sf_definition cvtColorRGBtoUV420 RGB to UV420 conversion
/// @sf_description This filter performs color space conversion from RGB to UV420.
/// @sf_group ISP
/// @sf_preserve imgSize, dataType
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/cvtColorRGBtoUV420/arch/$(MV_SOC_REV_DIR)/shave/src/cvtColorRGBtoUV420.asm
/// @{

/// @brief Shave function of the @ref cvtColorRGBtoUV420 filter.
void SVU_SYM(svuCvtColorRGBtoUV420)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuCvtColorRGBtoUV420);

/// @}

#endif // __SIPP_CVTCOLORRGBTOYUV420_H__
