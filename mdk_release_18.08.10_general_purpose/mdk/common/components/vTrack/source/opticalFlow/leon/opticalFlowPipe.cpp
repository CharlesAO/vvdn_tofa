#include "mv_types.h"
#include "vTrack.h"
#include "swcFrameTypes.h"
#if defined(MA2150) || defined(MA2450)
#include "OsDrvShaveL2Cache.h"
#else
#include "OsDrvShaveL2c.h"
#endif
#include "OsDrvSvu.h"

#include "vTrackPrivate.h"
#include "opticalFlowPrivate.h"
#include "opticalFlowApi.h"
#include <pthread.h>
#include <rtems.h>
#include "mvMacros.h"

extern DynamicContext_t MODULE_DATA(of);

OpticalFlow::OpticalFlow(){
    ofContextInstanceDataPtr = &ofContextInstanceData;
}


void
OpticalFlow::ofInit(/*tvPyramidBuffer* pyramid,*/
        tvOpticalFlowCfg* algConfig,
        ofResourceCfg_t* resCfg) {
    this->ofAlgConfig = *algConfig;
    if(algConfig->algoType == OF_MOVI || algConfig->algoType == OF_OPENCV)
    {
        numShaves = resCfg->numShaves;
        assert(numShaves<=MAX_SHAVES_OF);
        assert(algConfig->algoType != OF_OPENCV); //not supported on myriad
        assert(algConfig->subpixelDetection == 0); //not supported on myriad
        ofAlgConfig = *algConfig;
        shaveList = resCfg->shaveList;
        cacheData = resCfg->cachePartData;
        cacheInstr = resCfg->cachePartInstr;
        ofContext = MODULE_DATA(of);
        ofContext.instancesData = ofContextInstanceDataPtr;
        *ofContext.instancesData = *MODULE_DATA(of).instancesData;
    }
#if defined(MA2480)
    else if(algConfig->algoType == OF_MEST)
    {
        mestRes.coordsHeapSize = MEST_GET_COORDS_HEAP_BYTES(algConfig->maxNumFeatures);
        mestRes.coordsHeapStartAddress = (uint32_t*) MEST_COORDS_HEAP;
        mestRes.resultsTmpHeapSize = MEST_GET_RES_HEAP_BYTES(algConfig->maxNumFeatures);
        mestRes.resultsTmpHeapStartAddress = (uint32_t*) MEST_RES_TMP_HEAP;
        mestMutexInit();
        // This 'mestInitStatus' flag will tell the status of the running:
        // - the first frame and only frame
        // - both frames present, the only ROI must be added
        // - the ROI must be updated
        mestInitStatus = 0;
    }
#endif
    else
    {
        assert(0 && "Invalid optical flow configuration!"); // Crash the app
    }
}

