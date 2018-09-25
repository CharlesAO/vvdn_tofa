///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Raw filter unit test modified from C model test...
///            /lookuptabletest/test.cpp

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

#include "testHwSharpen.h"
#include "testMacros.h"


#define SIPP_TEST_SYNC_API  // Set to use the synchronous API
//#define SIPP_USE_PRECALC_SCHEDULE



/////////////////////////////////////////////////////
// Global variables
/////////////////////////////////////////////////////


TestHwSharpen testSharpenPipe;

//UInt8      DDR_DATA TestHwSharpen_dmaIn0_buffer[SIPP_HW_SHARPEN_BUFFER_WIDTH* SIPP_HW_SHARPEN_BUFFER_HEIGHT* 1  ]ALIGNED(8);
//UInt8      DDR_DATA TestHwSharpen_dmaOut0_buffer[SIPP_HW_SHARPEN_BUFFER_WIDTH* SIPP_HW_SHARPEN_BUFFER_HEIGHT* 1  ]ALIGNED(8);

#ifndef SIPP_TEST_SYNC_API
volatile u32 testComplete = 0;
#endif

/////////////////////////////////////////////////////
// Function prototypes
/////////////////////////////////////////////////////

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

    buildTestHwSharpen(&testSharpenPipe);

    if (sippPipeGetErrorStatus (testSharpenPipe.pl))
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
    sippRegisterEventCallback (testSharpenPipe.pl,
                               appSippCallback);
    #endif

    configTestHwSharpen(&testSharpenPipe);

    readTestHwSharpenInput(&testSharpenPipe);

    #ifdef SIPP_USE_PRECALC_SCHEDULE
    // Read offline schedule decisions
    dbgSchedInit(testSharpenPipe.pl);
    #endif

    #ifdef SIPP_TEST_SYNC_API
    sippProcessFrame(testSharpenPipe.pl);

    if (sippPipeGetErrorStatus (testSharpenPipe.pl))
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
    sippProcessFrameNB(testSharpenPipe.pl);

    if (sippPipeGetErrorStatus (testSharpenPipe.pl))
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

    while ( testComplete == 0x0 )                // Move below 8 lines or so into testcommon so this becomes ne function call
    {

    }
    #endif

    #ifdef  SIPP_PC
    sippGenericRuntimeWaitIsrThreadTerm ();
    #endif


    writeTestHwSharpenOutput(&testSharpenPipe);

    sippRdFileU8 (TestHwSharpen_dmaIn0_buffer, SIPP_HW_SHARPEN_BUFFER_SIZE, "../../../../../../../resources/Ref_sharpenOutput_pepper512x512.y");
    sippDbgCompareU8 (TestHwSharpen_dmaIn0_buffer, TestHwSharpen_dmaOut0_buffer, SIPP_HW_SHARPEN_BUFFER_SIZE);

    #ifdef SIPP_VCS
    unitTestFinalReport();
    #endif

    return 0;
}
