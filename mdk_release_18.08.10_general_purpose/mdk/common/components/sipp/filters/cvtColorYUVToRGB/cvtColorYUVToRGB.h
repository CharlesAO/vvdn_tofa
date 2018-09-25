///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref cvtColorYUVtoRGB SIPP filter API.
///

#ifndef __SIPP_CVTCOLORYUVTORGB_H__
#define __SIPP_CVTCOLORYUVTORGB_H__

#include <sipp.h>

/// @sf_definition cvtColorYUVtoRGB YUV to RGB conversion
/// @sf_description This filter performs color space conversion from YUV to RGB.
/// @sf_group ISP
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/cvtColorKernelYUVtoRGB/arch/$(MV_SOC_REV_DIR)/shave/src/cvtColorKernelYUVtoRGB.asm
/// @{

/// @brief Shave function of the @ref cvtColorYUVtoRGB filter.
void SVU_SYM(svuCvtColorYUVToRGB)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuCvtColorYUVToRGB);

/// @}

#endif // __SIPP_CVTCOLORYUVTORGB_H__
