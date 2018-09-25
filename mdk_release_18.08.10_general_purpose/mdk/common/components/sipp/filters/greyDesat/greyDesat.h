/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref greyDesat SIPP filter API.
///

#ifndef __SIPP_GREYDESAT_H__
#define __SIPP_GREYDESAT_H__

#include <sipp.h>

/// @sf_definition greyDesat greyDesat
/// @sf_description This filter desaturates areas which are already very close to grey.
/// @sf_group ISP
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 3x3
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/greyDesat/arch/$(MV_SOC_REV_DIR)/shave/src/greyDesat.asm
/// @{

/// @brief Parameter structure of the @ref greyDesat filter.
typedef struct
{
    int offset;
    int slope;
    Int32 grey[3];
} GreyDesatParam;

/// @brief Shave function of the @ref greyDesat filter
void SVU_SYM(svuGreyDesat)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuGreyDesat);

/// @}

#endif //__SIPP_GREYDESAT_H__
