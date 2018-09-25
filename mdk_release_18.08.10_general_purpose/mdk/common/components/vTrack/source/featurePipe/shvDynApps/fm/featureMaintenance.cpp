///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///

#include "featureMaintenancePrivate.h"
#include "global_constants.h"
#include "vTrackHarrisResponse.h"
#include "vTrack.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <svuCommonShave.h>
#include "vTrackPrivateTypes.h"

#define RELOCALIZE_SQ_DISTANCE_THR 1


#include "mvcv.h"
#include "mvcv_types.h"

#define CURR(ring_buffer) ring_buffer[(g_framesNum + 0) % 2]
#define NEXT(ring_buffer) ring_buffer[(g_framesNum + 1) % 2]

#define INSERTION_SUCCESS 0
#define INSERTION_FAILED -1
static u32 g_framesNum;
static u32* g_featuresCount;
// TODO: get rid of this
static tvFeatureCell** g_featureCells;
static u32* g_featureIdCounter;
static mvCvPoint2D32f** g_features;
static FeatureMetadata** g_featureMetadata;
static fp32** g_featureError;
static u32** g_cellFeatureCount;
static fmMode mode;
static bool filterFeatures;

static inline float distSqr(mvCvPoint2D32f* pt1, mvCvPoint2D32f* pt2)
{
    float dx = pt1->x - pt2->x;
    float dy = pt1->y - pt2->y;
    return dx * dx + dy * dy;
}

static inline u32 SelectCellGridIndex(const mvCvPoint2D32f* crtFrameFeatures,
                                      u32 cellWidth,
                                      u32 cellHeight,
                                      u32 cellGridDimension)
{
    u32 c = (u32) ((crtFrameFeatures->x) / cellWidth);
    u32 r = (u32) ((crtFrameFeatures->y) / cellHeight);
    if (c >= cellGridDimension || r >= cellGridDimension)
        return 0xFFFFFFFF;
    else
        return r * cellGridDimension + c;
}
static inline void copyFeature(mvCvPoint2D32f* destFeature, fp32* destError,
                               FeatureMetadata* destMeta,
                               const mvCvPoint2D32f* srcFeature, const fp32* srcError,
                               const FeatureMetadata* srcMeta )
{
    *destFeature = *srcFeature;
    *destError = *srcError;
    *destMeta = *srcMeta;
}
static inline s32
insertFeature(mvCvPoint2D32f* destFeatureBuff, fp32* destErrorBuff,
              FeatureMetadata* destMetaBuff, u32* currentFeatCnt,
              const mvCvPoint2D32f* srcFeature, const fp32* srcError,
              const FeatureMetadata* srcMeta,
              const fp32 minimumFeatureDist, u32 cellMaxFeatures)
{
    s32 featIndex;
    u32 dropList[cellMaxFeatures];
    s32 dropCount = 0;

    // Check if features are too close to each other
    if(minimumFeatureDist > 0)
    {
        for (featIndex = (*currentFeatCnt) - 1; featIndex >= 0; featIndex--)
        {
            fp32 dist_sqr = distSqr(&destFeatureBuff[featIndex],
                                    (mvCvPoint2D32f*)srcFeature);
            // if the feature is too close to this feature
            if (dist_sqr < minimumFeatureDist)
            {
                if (destMetaBuff[featIndex].age > srcMeta->age)
                {
                    // the candidate is younger or the same age, so drop it
                    // the current feature don't have it's age increased, yet, so we need to use > and not >=
                    return INSERTION_FAILED;
                }
                else
                {
                    // the candidate is older, mark this feature to drop
                    // we may have multiple features which need drop
                    dropList[dropCount++] = featIndex;
                }
            }
        }
        assert(dropCount < (s32)cellMaxFeatures);
    }
    if (dropCount == 0)
    {
        copyFeature(&destFeatureBuff[*currentFeatCnt],
                    &destErrorBuff[*currentFeatCnt],
                    &destMetaBuff[*currentFeatCnt],
                    srcFeature, srcError, srcMeta);
        destMetaBuff[*currentFeatCnt].age++;

        // one feature added, no feature removed -> increment count
        (*currentFeatCnt)++;
    }
    else if (dropCount == 1)
    {
        copyFeature(&destFeatureBuff[dropList[0]],
                    &destErrorBuff[dropList[0]],
                    &destMetaBuff[dropList[0]],
                    srcFeature, srcError, srcMeta);
        destMetaBuff[dropList[0]].age++;
        // one feature added, one feature removed -> count remains the same

    }
    else
    {
        // copy candidate to the position where the last feature was dropped
        copyFeature(&destFeatureBuff[dropList[dropCount - 1]],
                    &destErrorBuff[dropList[dropCount - 1]],
                    &destMetaBuff[dropList[dropCount - 1]],
                    srcFeature, srcError, srcMeta);
        destMetaBuff[dropList[dropCount - 1]].age++;
        dropCount--;
        // drop every feature which is too close to the candidate
        while (dropCount > 0)
        {
            copyFeature(&destFeatureBuff[dropList[dropCount - 1]],
                        &destErrorBuff[dropList[dropCount - 1]],
                        &destMetaBuff[dropList[dropCount - 1]],
                        &destFeatureBuff[*currentFeatCnt - 1],
                        &destErrorBuff[*currentFeatCnt - 1],
                        &destMetaBuff[*currentFeatCnt - 1]);
            (*currentFeatCnt)--;
            dropCount--;
        }
    }
    return INSERTION_SUCCESS;

}

