///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @defgroup bicubicWarpApi Bicubic Warp API
/// @ingroup  bicubicWarpApi
/// @{
/// @brief     Bicubic Warp API
///
/// This is the functions API for the Bicubic Warp component
///

#ifndef __BICUBIC_WARP_API_H__
#define __BICUBIC_WARP_API_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "bicubicWarpApiDefines.h"

/// @brief Initialize bicubic block
/// @param[in] ctx - pointer to bicubicWarpContext
/// @return    int
///
int bicubicWarpInit(bicubicWarpContext* ctx);

/// @brief Run bicubic block
/// @param[in] ctx - pointer to bicubicWarpContext
/// @return    int
///
int bicubicWarpProcessFrame(bicubicWarpContext* ctx);

/// @brief Generate rectified coordinates based on rotation and translation (RT) relative to the center.
/// @param[in] ctx - pointer to bicubicWarpContext
/// @return    int
///
void bicubicWarpGenerateMeshRT(bicubicWarpContext* ctx);

/// @brief Generate rectified coordinates based on the homography (OpenCV style)
/// @param[in] ctx - pointer to bicubicWarpContext
/// @return    int
///
void bicubicWarpGenerateMeshHomographyRTP(bicubicWarpContext* ctx);

/// @brief Generate rectified coordinates from LUT maps
/// @param[in] ctx - pointer to bicubicWarpContext
/// @return    int
///
void bicubicWarpGenerateMeshFromLUTMaps(bicubicWarpContext* ctx);

#ifdef __cplusplus
}
#endif

#endif // __BICUBIC_WARP_RTEMS_API_H__
