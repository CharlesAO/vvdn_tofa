///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref arithmeticSub SIPP filter API.
///

#ifndef __SIPP_ARITHMETICSUB_H__
#define __SIPP_ARITHMETICSUB_H__ 

#include <sipp.h>

/// @sf_definition arithmeticSub Arithmetic subtraction
/// @sf_description This filter performs subtraction two input images.
/// @sf_group Arithmetic
/// @defgroup Arithmetic
/// @ingroup SIPP_Software_Filters
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- name: input1 datatypes: UInt8; kernels: 1x1
/// 	- name: input2 datatypes: UInt8; kernels: 1x1
/// @sf_mandatoryinputs 2
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/arithmeticSub/arch/$(MV_SOC_REV_DIR)/shave/src/arithmeticSub.asm
/// @{

/// @brief Shave function of the @ref arithmeticSub filter.
void SVU_SYM(svuArithmeticSub)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuArithmeticSub);

/// @}

#endif // __SIPP_ARITHMETICSUB_H__ 

