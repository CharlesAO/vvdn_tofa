///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref convertU8ToF16 SIPP filter API.
///

#ifndef __SIPP_CONVERTU8TOF16_H__
#define __SIPP_CONVERTU8TOF16_H__

#include <sipp.h>

/// @sf_definition convertU8ToF16 Convert U8 To F16
/// @sf_description This filter performs a conversion from U8 to F16
/// @sf_group Arithmetic
/// @sf_preserve numPlanes, imgSize
/// @sf_outdatatype half
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convert_u8_fp16/arch/$(MV_SOC_REV_DIR)/shave/src/convert_u8_fp16.asm
/// @{

/// @brief Shave function of the @ref convertU8ToF16 filter.
void SVU_SYM(svuConvertU8ToF16)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConvertU8ToF16);

/// @}

#endif // __SIPP_CONVERTU8TOF16_H__
