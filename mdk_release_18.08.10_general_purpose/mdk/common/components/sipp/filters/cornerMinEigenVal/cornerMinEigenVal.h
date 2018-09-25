///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref cornerMinEigenVal SIPP filter API.
///

#ifndef __SIPP_CORNER_MIN_EIGENVAL_H__
#define __SIPP_CORNER_MIN_EIGENVAL_H__ 

#include <sipp.h>

/// @sf_definition cornerMinEigenVal Corner Min Eigenvalue
/// @sf_description This filter performs corner detection using minimum eigenvalue algorithm for a line.
/// @sf_group CV
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 5x5
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/cornerMinEigenVal/arch/$(MV_SOC_REV_DIR)/shave/src/cornerMinEigenVal.asm
/// @{

/// @brief Shave function of the @ref cornerMinEigenVal filter.
void SVU_SYM(svuCornerMinEigenVal)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuCornerMinEigenVal);

/// @}

#endif // __SIPP_CORNER_MIN_EIGENVAL_H__
