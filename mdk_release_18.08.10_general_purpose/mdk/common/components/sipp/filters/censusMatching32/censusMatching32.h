///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref censusMatching32 SIPP filter API.
///

#ifndef __SIPP_CENSUSMATCHING32_H__
#define __SIPP_CENSUSMATCHING32_H__

#include <sipp.h>

/// @sf_definition censusMatching32 censusMatching32
/// @sf_description  mvcvCensusMatching32 - performs an XOR operation between pixel one pixel in *in1 and 16 pixels from *in2 and counts up how many values of 1 are in the result
/// @sf_group Arithmetic
/// @sf_preserve numPlanes
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt32; kernels: 1x1
/// 	- datatypes: UInt32; kernels: 1x64
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/censusMatching32/arch/$(MV_SOC_REV_DIR)/shave/src/censusMatching32.asm
/// @{

/// @brief Parameter structure of the @ref censusMatching32 filter.
typedef struct
{
    /// @sf_pfdesc flag that enables right crossing (by default left crossing)
    UInt32 flag;
}
CensusMatching32Param;

/// @brief Shave function of the @ref censusMatching32 filter.
void SVU_SYM(svuCensusMatching32)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuCensusMatching32);
/// @}

#endif // __SIPP_CENSUSMATCHING32_H__
