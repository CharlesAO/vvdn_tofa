///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref cvtColorRGBfp16toUV420U8 SIPP filter API.
///

#ifndef __SIPP_CVTCOLOR_RGB_FP16_TO_UV420_U8_H__
#define __SIPP_CVTCOLOR_RGB_FP16_TO_UV420_U8_H__ 

#include <sipp.h>

/// @sf_definition cvtColorRGBfp16toUV420U8 RGB(fp16) to UV420(u8) conversion
/// @sf_description This filter performs color space conversion RGB(fp16) to UV420(U8) for one line in an image.
/// @sf_group ISP
/// @sf_preserve imgSize
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: half; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/cvtColorRGBfp16ToUV420U8/arch/$(MV_SOC_REV_DIR)/shave/src/cvtColorRGBfp16ToUV420U8.asm
/// @{

/// @brief Shave function of the @ref cvtColorRGBfp16toUV420U8 filter.
void SVU_SYM(svuCvtColorRGBfp16ToUV420U8)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuCvtColorRGBfp16ToUV420U8);

/// @}

#endif //__SIPP_CVTCOLOR_RGB_FP16_TO_UV420_U8_H__
