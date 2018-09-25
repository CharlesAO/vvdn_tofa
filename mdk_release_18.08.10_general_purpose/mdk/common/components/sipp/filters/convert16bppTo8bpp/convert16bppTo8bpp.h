///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref convert16bppTo8bpp SIPP filter API.
///

#ifndef __SIPP_CONVERT16BPPTO8BPP_H__
#define __SIPP_CONVERT16BPPTO8BPP_H__ 


#include <sipp.h>

/// @sf_definition convert16bppTo8bpp Convert 16bpp To 8bpp
/// @sf_description This filter performs conversion from 16 bpp input array to 8 bpp.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_preserve imgSize, numPlanes
/// @sf_inputs
/// 	- datatypes: UInt16; kernels: 1x1
/// @{

/// @brief Shave function of the @ref convert16bppTo8bpp filter.
void SVU_SYM(svuConvert16bppTo8bpp)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConvert16bppTo8bpp);

/// @}

#endif // __SIPP_CONVERT16BPPTO8BPP_H__
