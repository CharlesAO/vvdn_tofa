///
/// @file scale2xBilinHV_025_075_Fp16ToFp16.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref scale2xBilinHV_025_075_Fp16ToFp16 SIPP filter API.
///

#ifndef __SIPP_SCALE2XBILINHV_025_075_Fp16ToFp16_H__
#define __SIPP_SCALE2XBILINHV_025_075_Fp16ToFp16_H__ 

#include <sipp.h>

/// @sf_definition scale2xBilinHV_025_075_Fp16ToFp16 Upscale by 2 with phases 0.25 and 0.75 fp16 to fp16
/// @sf_description This filter performs bilinear upscale with 2 factor; Horizontal and Vertical directions; phases 0.25 and 0.75 
/// @sf_group ISP
/// @sf_flags SIPP_RESIZE
/// @sf_outdatatype half
/// @sf_preserve numPlanes, dataType
/// @sf_inputs
/// 	- datatypes: half; kernels: 2x2
/// @sf_mandatoryinputs 1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/scale2xBilinHV025_Fp16ToFp16/arch/$(MV_SOC_REV_DIR)/shave/src/scale2xBilinHV025_Fp16ToFp16.asm,
///            $(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/scale2xBilinHV075_Fp16ToFp16/arch/$(MV_SOC_REV_DIR)/shave/src/scale2xBilinHV075_Fp16ToFp16.asm
/// @{

/// @brief Shave function of the @ref scale2xBilinHV_025_075_Fp16ToFp16 filter.
void SVU_SYM(svuScale2xBilinHV_025_075_Fp16ToFp16)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuScale2xBilinHV_025_075_Fp16ToFp16);

/// @}

#endif // __SIPP_SCALE2XBILINHV_025_075_Fp16ToFp16_H__ 
