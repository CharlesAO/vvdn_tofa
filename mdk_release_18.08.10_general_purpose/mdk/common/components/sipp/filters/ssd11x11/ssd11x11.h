/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref ssd11x11 SIPP filter API.
///

#ifndef __SIPP_SSD11x11_H__
#define __SIPP_SSD11x11_H__ 

#include <sipp.h>

/// @sf_definition ssd11x11 Sum of Squared Differences 11x11
/// @sf_description This filter performs sum of squared differences (SSD), the differences are squared and aggregated within a square window (11x11 in this case).
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- name: input1; datatypes: UInt8; kernels: 11x11
/// 	- name: input2; datatypes: UInt8; kernels: 11x11
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/sumOfSquaredDiff11x11/arch/$(MV_SOC_REV_DIR)/shave/src/sumOfSquaredDiff11x11.asm
/// @{

/// @brief Shave function of the @ref ssd11x11 filter.
void SVU_SYM(svuSSD11x11)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuSSD11x11);

/// @}

#endif // __SIPP_SSD11x11_H__ 
