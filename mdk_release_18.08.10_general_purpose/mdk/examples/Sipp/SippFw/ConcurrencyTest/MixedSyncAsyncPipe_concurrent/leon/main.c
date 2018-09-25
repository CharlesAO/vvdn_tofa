///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application to test running SIPP pipelines concurrently
///            This test uses pipelines which have no conflicting SHAVE or
///            HW filter resource and therefore should be capable of running
///            concurrently

#include <sipp.h>
#include <sippTestCommon.h>
#include <filters/conv5x5/conv5x5.h>
#include <filters/boxFilter5x5/boxFilter5x5.h>
#include <string.h>

#if defined(SIPP_VCS)
#include <UnitTestApi.h>
#include <DrvTimer.h>
#include <DrvDdr.h>
#endif

#include "ConcurrencyAppTypes.h"

/////////////////////////////////////////////////////
// Test specific macros
/////////////////////////////////////////////////////

#ifndef SIPP_PC
#define SIPP_USE_PRECALC_SCHEDULE
#endif
#define CONCURRENCY_APP_NUM_PIPELINES       0x2
#define CONCURRENCY_APP_FRAMES_PER_PIPE     0x3
#define CONCURRENCY_TEST_CREATE_CONFLICT    0             // Set non-zero to create conflicts among the SHAVE resource requests
                                                          // which cause the pipelines to be run serially

// Pipeline 0 Setup
#define CONCURRENCY_APP_PIPE0_WIDTH         1280
#define CONCURRENCY_APP_PIPE0_HEIGHT        720
#define CONCURRENCY_APP_PIPE0_NUM_PLANES    1
#define CONCURRENCY_APP_PIPE0_ASYNC         1
typedef u8  t_Pipe0;

// Pipeline 1 Setup
#define CONCURRENCY_APP_PIPE1_WIDTH         1280
#define CONCURRENCY_APP_PIPE1_HEIGHT        720
#define CONCURRENCY_APP_PIPE1_NUM_PLANES    1
#define CONCURRENCY_APP_PIPE1_ASYNC         0             // Mark this pipe as sync
typedef u8  t_Pipe1;

#if CONCURRENCY_APP_NUM_PIPELINES > 0x2
// Pipeline 2 Setup
#define CONCURRENCY_APP_PIPE2_WIDTH         1280
#define CONCURRENCY_APP_PIPE2_HEIGHT        720
#define CONCURRENCY_APP_PIPE2_NUM_PLANES    1
#define CONCURRENCY_APP_PIPE2_ASYNC         1
typedef u8  t_Pipe2;

#endif

#if CONCURRENCY_APP_NUM_PIPELINES > 0x3
// Pipeline 3 Setup
#define CONCURRENCY_APP_PIPE3_WIDTH         1280
#define CONCURRENCY_APP_PIPE3_HEIGHT        720
#define CONCURRENCY_APP_PIPE3_NUM_PLANES    1
#define CONCURRENCY_APP_PIPE3_ASYNC         1
typedef u8  t_Pipe3;

#endif


/////////////////////////////////////////////////////
// Global variables
/////////////////////////////////////////////////////

#include "appParams.h"

u32    sippErrorLog[SIPP_ERROR_HISTORY_SIZE];
volatile u32       testComplete = 0;
ConcurrencyAppPipe concurrencyPipeList[CONCURRENCY_APP_NUM_PIPELINES];

