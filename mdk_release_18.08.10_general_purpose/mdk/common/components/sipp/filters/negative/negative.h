///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref negative SIPP filter API.
///

#ifndef __SIPP_NEGATIVE_H__
#define __SIPP_NEGATIVE_H__ 

#include <sipp.h>

/// @sf_definition negative Negative
/// @sf_description This filter creates the negative of the image.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @{

/// @brief Shave function of the @ref negative filter.
void SVU_SYM(svuNegative)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuNegative);

/// @}

#endif // __SIPP_NEGATIVE_H__ 
