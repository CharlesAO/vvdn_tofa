///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

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
#include "testHwSigmaDenoise.h"



#define SIPP_TEST_SYNC_API  // Set to use the synchronous API
//#define SIPP_USE_PRECALC_SCHEDULE


/////////////////////////////////////////////////////
// Global variables
/////////////////////////////////////////////////////
TestHwSigmaDenoise sigmaDenoisePipe;






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

    buildTestHwSigmaDenoise(&sigmaDenoisePipe);

    if (sippPipeGetErrorStatus (sigmaDenoisePipe.pl))
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
    sippRegisterEventCallback (sigmaDenoisePipe.pl,
                               appSippCallback);
    #endif

    configTestHwSigmaDenoise(&sigmaDenoisePipe);
    readTestHwSigmaDenoiseInput(&sigmaDenoisePipe);
    #ifdef SIPP_USE_PRECALC_SCHEDULE
    // Read offline schedule decisions
    dbgSchedInit(pPipe);
    #endif

    #ifdef SIPP_TEST_SYNC_API
    sippProcessFrame(sigmaDenoisePipe.pl);

    if (sippPipeGetErrorStatus (sigmaDenoisePipe.pl))
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
    sippProcessFrameNB(sigmaDenoisePipe.pl);

    if (sippPipeGetErrorStatus (sigmaDenoisePipe.pl))
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

    writeTestHwSigmaDenoiseOutput(&sigmaDenoisePipe);

    sippRdFileU8((u8*)TestHwSigmaDenoise_dmaIn0_buffer, SIPP_HW_SIGMA_DNS_BUFFER_SIZE, "../../../../../../../resources/Ref_sigmaOutput_1600x1300_10bpp.raw");
    sippDbgCompareU8((u8*)TestHwSigmaDenoise_dmaIn0_buffer, (u8*)TestHwSigmaDenoise_dmaOut0_buffer, SIPP_HW_SIGMA_DNS_BUFFER_SIZE);

    #ifdef SIPP_VCS
    unitTestFinalReport();
    #endif

    return 0;
}
