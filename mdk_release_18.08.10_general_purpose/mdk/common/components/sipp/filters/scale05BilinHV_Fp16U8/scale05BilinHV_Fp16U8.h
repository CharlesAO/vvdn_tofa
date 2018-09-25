///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref scale05BilinHV_Fp16U8 SIPP filter API.
///

#ifndef __SIPP_SCALE05BILINHV_FP16U8_H__
#define __SIPP_SCALE05BILINHV_FP16U8_H__ 

#include <sipp.h>

/// @sf_definition scale05BilinHV_Fp16U8 Downscale by 2 (fp16/u8)
/// @sf_description This filter performs bilinear downscale with 0.5 factor – Horizontal and Vertical directions. fp16 in/u8 out variant.
/// @sf_group Arithmetic
/// @sf_preserve numPlanes, dataType
/// @sf_flags SIPP_RESIZE
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: half; kernels: 2x2
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/scale05BilinHV_Fp16U8/arch/$(MV_SOC_REV_DIR)/shave/src/scale05BilinHV_Fp16U8.asm
/// @{

/// @brief Shave function of the @ref scale05BilinHV_Fp16U8 filter.
void SVU_SYM(svuScale05BilinHV_Fp16U8)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuScale05BilinHV_Fp16U8); 

/// @}

#endif // __SIPP_SCALE05BILINHV_FP16U8_H__ 
