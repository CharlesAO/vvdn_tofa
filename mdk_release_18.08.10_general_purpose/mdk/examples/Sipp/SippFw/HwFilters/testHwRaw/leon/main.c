///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Raw filter unit test modified from C model test...
///            /rawtest/test01.cpp

/////////////////////////////////////////////////////
// Include files
/////////////////////////////////////////////////////

#include <sipp.h>
#include <sippTestCommon.h>
#include "testHwRaw.h"

#ifdef SIPP_VCS
#include <UnitTestApi.h>
#include <DrvTimer.h>
#include <DrvDdr.h>
#endif

/////////////////////////////////////////////////////
// Test Macros
/////////////////////////////////////////////////////

#define SIPP_TEST_SYNC_API  // Set to use the synchronous API
//#define SIPP_USE_PRECALC_SCHEDULE

/////////////////////////////////////////////////////
// Global variables
/////////////////////////////////////////////////////

TestHwRaw testRaw;

#ifndef SIPP_TEST_SYNC_API
volatile u32 testComplete = 0;
#endif

//Statistics buffers
ae_patch_stats       DDR_DATA aeStatsBuff[AE_PATCHES_NO];
AF_paxel_statistics  DDR_DATA afStatsBuff[AF_PATCHES_NO];
u32             DDR_DATA histLuma   [256];
u32             DDR_DATA histRGB    [3*128];

//############################################################################################
//expected data from /doc/rawGold.xlsx
//############################################################################################
/*BAYER*/ static u32 bayerLumaHist[256] = {
/*BAYER*/     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*BAYER*/     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0,
/*BAYER*/     0, 0, 1, 1, 0, 2, 1, 1, 2, 2, 2, 3, 2, 2, 2, 5, 3, 4, 2, 0, 4, 3, 5, 1, 5,11, 7, 5, 6, 6, 6, 5,
/*BAYER*/     9,11, 8, 5,10,11,10,14,11, 9,10, 9, 4,11,13,10,11,11,10,13,12,13,11,13,14,16,13,12,17,15,14,22,
/*BAYER*/    16,16,19,19,21, 8,12,20, 9,20,11,18,16, 6,15,18,16,10,14,16, 9,13,14,13,11,14, 9,15, 6, 9, 7, 9,
/*BAYER*/     7,10, 5, 7, 4, 8, 6, 6, 3, 8, 2, 4, 5,10, 2, 2, 1, 3, 1, 1, 1, 1, 4, 4, 2, 1, 1, 1, 0, 1, 1, 1,
/*BAYER*/     0, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*BAYER*/     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
/*BAYER*/ };
/*BAYER*/
/*BAYER*/ static u32 bayerRgbHist[3][128] =
/*BAYER*/ {
/*BAYER*/   {1,2,2,0,2,2,0,3,2,4,5,2,2,3,1,1,2,0,3,1,1,1,3,2,4,1,2,0,1,1,3,3, //Red
/*BAYER*/    0,2,2,2,3,5,2,3,3,1,2,1,3,4,1,3,1,1,0,1,0,4,2,3,1,1,1,2,2,1,2,3,
/*BAYER*/    2,3,1,0,7,2,1,4,1,0,3,2,1,1,1,2,3,3,2,3,0,0,0,1,5,2,2,2,2,3,2,1,
/*BAYER*/    3,1,4,3,4,2,6,2,5,2,2,5,2,1,4,3,3,0,0,2,2,1,2,0,3,2,0,2,4,1,1,0},
/*BAYER*/
/*BAYER*/   {1,1,4,1,5,2,6,3,3,1,6,5,7,2,4,3,5,3,7,6,2,4,5,1,4,4,2,4,5,0,4,4, //Green
/*BAYER*/    1,4,6,6,2,3,5,6,2,4,3,0,7,3,3,8,7,5,4,4,4,3,3,1,4,4,4,6,8,4,5,1,
/*BAYER*/    6,2,4,4,6,5,5,7,6,7,3,5,5,6,4,2,6,4,9,5,1,4,2,5,5,4,3,4,3,3,8,3,
/*BAYER*/    2,1,6,5,7,3,3,4,4,4,5,2,5,3,1,5,4,3,2,5,3,5,1,2,6,6,3,7,5,6,3,1},
/*BAYER*/
/*BAYER*/   {3,0,1,7,2,1,0,3,0,0,5,4,3,1,3,2,1,1,4,2,1,2,2,1,0,3,1,1,3,1,1,1, //Blue
/*BAYER*/    1,4,0,2,0,0,0,2,5,0,3,2,4,3,1,3,2,1,1,1,3,3,4,2,2,4,4,0,3,3,0,5,
/*BAYER*/    3,2,3,1,2,1,0,3,2,2,4,1,2,4,0,5,2,2,2,1,2,1,1,0,4,5,1,5,1,1,2,3,
/*BAYER*/    3,0,1,4,2,1,1,1,3,2,3,3,3,2,3,1,2,1,2,1,2,3,1,1,1,1,3,1,2,2,3,1}
/*BAYER*/ };
/*BAYER*/
/*BAYER*/ static ae_patch_stats  bayerAeStats[AE_PATCHES_NO] = {
/*BAYER*/     {{0,0,0,0}, {484,517,366,514}, {0,0,0,0}},   {{0,0,0,0}, {698,732,662,565}, {0,0,0,0}},   {{0,0,0,0}, {672,523,570,464}, {0,0,0,0}},
/*BAYER*/     {{0,0,0,0}, {580,303,602,519}, {0,0,0,0}},   {{0,0,0,0}, {433,593,500,525}, {0,0,0,0}},   {{0,0,0,0}, {343,406,336,493}, {0,0,0,0}},
/*BAYER*/ };

