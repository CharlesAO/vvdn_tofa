///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref dilate3x3 SIPP filter API.
///

#ifndef __SIPP_DILATE3x3_H__
#define __SIPP_DILATE3x3_H__ 

#include <sipp.h>

/// @sf_definition dilate3x3 Dilate 3x3
/// @sf_description This filter performs a dilate on the source image using the specified structuring element.
/// @sf_group CV
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 3x3
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/dilate3x3/arch/$(MV_SOC_REV_DIR)/shave/src/dilate3x3.asm
/// @{

/// @brief Parameter structure of the @ref dilate3x3 filter
typedef struct
{
    /// @sf_pfdesc 3x3 dilate matrix.
    UInt8* dMat[3];
}
Dilate3x3Param;

/// @brief Shave function of the @ref dilate3x3 filter.
void SVU_SYM(svuDilate3x3)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuDilate3x3);

/// @}

#endif // __SIPP_DILATE3x3_H__
