///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref bitwiseOrMask SIPP filter API.
///

#ifndef __SIPP_BITWISEORMASK_H__
#define __SIPP_BITWISEORMASK_H__ 

#include <sipp.h>

/// @sf_definition bitwiseOrMask Bitwise Or with mask
/// @sf_description This filter performs per-element bit-wise logical disjunction(OR) for two input images, depending on the mask value.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- name: input1 datatypes: UInt8; kernels: 1x1
/// 	- name: input2 datatypes: UInt8; kernels: 1x1
/// @sf_mandatoryinputs 2
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/bitwiseOrMask/arch/$(MV_SOC_REV_DIR)/shave/src/bitwiseOrMask.asm
/// @{


/// @brief Shave function of the @ref bitwiseOrMask filter.
void SVU_SYM(svuBitwiseOrMask)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuBitwiseOrMask);

/// @}

#endif // __SIPP_BITWISEORMASK_H__ 
