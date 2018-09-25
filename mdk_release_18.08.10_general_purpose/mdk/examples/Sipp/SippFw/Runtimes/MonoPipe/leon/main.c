///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///

#if defined(SIPP_VCS)
#include <DrvTimer.h>
#include <DrvDdr.h>
#endif

#include "app_config.h"
#include "monoPipe.h"

/////////////////////////////////////////////////////
// Test Macros
/////////////////////////////////////////////////////

#define SIPP_TEST_NUM_FRAMES            3
#define USE_PRECOMP_SCHED

/////////////////////////////////////////////////////
// Global variables
/////////////////////////////////////////////////////

MonoPipe myPl;
volatile u32 testComplete = 0;

/////////////////////////////////////////////////////
// Code
/////////////////////////////////////////////////////

void MonoPipeCreateParams ();

//############################################################################
void appSippCallback (SippPipeline *             pPipeline,
                      eSIPP_PIPELINE_EVENT       eEvent,
                      SIPP_PIPELINE_EVENT_DATA * ptEventData)
{
    UNUSED (pPipeline);
    UNUSED (ptEventData);

    if (eEvent == eSIPP_PIPELINE_FRAME_DONE)
    {
        testComplete = 1;
    }

}

//############################################################################

int main (int argc, char *argv[])
{
    UNUSED (argc);
    UNUSED (argv);
    u32    sippErrorLog[SIPP_ERROR_HISTORY_SIZE];

    uint64_t startT, endT, frameT;

    sippPlatformInit();

    if (PROCESS_LEON_OS == swcWhoAmI())
    {
        printf ("App Starting on LEON OS\n");
        printf ("Please make sure IRQ sources for dynamically route between LeonOS and LeonRT are set\n");
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

    buildMonoPipe (&myPl);

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

    #ifdef USE_PRECOMP_SCHED
    sippPipeSetNumLinesPerBuf (myPl.dmaIn0, 0, 24);
    sippPipeSetNumLinesPerBuf (myPl.rawMa2x5x0, 0, 16);
    sippPipeSetNumLinesPerBuf (myPl.sharpenMa2x5x0, 0, 16);
    sippPipeSetNumLinesPerBuf (myPl.lutMa2x5x0, 0, 16);
    #endif

    // Register callback
    sippRegisterEventCallback (myPl.pl,
                               appSippCallback);

    MonoPipeCreateParams ();
    configMonoPipe (&myPl);

    readMonoPipeInput(&myPl);

    // Set a flag to enable consideration of oPipe runtime (that which we wish to use here)
    myPl.pl->flags |= PLF_CONSIDER_OPIPE_RT;

    sippFinalizePipeline (myPl.pl);

    for (u32 run = 0; run < SIPP_TEST_NUM_FRAMES; run++)
    {
        DrvTimerGetSystemTicks64 (&startT);

        // Clear flag before each run
        testComplete = 0;
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

        DrvTimerGetSystemTicks64(&endT);
        frameT = endT - startT;
        printf(" Performance: %f cc/pix \n", ((float)frameT) / (MONO_PIPE_INPUT_BUFFER_WIDTH * MONO_PIPE_INPUT_BUFFER_HEIGHT));
    }

    writeMonoPipeOutput (&myPl);

    sippRdFileU8 ((UInt8*)MonoPipe_dmaIn0_buffer, MONO_PIPE_INPUT_BUFFER_WIDTH * MONO_PIPE_INPUT_BUFFER_HEIGHT , "../../../../../../../resources/ref_monoPipe_out_640x480.raw");
    sippDbgCompareU8((UInt8*)MonoPipe_dmaIn0_buffer, MonoPipe_dmaOut0_buffer, MONO_PIPE_INPUT_BUFFER_WIDTH * MONO_PIPE_INPUT_BUFFER_HEIGHT);

    #ifdef SIPP_VCS
    unitTestFinalReport();
    #endif

    return 0;
}
