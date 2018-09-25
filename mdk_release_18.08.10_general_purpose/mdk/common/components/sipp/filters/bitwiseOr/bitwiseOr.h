///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref bitwiseOr SIPP filter API.
///

#ifndef __SIPP_BITWISEOR_H__
#define __SIPP_BITWISEOR_H__ 

#include <sipp.h>

/// @sf_definition bitwiseOr Bitwise Or
/// @sf_description This filter performs per-element bit-wise logical disjunction(OR) for two input images.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- name: input1 datatypes: UInt8; kernels: 1x1
/// 	- name: input2 datatypes: UInt8; kernels: 1x1
/// @sf_mandatoryinputs 2
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/bitwiseOr/arch/$(MV_SOC_REV_DIR)/shave/src/bitwiseOr.asm
/// @{


/// @brief Shave function of the @ref bitwiseOr filter.
void SVU_SYM(svuBitwiseOr)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuBitwiseOr);

/// @}

#endif // __SIPP_BITWISEOR_H__ 
