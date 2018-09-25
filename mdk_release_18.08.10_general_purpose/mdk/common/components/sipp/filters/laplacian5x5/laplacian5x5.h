///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref laplacian5x5 SIPP filter API.
///    

#ifndef __SIPP_LAPLACIAN5x5_H__
#define __SIPP_LAPLACIAN5x5_H__ 

#include <sipp.h>

/// @sf_definition laplacian5x5 Laplacian 5x5
/// @sf_description  The filter applies a Laplacian filter with custom size.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 5x5
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/sLaplacian5x5/arch/$(MV_SOC_REV_DIR)/shave/src/sLaplacian5x5.asm

/// @{

/// @brief Shave function of the @ref laplacian5x5 filter.
void SVU_SYM(svuLaplacian5x5)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuLaplacian5x5);

/// @}

#endif //__SIPP_LAPLACIAN5x5_H__
