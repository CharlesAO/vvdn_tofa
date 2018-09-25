///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref boxFilter15x15 SIPP filter API.
///

#ifndef __SIPP_BOXFILTER15x15_H__
#define __SIPP_BOXFILTER15x15_H__ 

#include <sipp.h>

/// @sf_definition boxFilter15x15 Box Filter 15x15
/// @sf_description This filter applies the box filter on the source image using the specified structuring element.
/// @sf_group CV
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 15x15
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/boxFilter15x15/arch/$(MV_SOC_REV_DIR)/shave/src/boxFilter15x15.asm
/// @{

/// @brief Parameter structure of the @ref boxFilter15x15 filter
typedef struct
{
/// @sf_desc 1 to normalize to kernel size, 0 otherwise
    UInt32 normalize;
}
BoxFilter15x15Param;


/// @brief Shave function of the @ref boxFilter15x15 filter.
void SVU_SYM(svuBoxFilter15x15)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuBoxFilter15x15);

/// @}

#endif // __SIPP_BOXFILTER15x15_H__ 
