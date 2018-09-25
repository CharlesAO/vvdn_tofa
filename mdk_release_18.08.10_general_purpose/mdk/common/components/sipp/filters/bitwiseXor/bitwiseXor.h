///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref bitwiseXor SIPP filter API.
///

#ifndef __SIPP_BITWISEXOR_H__
#define __SIPP_BITWISEXOR_H__ 

#include <sipp.h>

/// @sf_definition bitwiseXor Bitwise Xor
/// @sf_description This filter performs per-element bit-wise logical conjunction(XOR) for two input images.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- name: input1 datatypes: UInt8; kernels: 1x1
/// 	- name: input2 datatypes: UInt8; kernels: 1x1
/// @sf_mandatoryinputs 2
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/bitwiseXor/arch/$(MV_SOC_REV_DIR)/shave/src/bitwiseXor.asm
/// @{


/// @brief Shave function of the @ref bitwiseXor filter.
void SVU_SYM(svuBitwiseXor)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuBitwiseXor);

/// @}

#endif // __SIPP_BITWISEXOR_H__ 
