///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Median filter unit test modified from C model test...
///            /mediantest/test.cpp
///
///            In the oPipe the median filter is used to process sub-sampled
///            3-plane chroma. In this use case the median result may be alpha
///            blended with the original input based on the local luma intensity
///            such that in low light conditions, where the chroma information
///            is less reliable, the results are pulled towards the median, but
///            where lighting conditions are good the filter effect is weaker

/////////////////////////////////////////////////////
// Include files
/////////////////////////////////////////////////////

#include <sipp.h>
#include <sippTestCommon.h>

#if defined(SIPP_VCS)
#include <UnitTestApi.h>
#include <DrvTimer.h>
#include <DrvDdr.h>
#endif

#include "testData.h"
#include "testHwMedian.h"

/////////////////////////////////////////////////////
// Global Macros
/////////////////////////////////////////////////////

#define SIPP_TEST_SYNC_API  // Set to use the synchronous API
#define SIPP_USE_PRECALC_SCHEDULE

/////////////////////////////////////////////////////
// Global variables
/////////////////////////////////////////////////////
TestHwMedian medianPipe;

#ifndef SIPP_TEST_SYNC_API
volatile u32 testComplete = 0;
#endif

#ifdef SIPP_USE_PRECALC_SCHEDULE
#include "pcDumpSchedule.hh"
#endif




#ifndef SIPP_TEST_SYNC_API
void appSippCallback ( SippPipeline *             pPipeline,
                       eSIPP_PIPELINE_EVENT       eEvent,
                       SIPP_PIPELINE_EVENT_DATA * ptEventData )
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

    buildTestHwMedian(&medianPipe);

    if (sippPipeGetErrorStatus (medianPipe.pl))
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
    sippRegisterEventCallback (medianPipe.pl,
                               appSippCallback);
    #endif

    configTestHwMedian(&medianPipe);

    readTestHwMedianInput(&medianPipe);
    #ifdef SIPP_USE_PRECALC_SCHEDULE
    // Read offline schedule decisions
    dbgSchedInit (medianPipe.pl);
    #endif

    #ifdef SIPP_TEST_SYNC_API
    sippProcessFrame(medianPipe.pl);

    if (sippPipeGetErrorStatus (medianPipe.pl))
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
    sippProcessFrameNB(medianPipe.pl);

    if (sippPipeGetErrorStatus (medianPipe.pl))
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

    while ( testComplete == 0x0 )
    {

    }
    #endif

    #ifdef  SIPP_PC
    sippGenericRuntimeWaitIsrThreadTerm ();
    #endif

    writeTestHwMedianOutput(&medianPipe);
    sippRdFileU8 (TestHwMedian_dmaIn0_buffer, SIPP_HW_MED_CHROMA_BUFFER_SIZE , "../../../../../../../resources/Ref_MedianOutput_192x145_8bpp.raw");
    sippDbgCompareU8  (TestHwMedian_dmaIn0_buffer, TestHwMedian_dmaOut0_buffer, SIPP_HW_MED_CHROMA_BUFFER_SIZE);

    #ifdef SIPP_VCS
    unitTestFinalReport();
    #endif

    return 0;
}