#if CONCURRENCY_APP_NUM_PIPELINES > 0x3
#if CONCURRENCY_TEST_CREATE_CONFLICT > 0
u32                sliceFirst[CONCURRENCY_APP_NUM_PIPELINES] = {0,3,6,9};
u32                sliceLast[CONCURRENCY_APP_NUM_PIPELINES]  = {3,6,9,11};
#else
u32                sliceFirst[CONCURRENCY_APP_NUM_PIPELINES] = {0,3,6,9};
u32                sliceLast[CONCURRENCY_APP_NUM_PIPELINES]  = {2,5,8,11};
#endif
#elif CONCURRENCY_APP_NUM_PIPELINES > 0x2
#if CONCURRENCY_TEST_CREATE_CONFLICT > 0
u32                sliceFirst[CONCURRENCY_APP_NUM_PIPELINES] = {0,4,8};
u32                sliceLast[CONCURRENCY_APP_NUM_PIPELINES]  = {4,8,11};
#else
u32                sliceFirst[CONCURRENCY_APP_NUM_PIPELINES] = {0,4,8};
u32                sliceLast[CONCURRENCY_APP_NUM_PIPELINES]  = {3,7,11};
#endif
#else
#if CONCURRENCY_TEST_CREATE_CONFLICT > 0
u32                sliceFirst[CONCURRENCY_APP_NUM_PIPELINES] = {0,6};
u32                sliceLast[CONCURRENCY_APP_NUM_PIPELINES]  = {6,11};
#else
u32                sliceFirst[CONCURRENCY_APP_NUM_PIPELINES] = {0,6};
u32                sliceLast[CONCURRENCY_APP_NUM_PIPELINES]  = {5,11};
#endif
#endif

u8 *               pmBinImg[CONCURRENCY_APP_NUM_PIPELINES];  // How do we get the different binaries? Or do we use one binary with different entry points?? Programmers Guide...


t_Pipe0 DDR_DATA DMA0_InputBuffer[CONCURRENCY_APP_PIPE0_WIDTH * CONCURRENCY_APP_PIPE0_HEIGHT * CONCURRENCY_APP_PIPE0_NUM_PLANES] ALIGNED(8);
t_Pipe1 DDR_DATA DMA1_InputBuffer[CONCURRENCY_APP_PIPE1_WIDTH * CONCURRENCY_APP_PIPE1_HEIGHT * CONCURRENCY_APP_PIPE1_NUM_PLANES] ALIGNED(8);
#if CONCURRENCY_APP_NUM_PIPELINES > 0x2
t_Pipe2 DDR_DATA DMA2_InputBuffer[CONCURRENCY_APP_PIPE2_WIDTH * CONCURRENCY_APP_PIPE2_HEIGHT * CONCURRENCY_APP_PIPE2_NUM_PLANES] ALIGNED(8);
#endif
#if CONCURRENCY_APP_NUM_PIPELINES > 0x3
t_Pipe3 DDR_DATA DMA3_InputBuffer[CONCURRENCY_APP_PIPE3_WIDTH * CONCURRENCY_APP_PIPE3_HEIGHT * CONCURRENCY_APP_PIPE3_NUM_PLANES] ALIGNED(8);
#endif

t_Pipe0 DDR_DATA DMA0_OutputBuffer[CONCURRENCY_APP_PIPE0_WIDTH * CONCURRENCY_APP_PIPE0_HEIGHT * CONCURRENCY_APP_PIPE0_NUM_PLANES] ALIGNED(8);
t_Pipe1 DDR_DATA DMA1_OutputBuffer[CONCURRENCY_APP_PIPE1_WIDTH * CONCURRENCY_APP_PIPE1_HEIGHT * CONCURRENCY_APP_PIPE1_NUM_PLANES] ALIGNED(8);
#if CONCURRENCY_APP_NUM_PIPELINES > 0x2
t_Pipe2 DDR_DATA DMA2_OutputBuffer[CONCURRENCY_APP_PIPE2_WIDTH * CONCURRENCY_APP_PIPE2_HEIGHT * CONCURRENCY_APP_PIPE2_NUM_PLANES] ALIGNED(8);
#endif
#if CONCURRENCY_APP_NUM_PIPELINES > 0x3
t_Pipe3 DDR_DATA DMA3_OutputBuffer[CONCURRENCY_APP_PIPE3_WIDTH * CONCURRENCY_APP_PIPE3_HEIGHT * CONCURRENCY_APP_PIPE3_NUM_PLANES] ALIGNED(8);
#endif

