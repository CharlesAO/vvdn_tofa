/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref convYuv444 SIPP filter API.
///

#ifndef __SIPP_CONVYUV444_H__
#define __SIPP_CONVYUV444_H__ 

#include <sipp.h>

/// @sf_definition convYuv444 Convert to YUV444
/// @sf_description This filter performs line conversion to YUV444.
/// @sf_group ISP
/// @sf_outdatatype UInt8
/// @sf_preserve imgSize, numPlanes
/// @sf_inputs
/// 	- datatypes: half; kernels: 1x1
/// @{

/// @brief Shave function of the @ref convYuv444 filter.
void SVU_SYM(svuRgbYuv444)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuRgbYuv444);

/// @}

#endif // __SIPP_CONVYUV444_H__
