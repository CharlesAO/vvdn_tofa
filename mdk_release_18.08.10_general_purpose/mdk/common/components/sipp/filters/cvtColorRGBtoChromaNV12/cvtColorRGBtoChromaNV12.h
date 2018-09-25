///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref cvtColorRGBtoChromaNV12 SIPP filter API.
///    

#ifndef __SIPP_CVTCOLORRGBTOCHROMANV12_H__
#define __SIPP_CVTCOLORRGBTOCHROMANV12_H__ 

#include <sipp.h>

/// @sf_definition cvtColorRGBtoChromaNV12  RGB to Chroma NV12 conversion
/// @sf_description  This filter performs conversion from RGB image format to Chroma NV12.
/// @sf_group ISP
/// @sf_outdatatype UInt8
/// @sf_preserve imgSize, dataType
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/cvtColorRGBtoChromaNV12/arch/$(MV_SOC_REV_DIR)/shave/src/cvtColorRGBtoChromaNV12.asm
/// @{

/// @brief Parameter structure of the @ref cvtColorRGBtoChromaNV12 filter.
typedef struct {
/// @sf_desc 3x3 conversion matrix
float* coefsMat;//[3*3];
/// @sf_desc 3 offset parameters
float* offset;//[1*3];

}  cvtColorChromaNV12Param;

/// @brief Shave function of the @ref cvtColorRGBtoChromaNV12 filter.
void SVU_SYM(svuCvtColorRGBtoChromaNV12)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuCvtColorRGBtoChromaNV12);

/// @}

#endif // __SIPP_CVTCOLORRGBTOCHROMANV12_H__
