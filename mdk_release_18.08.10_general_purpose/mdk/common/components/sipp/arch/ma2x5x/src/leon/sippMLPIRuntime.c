///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Multiple Liner Per Iteration runtime
///

#ifdef SIPP_SCRT_ENABLE_MLPI

/////////////////////////////////////////////////////////////////////////////////
//  Header Files
/////////////////////////////////////////////////////////////////////////////////

#include <sipp.h>
#include <sippInternal.h>

/////////////////////////////////////////////////////////////////////////////////
//  Global Variables
/////////////////////////////////////////////////////////////////////////////////
#ifdef BUGZILLA_22709
#ifdef SIPP_NO_IRQ_BARRIER2
extern pSIPP_HW_SESSION pgSippHW;
#endif
#endif
#ifdef SIPP_SW_TIME_EXACT
extern u64 SwStartTick[3000];
extern u64 SwStopTick[3000];
#endif

extern tMLPIStartCQCtrl tsMLPIStartCQCtrl[SIPP_MAX_SUPPORTED_HW_PIPELINES][0x2];
extern tSippFramework   gSippFramework;
extern u32              sippGlobalOBFLIncStatus;

/////////////////////////////////////////////////////////////////////////////////
//  Function Prototypes
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//  Code
/////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIDbRecord                                                 //
//                                                                                //
//  DESCRIPTION: Record Input and Output line pointers for current filter         //
//               in the double-buffer                                             //
//                                                                                //
//  INPUTS:      fptr - pipeline to start                                         //
//               linesPerIter - number of lines of info to record for             //
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

// Don't need to call this for lots of HW filters???
// Surely only need to call if (fptr->flags & SIPP_REQ_SW_VIEW)

#ifdef RT_DBG
static inline void sippMLPIDbRecord (SippFilter * fptr,
                                     u8           printVals)
