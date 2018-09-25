///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref erode5x5 SIPP filter API.
///

#ifndef __SIPP_ERODE5x5_H__
#define __SIPP_ERODE5x5_H__ 

#include <sipp.h>

/// @sf_definition erode5x5 Erode 5x5
/// @sf_description This filter applies the erode filter on the source image using the specified structuring element.
/// @sf_group CV
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 5x5
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/erode5x5/arch/$(MV_SOC_REV_DIR)/shave/src/erode5x5.asm
/// @{

/// @brief Parameter structure of the @ref erode5x5 filter
typedef struct
{
    /// @sf_pfdesc 5x5 erode matrix.
    UInt8* eMat[5];
}
Erode5x5Param;

/// @brief Shave function of the @ref erode5x5 filter.
void SVU_SYM(svuErode5x5)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuErode5x5);

/// @}

#endif // __SIPP_ERODE5x5_H__
