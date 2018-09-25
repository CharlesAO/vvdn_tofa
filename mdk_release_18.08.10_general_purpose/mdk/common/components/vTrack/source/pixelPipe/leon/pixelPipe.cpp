///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     pixelPipeline for vtrack project
///

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <semaphore.h>
#include <rtems.h>

#include "mv_types.h"
#include "mvMacros.h"
#if defined(MA2150) || defined(MA2450)
#include "OsDrvShaveL2Cache.h"
#else
#include "OsDrvShaveL2c.h"
#endif
#include "OsDrvSvuDefines.h"
#include "OsDrvSvu.h"
#include "fifoCommInitApi.h"

#include "vTrackPrivate.h"
#include "PixelPipeApi.h"
#include "pixelPipeHelper.h"

extern DynamicContext_t MODULE_DATA(ppm);
extern DynamicContext_t MODULE_DATA(pp_corners);
extern DynamicContext_t MODULE_DATA(pp_gauss);

PixelPipe::PixelPipe() {
    pixelPipeParams = (pixelPipeParams_t*) ALIGN_UP(&shaveBuf[0], 64);
        pixelPipeParams->isInitialised = false;

    ppContextInstanceDataPtr = &ppContextInstanceData;
    corContextInstanceDataPtr = &corContextInstanceData;
    gaussContextInstanceDataPtr = &gaussContextInstanceData;
}
extern const u32 ppmX_fifoCommMasterRun;
extern const u32 pp_gaussX_PyrLevel;
extern const u32 pp_gaussX_gaussInit;
#if defined(MA2150) || defined(MA2450)
extern const u32 pp_cornersX_cornerFilter;
#endif
sem_t ppSem;
int unused = sem_init(&ppSem, 0, 1);

