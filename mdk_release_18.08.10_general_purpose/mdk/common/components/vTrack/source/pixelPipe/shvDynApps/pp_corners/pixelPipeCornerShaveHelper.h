#ifndef PP_SHV_HELPER_H
#define PP_SHV_HELPER_H
#include "vTrackPrivateTypes.h"
#include "pixelPipeHelper.h"
#define SCALING_FOR_HARRIS_FP16 24

void
insertThreshAndSort(tvFeatureCell* featureData,
             u32 x,
             u32 y,
             fp32 harrisScore,
             u32 maxFeatPerCell,
             fp32& threshold);

void
insertThreshOnly(tvFeatureCell* featureData,
             u32 x,
             u32 y,
             fp32 harrisScore,
             u32 maxFeatPerCell,
             fp32& threshold);

void smartInsert(tvFeatureCell* featureData, u32 x, u32 y,
                        fp32 harrisScore, u32 maxFeatPerCell, fp32& threshold);
void insertFeat(t_vTrackHarrisFeat* featureData, u32 x, u32 y, fp32 score);
void* swcSolveShaveRelAddr(void* inAddr);


#endif
