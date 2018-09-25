/// =====================================================================================
///
///        @file:      aeStats.c
///        @brief:     simpleMonoAE Statistics calculation module
///        @copyright: All code copyright Movidius Ltd 2015, all rights reserved.
///                  For License Warranty see: common/license.txt
/// =====================================================================================
///

/// System Includes
/// -------------------------------------------------------------------------------------
#include <aeStats.h>
#include <stdio.h>
#include <string.h>
#include "swcLeonUtils.h"
#include <sipp.h>
#include <sippTestCommon.h>

#ifndef MA2480
#endif
#include <include/sippHwDefs.h>
#include <swcFrameTypes.h>
#include <mvLog.h>

#include "semaphore.h"
/// Application Includes
/// -------------------------------------------------------------------------------------
#include "aeStats.h"

#ifndef CVRT_SLICE
#define CVRT_SLICE 6
#endif

#undef CMX_DATA // TODO: nasty thing here
#define CMX_DATA __attribute__((section(".cmx.data")))

/// Source Specific #defines and types (typedef,enum,struct)
/// -------------------------------------------------------------------------------------
#define AE_BAYER          1
#define AE_GRBG           0
#define AE_PATCH_STATS_EN     1
#define AF_PATCH_STATS_EN     0
#define AE_Y_HIST_STATS_EN    0
#define AE_RGB_HIST_STATS_EN  0
#define AE_STATS_DIGI_GAIN           0x0100

#define AE_STATS_CONFIG_NOT_INIT    (0)
#define AE_STATS_CONFIG_READY       (1)

/// Global Data (Only if absolutely necessary)
/// -------------------------------------------------------------------------------------
SippPipeline *aeStatsPipe;

SippFilter *aeStatsDmaIn;
SippFilter *aeStatsDmaOutRaw;
SippFilter *aeStatsRawBLK;

/// Static Local Data
/// -------------------------------------------------------------------------------------

static aeStatsConfig_t aeStatsCfg;

/// Static Function Prototypes
/// -------------------------------------------------------------------------------------
static void aeStatsBuildAndConfigurePipeline(aeStatsConfig_t *aeCfg);

/// Functions Implementation
/// -------------------------------------------------------------------------------------

