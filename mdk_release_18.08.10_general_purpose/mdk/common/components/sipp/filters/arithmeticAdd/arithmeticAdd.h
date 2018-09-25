///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref arithmeticAdd SIPP filter API.
///

#ifndef __SIPP_ARITHMETICADD_H__
#define __SIPP_ARITHMETICADD_H__ 

#include <sipp.h>

/// @sf_definition arithmeticAdd Arithmetic addition
/// @sf_description This filter performs addition two input images.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- name: input1; datatypes: UInt8; kernels: 1x1
/// 	- name: input2; datatypes: UInt8; kernels: 1x1
/// @sf_mandatoryinputs 2
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/arithmeticAdd/arch/$(MV_SOC_REV_DIR)/shave/src/arithmeticAdd.asm
/// @{

/// @brief Shave function of the @ref arithmeticAdd filter.
void SVU_SYM(svuArithmeticAdd)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuArithmeticAdd);

/// @}

#endif // __SIPP_ARITHMETICADD_H__ 
