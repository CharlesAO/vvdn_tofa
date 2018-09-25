///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Optical Flow kernel Api
///
///
#ifndef __CALC_OPTICAL_FLOW_H__
#define __CALC_OPTICAL_FLOW_H__

#include <mvcv_types.h>
#include <mvcv_macro.h>

/// Optical Flow algorithm - golden implementation extracted from OpenCV
/// @param[in] imgA         First input image
/// @param[in] imgB         Second input image
/// @param[in] size         Size of the input image
/// @param[out] pyrA        Pointer to a preallocated buffer which needs to accomodate all pyramid levels
/// @param[out] pyrB        Pointer to a preallocated buffer which needs to accomodate all pyramid levels
/// @param[in] featuresA    Input list of features
/// @param[out] featuresB   Output list of tracked features
/// @param[out] status      Found/not found status of each input feature
/// @param[out] error       Error value for each tracked feature
/// @param[in] winSize      Size of the search window used for tracking
/// @param[in] level        Number of pyramid levels to generate
/// @param[in] criteria     Termination criteria for the tracking algorithm (minimum error or maximum iterations reached)
/// @param[in] flags        Flags specifying various runtime options
/// @param[in] nb_points    Number of input features
/// @return                 Returns 0 if it has finished succesfully, non-0 for error
s32 calcOpticalFlowPyrLK(u8* imgA, u8* imgB, mvCvSize size, u8* pyrA, u8* pyrB,
                         mvCvPoint2D32f* featuresA,
                         mvCvPoint2D32f* featuresB, u8 *status, fp32 *error,
                         mvCvSize winSize,
                         u32 level, mvCvTermCriteria criteria, u32 flags,
                         u32 nb_points);

/// Optical Flow algorithm - optimized implementation for Myriad
/// @param[in] pyrA         Vector containing the first image pyramid including the initial image on index 0
/// @param[in] pyrB         Vector containing the second image pyramid including the initial image on index 0
/// @param[in] featuresA    Input list of features
/// @param[in] featuresB    Output list of tracked features
/// @param[out] status      Found/not found status of each input feature
/// @param[out] error       Error value for each tracked feature
/// @param[in] _winSize     Size of the search window used for tracking
/// @param[in] levels       Number of pyramid levels to generate
/// @param[in] criteria     Termination criteria for the tracking algorithm (minimum error or maximum iterations reached)
/// @param[in] flags        Flags specifying various runtime options
/// @param[in] count        Number of input features
/// @return                 Returns 0 if it has finished succesfully, non-0 for error
MVCV_FUNC(s32, mvcvCalcOpticalFlowPyrLK_tuned, Mat pyrA[], Mat pyrB[],
          mvCvPoint2D32f* featuresA,
          mvCvPoint2D32f* featuresB,
          u8 *status, fp32 *error, mvCvSize _winSize, u32 levels,
          mvCvTermCriteria criteria,
          u32 flags, u32 count)

#endif
