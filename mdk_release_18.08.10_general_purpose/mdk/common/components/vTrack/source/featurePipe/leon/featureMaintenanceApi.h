///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
#ifndef FEATURE_MAINTENANCE_H
#define FEATURE_MAINTENANCE_H

#include "mv_types.h"
#include "swcFrameTypes.h"
#include "vTrack.h"
#include "vPipePublicTypes.h"
#include "global_constants.h"
#ifndef __PC__
#include "theDynContext.h"
#endif
#include "featureMaintenanceTypes.h"

typedef u32 swcShaveUnit_t;
typedef struct fmResourceCfg {
    swcShaveUnit_t  shaveNum;
    uint8_t cachePartData;
    uint8_t cachePartInstr;
} fmResourceCfg_t;

class FeatMaintenance {
public:
    FeatMaintenance();
    ~FeatMaintenance();
    void fmRun(const u32 frameNum,
                        const frameBuffer* const fullFrame,
                        const tvXYLoc featuresTmp[],
                        const fp32 featuresErrorTmp[],
                        FMFeatureThresholds_t* thresholds);

    void fmInit(FMSetupCfg* cfg,
            fmResourceCfg_t* resCfg);
private:
    uint32_t fmShaveNum;
    uint8_t featPipeCacheData;
    uint8_t featPipeCacheInstr;
    FMSetupCfg g_fmCfg;
#ifndef __PC__
    DynamicContext_t fmContext ALIGNED(64);
    DynamicContextInstances_elm fmContextInstanceData ALIGNED(64);
    DynamicContextInstancesPtr fmContextInstanceDataPtr ALIGNED(64);
#endif
} ALIGNED(64);

#endif
