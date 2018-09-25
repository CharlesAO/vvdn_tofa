///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref cvtColorRGBtoLuma SIPP filter API.
///

#ifndef __SIPP_CVTCOLORRGBTOLUMA_H__
#define __SIPP_CVTCOLORRGBTOLUMA_H__ 

#include <sipp.h>

/// @sf_definition cvtColorRGBtoLuma RGB to Luma conversion
/// @sf_description This filter performs conversion from RGB image format to Luma.
/// @sf_group ISP
/// @defgroup ISP
/// @ingroup SIPP_Software_Filters
/// @sf_preserve imgSize
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/cvtColorRGBtoLuma/arch/$(MV_SOC_REV_DIR)/shave/src/cvtColorRGBtoLuma.asm
/// @{

/// @brief Shave function of the @ref cvtColorRGBtoLuma filter.
void SVU_SYM(svuCvtColorRGBtoLuma)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuCvtColorRGBtoLuma);

/// @}

#endif // __SIPP_CVTCOLORRGBTOLUMA_H__
