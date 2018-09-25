///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref boxFilter5x5 SIPP filter API.
///

#ifndef __SIPP_BOXFILTER5x5_H__
#define __SIPP_BOXFILTER5x5_H__

#include <sipp.h>

/// @sf_definition boxFilter5x5 Box Filter 5x5
/// @sf_description This filter applies the box filter on the source image using the specified structuring element.
/// @sf_group CV
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 5x5
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/boxFilter5x5/arch/$(MV_SOC_REV_DIR)/shave/src/boxFilter5x5.asm
/// @{

/// @brief Parameter structure of the @ref boxFilter5x5 filter
typedef struct
{
/// @sf_desc 1 to normalize to kernel size, 0 otherwise
    UInt32 normalize;
}
BoxFilter5x5Param;

/// @brief Shave function of the @ref boxFilter5x5 filter.
void SVU_SYM(svuBoxFilter5x5)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuBoxFilter5x5);

/// @}

#endif // __SIPP_BOXFILTER5x5_H__