/////////////////////////////////////////////////////
// Code
/////////////////////////////////////////////////////

#ifdef SIPP_USE_PRECALC_SCHEDULE
#include "pcDumpSchedule.hh"
#endif

#ifndef SIPP_TEST_SYNC_API
void appSippCallback ( SippPipeline *             pPipeline,
                       eSIPP_PIPELINE_EVENT       eEvent,
                       SIPP_PIPELINE_EVENT_DATA * ptEventData
                     )
{
    if (eEvent == eSIPP_PIPELINE_FRAME_DONE)
    {
        printf ("appSippCallback : Frame done event received : Test proceeding to checks and termination\n");
        testComplete = 1;
    }
}

#endif

int main (int argc, char *argv[])
{
    UNUSED (argc);
    UNUSED (argv);
    u32    sippErrorLog[SIPP_ERROR_HISTORY_SIZE];

    sippPlatformInit ();

    #ifndef SIPP_PC
    if (PROCESS_LEON_OS == swcWhoAmI())
    {
        printf ("App Starting on LEON OS\n");

        #ifndef SIPP_TEST_SYNC_API
        printf ("Please make sure IRQ sources for dynamically route between LeonOS and LeonRT are set\n");
        #endif
    }
    else
    {
        printf ("App Starting on LEON RT\n");
    }
    #endif

    #ifndef SIPP_TEST_SYNC_API
    sippPlatformInitAsync ();
    #endif

    #ifdef SIPP_VCS
    unitTestInit();
    DrvTimerInit();
    DrvDdrInitialise(NULL);
    #endif

    buildTestHwRaw(&testRaw);

    if (sippPipeGetErrorStatus (testRaw.pl))
    {
        u32 errNum;
        printf ("Pipeline creation error\nError codes:\n");
        errNum = sippGetErrorHistory (sippErrorLog);
        while (errNum)
        {
            printf ("%08lx\n", sippErrorLog[errNum - 0x1]);
            errNum--;
        }
    }

    #ifndef SIPP_TEST_SYNC_API
    // Register callback for async API
    sippRegisterEventCallback (testRaw.pl,
                               appSippCallback);
    #endif

    configTestHwRaw(&testRaw);

    #ifdef SIPP_USE_PRECALC_SCHEDULE
    // Read offline schedule decisions
    dbgSchedInit (testRaw.pl);
    #endif

    #ifdef SIPP_TEST_SYNC_API
    sippProcessFrame (testRaw.pl);

    if (sippPipeGetErrorStatus (testRaw.pl))
    {
        u32 errNum;
        printf ("Pipeline runtime error\nError codes:\n");
        errNum = sippGetErrorHistory (sippErrorLog);
        while (errNum)
        {
            printf ("%08lx\n", sippErrorLog[errNum - 0x1]);
            errNum--;
        }
    }

    #else
    sippProcessFrameNB (testRaw.pl);

    if (sippPipeGetErrorStatus (testRaw.pl))
    {
        u32 errNum;
        printf ("Pipeline runtime error\nError codes:\n");
        errNum = sippGetErrorHistory (sippErrorLog);
        while (errNum)
        {
            printf ("%08lx\n", sippErrorLog[errNum - 0x1]);
            errNum--;
        }
    }

    while (testComplete == 0x0)
    {

    }
    #endif

    #ifdef  SIPP_PC
    sippGenericRuntimeWaitIsrThreadTerm ();
    #endif

    writeTestHwRawOutput(&testRaw);

    // Write out the stats files for off-line inspection
    sippWrFileU8 ((u8*)histLuma,sizeof(histLuma),"BAYER_lumaHist.stat");
    sippWrFileU8 ((u8*)&histRGB[0*128],128*sizeof(u32),"BAYER_R-hist.stat");
    sippWrFileU8 ((u8*)&histRGB[1*128],128*sizeof(u32),"BAYER_G-hist.stat");
    sippWrFileU8 ((u8*)&histRGB[2*128],128*sizeof(u32),"BAYER_B-hist.stat");
    sippWrFileU8 ((u8*)aeStatsBuff,sizeof(aeStatsBuff) * sizeof(u8),"BAYER_Ae.stat");

    sippDbgCompareU8 ((u8*)bayerLumaHist,    (u8*)histLuma,         sizeof(histLuma));
    sippDbgCompareU8 ((u8*)bayerRgbHist[0],  (u8*)&histRGB[0*128],  128*sizeof(u32));
    sippDbgCompareU8 ((u8*)bayerRgbHist[1],  (u8*)&histRGB[1*128],  128*sizeof(u32));
    sippDbgCompareU8 ((u8*)bayerRgbHist[2],  (u8*)&histRGB[2*128],  128*sizeof(u32));
    sippDbgCompareU8 ((u8*)bayerAeStats,     (u8*)aeStatsBuff    ,  sizeof(aeStatsBuff));

    #ifdef SIPP_VCS
    unitTestFinalReport();
    #endif

    return 0;
}
