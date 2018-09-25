///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief  Unite test taken from sipphw/dogltmtest/test.cpp
///

#include <sipp.h>
#include <sippTestCommon.h>

#if defined(SIPP_VCS)
#include <UnitTestApi.h>
#include <DrvTimer.h>
#include <DrvDdr.h>
#endif

#include "testHwDogLtmSetup.h"
#include "testHwDoGLTM.h"

/////////////////////////////////////////////////////
// Test specific macros
/////////////////////////////////////////////////////

#define SIPP_TEST_SYNC_API  // Set to use the synchronous API
//#define SIPP_USE_PRECALC_SCHEDULE

/////////////////////////////////////////////////////
// Global variables
/////////////////////////////////////////////////////

#ifdef SIPP_USE_PRECALC_SCHEDULE
#include "pcDumpSchedule.hh"
#endif

TestHwDoGLTM testDogLTMPipe;

#ifndef SIPP_TEST_SYNC_API
volatile u32 testComplete = 0;
#endif

const uint16_t ltm_curves[NPOINTS][NCURVES] =
{
   {    0,     0,     0,     0,     0,     0,     0,     0 },
   {  358,   210,   179,   169,   164,   162,   160,   158 },
   {  713,   534,   420,   379,   358,   346,   338,   333 },
   { 1061,   892,   722,   631,   584,   556,   537,   525 },
   { 1401,  1246,  1068,   924,   841,   791,   757,   734 },
   { 1729,  1593,  1426,  1251,  1129,  1051,   999,   961 },
   { 2043,  1928,  1779,  1602,  1445,  1336,  1261,  1207 },
   { 2342,  2248,  2123,  1960,  1783,  1644,  1545,  1472 },
   { 2623,  2550,  2451,  2312,  2135,  1972,  1847,  1753 },
   { 2888,  2834,  2759,  2650,  2493,  2316,  2167,  2052 },
   { 3134,  3096,  3044,  2966,  2844,  2669,  2502,  2366 },
   { 3361,  3338,  3304,  3254,  3171,  3027,  2849,  2694 },
   { 3570,  3558,  3539,  3511,  3464,  3373,  3203,  3034 },
   { 3762,  3757,  3749,  3737,  3716,  3675,  3561,  3382 },
   { 3937,  3935,  3933,  3931,  3926,  3916,  3885,  3737 },
   { 4095,  4095,  4095,  4095,  4095,  4095,  4095,  4095 }
};

u8 KCSet0[] =
{
    0x01,
    0x01,
    0x01,
    0x10,
    0x3d,
    0x60
};

u8 KCSet1[] =
{
    0x05,
    0x08,
    0x0b,
    0x10,
    0x14,
    0x19,
    0x1c,
    0x1e
};

u8 DDR_DATA SippHwDoGLTM_dmaRef_buffer[SIPP_HW_DOG_LTM_BUFFER_SIZE];

///////////////////////////////////////////////////////////
// Code
///////////////////////////////////////////////////////////


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

    sippPlatformInit();

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

    buildTestHwDoGLTM(&testDogLTMPipe);

    if (sippPipeGetErrorStatus (testDogLTMPipe.pl))
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
   // register callback for async API
    sippRegisterEventCallback (testDogLTMPipe.pl,
                               appSippCallback);
    #endif

    configTestHwDoGLTM(&testDogLTMPipe);

    readTestHwDoGLTMInput(&testDogLTMPipe);

    #ifdef SIPP_USE_PRECALC_SCHEDULE
    //Read offline schedule decisions
    dbgSchedInit(testDogLTMPipe.pl);
    #endif

    #ifdef SIPP_TEST_SYNC_API
    sippProcessFrame(testDogLTMPipe.pl);

    if (sippPipeGetErrorStatus (testDogLTMPipe.pl))
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
    sippProcessFrameNB(testDogLTMPipe.pl);

    if (sippPipeGetErrorStatus (testDogLTMPipe.pl))
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

    writeTestHwDoGLTMOutput(&testDogLTMPipe);

    #if 1
    sippRdFileU8 (SippHwDoGLTM_dmaRef_buffer, SIPP_HW_DOG_LTM_BUFFER_SIZE, "../../../../../../../resources/Ref_dogLtmOutput_8bit_1584x1290.raw");
    sippDbgCompareU8 (SippHwDoGLTM_dmaRef_buffer,TestHwDoGLTM_dmaOut0_buffer, SIPP_HW_DOG_LTM_BUFFER_SIZE);
    #endif

    #ifdef SIPP_VCS
    unitTestFinalReport();
    #endif

   return 0;
}
