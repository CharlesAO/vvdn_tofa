///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref bitwiseAnd SIPP filter API.
///

#ifndef __SIPP_BITWISEAND_H__
#define __SIPP_BITWISEAND_H__ 

#include <sipp.h>

/// @sf_definition bitwiseAnd Bitwise And
/// @sf_description This filter performs per-element bit-wise logical conjunction(AND) for two input images.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- name: input1 datatypes: UInt8; kernels: 1x1
/// 	- name: input2 datatypes: UInt8; kernels: 1x1
/// @sf_mandatoryinputs 2
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/bitwiseAnd/arch/$(MV_SOC_REV_DIR)/shave/src/bitwiseAnd.asm
/// @{


/// @brief Shave function of the @ref bitwiseAnd filter.
void SVU_SYM(svuBitwiseAnd)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuBitwiseAnd);

/// @}

#endif // __SIPP_BITWISEAND_H__ 
