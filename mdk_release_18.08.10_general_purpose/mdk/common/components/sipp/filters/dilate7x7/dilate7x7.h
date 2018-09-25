///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref dilate7x7 SIPP filter API.
///

#ifndef __SIPP_DILATE7x7_H__
#define __SIPP_DILATE7x7_H__ 

#include <sipp.h>

/// @sf_definition dilate7x7 Dilate 7x7
/// @sf_description This filter performs a dilate on the source image using the specified structuring element.
/// @sf_group CV
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 7x7
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/dilate7x7/arch/$(MV_SOC_REV_DIR)/shave/src/dilate7x7.asm
/// @{

/// @brief Parameter structure of the @ref dilate7x7 filter
typedef struct
{
    /// @sf_pfdesc 7x7 dilate matrix.
    UInt8* dMat[7];
}
Dilate7x7Param;

/// @brief Shave function of the @ref dilate7x7 filter.
void SVU_SYM(svuDilate7x7)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuDilate7x7);

/// @}

#endif // __SIPP_DILATE7x7_H__ 
