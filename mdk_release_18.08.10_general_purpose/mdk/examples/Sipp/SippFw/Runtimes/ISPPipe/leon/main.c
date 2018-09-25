///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief 1080p variant of ISPPipe running in oPipe runtime
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
#define SIPP_TEST_NUM_FRAMES  5
#define USE_PRECOMP_SCHED

/////////////////////////////////////////////////////
// Global variables
/////////////////////////////////////////////////////

ISPPipe myPl;
volatile u32 testComplete   = 0;
volatile u32 frameDone      = 0;
volatile u32 framesComplete = 0;

/////////////////////////////////////////////////////
// Code
/////////////////////////////////////////////////////

void ISPPipeCreateParams ( );

void appSippCallback ( SippPipeline *             pPipeline,
                       eSIPP_PIPELINE_EVENT       eEvent,
                       SIPP_PIPELINE_EVENT_DATA * ptEventData
                     )
{
    UNUSED(pPipeline);
    UNUSED(ptEventData);

    if (eEvent == eSIPP_PIPELINE_FRAME_DONE)
    {
        frameDone = 1;
        framesComplete++;
        printf ("appSippCallback : Frame done event received : %lu\n", framesComplete);

    }

}

void appTask (SippPipeline * pPipeline)
{
    while (testComplete == 0)
    {
        if (frameDone)
        {
            frameDone = 0;

            if (framesComplete == SIPP_TEST_NUM_FRAMES)
            {
                testComplete = 1;
                printf ("appTask : %lu framesComplete Test proceeding to checks and termination\n", framesComplete);
            }
            else
            {
                printf("Call process frame async\n");
                sippProcessFrameNB (pPipeline);
            }
        }
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

    #ifdef USE_PRECOMP_SCHED
    sippPipeSetNumLinesPerBuf (myPl.dmaIn0, 0, 24);
    sippPipeSetNumLinesPerBuf (myPl.debayerMa2x5x0, 1, 32);
    sippPipeSetNumLinesPerBuf (myPl.sharpenMa2x5x0, 0, 8);
    sippPipeSetNumLinesPerBuf (myPl.lutMa2x5x0, 0, 16);
    #endif

    // Register callback for async API
    sippRegisterEventCallback (myPl.pl,
                               appSippCallback);
    ISPPipeCreateParams ();
    configISPPipe (&myPl);

    readISPPipeInput (&myPl);

    // Set a flag to enable consideration of oPipe runtime (that which we wish to use here)
    myPl.pl->flags |= PLF_CONSIDER_OPIPE_RT;

    sippFinalizePipeline (myPl.pl);

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

    appTask (myPl.pl);

    DrvTimerGetSystemTicks64 (&endT);
    frameT = endT - startT;
    printf(" Performance: %f cc/pix \n", ((float)frameT) / (SIPP_ISP_PIPE_BUFFER_WIDTH * SIPP_ISP_PIPE_BUFFER_HEIGHT * SIPP_TEST_NUM_FRAMES));

    writeISPPipeOutput (&myPl);

    #if defined(MA2150)
    sippRdFileU8 ((UInt8*)ISPPipe_dmaIn0_buffer, SIPP_ISP_PIPE_BUFFER_WIDTH * SIPP_ISP_PIPE_BUFFER_HEIGHT * SIPP_ISP_PIPE_BUFFER_OUTPUT_NUM_PLANES, "../../../../../../../resources/ref_isp_out_ma2150_1920x1080_P444.yuv");
    #elif defined(MA2450)
    sippRdFileU8 ((UInt8*)ISPPipe_dmaIn0_buffer, SIPP_ISP_PIPE_BUFFER_WIDTH * SIPP_ISP_PIPE_BUFFER_HEIGHT * SIPP_ISP_PIPE_BUFFER_OUTPUT_NUM_PLANES, "../../../../../../../resources/ref_isp_out_ma2450_1920x1080_P444.yuv");
    #endif

    sippDbgCompareU8((UInt8*)ISPPipe_dmaIn0_buffer, ISPPipe_dmaOut0_buffer, SIPP_ISP_PIPE_BUFFER_WIDTH * SIPP_ISP_PIPE_BUFFER_HEIGHT * SIPP_ISP_PIPE_BUFFER_OUTPUT_NUM_PLANES);

    #ifdef SIPP_VCS
    unitTestFinalReport();
    #endif

    return 0;
}
