///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief  Unit test taken from sipphw/genchromatest/test.cpp
///

#include <sipp.h>
#include <sippTestCommon.h>

#if defined(SIPP_VCS)
#include <UnitTestApi.h>
#include <DrvTimer.h>
#include <DrvDdr.h>
#endif

#include "testHwGenChromaSetup.h"
#include "testHwGenChroma.h"


/////////////////////////////////////////////////////
// Test specific macros
/////////////////////////////////////////////////////

#define SIPP_TEST_SYNC_API  // Set to use the synchronous API
//#define SIPP_USE_PRECALC_SCHEDULE



//////////////////////////////////////////////////////
// Notes
//
// 1) Chroma Gen does a vertical and horizontal downscale by 2 using a 2x2 kernel to produce
//    one output. If the input image dimensions are odd then what resolution output is porduced?
//    In the horizontal direction we will produce one extra output for the 'spare' input
//    In the vertical direction we produce no further output for the 'spare' input
// 2)


/////////////////////////////////////////////////////
// Global variables
/////////////////////////////////////////////////////

#ifdef SIPP_USE_PRECALC_SCHEDULE
#include "pcDumpSchedule.hh"
#endif

TestHwGenChroma testGenChromaPipe;

#ifndef SIPP_TEST_SYNC_API
volatile u32 testComplete = 0;
#endif

/////////////////////////////////////////////////////
// Code
/////////////////////////////////////////////////////

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

    buildTestHwGenChroma (&testGenChromaPipe);

    if (sippPipeGetErrorStatus (testGenChromaPipe.pl))
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
    sippRegisterEventCallback (testGenChromaPipe.pl,
                               appSippCallback);
    #endif

    configTestHwGenChroma (&testGenChromaPipe);

    readTestHwGenChromaInput (&testGenChromaPipe);

    #ifdef SIPP_USE_PRECALC_SCHEDULE
    // Read offline schedule decisions
    dbgSchedInit(testGenChromaPipe.pl);
    #endif

    #ifdef SIPP_TEST_SYNC_API
    sippProcessFrame(testGenChromaPipe.pl);

    if (sippPipeGetErrorStatus (testGenChromaPipe.pl))
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
    sippProcessFrameNB(testGenChromaPipe.pl);

    if (sippPipeGetErrorStatus (testGenChromaPipe.pl))
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

    writeTestHwGenChromaOutput (&testGenChromaPipe);
//    sippRdFileU8(SippHwChromaGen_dmaRef_buffer, SIPP_HW_CHR_GEN_BUFFER_SIZE, "../../../hwModels/ma2150/test_images/uvdenoise_unitTestOut.rgb");
//    sippDbgCompareU8((u8*)SippHwChromaGen_dmaRef_buffer,(uint8_t*)SippHwChromaGen_dmaOut0_buffer,SIPP_HW_CHR_GEN_BUFFER_SIZE);

    #ifdef SIPP_VCS
    unitTestFinalReport();
    #endif

    return 0;
}
