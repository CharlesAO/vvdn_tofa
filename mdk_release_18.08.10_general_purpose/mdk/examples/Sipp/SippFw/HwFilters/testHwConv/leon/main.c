///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Convolution filter unit test taken from c-model test
///        /convolutiontest/test.cpp
///

#include <sipp.h>
#include <sippTestCommon.h>

#if defined(SIPP_VCS)
#include <UnitTestApi.h>
#include <DrvTimer.h>
#include <DrvDdr.h>

#endif

#include "testHwConv.h"

/////////////////////////////////////////////////////
// Test specific macros
/////////////////////////////////////////////////////

#define SIPP_TEST_SYNC_API  // Set to use the synchronous API
//#define SIPP_USE_PRECALC_SCHEDULE

#define SIPP_HW_CONV_BUFFER_WIDTH      1296
#define SIPP_HW_CONV_BUFFER_HEIGHT     972
#define SIPP_HW_CONV_BUFFER_NUM_PLANES  1
#define SIPP_HW_CONV_BUFFER_SIZE  (SIPP_HW_CONV_BUFFER_WIDTH * SIPP_HW_CONV_BUFFER_HEIGHT * SIPP_HW_CONV_BUFFER_NUM_PLANES)

/////////////////////////////////////////////////////
// Global variables
/////////////////////////////////////////////////////

#ifdef SIPP_USE_PRECALC_SCHEDULE
#include "pcDumpSchedule.hh"
#endif

TestHwConv testConvPipe;

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

    buildTestHwConv(&testConvPipe);

    if (sippPipeGetErrorStatus (testConvPipe.pl))
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
    sippRegisterEventCallback (testConvPipe.pl,
                               appSippCallback);
    #endif

    configTestHwConv(&testConvPipe);

    readTestHwConvInput(&testConvPipe);

    #ifdef SIPP_USE_PRECALC_SCHEDULE
    // Read offline schedule decisions
    dbgSchedInit (testConvPipe.pl);
    #endif

    #ifdef SIPP_TEST_SYNC_API
    sippProcessFrame (testConvPipe.pl);

    if (sippPipeGetErrorStatus (testConvPipe.pl))
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
    sippProcessFrameNB (testConvPipe.pl);

    if (sippPipeGetErrorStatus (testConvPipe.pl))
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

    writeTestHwConvOutput(&testConvPipe);

    sippRdFileU8 (TestHwConv_dmaIn0_buffer, SIPP_HW_CONV_BUFFER_SIZE, "../../../../../../../resources/0_0_convOutput_1296x972_P400.rgb");
    sippDbgCompareU8((u8*)TestHwConv_dmaIn0_buffer,    (uint8_t*)TestHwConv_dmaOut0_buffer,  SIPP_HW_CONV_BUFFER_SIZE);

    #ifdef SIPP_VCS
    unitTestFinalReport ();
    #endif

   return 0;
}
