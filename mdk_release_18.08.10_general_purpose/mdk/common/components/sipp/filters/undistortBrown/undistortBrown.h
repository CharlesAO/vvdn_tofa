/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref undistortBrown SIPP filter API.
///

//http://en.wikipedia.org/wiki/Distortion_%28optics%29

#ifndef __SIPP_UNDISTORTBROWN_H__
#define __SIPP_UNDISTORTBROWN_H__

#include <sipp.h>

/// @sf_definition undistortBrown Undistort
/// @sf_description This filter applies undistort using Brown's distortion model for known lens distortion coefficients. 
/// @sf_group ISP
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 128x2
/// @{

/// @brief Parameter structure of the @ref undistortBrown filter.
typedef struct
{
	/// @sf_desc Distortion center
    int   cx, cy;  //Distortion center
	/// @sf_desc Tangential distortion coefficients
    float p1, p2;  //Tangential distortion coefs
	/// @sf_desc Radial distortion coefficients
    float k1, k2;  //Radial distortion coefficient
}
UndistortBParam;

/// @brief Shave function of the @ref undistortBrown filter
void SVU_SYM(svuUndistortBrown)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuUndistortBrown);

/// @}

#endif //__SIPP_UNDISTORTBROWN_H__