// This will be linked with makefile and represent the start address for test input image
extern UInt8 test_image;
u16 conv5x50Cfg_cMat[CONCURRENCY_APP_NUM_PIPELINES][25];

#ifdef SIPP_VCS
tyAuxClkDividerCfg auxClk[] =
{
    {0, 0, 0, 0} // Null Terminated List
};

tySocClockConfig clockConfig =
{
    .refClk0InputKhz         = 12000,           // Default 12Mhz input clock
    .refClk1InputKhz         = 0,               // Assume no secondary oscillator for now
    .targetPll0FreqKhz       = 500000,
    .targetPll1FreqKhz       = 0,               // will be set by DDR driver
    .clkSrcPll1              = CLK_SRC_REFCLK0, // Supply both PLLS from REFCLK0
    .masterClkDivNumerator   = 1,
    .masterClkDivDenominator = 1,
    .cssDssClockEnableMask   = DEFAULT_CORE_CSS_DSS_CLOCKS,
    .mssClockEnableMask      = -1,                            // Not enabling any MSS clocks for now
    .upaClockEnableMask      = DEFAULT_UPA_CLOCKS,
    .pAuxClkCfg              = auxClk
};

#endif

/////////////////////////////////////////////////////
// Function prototypes
/////////////////////////////////////////////////////

void initConvMat (SippFilter * conv5x5, u32 pipeId, u32 ks);
void appSippCallback (SippPipeline *             pPipeline,
                      eSIPP_PIPELINE_EVENT       eEvent,
                      SIPP_PIPELINE_EVENT_DATA * ptEventData);

/////////////////////////////////////////////////////
// Code
/////////////////////////////////////////////////////

#ifdef SIPP_USE_PRECALC_SCHEDULE
#include "pcDumpSchedule.hh"
#include "pcDumpSchedule_1.hh"
#if CONCURRENCY_APP_NUM_PIPELINES > 0x2
#include "pcDumpSchedule_2.hh"
#endif
#if CONCURRENCY_APP_NUM_PIPELINES > 0x3
#include "pcDumpSchedule_3.hh"
#endif
#endif


////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appLookupHandle                                              //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void appInit ()
{
    memset (concurrencyPipeList, 0x0, sizeof(concurrencyPipeList));

    concurrencyPipeList[0].runAsync = CONCURRENCY_APP_PIPE0_ASYNC;
    concurrencyPipeList[1].runAsync = CONCURRENCY_APP_PIPE1_ASYNC;
    #if CONCURRENCY_APP_NUM_PIPELINES > 0x2
    concurrencyPipeList[2].runAsync = CONCURRENCY_APP_PIPE2_ASYNC;
    #endif
    #if CONCURRENCY_APP_NUM_PIPELINES > 0x3
    concurrencyPipeList[3].runAsync = CONCURRENCY_APP_PIPE3_ASYNC;
    #endif
}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appLookupHandle                                              //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

