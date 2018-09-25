///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref boxFilter9x9 SIPP filter API.
///

#ifndef __SIPP_BOXFILTER9x9_H__
#define __SIPP_BOXFILTER9x9_H__ 

#include <sipp.h>

/// @sf_definition boxFilter9x9 Box Filter 9x9
/// @sf_description This filter applies the box filter on the source image using the specified structuring element.
/// @sf_group CV
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 9x9
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/boxFilter9x9/arch/$(MV_SOC_REV_DIR)/shave/src/boxFilter9x9.asm
/// @{

/// @brief Parameter structure of the @ref boxFilter9x9 filter
typedef struct
{
/// @sf_desc 1 to normalize to kernel size, 0 otherwise
    UInt32 normalize;
}
BoxFilter9x9Param;

/// @brief Shave function of the @ref boxFilter9x9 filter.
void SVU_SYM(svuBoxFilter9x9)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuBoxFilter9x9);

/// @}

#endif // __SIPP_BOXFILTER9x9_H__ 
