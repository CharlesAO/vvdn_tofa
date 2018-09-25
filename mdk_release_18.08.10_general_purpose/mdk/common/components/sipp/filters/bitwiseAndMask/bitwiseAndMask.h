///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref bitwiseAndMask SIPP filter API.
///

#ifndef __SIPP_BITWISEANDMASK_H__
#define __SIPP_BITWISEANDMASK_H__ 

#include <sipp.h>

/// @sf_definition bitwiseAndMask Bitwise And with mask
/// @sf_description This filter performs per-element bit-wise logical conjunction(AND) for two input images, depending on the mask value.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- name: input1 datatypes: UInt8; kernels: 1x1
/// 	- name: input2 datatypes: UInt8; kernels: 1x1
/// @sf_mandatoryinputs 2
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/bitwiseAndMask/arch/$(MV_SOC_REV_DIR)/shave/src/bitwiseAndMask.asm
/// @{


/// @brief Shave function of the @ref bitwiseAndMask filter.
void SVU_SYM(svubitwiseAndMask)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svubitwiseAndMask);

/// @}

#endif // __SIPP_BITWISEANDMASK_H__ 
