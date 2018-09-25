///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref cvtColorNV21toRGB SIPP filter API.
///

#ifndef __SIPP_CVTCOLOR_NV21_TO_RGB_H__
#define __SIPP_CVTCOLOR_NV21_TO_RGB_H__ 

#include <sipp.h>

/// @sf_definition cvtColorNV21toRGB NV21 to RGB conversion
/// @sf_description This filter performs color space conversion RGBfp16 to LumaU8 for one line in an image.
/// @sf_group ISP
/// @sf_preserve imgSize, dataType
/// @sf_outdatatype UInt8
/// @sf_mandatoryinputs 2
/// @sf_inputs
/// 	- name: yin;  datatypes: UInt8; kernels: 1x1
/// 	- name: uvin; datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/cvtColorNV21toRGB/arch/$(MV_SOC_REV_DIR)/shave/src/cvtColorNV21toRGB.asm
/// @{

/// @brief Shave function of the @ref cvtColorNV21toRGB filter.
void SVU_SYM(svucvtColorNV21toRGB)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svucvtColorNV21toRGB);

/// @}

#endif //__SIPP_CVTCOLOR_NV21_TO_RGB_H__
