///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref cvtColorChromaYUVToNV12 SIPP filter API.
///    

#ifndef __SIPP_CVTCOLORCHROMAYUVTONV12_H__
#define __SIPP_CVTCOLORCHROMAYUVTONV12_H__

#include <sipp.h>
#include <swcFrameTypes.h>

/// @sf_definition cvtColorChromaYUVToNV12 YUV to NV12 chroma conversion
/// @sf_description  This filter performs conversion from YUV image format to NV12 for the chroma part only. The luma part is identical between these two formats and needs to be copied separately.
/// @sf_group ISP
/// @sf_outdatatype UInt8
/// @sf_preserve imgSize, dataType
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/cvtColorKernelChromaYUVToNV12/arch/$(MV_SOC_REV_DIR)/shave/src/cvtColorKernelChromaYUVToNV12.asm

/// @brief Parameter structure of the @ref cvtColorChromaYUVToNV12 filter.
typedef struct {
/// @sf_desc input frame format, supported: yuv420p, yuv444p
frameType inputFrameType;
UInt8 needs2Parents;
}  CvtColorChromaYUVToNV12Param;

/// @brief Shave function of the @ref cvtColorChromaYUVToNV12 filter.
void SVU_SYM(svuCvtColorChromaYUVToNV12)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuCvtColorChromaYUVToNV12);

/// @}

#endif // __SIPP_CVTCOLORCHROMAYUVTONV12_H__