/// ===  FUNCTION  ======================================================================
///  Name:  aeStatsBuildAndConfigurePipeline
///  Description: Builds and configures the AE Statistics pipeline
/// =====================================================================================
static void aeStatsBuildAndConfigurePipeline(aeStatsConfig_t *aeCfg)
{
    u32 imgWidth = aeCfg->imgWidth;
    u32 imgHeight = aeCfg->imgHeight;
    u32 sizeBytes;
    u32 sizeBits;
    u32 AE_STATS_DIGI_SAT;

    switch(aeCfg->imgBpp)
    {
    case 1:
        sizeBytes = SZ(UInt8);
        sizeBits = 8;
        AE_STATS_DIGI_SAT = 0xFF;

        break;
    case 2:
        sizeBytes = SZ(UInt16);
        sizeBits = 10;
        AE_STATS_DIGI_SAT = 0x3FF;
        break;
    default:
        assert(0);
    }

    aeStatsPipe = sippCreatePipeline(CVRT_SLICE, CVRT_SLICE, NULL);

    aeStatsDmaIn  = sippCreateFilter(aeStatsPipe, 0x00, imgWidth, imgHeight,
                                     N_PL(1), sizeBytes, SZ(DmaParam),
                                     (FnSvuRun)SIPP_DMA_ID, 0);
    aeStatsRawBLK = sippCreateFilter(aeStatsPipe, 0x00, imgWidth, imgHeight,
                                     N_PL(1), sizeBytes, SIPP_AUTO,
                                     (FnSvuRun)SIPP_RAW_ID, 0);
    aeStatsDmaOutRaw = sippCreateFilter(aeStatsPipe, 0x00, imgWidth, imgHeight,
                                        N_PL(1), sizeBytes, SZ(DmaParam),
                                        (FnSvuRun)SIPP_DMA_ID, 0);

    sippLinkFilter(aeStatsRawBLK, aeStatsDmaIn,  5, 5);
    sippLinkFilter(aeStatsDmaOutRaw, aeStatsRawBLK,  1, 1);

    /// Configure pipeline
    RawParam *rawCfg = (RawParam*)aeStatsRawBLK->params;

    rawCfg->statsPatchStart = (aeCfg->patchOffsetHorz <<  0)
                            | (aeCfg->patchOffsetVert << 16);
    rawCfg->statsPatchCfg   = ((aeCfg->patchNmbrHorz - 1) <<  0)
                            | ((aeCfg->patchNmbrVert - 1) <<  8)
                            | ((aeCfg->patchWidth    - 1) << 16)
                            | ((aeCfg->patchHeight   - 1) << 24);
    rawCfg->statsPatchSkip  = ((aeCfg->patchStepHorz - 1) <<  0)
                            | ((aeCfg->patchStepVert - 1) << 16);
#ifdef MA2X8X
    rawCfg->cfg = RAW_CFG(AE_BAYER, AE_GRBG, 0, 0, AE_PATCH_STATS_EN,
                          AE_RGB_HIST_STATS_EN, (sizeBits-1),
                          0, 0, AF_PATCH_STATS_EN, 0, AE_Y_HIST_STATS_EN, 0, 1);
#else
    rawCfg->cfg = RAW_CFG(AE_BAYER, AE_GRBG, 0, 0, AE_PATCH_STATS_EN,
                          AE_RGB_HIST_STATS_EN, (sizeBits-1),
                          0, 0, AF_PATCH_STATS_EN, 0, AE_Y_HIST_STATS_EN, 0);
#endif
    rawCfg->statsPlanes = 0;
    rawCfg->grgbPlat = 0;
    rawCfg->grgbDecay = 0;
    rawCfg->badPixCfg = 0;
    rawCfg->gainSat[0] = ((AE_STATS_DIGI_SAT<<16) | AE_STATS_DIGI_GAIN );
    rawCfg->gainSat[1] = ((AE_STATS_DIGI_SAT<<16) | AE_STATS_DIGI_GAIN );
    rawCfg->gainSat[2] = ((AE_STATS_DIGI_SAT<<16) | AE_STATS_DIGI_GAIN );
    rawCfg->gainSat[3] = ((AE_STATS_DIGI_SAT<<16) | AE_STATS_DIGI_GAIN );
    rawCfg->statsThresh = (0 << 16) | AE_STATS_DIGI_SAT;
}

/// ===  FUNCTION  ======================================================================
///  Name:  aeStatsRun
///  Description: Runs the statistics gatherig pipeline on the given image and outputs
///               statistics to the given buffer
/// =====================================================================================
#ifdef MA2480

sem_t aeSippProcessSem;
void appSippCallback ( SippPipeline *             pPipeline,
                       eSIPP_PIPELINE_EVENT       eEvent,
                       SIPP_PIPELINE_EVENT_DATA * ptEventData
                     )
{
    UNUSED(pPipeline);
    UNUSED(ptEventData);

    if (eEvent == eSIPP_PIPELINE_FRAME_DONE)
    {
        mvLog(MVLOG_DEBUG, "appSippCallback : Frame done event received : Test proceeding to checks and termination\n");
        sem_post(&aeSippProcessSem);
    }
}
#endif
void aeStatsRun(frameBuffer *frameIn, void *frame, ae_patch_stats *statPatchesBuffer)
{
    /// Assign DMA to new buffer
    DmaParam *dmaInCfg = (DmaParam*)aeStatsDmaIn->params;
    dmaInCfg->ddrAddr  = (uint32_t)frameIn->p1;

    DmaParam *dmaOutRawCfg = (DmaParam*)aeStatsDmaOutRaw->params;
    dmaOutRawCfg->ddrAddr  = (uint32_t)frame;

    /// Assign statistics patch buffer
    RawParam *rawCfg   = (RawParam*)aeStatsRawBLK->params;
    assert( (((uint32_t)statPatchesBuffer) & (0x7 << 28)) == (0x7 << 28)); // stats buffer must be in CMX
    rawCfg->statsBase  = (UInt32 *)((uint32_t)statPatchesBuffer & ~0x8000000); // sipp expects uncached addresses

    /// Run the pipeline
#ifndef MA2480
    sippProcessFrame(aeStatsPipe);
#else
    sippProcessFrameNB(aeStatsPipe);
    sem_wait(&aeSippProcessSem);
#endif
}


