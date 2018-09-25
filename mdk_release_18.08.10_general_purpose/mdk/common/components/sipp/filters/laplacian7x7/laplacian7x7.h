///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref laplacian7x7 SIPP filter API.
///    

#ifndef __SIPP_LAPLACIAN7x7_H__
#define __SIPP_LAPLACIAN7x7_H__ 

#include <sipp.h>

/// @sf_definition laplacian7x7 Laplacian 7x7
/// @sf_description  The filter applies a Laplacian filter with custom size.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 7x7
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/sLaplacian7x7/arch/$(MV_SOC_REV_DIR)/shave/src/sLaplacian7x7.asm

/// @{

/// @brief Shave function of the @ref laplacian7x7 filter.
void SVU_SYM(svuLaplacian7x7)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuLaplacian7x7);

/// @}

#endif //__SIPP_LAPLACIAN7x7_H__
