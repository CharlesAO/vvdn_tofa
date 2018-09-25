///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Multiple Line Per Iteration runtime, ma2x5x specific aspect
///

#ifdef SIPP_SCRT_ENABLE_MLPI

////////////////////////////////////////////////////
// Header files
////////////////////////////////////////////////////

#include <sipp.h>
#include <sippInternal.h>

/////////////////////////////////////////////////////////////////////////////////
//  Local macros
/////////////////////////////////////////////////////////////////////////////////

#define SIPP_MLPI_CQ_ADD_WRITE(qu, address, val) { \
                                                     qu->quEntry[qu->quNum].addr  = (u32)address; \
                                                     qu->quEntry[qu->quNum].value = val; \
                                                     qu->quNum++; \
                                                 }

/////////////////////////////////////////////////////////////////////////////////
//  Global Variables
/////////////////////////////////////////////////////////////////////////////////
#ifdef BUGZILLA_22709
#ifdef SIPP_NO_IRQ_BARRIER2
extern pSIPP_HW_SESSION pgSippHW;
#endif
#endif

tMLPIStartCQCtrl tsMLPIStartCQCtrl[SIPP_MAX_SUPPORTED_HW_PIPELINES][0x2];

#ifdef SIPP_HW_TIME_EXACT
extern u64 HwStartTick[3000];
extern u64 HwStopTick[3000];
#endif

////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////

extern void sippMLPIWaitUnits (SippPipeline * pl);
#ifdef SIPP_PC
void sippMLPIPCWaitUnits (pSippPipeline pPipe);
#endif

////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIStartHWUnits2x5x                                         //
//                                                                                //
//  DESCRIPTION: Start HW filters for current iteration                           //
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

