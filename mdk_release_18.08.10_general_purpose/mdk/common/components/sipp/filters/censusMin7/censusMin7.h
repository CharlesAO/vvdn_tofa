///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref censusMin7 SIPP filter API.
///

#ifndef __SIPP_CENSUSMIN7_H__
#define __SIPP_CENSUSMIN7_H__ 

#include <sipp.h>

/// @sf_definition censusMin7 censusMin7
/// @sf_description  mvcvCensusMin7 - computes minimum of 7 disparity costs values
/// @sf_group Arithmetic
/// @sf_preserve numPlanes
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/censusMin7/arch/$(MV_SOC_REV_DIR)/shave/src/censusMin7.asm
/// @{

/// @brief Parameter structure of the @ref censusMin7 filter.

/// @brief Shave function of the @ref censusMin7 filter.
void SVU_SYM(svuCensusMin7)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuCensusMin7);

/// @}

#endif // __SIPP_CENSUSMIN7_H__ 
