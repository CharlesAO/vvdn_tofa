///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

#ifndef __OPTICAL_FLOW_PRIVATE_H__
#define __OPTICAL_FLOW_PRIVATE_H__

#include "mv_types.h"
#include "opticalFlowTypes.h"
#include "vTrackOutput.h" //tvXYLoc

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
class Mat;
    /// Setup parameters which are unlikely to change from one run to another
    /// @param[in] pyrImgPrev[] - array of frameBuffer representing the previous pyramid levels
    /// including the initial image as the first element
    /// @param[in] pyrImgCur[] - array of frameBuffer representing the current pyramid levels
    /// including the initial image as the first element
    /// @param[out] featureError[] - buffer containing features error
    /// @param[in]    winSize - window size used for tracking
    /// @param[in]    pyrLevels - number of levels in the pyramid including the initial image
    /// @param[in]    termCriteria - termination critera controlling the convergence of tracking
    /// @return - void
    ///
void opticalFlowSetup(  tvPyramidBuffer* pyramid,
                        tvOpticalFlowCfg* algConfig,
                        Mat g_pyrA[],
                        Mat g_pyrB[]
);
    /// Runs the Lucas-Kanade Pyramidal feature tracking algorithm
    /// @param[in] pyrImgPrev[] - array of frameBuffer representing the previous pyramid levels
    /// including the initial image as the first element
    /// @param[in] pyrImgCur[] - array of frameBuffer representing the current pyramid levels
    /// including the initial image as the first element
    /// @param[in] featuresPrev - array of features in the previous image
    /// @param[out] featuresCur - array of features tracked in the current image
    /// @param[in]     featuresCount - number of input features
    ///
    void opticalFlowRun(tvOpticalFlowCfg* algConfig,
            tvPyramidBuffer* pyrImgPrev,
            tvPyramidBuffer* pyrImgCur,
            tvXYLoc featuresPrev[],
            tvXYLoc featuresCur[],
            fp32 featuresError[],
            u32 featuresCount);
#ifdef __cplusplus
}
#endif

#endif


