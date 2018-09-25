///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
#ifndef FEATURE_MAINTENANCE_TYPES_H
#define FEATURE_MAINTENANCE_TYPES_H

#include "mv_types.h"
#include "swcFrameTypes.h"
#include "vTrack.h"

struct tvFeatureCell;

enum fmMode {
    FM_PP_ONLY,
    FM_TRACKED_ONLY,
    FM_PP_AND_TRACKED
};

typedef struct
{
    fmMode mode;
    bool filterFeatures;
    u32* featureIdCounter;
    u32* featuresCount;
    tvFeatureCell** featureCells;
    tvXYLoc** features;
    FeatureMetadata** featuresMetadata;
    fp32** featuresError;
    u32** cellFeatureCount;
    u32 cellGridDimension;
    u32 targetNumFeatures;
    u32 maxNumFeatures;
} FMSetupCfg;

#endif
