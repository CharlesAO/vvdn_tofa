/// @file      sippScheduleIsr.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP framework
///            ISRs used by the schedule based runtimes in asymc mode
///            Platform(s) supported : ma2x5x
///

/////////////////////////////////////////////////////////////////////////////////
//  Header Files
/////////////////////////////////////////////////////////////////////////////////
#include <sipp.h>
#include <sippInternal.h>
#ifdef MYRIAD2
#include <DrvIcbDefines.h>
#include <DrvIcb.h>
#endif
#ifdef SIPP_PC
#include "wrapperSem.h"
#endif

/////////////////////////////////////////////////////////////////////////////////
//  Global Variables
/////////////////////////////////////////////////////////////////////////////////
extern pSIPP_HW_SESSION pgSippHW;
#ifdef SIPP_PC
extern Semaphore *      serialiseSem;
#endif

u32 sippGlobalOBFLIncStatus = 0;

/////////////////////////////////////////////////////////////////////////////////
//  Global Macros
/////////////////////////////////////////////////////////////////////////////////

#ifdef SIPP_PC
// From DrvIcbDefines.h which I cannot include due to type conflicts with Sipp HW c-model
#define IRQ_SVE_0       (  32 )           // LOS & LRT
#endif
/////////////////////////////////////////////////////////////////////////////////
//  Code
/////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippIbflDecHandler                                               //
//                                                                                //
//  DESCRIPTION: Main handler for input buffer fill level decrement ISR           //
//                                                                                //
//  INPUTS:      irqSource - Label from ICB of the triggered interrupt            //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       The active pipeline wait masks are checked and appropriate       //
//               pipelines triggered                                              //
//                                                                                //
//  CONTEXT:     Called as an ISR                                                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippIbflDecHandler (u32 irqSource)
{
    u32                 pipeIdx;
    u32                 status;

    #ifdef MYRIAD2
    #ifdef RT_DBG
    SIPP_HISTORY_LOG(0xCACA0000 + irqSource);
    #endif
    VCS_PRINT_INT(0xCACA0000 + irqSource); // VCS debug msg
    #endif

    status = GET_REG_WORD_VAL(SIPP_INT0_STATUS_ADR);

    #ifdef RT_DBG
    SIPP_HISTORY_LOG(status);
    #endif

    for (pipeIdx = 0; pipeIdx < SIPP_MAX_SUPPORTED_HW_PIPELINES; pipeIdx++)
    {
        if (pgSippHW->pSippLoadedHWPipe[pipeIdx])
        {
            #ifdef SIPP_SCRT_ENABLE_OPIPE
            SippPipeline * pPipe;

            pPipe  = pgSippHW->pSippLoadedHWPipe[pipeIdx];

            if (pPipe->pfnSippSchedule == sippOPipeSchedule)
            {
                u32 localStatus = sippOpipeHandleIbflDec (pPipe, status);
                status         &= (~localStatus); // Make sure irq does not get cleared twice!
            }
            #endif
        }
    }

    SET_REG_WORD(SIPP_INT0_CLEAR_ADR, status);

    #ifndef SIPP_PC
    DrvIcbIrqClear (irqSource);
    #endif
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippObflIncHandler                                               //
//                                                                                //
//  DESCRIPTION: Main handler for output buffer fill level increment ISR          //
//                                                                                //
//  INPUTS:      irqSource - Label from ICB of the triggered interrupt            //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       The active pipeline wait masks are checked and appropriate       //
//               pipelines triggered                                              //
//                                                                                //
//  CONTEXT:     Called as an ISR                                                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippObflIncHandler (u32 irqSource)
{
    SippPipeline *      pPipe;
    u32                 pipeIdx;
    u32                 status;
    #ifdef RT_DBG
    ptSippPipelineSuper ptSuperPipe;
    #endif

    #ifdef MYRIAD2
    #ifdef RT_DBG
    SIPP_HISTORY_LOG(0xCBCB0000 + irqSource);
    #endif
    VCS_PRINT_INT(0xCBCB0000 + irqSource); // VCS debug msg
    #endif

    status                   = GET_REG_WORD_VAL(SIPP_INT1_STATUS_ADR);
    sippGlobalOBFLIncStatus |= status;

    #ifdef RT_DBG
    SIPP_HISTORY_LOG(status);
    #endif

    for (pipeIdx = 0; pipeIdx < SIPP_MAX_SUPPORTED_HW_PIPELINES; pipeIdx++)
    {
        if (pgSippHW->pSippLoadedHWPipe[pipeIdx])
        {
            u32 waitMask;
            UNUSED(waitMask);
            #ifdef BUGZILLA_22709
            u32 waitEoFMask;
            UNUSED(waitEoFMask);
            #endif

            pPipe  = pgSippHW->pSippLoadedHWPipe[pipeIdx];

            #ifdef SIPP_SCRT_ENABLE_OPIPE
            if (pPipe->pfnSippSchedule == sippOPipeSchedule)
            {
                u32 localStatus          = sippOpipeHandleObflInc (pPipe, status, 0);
                sippGlobalOBFLIncStatus &= (~localStatus);
                status                  &= (~localStatus); // Make sure irq does not get cleared twice!
            }
            else
            #endif
            {
                // Non-opipe runtimes
                waitMask = pPipe->schedInfo[pPipe->iteration].sippHwWaitMask;

                #ifdef BUGZILLA_22709
                waitEoFMask  = pPipe->schedInfo[pPipe->iteration].sippHwWaitEoFMask;
                waitMask    &= (~waitEoFMask);
                #endif

                if ((pPipe->useSyncRuntime == 0) &&
                    (waitMask))
                {
                    if((sippGlobalOBFLIncStatus & waitMask) == waitMask)
                    {
                        #ifdef RT_DBG
                        ptSuperPipe = (ptSippPipelineSuper)pPipe;
                        SIPP_HISTORY_LOG(0xFF000000|ptSuperPipe->uPipeIdx);

                        #endif

                        #ifdef SIPP_RUNTIME_PROFILING
                        if (pPipe->pRTStats)
                        {
                            DrvTimerGetSystemTicks64 (&pPipe->pRTStats->iterTime[(pPipe->iteration * SIPP_RT_PER_ITER_STATS_SIZE) + SIPP_ITER_STAT_ASYNC_HW_POS]);
                        }
                        #endif

                        sippGlobalOBFLIncStatus &= (~waitMask);
                        status                  &= (~waitMask); // Make sure irq does not get cleared twice!
                        SET_REG_WORD(SIPP_INT1_CLEAR_ADR, pPipe->schedInfo[pPipe->iteration].sippHwWaitMask);
                        sippCheckIterComplete (pPipe, PFL_SIPP_DONE);
                    }
                }
            }
        }
    }

    SET_REG_WORD(SIPP_INT1_CLEAR_ADR, status);

    #ifndef SIPP_PC
    DrvIcbIrqClear (irqSource);
    #endif
}

#ifdef BUGZILLA_22709

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippFrameDoneIrqHandler                                          //
//                                                                                //
//  DESCRIPTION: HW filter end of frame interrupt service routine                 //
//                                                                                //
//  INPUTS:      irqSource - Label from ICB of the triggered interrupt            //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       The active pipeline wait masks are checked and appropriate       //
//               pipelines triggered                                              //
//                                                                                //
//  CONTEXT:     Called as an ISR                                                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippFrameDoneIrqHandler (u32 irqSource)
{
    SippPipeline *      pPipe;
    u32                 pipeIdx;
    u32                 regVal;
    #ifdef RT_DBG
    ptSippPipelineSuper ptSuperPipe;
    #endif
    #ifdef MYRIAD2
    #ifdef RT_DBG
    SIPP_HISTORY_LOG(0xCACA0000 + irqSource);
    #endif
    VCS_PRINT_INT(0xCACA0000 + irqSource); // VCS debug msg
    #endif

    regVal = GET_REG_WORD_VAL(SIPP_INT2_STATUS_ADR);

    #ifdef RT_DBG
    SIPP_HISTORY_LOG(regVal);
    #endif

    if (pgSippHW->uNumCurrHwPipes)
    {
        for (pipeIdx = 0; pipeIdx < SIPP_MAX_SUPPORTED_HW_PIPELINES; pipeIdx++)
        {
            if (pgSippHW->pSippLoadedHWPipe[pipeIdx])
            {
                pPipe = pgSippHW->pSippLoadedHWPipe[pipeIdx];

                #ifdef SIPP_SCRT_ENABLE_OPIPE
                if (pPipe->pfnSippSchedule == sippOPipeSchedule)
                {
                    sippOPipeEofIsr (pPipe, regVal);
                }
                else
                #endif
                {
                    if ((pPipe->useSyncRuntime == 0) &&
                    (pPipe->schedInfo[pPipe->iteration].sippHwWaitEoFMask))
                    {

                        #ifdef SIPP_NO_IRQ_BARRIER2
                        // Check if any of the units showing EOF belong to this pipe
                        if (regVal & pPipe->schedInfo[pPipe->iteration].sippHwWaitEoFMask)
                        {

                            #ifdef RT_DBG
                            ptSuperPipe = (ptSippPipelineSuper)pPipe;
                            SIPP_HISTORY_LOG(0xFF000000|ptSuperPipe->uPipeIdx);
                            #endif

                            // Clear only those units showing EOF belonging to the current pipe
                            SET_REG_WORD(SIPP_INT1_CLEAR_ADR, (regVal & pPipe->schedInfo[pPipe->iteration].sippHwWaitEoFMask));
                            SET_REG_WORD(SIPP_INT2_CLEAR_ADR, (regVal & pPipe->schedInfo[pPipe->iteration].sippHwWaitEoFMask));

                            pgSippHW->sippHWEOFMask[pipeIdx] |= (regVal & pPipe->schedInfo[pPipe->iteration].sippHwWaitEoFMask);

                            #ifdef SIPP_RUNTIME_PROFILING
                            if (pPipe->pRTStats)
                            {
                                DrvTimerGetSystemTicks64 (&pPipe->pRTStats->iterTime[(pPipe->iteration * SIPP_RT_PER_ITER_STATS_SIZE) + SIPP_ITER_STAT_ASYNC_HW_POS]);
                            }
                            #endif

                            if (pgSippHW->sippHWEOFMask[pipeIdx] == pPipe->schedInfo[pPipe->iteration].sippHwWaitEoFMask)
                            {
                                sippCheckIterComplete (pPipe, PFL_SIPP_EOF_DONE);
                            }
                        }
                        #else
                        if ((regVal & pPipe->schedInfo[pPipe->iteration].sippHwWaitEoFMask) ==
                            pPipe->schedInfo[pPipe->iteration].sippHwWaitEoFMask)
                        {

                            #ifdef RT_DBG
                            ptSuperPipe = (ptSippPipelineSuper)pPipe;
                            SIPP_HISTORY_LOG(0xFF000000|ptSuperPipe->uPipeIdx);

                            #endif

                            #ifdef SIPP_RUNTIME_PROFILING
                            if (pPipe->pRTStats)
                            {
                                DrvTimerGetSystemTicks64 (&pPipe->pRTStats->iterTime[(pPipe->iteration * SIPP_RT_PER_ITER_STATS_SIZE) + SIPP_ITER_STAT_ASYNC_HW_POS]);
                            }
                            #endif

                            SET_REG_WORD(SIPP_INT1_CLEAR_ADR, pPipe->schedInfo[pPipe->iteration].sippHwWaitEoFMask);
                            SET_REG_WORD(SIPP_INT2_CLEAR_ADR, pPipe->schedInfo[pPipe->iteration].sippHwWaitEoFMask);
                            sippCheckIterComplete (pPipe, PFL_SIPP_EOF_DONE);
                        }
                        #endif
                    }
                }
            }
        }

        sippGlobalOBFLIncStatus &= (~regVal);
        SET_REG_WORD(SIPP_INT1_CLEAR_ADR, regVal);
        SET_REG_WORD(SIPP_INT2_CLEAR_ADR, regVal);
    }
    else
    {
        sippGlobalOBFLIncStatus &= (~regVal);
        SET_REG_WORD(SIPP_INT1_CLEAR_ADR, regVal);
        SET_REG_WORD(SIPP_INT2_CLEAR_ADR, regVal);
    }

    #ifndef SIPP_PC
    DrvIcbIrqClear (irqSource);
    #endif
}

#endif

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippSvuDoneIrqHandler                                            //
//                                                                                //
//  DESCRIPTION: Shave done interrupt service routine                             //
//                                                                                //
//  INPUTS:      irqSource - Label from ICB of the triggered interrupt            //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     Called as an ISR                                                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippSvuDoneIrqHandler (u32 irqSource)
{
    SippPipeline *      pPipe;
    u32                 pipeIdx;
    #ifdef RT_DBG
    ptSippPipelineSuper ptSuperPipe;
    #endif

    // So which pipeline is this shave interrupting us for?
    pipeIdx = irqSource - IRQ_SVE_0;

    #ifdef MYRIAD2
    SET_REG_WORD (DCU_IRR(pipeIdx), 0xFFFFFFFF);
    #endif

    if (pgSippHW->pSippCurrSvuPipe[pipeIdx])
    {
        pPipe = pgSippHW->pSippCurrSvuPipe[pipeIdx];
        #ifdef RT_DBG
        ptSuperPipe = (ptSippPipelineSuper)pPipe;
        SIPP_HISTORY_LOG(0xEE000000|ptSuperPipe->uPipeIdx);
        #endif

        #ifdef SIPP_RUNTIME_PROFILING
        if (pPipe->pRTStats)
        {
            DrvTimerGetSystemTicks64 (&pPipe->pRTStats->iterTime[(pPipe->iteration * SIPP_RT_PER_ITER_STATS_SIZE) + SIPP_ITER_STAT_ASYNC_SW_POS]);
        }
        #endif

        sippCheckIterComplete (pPipe, PFL_SVU_DONE);
    }
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippCheckIterComplete                                            //
//                                                                                //
//  DESCRIPTION: Make sure all expected clients for a pipeline iteration are      //
//               complete                                                         //
//                                                                                //
//  INPUTS:      pPipe - Pipeline under observation                               //
//               flag  - any new flag the caller wished to set                    //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       The active interrupt barriers are checked and appropriate        //
//               pipelines triggered                                              //
//                                                                                //
//  CONTEXT:     Called as an ISR                                                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippCheckIterComplete (SippPipeline * pPipe, u32 flag)
{
    #ifdef SIPP_PC
    // Claim serialisation semaphore
    serialiseSem->Wait();
    #endif

    pPipe->isrFlags |= flag;

    if (pPipe->iteration < pPipe->nIter)
    {
    	// For the current iteration, which are we waiting on?
        #ifdef RT_DBG
        SIPP_HISTORY_LOG(0xDADD0000 + pPipe->iteration);
        #endif
        #ifdef RT_DBG
        SIPP_PIPE_LOG((0xBB000000 | flag),pPipe);
        #endif

        if (pPipe->setIterMask)
        {
            if (pPipe->schedInfo[pPipe->iteration].dmaMask)
            {
                pPipe->allDoneMask |= PFL_DMA_DONE;
            }

            if ((pPipe->schedInfo[pPipe->iteration].shaveMask) ||
                (pPipe->flags & PLF_REQUIRES_SW_PADDING))
            {
                pPipe->allDoneMask |= PFL_SVU_DONE;
            }

            if (pPipe->schedInfo[pPipe->iteration].sippHwWaitMask)
            {
                pPipe->allDoneMask |= PFL_SIPP_DONE;
            }

            #ifdef BUGZILLA_22709
            if (pPipe->schedInfo[pPipe->iteration].sippHwWaitEoFMask)
            {
                u32 waitMask = pPipe->schedInfo[pPipe->iteration].sippHwWaitMask;

                pPipe->allDoneMask |= PFL_SIPP_EOF_DONE;

                waitMask &= (~pPipe->schedInfo[pPipe->iteration].sippHwWaitEoFMask);
                if (waitMask == 0)
                {
                    pPipe->allDoneMask &= (~PFL_SIPP_DONE);
                }
            }
            #endif

            pPipe->setIterMask = 0;
        }


        #ifdef RT_DBG
        SIPP_HISTORY_LOG(0xDADE0000 + pPipe->allDoneMask);
        #endif

        if (pPipe->isrFlags == pPipe->allDoneMask)
        {
            #ifdef SIPP_PC
            u32 allDoneMask = pPipe->allDoneMask;
            #endif
            pPipe->setIterMask = 1;
            pPipe->allDoneMask = 0;

            #ifdef RT_DBG
            SIPP_HISTORY_LOG(0xDADF0000);
            #endif
            #ifdef SIPP_PC
            if (allDoneMask)
            #endif
            if (pPipe->pfnSippRunIterDone (pPipe) == 0)
            {
                pPipe->pfnSippRunNextIter (pPipe);
            }
        }
    }

    #ifdef SIPP_PC
    // Release serialisation semaphore
    serialiseSem->Post();
    #endif

}