#else
static inline void sippMLPIDbRecord (SippFilter * fptr)
#endif
{
    u32 i, iterLineNum;

    for (i = 0; i < fptr->numIBufs; i++)
    {
        s32 parentBufIdx = fptr->parentOBufIdx[i];

        if (fptr->KSIterList[i])
        {
            for (iterLineNum = 0; iterLineNum < fptr->linesThisIter[fptr->schNo & 0x1]; iterLineNum++)
            {
                if (fptr->parentsKS[i] != SIPP_INVALID_PARENT_KS)
                {
                    fptr->dbLinesIn[i][fptr->schNo & 1][iterLineNum] = &((u32*)fptr->parents[i]->linePtrs[parentBufIdx])[fptr->KSIterList[i][(fptr->schNo << fptr->linesPerIterShift) + iterLineNum]];

                    #ifdef RT_DBG
                    if (printVals)
                    {
                        sippPalTrace (SIPP_RUNTIME_TL_DEBUG, "dbLinesIn[%lu][%lu] = %08lx\n", i, iterLineNum, *fptr->dbLinesIn[i][fptr->schNo & 0x1][iterLineNum]);
                    }
                    #endif
                }
            }
        }
        else
        {
            for (iterLineNum = 0; iterLineNum < fptr->linesThisIter[fptr->schNo & 0x1]; iterLineNum++)
            {
                if (fptr->parentsKS[i] != SIPP_INVALID_PARENT_KS)
                {
                    fptr->dbLinesIn[i][fptr->schNo & 0x1][iterLineNum] = &((u32*)fptr->parents[i]->linePtrs[parentBufIdx])[fptr->parentsKS[i] + iterLineNum];

                    #ifdef RT_DBG
                    if (printVals)
                    {
                        sippPalTrace (SIPP_RUNTIME_TL_DEBUG, "dbLinesIn[%lu][%lu] = %08lx\n", i, iterLineNum, *fptr->dbLinesIn[i][fptr->schNo & 0x1][iterLineNum]);
                    }
                    #endif
                }
            }
        }
    }

    // Record in current job the OUTPUT ptr:
    for (i = 0; i < fptr->numOBufs; i++)
    {
        if (fptr->nLines[i])
        {
            for (iterLineNum = 0; iterLineNum < fptr->linesThisIter[fptr->schNo & 0x1]; iterLineNum++)
            {
                fptr->dbLineOut[i][fptr->schNo & 0x1][iterLineNum] = fptr->outLinePtr[i][iterLineNum];
                #ifdef RT_DBG
                if (printVals)
                {
                    sippPalTrace (SIPP_RUNTIME_TL_DEBUG, "dbLinesOut[%lu][%lu] = %08lx\n", i, iterLineNum, fptr->dbLineOut[i][fptr->schNo & 0x1][iterLineNum]);
                }
                #endif
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPILinePrepare                                              //
//                                                                                //
//  DESCRIPTION: Main Line setup routine                                          //
//                                                                                //
//  INPUTS:      pPipe - pipeline to start                                        //
//               iteration - the active runtime iteration                         //
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

void sippMLPILinePrepare (SippPipeline * pPipe,
                          int            iteration)
{
    SippFilter * fptr;
    u32          i, o;
    u32          lines2Roll;
    u32          oldHwWaitMask = (iteration == 0x0) ? 0x0 : pPipe->schedInfo[iteration - 0x1].sippHwWaitMask;

    if ((u32)iteration >= pPipe->nIter)
        return;

    // Record previous schedule decision
    pPipe->oldRunMask = pPipe->canRunMask;

    // Read schedule decision (for current iteration)
    pPipe->canRunMask = pPipe->schedInfo[iteration].allMask;

    //=============================================================================
    // Output Buffer progress for runnable tasks:
    // i.e. all filters that finised in Previous iteration will get a roll now:
    //=============================================================================
    for (i = 0; i < pPipe->nFilters; i++)
    {
        fptr = pPipe->filters[i];

        // Roll output buffer only for filters that have output buffers (or consumers)
        // TBD: unify this... somewhere I use nCons, in other places nLines etc...
        #ifdef __sparc
        if (fptr->flags & SIPP_REQ_SW_VIEW)
        #endif
        {
            u8 rollOutputBuffer = 0;

            // Is this a HW filter
            if (fptr->unit <= SIPP_MAX_FILTER_ID)
            {
                if (((0x1 << fptr->unit) & oldHwWaitMask) &&
                    (fptr->nCons))
                {
                    rollOutputBuffer        = 1;
                    lines2Roll              = fptr->firstRunRollLines ? fptr->firstRunRollLines : fptr->linesPerIter;
                    fptr->firstRunRollLines = 0;
                }
                else if (fptr->firstRunRollLines == 0)
                {
                    // We need to keep rolling beyond the end of the picture based on allMask
                    if ((pPipe->oldRunMask & (1 << i)) && (fptr->nCons))
                    {
                        rollOutputBuffer = 1;
                        lines2Roll       = fptr->linesPerIter;
                    }
                }
            }
            else if ((pPipe->oldRunMask & (1 << i)) && (fptr->nCons)) // OPT: could use a mask here to optimize
            {
                rollOutputBuffer = 1;
                lines2Roll       = fptr->linesPerIter;
            }

            if (rollOutputBuffer)
            {
                for (o = 0; o < fptr->numOBufs; o++)
                {
                    if (fptr->nLines[o])
                    {
                        u32 runNum     = fptr->schNo;
                        u32 outLineNum = runNum << fptr->linesPerIterShift;
                        u32 lineIterNum;

                        if ((outLineNum + lines2Roll - fptr->outLineDeficit) <= fptr->outputH)
                        {
                            for (lineIterNum = 0; lineIterNum < lines2Roll; lineIterNum++, outLineNum++)
                            {
                                #ifdef SIPP_F_DUMPS
                                // alu: get ptr to prev line, move to next one and compute Consumers pointers
                                fptr->outLinePtrPrev[o][lineIterNum] = fptr->outLinePtr[o][lineIterNum]; // this is for CAPTURE feature
                                #endif

                                fptr->lnToPad[o][runNum & 1][lineIterNum] = fptr->outLinePtr[o][lineIterNum];

                                fptr->linePtrs[o] += sizeof(u8*);
                                if (fptr->linePtrs[o] == fptr->linePtrs3rdBase[o])
                                {
                                    fptr->linePtrs[o]  = fptr->linePtrs2ndBase[o];
                                }

                                // New line that gets into buffer is the new output line for current scheduled iter
                                fptr->outLinePtr[o][lineIterNum] = (u8*)(*(((u32 *)fptr->linePtrs[o]) + fptr->outLineOffset[o]));
                            }
                        }
                        else
                        {
                            if (fptr->linePtrs[o] < fptr->linePtrs4thBase[o])
                            {
                            	fptr->linePtrs[o] = fptr->linePtrs4thBase[o];

                                for (lineIterNum = 0; lineIterNum < lines2Roll; lineIterNum++, outLineNum++)
                                {
                                    #ifdef SIPP_F_DUMPS
                                    // alu: get ptr to prev line, move to next one and compute Consumers pointers
                                    fptr->outLinePtrPrev[o][lineIterNum] = fptr->outLinePtr[o][lineIterNum]; // this is for CAPTURE feature
                                    #endif

                                    fptr->lnToPad[o][runNum & 1][lineIterNum] = fptr->outLinePtr[o][lineIterNum];

                                    // New line that gets into buffer is the new output line for current scheduled iter
                                    fptr->outLinePtr[o][lineIterNum] = (u8*)(*(((u32 *)fptr->linePtrs[o]) + (fptr->nLines[o] - (lines2Roll + fptr->outLineDeficit) + lineIterNum)));
                                }
                            }
                            else
                            {
                                for (lineIterNum = 0; lineIterNum < lines2Roll; lineIterNum++, outLineNum++)
                                {
                                    #ifdef SIPP_F_DUMPS
                                    // alu: get ptr to prev line, move to next one and compute Consumers pointers
                                    fptr->outLinePtrPrev[o][lineIterNum] = fptr->outLinePtr[o][lineIterNum]; // this is for CAPTURE feature
                                    #endif

                                    fptr->lnToPad[o][runNum & 1][lineIterNum] = fptr->outLinePtr[o][lineIterNum];

                                    if (fptr->linePtrs[o] < fptr->linePtrs5thBase[o])
                                    {
                                        fptr->linePtrs[o] += sizeof(u8*);
                                    }

                                    // New line that gets into buffer is the new output line for current scheduled iter
                                    fptr->outLinePtr[o][lineIterNum] = (u8*)(*(((u32*)fptr->linePtrs[o]) + (fptr->nLines[o]-1)));
                                }
                            }
                        }
                    }
                }
            }
        }

        if (fptr->ptrChunkPos)
        {
            fptr->ptrChunkPos->YPos[fptr->schNo & 1] = (fptr->schNo << fptr->linesPerIterShift);
        }

    } // loop through all filters

    //=============================================================================
    // Once all pointers are advanced, we can sample the pointers and compute line params
    // for this scheduled run; it needs to be done AFTER buffers advancements,
    // otherwise it depends on order in which buffers get updated ?
    //=============================================================================
    for (i = 0; i < pPipe->nFilters; i++)
    {
        fptr = pPipe->filters[i]; // ptr to i-th filter

        if (fptr->unit <= SIPP_MAX_FILTER_ID)
        {
            if (pPipe->schedInfo[iteration].sippHwWaitMask & (0x1 << fptr->unit))
            {
                #ifdef SIPP_PC
                if ((fptr->schNo << fptr->linesPerIterShift) < fptr->outputH)
                {
                    // Record the I/O pointers
                    #if RT_DBG
                    sippMLPIDbRecord (fptr, (u8)(fptr->schNo == 0x0 ? 0x1 : 0x0));
                    #else
                    sippMLPIDbRecord (fptr);
                    #endif
                }
                #endif

                fptr->schNo++;      // Decision at this point to minimise changes needed to assembler is to retain schedule number as the number of times
                                    // a unit is scheduled to produce output
                                    // This is irrespective of how many lines it will produce each time it is scheduled
            }
        }
        else if (pPipe->canRunMask & (1 << i))
        {
            u32 startLineNo = fptr->schNo << fptr->linesPerIterShift;

            if (startLineNo < fptr->outputH)
            {
                u32   linesThisIter = fptr->outputH - startLineNo;
                bool  ksAdjust      = 0;
                u32 * ksAdjustAddrArr[SIPP_FILTER_MAX_IBUFS];
                u32   ksAdjustValArr[SIPP_FILTER_MAX_IBUFS];

                if (linesThisIter > fptr->linesPerIter)
                {
                    fptr->linesThisIter[fptr->schNo & 0x1] = fptr->linesPerIter;
                }
                else if (fptr->outputH % fptr->linesPerIter)
                {
                    u32 locIBufIdx;
                    fptr->linesThisIter[fptr->schNo & 0x1] = linesThisIter;

                    // Establish if the producer is at a finer granularity
                    // Increment KS by the delta (the producer buffer may not have sufficiently rolled - it may be at a finer granularity
                    // of LPI - it may never be at a coarser granularity - see rule 3 of LPI setting algo)

                    // If the filter's output height is an integer multiple of the filter's lines per iter
                    // This is NOT required
                    for (locIBufIdx = 0; locIBufIdx < fptr->numIBufs; locIBufIdx++)
                    {
                        if (fptr->parents[locIBufIdx]->linesPerIter < fptr->linesPerIter)
                        {
                            // If parent has insufficiently rolled, we need to modify the kernel start
                            // to compensate
                            // But - will it always be just one roll out - or could it sometimes be zero or two?
                            ksAdjustAddrArr[ksAdjust] = &fptr->parentsKS[0];
                            ksAdjustValArr[ksAdjust]  = fptr->parentsKS[0];
                            ksAdjust++;
                            fptr->parentsKS[0] += fptr->parents[locIBufIdx]->linesPerIter;
                        }
                    }
                }

                // Record the I/O pointers
                #if RT_DBG
                sippMLPIDbRecord (fptr, (u8)(fptr->schNo == 0x0 ? 0x1 : 0x0));
                #else
                sippMLPIDbRecord (fptr);
                #endif

                if (ksAdjust)
                {
                    // Undo this adjustment for any future frames
                    u32 adjustIdx;

                    for (adjustIdx = 0; adjustIdx < ksAdjust; adjustIdx++)
                    {
                        *ksAdjustAddrArr[adjustIdx] = ksAdjustValArr[adjustIdx];
                    }
                }
            }

            fptr->schNo++;          // Decision at this point to minimise changes needed to assembler is to retain schedule number as the number of times
                                    // a unit is scheduled to produce output
                                    // This is irrespective of how many lines it will produce each time it is scheduled
        }
    }

    //=============================================================================
    //Debug: show execution mask
    //=============================================================================
    #ifdef SIPP_PC
    if (pPipe->dbgLevel > 0)
    {
        sippDbgDumpRunMask (pPipe->canRunMask, iteration, 0);
    }
    #endif

    #ifndef SIPP_USE_CMXDMA_DRIVER
    if (pPipe->flags & PLF_ENABLE_SW_QU_USE)
    {
        sippKickDmaCQ (pPipe,
                       iteration);
    }
    #endif
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIRuntimeFrameReset                                        //
//                                                                                //
//  DESCRIPTION: Initialise all pipe and filter struct members for start of a     //
//               new frame                                                        //
//                                                                                //
//  INPUTS:      pPipe - pipeline                                                 //
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

void sippMLPIRuntimeFrameReset (SippPipeline * pPipe)
{
    SippFilter * fptr;
    u32          i, o, iterNum;

    // Reset stuff at the beginning of a frame
    for (i = 0; i < pPipe->nFilters; i++)
    {
        fptr = pPipe->filters[i];

        // Reset pointer sliding window
        for (o = 0; o < fptr->numOBufs; o++)
        {
            fptr->linePtrs[o] = (u8*)fptr->linePtrs1stBase[o];
        }

        // Clear counters
        fptr->exeNo = 0;
        fptr->schNo = 0;

        // Filters that map on HW units:
        if (fptr->iBuf[0]) fptr->iBuf[0]->ctx = 0;
        if (fptr->iBuf[1]) fptr->iBuf[1]->ctx = 0;
        if (fptr->oBuf[0]) fptr->oBuf[0]->ctx = 0;
        if (fptr->oBuf[1]) fptr->oBuf[1]->ctx = 0;

        #if defined(__sparc)
        if (fptr->flags & SIPP_REQ_SW_VIEW)
        #endif
        {

            for (o = 0; o < fptr->numOBufs; o++)
            {
                if (fptr->nLines[o])
                {
                    for (iterNum = 0; iterNum < fptr->linesPerIter; iterNum++)
                    {
                        fptr->outLinePtr[o][iterNum]      = (u8*)(*(((u32*)fptr->linePtrs[o]) + (fptr->nLines[o] - fptr->linesPerIter + iterNum)));
                        #ifdef SIPP_F_DUMPS
                        fptr->outLinePtrPrev[o][iterNum]  = fptr->outLinePtr[o][iterNum]; // Initially, set to valid addr
                        #endif
                        fptr->lnToPad[o][0][iterNum]      = fptr->outLinePtr[o][iterNum]; // Initially, set to valid addr
                        fptr->lnToPad[o][1][iterNum]      = fptr->outLinePtr[o][iterNum]; // Initially, set to valid addr
                    }
                }
            }
        }
    }

    // Build & open dump file names:
    #ifdef SIPP_F_DUMPS
    sippDbgCreateDumpFiles (pPipe);
    #endif

}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIStartUnits                                               //
//                                                                                //
//  DESCRIPTION: Starts Shaves, DMAs and SIPP-HW-units that are used in           //
//               Single-Context                                                   //
//                                                                                //
//  INPUTS:      pPipe - pipeline                                                 //
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

void sippMLPIStartUnits (SippPipeline * pPipe)
{
    #if defined(SIPP_PC) || defined(MYRIAD2)

    ////////////////////////////////////////////////////////
    // 1) Quick start of Shaves.
    //   Important note: Shaves need to run to do H-Padding, even if
    //                   no SW filters run in initial iterations
    if ((pPipe->schedInfo[pPipe->iteration].shaveMask) ||
        (pPipe->flags & PLF_REQUIRES_SW_PADDING))
    {
        #ifdef MYRIAD2

        pPipe->svuSyncSem = pPipe->gi.numShaves;   // Rearm semaphore

        #ifdef SIPP_SW_TIME_EXACT
        DrvTimerGetSystemTicks64 (&SwStartTick[pPipe->iteration]);
        #endif

        // Get Alternate: blocks shaves to get into N+1 iteration
        SET_REG_WORD(pPipe->svuSyncMtxAddr, pPipe->svuSyncMtx[1-pPipe->svuSyncMtxParity] | MTX_REQ_ON_RETRY);
        // Free main, this triggers shaves for current Iteration
        SET_REG_WORD(pPipe->svuSyncMtxAddr, pPipe->svuSyncMtx[  pPipe->svuSyncMtxParity] | MTX_RELEASE);
        // Toggle parity for next run !
        pPipe->svuSyncMtxParity = 1 - pPipe->svuSyncMtxParity;

        VCS_PRINT_INT(0x55550000 | pPipe->iteration);

        #ifdef SIPP_SW_TIME_EXACT
        while (swcLeonReadNoCacheU32 (((u32)&pPipe->svuSyncSem) | 0x08000000))
        {
            NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;
            NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;
        }
        DrvTimerGetSystemTicks64 (&SwStopTick[pPipe->iteration]);
        #endif
        #else

        // PC sim
        sippKickShaveM1PC (pPipe);

        #endif
    }

    ////////////////////////////////////////////////////////
    // 2) Quick start of all HW Sipp

    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    #ifndef SIPP_USE_CMXDMA_DRIVER
    if (!(pPipe->flags & PLF_ENABLE_SW_QU_USE))
    {
        sippMLPIStartHWUnits2x5x (pPipe);
    }
    #else
    sippMLPIStartHWUnits2x5x (pPipe);
    #endif

    #ifdef RT_DBG
    SIPP_PIPE_LOG((0xAA000000 | pPipe->iteration),pPipe);
    #endif

    #endif

    ////////////////////////////////////////////////////////
    // 3) DMA is very unlikely to be the bottleneck, is likely to finish
    //    very quickly, so start it last
    // In a concurrent pipe scenario - this may not be the case!
    if (pPipe->schedInfo[pPipe->iteration].dmaMask)
    {
        #ifdef SIPP_USE_CMXDMA_DRIVER
        sippDrvKickDma (pPipe);
        #else
        if (pPipe->flags & PLF_ENABLE_SW_QU_USE)
        {
            sippRunDmaCQ (pPipe);
        }
        else
        {
            sippKickDma (pPipe);
        }
        #endif
    }

    #endif

    ////////////////////////////////////////////////////////
    // 4) Update next Exec Num values in ALTERNATE variables
    //    We cannot update the exeNo directly as all numbers need to be stable during iteration
    //    Shaves samples own or parent "exeNo" to figure out pointer set
    {
        SippFilter ** filters    = pPipe->filters;
        u32         * nxtExeNo   = pPipe->nxtExeNo;
        u32           allMask    = pPipe->schedInfo[pPipe->iteration].allMask;
        u32           hwWaitMask = pPipe->schedInfo[pPipe->iteration].sippHwWaitMask;
        u32           i;

        for (i = 0; i < pPipe->nFilters; i++)
        {
            nxtExeNo[i] = filters[i]->exeNo;

            // This is based on producing output as opposed to just running
            if (filters[i]->unit <= SIPP_MAX_FILTER_ID)
            {
                if ((0x1 << filters[i]->unit) & hwWaitMask)
                    nxtExeNo[i]++;
            }
            else if (allMask & (1 << i))
            {
                nxtExeNo[i]++;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIUpdateExecNums                                           //
//                                                                                //
//  DESCRIPTION: Update filter execution iteration number                         //
//                                                                                //
//  INPUTS:      pl - pipeline                                                    //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       Copy updated run-numbers over current values                     //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippMLPIUpdateExecNums (SippPipeline * pl)
{
    SippFilter ** filters  = pl->filters;
    u32         * nxtExeNo = pl->nxtExeNo;
    u32           j;

    for (j = 0; j < pl->nFilters; j++)
    {
        filters[j]->exeNo = nxtExeNo[j];
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIRunFirstIterHwFilt                                       //
//                                                                                //
//  DESCRIPTION: Handle non regular number of output line requirements for        //
//               HW filters on their first run iteration                          //
//                                                                                //
//  INPUTS:      pPipe - Pipeline                                                 //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       In multi lines per iteration scenarios, the first time a filter  //
//               runs, should it have an inherent latency as many of the HW       //
//               filters do - especially when connected in an OSE - then the      //
//               number of lines produced on the first iteration may be less than //
//               the number of lines per iteration specified for the filter.      //
//               If this first iteration number of lines is not a power of 2,     //
//               then it cannot be handled in one operation of the HW (the irq    //
//               rate register only accepts powers of two). So the HW filters     //
//               needing this assistance are run further times, each for a power  //
//               of 2 number of lines until the lines desired to be produced on   //
//               the first iteration are complete                                 //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippMLPIRunFirstIterHwFilt (SippPipeline * pPipe)
{
    u32 oBufIdx;
    u32 filtIdx;
    #ifdef MYRIAD2
    u32 status, waitMask;
    #endif

    #ifndef SIPP_USE_CMXDMA_DRIVER
    if (pPipe->flags & PLF_ENABLE_SW_QU_USE)
    {
        ptMLPIStartCQCtrl pMLPIStartCQCtrl = &tsMLPIStartCQCtrl[pPipe->HWPipeID][pPipe->iteration & 0x1];

        pPipe->firstUseInProg = pMLPIStartCQCtrl->firstUseInProg;

        if (pPipe->firstUseInProg)
        {
            pPipe->nFirstRunFilts = pMLPIStartCQCtrl->nFirstRunFilts;

            for (filtIdx = 0; filtIdx < pPipe->nFirstRunFilts; filtIdx++)
            {
                pPipe->firstRunFilts[filtIdx] = pMLPIStartCQCtrl->firstRunFilts[filtIdx];
            }

            pMLPIStartCQCtrl->firstUseInProg = 0;
        }
    }
    #endif

    // Do we have any other multi iter work to do?
    while (pPipe->firstUseInProg)
    {
        #ifdef MYRIAD2
        waitMask = 0;
        #endif

        // Set up the filter for a lesser number of lines to cause the interrupt
        for (filtIdx = 0; filtIdx < pPipe->nFirstRunFilts; filtIdx++)
        {
            SippFilter * ptrFilt = pPipe->firstRunFilts[filtIdx];

            u32 runLines = sippPalFindHighestBit (ptrFilt->firstRunNumLines);

            if (runLines != 0xFFFFFFFF)
            {
                for (oBufIdx = 0; oBufIdx < ptrFilt->numOBufs; oBufIdx++)
                {
                    u32 localRunLines = (0x1 << runLines);
                    u32 bufferID      = sippPalFindHighestBit (ptrFilt->oBufs[oBufIdx]->hwOutputBufId);

                    if (bufferID != 0xFFFFFFFF)
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

                    #ifdef MYRIAD2
                    waitMask |= ptrFilt->oBufs[oBufIdx]->hwOutputBufId;
                    #endif
                }

                ptrFilt->firstRunNumLines &= (~(0x1 << runLines));
            }
            else
            {
                u32 bufRemoveMask;

                for (oBufIdx = 0x0, bufRemoveMask = 0x0; oBufIdx < ptrFilt->numOBufs; oBufIdx++)
                {
                    u32 bufferID   = sippPalFindHighestBit (ptrFilt->oBufs[oBufIdx]->hwOutputBufId);
                    bufRemoveMask |= ptrFilt->oBufs[oBufIdx]->hwOutputBufId;

                    if (bufferID != 0xFFFFFFFF)
                    {
                        // Reset the irq rate for the buffer
                        sippBufSetupIrqRate (ptrFilt->oBuf[oBufIdx],
                                             0x1,
                                             bufferID,
                                             ptrFilt->linesPerIter);
                    }
                }

                pPipe->firstUseInProg &= (~(bufRemoveMask));
            }
        }

        #ifndef SIPP_PC

        if (waitMask)
        {
            // If running in async mode, ensure we don't actually trigger an irq
            if (pPipe->useSyncRuntime == 0)
            {
                #ifndef NO_INTERRUPT_BARRIERS_MODE
                SET_REG_WORD((SIPP_INT1_BARRIER0_ADR + (pPipe->HWPipeID << 0x2)), 0xFFFFFFFF);
                #endif
            }

            // Wait till status bit gets set
            do
            {
                // Typically, filters are done when obfl_inc asserts
                // this applies to most filters (i.e. that have associated output buffer)
                status = GET_REG_WORD_VAL(SIPP_INT1_STATUS_ADR);

                NOP;NOP;NOP;NOP;NOP;

            } while ((status & waitMask) != waitMask);

            // Clear status bits
            SET_REG_WORD(SIPP_INT1_CLEAR_ADR, waitMask);
        }
        #endif
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIWaitUnits                                                //
//                                                                                //
//  DESCRIPTION: Used in synchronous modes to wait on completion of all units     //
//               for an iteration                                                 //
//                                                                                //
//  INPUTS:      pPipe - Pipeline                                                 //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       Can wait for DMAs, Shave and SIPP-HW                             //
//               Fast units are checked first, so that when slowest units are     //
//               done, we just exit.                                              //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippMLPIWaitUnits (SippPipeline * pPipe)
{
    #ifdef MYRIAD2
    u32 status, waitMask;
    #ifdef BUGZILLA_22709
    u32 waitEoFMask;
    #endif
    #endif
    #ifdef SIPP_RUNTIME_PROFILING
    u32 DMAWaitCycles   = 0;
    u32 HWWaitCycles    = 0;
    u32 ShaveWaitCycles = 0;
    #endif

    #if defined(SIPP_F_DUMPS)
    ////////////////////////////////////////////////////////
    // On PC all models are blocking, so can do dumps here
    // no need to wait() for units to execute
    if (pPipe->dbgLevel > 0)
    {
        sippDbgDumpFilterOuts (pPipe);
    }
    #endif

    ////////////////////////////////////////////////////////
    // Wait for DMA
    #ifndef SIPP_DMA_TIME_EXACT
    if (pPipe->schedInfo[pPipe->iteration].dmaMask)
    {
        #ifdef SIPP_RUNTIME_PROFILING
        DMAWaitCycles = sippWaitDma ();
        #else
        sippWaitDma ();
        #endif
    }

    #endif

    ////////////////////////////////////////////////////////
    // Wait for HW
    #ifdef MYRIAD2
    // If barrier is nonzero, then we need to wait for something.
    waitMask     = pPipe->schedInfo[pPipe->iteration].sippHwWaitMask;

    #ifdef BUGZILLA_22709
    waitEoFMask  = pPipe->schedInfo[pPipe->iteration].sippHwWaitEoFMask;
    waitMask    &= (~waitEoFMask);
    #endif

    if (waitMask)
    {
        status = GET_REG_WORD_VAL(SIPP_INT1_STATUS_ADR);
        // If we got MIPI-TX filters in pipeline, their "line-done" event is IBFL_DEC assertion
        // (as MIPI-TX filters don't have an output buffer associated)
        if (waitMask & (SIPP_MIPI_TX0_ID_MASK|SIPP_MIPI_TX1_ID_MASK))
        {
            status |= GET_REG_WORD_VAL(SIPP_INT0_STATUS_ADR) & (SIPP_MIPI_TX0_ID_MASK|SIPP_MIPI_TX1_ID_MASK);
        }

        // Wait till status bit gets set
        while ((status & waitMask) != waitMask)
        {
            // Typically, filters are done when obfl_inc asserts
            // this applies to most filters (i.e. that have associated output buffer)
            status = GET_REG_WORD_VAL(SIPP_INT1_STATUS_ADR);

            // If we got MIPI-TX filters in pipeline, their "line-done" event is IBFL_DEC assertion
            // (as MIPI-TX filters don't have an output buffer associated)
            if (waitMask & (SIPP_MIPI_TX0_ID_MASK|SIPP_MIPI_TX1_ID_MASK))
            {
                status |= GET_REG_WORD_VAL(SIPP_INT0_STATUS_ADR) & (SIPP_MIPI_TX0_ID_MASK|SIPP_MIPI_TX1_ID_MASK);
            }

            NOP;NOP;NOP;NOP;NOP;

            #ifdef SIPP_RUNTIME_PROFILING
            HWWaitCycles++;
            #endif

        }

        // Clear status bits
        SET_REG_WORD(SIPP_INT1_CLEAR_ADR, waitMask);

        // Clear ibfl as well if got MIPI-TX filters !
        if (waitMask & (SIPP_MIPI_TX0_ID_MASK|SIPP_MIPI_TX1_ID_MASK))
        {
            SET_REG_WORD(SIPP_INT0_CLEAR_ADR, waitMask);
        }
    }

    sippMLPIRunFirstIterHwFilt (pPipe);

    #ifdef BUGZILLA_22709
    if (waitEoFMask)
    {
        // Wait till status bit gets set
        do
        {
            // Check End of Frame interrupt
            status = GET_REG_WORD_VAL(SIPP_INT2_STATUS_ADR);

            NOP;NOP;NOP;NOP;NOP;

        } while ((status & waitEoFMask) != waitEoFMask);

        // Clear status bits
        SET_REG_WORD(SIPP_INT2_CLEAR_ADR, waitEoFMask);
        SET_REG_WORD(SIPP_INT1_CLEAR_ADR, waitEoFMask);
    }

    #endif // BUGZILLA_22709
    #endif // MYRIAD2

    //////////////////////////////////////////////////////////
    // Wait for Shave
    #ifndef SIPP_SW_TIME_EXACT
    if ((pPipe->schedInfo[pPipe->iteration].shaveMask) ||
        (pPipe->flags & PLF_REQUIRES_SW_PADDING))
    {
        #if defined(__sparc)
        #if defined(MYRIAD2)
        // swcLeonReadNoCacheU32 bypasses Leon-L1 data cache
        // | 0x08000000          bypasses Leon-L2 data cache

        #ifndef POWER_MEASURE_NO_SHAVES
        while (swcLeonReadNoCacheU32 (((u32)&pPipe->svuSyncSem) | 0x08000000))
        {
            NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;
            NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;
            #ifdef SIPP_RUNTIME_PROFILING
            ShaveWaitCycles++;
            #endif
        }

        #endif
        #endif
        #endif
    }
    #endif

    #ifdef SIPP_RUNTIME_PROFILING
    if (pPipe->pRTStats)
    {
        pPipe->pRTStats->DMAWaitCycles   += DMAWaitCycles;
        pPipe->pRTStats->HWWaitCycles    += HWWaitCycles;
        pPipe->pRTStats->ShaveWaitCycles += ShaveWaitCycles;

        pPipe->pRTStats->iterTime[(pPipe->iteration * SIPP_RT_PER_ITER_STATS_SIZE) + SIPP_ITER_STAT_CDMA_CYCLE_POS] = DMAWaitCycles;
        pPipe->pRTStats->iterTime[(pPipe->iteration * SIPP_RT_PER_ITER_STATS_SIZE) + SIPP_ITER_STAT_HW_CYCLE_POS]   = HWWaitCycles;
        pPipe->pRTStats->iterTime[(pPipe->iteration * SIPP_RT_PER_ITER_STATS_SIZE) + SIPP_ITER_STAT_SW_CYCLE_POS]   = ShaveWaitCycles;
    }
    #endif

    /////////////////////////////////////////////////////////
    // Final step: this is Leon-End-Of-Iteration-Overhead
    sippMLPIUpdateExecNums (pPipe);
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIRuntimeLoadPipeline                                      //
//                                                                                //
//  DESCRIPTION: Load the HW with the settings for the current pipe               //
//                                                                                //
//  INPUTS:      pPipe - Pipeline                                                 //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       Sometimes it's needed to alter params mapped in shave local      //
//               slices. E.g. :                                                   //
//               * update common params, they'll get loaded on HW units in next   //
//                 step                                                           //
//               * [load pipeline] (shave sections, HW blocks)                    //
//               * update slice-params: can only do this after pipe is loaded     //
//                 would need to update each individual copy of those params for  //
//                 each Shave                                                     //
//               * [process pipeline]                                             //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippMLPIRuntimeLoadPipeline (SippPipeline * pPipe)
{
    u32 regVal, newVals, buffIdx, filtIdx;
    #ifdef BUGZILLA_22709
    u32 newVals2 = 0;
    #endif
    SIPP_PAL_CRIT_STATE uCS;

    // Clear the interrupt barrier
    if (pPipe->useSyncRuntime == 0)
    {
        SET_REG_WORD((SIPP_INT1_BARRIER0_ADR + ((pPipe->HWPipeID)<< 0x2)), 0);
        #ifdef BUGZILLA_22709
        #ifdef SIPP_NO_IRQ_BARRIER2
        pgSippHW->sippHWEOFMask[pPipe->HWPipeID] = 0;
        #else
        SET_REG_WORD((SIPP_INT2_BARRIER0_ADR + ((pPipe->HWPipeID)<< 0x2)), 0);
        #endif
        #endif
    }

    // Loop thru HW filters
    for (filtIdx = 0, newVals = 0; filtIdx <  pPipe->nFilters; filtIdx++)
    {
        SippFilter * ptrFilt = pPipe->filters[filtIdx];

        if (ptrFilt->unit <= SIPP_MAX_FILTER_ID)
        {
            for (buffIdx = 0; buffIdx < ptrFilt->numOBufs; buffIdx++)
            {
                newVals |= gSipp.uInfo[ptrFilt->unit].hwGetObufIds (ptrFilt, buffIdx);
                #ifdef BUGZILLA_22709
                if (ptrFilt->nLines[buffIdx])
                {
                    if (ptrFilt->bug22709Active)
                    {
                        newVals2 |= gSipp.uInfo[ptrFilt->unit].hwGetObufIds (ptrFilt, buffIdx);
                    }
                }
                #endif
            }
        }
    }

    sippPalCriticalSectionBegin (&uCS);

    sippGlobalOBFLIncStatus &= ~newVals;

    SET_REG_WORD(SIPP_INT1_CLEAR_ADR, newVals);
    #ifdef BUGZILLA_22709
    SET_REG_WORD(SIPP_INT2_CLEAR_ADR, newVals2);
    #endif

    regVal = GET_REG_WORD_VAL(SIPP_INT1_ENABLE_ADR);
    SET_REG_WORD(SIPP_INT1_ENABLE_ADR, (regVal | newVals));

    sippPalCriticalSectionEnd (uCS);

    #ifdef BUGZILLA_22709
    newVals2 &= ((0x1 << SIPP_MAX_FILTER_ID) - 0x1);

    // ***ALERT - RMW of this register should be protected ***/
    regVal = GET_REG_WORD_VAL(SIPP_INT2_ENABLE_ADR);
    SET_REG_WORD(SIPP_INT2_ENABLE_ADR, (regVal | newVals2));
    #endif

}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIRuntimeProcessIters                                      //
//                                                                                //
//  DESCRIPTION: Servicing function for a process Iters command from the          //
//               scheduler                                                        //
//                                                                                //
//  INPUTS:      pPipe - Pipeline                                                 //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippMLPIRuntimeProcessIters (SippPipeline * pPipe,
                                  u32            numIters)
{

    sippMLPIRuntimeLoadPipeline (pPipe);

    if (pPipe->iteration == 0)
    {
        pPipe->canRunMask = 0;
        sippMLPILinePrepare (pPipe, 0/*iteration*/);

        #ifndef SIPP_USE_CMXDMA_DRIVER
        if (pPipe->flags & PLF_ENABLE_SW_QU_USE)
        {
            sippMLPIStartHWUnits2x5xCQ (pPipe, 0/*iteration*/);
        }
        #endif
    }

    // Inform client processing is to begin (useful for perf measurements)
    sippEventNotify (pPipe,
                     eSIPP_PIPELINE_STARTED,
                     (SIPP_PIPELINE_EVENT_DATA *)NULL);

    #ifdef SIPP_RUNTIME_PROFILING
    if (pPipe->pRTStats)
    {
        pPipe->pRTStats->DMAWaitCycles   = 0;
        pPipe->pRTStats->HWWaitCycles    = 0;
        pPipe->pRTStats->ShaveWaitCycles = 0;
    }
    #endif

    if (pPipe->useSyncRuntime)
    {
        //////////////////////////////////////////////////////
        //Figure out how many iterations are left:
        if (pPipe->iteration + numIters > pPipe->nIter)
        {
            pPipe->itersLeft = pPipe->nIter - pPipe->iteration;
        }
        else
        {
            pPipe->itersLeft = numIters;
        }

        //////////////////////////////////////////////////////
        // The MAIN iteration loop - this tends to be quite HW
        // specific so will be implemented in
        // the HW specific files
        sippMLPIRuntimeHWProcessIters (pPipe,
                                       pPipe->itersLeft);


        //////////////////////////////////////////////////////
        // End of Frame ?
        if (pPipe->iteration == pPipe->nIter)
        {
            pPipe->gi.curFrame++;
            pPipe->iteration = 0;

            // Do we go ahead and issue a done event in sync mode? At the moment I will not
            // but we do need to post an update status event
            sippEventNotify (pPipe,
                             eSIPP_PIPELINE_SYNC_OP_DONE,
                             (SIPP_PIPELINE_EVENT_DATA *)0x0);

            #if defined(MYRIAD2)
            // Cmxdma finishes last transactions and says DONE, but the WRITES didn't yet reach
            // the DDR memory array (still on busses ...)
            // So before dumping DDR content for checks, need to wait "a bit"
            // extern void sippWait (u32 numx100);
            // sippWait (20);
            #endif
        }

    }
    else
    {
        SIPP_PAL_CRIT_STATE CS;
        /* Using the async runtime */
        pPipe->endIter = pPipe->iteration + numIters; // should this be -1?

        sippPalCriticalSectionBegin (&CS);
        // Use critical section protection here to ensure this function
        // is completed in thread context before it is called in ISR
        sippMLPIRunNextIter (pPipe);

        sippPalCriticalSectionEnd (CS);

    }
}

///////////////////////////////////////////////////////////////////////
// Async runtime servicing functions
//

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIRunIterDone                                              //
//                                                                                //
//  DESCRIPTION: Called in asynchronous execution to mark the end of an iteration //
//               and check if the run is complete                                 //
//                                                                                //
//  INPUTS:      pPipe - Pipeline                                                 //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippMLPIRunIterDone (SippPipeline * pPipe)
{
    u32 retVal = 0;

    pPipe->isrFlags = 0;

    #if defined(SIPP_F_DUMPS)
    if (pPipe->dbgLevel > 0)
    {
        sippDbgDumpFilterOuts (pPipe);
    }
    #endif

    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    #ifndef SIPP_USE_CMXDMA_DRIVER
    if (pPipe->flags & PLF_ENABLE_SW_QU_USE)
    {
        sippMLPIRunUpdateCQ (pPipe);
    }
    else
    {
        sippMLPIUpdateHWUnits2x5x (pPipe);
    }
    #else
    sippMLPIUpdateHWUnits2x5x (pPipe);
    #endif

    #endif

    sippMLPIUpdateExecNums (pPipe);

    // Print something to indicate progress in VCS
    VCS_PRINT_INT(0xABCD0000 + pPipe->iteration);
    pPipe->iteration++;

    // Check end of run conditions
    if (pPipe->iteration == pPipe->nIter)
    {
        // TODO - Check if below is still needed...
        #if defined(MYRIAD2)
        // Cmxdma finishes last transactions and says DONE, but the WRITES didn't yet reach
        // the DDR memory array (still on busses ...)
        // So before dumping DDR content for checks, need to wait "a bit"
        extern void sippWait (u32 numx100);
        sippWait (20);
        #endif

        #ifdef RT_DBG
        SIPP_PIPE_LOG((0xDD000000 | pPipe->iteration),pPipe);
        #endif

        // End of frame callback
        // Could return some stats as the data
        sippEventNotify (pPipe,
                         eSIPP_PIPELINE_FRAME_DONE,
                         (SIPP_PIPELINE_EVENT_DATA *)0);

        // Remove from the loaded list
        sippHWSessionRemoveLoadedPipe (pPipe);

        retVal = 1;
    }
    else if (pPipe->iteration == pPipe->endIter)
    {
        // TODO - Check if below is still needed...
        #if defined(MYRIAD2)
        // Cmxdma finishes last transactions and says DONE, but the WRITES didn't yet reach
        // the DDR memory array (still on busses ...)
        // So before dumping DDR content for checks, need to wait "a bit"
        extern void sippWait (u32 numx100);
        sippWait (20);
        #endif

        // Could return some stats as the data
        sippEventNotify (pPipe,
                         eSIPP_PIPELINE_ITERS_DONE,
                         (SIPP_PIPELINE_EVENT_DATA *)0);
        retVal = 1;
    }

    return retVal;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIRunNextIter                                              //
//                                                                                //
//  DESCRIPTION: Called in asynchronous execution to kick off the next iteration  //
//                                                                                //
//  INPUTS:      pPipe - Pipeline                                                 //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

#ifdef SIPP_PC
#include "wrapperSem.h"
extern Semaphore * cmxDmaSyncSem;
#endif

void sippMLPIRunNextIter (pSippPipeline pPipe)
{
    u32 uLocalIteration = pPipe->iteration;
    UNUSED (uLocalIteration); // hush the compiler warning

    // Print something to indicate progress in VCS
    VCS_PRINT_INT(0xABCD0000 + pPipe->iteration);

    sippMLPIRunFirstIterHwFilt (pPipe);

    #ifdef RT_DBG
    sippPalTrace (SIPP_RUNTIME_TL_INFO, "========== %5d ==========\n", pPipe->iteration);
    #endif

    #ifdef SIPP_RUNTIME_PROFILING
    if (pPipe->pRTStats)
    {
        DrvTimerGetSystemTicks64 (&pPipe->pRTStats->iterTime[(pPipe->iteration * SIPP_RT_PER_ITER_STATS_SIZE) + SIPP_ITER_STAT_ASYNC_START_POS]);
    }
    #endif

    // Start current iteration
    #ifndef SIPP_USE_CMXDMA_DRIVER
    if (pPipe->flags & PLF_ENABLE_SW_QU_USE)
    {
        sippMLPIRunStartCQ (pPipe);
    }
    #endif

    sippMLPIStartUnits (pPipe);

    // Setup for next line in background
    sippMLPILinePrepare (pPipe, pPipe->iteration + 1);

    #ifdef SIPP_RUNTIME_PROFILING
    if (pPipe->pRTStats)
    {
        DrvTimerGetSystemTicks64 (&pPipe->pRTStats->iterTime[(pPipe->iteration * SIPP_RT_PER_ITER_STATS_SIZE) + SIPP_ITER_STAT_ASYNC_FW_POS]);
    }
    #endif

    #ifndef SIPP_USE_CMXDMA_DRIVER
    if (pPipe->flags & PLF_ENABLE_SW_QU_USE)
    {
        sippMLPIStartHWUnits2x5xCQ (pPipe, pPipe->iteration + 1);
        sippMLPIUpdateHWUnits2x5xCQ (pPipe);
    }
    #endif

    #ifdef SIPP_PC
    if (pPipe->schedInfo[pPipe->iteration].dmaMask)
    {
        cmxDmaSyncSem->Post ();
    }
    #endif

    VCS_PRINT_INT(0xDCBA0000 + uLocalIteration);

}


///////////////////////////////////////////////////////////////////////
// Public API
//
// The following functions are the common hook functions to be implemented by all
// compatible runtimes called by the session controller

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIRuntimeClaimHWResource                                   //
//                                                                                //
//  DESCRIPTION: Setup resources required from outside the SIPP HW (for exmaple)  //
//               the interrupts                                                   //
//                                                                                //
//  INPUTS:      pPipe - Pipeline                                                 //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippMLPIRuntimeClaimHWResource (pSippPipeline pPipe)
{
    #ifndef SIPP_PC
    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    // Sync calls run in isolation - we can safely disable the interrupts therefore
    // assuming SIPP is only client of this HW
    if (swcWhoAmI() == PROCESS_LEON_OS)
    {
        if (pPipe->useSyncRuntime)
        {
            #if !defined(MULTI_SIPP_DRIVERS)
            // Shouldn't touch unused resources
            DrvIcbDisableIrq (gSippFramework.dynIrqSipp1);
            SET_REG_DWORD(CDMA_INT_EN_ADR,
                          GET_REG_WORD_VAL(CDMA_INT_EN_ADR) & (~(1 << SIPP_CDMA_INT_NO)));
            #ifdef BUGZILLA_22709
            DrvIcbDisableIrq (gSippFramework.dynIrqSipp2);
            #endif
            #endif
        }
        else
        {
            DrvIcbEnableIrq (gSippFramework.dynIrqSipp1);
            SET_REG_DWORD(CDMA_INT_EN_ADR,
                          GET_REG_WORD_VAL(CDMA_INT_EN_ADR) | (1 << SIPP_CDMA_INT_NO));
            #ifdef BUGZILLA_22709
            DrvIcbEnableIrq (gSippFramework.dynIrqSipp2);
            #endif
        }
    }
    else
    {
        if (pPipe->useSyncRuntime)
        {
            #if !defined(MULTI_SIPP_DRIVERS)
            // Shouldn't touch unused resources
            DrvIcbDisableIrq (IRQ_SIPP_1);
            SET_REG_DWORD(CDMA_INT_EN_ADR,
                          GET_REG_WORD_VAL(CDMA_INT_EN_ADR) & (~(1 << SIPP_CDMA_INT_NO)));
            #ifdef BUGZILLA_22709
            DrvIcbDisableIrq (IRQ_SIPP_2);
            #endif
            #endif
        }
        else
        {
            DrvIcbEnableIrq (IRQ_SIPP_1);
            SET_REG_DWORD(CDMA_INT_EN_ADR,
                          GET_REG_WORD_VAL(CDMA_INT_EN_ADR) | (1 << SIPP_CDMA_INT_NO));
            #ifdef BUGZILLA_22709
            DrvIcbEnableIrq (IRQ_SIPP_2);
            #endif
        }
    }
    #endif
    #endif

}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIRuntime                                                  //
//                                                                                //
//  DESCRIPTION: Top level event handler for the MLPI runtime                     //
//                                                                                //
//  INPUTS:      pPipe - Pipeline                                                 //
//               eEvent - event to handle                                         //
//               pData  - pointer to any data associated with the event           //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippMLPIRuntime (pSippPipeline                    pPipe,
                      eSIPP_ACCESS_SCHEDULER_EVENT     eEvent,
                      SIPP_ACCESS_SCHEDULER_EVENT_DATA pData)
{
    switch (eEvent)
    {
        case eSIPP_ACCESS_SCHEDULER_CMD_PROCESS_ITERS :
        {
            // Copy over the number of iterations
            u32 uNumIters = *(u32 *)pData;

            sippMLPIRuntimeProcessIters (pPipe,
                                         uNumIters);

        } break;
        default : break;

    }
}

#endif /* SIPP_SCRT_ENABLE_MLPI */

// End of file
