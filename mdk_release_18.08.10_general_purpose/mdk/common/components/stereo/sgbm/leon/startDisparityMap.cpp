///
/// @file startDisparityMap.c
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief sgbm module file to start SHAVEs
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include "startDisparityMap.h"
#include "disparityMapApiDefines.h"

#include <swcTestUtils.h>

#include <VcsHooksApi.h>
#include <swcShaveLoader.h>
#include <DrvShaveL2Cache.h>
#include <OsDrvShaveL2Cache.h>
#include <OsDrvSvu.h>
#include <OsDrvTimer.h>
#include <theDynContext.h>

// 2:  Source Specific defines and types
// ----------------------------------------------------------------------------

//#define PROFILE_SGBM_STALLS
#define DEBUG_PRINTF(...)
//#define DEBUG_PRINTF printf
#define MORE_DEBUG_PRINT(...)
//#define MORE_DEBUG_PRINT printf

// Shave entrypoints
// ----------------------------------------------------------------------------
#ifdef SGBM_SOURCE_CODE_BUILD
#define SGBM_APPLICATION SGBMApp
#else
#define SGBM_APPLICATION SGBMDynApp
#endif
#define MAX_MODULES 12
extern DynamicContext_t MODULE_DATA(SGBM_APPLICATION);
DynamicContext_t sgbmAppModule[MAX_MODULES];
struct DynamicContextInstances_elm sgbmAppPrivateData[MAX_MODULES];

volatile u32 perfInit, perfDisp;

// Configure image tiles addresses for input and output
static void setupTileConfiguration(stereoUserConfig_t* userCfg,
                                u8* imgL, u8* imgR,
                                u8* downImgL, u8* downImgR,
                                u8* dispMapL0, u8* dispMapL1,
                                u8* confidenceMap,
                                stereoTileConfig_t outTiles[])
{
    const frameSpec quarterInputSize =
    {
            .type   = userCfg->frame->type,
            .height = userCfg->frame->height / SCALE_FACTOR,
            .width  = userCfg->frame->width / SCALE_FACTOR,
            .stride = userCfg->frame->width / SCALE_FACTOR * userCfg->frame->bytesPP,
            .bytesPP= userCfg->frame->bytesPP,
    };
    const frameSpec fullInputSize = *userCfg->frame;

    stereoTileConfig_t pyrCfg[2];

    pyrCfg[0].leftImage     = { .spec = quarterInputSize, .p1 = downImgL,  .p2=NULL, .p3=NULL };
    pyrCfg[0].rightImage    = { .spec = quarterInputSize, .p1 = downImgR,  .p2=NULL, .p3=NULL };
    pyrCfg[0].disparityMap  = { .spec = quarterInputSize, .p1 = dispMapL0, .p2=NULL, .p3=NULL };
    pyrCfg[0].confidenceMap = { .spec = quarterInputSize, .p1 = confidenceMap, .p2=NULL, .p3=NULL };
    pyrCfg[0].censusKernelSize  = CENSUS_KERNEL_SIZE;
    pyrCfg[0].disparities       = userCfg->maxDisparities / SCALE_FACTOR;

    pyrCfg[1].leftImage     = { .spec = fullInputSize, .p1 = imgL, .p2=NULL, .p3=NULL };
    pyrCfg[1].rightImage    = { .spec = fullInputSize, .p1 = imgR, .p2=NULL, .p3=NULL };
    pyrCfg[1].disparityMap  = { .spec = fullInputSize, .p1 = dispMapL1, .p2=NULL, .p3=NULL };
    //pyrCfg[1].confidenceMap  = { .spec = 0, .p1 = NULL, .p2=NULL, .p3=NULL };
    pyrCfg[1].censusKernelSize = CENSUS_KERNEL_SIZE;
    pyrCfg[1].disparities    = userCfg->maxDisparities / SCALE_FACTOR;

    memcpy(outTiles, pyrCfg, sizeof(pyrCfg));
}