u32 appLookupHandle (SippPipeline * pPipeline)
{
    u32 idx;

    for (idx = 0; idx < CONCURRENCY_APP_NUM_PIPELINES; idx++)
    {
        ConcurrencyAppPipe * pipe = &concurrencyPipeList[idx];

        if (pipe->sippFwHandle == pPipeline)
        {
            return idx;
        }
    }

    return 0xFF;
}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appCreatePipelines                                           //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void appCreatePipelines ( )
{
    u32 idx;
    for (idx = 0; idx < CONCURRENCY_APP_NUM_PIPELINES; idx++ )
    {
        ConcurrencyAppPipe * pipe = &concurrencyPipeList[idx];

        pipe->sippFwHandle = sippCreatePipeline (sliceFirst[idx],
                                                 sliceLast[idx],
                                                 SIPP_MBIN(mbinImgSipp));

        if (sippPipeGetErrorStatus (pipe->sippFwHandle))
        {
            u32 errNum;
            printf ("Pipeline creation error\nError codes:\n");
            errNum = sippGetErrorHistory (sippErrorLog);
            while (errNum)
            {
                printf ("%08lx\n", sippErrorLog[errNum - 0x1]);
                errNum--;
            }
            exit (0x0);
        }

        // Register callback for async API
        sippRegisterEventCallback (pipe->sippFwHandle,
                                   appSippCallback);
    }
}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appConfigurePipelines                                        //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void appConfigurePipelines ( )
{
    ConcurrencyAppPipe * pipe;
    DmaParam           * dmaInCfg;
    DmaParam           * dmaOutCfg;

    // Pipeline 0 - create and config filters
    pipe           = &concurrencyPipeList[0];

    pipe->dmaIn    =  sippCreateFilter (pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE0_WIDTH, CONCURRENCY_APP_PIPE0_HEIGHT, N_PL(CONCURRENCY_APP_PIPE0_NUM_PLANES), SZ(t_Pipe0), SIPP_AUTO,             (FnSvuRun)SIPP_DMA_ID,       0);
    pipe->HWFilter =  sippCreateFilter (pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE0_WIDTH, CONCURRENCY_APP_PIPE0_HEIGHT, N_PL(CONCURRENCY_APP_PIPE0_NUM_PLANES), SZ(t_Pipe0), SIPP_AUTO,             (FnSvuRun)SIPP_CONV_ID,      0);
    pipe->SWFilter =  sippCreateFilter (pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE0_WIDTH, CONCURRENCY_APP_PIPE0_HEIGHT, N_PL(CONCURRENCY_APP_PIPE0_NUM_PLANES), SZ(t_Pipe0), SZ(BoxFilter5x5Param), SVU_SYM(svuBoxFilter5x5), 0);
    pipe->dmaOut   =  sippCreateFilter (pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE0_WIDTH, CONCURRENCY_APP_PIPE0_HEIGHT, N_PL(CONCURRENCY_APP_PIPE0_NUM_PLANES), SZ(t_Pipe0), SIPP_AUTO,             (FnSvuRun)SIPP_DMA_ID,       0);


    sippLinkFilter (pipe->HWFilter, pipe->dmaIn,    5, 5 );
    sippLinkFilter (pipe->SWFilter, pipe->HWFilter, 5, 5 );
    sippLinkFilter (pipe->dmaOut,   pipe->SWFilter, 1, 0 );

    if (sippPipeGetErrorStatus (pipe->sippFwHandle))
    {
        u32 errNum;
        printf ("Pipeline 0 creation error\nError codes:\n");
        errNum = sippGetErrorHistory (sippErrorLog);
        while (errNum)
        {
            printf ("%08lx\n", sippErrorLog[errNum - 0x1]);
            errNum--;
        }
        exit (0x0);
    }

    dmaInCfg           = (DmaParam *)pipe->dmaIn->params;
    dmaOutCfg          = (DmaParam *)pipe->dmaOut->params;
    dmaInCfg->ddrAddr  = (UInt32)DMA0_InputBuffer;
    dmaOutCfg->ddrAddr = (UInt32)DMA0_OutputBuffer;

    ConvParam * HWFilterCfg = (ConvParam *)pipe->HWFilter->params;
    HWFilterCfg->cfg = KERNEL_SIZE |
                       (1  << 3)   | //OUTPUT_CLAMP
                       (0  << 4)   | //ABS_BIT
                       (0  << 5)   | //SQ_BIT
                       (0  << 6)   | //ACCUM_BIT
                       (0  << 7)   | //OUTPUT_DISABLE
                       (0  << 8);    //ACCUM_THR(fp16)

    BoxFilter5x5Param * SWFilterCfg = (BoxFilter5x5Param *)pipe->SWFilter->params;
    SWFilterCfg->normalize = 0;

    // Pack params according to HW spec
    packConv5x5CCM (HWFilterCfg, hwKer);

    #ifdef SIPP_USE_PRECALC_SCHEDULE
    // Read offline schedule decisions
    dbgSchedInit (pipe->sippFwHandle);
    #endif

    sippFinalizePipeline (pipe->sippFwHandle);

    if (sippPipeGetErrorStatus (pipe->sippFwHandle))
    {
        u32 errNum;
        printf ("Pipeline 0 finalization failed\nError codes:\n");
        errNum = sippGetErrorHistory (sippErrorLog);
        while (errNum)
        {
            printf ("%08lx\n", sippErrorLog[errNum - 0x1]);
            errNum--;
        }
    }

    // Pipeline 1 - create and config filters
    pipe           = &concurrencyPipeList[1];
    pipe->dmaIn    =  sippCreateFilter (pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE0_WIDTH, CONCURRENCY_APP_PIPE0_HEIGHT, N_PL(CONCURRENCY_APP_PIPE0_NUM_PLANES), SZ(t_Pipe1), SIPP_AUTO,        (FnSvuRun)SIPP_DMA_ID, 0);
    pipe->SWFilter =  sippCreateFilter (pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE0_WIDTH, CONCURRENCY_APP_PIPE0_HEIGHT, N_PL(CONCURRENCY_APP_PIPE0_NUM_PLANES), SZ(t_Pipe1), SZ(Conv5x5Param), SVU_SYM(svuConv5x5),   0);
    pipe->HWFilter =  sippCreateFilter (pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE0_WIDTH, CONCURRENCY_APP_PIPE0_HEIGHT, N_PL(CONCURRENCY_APP_PIPE0_NUM_PLANES), SZ(t_Pipe1), SIPP_AUTO,        (FnSvuRun)SIPP_MED_ID, 0);
    pipe->dmaOut   =  sippCreateFilter (pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE0_WIDTH, CONCURRENCY_APP_PIPE0_HEIGHT, N_PL(CONCURRENCY_APP_PIPE0_NUM_PLANES), SZ(t_Pipe1), SIPP_AUTO,        (FnSvuRun)SIPP_DMA_ID, 0);


    sippLinkFilter (pipe->SWFilter, pipe->dmaIn,    5, 5 );
    sippLinkFilter (pipe->HWFilter, pipe->SWFilter, 5, 5 );
    sippLinkFilter (pipe->dmaOut,   pipe->HWFilter, 1, 0 );

    if (sippPipeGetErrorStatus (pipe->sippFwHandle))
    {
        u32 errNum;
        printf ("Pipeline 1 creation error\nError codes:\n");
        errNum = sippGetErrorHistory (sippErrorLog);
        while (errNum)
        {
            printf ("%08lx\n", sippErrorLog[errNum - 0x1]);
            errNum--;
        }
        exit (0x0);
    }

    dmaInCfg           = (DmaParam *)pipe->dmaIn->params;
    dmaOutCfg          = (DmaParam *)pipe->dmaOut->params;
    dmaInCfg->ddrAddr  = (UInt32)DMA1_InputBuffer;
    dmaOutCfg->ddrAddr = (UInt32)DMA1_OutputBuffer;

    initConvMat (pipe->SWFilter, 0x1, KERNEL_SIZE);
    MedParam * MedHWFilterCfg = (MedParam *)pipe->HWFilter->params;
    MedHWFilterCfg->cfg = KERNEL_SIZE                              | // kernel sz
                          (((KERNEL_SIZE * KERNEL_SIZE)>>1) <<  8) | // output sel
                          ( 0                               << 16);  // threshold

    #ifdef SIPP_USE_PRECALC_SCHEDULE
    // Read offline schedule decisions
    dbgSchedInit_1 (pipe->sippFwHandle);
    #endif

    sippFinalizePipeline (pipe->sippFwHandle);

    if (sippPipeGetErrorStatus (pipe->sippFwHandle))
    {
        u32 errNum;
        printf ("Pipeline 1 finalization failed\nError codes:\n");
        errNum = sippGetErrorHistory (sippErrorLog);
        while (errNum)
        {
            printf ("%08lx\n", sippErrorLog[errNum - 0x1]);
            errNum--;
        }
    }

    #if CONCURRENCY_APP_NUM_PIPELINES > 2
    // Pipeline 2 - create and config filters
    pipe           = &concurrencyPipeList[2];

    #endif

    #if CONCURRENCY_APP_NUM_PIPELINES > 3
    // Pipeline 3 - create and config filters
    pipe           = &concurrencyPipeList[3];

    #endif

}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appLogEvent                                                  //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void appLogEvent (ConcurrencyAppPipe * pipe,
                  CC_APP_PIPE_EVENT    event)
{
    #ifndef SIPP_PC
    // Use critical sections to future proof against this happening in an RTOS thread
    u32 storedIL = swcLeonGetPIL ();
    swcLeonSetPIL (15);
    #endif

    pipe->eventList[pipe->eventListWrPtr++] = event;
    if (pipe->eventListWrPtr == CC_APP_EVENT_HISTORY_SIZE) pipe->eventListWrPtr = 0;

    #ifndef SIPP_PC
    swcLeonSetPIL (storedIL);
    #endif

}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appProcessEvent                                              //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

