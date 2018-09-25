///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///
#if defined(SIPP_VCS)
	#include <DrvTimer.h>
	#include <DrvDdr.h>
#endif

#include "app_config.h"
#include "ISPPipe.h"

/////////////////////////////////////////////////////
// Test Macros
/////////////////////////////////////////////////////
#define SIPP_TEST_SYNC_API
//#define SIPP_USE_PRECALC_SCHEDULE
//#define DISABLE_OPIPE_CONNECTIONS

/////////////////////////////////////////////////////
// Global variables
/////////////////////////////////////////////////////

ISPPipe myPl;

#ifndef SIPP_TEST_SYNC_API
volatile u32 testComplete = 0;
#endif

/////////////////////////////////////////////////////
// Code
/////////////////////////////////////////////////////

#ifdef SIPP_USE_PRECALC_SCHEDULE
#include "pcDumpSchedule.hh"
#endif

/////////////////////////////////////////////////////

void ISPPipeCreateParams ( );

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

    #ifndef SIPP_TEST_SYNC_API
    testComplete = 0;
    #endif

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

    buildISPPipe (&myPl);

    if (sippPipeGetErrorStatus (myPl.pl))
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
    sippRegisterEventCallback (myPl.pl,
                               appSippCallback);
    #endif

    ISPPipeCreateParams ();

    configISPPipe (&myPl);

    #ifdef DISABLE_OPIPE_CONNECTIONS
    myPl.pl->flags |= (PLF_DISABLE_OPIPE_CONS);
    #endif

    #ifdef SIPP_USE_PRECALC_SCHEDULE
    // Read offline schedule decisions
    dbgSchedInit(myPl.pl);
    #endif

    readISPPipeInput (&myPl);
    #ifdef SIPP_TEST_SYNC_API
    sippProcessFrame (myPl.pl);

    if (sippPipeGetErrorStatus (myPl.pl))
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
    sippProcessFrameNB (myPl.pl);

    if (sippPipeGetErrorStatus (myPl.pl))
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

    writeISPPipeOutput (&myPl);

    #ifdef SIPP_VCS
    unitTestFinalReport();
    #endif

    return 0;
}
