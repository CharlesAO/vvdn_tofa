///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Convolution HW filter unit test for oPipe runtime
///

#include <sipp.h>
#include <sippTestCommon.h>

#if defined(SIPP_VCS)
#include <UnitTestApi.h>
#include <DrvTimer.h>
#include <DrvDdr.h>
#include <swcLeonUtils.h>
#endif

#include "HwConv.h"

/////////////////////////////////////////////////////
// Test specific macros
/////////////////////////////////////////////////////
#define DYNAMIC_IRQ_SIPP_TEST_0 IRQ_DYNAMIC_0
#define DYNAMIC_IRQ_SIPP_TEST_1 IRQ_DYNAMIC_1
#define DYNAMIC_IRQ_SIPP_TEST_2 IRQ_DYNAMIC_2

/////////////////////////////////////////////////////
// Global variables
/////////////////////////////////////////////////////
HwConv testConvPipe;
volatile u32 testComplete = 0;

/////////////////////////////////////////////////////
// Code
/////////////////////////////////////////////////////

void appSippCallback ( SippPipeline *             pPipeline,
                       eSIPP_PIPELINE_EVENT       eEvent,
                       SIPP_PIPELINE_EVENT_DATA * ptEventData)
{
    UNUSED(pPipeline);
    UNUSED(ptEventData);

    if (eEvent == eSIPP_PIPELINE_FRAME_DONE)
    {
        printf ("appSippCallback : Frame done event received : Test proceeding to checks and termination\n");
        testComplete = 1;
    }
}

int main (int argc, char *argv[])
{
    UNUSED (argc);
    UNUSED (argv);
    u32    sippErrorLog[SIPP_ERROR_HISTORY_SIZE];

    uint64_t startT, endT, frameT;

    sippPlatformInit ();

    if (PROCESS_LEON_OS == swcWhoAmI())
    {
        printf ("App Starting on LEON OS\n");
        printf ("Please make sure IRQ sources for dynamically route between LeonOS and LeonRT are set\n");

        // Re-route irqs to Leon OS
        sippDynRouteIrq (DYNAMIC_IRQ_SIPP_TEST_0, DYNAMIC_IRQ_SIPP_TEST_1, DYNAMIC_IRQ_SIPP_TEST_2);
    }
    else
    {
        printf ("App Starting on LEON RT\n");
    }

    sippPlatformInitAsync ();

    #ifdef SIPP_VCS
    unitTestInit ();
    DrvTimerInit ();
    DrvDdrInitialise (NULL);
    #endif

    buildHwConv (&testConvPipe);

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

    // Register callback
    sippRegisterEventCallback (testConvPipe.pl,
                               appSippCallback);

    configHwConv (&testConvPipe);

    readHwConvInput (&testConvPipe);

    // Set a flag to enable consideration of oPipe runtime (that which we wish to use here)
    testConvPipe.pl->flags |= PLF_CONSIDER_OPIPE_RT;

    sippFinalizePipeline (testConvPipe.pl);

    DrvTimerGetSystemTicks64 (&startT);

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

    DrvTimerGetSystemTicks64 (&endT);
    frameT = endT - startT;
    printf(" Performance: %f cc/pix \n", ((float)frameT) / (SIPP_HW_CONV_BUFFER_WIDTH*SIPP_HW_CONV_BUFFER_HEIGHT));

    writeHwConvOutput(&testConvPipe);

    sippRdFileU8 (HwConv_dmaIn0_buffer, SIPP_HW_CONV_BUFFER_SIZE, "../../../../../../../resources/0_0_convOutput_1296x972_P400.rgb");
    sippDbgCompareU8((UInt8*)HwConv_dmaIn0_buffer, (UInt8*)HwConv_dmaOut0_buffer, SIPP_HW_CONV_BUFFER_SIZE);

    #ifdef SIPP_VCS
    unitTestFinalReport ();
    #endif

   return 0;
}
