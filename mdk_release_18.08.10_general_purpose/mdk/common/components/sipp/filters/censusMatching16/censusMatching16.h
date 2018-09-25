///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref censusMatching16 SIPP filter API.
///

#ifndef __SIPP_CENSUSMATCHING16_H__
#define __SIPP_CENSUSMATCHING16_H__ 

#include <sipp.h>

/// @sf_definition censusMatching16 censusMatching16
/// @sf_description  mvcvCensusMatching16 - performs an XOR operation between pixel one pixel in *in1 and 16 pixels from *in2 and counts up how many values of 1 are in the result
/// @sf_group Arithmetic
/// @sf_preserve numPlanes
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt32; kernels: 1x1
/// 	- datatypes: UInt32; kernels: 1x32
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/censusMatching16/arch/$(MV_SOC_REV_DIR)/shave/src/censusMatching16.asm
/// @{

/// @brief Parameter structure of the @ref censusMatching16 filter.


/// @brief Shave function of the @ref censusMatching16 filter.
void SVU_SYM(svuCensusMatching16)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuCensusMatching16);

/// @}

#endif // __SIPP_CENSUSMATCHING16_H__ 