// Run Disparity Map computation
extern "C" void runDisparityMap(stereoUserConfig_t* userCfg, int* dataL2Cpartitions,
                    u8* leftImage, u8* rightImage,
                    u8* downsampledLeftImage, u8* downsampledRightImage,
                    u8* combinedCostCube,
                    outputType* predictedDepthMapLR, outputType* depthMapLR,
                    u8* confidence,
                    u8* disparityMedianIntegerBuffArea, outputType* disparityMedianSubpixelBuffArea,
                    u32 flag, u32 threadNo)
{
    u32 list_shaves_sgbm[] = {0, 0, 0, 0, 0};

    list_shaves_sgbm[0] = userCfg->list_of_shaves_thread_1[0];
    list_shaves_sgbm[1] = userCfg->list_of_shaves_thread_1[1];
    list_shaves_sgbm[2] = userCfg->list_of_shaves_thread_1[2];
    list_shaves_sgbm[3] = userCfg->list_of_shaves_thread_1[3];
    list_shaves_sgbm[4] = userCfg->list_of_shaves_thread_1[4];

    DEBUG_PRINTF("sgbm th=%d. shv=%d...%d\n", threadNo, list_shaves_sgbm[0], list_shaves_sgbm[SHAVES_USED - 1]);
    sgbmAppModule[threadNo] = MODULE_DATA(SGBM_APPLICATION);
    sgbmAppPrivateData[threadNo] = *(MODULE_DATA(SGBM_APPLICATION).instancesData);
    sgbmAppModule[threadNo].instancesData = &sgbmAppPrivateData[threadNo];

    u64 cycles_elapsed;
    tyTimeStamp timer_data;

    stereoTileConfig_t tiles[2];
    setupTileConfiguration(userCfg,
                        leftImage, rightImage,
                        downsampledLeftImage, downsampledRightImage,
                        (u8*)predictedDepthMapLR, (u8*)depthMapLR, confidence, tiles);
    DEBUG_PRINTF("Disparities: L0:%lu L1:%lu\n", tiles[0].disparities, tiles[1].disparities);

    OsDrvTimerStartTicksCount(&timer_data);

    CHECK(OsDrvSvuSetupDynShaveApps(&sgbmAppModule[threadNo], list_shaves_sgbm, SGBM_SHAVES_USED));
#ifdef PROFILE_SGBM_STALLS
    PROF_PRINT("--- SHAVE Profiling for SGBM --- \n");
    swcShaveProfStartGathering(list_shaves_sgbm[0], &perfStr[threadNo]);
#endif
    CHECK(OsDrvSvuOpenShaves(list_shaves_sgbm, SGBM_SHAVES_USED, OS_MYR_PROTECTION_SELF));

    for (u32 i = 0; i < SGBM_SHAVES_USED; i++)
        CHECK(OsDrvSvuRunShaveAlgoOnAssignedShaveCC(&sgbmAppModule[threadNo], list_shaves_sgbm[i], "iiiiii",
                combinedCostCube, userCfg, &tiles, disparityMedianSubpixelBuffArea, disparityMedianIntegerBuffArea, flag));

    CHECK(OsDrvSvuDynWaitShaves(list_shaves_sgbm, SGBM_SHAVES_USED, OS_DRV_SVU_WAIT_FOREVER, NULL));

#ifdef PROFILE_SGBM_STALLS
    swcShaveProfStopGathering(list_shaves_sgbm[0], &perfStr[threadNo]);
    swcShaveProfPrint(list_shaves_sgbm[0], &perfStr[threadNo]);
#endif
    CHECK(OsDrvShaveL2CachePartitionFlush((u32)dataL2Cpartitions,PERFORM_INVALIDATION));

    CHECK(OsDrvSvuCloseShaves(list_shaves_sgbm, SGBM_SHAVES_USED));
    CHECK(OsDrvSvuCleanupDynShaveApps(&sgbmAppModule[threadNo]));

    OsDrvTimerGetElapsedTicks(&timer_data, &cycles_elapsed);
    perfDisp = cycles_elapsed;
    if (flag == 0)
    {
        PROF_PRINT("SGBM thread (shaves %d...%d) runtime: %f ms\n\n",
            list_shaves_sgbm[0], list_shaves_sgbm[SGBM_SHAVES_USED-1],
            cycles_elapsed / 600000.0);
    }
}
