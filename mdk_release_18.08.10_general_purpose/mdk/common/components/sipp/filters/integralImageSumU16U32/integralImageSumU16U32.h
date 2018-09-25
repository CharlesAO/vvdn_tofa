///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref integralImageSumU16U32 SIPP filter API.
///    

#ifndef __SIPP_INTEGRAL_IMAGE_SUM_U16U32_H__
#define __SIPP_INTEGRAL_IMAGE_SUM_U16U32_H__ 

#include <sipp.h>

/// @sf_definition integralImageSumU16U32  Integral Image Sum(U16toU32)
/// @sf_description  This filter makes the sum of all pixels before it and on the left of it's column(this particular case makes square sum of pixels in U32 format).
/// @sf_group CV
/// @sf_preserve imgSize, numPlanes
/// @sf_outdatatype UInt32
/// @sf_inputs
/// 	- datatypes: UInt16; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/integralImageSumU16U32/arch/$(MV_SOC_REV_DIR)/shave/src/integralImageSumU16U32.asm
/// @{


/// @brief Shave function of the @ref integralImageSumU16U32 filter.
void SVU_SYM(svuIntegralImageSumU16U32)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuIntegralImageSumU16U32);

/// @}

#endif //__SIPP_INTEGRAL_IMAGE_SUM_U16U32_H__
