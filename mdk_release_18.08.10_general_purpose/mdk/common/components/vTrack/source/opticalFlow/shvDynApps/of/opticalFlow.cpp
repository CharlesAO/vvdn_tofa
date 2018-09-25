///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///


#include "mv_types.h"
#include "swcFrameTypes.h"
#include "svuCommonShave.h"
#include "mvcv.h"
#include "stdio.h"
#include "string.h"
#include "vTrack.h"
#include <opticalFlowPyrLK.h>
#include "opticalFlowPrivate.h"

static mvCvSize g_winSize;
static u32 g_pyrLevels;
static mvCvTermCriteria g_termCriteria;


void opticalFlowSetup(  tvPyramidBuffer* pyramid,
                        tvOpticalFlowCfg* algConfig,
                        Mat g_pyrA[MAX_PYRAMID_LEVELS],
                        Mat g_pyrB[MAX_PYRAMID_LEVELS]
){
    assert(algConfig->algoType == OF_MOVI); // extended config only valid for PC
    assert(algConfig->subpixelDetection == 0);

    g_winSize.width = algConfig->winSize.width;
    g_winSize.height = algConfig->winSize.height;
    g_pyrLevels = (algConfig->pyrLevels < MAX_PYRAMID_LEVELS) ? algConfig->pyrLevels :
        MAX_PYRAMID_LEVELS;
    g_termCriteria.max_iter = algConfig->termCriteria.max_iter;
    g_termCriteria.epsilon = algConfig->termCriteria.epsilon;

    for (u32 i = 0; i < g_pyrLevels + 1; i++)
    {
        mvcvSetData1(&g_pyrA[i], pyramid->pyrLevel[i].spec.height,
                     pyramid->pyrLevel[i].spec.width,
                     0, CV_8U);
        g_pyrA[i].step0[0] = pyramid->pyrLevel[i].spec.stride;
        g_pyrA[i].step0[1] = pyramid->pyrLevel[i].spec.bytesPP;
        g_pyrA[i].step = g_pyrA[i].step0[0];

        mvcvSetData1(&g_pyrB[i], pyramid->pyrLevel[i].spec.height,
                     pyramid->pyrLevel[i].spec.width,
                     0, CV_8U);
        g_pyrB[i].step0[0] = pyramid->pyrLevel[i].spec.stride;
        g_pyrB[i].step0[1] = pyramid->pyrLevel[i].spec.bytesPP;
        g_pyrB[i].step = g_pyrB[i].step0[0];
    }
}

void opticalFlowRun(tvOpticalFlowCfg* algConfig,
                    tvPyramidBuffer* pyrImgPrev,
                    tvPyramidBuffer* pyrImgCur,
                    tvXYLoc featuresPrev[],
                    tvXYLoc featuresCur[],
                    fp32 featuresError[],
                    u32 featuresCount)
{
    Mat g_pyrA[MAX_PYRAMID_LEVELS];
    Mat g_pyrB[MAX_PYRAMID_LEVELS];
    opticalFlowSetup(pyrImgPrev, algConfig, g_pyrA, g_pyrB);
    for (u32 i = 0; i < g_pyrLevels + 1; i++)
    {
        g_pyrA[i].data = (u8*) (pyrImgPrev->pyrLevel[i].p1) + PADDING_H;
        g_pyrB[i].data = (u8*) (pyrImgCur->pyrLevel[i].p1) + PADDING_H;
    }
    //TODO: Do we need this?
    u8 status[featuresCount];
    memset(featuresError, 0, featuresCount * sizeof(featuresError[0]));
    
    // Run PyrLK algorithm
    mvcvCalcOpticalFlowPyrLK_tuned(g_pyrA, g_pyrB,
                                   (mvCvPoint2D32f*) featuresPrev,
                                   (mvCvPoint2D32f*) featuresCur,
                                   status,
                                   featuresError, g_winSize, g_pyrLevels,
                                   g_termCriteria,
                                   MVCV_LKFLOW_INITIAL_GUESSES | MVCV_LKFLOW_PADDED_PYRAMIDS,
                                   featuresCount);
    // Scale errors within the range that makes sense to FM
    for (u32 i = 0; i < featuresCount; i++)
    {
        featuresError[i] *= 200.0f;
    }
    SHAVE_HALT;
}

/// TODO: This is a work-around for newer toolchain requiring main
int main(void)
{
    return 0;
}
