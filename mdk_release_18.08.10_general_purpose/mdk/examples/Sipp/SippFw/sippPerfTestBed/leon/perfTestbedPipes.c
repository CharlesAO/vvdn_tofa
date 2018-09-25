///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Setup the pipelines on behalf of the perfTestBed app

#include "perfTestbed.h"
#include <filters/conv5x5/conv5x5.h>
#include <filters/boxFilter5x5/boxFilter5x5.h>

/////////////////////////////////////////////////////
// Global variables
/////////////////////////////////////////////////////

#include "appParams.h"

u32    sippErrorLog[SIPP_ERROR_HISTORY_SIZE];

// This will be linked with makefile and represent the start address for test input image
extern UInt8 test_image;

PerfTestbedPipe perfTestbedPipeList[PERF_TESTBED_NUM_PIPELINES];

#if PERF_TESTBED_NUM_PIPELINES > 0x3
u32                sliceFirst[PERF_TESTBED_NUM_PIPELINES] = {0,3,6,9};
u32                sliceLast[PERF_TESTBED_NUM_PIPELINES]  = {2,5,8,11};
#elif PERF_TESTBED_NUM_PIPELINES > 0x2
u32                sliceFirst[PERF_TESTBED_NUM_PIPELINES] = {0,4,8};
u32                sliceLast[PERF_TESTBED_NUM_PIPELINES]  = {3,7,11};
#elif PERF_TESTBED_NUM_PIPELINES > 0x1
u32                sliceFirst[PERF_TESTBED_NUM_PIPELINES] = {0,6};
u32                sliceLast[PERF_TESTBED_NUM_PIPELINES]  = {5,11};
#else
u32                sliceFirst[PERF_TESTBED_NUM_PIPELINES] = {0};
u32                sliceLast[PERF_TESTBED_NUM_PIPELINES]  = {11};
#endif

u8 *               pmBinImg[PERF_TESTBED_NUM_PIPELINES];


t_Pipe0 DDR_DATA DMA0_InputBuffer[PERF_TESTBED_PIPE0_WIDTH * PERF_TESTBED_PIPE0_HEIGHT * PERF_TESTBED_PIPE0_NUM_PLANES] ALIGNED(8);
#if PERF_TESTBED_NUM_PIPELINES > 0x1
t_Pipe1 DDR_DATA DMA1_InputBuffer[PERF_TESTBED_PIPE1_WIDTH * PERF_TESTBED_PIPE1_HEIGHT * PERF_TESTBED_PIPE1_NUM_PLANES] ALIGNED(8);
#endif
#if PERF_TESTBED_NUM_PIPELINES > 0x2
t_Pipe2 DDR_DATA DMA2_InputBuffer[PERF_TESTBED_PIPE2_WIDTH * PERF_TESTBED_PIPE2_HEIGHT * PERF_TESTBED_PIPE2_NUM_PLANES] ALIGNED(8);
#endif
#if PERF_TESTBED_NUM_PIPELINES > 0x3
t_Pipe3 DDR_DATA DMA3_InputBuffer[PERF_TESTBED_PIPE3_WIDTH * PERF_TESTBED_PIPE3_HEIGHT * PERF_TESTBED_PIPE3_NUM_PLANES] ALIGNED(8);
#endif

t_Pipe0 DDR_DATA DMA0_OutputBuffer[PERF_TESTBED_PIPE0_WIDTH * PERF_TESTBED_PIPE0_HEIGHT * PERF_TESTBED_PIPE0_NUM_PLANES] ALIGNED(8);
#if PERF_TESTBED_NUM_PIPELINES > 0x1
t_Pipe1 DDR_DATA DMA1_OutputBuffer[PERF_TESTBED_PIPE1_WIDTH * PERF_TESTBED_PIPE1_HEIGHT * PERF_TESTBED_PIPE1_NUM_PLANES] ALIGNED(8);
#endif
#if PERF_TESTBED_NUM_PIPELINES > 0x2
t_Pipe2 DDR_DATA DMA2_OutputBuffer[PERF_TESTBED_PIPE2_WIDTH * PERF_TESTBED_PIPE2_HEIGHT * PERF_TESTBED_PIPE2_NUM_PLANES] ALIGNED(8);
#endif
#if PERF_TESTBED_NUM_PIPELINES > 0x3
t_Pipe3 DDR_DATA DMA3_OutputBuffer[PERF_TESTBED_PIPE3_WIDTH * PERF_TESTBED_PIPE3_HEIGHT * PERF_TESTBED_PIPE3_NUM_PLANES] ALIGNED(8);
#endif

u16 conv5x50Cfg_cMat[PERF_TESTBED_NUM_PIPELINES][25];

/////////////////////////////////////////////////////
// Function prototypes
/////////////////////////////////////////////////////

void initConvMat (SippFilter * conv5x5, u32 pipeId, u32 ks);