void
PixelPipe::initPixelPipe(t_pPipeResourceCfg* ppResource, pyramidAlgoType_t pyrAlg, cornerConfig_t corCfg)
{
    t_pPipeShaveConfig* pp_shaveCfg;

    pp_shaveCfg = ppResource->shaveConfig;

    cornerShavesList = pp_shaveCfg->cornerShaveList;
    gaussShavesList  = pp_shaveCfg->gaussShaveList;

    ppMasterShaveNum = *pp_shaveCfg->ppShaveNum;

    gaussNumShaves   = pp_shaveCfg->gaussNoShaves;
    cornerNumShaves  = pp_shaveCfg->cornerNoShaves;
    slaveNumShaves = gaussNumShaves + cornerNumShaves;

    pixelPipeParams->svuBuff = (u8*)ppResource->ppBuffs;
    pixelPipeParams->svuBuffSize = ppResource->ppBuffsSize;
    pixelPipeParams->dmaBuff = (u8*)ppResource->ppCmxBuffs;
    pixelPipeParams->dmaBuffSize = ppResource->ppCmxBuffsSize;
    pixelPipeParams->svuBuff = (u8*) ALIGN_UP((u32)pixelPipeParams->svuBuff,64);
    ppCacheData = ppResource->ppCacheData;
    ppCacheInstr = ppResource->ppCacheInstr;
    pixelPipeParams->pyrAlg = pyrAlg;
    pixelPipeParams->corCfg = corCfg;

    pixelPipeParams->isInitialised = false;

    if(slaveNumShaves) { // allocate and init for FIFO stuff
        masterHandler = (fifoCommMasterHandler_t*) pixelPipeParams->svuBuff;

        pixelPipeParams->svuBuff += ALIGN_UP(sizeof(fifoCommMasterHandler_t), 64);

        slaveHandler = (fifoCommSlaveHandler_t*) pixelPipeParams->svuBuff;
        pixelPipeParams->svuBuff += ALIGN_UP((sizeof(fifoCommSlaveHandler_t) * slaveNumShaves), 64);

        shaveTaskTypes = (fifoCommTask_t*) pixelPipeParams->svuBuff;
        pixelPipeParams->svuBuff += ALIGN_UP((sizeof(fifoCommTask_t) * slaveNumShaves),64);

        taskTypes = (fifoCommTask_t*) pixelPipeParams->svuBuff;
        int numTaskTypes = 0; //incremented below

        t_ppFifoCfg* fifoCfg;
        fifoCfg = &(ppResource->fifoCfg);

        assert(slaveNumShaves <= NUM_SHAVES_SLAVE);

        fifoCommMasterInit(masterHandler, (fifoCommMasterCallback_t)(&ppmX_fifoCommMasterRun));
        u32 shaveNum = 0;
        pixelPipeParams->gaussTask = NULL;
        pixelPipeParams->cornerTask = NULL; //these are null unless we have slave shaves for them
        if(gaussNumShaves) {
            pixelPipeParams->svuBuff += ALIGN_UP(sizeof(fifoCommTask_t), 64);
            memcpy(slaveShaves, gaussShavesList, gaussNumShaves * sizeof(swcShaveUnit_t));
            fifoCommMasterRegisterTaskType(masterHandler, &taskTypes[numTaskTypes],
                    fifoCfg->fifo1,fifoCfg->fifo2);
            for(; shaveNum < gaussNumShaves; shaveNum++)
            {
                fifoCommSlaveInit(&slaveHandler[shaveNum]);
                fifoCommSlaveRegisterTaskType(&slaveHandler[shaveNum], &shaveTaskTypes[shaveNum],
                        &taskTypes[numTaskTypes], (fifoCommTaskCallback_t)(&pp_gaussX_gaussInit),
                        (fifoCommTaskCallback_t)(&pp_gaussX_PyrLevel));
            }
            pixelPipeParams->gaussTask = &taskTypes[numTaskTypes];
            numTaskTypes++;
        }
#if defined(MA2150) || defined(MA2450)
        if(cornerNumShaves) {
            pixelPipeParams->svuBuff += ALIGN_UP(sizeof(fifoCommTask_t), 64);
            memcpy(&slaveShaves[gaussNumShaves], cornerShavesList, cornerNumShaves * sizeof(swcShaveUnit_t));
            fifoCommMasterRegisterTaskType(masterHandler, &taskTypes[numTaskTypes],
                    fifoCfg->fifo3,fifoCfg->fifo4);
            for(; shaveNum <  slaveNumShaves; shaveNum++)
            {
                fifoCommSlaveInit(&slaveHandler[shaveNum]);
                fifoCommSlaveRegisterTaskType(&slaveHandler[shaveNum], &shaveTaskTypes[shaveNum],
                        &taskTypes[numTaskTypes], NULL,
                        (fifoCommTaskCallback_t)(&pp_cornersX_cornerFilter));

            }
            pixelPipeParams->cornerTask = &taskTypes[numTaskTypes];
            numTaskTypes++;
        }
#else
        assert(cornerNumShaves==0);
#endif
        //more is allocated from svuBuff later so need to reduce the size to what's available now
        pixelPipeParams->svuBuffSize -= ((u32)pixelPipeParams->svuBuff - (u32)ppResource->ppBuffs);
    }
    ppContext = MODULE_DATA(ppm);
    ppContext.instancesData = ppContextInstanceDataPtr;
    *ppContext.instancesData = *MODULE_DATA(ppm).instancesData;
    corContext = MODULE_DATA(pp_corners);
    corContext.instancesData = corContextInstanceDataPtr;
    *corContext.instancesData = *MODULE_DATA(pp_corners).instancesData;
    gaussContext = MODULE_DATA(pp_gauss);
    gaussContext.instancesData = gaussContextInstanceDataPtr;
    *gaussContext.instancesData = *MODULE_DATA(pp_gauss).instancesData;
}

