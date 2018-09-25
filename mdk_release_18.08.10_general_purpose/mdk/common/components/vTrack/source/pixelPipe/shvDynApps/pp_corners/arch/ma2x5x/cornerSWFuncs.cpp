#include "mv_types.h"
#include "assert.h"
#include "pixelPipeHelper.h"
#include "pixelPipeCornerShaveHelper.h"
#include "thresholdFilter.h"
#include "nonMax3x3_fp32.h"
#include "cornerSWFuncs.h"

void (*insert)(tvFeatureCell*, u32, u32, fp32 , u32, fp32&);

static void nonMaxFilter(u16* candPos, u32 candCount,
                         fp32* corners1,
                         fp32* corners2,
                         fp32* corners3,
                         u32 lineNo,
                         tvFeatureCell* featureData,
                        u32 cellGridDimension, u32 maxFeatPerCell,
                        fp32& threshold)
{
    u32 candOutCount;
    u32 i;
    u16 candOutPos[MAX_FRAME_WIDTH / cellGridDimension];
    mvcvNonMax3x3_fp32_asm(candPos, candCount, corners1, corners2, corners3,
                           candOutPos, &candOutCount);
    for (i = 0; i < candOutCount; i++)
    {
        u32 pozX = candOutPos[i];
        insert(featureData, pozX, lineNo, corners2[pozX], maxFeatPerCell, threshold);
    }
}

void cornerFilter( void* params)
{
    cornerParameters_t* cornerParameters = (cornerParameters_t *) params;
    if(cornerParameters->insertAndSort) {
        insert = insertThreshAndSort;
    } else {
        insert = insertThreshOnly;
    }
    u32 cellGridDimension = cornerParameters->cellGridDimension;
    u32 maxNumFeatures = cornerParameters->maxNumFeatures;
    HARRIS_OUTPUT_TYPE* corners1 = cornerParameters->corners1;
    HARRIS_OUTPUT_TYPE* corners2 = cornerParameters->corners2;
    HARRIS_OUTPUT_TYPE* corners3 = cornerParameters->corners3;
    u32 cellWidth = cornerParameters->width / cellGridDimension;
    u32 cellHeight = cornerParameters->height / cellGridDimension;
    u32 lineNo    = cornerParameters->lineNo;
    u32 cellIdx = lineNo / cellHeight;
    u32 cellRowOffset = cellIdx * cellGridDimension;
    u32 maxFeatPerCell = maxNumFeatures / (cellGridDimension * cellGridDimension);
    fp32* thresholds = cornerParameters->thresholds;
    tvFeatureCell* outCellData = cornerParameters->outCellData;
    assert(cellIdx < cellGridDimension);

    for (u32 cell = 0; cell < cellGridDimension; cell++)
    {
        u16 featureCand[MAX_FRAME_WIDTH / cellGridDimension];
        u32 candCount = 0;
        mvcvThresholdFilter_asm(
                        corners2,
                        thresholds[cellRowOffset + cell],
                        cellWidth,
                        cell * cellWidth, featureCand, &candCount);
        if (candCount > 0)
        {
            nonMaxFilter(featureCand,
                         candCount,
                         corners1,
                         corners2,
                         corners3,
                         lineNo,
                         &outCellData[cellRowOffset + cell], cellGridDimension, maxFeatPerCell,
                         thresholds[cellRowOffset + cell]);
        }
    }
}