/////////////////////////////////////////////////////
// Code
/////////////////////////////////////////////////////

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
    PerfTestbedPipe * pipe;
    DmaParam        * dmaInCfg;
    DmaParam        * dmaOutCfg;

    // Pipeline 0 - create and config filters
    pipe           = &perfTestbedPipeList[0];

    pipe->dmaIn    =  sippCreateFilter (pipe->sippFwHandle, 0x0, PERF_TESTBED_PIPE0_WIDTH, PERF_TESTBED_PIPE0_HEIGHT, N_PL(PERF_TESTBED_PIPE0_NUM_PLANES), SZ(t_Pipe0), SIPP_AUTO, (FnSvuRun)SIPP_DMA_ID,       0);
    pipe->HWFilter =  sippCreateFilter (pipe->sippFwHandle, 0x0, PERF_TESTBED_PIPE0_WIDTH, PERF_TESTBED_PIPE0_HEIGHT, N_PL(PERF_TESTBED_PIPE0_NUM_PLANES), SZ(t_Pipe0), SIPP_AUTO, (FnSvuRun)SIPP_CONV_ID,      0);
    pipe->SWFilter =  sippCreateFilter (pipe->sippFwHandle, 0x0, PERF_TESTBED_PIPE0_WIDTH, PERF_TESTBED_PIPE0_HEIGHT, N_PL(PERF_TESTBED_PIPE0_NUM_PLANES), SZ(t_Pipe0), SZ(BoxFilter5x5Param),     SVU_SYM(svuBoxFilter5x5), 0);
    pipe->dmaOut   =  sippCreateFilter (pipe->sippFwHandle, 0x0, PERF_TESTBED_PIPE0_WIDTH, PERF_TESTBED_PIPE0_HEIGHT, N_PL(PERF_TESTBED_PIPE0_NUM_PLANES), SZ(t_Pipe0), SIPP_AUTO, (FnSvuRun)SIPP_DMA_ID,       0);

    #ifdef DISABLE_OPIPE_CONNECTIONS
    pipe->sippFwHandle->flags |= (PLF_DISABLE_OPIPE_CONS);
    #endif
    #ifndef DISABLE_SIPP_SW_QU
    pipe->sippFwHandle->flags |= (PLF_ENABLE_SW_QU_USE);
    #endif
    #ifdef SIPP_RUNTIME_PROFILING
    pipe->sippFwHandle->flags |= (PLF_PROVIDE_RT_STATS);
    #endif

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

    // Pack params according to HW spec
    packConv5x5CCM (HWFilterCfg, hwKer);

    BoxFilter5x5Param * SWFilterCfg = (BoxFilter5x5Param *)pipe->SWFilter->params;

    SWFilterCfg->normalize = 0;

    sippPipeSetLinesPerIter (pipe->sippFwHandle,
                             PERF_TESTBED_PIPE0_LINES_PER_ITER);

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

    #if PERF_TESTBED_NUM_PIPELINES > 1

    // Pipeline 1 - create and config filters
    pipe           = &perfTestbedPipeList[1];
    pipe->dmaIn    =  sippCreateFilter (pipe->sippFwHandle, 0x0, PERF_TESTBED_PIPE0_WIDTH, PERF_TESTBED_PIPE0_HEIGHT, N_PL(PERF_TESTBED_PIPE0_NUM_PLANES), SZ(t_Pipe1), SIPP_AUTO,        (FnSvuRun)SIPP_DMA_ID, 0);
    pipe->SWFilter =  sippCreateFilter (pipe->sippFwHandle, 0x0, PERF_TESTBED_PIPE0_WIDTH, PERF_TESTBED_PIPE0_HEIGHT, N_PL(PERF_TESTBED_PIPE0_NUM_PLANES), SZ(t_Pipe1), SZ(Conv5x5Param), SVU_SYM(svuConv5x5),   0);
    pipe->HWFilter =  sippCreateFilter (pipe->sippFwHandle, 0x0, PERF_TESTBED_PIPE0_WIDTH, PERF_TESTBED_PIPE0_HEIGHT, N_PL(PERF_TESTBED_PIPE0_NUM_PLANES), SZ(t_Pipe1), SIPP_AUTO,        (FnSvuRun)SIPP_MED_ID, 0);
    pipe->dmaOut   =  sippCreateFilter (pipe->sippFwHandle, 0x0, PERF_TESTBED_PIPE0_WIDTH, PERF_TESTBED_PIPE0_HEIGHT, N_PL(PERF_TESTBED_PIPE0_NUM_PLANES), SZ(t_Pipe1), SIPP_AUTO,        (FnSvuRun)SIPP_DMA_ID, 0);

    #ifdef DISABLE_OPIPE_CONNECTIONS
    pipe->sippFwHandle->flags |= (PLF_DISABLE_OPIPE_CONS);
    #endif
    #ifndef DISABLE_SIPP_SW_QU
    pipe->sippFwHandle->flags |= (PLF_ENABLE_SW_QU_USE);
    #endif
    #ifdef SIPP_RUNTIME_PROFILING
    pipe->sippFwHandle->flags |= (PLF_PROVIDE_RT_STATS);
    #endif

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

    sippPipeSetLinesPerIter (pipe->sippFwHandle,
                             PERF_TESTBED_PIPE1_LINES_PER_ITER);

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

    #endif

    #if PERF_TESTBED_NUM_PIPELINES > 2
    // Pipeline 2 - create and config filters
    pipe           = &perfTestbedPipeList[2];

    #endif

    #if PERF_TESTBED_NUM_PIPELINES > 3
    // Pipeline 3 - create and config filters
    pipe           = &perfTestbedPipeList[3];

    #endif

}
