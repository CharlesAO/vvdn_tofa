///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

////////////////////////////////////////////////////
// Header files
////////////////////////////////////////////////////

#include <sipp.h>
#include <sippInternal.h>

////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////

extern void sippGenericWaitUnits(SippPipeline *pl);
#ifdef SIPP_PC
void sippGenericPCWaitUnits (pSippPipeline pPipe);
#endif
#ifdef SIPP_HW_TIME_EXACT
u64 HwStartTick[3000];
u64 HwStopTick[3000];
#endif

////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericStartHWUnits2x5x                                      //
//                                                                                //
//  DESCRIPTION: Start HW filters for current iteration                           //
//               more notes to follow when it works...                            //
//                                                                                //
//  INPUTS:      pPipe - pipeline to start                                        //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       If async runtime is in use - the appropriate interrupt barrier   //
//               is setup                                                         //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippGenericStartHWUnits2x5x (SippPipeline * pPipe)
{
    // Setup the interrupt barrier with pPipe->schedInfo[pPipe->iteration].sippHwWaitMask;
    #ifndef SIPP_PC
    if (pPipe->useSyncRuntime == 0)
    {
        #ifndef NO_INTERRUPT_BARRIERS_MODE
        // Movisim crashes when we write to this at the mo (mid August 2015)!!
        SET_REG_WORD((SIPP_INT1_BARRIER0_ADR + ((pPipe->HWPipeID)<< 0x2)), pPipe->schedInfo[pPipe->iteration].sippHwWaitMask);
        #endif
    }
    #endif

    #ifdef SIPP_HW_TIME_EXACT
    DrvTimerGetSystemTicks64(&HwStartTick[pPipe->iteration]);
    #endif
    SET_REG_WORD(SIPP_OBFL_DEC_ADR, pPipe->schedInfo[pPipe->iteration].sippHwWaitMask);

    #ifdef SIPP_HW_TIME_EXACT
    if (pPipe->schedInfo[pPipe->iteration].sippHwWaitMask)
    {
        u32 status;
        // Wait till status bit gets set
        do
        {
            // Typically, filters are done when obfl_inc asserts
            // this applies to most filters (i.e. that have associated output buffer)
            status = GET_REG_WORD_VAL(SIPP_INT1_STATUS_ADR);

            NOP;NOP;NOP;NOP;NOP;

        } while ((status & pPipe->schedInfo[pPipe->iteration].sippHwWaitMask) != pPipe->schedInfo[pPipe->iteration].sippHwWaitMask);

        DrvTimerGetSystemTicks64(&HwStopTick[pPipe->iteration]);
        // Clear status bits
        SET_REG_WORD(SIPP_INT1_CLEAR_ADR, pPipe->schedInfo[pPipe->iteration].sippHwWaitMask);
        pPipe->schedInfo[pPipe->iteration].sippHwWaitMask = 0;

    }
    #endif
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericBlockHWUnits2x5x                                      //
//                                                                                //
//  DESCRIPTION: Fill the output buffers of all HW units to be fed input data on  //
//               the current iteration of the runtime                             //
//                                                                                //
//  INPUTS:      pPipe - pipeline containing HW filters to potentially block      //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       There are filters which may be scheduled to run on an iteration  //
//               but not waited on (polyFir, DogLTM, OSEs with latencies)         //
//               On iterations such as these, the output buffer levels will be    //
//               left with space in them at the end of the iteration. So, on a    //
//               new iteration, incrementing the input buffer could cause a run   //
//               and therefore a line to be porduced BEFORE the filter is started //
//               by again decrementing its output buffer which could produce a    //
//               second line - this function blocks filters at their output       //
//               buffers until they are started. A notable example is the DogLTM  //
//               which produces an output line on its 1st run iteration, not on   //
//               its second but on every iteration thereafter. Not cleaning up    //
//               its output buffer after the non-producing iteration causes       //
//               the filter to run twice on the next iteration due to its         //
//               downsampling then upsampling nature                              //
//                                                                                //
//               However, there is an obvious advantage to starting HW as quickly //
//               as possible on any iteration so maybe a better solution is       //
//               possible                                                         //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippGenericBlockHWUnits2x5x (SippPipeline * pPipe)
{
    u32          uSetMask = pPipe->schedInfo[pPipe->iteration].sippHwWaitMask;
    SippFilter * ptrFilt;
    u32          filtIdx;

    for (filtIdx = 0; filtIdx < pPipe->nFilters; filtIdx++)
    {
        // Should have a better list with only the HW filters in it to save time
        ptrFilt = pPipe->filters[filtIdx];

        if (ptrFilt->unit <= SIPP_MAX_FILTER_ID)
        {
            if (uSetMask & (1 << ptrFilt->unit))
            {
                #ifdef RT_DBG
                sippPalTrace(SIPP_RUNTIME_TL_INFO, "Blocking unit %d\n", ptrFilt->unit);
                #endif

                gSipp.uInfo[ptrFilt->unit].hwFnSetOBufs(ptrFilt, eSIPP_SET_OBUF_SPACE_FULL);

            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericUpdateHWUnits2x5x                                     //
//                                                                                //
//  DESCRIPTION: Per iteration call to update input buffer fill for HW filters    //
//               connected to CMX buffers whose filter levels incremeneted on     //
//               last iteration                                                   //
//                                                                                //
//  INPUTS:      pPipe - pipeline to start                                        //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       This is called at the start of an iteration so that we are in    //
//               control of when HW becomes active. It could be called at the     //
//               end of an iteration but it may cause some HW to restart!!        //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippGenericUpdateHWUnits2x5x (SippPipeline * pPipe)
{
    #ifdef RT_DBG
    sippPalTrace (SIPP_RUNTIME_TL_INFO, "Incrementing LBs on units %08x\n", pPipe->schedInfo[pPipe->iteration].sippIbflIncMask);
    #endif

    SET_REG_WORD(SIPP_IBFL_INC_ADR, pPipe->schedInfo[pPipe->iteration].sippIbflIncMask);

}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericRuntimeHWProcessIters                                 //
//                                                                                //
//  DESCRIPTION: This is the synchronous API runtime function                     //
//                                                                                //
//  INPUTS:      pPipe - pipeline to start                                        //
//               numIters - number of iterations of the schedule to complete      //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       Blocking call                                                    //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippGenericRuntimeHWProcessIters (pSippPipeline pPipe,
                                       u32           numIters)
{
    u32 i;
    #ifdef SIPP_RUNTIME_PROFILING
    u8 collectStats = (pPipe->pRTStats) ? 0x1 : 0x0;
    #endif

    for (i = 0; i < numIters; i++, pPipe->iteration++)
    {
        #ifdef RT_DBG
        sippPalTrace (SIPP_RUNTIME_TL_INFO, "======== %5d ==========\n", pPipe->iteration);
        #endif

        /////////////////////////////////////////////////
        // Start current iteration
        sippGenericStartUnits (pPipe);

        #ifdef SIPP_RUNTIME_PROFILING
        if (collectStats) DrvTimerGetSystemTicks64 (&pPipe->pRTStats->iterTime[(pPipe->iteration * SIPP_RT_PER_ITER_STATS_SIZE) + SIPP_ITER_STAT_STARTED_POS]);
        #endif

        /////////////////////////////////////////////////
        // Setup for next line in background
        sippGenericLinePrepare (pPipe, pPipe->iteration + 1);

        /////////////////////////////////////////////////
        // Wait unit completion
        #ifdef SIPP_PC
        sippGenericPCWaitUnits (pPipe);
        #else
        sippGenericWaitUnits (pPipe);

        #ifdef SIPP_RUNTIME_PROFILING
        if (collectStats) DrvTimerGetSystemTicks64(&pPipe->pRTStats->iterTime[(pPipe->iteration * SIPP_RT_PER_ITER_STATS_SIZE) + SIPP_ITER_STAT_COMPLETE_POS]);
        #endif
        #endif

        /////////////////////////////////////////////////
        // Update any HW units with new input
        sippGenericUpdateHWUnits2x5x (pPipe);

        // Print something to indicate progress in VCS
        VCS_PRINT_INT(0xABCD0000 + pPipe->iteration);
    }
}


#ifdef SIPP_PC

//////////////////////////////////////////////////////////////////////////////////
// Sipp c-model Runtime section
//

#include "sippHwM2Factory.h"
#include "wrapperSem.h"

#define IRQ_SVE_0 (  32 )

extern Semaphore *      serialiseSem;
extern Semaphore *      hwSyncSem;
extern Semaphore *      svuSyncSem;
extern Semaphore *      cmxDmaSyncSem;
extern SIPP_HW_SESSION  gSippHW;
#ifndef DISABLE_TRACE
extern Semaphore *      palTraceSerialiseSem;
#endif /* !DISABLE_TRACE */

#ifdef WIN32
HANDLE    pHWIsrThread, pSvuIsrThread, pCmxDmaIsrThread;
DWORD     hwIsrThreadId, svuIsrThreadId, cmxDmaIsrThreadId;
#else
pthread_t pHWIsrThread, pSvuIsrThread, pCmxDmaIsrThread;
#endif

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericRuntimeHWIsrThread                                    //
//                                                                                //
//  DESCRIPTION: MA2x5x_HW c-model ISR thread                                     //
//                                                                                //
//  INPUTS:      ptr - This is a pointer to pointer to SIPP pipeline struct       //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       Triggered from sippGenericPCRuntimeIsr                           //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
DWORD WINAPI sippGenericRuntimeHWIsrThread (void *ptr) {
#else
void * sippGenericRuntimeHWIsrThread (void * ptr) {
#endif
    pSippPipeline pPipe;
    u8            runDone = 0;

    while (runDone == 0)
    {
        hwSyncSem->Wait();
        pPipe = *(pSippPipeline *)ptr;  // Load here as will not be known at thread creation time

        if (pPipe->useSyncRuntime == 0)
        {
            sippObflIncHandler (0x0);

            if (pPipe->iteration >= pPipe->nIter)
            {
                runDone = 1;
            }
        }
        else
        {
            runDone = 1;
        }
    }

    printf("sippGenericRuntimeHWIsrThread : Thread terminating\n");

    #ifdef WIN32
    ExitThread((DWORD) 0);
    #else
    pthread_exit((void *)0);
    #endif

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericRuntimeSvuIsrThread                                   //
//                                                                                //
//  DESCRIPTION: MA2x5x SHAVE c-model ISR thread                                  //
//                                                                                //
//  INPUTS:      ptr - This is a pointer to pointer to SIPP pipeline struct       //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       Triggered from sippKickShaveM1PC                                 //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
DWORD WINAPI sippGenericRuntimeSvuIsrThread (void *ptr) {
#else
void * sippGenericRuntimeSvuIsrThread (void * ptr) {
#endif
    pSippPipeline pPipe;
    u8            runDone = 0;

    while (runDone == 0)
    {
        svuSyncSem->Wait();
        pPipe = *(pSippPipeline *)ptr;  // Load here as will not be known at thread creation time

        if (pPipe->useSyncRuntime == 0)
        {
            sippSvuDoneIrqHandler (IRQ_SVE_0);

            if (pPipe->iteration >= pPipe->nIter)
            {
                runDone = 1;
            }
        }
        else
        {
            runDone = 1;
        }
    }

    printf("sippGenericRuntimeSvuIsrThread : Thread terminating\n");

    #ifdef WIN32
    ExitThread((DWORD) 0);
    #else
    pthread_exit((void *)0);
    #endif

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericRuntimeCmxDmaIsrThread                                //
//                                                                                //
//  DESCRIPTION: MA2x5x CMX DMA c-model ISR thread                                //
//                                                                                //
//  INPUTS:      ptr - This is a pointer to pointer to SIPP pipeline struct       //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       Triggered from sippRunNextIter                                   //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
DWORD WINAPI sippGenericRuntimeCmxDmaIsrThread (void *ptr) {
#else
void * sippGenericRuntimeCmxDmaIsrThread (void * ptr) {
#endif
    pSippPipeline pPipe;
    u8            runDone = 0;

    while (runDone == 0)
    {
        cmxDmaSyncSem->Wait();
        pPipe = *(pSippPipeline *)ptr;  // Load here as will not be known at thread creation time

        if (pPipe->useSyncRuntime == 0)
        {
            sippCmxDmaDoneIrqHandler ( );

            if (pPipe->iteration >= pPipe->nIter)
            {
                runDone = 1;
            }
        }
        else
        {
            runDone = 1;
        }
    }

    printf("sippGenericRuntimeCmxDmaIsrThread : Thread terminating\n");

    #ifdef WIN32
    ExitThread((DWORD) 0);
    #else
    pthread_exit((void *)0);
    #endif

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericRuntimeAsyncCreateIsrThreads                          //
//                                                                                //
//  DESCRIPTION: Create the c-model ISR threads                                   //
//                                                                                //
//  INPUTS:      None.                                                            //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////


void sippGenericRuntimeAsyncCreateIsrThreads ()
{
    #ifdef WIN32
    pHWIsrThread     = CreateThread(0, 0, sippGenericRuntimeHWIsrThread, (LPVOID)&gSippHW.pSippLoadedHWPipe[0x0], 0, &hwIsrThreadId);
    pSvuIsrThread    = CreateThread(0, 0, sippGenericRuntimeSvuIsrThread, (LPVOID)&gSippHW.pSippLoadedHWPipe[0x0], 0, &svuIsrThreadId);
    pCmxDmaIsrThread = CreateThread(0, 0, sippGenericRuntimeCmxDmaIsrThread, (LPVOID)&gSippHW.pSippLoadedHWPipe[0x0], 0, &cmxDmaIsrThreadId);
    #else
    pthread_create(&pHWIsrThread, 0, sippGenericRuntimeHWIsrThread, (void *)&gSippHW.pSippLoadedHWPipe[0x0]);
    pthread_create(&pSvuIsrThread, 0, sippGenericRuntimeSvuIsrThread, (void *)&gSippHW.pSippLoadedHWPipe[0x0]);
    pthread_create(&pCmxDmaIsrThread, 0, sippGenericRuntimeCmxDmaIsrThread, (void *)&gSippHW.pSippLoadedHWPipe[0x0]);
    #endif

}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericRuntimeWaitIsrThreadTerm                              //
//                                                                                //
//  DESCRIPTION: Terminate all the ISR threads                                    //
//                                                                                //
//  INPUTS:      None.                                                            //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippGenericRuntimeWaitIsrThreadTerm ()
{
    sippPalTrace(SIPP_RUNTIME_TL_INFO, "sippGenericRuntimeWaitIsrThreadTerm : Terminating ISR threads\n");

    // Ensure any threads unused in last iteration can proceed
    hwSyncSem->Post();
    svuSyncSem->Post();
    cmxDmaSyncSem->Post();

    #ifdef WIN32
    WaitForSingleObject (pHWIsrThread, 60000);
    WaitForSingleObject (pSvuIsrThread, 60000);
    WaitForSingleObject (pCmxDmaIsrThread, 60000);
    #else
    pthread_join (pHWIsrThread, 0);
    pthread_join (pSvuIsrThread, 0);
    pthread_join (pCmxDmaIsrThread, 0);
    #endif

    // Delete the thread resources
    delete serialiseSem;
    delete hwSyncSem;
    delete svuSyncSem;
    delete cmxDmaSyncSem;
    #ifndef DISABLE_TRACE
    delete palTraceSerialiseSem;
    #endif /* !DISABLE_TRACE */

}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericPCRuntimeIsr                                          //
//                                                                                //
//  DESCRIPTION: MA2x5x_HW c-model callback                                       //
//                                                                                //
//  INPUTS:      pObj - SippHwFactory pointer                                     //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       The SIPP c-model is not multi-threaded                           //
//               So this ISR is registered with the SippHwM2Factory and it will   //
//               be called inline. To replicate conventional ISR behaviour we     //
//               will have this isr trigger an alternate thread for isr           //
//               processing.                                                      //
//               Note the c-model will only be single pipeline capable for now    //
//                                                                                //
//  CONTEXT:     Callback for MA2x5x_HW model                                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippGenericPCRuntimeIsr (void * pObj)
{
    u32             regVal, regVal0;
    SippHwFactory * hw;
    u32             waitMask;
    pSippPipeline   pPipe;

    pPipe    = gSippHW.pSippLoadedHWPipe[0x0];
    waitMask = pPipe->schedInfo[pPipe->iteration].sippHwWaitMask;
    hw       = (SippHwFactory *)pObj;

    #ifdef RT_DBG
    sippPalTrace(SIPP_RUNTIME_TL_INFO, "sippGenericPCRuntimeIsr : Entering irq handler - waitMask = %08x\n", waitMask);
    #endif

    regVal0 = hw->ApbRead (SIPP_INT0_STATUS_ADR);
    regVal  = hw->ApbRead (SIPP_INT1_STATUS_ADR);

    #ifdef RT_DBG
    sippPalTrace(SIPP_RUNTIME_TL_INFO, "sippGenericPCRuntimeIsr : Entering irq handler - waitMask = %08x - Status[0] = %08x, Status[1] = %08x\n", waitMask, regVal0, regVal);
    #endif

    hw->ApbWrite(SIPP_INT0_CLEAR_ADR, regVal0);

    if ((regVal == waitMask) &&
        (pPipe->useSyncRuntime == 0))
    {
        // Enable the irq handling thread
        hwSyncSem->Post();
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericPCWaitUnits                                           //
//                                                                                //
//  DESCRIPTION: Used as a fake wait in sync API for PC model only                //
//                                                                                //
//  INPUTS:      pPipe  - pipeline under test                                     //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippGenericPCWaitUnits (pSippPipeline pPipe)
{
    u32 waitMask = pPipe->schedInfo[pPipe->iteration].sippHwWaitMask;

    if (waitMask)
    {
        u32 status;
        sippPalTrace (SIPP_RUNTIME_TL_INFO, "sippGenericPCWaitUnits : waitMask %08x\n", waitMask);

        status = GET_REG_WORD_VAL(SIPP_INT1_STATUS_ADR);

        if ((status & waitMask) != waitMask)
        {
            sippPalTrace (SIPP_RUNTIME_TL_INFO, "sippGenericPCWaitUnits : HW Filters did not complete\n");
            sippError (pPipe->errorStatus, E_PC_RUNTIME_FAILURE);
        }
        else
        {
            #ifdef RT_DBG
            sippPalTrace (SIPP_RUNTIME_TL_INFO, "sippGenericPCWaitUnits : HW Filters complete\n");
            #endif

            //Clear status bits
            SET_REG_WORD (SIPP_INT1_CLEAR_ADR, waitMask);
        }
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Wait for DMA - This is a TBD - dma will be a blocking call so no need to do this
    if (pPipe->schedInfo[pPipe->iteration].dmaMask)
    {
        sippWaitDma ();
    }

    #if defined(SIPP_F_DUMPS)
    if (pPipe->dbgLevel > 0)
    {
        sippDbgDumpFilterOuts (pPipe);
    }
    #endif

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Final step: this is Leon-End-Of-Iteration-Overhead
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    sippGenericUpdateExecNums (pPipe);
}

#endif /* SIPP_PC */

