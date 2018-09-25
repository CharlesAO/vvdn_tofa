#include "mv_types.h"
#include "featureMaintenancePrivate.h"
#include "featureMaintenanceApi.h"

FeatMaintenance::FeatMaintenance() {}
FeatMaintenance::~FeatMaintenance() {}

void
FeatMaintenance::fmRun(const u32 frameNum,
                        const frameBuffer* const fullFrame,
                        const tvXYLoc featuresTmp[],
                        const fp32 featuresErrorTmp[],
                        FMFeatureThresholds_t* thresholds) {
    featureMaintenance(&g_fmCfg, frameNum, fullFrame, featuresTmp,
                    featuresErrorTmp, thresholds);
}

void
FeatMaintenance::fmInit(FMSetupCfg* cfg, fmResourceCfg_t* resCfg) {
    (void) resCfg;
    g_fmCfg = *cfg;
}

