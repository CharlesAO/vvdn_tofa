///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref cornerMinEigenValpatched SIPP filter API.
///

#ifndef __SIPP_CORNER_MIN_EIGENVALPATCHED_H__
#define __SIPP_CORNER_MIN_EIGENVALPATCHED_H__ 

#include <sipp.h>

/// @sf_definition cornerMinEigenValpatched Corner Min Eigenvalue Patched
/// @sf_description This filter performs corner detection using minimum eigenvalue algorithm for a single pixel.
/// @sf_group CV
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 5x5
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/cornerMinEigenVal_patched/arch/$(MV_SOC_REV_DIR)/shave/src/cornerMinEigenVal_patched.asm
/// @{


/// @brief Shave function of the @ref cornerMinEigenValpatched filter.
void SVU_SYM(svuCornerMinEigenValpatched)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuCornerMinEigenValpatched);

/// @}

#endif // __SIPP_CORNER_MIN_EIGENVALPATCHED_H__
