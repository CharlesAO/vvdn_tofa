///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref scharr_fp16 SIPP filter API.
///

#ifndef __SIPP_SCHARR_H__
#define __SIPP_SCHARR_H__ 

#include <sipp.h>

/// @sf_definition scharr_fp16 scharr_fp16
/// @sf_description This kernel performs scharr edge detection operator
/// @sf_group CV
/// @sf_outdatatype half
/// @sf_inputs
/// 	- datatypes: half; kernels: 3x3
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/scharr_fp16/arch/$(MV_SOC_REV_DIR)/shave/src/scharr_fp16.asm
/// @{

/// @brief Shave function of the @ref scharr_fp16 filter.
void SVU_SYM(svuScharr_fp16)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuScharr_fp16);

/// @}

#endif // __SIPP_SCHARR_H__ 
