///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref boxFilter3x3 SIPP filter API.
///

#ifndef __SIPP_BOXFILTER3x3_H__
#define __SIPP_BOXFILTER3x3_H__ 

#include <sipp.h>

/// @sf_definition boxFilter3x3 Box Filter 3x3
/// @sf_description This filter applies the box filter on the source image using the specified structuring element.
/// @sf_group CV
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 3x3
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/boxFilter3x3/arch/$(MV_SOC_REV_DIR)/shave/src/boxFilter3x3.asm
/// @{

/// @brief Parameter structure of the @ref boxFilter3x3 filter
typedef struct
{
/// @sf_desc 1 to normalize to kernel size, 0 otherwise
    UInt32 normalize;
}
BoxFilter3x3Param;

/// @brief Shave function of the @ref boxFilter3x3 filter.
void SVU_SYM(svuBoxFilter3x3)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuBoxFilter3x3);

/// @}

#endif // __SIPP_BOXFILTER3x3_H__ 