void sippMLPIStartHWUnits2x5x (SippPipeline * pPipe)
{
    u32          filtIdx;
    u32          startMask    = pPipe->schedInfo[pPipe->iteration].sippHwWaitMask;
    u32          numLinesIter = pPipe->linesPerIter;
    SippFilter * ptrFilt;
    u32          oBufIdx;

    // Setup the interrupt barrier
    #ifndef SIPP_PC
    if (pPipe->useSyncRuntime == 0)
    {
        u32 barrier1Mask = startMask;
        #ifdef BUGZILLA_22709
        u32 barrier2Mask = pPipe->schedInfo[pPipe->iteration].sippHwWaitEoFMask;

        if (barrier2Mask)
        {
            barrier1Mask &= (~barrier2Mask);
        }

        #ifdef SIPP_NO_IRQ_BARRIER2
        pgSippHW->sippHWEOFMask[pPipe->HWPipeID] = 0;
        #else
        SET_REG_WORD((SIPP_INT2_BARRIER0_ADR + (pPipe->HWPipeID << 0x2)), barrier2Mask);
        #endif
        #endif // BUGZILLA_22709

        #ifndef NO_INTERRUPT_BARRIERS_MODE
        SET_REG_WORD((SIPP_INT1_BARRIER0_ADR + (pPipe->HWPipeID << 0x2)), barrier1Mask);
        #endif
    }
    #endif

    #ifdef SIPP_HW_TIME_EXACT
    DrvTimerGetSystemTicks64 (&HwStartTick[pPipe->iteration]);
    #endif

    if ((startMask & pPipe->firstUseMask) != startMask)
    {
        // Some of these HW filters are being started for the first time...
        // Establish which ones
        u32 firstStartMask    = startMask & (~(pPipe->firstUseMask));
        u32 manageFirstMask   = 0;

        pPipe->nFirstRunFilts = 0;

        // Establish if their first run differs in the number of iterations
        // expected from the remaining runs
        for (filtIdx = 0; filtIdx < pPipe->nFilters; filtIdx++)
        {
            // Should have a better list with only the HW filters in it to save time
            ptrFilt = pPipe->filters[filtIdx];

            if (ptrFilt->unit <= SIPP_MAX_FILTER_ID)
            {
                if (firstStartMask & (1 << ptrFilt->unit))
                {
                    if (ptrFilt->firstRunNumLines != ptrFilt->linesPerIter)
                    {
                        u32 oBufIdx;

                        for (oBufIdx = 0x0; oBufIdx < ptrFilt->numOBufs; oBufIdx++)
                        {
                            manageFirstMask |= ptrFilt->oBufs[oBufIdx]->hwOutputBufId;
                        }

                        pPipe->firstRunFilts[pPipe->nFirstRunFilts++] = ptrFilt;
                    }
                }
            }
        }

        // If the number of iters differs, mark that processing a first run of a filter is now in progress
        pPipe->firstUseInProg = manageFirstMask;

        // Mark that these filters have now been started at least once
        pPipe->firstUseMask  |= startMask;

        // Update startMask for this iter so that only filters not handled here are handled
        startMask &= (~(manageFirstMask));

    }

    if (pPipe->firstUseInProg)
    {
        // Set up the filter for a lesser number of lines to cause the interrupt
        for (filtIdx = 0; filtIdx < pPipe->nFirstRunFilts; filtIdx++)
        {
            ptrFilt = pPipe->firstRunFilts[filtIdx];

            u32 runLines = sippPalFindHighestBit (ptrFilt->firstRunNumLines);

            for (oBufIdx = 0; oBufIdx < ptrFilt->numOBufs; oBufIdx++)
            {
                u32 localRunLines = (0x1 << runLines);
                u32 bufferID      = sippPalFindHighestBit (ptrFilt->oBufs[oBufIdx]->hwOutputBufId);

                if (bufferID !=  0xFFFFFFFF)
                {
                    sippBufSetupIrqRate (ptrFilt->oBuf[oBufIdx],
                                         0x1,
                                         bufferID,
                                         localRunLines);

                    while (localRunLines)
                    {
                        SET_REG_WORD(SIPP_OBFL_DEC_ADR, ptrFilt->oBufs[oBufIdx]->hwOutputBufId);
                        localRunLines--;
                    }
                }
            }

            ptrFilt->firstRunNumLines &= (~(0x1 << runLines));

        }
    }

    if (pPipe->startDelta)
    {
        u32 deltaStartMask = startMask & ~(pPipe->startDelta);

        if (deltaStartMask)
        {
            while (numLinesIter)
            {
                SET_REG_WORD(SIPP_OBFL_DEC_ADR, deltaStartMask);
                numLinesIter--;
            }
        }

        // Now do all the filters with non standard lines per iteration
        deltaStartMask   = startMask & pPipe->startDelta;

        if (deltaStartMask)
        {
            for (filtIdx = 0; filtIdx < pPipe->nFilters; filtIdx++)
            {
                // Should have a better list with only the HW filters in it to save time
                ptrFilt = pPipe->filters[filtIdx];

                if (ptrFilt->unit <= SIPP_MAX_FILTER_ID)
                {
                    if (deltaStartMask & (1 << ptrFilt->unit))
                    {
                        for (oBufIdx = 0; oBufIdx < ptrFilt->numOBufs; oBufIdx++)
                        {
                            u32 numIters = ptrFilt->linesPerIter;

                            while (numIters)
                            {
                                SET_REG_WORD(SIPP_OBFL_DEC_ADR, ptrFilt->oBufs[oBufIdx]->hwOutputBufId);
                                numIters--;
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        while (numLinesIter)
        {
            SET_REG_WORD(SIPP_OBFL_DEC_ADR, startMask);
            numLinesIter--;
        }
    }

    #ifdef SIPP_HW_TIME_EXACT
    extern void sippMLPIRunFirstIterHwFilt (SippPipeline * pPipe);

    {
        u32 waitMask     = pPipe->schedInfo[pPipe->iteration].sippHwWaitMask;

        #ifdef BUGZILLA_22709
        u32 waitEoFMask  = pPipe->schedInfo[pPipe->iteration].sippHwWaitEoFMask;
        waitMask    &= (~waitEoFMask);
        #endif

        if (waitMask)
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

            DrvTimerGetSystemTicks64 (&HwStopTick[pPipe->iteration]);

            // Clear status bits
            SET_REG_WORD(SIPP_INT1_CLEAR_ADR, pPipe->schedInfo[pPipe->iteration].sippHwWaitMask);
            pPipe->schedInfo[pPipe->iteration].sippHwWaitMask = 0;
        }

        sippMLPIRunFirstIterHwFilt (pPipe);

        DrvTimerGetSystemTicks64 (&HwStopTick[pPipe->iteration]);
    }
    #endif

}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIStartHWUnits2x5xCQ                                       //
//                                                                                //
//  DESCRIPTION: SW CQ version of sippMLPIStartHWUnits                            //
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

void sippMLPIStartHWUnits2x5xCQ (SippPipeline * pPipe,
                                 u32            iteration)
{
    u32               filtIdx;
    u32               startMask        = pPipe->schedInfo[iteration].sippHwWaitMask;
    u32               numLinesIter     = pPipe->linesPerIter;
    SippFilter *      ptrFilt;
    u32               oBufIdx;
    psSippCMDQu       pQu              = &pPipe->tCMDStartQu;
    ptMLPIStartCQCtrl pMLPIStartCQCtrl = &tsMLPIStartCQCtrl[pPipe->HWPipeID][iteration & 0x1];

    if ((u32)iteration >= pPipe->nIter)
        return;

    // Setup the interrupt barrier
    #ifndef SIPP_PC
    if (pPipe->useSyncRuntime == 0)
    {
        u32 barrier1Mask = startMask;
        #ifdef BUGZILLA_22709
        u32 barrier2Mask = pPipe->schedInfo[iteration].sippHwWaitEoFMask;

        if (barrier2Mask)
        {
            barrier1Mask &= (~barrier2Mask);
        }

        #ifdef SIPP_NO_IRQ_BARRIER2
        SIPP_MLPI_CQ_ADD_WRITE(pQu, &pgSippHW->sippHWEOFMask[pPipe->HWPipeID], 0);
        #else
        SIPP_MLPI_CQ_ADD_WRITE(pQu, (SIPP_INT2_BARRIER0_ADR + (pPipe->HWPipeID << 0x2)), barrier2Mask);
        #endif
        #endif // BUGZILLA_22709

        #ifndef NO_INTERRUPT_BARRIERS_MODE
        SIPP_MLPI_CQ_ADD_WRITE(pQu, (SIPP_INT1_BARRIER0_ADR + (pPipe->HWPipeID << 0x2)), barrier1Mask);
        #endif
    }
    #endif

    if ((startMask & pPipe->firstUseMask) != startMask)
    {
        // Some of these HW filters are being started for the first time...
        // Establish which ones
        u32 firstStartMask    = startMask & (~(pPipe->firstUseMask));
        u32 manageFirstMask   = 0;

        pMLPIStartCQCtrl->nFirstRunFilts = 0;

        // Establish if their first run differs in the number of iterations
        // expected from the remaining runs
        for (filtIdx = 0; filtIdx < pPipe->nFilters; filtIdx++)
        {
            // Should have a better list with only the HW filters in it to save time
            ptrFilt = pPipe->filters[filtIdx];

            if (ptrFilt->unit <= SIPP_MAX_FILTER_ID)
            {
                if (firstStartMask & (1 << ptrFilt->unit))
                {
                    if (ptrFilt->firstRunNumLines != ptrFilt->linesPerIter)
                    {
                        u32 oBufIdx;

                        for (oBufIdx = 0x0; oBufIdx < ptrFilt->numOBufs; oBufIdx++)
                        {
                            manageFirstMask |= ptrFilt->oBufs[oBufIdx]->hwOutputBufId;
                        }

                        pMLPIStartCQCtrl->firstRunFilts[pMLPIStartCQCtrl->nFirstRunFilts++] = ptrFilt;
                        // pPipe->firstRunFilts[pPipe->nFirstRunFilts++] = ptrFilt;
                    }
                }
            }
        }

        // If the number of iters differs, mark that processing a first run of a filter is now in progress
        pMLPIStartCQCtrl->firstUseInProg = manageFirstMask;

        // Mark that these filters have now been started at least once
        pPipe->firstUseMask  |= startMask;

        // Update startMask for this iter so that only filters not handled here are handled
        startMask &= (~(manageFirstMask));

    }

    if (pMLPIStartCQCtrl->firstUseInProg)
    {
        // Set up the filter for a lesser number of lines to cause the interrupt
        for (filtIdx = 0; filtIdx < pMLPIStartCQCtrl->nFirstRunFilts; filtIdx++)
        {
            ptrFilt = pMLPIStartCQCtrl->firstRunFilts[filtIdx];

            u32 runLines = sippPalFindHighestBit (ptrFilt->firstRunNumLines);

            for (oBufIdx = 0; oBufIdx < ptrFilt->numOBufs; oBufIdx++)
            {
                u32 localRunLines = (0x1 << runLines);
                u32 bufferID      = sippPalFindHighestBit (ptrFilt->oBufs[oBufIdx]->hwOutputBufId);

                if (bufferID !=  0xFFFFFFFF)
                {
                    sippBufSetupIrqRateCQ (ptrFilt->oBuf[oBufIdx],
                                           0x1,
                                           bufferID,
                                           localRunLines,
                                           pQu);

                    while (localRunLines)
                    {
                        SIPP_MLPI_CQ_ADD_WRITE(pQu, SIPP_OBFL_DEC_ADR, ptrFilt->oBufs[oBufIdx]->hwOutputBufId);
                        localRunLines--;
                    }
                }
            }

            ptrFilt->firstRunNumLines &= (~(0x1 << runLines));

        }
    }

    if (pPipe->startDelta)
    {
        u32 deltaStartMask = startMask & ~(pPipe->startDelta);

        if (deltaStartMask)
        {
            while (numLinesIter)
            {
                SIPP_MLPI_CQ_ADD_WRITE(pQu, SIPP_OBFL_DEC_ADR, deltaStartMask);
                numLinesIter--;
            }
        }

        // Now do all the filters with non standard lines per iteration
        deltaStartMask   = startMask & pPipe->startDelta;

        if (deltaStartMask)
        {
            for (filtIdx = 0; filtIdx < pPipe->nFilters; filtIdx++)
            {
                // Should have a better list with only the HW filters in it to save time
                ptrFilt = pPipe->filters[filtIdx];

                if (ptrFilt->unit <= SIPP_MAX_FILTER_ID)
                {
                    if (deltaStartMask & (1 << ptrFilt->unit))
                    {
                        for (oBufIdx = 0; oBufIdx < ptrFilt->numOBufs; oBufIdx++)
                        {
                            u32 numIters = ptrFilt->linesPerIter;

                            while (numIters)
                            {
                                SIPP_MLPI_CQ_ADD_WRITE(pQu, SIPP_OBFL_DEC_ADR, ptrFilt->oBufs[oBufIdx]->hwOutputBufId);
                                numIters--;
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        while (numLinesIter)
        {
            SIPP_MLPI_CQ_ADD_WRITE(pQu, SIPP_OBFL_DEC_ADR, startMask);
            numLinesIter--;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIRunStartCQ                                               //
//                                                                                //
//  DESCRIPTION: Uses the values collected in a software command queue by         //
//               sippMLPIStartHWUnits2x5xCQ to start HW filters                   //
//                                                                                //
//  INPUTS:      pPipe - pipeline to start                                        //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippMLPIRunStartCQ (SippPipeline * pPipe)
{
    psSippCMDQu  pQu = &pPipe->tCMDStartQu;
    u32          writeIdx;

    if (pQu->quNum > pQu->quSize)
    {
        sippPalTrace (SIPP_RUNTIME_TL_CATASTROPHE, "MLPI CQ Iter error @ iter %lu\n", pPipe->iteration);
    }

    for (writeIdx = 0; writeIdx < pQu->quNum; writeIdx++)
    {
        SET_REG_WORD(pQu->quEntry[writeIdx].addr, pQu->quEntry[writeIdx].value);
    }

    // Reset the number of qu entries
    pQu->quNum = 0;

}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIUpdateHWUnits2x5x                                        //
//                                                                                //
//  DESCRIPTION: Per iteration call to update input buffer fill for HW filters    //
//               connected to CMX buffers whose filter levels incremented on      //
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

void sippMLPIUpdateHWUnits2x5x (SippPipeline * pPipe)
{
    u32          numIters    = pPipe->linesPerIter;
    u32          ibflIncMask = pPipe->schedInfo[pPipe->iteration].sippIbflIncMask;
    u32          filtIdx;
    SippFilter * ptrFilt;

    #ifdef RT_DBG
    sippPalTrace (SIPP_RUNTIME_TL_INFO, "Incrementing LBs on units %08x\n", pPipe->schedInfo[pPipe->iteration].sippIbflIncMask);
    #endif

    if ((ibflIncMask & pPipe->firstIbflUseMask) != ibflIncMask)
    {
        // Some of these filters are outputting data for the first time...
        // Establish which ones
        u32 firstIbflMask   = ibflIncMask & (~(pPipe->firstIbflUseMask));
        u32 manageFirstMask = 0;

        // Establish if their first run differs in the number of iterations
        // expected from the remaining runs
        // This should only be the case for HW filters
        for (filtIdx = 0; filtIdx < pPipe->nFilters; filtIdx++)
        {
            // Should have a better list with only the HW filters in it to save time
            ptrFilt = pPipe->filters[filtIdx];

            if (ptrFilt->unit <= SIPP_MAX_FILTER_ID)
            {
                u32 oBufIdx;

                for (oBufIdx = 0; oBufIdx < ptrFilt->numOBufs; oBufIdx++)
                {
                    if (ptrFilt->oBufs[oBufIdx]->hwInputBufId & firstIbflMask)
                    {
                        u32 firstIters = ptrFilt->firstIterLines;

                        while (firstIters)
                        {
                            SET_REG_WORD(SIPP_IBFL_INC_ADR, ptrFilt->oBufs[oBufIdx]->hwInputBufId);
                            firstIters--;
                        }

                        manageFirstMask |= ptrFilt->oBufs[oBufIdx]->hwInputBufId;
                    }
                }
            }
        }

        // Mark that these filters have now been started at least once
        pPipe->firstIbflUseMask  |= ibflIncMask;

        // Update startMask for this iter so that only filters not handled here are handled in remainder of function
        ibflIncMask &= (~(manageFirstMask));
    }

    if (pPipe->ibflIncDelta)
    {
        u32 deltaIbflIncMask = ibflIncMask & ~(pPipe->ibflIncDelta);

        if (deltaIbflIncMask)
        {
            while (numIters)
            {
                SET_REG_WORD(SIPP_IBFL_INC_ADR, deltaIbflIncMask);
                numIters--;
            }
        }

        deltaIbflIncMask   = ibflIncMask & pPipe->ibflIncDelta;

        if (deltaIbflIncMask)
        {
            u32 filtIdx;

            for (filtIdx = 0; filtIdx < pPipe->nFilters; filtIdx++)
            {
                SippFilter * ptrFilt;
                u32          oBufIdx;

                // Could have a better list with only the relevant filters in it to save time
                ptrFilt = pPipe->filters[filtIdx];

                for (oBufIdx = 0; oBufIdx < ptrFilt->numOBufs; oBufIdx++)
                {
                    if (ptrFilt->oBufs[oBufIdx]->hwInputBufId & deltaIbflIncMask)
                    {
                        u32 filtNumIters = ptrFilt->linesPerIter;

                        while (filtNumIters)
                        {
                            SET_REG_WORD(SIPP_IBFL_INC_ADR, ptrFilt->oBufs[oBufIdx]->hwInputBufId);
                            filtNumIters--;
                        }
                    }
                }
            }
        }
    }
    else
    {
        while (numIters)
        {
            SET_REG_WORD(SIPP_IBFL_INC_ADR, ibflIncMask);
            numIters--;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIUpdateHWUnits2x5xCQ                                      //
//                                                                                //
//  DESCRIPTION: A version of sippMLPIUpdateHWUnits2x5x which writes to a SW      //
//               command queue as opposed to registers, enabling a fast update to //
//               take place at a later stage                                      //
//                                                                                //
//  INPUTS:      pPipe - pipeline to update                                       //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       This function should oiverlap with the framework 'waiting'       //
//               period. The SW CQ is drained when the wait is complete to enable //
//               update and filter restart to happen more quickly                 //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippMLPIUpdateHWUnits2x5xCQ (SippPipeline * pPipe)
{
    u32          numIters    = pPipe->linesPerIter;
    u32          ibflIncMask = pPipe->schedInfo[pPipe->iteration].sippIbflIncMask;
    u32          filtIdx;
    SippFilter * ptrFilt;
    psSippCMDQu  pQu         = &pPipe->tCMDUpdateQu;

    #ifdef RT_DBG
    sippPalTrace (SIPP_RUNTIME_TL_INFO, "Incrementing LBs on units %08x\n", pPipe->schedInfo[pPipe->iteration].sippIbflIncMask);
    #endif

    if ((ibflIncMask & pPipe->firstIbflUseMask) != ibflIncMask)
    {
        // Some of these filters are outputting data for the first time...
        // Establish which ones
        u32 firstIbflMask   = ibflIncMask & (~(pPipe->firstIbflUseMask));
        u32 manageFirstMask = 0;

        // Establish if their first run differs in the number of iterations
        // expected from the remaining runs
        // This should only be the case for HW filters
        for (filtIdx = 0; filtIdx < pPipe->nFilters; filtIdx++)
        {
            // Should have a better list with only the HW filters in it to save time
            ptrFilt = pPipe->filters[filtIdx];

            if (ptrFilt->unit <= SIPP_MAX_FILTER_ID)
            {
                u32 oBufIdx;

                for (oBufIdx = 0; oBufIdx < ptrFilt->numOBufs; oBufIdx++)
                {
                    if (ptrFilt->oBufs[oBufIdx]->hwInputBufId & firstIbflMask)
                    {
                        u32 firstIters = ptrFilt->firstIterLines;

                        while (firstIters)
                        {
                            SIPP_MLPI_CQ_ADD_WRITE(pQu, SIPP_IBFL_INC_ADR, ptrFilt->oBufs[oBufIdx]->hwInputBufId);
                            firstIters--;
                        }

                        manageFirstMask |= ptrFilt->oBufs[oBufIdx]->hwInputBufId;
                    }
                }
            }
        }

        // Mark that these filters have now been started at least once
        pPipe->firstIbflUseMask  |= ibflIncMask;

        // Update startMask for this iter so that only filters not handled here are handled in remainder of function
        ibflIncMask &= (~(manageFirstMask));
    }

    if (pPipe->ibflIncDelta)
    {
        u32 deltaIbflIncMask = ibflIncMask & ~(pPipe->ibflIncDelta);

        if (deltaIbflIncMask)
        {
            while (numIters)
            {
                SIPP_MLPI_CQ_ADD_WRITE(pQu, SIPP_IBFL_INC_ADR, deltaIbflIncMask);
                numIters--;
            }
        }

        deltaIbflIncMask   = ibflIncMask & pPipe->ibflIncDelta;

        if (deltaIbflIncMask)
        {
            u32 filtIdx;

            for (filtIdx = 0; filtIdx < pPipe->nFilters; filtIdx++)
            {
                SippFilter * ptrFilt;
                u32          oBufIdx;

                // Could have a better list with only the relevant filters in it to save time
                ptrFilt = pPipe->filters[filtIdx];

                for (oBufIdx = 0; oBufIdx < ptrFilt->numOBufs; oBufIdx++)
                {
                    if (ptrFilt->oBufs[oBufIdx]->hwInputBufId & deltaIbflIncMask)
                    {
                        u32 filtNumIters = ptrFilt->linesPerIter;

                        while (filtNumIters)
                        {
                            SIPP_MLPI_CQ_ADD_WRITE(pQu, SIPP_IBFL_INC_ADR, ptrFilt->oBufs[oBufIdx]->hwInputBufId);
                            filtNumIters--;
                        }
                    }
                }
            }
        }
    }
    else
    {
        while (numIters)
        {
            SIPP_MLPI_CQ_ADD_WRITE(pQu, SIPP_IBFL_INC_ADR, ibflIncMask);
            numIters--;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIRunUpdateCQ                                              //
//                                                                                //
//  DESCRIPTION: Uses the values collcted in a software command queue by          //
//               sippMLPIUpdateHWUnits2x5xCQ to update input buffer fill for HW   //
//               filters connected to CMX buffers whose filter levels incremented //
//               on last iteration                                                //
//                                                                                //
//  INPUTS:      pPipe - pipeline to ppdate                                       //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippMLPIRunUpdateCQ (SippPipeline * pPipe)
{
    psSippCMDQu pQu = &pPipe->tCMDUpdateQu;
    u32         writeIdx;

    if (pQu->quNum > pQu->quSize)
    {
        sippPalTrace (SIPP_RUNTIME_TL_CATASTROPHE, "MLPI CQ Iter error @ iter %lu\n", pPipe->iteration);
    }

    for (writeIdx = 0; writeIdx < pQu->quNum; writeIdx++)
    {
        SET_REG_WORD(pQu->quEntry[writeIdx].addr, pQu->quEntry[writeIdx].value);
    }

    // Reset the number of qu entries
    pQu->quNum = 0;

}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIRuntimeHWProcessIters                                    //
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

void sippMLPIRuntimeHWProcessIters (pSippPipeline pPipe,
                                    u32           numIters)
{
    u32 i;
    #ifdef SIPP_RUNTIME_PROFILING
    u8 collectStats = (pPipe->pRTStats) ? 0x1 : 0x0;
    #endif

    for (i = 0; i < numIters; i++, pPipe->iteration++)
    {
        #ifdef RT_DBG
        sippPalTrace (SIPP_RUNTIME_TL_INFO, "========== %5d ==========\n", pPipe->iteration);
        #endif

        /////////////////////////////////////////////////
        // Start current iteration
        #ifndef SIPP_USE_CMXDMA_DRIVER
        if (pPipe->flags & PLF_ENABLE_SW_QU_USE)
        {
            sippMLPIRunStartCQ (pPipe);
        }
        #endif

        sippMLPIStartUnits (pPipe);

        #ifdef SIPP_RUNTIME_PROFILING
        if (collectStats) DrvTimerGetSystemTicks64 (&pPipe->pRTStats->iterTime[(pPipe->iteration * SIPP_RT_PER_ITER_STATS_SIZE) + SIPP_ITER_STAT_STARTED_POS]);
        #endif

        /////////////////////////////////////////////////
        // Setup for next line in background
        sippMLPILinePrepare (pPipe, pPipe->iteration + 1);

        #ifndef SIPP_USE_CMXDMA_DRIVER
        if (pPipe->flags & PLF_ENABLE_SW_QU_USE)
        {
            sippMLPIStartHWUnits2x5xCQ (pPipe, pPipe->iteration + 1);
            sippMLPIUpdateHWUnits2x5xCQ (pPipe);
        }
        #endif

        /////////////////////////////////////////////////
        // Wait unit completion
        #ifdef SIPP_PC
        sippMLPIPCWaitUnits (pPipe);
        #else
        sippMLPIWaitUnits (pPipe);
        #endif

        #ifdef SIPP_RUNTIME_PROFILING
        if (collectStats) DrvTimerGetSystemTicks64 (&pPipe->pRTStats->iterTime[(pPipe->iteration * SIPP_RT_PER_ITER_STATS_SIZE) + SIPP_ITER_STAT_COMPLETE_POS]);
        #endif

        /////////////////////////////////////////////////
        // Update any HW units with new input
        #ifndef SIPP_USE_CMXDMA_DRIVER
        if (pPipe->flags & PLF_ENABLE_SW_QU_USE)
        {
            sippMLPIRunUpdateCQ (pPipe);
        }
        else
        #endif
        {
            sippMLPIUpdateHWUnits2x5x (pPipe);
        }

        // Print something to indicate progress in VCS
        VCS_PRINT_INT(0xABCD0000 + pPipe->iteration);
    }
}


#ifdef SIPP_PC

//////////////////////////////////////////////////////////////////////////////////
// Sipp c-model Runtime section
//

extern void sippMLPIRunFirstIterHwFilt (SippPipeline * pPipe);

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIPCWaitUnits                                              //
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

void sippMLPIPCWaitUnits (pSippPipeline pPipe)
{
    u32 waitMask = pPipe->schedInfo[pPipe->iteration].sippHwWaitMask;

    if (waitMask)
    {
        u32 status;
        sippPalTrace (SIPP_RUNTIME_TL_INFO, "sippMLPIPCWaitUnits : waitMask %08x\n", waitMask);

        status = GET_REG_WORD_VAL(SIPP_INT1_STATUS_ADR);

        if ((status & waitMask) != waitMask)
        {
            sippPalTrace (SIPP_RUNTIME_TL_INFO, "sippMLPIPCWaitUnits : HW Filters did not complete\n");
            sippError (pPipe->errorStatus, E_PC_RUNTIME_FAILURE);
        }
        else
        {
            #ifdef RT_DBG
            sippPalTrace (SIPP_RUNTIME_TL_INFO, "sippMLPIPCWaitUnits : HW Filters complete\n");
            #endif

            //Clear status bits
            SET_REG_WORD (SIPP_INT1_CLEAR_ADR, waitMask);
        }
    }

    sippMLPIRunFirstIterHwFilt (pPipe);

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
    //Final step
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    sippMLPIUpdateExecNums (pPipe);
}

#endif /* SIPP_PC */

#endif // SIPP_SCRT_ENABLE_MLPI

// End of file