OpticalFlow::~OpticalFlow() {
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
    assert(numCells <= MAX_OF_TASKS);
    u32 i;
    u32 cell;
    u32 running;
    int status = 0;

    if(this->ofAlgConfig.algoType == OF_MOVI)
    {
    #if defined(MA2150) || defined(MA2450)
        OsDrvShaveL2CachePartitionInvalidate(cacheData);
    #else
        OsDrvShaveL2cFlushInvPart(cacheData, HGL_SHAVE_L2C_INV);
    #endif
        for (i = 0; i < numShaves; i++)
        {
            swcShaveUnit_t shaveNo = shaveList[i];
            localTurnOnShave (shaveNo);
    #if defined(MA2150) || defined(MA2450)
            OsDrvShaveL2CSetWindowPartition((shaveId_t)shaveNo,
                                            SHAVEL2CACHEWIN_C, cacheData);
            OsDrvShaveL2CSetNonWindowedPartition((shaveId_t)shaveNo,
                                                 cacheData, NON_WINDOWED_DATA_PARTITION);
            OsDrvShaveL2CSetNonWindowedPartition((shaveId_t)shaveNo, cacheInstr, NON_WINDOWED_INSTRUCTIONS_PARTITION);
    #else
            OsDrvShaveL2cAssignPart(shaveNo, cacheData, OS_DRV_SHAVE_L2C_WIN_PART, OS_DRV_SHAVE_L2C_WIN_C);
            OsDrvShaveL2cAssignPart(shaveNo, cacheData, OS_DRV_SHAVE_L2C_NON_WIN_DATA_PART, OS_DRV_SHAVE_L2C_WIN_A);
            OsDrvShaveL2cAssignPart(shaveNo, cacheInstr, OS_DRV_SHAVE_L2C_NON_WIN_INST_PART, OS_DRV_SHAVE_L2C_WIN_A);
    #endif

        }
        status += OsDrvSvuSetupDynShaveApps(&ofContext, shaveList, numShaves);
        status += OsDrvSvuOpenShaves(shaveList, numShaves, OS_MYR_PROTECTION_SELF);

        cell = 0;
        while(cell < numCells) {
            u32 shaveNo;
            uint32_t tasksStarted = 0;
            u32 j;
            for(j=0; j<numShaves; j++) {
                shaveNo = shaveList[j];
                // Start a new optical flow shave
                status += OsDrvSvuRunShaveAlgoOnAssignedShaveCC(&ofContext, shaveNo,
                        "iiiiiii",
                        (u32) &ofAlgConfig,
                        (u32) pyrImgPrev,
                        (u32) pyrImgCur,
                        (u32) &featuresPrev [cell * maxFeatPerCell],
                        (u32) &featuresCur  [cell * maxFeatPerCell],
                        (u32) &featuresError[cell * maxFeatPerCell],
                        featuresCount[cell]);

                tasksStarted++;
                cell++;
                if (cell >= numCells)
                    break;
            }
            status += OsDrvSvuDynWaitShaves(shaveList, tasksStarted, OS_DRV_SVU_WAIT_FOREVER, &running);
        }
        #if defined(MA2150) || defined(MA2450)
            OsDrvShaveL2CachePartitionFlush(cacheData, PERFORM_INVALIDATION);
        #else
            OsDrvShaveL2cFlushInvPart(cacheData, HGL_SHAVE_L2C_FLUSH_INV);
        #endif

        status += OsDrvSvuCloseShaves(shaveList, numShaves);
        status += OsDrvSvuCleanupDynShaveApps(&ofContext);
        rtems_cache_invalidate_data_range(&featuresError[0], maxFeatPerCell * numCells * sizeof(fp32));
        rtems_cache_invalidate_data_range(&featuresCur[0],   maxFeatPerCell * numCells * sizeof(fp32));
        // Turn off all shaves
        for (i = 0; i < numShaves; i++)
        {
            u32 shaveNo = shaveList[i];
            localTurnOffShave (shaveNo);
        }
    }
#if defined(MA2480)
    else if(this->ofAlgConfig.algoType == OF_MEST)
    {
        const frameBuffer prevBuf = pyrImgPrev->pyrLevel[0];
        const frameBuffer currBuf = pyrImgCur->pyrLevel[0];
        uint32_t featuresCnt = featuresCount[0];
        mestError_t mestStatus = MEST_ROI_SUCCESS;

        const int halfBlockSize = 4;
        uint32_t j = 0;
        // Shift points for MEST and remove 'broken'(negative) points
        for(uint32_t i = 0; i < featuresCnt; i++)
            if(featuresPrev[i].x >= halfBlockSize && featuresPrev[i].y >= halfBlockSize)
            {
                featuresPrev[j].x = featuresPrev[i].x - halfBlockSize;
                featuresPrev[j].y = featuresPrev[i].y - halfBlockSize;
                j++;
            }
        featuresCnt = j;
        featuresCount[0] = featuresCnt;

        if(mestInitStatus == 0)
        {
            mestRoi.topLeft.x = 0;
            mestRoi.topLeft.y = 0;
            mestRoi.bottomRight.x = currBuf.spec.width - 1;
            mestRoi.bottomRight.y = currBuf.spec.height - 1;
            this->mestStaticConfig.spec = currBuf.spec;
            this->mestStaticConfig.spec.stride =
                    this->mestStaticConfig.spec.width *
                    this->mestStaticConfig.spec.bytesPP;
            if(mestInit(&mestHandler, MEST_MODE_NORMAL, &mestRes, &this->mestStaticConfig)
                    != MEST_SUCCESS)
            {
                puts("MEST buffers initialize error!");
                return -1;
            }
            this->mestRuntimeConfig = mestGetRuntimeConfig(&mestHandler);
            // Use the default values in this test
            mestSetRuntimeConfig(&mestHandler, &this->mestRuntimeConfig);
            mestId = mestROIgetNextId(&mestHandler);
            mestInitStatus = 1;
        }
        if(!featuresCnt)
            return status;

        if(mestInitStatus == 1)
        {
            mestStatus = mestAddROI(&mestHandler,
                                    &mestRoi,
                                    (xyPos_t*) featuresPrev,
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
                                               (xyPos_t*) featuresPrev,
                                               featuresCnt);
        }

        if(mestStatus != MEST_ROI_SUCCESS)
            return status;

        mestBulkOutput_t mestBulk;
        mestResults_t mestResult;

        mestBulk.address = MEST_RES_HEAP;
        mestBulk.size = mestGetResultSize(&mestHandler);

        mestRun(&mestHandler, prevBuf, currBuf, &mestBulk);

        mestParseOutput(mestBulk.address, &mestResult);

        for(uint32_t i = 0; i < featuresCnt; i++)
        {
            featuresPrev[i].x += halfBlockSize;
            featuresPrev[i].y += halfBlockSize;
        }

        for(uint32_t i = 0; i < featuresCnt; i++)
        {
            featuresCur[i].x = featuresPrev[i].x + mestResult.meta.outputFlows[i].x;
            featuresCur[i].y = featuresPrev[i].y + mestResult.meta.outputFlows[i].y;
            featuresError[i] = mestResult.meta.costs[i];
        }
    }
#endif

    return status;
}