u32
PixelPipe::pixelPipe(frameBuffer* in_img, tvFeatureCell** feature_cells,
        tvPyramidBuffer* pyramidBuffer,
        fp32* thresholds,
        u32 num_pyr_levels, u32 num_pyrs,
        u32 cellGridDimension, u32 maxNumFeatures,
        u32 targetNumFeatures, ppThresholds_t* thresholdCfg)
{
    u32 nCells=cellGridDimension*cellGridDimension;
    u32 running;
    u32 i;
    int status = 0;
    pixelPipeParams->feature_cells = feature_cells;
    pixelPipeParams->in_img = in_img;
    pixelPipeParams->num_pyr_levels = num_pyr_levels;
    pixelPipeParams->num_pyrs = num_pyrs;
    pixelPipeParams->thresholds = harrisThresholds;
    pixelPipeParams->pyramidBuffer = pyramidBuffer;
    pixelPipeParams->cellGridDimension = cellGridDimension;
    pixelPipeParams->maxNumFeatures = maxNumFeatures;
    for(i = 0; i < pixelPipeParams->num_pyrs; i++)
        UpdateCellThresholds(thresholds, pixelPipeParams->feature_cells[i], thresholdCfg, nCells, targetNumFeatures);
    for(i=0; i<nCells; i++) {
        // copy thresholds into harrisThresholds array. This might be scaled on the shave. We want to keep vPipe/pp on the leon  oblivious to this
        harrisThresholds[i] = thresholds[i];
    }
    sem_wait(&ppSem);
    //setup shaves

#if defined(MA2150) || defined(MA2450)
    if(cornerNumShaves) {
        status += OsDrvSvuSetupDynShaveApps(&corContext, cornerShavesList, cornerNumShaves);
    }
#endif
    if(gaussNumShaves) {
        status += OsDrvSvuSetupDynShaveApps(&gaussContext, gaussShavesList, gaussNumShaves);
    }
    if(slaveNumShaves) {
        //open shaves
        status += OsDrvSvuOpenShaves(slaveShaves, slaveNumShaves, OS_MYR_PROTECTION_SELF);
    }
    i=0;
    if(gaussNumShaves) {
        for(; i < gaussNumShaves; i++)
        {
            status += OsDrvSvuRunShaveAlgoOnAssignedShaveCC(&gaussContext, slaveShaves[i],
                    "i", (u32)&slaveHandler[i]);
        }
    }

#if defined(MA2150) || defined(MA2450)
    if(cornerNumShaves) {
        for(; i < slaveNumShaves; i++)
        {
            status += OsDrvSvuRunShaveAlgoOnAssignedShaveCC(&corContext, slaveShaves[i],
                    "i", (u32)&slaveHandler[i]);
        }
    }
#endif

    status += OsDrvSvuSetupDynShaveApps(&ppContext, &ppMasterShaveNum, 1);
    status += OsDrvSvuOpenShaves( &ppMasterShaveNum, 1, OS_MYR_PROTECTION_SELF);

    status += OsDrvSvuRunShaveAlgoOnAssignedShaveCC(&ppContext, ppMasterShaveNum, "ii",
            shaveBuf, sizeof(shaveBuf));


    status += OsDrvSvuDynWaitShaves(&ppMasterShaveNum, 1, OS_DRV_SVU_WAIT_FOREVER, &running);



    status += OsDrvSvuCloseShaves(&ppMasterShaveNum, 1);
    if(slaveNumShaves) {
        status += OsDrvSvuStopShaves(slaveShaves, slaveNumShaves);
        status += OsDrvSvuCloseShaves(slaveShaves, slaveNumShaves);
    }
#if defined(MA2150) || defined(MA2450)
    OsDrvShaveL2CachePartitionFlush(ppCacheData, PERFORM_INVALIDATION);
#else
    OsDrvShaveL2cFlushInvPart(ppCacheData, HGL_SHAVE_L2C_FLUSH_INV);
//    assert(sc == 0);
#endif
    //just kill the shaves. They are in a low power wait
    status += OsDrvSvuCleanupDynShaveApps(&ppContext);
    if(gaussNumShaves) {
        status += OsDrvSvuCleanupDynShaveApps(&gaussContext);
    }
#if defined(MA2150) || defined(MA2450)
    if(cornerNumShaves) {
        status += OsDrvSvuCleanupDynShaveApps(&corContext);
    }
#endif
    sem_post(&ppSem);

    return status;
}

//
// Public API
//
void
PixelPipe::ppInit(t_pPipeResourceCfg* vpResource, pyramidAlgoType_t pyrAlg, cornerConfig_t corCfg) {
    initPixelPipe(vpResource, pyrAlg, corCfg);
}


