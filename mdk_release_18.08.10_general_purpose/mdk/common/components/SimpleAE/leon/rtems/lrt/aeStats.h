/// =====================================================================================
///
///        @file:      aeStats.h
///        @brief:     API header for simpleMonoAE Statistics calculation module
///        @copyright: All code copyright Movidius Ltd 2015, all rights reserved.
///                  For License Warranty see: common/license.txt
/// =====================================================================================
///

#ifndef AE_STATS_H
#define AE_STATS_H

#include <swcFrameTypes.h>
#include <sipp.h>
#include <include/sippHwDefs.h>
//#include "sippPlatform.h"

/// System Includes
/// -------------------------------------------------------------------------------------

/// Application Includes
/// -------------------------------------------------------------------------------------

/// Source Specific #defines and types (typedef,enum,struct)
/// -------------------------------------------------------------------------------------

#define AE_STATS_PATCH_H            (32)    /// Patch height (max 256)
#define AE_STATS_PATCH_W            (32)    /// Patch width (max 256)

#define AE_STATS_MAX_PATCHES_HORZ   (64)    /// Max Nr of patches in horizontal direction
#define AE_STATS_MAX_PATCHES_VERT   (64)    /// Max Nr of patches in vertical direction

typedef struct
{
    int init;
    u32 imgWidth;
    u32 imgHeight;
    u32 imgBpp;
    u32 patchWidth;
    u32 patchHeight;
    u32 patchStepHorz;
    u32 patchStepVert;
    u32 patchNmbrHorz;
    u32 patchNmbrVert;
    u32 patchOffsetHorz;
    u32 patchOffsetVert;
} aeStatsConfig_t;

/// Global Data (Only if absolutely necessary)
/// -------------------------------------------------------------------------------------

/// Function Prototypes
/// -------------------------------------------------------------------------------------

void aeStatsRun(frameBuffer *frameIn, void *frame, ae_patch_stats *statPatchesBuffer);
void aeStatsInit();
aeStatsConfig_t * aeStatsCheckIfConfig(u32 imgWidth, u32 imgHeight, u32 imgBpp);

#endif /// AE_STATS_H
