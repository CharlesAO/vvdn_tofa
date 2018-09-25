///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref laplacian3x3 SIPP filter API.
///    

#ifndef __SIPP_LAPLACIAN3x3_H__
#define __SIPP_LAPLACIAN3x3_H__ 

#include <sipp.h>

/// @sf_definition laplacian3x3 Laplacian 3x3
/// @sf_description  The filter applies a Laplacian filter with custom size.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 3x3
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/sLaplacian3x3/arch/$(MV_SOC_REV_DIR)/shave/src/sLaplacian3x3.asm

/// @{

/// @brief Shave function of the @ref laplacian3x3 filter.
void SVU_SYM(svuLaplacian3x3)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuLaplacian3x3);

/// @}

#endif //__SIPP_LAPLACIAN3x3_H__
