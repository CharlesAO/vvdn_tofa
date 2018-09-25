///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief  Unite test taken from sipphw/uvdenoisetest/test.cpp
///

#include <sipp.h>
#include <sippTestCommon.h>

#if defined(SIPP_VCS)
#include <UnitTestApi.h>
#include <DrvTimer.h>
#include <DrvDdr.h>
#endif

#include "testHwChromaDnsSetup.h"
#include "testHwChromaDns.h"

/////////////////////////////////////////////////////
// Test specific macros
/////////////////////////////////////////////////////

#define SIPP_TEST_SYNC_API  // Set to use the synchronous API
//#define SIPP_USE_PRECALC_SCHEDULE

/////////////////////////////////////////////////////
// Global variables
/////////////////////////////////////////////////////

TestHwChromaDns testChromaDnsPipe;

#ifndef SIPP_TEST_SYNC_API
volatile u32 testComplete = 0;
#endif

UInt8      DDR_DATA TestHwChromaDns_dmaRef0_buffer[792      * 645     * 3  ]ALIGNED(8);

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

    buildTestHwChromaDns(&testChromaDnsPipe);

    if (sippPipeGetErrorStatus (testChromaDnsPipe.pl))
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
    sippRegisterEventCallback (testChromaDnsPipe.pl,
                               appSippCallback);
    #endif


    configTestHwChromaDns(&testChromaDnsPipe);
    readTestHwChromaDnsInput(&testChromaDnsPipe);

    #ifdef SIPP_USE_PRECALC_SCHEDULE
    // Read offline schedule decisions
    dbgSchedInit (pPipe);
    #endif

    #ifdef SIPP_TEST_SYNC_API
    sippProcessFrame(testChromaDnsPipe.pl);

    if (sippPipeGetErrorStatus (testChromaDnsPipe.pl))
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
    sippProcessFrameNB(testChromaDnsPipe.pl);

    if (sippPipeGetErrorStatus (testChromaDnsPipe.pl))
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

    writeTestHwChromaDnsOutput(&testChromaDnsPipe);

    #if 0
    TBD - Test data needs regnerated after c-model updates
    sippRdFileU8(TestHwChromaDns_dmaRef0_buffer, SIPP_HW_CHR_DNS_BUFFER_SIZE, "../../../../../../../resources/uvdenoise_unitTestOut.rgb");
    sippDbgCompareU8((u8*)TestHwChromaDns_dmaRef0_buffer,(uint8_t*)TestHwChromaDns_dmaOut0_buffer,SIPP_HW_CHR_DNS_BUFFER_SIZE);
    #endif

    #ifdef SIPP_VCS
    unitTestFinalReport();
    #endif

   return 0;
}
