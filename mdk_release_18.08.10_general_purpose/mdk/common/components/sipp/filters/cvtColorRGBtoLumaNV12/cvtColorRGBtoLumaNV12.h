///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref cvtColorRGBtoLumaNV12 SIPP filter API.
///    

#ifndef __SIPP_CVTCOLORRGBTOLUMANV12_H__
#define __SIPP_CVTCOLORRGBTOLUMANV12_H__ 

#include <sipp.h>

/// @sf_definition cvtColorRGBtoLumaNV12  RGB to Luma NV12 conversion
/// @sf_description  This filter performs conversion from RGB image format to Luma NV12.
/// @sf_group ISP
/// @sf_outdatatype UInt8
/// @sf_preserve imgSize, dataType
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/cvtColorRGBtoLumaNV12/arch/$(MV_SOC_REV_DIR)/shave/src/cvtColorRGBtoLumaNV12.asm
/// @{

/// @brief Parameter structure of the @ref cvtColorRGBtoLumaNV12 filter.
typedef struct {
/// @sf_desc 3x3 conversion matrix
float* coefsMat;//[3*3];
/// @sf_desc 3 offset parameters
float* offset;//[1*3];

}  cvtColorLumaNV12Param;

/// @brief Shave function of the @ref cvtColorRGBtoLumaNV12 filter.
void SVU_SYM(svuCvtColorRGBtoLumaNV12)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuCvtColorRGBtoLumaNV12);

/// @}

#endif // __SIPP_CVTCOLORRGBTOLUMANV12_H__
