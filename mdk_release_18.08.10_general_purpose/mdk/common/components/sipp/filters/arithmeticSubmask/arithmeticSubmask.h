///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref arithmeticSubmask SIPP filter API.
///

#ifndef __SIPP_ARITHMETICSUBMASK_H__
#define __SIPP_ARITHMETICSUBMASK_H__ 

#include <sipp.h>

/// @sf_definition arithmeticSubmask Arithmetic subtraction with mask
/// @sf_description This filter performs subtraction of two input images depending on the mask value.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- name: input1 datatypes: UInt8; kernels: 1x1
/// 	- name: input2 datatypes: UInt8; kernels: 1x1
/// @sf_mandatoryinputs 2
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/arithmeticSubmask/arch/$(MV_SOC_REV_DIR)/shave/src/arithmeticSubmask.asm
/// @{


/// @brief Shave function of the @ref arithmeticSubmask filter.
void SVU_SYM(svuArithmeticSubmask)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuArithmeticSubmask);

/// @}

#endif // __SIPP_ARITHMETICSUBMASK_H__
