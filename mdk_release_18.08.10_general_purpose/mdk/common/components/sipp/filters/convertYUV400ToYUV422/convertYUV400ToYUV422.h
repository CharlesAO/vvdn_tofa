///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref ConvertYUV400ToYUV422 SIPP filter API.
///

#ifndef __SIPP_ConvertYUV400ToYUV422__
#define __SIPP_ConvertYUV400ToYUV422__

#include <sipp.h>
/// @sf_definition ConvertYUV400ToYUV422 YUV400 to YUV422 conversion
/// @sf_description This filter performs a color conversion from a simple yuv400 to yuv422 with value for chromas 0x80
/// @sf_group ISP
/// @sf_outdatatype UInt16
/// @sf_preserve
/// @sf_inputs
/// 	- datatypes: UInt8; kernels:1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convertYUV400ToYUV422/arch/$(MV_SOC_REV_DIR)/shave/src/convertYUV400ToYUV422.asm
/// @{

/// @brief Shave function of the @ref ConvertYUV400ToYUV422 filter.
void SVU_SYM(svuConvertYUV400ToYUV422)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConvertYUV400ToYUV422);

/// @}

#endif // __SIPP_ConvertYUV400ToYUV422__

