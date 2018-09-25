///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref dilate5x5 SIPP filter API.
///

#ifndef __SIPP_DILATE5x5_H__
#define __SIPP_DILATE5x5_H__ 

#include <sipp.h>

/// @sf_definition dilate5x5 Dilate 5x5
/// @sf_description This filter performs a dilate on the source image using the specified structuring element.
/// @sf_group CV
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 5x5
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/dilate5x5/arch/$(MV_SOC_REV_DIR)/shave/src/dilate5x5.asm
/// @{

/// @brief Parameter structure of the @ref dilate5x5 filter
typedef struct
{
    /// @sf_pfdesc 5x5 dilate matrix.
    UInt8* dMat[5];
}
Dilate5x5Param;

/// @brief Shave function of the @ref dilate5x5 filter.
void SVU_SYM(svuDilate5x5)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuDilate5x5);

/// @}

#endif // __SIPP_DILATE5x5_H__