/// ===  FUNCTION  ======================================================================
///  Name:  aeStatsInit
///  Description: initializes the sipp framework
/// =====================================================================================
void aeStatsInit()
{
    /// Initialize the sipp framework
    sippInitialize();
#ifdef MA2480
    sippPlatformInitAsync ();
    sem_init(&aeSippProcessSem, 0 , 0);
#endif

    memset(&aeStatsCfg, 0, sizeof(aeStatsCfg));
    aeStatsCfg.init = AE_STATS_CONFIG_NOT_INIT;
}

/// ===  FUNCTION  ======================================================================
///  Name:  aeStatsCheckIfConfig
///  Description: check if AE Statistics pipe was initialized -> if not - create the pipe.
///  When the mounted image sensor is detected runtime - the Camera resolution (needed for
///  pipe initialization) is known only on the first frame ready message from Frame Pump.
/// =====================================================================================
aeStatsConfig_t * aeStatsCheckIfConfig(u32 imgWidth, u32 imgHeight, u32 imgBpp)
{
    if (aeStatsCfg.init == AE_STATS_CONFIG_NOT_INIT)
    {
        aeStatsCfg.imgWidth = imgWidth;
        aeStatsCfg.imgHeight = imgHeight;
        aeStatsCfg.imgBpp = imgBpp;
        aeStatsCfg.patchWidth = AE_STATS_PATCH_W;
        aeStatsCfg.patchHeight = AE_STATS_PATCH_H;
        aeStatsCfg.patchStepHorz = aeStatsCfg.patchWidth;               /// X skip till next patch (TLC to TLC)
        aeStatsCfg.patchStepVert = aeStatsCfg.patchHeight;              /// Y skip till next patch (TLC to TLC)
        aeStatsCfg.patchNmbrHorz = imgWidth / aeStatsCfg.patchWidth;
        aeStatsCfg.patchNmbrVert = imgHeight / aeStatsCfg.patchHeight;
        if (aeStatsCfg.patchNmbrHorz > AE_STATS_MAX_PATCHES_HORZ)
            aeStatsCfg.patchNmbrHorz = AE_STATS_MAX_PATCHES_HORZ;
        if (aeStatsCfg.patchNmbrVert > AE_STATS_MAX_PATCHES_VERT)
            aeStatsCfg.patchNmbrVert = AE_STATS_MAX_PATCHES_VERT;
        aeStatsCfg.patchOffsetHorz = (imgWidth - aeStatsCfg.patchNmbrHorz * aeStatsCfg.patchWidth) / 2;     /// First patch start X offset
        aeStatsCfg.patchOffsetVert = (imgHeight - aeStatsCfg.patchNmbrVert * aeStatsCfg.patchHeight) / 2;   /// First patch start Y offset
        mvLog(MVLOG_DEBUG, "AE Statistics Patch configuration for camera resolution %lux%lu:\n",
                            aeStatsCfg.imgWidth, aeStatsCfg.imgHeight);
        mvLog(MVLOG_DEBUG, "Size: width %lu, height %lu; \tStep: horz %lu, vert %lu;\n",
                            aeStatsCfg.patchWidth, aeStatsCfg.patchHeight,
                            aeStatsCfg.patchStepHorz, aeStatsCfg.patchStepVert);
        mvLog(MVLOG_DEBUG, "Number: horz %lu, vert %lu; \tStart offset: horz %lu, vert %lu.\n",
                            aeStatsCfg.patchNmbrHorz, aeStatsCfg.patchNmbrVert,
                            aeStatsCfg.patchOffsetHorz, aeStatsCfg.patchOffsetVert);

        /// Build and configure the pipeline
        aeStatsBuildAndConfigurePipeline(&aeStatsCfg);
        sippMemStatus();

#ifdef MA2480
    // Register callback for async API
    sippRegisterEventCallback (aeStatsPipe,
                               appSippCallback);
#endif

        aeStatsCfg.init = AE_STATS_CONFIG_READY;
    }

    return (&aeStatsCfg);
}
