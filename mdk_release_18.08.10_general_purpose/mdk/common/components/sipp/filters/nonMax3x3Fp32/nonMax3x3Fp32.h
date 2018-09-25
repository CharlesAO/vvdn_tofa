///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref nonMax3x3Fp32 SIPP filter API.
///

#ifndef __SIPP_NONMAX3x3FP32_H__
#define __SIPP_NONMAX3x3FP32_H__ 

#include <sipp.h>

/// @sf_definition nonMax3x3Fp32 nonMaxFp32
/// @sf_description This filter verifies whether each element from the central line is the maximum within a 3x3 range
/// @sf_group Arithmetic
/// @ingroup SIPP_Software_Filters
/// @sf_preserve imgSize
/// @sf_outdatatype UInt16
/// @sf_inputs
/// 	- datatypes: float; kernels: 3x3
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/nonMax3x3_fp32/arch/$(MV_SOC_REV_DIR)/shave/src/nonMax3x3_fp32.asm
/// @{

typedef struct
{
   UInt16* candPos;
}
nonMax3x3Fp32Param;

/// @brief Shave function of the @ref cvtColorRGBtoLuma filter.
void SVU_SYM(svuNonMax3x3Fp32)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuNonMax3x3Fp32);

/// @}

#endif // __SIPP_NONMAX3x3FP32_H__ 
