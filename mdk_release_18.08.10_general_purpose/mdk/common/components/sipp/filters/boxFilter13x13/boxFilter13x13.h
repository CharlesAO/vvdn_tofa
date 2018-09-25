///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref boxFilter13x13 SIPP filter API.
///

#ifndef __SIPP_BOXFILTER13x13_H__
#define __SIPP_BOXFILTER13x13_H__ 

#include <sipp.h>

/// @sf_definition boxFilter13x13 Box Filter 13x13
/// @sf_description This filter applies the box filter on the source image using the specified structuring element.
/// @sf_group CV
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 13x13
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/boxFilter13x13/arch/$(MV_SOC_REV_DIR)/shave/src/boxFilter13x13.asm
/// @{

/// @brief Parameter structure of the @ref boxFilter13x13 filter
typedef struct
{
/// @sf_desc 1 to normalize to kernel size, 0 otherwise
    UInt32 normalize;
}
BoxFilter13x13Param;

/// @brief Shave function of the @ref boxFilter13x13 filter.
void SVU_SYM(svuBoxFilter13x13)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuBoxFilter13x13);

/// @}

#endif // __SIPP_BOXFILTER13x13_H__ 
