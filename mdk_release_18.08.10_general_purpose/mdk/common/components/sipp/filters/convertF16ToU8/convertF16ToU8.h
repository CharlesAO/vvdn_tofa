///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref convertF16ToU8 SIPP filter API.
///

#ifndef __SIPP_CONVERTF16TOU8_H__
#define __SIPP_CONVERTF16TOU8_H__

#include <sipp.h>

/// @sf_definition convertF16ToU8 Convert F16 To U8
/// @sf_description This filter performs F16 to U8 conversion.
/// @sf_group Arithmetic
/// @sf_preserve numPlanes, imgSize
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: half; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convert_fp16_u8/arch/$(MV_SOC_REV_DIR)/shave/src/convert_fp16_u8.asm
/// @{

/// @brief Shave function of the @ref convertF16ToU8 filter.
void SVU_SYM(svuConvertF16ToU8)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConvertF16ToU8);

/// @}

#endif // __SIPP_CONVERTF16TOU8_H__
