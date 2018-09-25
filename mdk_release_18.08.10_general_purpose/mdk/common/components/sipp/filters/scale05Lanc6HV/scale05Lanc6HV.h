///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref scale05Lanc6HV SIPP filter API.
///

#ifndef __SIPP_SCALE05LANC6HV_H__
#define __SIPP_SCALE05LANC6HV_H__ 

#include <sipp.h>

/// @sf_definition scale05Lanc6HV Lanczos Downscale by 2 (6 taps)
/// @sf_description This filter applies a lanczos downscale, with factor 0.5, and 6 taps; Horizontal and vertical directions.
/// @sf_group Arithmetic
/// @sf_preserve numPlanes, dataType
/// @sf_flags SIPP_RESIZE
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 6x6
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/scale05Lanc6HV/arch/$(MV_SOC_REV_DIR)/shave/src/scale05Lanc6HV.asm
/// @{

/// @brief Shave function of the @ref scale05Lanc6HV filter.
void SVU_SYM(svuScl05Lanc6)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuScl05Lanc6);

/// @}

#endif // __SIPP_SCALE05LANC6HV_H__ 
