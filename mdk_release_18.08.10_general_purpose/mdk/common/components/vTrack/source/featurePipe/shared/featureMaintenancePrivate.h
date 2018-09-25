#ifndef FEATURE_MAINTENANCE_PRIVATE_H
#define FEATURE_MAINTENANCE_PRIVATE_H
#include "vTrackOutput.h"
#include "featureMaintenanceTypes.h"
#include "vPipePublicTypes.h"
#ifdef __cplusplus
extern "C" {
#endif

void featureMaintenance(FMSetupCfg* algConfig,
                        const u32 frameNum,
                        const frameBuffer* const fullFrame,
                        const tvXYLoc featuresTmp[],
                        const fp32 featuresErrorTmp[],
                        FMFeatureThresholds_t* thresholds);

void featureMaintenanceSetup(FMSetupCfg* cfg);

#ifdef __cplusplus
}
#endif

#endif
