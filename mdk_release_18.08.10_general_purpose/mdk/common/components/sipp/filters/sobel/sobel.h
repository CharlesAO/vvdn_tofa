///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref sobel SIPP filter API.
///

#ifndef __SIPP_SOBEL_H__
#define __SIPP_SOBEL_H__ 

#include <sipp.h>

/// @sf_definition sobel Sobel
/// @sf_description This filter performs sobel edge detection operator.
/// @sf_group CV
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 3x3
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/sobel/arch/$(MV_SOC_REV_DIR)/shave/src/sobel.asm
/// @{

/// @brief Shave function of the @ref sobel filter.
void SVU_SYM(svuSobel)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuSobel);

/// @}

#endif // __SIPP_SOBEL_H__ 
