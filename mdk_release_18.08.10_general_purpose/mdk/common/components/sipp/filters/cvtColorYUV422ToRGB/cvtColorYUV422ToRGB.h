///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref cvtColorYUV422toRGB SIPP filter API.
///

#ifndef __SIPP_CVTCOLORYUV422TORGB_H__
#define __SIPP_CVTCOLORYUV422TORGB_H__

#include <sipp.h>

/// @sf_definition cvtColorYUV422toRGB YUV422 to RGB conversion
/// @sf_description This filter performs color space conversion from YUV422 to RGB.
/// @sf_group ISP
/// @sf_preserve dataType
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/cvtColorKernelYUV422ToRGB/arch/$(MV_SOC_REV_DIR)/shave/src/cvtColorKernelYUV422ToRGB.asm
/// @{

/// @brief Shave function of the @ref cvtColorYUV422ToRGB filter.
void SVU_SYM(svuCvtColorYUV422ToRGB)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuCvtColorYUV422ToRGB);

/// @}

#endif // __SIPP_CVTCOLORYUV422TORGB_H__
