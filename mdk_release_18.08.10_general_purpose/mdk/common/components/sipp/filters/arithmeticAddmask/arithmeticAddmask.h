///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref arithmeticAddmask SIPP filter API.
///

#ifndef __SIPP_ARITHMETICADDMASK_H__
#define __SIPP_ARITHMETICADDMASK_H__ 

#include <sipp.h>

/// @sf_definition arithmeticAddmask Arithmetic addition with mask
/// @sf_description This filter performs addition two input images depending on the mask value.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- name: input1 datatypes: UInt8; kernels: 1x1
/// 	- name: input2 datatypes: UInt8; kernels: 1x1
///		- name: input3 datatypes: UInt8; kernels: 1x1
/// @sf_mandatoryinputs 3
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/arithmeticAddmask/arch/$(MV_SOC_REV_DIR)/shave/src/arithmeticAddmask.asm
/// @{


/// @brief Shave function of the @ref arithmeticAddmask filter.
void SVU_SYM(svuArithmeticAddmask)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuArithmeticAddmask);

/// @}

#endif // __SIPP_ARITHMETICADDMASK_H__ 
