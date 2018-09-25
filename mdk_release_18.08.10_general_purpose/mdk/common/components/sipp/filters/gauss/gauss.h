///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref gauss SIPP filter API.
///

#ifndef __SIPP_GAUSS_H__
#define __SIPP_GAUSS_H__ 

#include <sipp.h>

/// @sf_definition gauss Gauss Blur
/// @sf_description This filter applies gaussian blur on Luma channel.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
///		- datatypes: UInt8; kernels: 5x5
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/gauss/arch/$(MV_SOC_REV_DIR)/shave/src/gauss.asm
/// @{

/// @brief Shave function of the @ref gauss filter.
void SVU_SYM(svuGauss)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuGauss);

/// @}

#endif // __SIPP_GAUSS_H__ 