void collectTrackedFeatures(const frameBuffer* frame,
                            const u32 cellWidth,
                            const u32 cellHeight,
                            const u32 cellCol,
                            const u32 cellRow,
                            const u32 featureCountIn[],
                            u32 featureCountOut[],
                            const u32 cellGridDimension,
                            const u32 nCells,
                            const u32 cellMaxFeatures,
                            const mvCvPoint2D32f frameFeaturesIn[],
                            const fp32 featureErrorIn[],
                            const FeatureMetadata featureMetadataIn[],
                            mvCvPoint2D32f frameFeaturesOut[],
                            float featureErrorOut[],
                            FeatureMetadata featureMetadataOut[],
                            const FMFeatureThresholds_t* thresholds)
{
    u32 cellIdx = cellRow * cellGridDimension + cellCol;
    fp32 cellTopLeftX = (fp32) (cellCol * cellWidth);
    fp32 cellTopLeftY = (fp32) (cellRow * cellHeight);
    const mvCvPoint2D32f* cellFeaturesIn =
            &frameFeaturesIn[cellIdx * cellMaxFeatures];
    const FeatureMetadata* cellFeatMetaIn =
            &featureMetadataIn[cellIdx * cellMaxFeatures];
    const fp32* cellFeatErrorsIn =
            &featureErrorIn[cellIdx * cellMaxFeatures];

    mvCvPoint2D32f* cellFeaturesOut =
            &frameFeaturesOut[cellIdx * cellMaxFeatures];
    FeatureMetadata* cellFeatMetaOut =
            &featureMetadataOut[cellIdx * cellMaxFeatures];
    fp32* cellFeatErrorsOut = &featureErrorOut[cellIdx * cellMaxFeatures];
    for (u32 i = 0; i < featureCountIn[cellIdx]; i++)
    {
        // TODO: here we should call threshold filter for the whole array

        if (filterFeatures && (cellFeatErrorsIn[i] > thresholds->lostFeatureTreshold))// + 120265512000)
        {
            continue;
        }
        assert(featureCountOut[cellIdx] <= cellMaxFeatures && "CollectCellFeatures: Cell has too many features\n");

        fp32 x = cellFeaturesIn[i].x;
        fp32 y = cellFeaturesIn[i].y;

        if (x < 0 || y < 0)
            continue;
        if ((cellTopLeftX <= x && x < cellTopLeftX + cellWidth) &&
            (cellTopLeftY <= y && y < cellTopLeftY + cellHeight))
        {
            if (featureCountOut[cellIdx] == cellMaxFeatures)
                continue;
            if(filterFeatures) {
                fp32 harrisScore = harrisResponse(frame->p1 + PADDING_H,
                                                  (u32) (x + 0.5),
                                                  (u32) (y + 0.5),
                                                  frame->spec.stride,
                                                  0.04);

                if (harrisScore <= thresholds->minimumHarrisResponseOldFeatures)
                {
                    continue;
                }
            }
            /// This is a good feature. Copy it to the output cell
            insertFeature(cellFeaturesOut,
                          cellFeatErrorsOut,
                          cellFeatMetaOut,
                          &featureCountOut[cellIdx],
                          &cellFeaturesIn[i],
                          &cellFeatErrorsIn[i],
                          &cellFeatMetaIn[i],
                          (filterFeatures) ? thresholds->minimumFeatureDist : 0,
                          cellMaxFeatures);
            assert(featureCountOut[cellIdx] <= cellMaxFeatures && "CollectCellFeatures: Too many features found in cell\n");
        }
        else
        {
            // This feature should be in another cell. Move it to the right one
            u32 newCellId = SelectCellGridIndex(&cellFeaturesIn[i], cellWidth, cellHeight, cellGridDimension);
            if (newCellId >= nCells)
            {
                // the point slipped out of the image. Note that newCellId is unsigned, so this will drop any feature which is outside
                continue;
            }
            if (featureCountOut[newCellId] < cellMaxFeatures)
            {
                u32 newLocation = cellMaxFeatures * newCellId;
                if(filterFeatures) {
                    fp32 harrisScore = harrisResponse(frame->p1 + PADDING_H,
                            (u32) (x + 0.5),
                            (u32) (y + 0.5),
                            frame->spec.stride, 0.04);

                    if (harrisScore <= thresholds->minimumHarrisResponseOldFeatures)
                    {
                        continue;
                    }
                }


                insertFeature(&frameFeaturesOut[newLocation],
                        &featureErrorOut[newLocation],
                        &featureMetadataOut[newLocation],
                        &featureCountOut[newCellId],
                        &cellFeaturesIn[i],
                        &cellFeatErrorsIn[i],
                        &cellFeatMetaIn[i],
                        (filterFeatures) ? thresholds->minimumFeatureDist : 0,
                        cellMaxFeatures);

            }
            else
            {
                // No more places in the cell. Will drop this feature :(
            }
        }
    }
}


