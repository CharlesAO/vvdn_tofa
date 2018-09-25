///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref gaussVx2_fp16 SIPP filter API.
///

#ifndef __SIPP_GAUSS_Vx2_FP16_H__
#define __SIPP_GAUSS_Vx2_FP16_H__ 

#include <sipp.h>

/// @sf_definition gaussVx2_fp16 GaussVx2_fp16
/// @sf_description This filter applies downscale 2x vertical gaussian blur. Has to be used in combination with GaussHx2 to obtain correct output.
/// @sf_group Arithmetic
/// @sf_flags SIPP_RESIZE
/// @sf_outdatatype half
/// @sf_preserve numPlanes, dataType
/// @sf_inputs
///		- datatypes: half; kernels: 5x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/gaussVx2_fp16/arch/$(MV_SOC_REV_DIR)/shave/src/gaussVx2_fp16.asm
/// @{

/// @brief Shave function of the @ref gaussVx2_fp16 filter.
void SVU_SYM(svuGaussVx2_fp16)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuGaussVx2_fp16);

/// @}

#endif // __SIPP_GAUSS_Vx2_FP16_H__ 
