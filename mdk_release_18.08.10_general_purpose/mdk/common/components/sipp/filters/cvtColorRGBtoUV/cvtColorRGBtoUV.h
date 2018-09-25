///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref cvtColorRGBtoUV SIPP filter API.
///

#ifndef __SIPP_CVTCOLORRGBTOUV_H__
#define __SIPP_CVTCOLORRGBTOUV_H__ 

#include <sipp.h>

/// @sf_definition cvtColorRGBtoUV RGB to UV conversion
/// @sf_description This filter performs color space conversion from RGB to UV.
/// @sf_group ISP
/// @sf_preserve imgSize, dataType
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/cvtColorRGBtoUV/arch/$(MV_SOC_REV_DIR)/shave/src/cvtColorRGBtoUV.asm
/// @{

/// @brief Shave function of the @ref cvtColorRGBtoUV filter.
void SVU_SYM(svuCvtColorRGBtoUV)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuCvtColorRGBtoUV);

/// @}

#endif // __SIPP_CVTCOLORRGBTOYUV_H__
