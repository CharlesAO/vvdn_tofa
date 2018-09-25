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

#include "testHwColorCombSetup.h"
#include "testHwColorComb.h"

/////////////////////////////////////////////////////
// Test Macros
/////////////////////////////////////////////////////

#define SIPP_TEST_SYNC_API  // Set to use the synchronous API
//#define SIPP_USE_PRECALC_SCHEDULE

/////////////////////////////////////////////////////
// Global variables
/////////////////////////////////////////////////////

#include "colorComb3dLut.h"

TestHwColorComb testColorComb;

#ifndef SIPP_TEST_SYNC_API
volatile u32 testComplete = 0;
#endif

u16 colCorrMatrix[] = {
  0x06ea,          // 1.729477f
  0xfeca,          // -0.571230f
  0xffba,          // -0.158720f
  0xfdb8,          // -0.303146f
  0x06cd,          // 1.701059f
  0xfd52,          // -0.397836f
  0xff5e,          // -0.069065f
  0xfe69,          // -0.670674f
  0x06f5,          // 1.739618f
};

/////////////////////////////////////////////////////
// Extern function prototypes
/////////////////////////////////////////////////////

/////////////////////////////////////////////////////
// Code
/////////////////////////////////////////////////////

#ifdef SIPP_USE_PRECALC_SCHEDULE
#include "pcDumpSchedule.hh"
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

    buildTestHwColorComb (&testColorComb);

    if (sippPipeGetErrorStatus (testColorComb.pl))
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

    readTestHwColorCombInput(&testColorComb);

    #ifndef SIPP_TEST_SYNC_API
    // Register callback for async API
    sippRegisterEventCallback (testColorComb.pl,
                               appSippCallback);
    #endif

    configTestHwColorComb (&testColorComb);

    #ifdef SIPP_USE_PRECALC_SCHEDULE
    // Read offline schedule decisions
    dbgSchedInit (testColorComb.pl);
    #endif

    #ifdef SIPP_TEST_SYNC_API
    sippProcessFrame (testColorComb.pl);

    if (sippPipeGetErrorStatus (testColorComb.pl))
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
    sippProcessFrameNB (testColorComb.pl);

    if (sippPipeGetErrorStatus (testColorComb.pl))
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

    #ifdef SIPP_PC
    sippGenericRuntimeWaitIsrThreadTerm ();
    #endif

    writeTestHwColorCombOutput(&testColorComb);

    #ifdef SIPP_VCS
    unitTestFinalReport();
    #endif

    return 0;
}
