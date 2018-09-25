///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref bitwiseNot SIPP filter API.
///

#ifndef __SIPP_BITWISENOT_H__
#define __SIPP_BITWISENOT_H__ 

#include <sipp.h>

/// @sf_definition bitwiseNot Bitwise Not
/// @sf_description This filter performs per-element bit-wise NOT.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/bitwiseNot/arch/$(MV_SOC_REV_DIR)/shave/src/bitwiseNot.asm
/// @{


/// @brief Shave function of the @ref bitwiseNot filter.
void SVU_SYM(svuBitwiseNot)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuBitwiseNot);

/// @}

#endif // __SIPP_BITWISENOT_H__ 