CC_APP_PIPE_EVENT appProcessEvent (ConcurrencyAppPipe * pipe)
{
    CC_APP_PIPE_EVENT retVal = CC_APP_PIPE_EVENT_NONE;

    if (pipe->eventListWrPtr != pipe->eventListRdPtr )
    {
        retVal = pipe->eventList[pipe->eventListRdPtr++];
        if (pipe->eventListRdPtr == CC_APP_EVENT_HISTORY_SIZE) pipe->eventListRdPtr = 0;

    }

    return retVal;
}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appSippCallback                                              //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void appSippCallback (SippPipeline *             pPipeline,
                      eSIPP_PIPELINE_EVENT       eEvent,
                      SIPP_PIPELINE_EVENT_DATA * ptEventData)
{
    // Find pipeline
    u32                  pipeIdx = appLookupHandle (pPipeline);
    ConcurrencyAppPipe * pipe    = &concurrencyPipeList[pipeIdx];

    UNUSED(ptEventData);

    if (eEvent == eSIPP_PIPELINE_FRAME_DONE)
    {
        printf ("Pipe %lu completes frame\n", pipeIdx);
        appLogEvent (pipe,
                     CC_APP_PIPE_EVENT_FRAME_DONE);
    }

}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appCheckComplete                                             //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

