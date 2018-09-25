///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref arithmeticSubFp16ToFp16 SIPP filter API.
///

#ifndef __SIPP_ARITHMETICSUBFP16TOFP16_H__
#define __SIPP_ARITHMETICSUBFP16TOFP16_H__ 

#include <sipp.h>

/// @sf_definition arithmeticSubFp16ToFp16 Arithmetic subtraction fp16
/// @sf_description This filter performs subtraction of two fp16 input images.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt16
/// @sf_inputs
/// 	- name: input1 datatypes: UInt16; kernels: 1x1
/// 	- name: input2 datatypes: UInt16; kernels: 1x1
/// @sf_mandatoryinputs 2
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/arithmeticSubFp16ToFp16/arch/$(MV_SOC_REV_DIR)/shave/src/arithmeticSubFp16ToFp16.asm
/// @{


/// @brief Shave function of the @ref arithmeticSubFp16ToFp16 filter.
void SVU_SYM(svuArithmeticSubFp16ToFp16)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuArithmeticSubFp16ToFp16);

/// @}

#endif // __SIPP_ARITHMETICSUBFP16TOFP16_H__ 
