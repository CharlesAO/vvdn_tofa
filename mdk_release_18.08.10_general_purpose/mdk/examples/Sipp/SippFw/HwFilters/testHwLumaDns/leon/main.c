///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

#include <sipp.h>
#include <sippTestCommon.h>
#include "testHwLumaDns.h"
#if defined(SIPP_VCS)
#include <UnitTestApi.h>
#include <DrvTimer.h>
#include <DrvDdr.h>

#endif

/////////////////////////////////////////////////////
// Test specific macros
/////////////////////////////////////////////////////

#define SIPP_TEST_SYNC_API  // Set to use the synchronous API
//#define SIPP_USE_PRECALC_SCHEDULE


/////////////////////////////////////////////////////
// Global variables
/////////////////////////////////////////////////////

TestHwLumaDns testLumaDnsPipe;

#ifndef SIPP_TEST_SYNC_API
volatile u32 testComplete = 0;
#endif

u32 gaussLut[4] = {0x9abddeef,0x12234578,0x0,0x0};
u32 gammaLut[5] = {0x665a4900,0x837d776f,0xa8890089,0xeadecfbe,0x1000fff5};

u8 DDR_DATA SippHwLumaDns_ref_buffer[SIPP_HW_LUM_DNS_BUFFER_SIZE];

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

    buildTestHwLumaDns (&testLumaDnsPipe);

    if (sippPipeGetErrorStatus (testLumaDnsPipe.pl))
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
    sippRegisterEventCallback (testLumaDnsPipe.pl,
                               appSippCallback);
    #endif

    configTestHwLumaDns (&testLumaDnsPipe);
    readTestHwLumaDnsInput(&testLumaDnsPipe);
    //sippRdFileU8 (SippHwLumaDns_dmaIn0_buffer, SIPP_HW_LUM_DNS_BUFFER_SIZE, "../../../../../../../resources/OCT_384x290_I_Y_dns_luma_8bpp.raw");

    #ifdef SIPP_USE_PRECALC_SCHEDULE
    // Read offline schedule decisions
    dbgSchedInit(testLumaDnsPipe.pl);
    #endif

    #ifdef SIPP_TEST_SYNC_API
    sippProcessFrame(testLumaDnsPipe.pl);

    if (sippPipeGetErrorStatus (testLumaDnsPipe.pl))
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
    sippProcessFrameNB(testLumaDnsPipe.pl);

    if (sippPipeGetErrorStatus (testLumaDnsPipe.pl))
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
    writeTestHwLumaDnsOutput(&testLumaDnsPipe);
    sippRdFileU8(SippHwLumaDns_ref_buffer, SIPP_HW_LUM_DNS_BUFFER_SIZE, "../../../../../../../resources/Ref_lumaOutput_384x290.raw");
    sippDbgCompareU8((u8*)SippHwLumaDns_ref_buffer,    (uint8_t*)TestHwLumaDns_dmaOut0_buffer,  SIPP_HW_LUM_DNS_BUFFER_SIZE);

    #ifdef SIPP_VCS
    unitTestFinalReport();
    #endif

   return 0;
}
