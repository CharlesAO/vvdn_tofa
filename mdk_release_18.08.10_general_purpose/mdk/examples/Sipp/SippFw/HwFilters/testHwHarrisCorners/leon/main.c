///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Harris filter unit test taken from c-model test
///        /harriscornerstest/test.cpp
///

#include <sipp.h>
#include <sippTestCommon.h>
#include "testHwHarrisCorners.h"

#if defined(SIPP_VCS)
#include <UnitTestApi.h>
#include <DrvTimer.h>
#include <DrvDdr.h>
#endif

/////////////////////////////////////////////////////
// Test specific macros
/////////////////////////////////////////////////////

#define SIPP_TEST_SYNC_API  // Set to use the synchronous API

/////////////////////////////////////////////////////
// Global variables
/////////////////////////////////////////////////////

#ifdef SIPP_USE_PRECALC_SCHEDULE
#include "pcDumpSchedule.hh"
#endif

TestHwHarrisCorners testHarris;

#ifndef SIPP_TEST_SYNC_API
volatile u32 testComplete = 0;
#endif

u32 DDR_DATA TestHwHarrisCorners_ref_buffer[SIPP_HW_HARRIS_BUFFER_SIZE];

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
    #endif // SIPP_PC

    #ifndef SIPP_TEST_SYNC_API
    sippPlatformInitAsync ();
    #endif

    #ifdef SIPP_VCS
    unitTestInit ();
    DrvTimerInit ();
    DrvDdrInitialise (NULL);
    #endif

    buildTestHwHarrisCorners (&testHarris);

    if (sippPipeGetErrorStatus (testHarris.pl))
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
    sippRegisterEventCallback (testHarris.pl,
                               appSippCallback);
    #endif

    configTestHwHarrisCorners (&testHarris);

    readTestHwHarrisCornersInput(&testHarris);

    #ifdef SIPP_USE_PRECALC_SCHEDULE
    // Read offline schedule decisions
    dbgSchedInit (testHarris.pl);
    #endif

    #ifdef SIPP_TEST_SYNC_API
    sippProcessFrame(testHarris.pl);

    if (sippPipeGetErrorStatus (testHarris.pl))
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
    sippProcessFrameNB(testHarris.pl);

    if (sippPipeGetErrorStatus (testHarris.pl))
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

    while ( testComplete == 0x0 )    //Move below 8 lines or so into testcommon
    {                               //so this becomes ne function call

    }

    #endif

    #ifdef  SIPP_PC
    sippGenericRuntimeWaitIsrThreadTerm ();
    #endif

    sippRdFileU8((u8*)TestHwHarrisCorners_ref_buffer,\
                 SIPP_HW_HARRIS_BUFFER_SIZE * sizeof(u32),\
                 "../../../../../../../resources/Ref_HarrtestOutput_fp32_1296x972_P400.rgb");

    writeTestHwHarrisCornersOutput(&testHarris);

    sippDbgCompareU8((u8*)TestHwHarrisCorners_ref_buffer,\
                     (u8*)TestHwHarrisCorners_dmaOut0_buffer,\
                     SIPP_HW_HARRIS_BUFFER_SIZE * sizeof(u32));

    #ifdef SIPP_VCS
    unitTestFinalReport();
    #endif

    return 0;
}
