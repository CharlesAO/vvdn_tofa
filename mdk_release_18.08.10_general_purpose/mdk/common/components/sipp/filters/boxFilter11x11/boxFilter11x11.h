///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref boxFilter11x11 SIPP filter API.
///

#ifndef __SIPP_BOXFILTER11x11_H__
#define __SIPP_BOXFILTER11x11_H__ 

#include <sipp.h>

/// @sf_definition boxFilter11x11 Box Filter 11x11
/// @sf_description This filter applies the box filter on the source image using the specified structuring element.
/// @sf_group CV
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 11x11
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/boxFilter11x11/arch/$(MV_SOC_REV_DIR)/shave/src/boxFilter11x11.asm
/// @{

/// @brief Parameter structure of the @ref boxFilter11x11 filter
typedef struct
{
/// @sf_desc 1 to normalize to kernel size, 0 otherwise
    UInt32 normalize;
}
BoxFilter11x11Param;

/// @brief Shave function of the @ref boxFilter11x11 filter.
void SVU_SYM(svuBoxFilter11x11)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuBoxFilter11x11);

/// @}

#endif // __SIPP_BOXFILTER11x11_H__ 
