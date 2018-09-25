#include "vTrack.h"
#include "opticalFlowPrivate.h"
#include "opticalFlowApi.h"

#include "MestApi.h"
#include <cstdio>
#include <assert.h>

static mestHandler_t mestHandler;
static mestResourceConfig_t mestRes;
static mestROIId_t mestId;
static mestROI_t mestRoi;
static mestParamConfig_t mestStaticConfig;
static mestRuntimeConfig_t mestRuntimeConfig;
static int mestInitStatus;

OpticalFlow::OpticalFlow() {}
OpticalFlow::~OpticalFlow() {}

void
OpticalFlow::ofInit(tvOpticalFlowCfg* algConfig,
        ofResourceCfg_t* resCfg) {
    (void) resCfg;
    ofAlgConfig = *algConfig;
    if(algConfig->algoType == OF_MEST) {
        mestRes.coordsHeapSize = MEST_GET_COORDS_HEAP_BYTES(algConfig->maxNumFeatures);
        mestRes.coordsHeapStartAddress = (uint32_t*) MEST_COORDS_HEAP;
        mestRes.resultsTmpHeapSize = MEST_GET_RES_HEAP_BYTES(algConfig->maxNumFeatures);
        mestRes.resultsTmpHeapStartAddress = (uint32_t*) MEST_RES_TMP_HEAP;

        mestMutexInit();
        mestInitStatus = 0;
//        mestHandler.runtimeConfig.filteringPolicy = MEST_MAXIMIZE_INLIER_COUNT;
//        mestHandler.runtimeConfig.minCostThreshold = 0;
//        mestHandler.runtimeConfig.maxCostThreshold = 200;

    }
}
int
OpticalFlow::ofRun(tvPyramidBuffer* pyrImgPrev,
        tvPyramidBuffer* pyrImgCur,
        tvXYLoc featuresPrev[],
        tvXYLoc featuresCur[],
        fp32 featuresError[],
        u32 featuresCount[],
        u32 numCells,
        u32 maxFeatPerCell) {
    u32 i;

    for (i = 0; i < numCells; i++)
    {
        u32 idx = i*maxFeatPerCell;
        if(ofAlgConfig.algoType != OF_MEST) {

            opticalFlowRun(&ofAlgConfig, pyrImgPrev, pyrImgCur,
                    &featuresPrev[idx],
                    &featuresCur[idx],
                    &featuresError[idx],
                    featuresCount[i]);

        } 
        else {
            tvXYLoc* mestFeaturesPrev = &featuresPrev[idx];
            tvXYLoc* mestFeaturesCur = &featuresCur[idx];
            fp32* mestFeaturesError = &featuresError[idx];
            u32 mestFeaturesCount = featuresCount[i];

            const int halfBlockSize = 4;
            const frameBuffer prevBuf = pyrImgPrev->pyrLevel[0];
            const frameBuffer currBuf = pyrImgCur->pyrLevel[0];

            uint32_t featuresCnt = mestFeaturesCount;
            uint32_t j = 0;
            mestError_t mestStatus = MEST_ROI_SUCCESS;
            // Shift points for MEST and remove 'broken'(negative) points
            for(uint32_t i = 0; i < featuresCnt; i++)
                if(mestFeaturesPrev[i].x >= halfBlockSize && mestFeaturesPrev[i].y >= halfBlockSize)
                {
                    mestFeaturesPrev[j].x = mestFeaturesPrev[i].x - halfBlockSize;
                    mestFeaturesPrev[j].y = mestFeaturesPrev[i].y - halfBlockSize;
                    j++;
                }
            featuresCnt = j;

            if(mestInitStatus == 0)
            {
                mestRoi.topLeft.x = 0;
                mestRoi.topLeft.y = 0;
                mestRoi.bottomRight.x = currBuf.spec.width - 1;
                mestRoi.bottomRight.y = currBuf.spec.height - 1;
                mestStaticConfig.spec = currBuf.spec;

                if(mestInit(&mestHandler, MEST_MODE_NORMAL, &mestRes, &mestStaticConfig) != MEST_SUCCESS)
                {
                    puts("MEST buffers initialize error!");
                    assert(0);
                }
                mestRuntimeConfig = mestGetRuntimeConfig(&mestHandler);
                // Use the default values in this test
                mestSetRuntimeConfig(&mestHandler, &mestRuntimeConfig);
                mestId = mestROIgetNextId(&mestHandler);
                mestInitStatus = 1;
            }
            if(!featuresCnt)
                return 0;

            if(mestInitStatus == 1)
            {
                mestStatus = mestAddROI(&mestHandler,
                        &mestRoi,
                        (xyPos_t*) mestFeaturesPrev,
                        featuresCnt,
                        MEST_USER_DEFINED,
                        mestId);
                if(mestStatus == MEST_ROI_SUCCESS)
                    mestInitStatus = 2;
            }
            else
            {
                mestStatus = mestUpdateROIFeatures(&mestHandler,
                        mestId,
                        (xyPos_t*) mestFeaturesPrev,
                        featuresCnt);
            }

            if(mestStatus != MEST_ROI_SUCCESS)
                return -1;

            mestBulkOutput_t mestResult;
            mestResults_t mestResultFinal;

            mestResult.address = MEST_RES_HEAP;
            mestResult.size = mestGetResultSize(&mestHandler);

            mestRun(&mestHandler, prevBuf, currBuf, &mestResult);

            mestParseOutput(mestResult.address, &mestResultFinal);

            for(uint32_t i = 0; i < featuresCnt; i++)
            {
                mestFeaturesPrev[i].x += halfBlockSize;
                mestFeaturesPrev[i].y += halfBlockSize;
            }

            for(uint32_t i = 0; i < featuresCnt; i++)
            {
                mestFeaturesCur[i].x = mestFeaturesPrev[i].x + mestResultFinal.meta.outputFlows[i].x;
                mestFeaturesCur[i].y = mestFeaturesPrev[i].y + mestResultFinal.meta.outputFlows[i].y;
                mestFeaturesError[i] = mestResultFinal.meta.costs[i];
            }

        }
    }
    return 0;
}

