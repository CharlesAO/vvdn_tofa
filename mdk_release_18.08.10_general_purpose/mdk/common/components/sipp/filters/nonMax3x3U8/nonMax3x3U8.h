///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref nonMax3x3U8 SIPP filter API.
///

#ifndef __SIPP_NONMAX3X3U8_H__
#define __SIPP_NONMAX3X3U8_H__ 

#include <sipp.h>

/// @sf_definition nonMax3x3U8 nonMax3x3U8 
/// @sf_description This filter verifies whether each element from the central line is the maximum within a 3x3 range
/// @sf_group Arithmetic
/// @ingroup SIPP_Software_Filters
/// @sf_preserve imgSize
/// @sf_outdatatype UInt16
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 3x3
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/nonMax3x3_u8/arch/$(MV_SOC_REV_DIR)/shave/src/nonMax3x3_u8.asm
/// @{

/// @brief Shave function of the @ref nonMax3x3U8 filter.
void SVU_SYM(svuNonMax3x3U8)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuNonMax3x3U8);

/// @}

#endif // __SIPP_NONMAX3X3U8_H__ 
