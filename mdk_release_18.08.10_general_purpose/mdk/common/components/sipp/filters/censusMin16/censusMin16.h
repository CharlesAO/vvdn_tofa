///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref censusMin16 SIPP filter API.
///

#ifndef __SIPP_CENSUSMIN16_H__
#define __SIPP_CENSUSMIN16_H__ 

#include <sipp.h>

/// @sf_definition censusMin16 censusMin16
/// @sf_description  mvcvCensusMin16 - computes minimum of 16 disparity costs values
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_preserve numPlanes
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/censusMin16/arch/$(MV_SOC_REV_DIR)/shave/src/censusMin16.asm
/// @{

/// @brief Parameter structure of the @ref censusMin16 filter.

/// @brief Shave function of the @ref censusMin16 filter.
void SVU_SYM(svuCensusMin16)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuCensusMin16);

/// @}

#endif // __SIPP_CENSUSMIN16_H__ 
