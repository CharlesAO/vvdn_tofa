///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref pyrDown SIPP filter API.
///

#ifndef __SIPP_PYRDOWN_H__
#define __SIPP_PYRDOWN_H__ 

#include <sipp.h>

/// @sf_definition pyrDown Pyramid Downscale
/// @sf_description This filter applies a pyramid operator using 5x5 gauss downscale operator.
/// @sf_group Arithmetic
/// @sf_flags SIPP_RESIZE
/// @sf_preserve numPlanes
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 5x5
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/pyrdown/arch/$(MV_SOC_REV_DIR)/shave/src/pyrdown.asm
/// @{

/// @brief Shave function of the @ref pyrDown filter.
void SVU_SYM(svuPyrDown)(SippFilter *fptr);
SHAVE_SYM_EXPORT (svuPyrDown);

/// @}

#endif // __SIPP_PYRDOWN_H__ 
