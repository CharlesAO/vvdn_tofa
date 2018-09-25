///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref boxFilter7x7 SIPP filter API.
///

#ifndef __SIPP_BOXFILTER7x7_H__
#define __SIPP_BOXFILTER7x7_H__ 

#include <sipp.h>

/// @sf_definition boxFilter7x7 Box Filter 7x7
/// @sf_description This filter applies the box filter on the source image using the specified structuring element.
/// @sf_group CV
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 7x7
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/boxFilter7x7/arch/$(MV_SOC_REV_DIR)/shave/src/boxFilter7x7.asm
/// @{

/// @brief Parameter structure of the @ref boxFilter7x7 filter
typedef struct
{
/// @sf_desc 1 to normalize to kernel size, 0 otherwise
    UInt32 normalize;
}
BoxFilter7x7Param;

/// @brief Shave function of the @ref boxFilter7x7 filter.
void SVU_SYM(svuBoxFilter7x7)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuBoxFilter7x7);

/// @}

#endif // __SIPP_BOXFILTER7x7_H__ 
