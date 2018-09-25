///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref integralImageSumU32M2 SIPP filter API.
///    

#ifndef __SIPP_INTEGRAL_IMAGE_SUM_U32_H__
#define __SIPP_INTEGRAL_IMAGE_SUM_U32_H__ 

#include <sipp.h>

/// @sf_definition integralImageSumU32M2  Integral Image Sum(U32)
/// @sf_description  This filter  makes the sum of all pixels before it and on the left of it's column(this particular case makes square sum of pixels in U32 format).
/// @sf_group CV
/// @sf_preserve imgSize, numPlanes
/// @sf_outdatatype UInt32
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/integralImageSumM2/arch/$(MV_SOC_REV_DIR)/shave/src/integralImageSumM2.asm
/// @{


/// @brief Shave function of the @ref integralImageSumU32M2 filter.
void SVU_SYM(svuIntegralImageSumU32M2)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuIntegralImageSumU32M2);

/// @}

#endif //__SIPP_INTEGRAL_IMAGE_SUM_U32_H__
