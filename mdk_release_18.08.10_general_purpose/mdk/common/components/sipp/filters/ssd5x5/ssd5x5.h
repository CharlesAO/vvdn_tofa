/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref ssd5x5 SIPP filter API.
///

#ifndef __SIPP_SSD5x5_H__
#define __SIPP_SSD5x5_H__ 

#include <sipp.h>

/// @sf_definition ssd5x5 Sum of Squared Differences 5x5
/// @sf_description This filter performs sum of squared differences (SSD), the differences are squared and aggregated within a square window (5x5 in this case).
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- name: input1; datatypes: UInt8; kernels: 5x5
/// 	- name: input2; datatypes: UInt8; kernels: 5x5
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/sumOfSquaredDiff5x5/arch/$(MV_SOC_REV_DIR)/shave/src/sumOfSquaredDiff5x5.asm
/// @{

/// @brief Shave function of the @ref ssd5x5 filter.
void SVU_SYM(svuSSD5x5)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuSSD5x5);

/// @}

#endif // __SIPP_SSD5x5_H__ 
