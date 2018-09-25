///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     pixelPipeline for vtrack project
///

#include <string.h>
#include <stdio.h>

#include <stdlib.h>
#include <assert.h>
#include "swcFrameTypes.h"

#include "mv_types.h"
#include "pixelPipeHelper.h"
#include "pixelPipeCornerShaveHelper.h"
#include "vTrack.h"
#include "gaussHx2.h"
#include "gaussVx2.h"
#include "thresholdFilter.h"
#include "nonMax3x3_fp32.h"
#include "fifoCommApiDefines.h"
#include "global_constants.h"

#if defined(MA2150) || defined(MA2450)
#include "scCmxDma.h"
#include "scCmxDmaDefines.h"
#define ShDrvCmxDmaInitialize ScCmxDmaInitialize
#define ShDrvCmxDmaTransactionHnd ScCmxDmaTransactionHnd
#define ShDrvCmxDmaTransaction ScCmxDmaTransaction
#define ShDrvCmxDmaCreateTransaction ScCmxDmaCreateTransaction
#define ShDrvCmxDmaStartTransfer ScCmxDmaStartTransfer
#define ShDrvCmxDmaWaitTransaction ScCmxDmaWaitTransaction
#else
#include "ShDrvCmxDma.h"
#endif

//#define mvcvThresholdFilter_asm mvcvThresholdFilter
//#define mvcvGaussVx2_asm mvcvGaussVx2
//#define mvcvGaussHx2_asm mvcvGaussHx2
//#define mvcvNonMax3x3_fp32_asm mvcvNonMax3x3_fp32
#include "svuCommonShave.h"

struct tvFeatureCell;


void
insertThreshAndSort(tvFeatureCell* featureData,
             u32 x,
             u32 y,
             fp32 harrisScore,
             u32 maxFeatPerCell,
             fp32& threshold) {
    smartInsert(featureData, x, y, harrisScore, maxFeatPerCell, threshold);
}

void
insertThreshOnly(tvFeatureCell* featureData,
             u32 x,
             u32 y,
             fp32 harrisScore,
             u32 maxFeatPerCell,
             fp32& threshold) {
    if(harrisScore < threshold) {
        return;
    }
    s32 i = featureData->featureCount;
    if(i<(int)maxFeatPerCell){
        insertFeat(&featureData->features[i], x, y, harrisScore);
        featureData->featureCount++;
    }
}

void smartInsert(tvFeatureCell* featureData, u32 x, u32 y,
                        fp32 harrisScore, u32 maxFeatPerCell, fp32& threshold)
{
    if(harrisScore < threshold) {
        return;
    }
    s32 i = featureData->featureCount;
    t_vTrackHarrisFeat* features = featureData->features;
    while ((i > 0) && (features[i - 1].harris_score < harrisScore))
    {
        features[i] = features[i - 1];
        i--;
    }
    insertFeat(&features[i], x, y, harrisScore);

    if (featureData->featureCount < maxFeatPerCell)
        (featureData->featureCount)++;
}

void insertFeat(t_vTrackHarrisFeat* feat, u32 x, u32 y, fp32 score) {
    feat->harris_score = score;
    feat->x = x;
    feat->y = y;

}