u32 appCheckComplete ( )
{
    u32 retVal = 0x1;
    u32 idx;

    for (idx = 0; idx < CONCURRENCY_APP_NUM_PIPELINES; idx++ )
    {
        ConcurrencyAppPipe * pipe = &concurrencyPipeList[idx];
        if (pipe->eState != CC_APP_PIPE_STATE_COMPLETE)
        {
            retVal = 0x0;
            break;
        }
    }

    return retVal;
}


////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appTask                                                      //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void appTask ()
{
    u32 pipeIdx;

    while (testComplete == 0)
    {
        for (pipeIdx = 0x0; pipeIdx < CONCURRENCY_APP_NUM_PIPELINES; pipeIdx++)
        {
            ConcurrencyAppPipe * pipe = &concurrencyPipeList[pipeIdx];

            switch (pipe->eState)
            {
                case CC_APP_PIPE_STATE_UNINIT   : {
                                                      pipe->eState = CC_APP_PIPE_STATE_IDLE;
                                                  } break;
                case CC_APP_PIPE_STATE_IDLE     : {
                                                      if (pipe->framesComplete < CONCURRENCY_APP_FRAMES_PER_PIPE)
                                                      {
                                                          if (pipe->runAsync)
                                                          {
                                                              printf("Pipe %lu call process frame async\n", pipeIdx);
                                                              sippProcessFrameNB (pipe->sippFwHandle);
                                                              pipe->eState = CC_APP_PIPE_STATE_RUNNING;
                                                          }
                                                          else
                                                          {
                                                              u32 errCode      = (u32)E_BLOCK_CALL_REJECTED;
                                                              u32 errMaskIdx   = errCode >> 5;
                                                              u32 errMaskEntry = 0x1 << (errCode & 0x1F);

                                                              // We wish to call the sync API
                                                              sippProcessFrame (pipe->sippFwHandle);

                                                              // Check if the call worked
                                                              // Interrogate the error
                                                              if (pipe->sippFwHandle->errorStatus[errMaskIdx] & errMaskEntry)
                                                              {
                                                                  printf ("Pipe %lu failed in call to blocking API sippProcessFrame due to active and/or pending operations\n", pipeIdx);
                                                                  // Clear the error
                                                                  pipe->sippFwHandle->errorStatus[errMaskIdx] &= (~errMaskEntry);
                                                              }
                                                              else
                                                              {
                                                                  printf ("Pipe %lu successfully called blocking API sippProcessFrame\n", pipeIdx);
                                                                  pipe->framesComplete++;
                                                                  pipe->eState = CC_APP_PIPE_STATE_IDLE;
                                                              }
                                                          }
                                                      }
                                                      else
                                                      {
                                                          printf ("Pipe %lu completed run of %lu images\n", pipeIdx, pipe->framesComplete);
                                                          sippFrameworkRun ();
                                                          pipe->eState = CC_APP_PIPE_STATE_COMPLETE;
                                                      }

                                                  } break;
                case CC_APP_PIPE_STATE_RUNNING  : {
                                                      CC_APP_PIPE_EVENT event;
                                                      while ((event = appProcessEvent (pipe)) != CC_APP_PIPE_EVENT_NONE)
                                                      {
                                                          switch (event)
                                                          {
                                                              case CC_APP_PIPE_EVENT_FRAME_DONE : {
                                                                                                      pipe->framesComplete++;
                                                                                                      pipe->eState = CC_APP_PIPE_STATE_IDLE;
                                                                                                  } break;
                                                              default : break;
                                                          }
                                                      }
                                                  } break;
                case CC_APP_PIPE_STATE_COMPLETE : {
                                                      if (appCheckComplete())
                                                      {
                                                          testComplete = 1;
                                                      }
                                                  } break;
                default                         : {
                                                      printf ("Unknown state on pipe %lu - Terminating early\n", pipeIdx);
                                                      exit (0x0);
                                                  } break;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// MAIN

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    main                                                         //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int main (int argc, char *argv[])
{
    UNUSED (argc);
    UNUSED (argv);

    #ifdef SIPP_VCS
    unitTestInit ();
    DrvCprInit();
    DrvDdrInitialise (NULL);
    DrvCprSetupClocks(&clockConfig);
    #endif

    sippPlatformInit ();

    #ifdef SIPP_VCS
    DrvTimerInit ();
    #endif

    #ifndef SIPP_PC
    if (PROCESS_LEON_OS == swcWhoAmI())
    {
        printf ("App Starting on LEON OS\n");
        printf ("Please make sure IRQ sources for dynamically route between LeonOS and LeonRT are set\n");
    }
    else
    {
    	printf ("App Starting on LEON RT\n");
    }
    #endif // SIPP_PC

    sippPlatformInitAsync ();

    appInit ();
    appCreatePipelines ();
    appConfigurePipelines ();

    // Load input data for pipes
    appTask ();

    #ifdef SIPP_VCS
    unitTestFinalReport();
    #endif

   return 0;
}
