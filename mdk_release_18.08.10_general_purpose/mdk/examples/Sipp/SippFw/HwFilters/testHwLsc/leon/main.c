///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Raw filter unit test modified from C model test...
///            /lsctest/test.cpp

/////////////////////////////////////////////////////
// Include files
/////////////////////////////////////////////////////

#include <sipp.h>
#include <sippTestCommon.h>

#include "bayerGainMeshMap.h"

#if defined(SIPP_VCS)
#include <UnitTestApi.h>
#include <DrvTimer.h>
#include <DrvDdr.h>
#endif

#include "testData.h"
#include "testHwLsc.h"
/////////////////////////////////////////////////////
// Test Macros
/////////////////////////////////////////////////////

#define SIPP_TEST_SYNC_API  // Set to use the synchronous API
//#define SIPP_USE_PRECALC_SCHEDULE



/////////////////////////////////////////////////////
// Global variables
/////////////////////////////////////////////////////

TestHwLsc lscPipe;




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
    unitTestInit();
    DrvTimerInit();
    DrvDdrInitialise(NULL);
    #endif

    buildTestHwLsc(&lscPipe);

    if (sippPipeGetErrorStatus (lscPipe.pl))
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
    sippRegisterEventCallback (lscPipe.pl,
                               appSippCallback);
    #endif

    configTestHwLsc(&lscPipe);

    readTestHwLscInput(&lscPipe);

    #ifdef SIPP_USE_PRECALC_SCHEDULE
    // Read offline schedule decisions
    dbgSchedInit (lscPipe.pl);
    #endif

    #ifdef SIPP_TEST_SYNC_API
    sippProcessFrame (lscPipe.pl);

    if (sippPipeGetErrorStatus (lscPipe.pl))
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
    sippProcessFrameNB (lscPipe.pl);

    if (sippPipeGetErrorStatus (lscPipe.pl))
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

    writeTestHwLscOutput(&lscPipe);

    #if 0
    // Regenerate test data
    sippRdFileU8 ((u8*)TestHwLsc_dmaIn0_buffer, SIPP_HW_LSC_BUFFER_SIZE * sizeof(u16), "../../../../../../../resources/Ref_outLSC_3264x2448.raw");
    sippDbgCompareU8 ((u8*)TestHwLsc_dmaIn0_buffer, (u8*)TestHwLsc_dmaOut0_buffer, SIPP_HW_LSC_BUFFER_SIZE * sizeof(u16));
    #endif

    #ifdef SIPP_VCS
    unitTestFinalReport();
    #endif

    return 0;
}
