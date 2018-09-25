///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref sad5x5 SIPP filter API.
///    

#ifndef __SIPP_SAD5x5_H__
#define __SIPP_SAD5x5_H__ 

#include <sipp.h>

/// @sf_definition sad5x5 Sum of Absolute Differences 5x5
/// @sf_description  The filter computes the sum of absolute differences between two input images on a 5x5 kernel.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- name: input1; datatypes: UInt8; kernels: 5x5
/// 	- name: input2; datatypes: UInt8; kernels: 5x5
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/sumOfAbsDiff5x5/arch/$(MV_SOC_REV_DIR)/shave/src/sumOfAbsDiff5x5.asm
/// @{

/// @brief Shave function of the @ref sad5x5 filter.
void SVU_SYM(svuSAD5x5)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuSAD5x5);

/// @}

#endif //__SIPP_SAD5x5_H__
