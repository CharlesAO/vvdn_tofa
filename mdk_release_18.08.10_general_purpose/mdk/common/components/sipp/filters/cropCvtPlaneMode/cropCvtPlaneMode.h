///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref CropCvtPlaneMode SIPP filter API.
///

#ifndef __SIPP_CROP_CVT_PLANE_MODE_H__
#define __SIPP_CROP_CVT_PLANE_MODE_H__

#include <sipp.h>

/// @sf_definition crop Crop ConvertPlane
/// @sf_description This filter performs a crop on the input image and converts RGB planar to interleaved
/// @sf_group CV
/// @sf_flags SIPP_CROP
/// @defgroup CV
/// @ingroup SIPP_Software_Filters
/// @sf_outdatatype UInt8
/// @sf_preserve numPlanes, dataType
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @{
typedef struct
{
 /// @sf_pfdesc Starting point on vertical crop
  UInt32 st_Y;//Starting point on Vertical
  //on horizontal max offset is given by padding param... to deal with this later...
}
CropParam;

/// @brief Shave function of the @ref crop filter.
void SVU_SYM(svuCropCvtPlaneMode)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuCropCvtPlaneMode);

/// @}

#endif // __SIPP_CROP_CVT_PLANE_MODE_H__