u32
PixelPipe::ppRun(frameBuffer* in_img, tvFeatureCell** feature_cells,
              tvPyramidBuffer* frmBuffer,
              fp32* thresholds,
              u32 num_pyr_levels, u32 num_pyrs,
              u32 cellGridDimension, u32 maxNumFeatures,
              u32 targetNumFeatures, ppThresholds_t* thresholdCfg) {
    u32 i;
    localTurnOnShave(ppMasterShaveNum);
#if defined(MA2150) || defined(MA2450)
    OsDrvShaveL2CachePartitionInvalidate(ppCacheData);
    OsDrvShaveL2CSetWindowPartition((shaveId_t)ppMasterShaveNum,
                                    SHAVEL2CACHEWIN_C, ppCacheData);
    OsDrvShaveL2CSetNonWindowedPartition((shaveId_t)ppMasterShaveNum,
                                         ppCacheData, NON_WINDOWED_DATA_PARTITION);
    OsDrvShaveL2CSetNonWindowedPartition((shaveId_t)ppMasterShaveNum, ppCacheInstr, NON_WINDOWED_INSTRUCTIONS_PARTITION);
#else
    int sc;
    sc = OsDrvShaveL2cFlushInvPart(ppCacheData, HGL_SHAVE_L2C_FLUSH_INV);
//    assert(sc == 0);
    sc = OsDrvShaveL2cAssignPart(ppMasterShaveNum, ppCacheData, OS_DRV_SHAVE_L2C_WIN_PART, OS_DRV_SHAVE_L2C_WIN_C);
//    assert(sc == 0);
    sc = OsDrvShaveL2cAssignPart(ppMasterShaveNum, ppCacheData, OS_DRV_SHAVE_L2C_NON_WIN_DATA_PART, OS_DRV_SHAVE_L2C_WIN_A);
//    assert(sc == 0);
    OsDrvShaveL2cAssignPart(ppMasterShaveNum, ppCacheInstr, OS_DRV_SHAVE_L2C_NON_WIN_INST_PART, OS_DRV_SHAVE_L2C_WIN_A);

    (void)sc;
#endif
    for(i = 0; i < slaveNumShaves; i++){
        localTurnOnShave(slaveShaves[i]);
#if defined(MA2150) || defined(MA2450)
        OsDrvShaveL2CSetWindowPartition((shaveId_t)slaveShaves[i],
                                        SHAVEL2CACHEWIN_C, ppCacheData);
        OsDrvShaveL2CSetNonWindowedPartition((shaveId_t)slaveShaves[i],
                                             ppCacheData, NON_WINDOWED_DATA_PARTITION);
        OsDrvShaveL2CSetNonWindowedPartition((shaveId_t)slaveShaves[i], ppCacheInstr, NON_WINDOWED_INSTRUCTIONS_PARTITION);
#else
        sc = OsDrvShaveL2cAssignPart(slaveShaves[i], ppCacheData, OS_DRV_SHAVE_L2C_WIN_PART, OS_DRV_SHAVE_L2C_WIN_C);
//        assert(sc == 0);

        sc = OsDrvShaveL2cAssignPart(slaveShaves[i], ppCacheData, OS_DRV_SHAVE_L2C_NON_WIN_DATA_PART, OS_DRV_SHAVE_L2C_WIN_A);
//        assert(sc == 0);
        OsDrvShaveL2cAssignPart(slaveShaves[i], ppCacheInstr, OS_DRV_SHAVE_L2C_NON_WIN_INST_PART, OS_DRV_SHAVE_L2C_WIN_A);

#endif

    }
    u32 ret = pixelPipe(in_img, feature_cells,
                        frmBuffer,
                        thresholds,
                        num_pyr_levels, num_pyrs,
                        cellGridDimension, maxNumFeatures, targetNumFeatures, thresholdCfg);
    localTurnOffShave(ppMasterShaveNum);
    for(i = 0; i < slaveNumShaves; i++)
        localTurnOffShave(slaveShaves[i]);
    u32 nCells = cellGridDimension * cellGridDimension;
    u32 maxFeatPerCell = maxNumFeatures/nCells;
    //invalidate current features
    for(i = 0; i<nCells; i++) {
        rtems_cache_invalidate_data_range(feature_cells[0][i].features, (maxFeatPerCell + 1)* sizeof(t_vTrackHarrisFeat));
    }
    //invalidate feature cointainer
    rtems_cache_invalidate_data_range(feature_cells[0], nCells * sizeof(tvFeatureCell));
    //invalidate shaveBuf (TODO:required??)
    uint8_t * actualShaveBufStart = ALIGN_UP(&shaveBuf[0], 64);
    rtems_cache_invalidate_data_range(actualShaveBufStart, ALIGN_DOWN(sizeof(shaveBuf) - (actualShaveBufStart - &shaveBuf[0]),64));

    //invalidate pyramids
    for(i=0; i<num_pyr_levels;i++) {
        frameBuffer* my_frame = &frmBuffer[0].pyrLevel[i];
        rtems_cache_invalidate_data_range(my_frame->p1 - (PADDING_V * my_frame->spec.stride),
            ((my_frame->spec.height + 2 * PADDING_V) * (my_frame->spec.stride + (2 * PADDING_H))));
    }
    return ret;
}
