/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref ssd7x7U8ToU32 SIPP filter API.
///

#ifndef __SIPP_SSD7x7U8TOU32_H__
#define __SIPP_SSD7x7U8TOU32_H__ 

#include <sipp.h>

/// @sf_definition ssd7x7U8ToU32 Sum of Squared Differences 7x7 (U8 to U32)
/// @sf_description This filter performs sum of squared differences (SSD) to a U32 output array, the differences are squared and aggregated within a square window (7x7 in this case).
/// @sf_group Arithmetic
/// @sf_preserve numPlanes, imgSize
/// @sf_outdatatype UInt32
/// @sf_inputs
/// 	- name: input1; datatypes: UInt8; kernels: 7x7
/// 	- name: input2; datatypes: UInt8; kernels: 7x7
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/sumOfSquaredDiff7x7U8ToU32/arch/$(MV_SOC_REV_DIR)/shave/src/sumOfSquaredDiff7x7U8ToU32.asm
/// @{

/// @brief Shave function of the @ref ssd7x7U8ToU32 filter.
void SVU_SYM(svuSSD7x7U8ToU32)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuSSD7x7U8ToU32);

/// @}

#endif // __SIPP_SSD7x7U8TOU32_H__ 
