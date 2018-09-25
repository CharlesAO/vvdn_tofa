///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref copy SIPP filter API.
///

#ifndef __SIPP_COPY_H__
#define __SIPP_COPY_H__ 


#include <sipp.h>

/// @sf_definition copy Copy
/// @sf_description This filter copies input image to output.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @{


/// @brief Shave function of the @ref copy filter.
void SVU_SYM(svuCopy)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuCopy);

/// @}

#endif // __SIPP_COPY_H__
