#include "mv_types.h"
#include "featureMaintenancePrivate.h"
#include "swcFrameTypes.h"
#if defined(MA2150) || defined(MA2450)
#include "OsDrvShaveL2Cache.h"
#else
#include "OsDrvShaveL2c.h"
#endif
#include "OsDrvSvu.h"

#include "vTrackPrivate.h"
#include "featureMaintenanceApi.h"

extern DynamicContext_t MODULE_DATA(fm);

FeatMaintenance::FeatMaintenance(){
    fmContextInstanceDataPtr = &fmContextInstanceData;
}

FeatMaintenance::~FeatMaintenance() {
}


void
FeatMaintenance::fmRun(const u32 frameNum,
        const frameBuffer* const fullFrame,
        const tvXYLoc featuresTmp[],
        const fp32 featuresErrorTmp[],
        FMFeatureThresholds_t* thresholds) {
    u32 running;
    localTurnOnShave (fmShaveNum);
#if defined(MA2150) || defined(MA2450)
    OsDrvShaveL2CachePartitionInvalidate(featPipeCacheData);

    OsDrvShaveL2CSetWindowPartition((shaveId_t)fmShaveNum,
                                    SHAVEL2CACHEWIN_C, featPipeCacheData);
    OsDrvShaveL2CSetNonWindowedPartition((shaveId_t)fmShaveNum,
                                         featPipeCacheData, NON_WINDOWED_DATA_PARTITION);
    OsDrvShaveL2CSetNonWindowedPartition((shaveId_t)fmShaveNum, featPipeCacheInstr, NON_WINDOWED_INSTRUCTIONS_PARTITION);
    #else
    OsDrvShaveL2cFlushInvPart(featPipeCacheData, HGL_SHAVE_L2C_INV);
    OsDrvShaveL2cAssignPart(fmShaveNum, featPipeCacheData, OS_DRV_SHAVE_L2C_WIN_PART, OS_DRV_SHAVE_L2C_WIN_C);
    OsDrvShaveL2cAssignPart(fmShaveNum, featPipeCacheData, OS_DRV_SHAVE_L2C_NON_WIN_DATA_PART, OS_DRV_SHAVE_L2C_WIN_A);
    OsDrvShaveL2cAssignPart(fmShaveNum, featPipeCacheInstr, OS_DRV_SHAVE_L2C_NON_WIN_INST_PART, OS_DRV_SHAVE_L2C_WIN_A);
#endif
    int status = 0;
    status += OsDrvSvuSetupDynShaveApps(&fmContext, &fmShaveNum, 1);
    status += OsDrvSvuOpenShaves(&fmShaveNum, 1, OS_MYR_PROTECTION_SELF);
    OsDrvSvuRunShaveAlgoOnAssignedShaveCC(&fmContext, fmShaveNum,
            "iiiiii",
            (u32) &g_fmCfg,
            frameNum,
            (u32) fullFrame,
            (u32) featuresTmp,
            (u32) featuresErrorTmp,
            (u32) thresholds);
    OsDrvSvuDynWaitShaves(&fmShaveNum, 1, OS_DRV_SVU_WAIT_FOREVER, &running);
#if defined(MA2150) || defined(MA2450)
    OsDrvShaveL2CachePartitionFlush(featPipeCacheData, PERFORM_INVALIDATION);
#else
    OsDrvShaveL2cFlushInvPart(featPipeCacheData, HGL_SHAVE_L2C_FLUSH_INV);
#endif

    status += OsDrvSvuCloseShaves(&fmShaveNum, 1);
    status += OsDrvSvuCleanupDynShaveApps(&fmContext);
    localTurnOffShave (fmShaveNum);
    rtems_cache_invalidate_data_range(g_fmCfg.cellFeatureCount[0], g_fmCfg.cellGridDimension * g_fmCfg.cellGridDimension *sizeof(u32));
    rtems_cache_invalidate_data_range(g_fmCfg.cellFeatureCount[1], g_fmCfg.cellGridDimension * g_fmCfg.cellGridDimension *sizeof(u32));
    rtems_cache_invalidate_data_range(g_fmCfg.featuresMetadata[0], g_fmCfg.maxNumFeatures * sizeof(g_fmCfg.featuresMetadata[0][0]));
    rtems_cache_invalidate_data_range(g_fmCfg.featuresMetadata[1], g_fmCfg.maxNumFeatures * sizeof(g_fmCfg.featuresMetadata[0][0]));
    rtems_cache_invalidate_data_range(g_fmCfg.features[0], g_fmCfg.maxNumFeatures * sizeof(g_fmCfg.features[0][0]));
    rtems_cache_invalidate_data_range(g_fmCfg.features[1], g_fmCfg.maxNumFeatures * sizeof(g_fmCfg.features[0][0]));
    rtems_cache_invalidate_data_range(g_fmCfg.featuresCount, 2 * sizeof(uint32_t));
}

void
FeatMaintenance::fmInit(FMSetupCfg* cfg, fmResourceCfg_t* resCfg) {
    fmShaveNum = resCfg->shaveNum;
    featPipeCacheData = resCfg->cachePartData;
    featPipeCacheInstr = resCfg->cachePartInstr;
    g_fmCfg = *cfg;


    fmContext = MODULE_DATA(fm);
    fmContext.instancesData = fmContextInstanceDataPtr;
    *fmContext.instancesData = *MODULE_DATA(fm).instancesData;
}

