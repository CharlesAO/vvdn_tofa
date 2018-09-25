///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref homography SIPP filter API.
///
#ifndef __SIPP_HOMOGRAPHY_H__
#define __SIPP_HOMOGRAPHY_H__ 

#include <sipp.h>

/// @sf_definition homography Homography
/// @sf_description This filter performs a homography transformation.
/// @sf_group CV
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 64x4
/// @{

/// @brief Parameter structure of the @ref homography filter.
typedef struct
{
	/// @sf_desc Pointer to float 3x3 Homography matrix
    float *homoMat3x3; //Ptr to 3x3 Homography matrix
}
HomographyParam;

/// @brief Shave function of the @ref homography filter.
void SVU_SYM(svuHomography)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuHomography);

/// @}

#endif //__SIPP_HOMOGRAPHY_H__