void featureMaintenance(FMSetupCfg* algConfig,
                        const u32 frameNum,
                        const frameBuffer* const fullFrame,
                        const tvXYLoc featuresTmp[],
                        const fp32 featuresErrorTmp[],
                        FMFeatureThresholds_t* thresholds)
{
    featureMaintenanceSetup(algConfig);
    u32 cellGridDimension = algConfig->cellGridDimension;
    u32 nCells = cellGridDimension*cellGridDimension;
    u32 targetNumFeatures = algConfig->targetNumFeatures;
    u32 maxNumFeatures = algConfig->maxNumFeatures;
    u32 cellMaxFeatures = maxNumFeatures/nCells;
    u32 cellCol, cellRow;
    // Search for candidate features in the specified cell
    u32 cellWidth = fullFrame->spec.width / cellGridDimension;
    u32 cellHeight = fullFrame->spec.height / cellGridDimension;
    // Need the frame number for double buffering to work
    g_framesNum = frameNum;
    // Reset output feature counters. Increment them when adding features
    memset(NEXT(g_cellFeatureCount), 0x00,
           sizeof(NEXT(g_cellFeatureCount)[0] ) * nCells);

    u32 cellIdx;

    if(mode != FM_PP_ONLY) {
        // Tracked points
        for (cellCol = 0, cellRow = 0; cellRow < cellGridDimension;)
        {
            cellIdx = cellRow * cellGridDimension + cellCol;

            // Fill the cell buffers with features
            collectTrackedFeatures(fullFrame,
                    cellWidth,
                    cellHeight,
                    cellCol,
                    cellRow,
                    CURR(g_cellFeatureCount),
                    NEXT(g_cellFeatureCount),
                    cellGridDimension,
                    nCells,
                    cellMaxFeatures,
                    (mvCvPoint2D32f*)featuresTmp,
                    featuresErrorTmp,
                    CURR(g_featureMetadata),
                    NEXT(g_features),
                    NEXT(g_featureError),
                    NEXT(g_featureMetadata),
                    thresholds);
            if (++cellCol >= cellGridDimension)
            {
                cellRow++;
                cellCol = 0;
            }

        }
    }

    if(mode != FM_TRACKED_ONLY) {
        // PP points
        u32 currentCnt;
        NEXT(g_featuresCount) = 0;
        for (cellIdx = 0 ; cellIdx < nCells; cellIdx++)
        {
            mvCvPoint2D32f* cellFeaturesOut = &NEXT(g_features)[cellIdx
                * cellMaxFeatures];
            FeatureMetadata* cellFeatMetaOut = &NEXT(g_featureMetadata)[cellIdx
                * cellMaxFeatures];
            fp32* cellFeatErrorsOut = &NEXT(g_featureError)[cellIdx * cellMaxFeatures];
            currentCnt = NEXT(g_cellFeatureCount)[cellIdx];
            u32 candIdx;
            const tvFeatureCell* currCand = &CURR(g_featureCells)[cellIdx];
            u32 cellTargetFeatures = targetNumFeatures/nCells;
            for (candIdx = 0; currentCnt < cellTargetFeatures &&
                    candIdx < currCand->featureCount;)
            {
                mvCvPoint2D32f candFeat;
                FeatureMetadata candFeatMeta;
                fp32 candFeatError;

                candFeat.x = currCand->features[candIdx].x;
                candFeat.y = currCand->features[candIdx].y;
                candFeatMeta.harris_score = currCand->features[candIdx].harris_score;
                candFeatMeta.age = 0;
                // TODO: This can be a simple counter. But it's ok to have it for debug right now
                candFeatMeta.id = (g_featureIdCounter[cellIdx] <<
                        MAX_FEAT_ID_DISTINGUISHER_OFFSET) |
                    cellIdx;
                candFeatError = 69;
                s32 stat = insertFeature(cellFeaturesOut,
                        cellFeatErrorsOut,
                        cellFeatMetaOut,
                        &currentCnt,
                        &candFeat,
                        &candFeatError,
                        &candFeatMeta,
                        (filterFeatures) ? thresholds->minimumFeatureDist : 0,
                        cellMaxFeatures);
                // If we inserted the feature, the increment the id
                // we can reuse it otherwise
                if (stat == INSERTION_SUCCESS)
                    g_featureIdCounter[cellIdx]++;
                candIdx++;
            }
            assert(currentCnt <= cellMaxFeatures);
            NEXT(g_cellFeatureCount)[cellIdx] = currentCnt;
            NEXT(g_featuresCount) += currentCnt;
            if (currentCnt < cellMaxFeatures)
            {
                // Clear output buffers so there won't be any old data remaining
                u32 gaps = cellMaxFeatures - currentCnt;
                memset(&cellFeaturesOut[currentCnt], 0x00, gaps * sizeof(mvCvPoint2D32f));
                memset(&cellFeatMetaOut[currentCnt], 0x00,
                        gaps * sizeof(FeatureMetadata));
                memset(&cellFeatErrorsOut[currentCnt], 0x00, gaps * sizeof(fp32));
            }
        }
    }
    SHAVE_HALT;
}

void featureMaintenanceSetup(FMSetupCfg* cfg)
{
    // Use a static set of pointers that are initialised at the beginning
    // of the program with various addresses that don't change between calls
    // to featureMaintenance();
    mode = cfg->mode;
    filterFeatures = cfg->filterFeatures;
    g_featuresCount = cfg->featuresCount;
    g_featureCells = cfg->featureCells;
    g_featureIdCounter = cfg->featureIdCounter;
    g_features = (mvCvPoint2D32f**) cfg->features;
    g_featureMetadata = cfg->featuresMetadata;
    g_featureError = cfg->featuresError;
    g_cellFeatureCount = cfg->cellFeatureCount;
}
