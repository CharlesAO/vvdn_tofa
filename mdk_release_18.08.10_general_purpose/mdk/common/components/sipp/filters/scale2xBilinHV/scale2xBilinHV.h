///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref scale2xBilinHV SIPP filter API.
///

#ifndef __SIPP_SCALE2xBILINHV_H__
#define __SIPP_SCALE2xBILINHV_H__ 

#include <sipp.h>

/// @sf_definition scale2xBilinHV Upscale by 2
/// @sf_description This filter performs bilinear upscale with 2 factor; Horizontal and Vertical directions.
/// @sf_group Arithmetic
/// @sf_preserve numPlanes, dataType
/// @sf_flags SIPP_RESIZE
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 2x2
/// @{

/// @brief Shave function of the @ref scale2xBilinHV filter.
void SVU_SYM(svuScl2xBilinHV)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuScl2xBilinHV); 

/// @}

#endif //__SIPP_SCALE2xBILINHV_H__
