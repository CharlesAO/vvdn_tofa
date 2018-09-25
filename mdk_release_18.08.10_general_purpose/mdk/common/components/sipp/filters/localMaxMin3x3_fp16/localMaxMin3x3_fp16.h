///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref localMaxMin3x3_fp16 SIPP filter API.
///

#ifndef __SIPP_LOCALMAXMIN3X3FP16_H__
#define __SIPP_LOCALMAXMIN3X3FP16_H__ 

#include <sipp.h>

/// @sf_definition localMaxMin3x3_fp16 localMaxMin3x3_fp16
/// @sf_description /// This kernel will find the points which are minimums or maximums in their 3x3 zone.
/// @sf_group Arithmetic
/// @ingroup SIPP_Software_Filters
/// @sf_preserve imgSize
/// @sf_outdatatype UInt32
/// @sf_inputs
/// 	- datatypes: half; kernels: 3x3
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/localMaxMin3x3_fp16/arch/$(MV_SOC_REV_DIR)/shave/src/localMaxMin3x3_fp16.asm
/// @{

/// @brief Shave function of the @ref localMaxMin3x3_fp16 filter.
void SVU_SYM(svuLocalMaxMin3x3_fp16)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuLocalMaxMin3x3_fp16);

/// @}

#endif // __SIPP_LOCALMAXMIN3X3FP16_H__ 
