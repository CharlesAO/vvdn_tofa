///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref censusMin64 SIPP filter API.
///

#ifndef __SIPP_CENSUSMin64_H__
#define __SIPP_CENSUSMin64_H__ 

#include <sipp.h>

/// @sf_definition censusMin64 censusMin64
/// @sf_description  mvcvCensusMin64 - computes minimum of 64 disparity costs values
/// @sf_group Arithmetic
/// @sf_preserve numPlanes
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/censusMin64/arch/$(MV_SOC_REV_DIR)/shave/src/censusMin64.asm
/// @{

/// @brief Parameter structure of the @ref censusMin64 filter.


/// @brief Shave function of the @ref censusMin64 filter.
void SVU_SYM(svuCensusMin64)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuCensusMin64);

/// @}

#endif // __SIPP_CENSUSMin64_H__ 
