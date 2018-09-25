///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref sad11x11 SIPP filter API.
///    

#ifndef __SIPP_SAD11x11_H__
#define __SIPP_SAD11x11_H__ 

#include <sipp.h>

/// @sf_definition sad11x11 Sum of Absolute Differences 11x11
/// @sf_description  The filter computes the sum of absolute differences between two input images on a 11x11 kernel.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- name: input1; datatypes: UInt8; kernels: 11x11
/// 	- name: input2; datatypes: UInt8; kernels: 11x11
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/sumOfAbsDiff11x11/arch/$(MV_SOC_REV_DIR)/shave/src/sumOfAbsDiff11x11.asm
/// @{

/// @brief Shave function of the @ref sad11x11 filter.
void SVU_SYM(svuSAD11x11)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuSAD11x11);

/// @}

#endif //__SIPP_SAD11x11_H__
