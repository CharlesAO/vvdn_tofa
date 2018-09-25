///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref meanStdDev SIPP filter API.
///

#ifndef __SIPP_MEANSTDDEV_H__
#define __SIPP_MEANSTDDEV_H__ 

#include <sipp.h>

/// @sf_definition meanStdDev meanStdDev
/// @sf_description This kernel calculates mean and standard deviation of an array of elements
/// @sf_group Arithmetic
/// @ingroup SIPP_Software_Filters
/// @sf_preserve numPlanes
/// @sf_outdatatype float
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/meanStdDev/arch/$(MV_SOC_REV_DIR)/shave/src/meanStdDev.asm
/// @{

/// @brief Parameter structure of the @ref meanStdDev filter.

/// @brief Shave function of the @ref meanStdDev filter.
void SVU_SYM(svuMeanStdDev)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuMeanStdDev);

/// @}

#endif // __SIPP_MEANSTDDEV_H__ 
