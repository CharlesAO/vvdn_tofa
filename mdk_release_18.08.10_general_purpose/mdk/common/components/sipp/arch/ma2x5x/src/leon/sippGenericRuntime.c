///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

/////////////////////////////////////////////////////////////////////////////////
//  Header Files
/////////////////////////////////////////////////////////////////////////////////

#include <sipp.h>
#include <sippInternal.h>

/////////////////////////////////////////////////////////////////////////////////
//  Global Variables
/////////////////////////////////////////////////////////////////////////////////
#ifdef SIPP_SW_TIME_EXACT
u64 SwStartTick[3000];
u64 SwStopTick[3000];
#endif

extern tSippFramework gSippFramework;
extern u32            sippGlobalOBFLIncStatus;

/////////////////////////////////////////////////////////////////////////////////
//  Function Prototypes
/////////////////////////////////////////////////////////////////////////////////
extern void sippGenericStartHWUnits2x5x (SippPipeline * pPipe);

//####################################################################################
//Record Input and Output line pointers for current filter in the double-buffer
//####################################################################################

#ifdef RT_DBG
static inline void sippGenericDbRecord (SippFilter *fptr, u8 printVals)
#else
static inline void sippGenericDbRecord (SippFilter *fptr)
#endif
{
    u32 i;

    // Record in current job the INPUT pointers
    // Rem : If the parent buf is not allocated - i.e. this is an Opipe connection
    // parentBufIdx will be -1 but this function should not be called in that
    // situation
    for (i = 0; i < fptr->numIBufs; i++)
    {
        s32 parentBufIdx = fptr->parentOBufIdx[i];

        if (fptr->parentsKS[i] != SIPP_INVALID_PARENT_KS)
        {
            fptr->dbLinesIn[i][fptr->schNo&1][0] = &((u32*)fptr->parents[i]->linePtrs[parentBufIdx])[fptr->parentsKS[i]];
        }
    }

    //Record in current job the OUTPUT ptr:
    for (i = 0; i < fptr->numOBufs; i++)
    {
        if(fptr->nLines[i])
        {
            fptr->dbLineOut[i][fptr->schNo&1][0] = fptr->outLinePtr[i][0];
        }
    }

    #ifdef RT_DBG
    if (printVals)
    {
        for (i = 0; i < fptr->nParents; i++)
        {
            if (fptr->parentsKS[i] != SIPP_INVALID_PARENT_KS)
            {
                sippPalTrace(SIPP_RUNTIME_TL_DEBUG, "dbLinesIn[%d] = %08x\n", i, *fptr->dbLinesIn[i][fptr->schNo&1][0]);
            }
        }
        for (i = 0; i < fptr->numOBufs; i++)
        {
            if(fptr->nLines[i])
            {
                sippPalTrace(SIPP_RUNTIME_TL_DEBUG, "dbLinesOut = %08x\n", fptr->dbLineOut[i][fptr->schNo&1][0]);
            }
        }
    }
    #endif

}

//####################################################################################
// Main Line setup routine
//####################################################################################
void sippGenericLinePrepare (SippPipeline * pl,
                             int            iteration)
{
    SippFilter * fptr;
    u32          i, o;
    u32          oldHwWaitMask = (iteration == 0x0) ? 0x0 : pl->schedInfo[iteration - 0x1].sippHwWaitMask;

    if ((u32)iteration >= pl->nIter)
        return;

    //Record previous schedule decision
    pl->oldRunMask = pl->canRunMask;

    // Read schedule decision (for current iteration)
    pl->canRunMask = pl->schedInfo[iteration].allMask;

    //=============================================================================
    // Output Buffer progress for runnable tasks:
    // i.e. all filters that finised in Previous iteration will get a roll now:
    //=============================================================================
    for (i = 0; i < pl->nFilters; i++)
    {
        fptr = pl->filters[i];

        //Roll output buffer only for filters that have output buffers (or consumers)
        //TBD: unify this... somewhere I use nCons, in other places nLines etc...
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
                    fptr->firstRunRollLines = 0;
                }
                else if (fptr->firstRunRollLines == 0)
                {
                    // We need to keep rolling beyond the end of the picture based on allMask
                    if ((pl->oldRunMask & (1 << i)) && (fptr->nCons))
                    {
                        rollOutputBuffer = 1;
                    }
                }
            }
            else if ((pl->oldRunMask & (1 << i)) && (fptr->nCons)) // OPT: could use a mask here to optimize
            {
                rollOutputBuffer = 1;
            }

            if (rollOutputBuffer)
            {
                for (o = 0; o < fptr->numOBufs; o++)
                {
                    if (fptr->nLines[o])
                    {
                        u32 outLineNum = fptr->schNo;

                        #ifdef SIPP_PC
                        // alu: get ptr to prev line, move to next one and compute Consumers pointers
                        fptr->outLinePtrPrev[o][0] = fptr->outLinePtr[o][0];//this is for CAPTURE feature
                        #endif

                        fptr->lnToPad[o][outLineNum & 1][0] = fptr->outLinePtr[o][0];

                        // Top Replication and typical operation
                        if (outLineNum < fptr->outputH)
                        {
                            fptr->linePtrs[o] += sizeof(u8*); // stupid pointer stuff... moving through at table of pointers with a U8 ptr.
                            if (fptr->linePtrs[o] == fptr->linePtrs3rdBase[o])
                            {
                                fptr->linePtrs[o]  = fptr->linePtrs2ndBase[o];
                            }
                        }

                        // Bottom replication
                        else if ((outLineNum == fptr->outputH) && (fptr->linePtrs[o] < fptr->linePtrs4thBase[o]))
                        {
                            // Bottom replication init
                            fptr->linePtrs[o] = fptr->linePtrs4thBase[o];
                        }
                        else
                        {
                            if(fptr->linePtrs[o] < fptr->linePtrs5thBase[o])
                            {
                                fptr->linePtrs[o] += sizeof(u8*);
                            }
                        }

                        // New line that gets into buffer is the new output line for current scheduled iter
                        fptr->outLinePtr[o][0] = (u8*)(*(((u32*)fptr->linePtrs[o]) + (fptr->nLines[o]-1)));
                    }
                }
            }
        }

        if (fptr->ptrChunkPos)
        {
            fptr->ptrChunkPos->YPos[fptr->schNo & 1] = fptr->schNo;
        }

    } // Loop through all filters


    //=============================================================================
    // Once all pointers are advanced, we can sample the pointers and compute line params
    // for this scheduled run; it needs to be done AFTER buffers advancements,
    // otherwise it depends on order in which buffers get updated ?
    //=============================================================================
    for (i = 0; i < pl->nFilters; i++)
    {
        fptr = pl->filters[i]; //ptr to i-th filter

        if (fptr->unit <= SIPP_MAX_FILTER_ID)
        {
            if (pl->schedInfo[iteration].sippHwWaitMask & (0x1 << fptr->unit))
            {
                #ifdef SIPP_PC
                if (fptr->schNo < fptr->outputH)
                {
                    // Record the I/O pointers
                    #if RT_DBG
                    sippGenericDbRecord (fptr,(u8)(fptr->schNo == 0x0 ? 0x1 : 0x0));
                    #else
                    sippGenericDbRecord (fptr);
                    #endif
                }
                #endif

                fptr->schNo++;
            }
        }
        else if (pl->canRunMask & (1 << i))
        {
            if (fptr->schNo < fptr->outputH)
            {
                // Record the I/O pointers
                #if RT_DBG
                sippGenericDbRecord (fptr,(u8)(fptr->schNo == 0x0 ? 0x1 : 0x0));
                #else
                sippGenericDbRecord (fptr);
                #endif
            }

            fptr->schNo++;
        }
    }

    //=============================================================================
    //Debug: show execution mask
    //=============================================================================
    #ifdef SIPP_PC
    if (pl->dbgLevel > 0)
    {
        sippDbgDumpRunMask(pl->canRunMask, iteration, 0);
    }
    #endif

    #ifndef SIPP_USE_CMXDMA_DRIVER
    if (pl->flags & PLF_ENABLE_SW_QU_USE)
    {
        sippKickDmaCQ (pl,
                       iteration);
    }
    #endif
}

//####################################################################################
// - init  "line pointes" for all buffers
// - reset counters....
//####################################################################################

void sippGenericRuntimeFrameReset (SippPipeline * pl)
{
    SippFilter * fptr;
    u32          i, o;

    // Reset stuff at the beginning of a frame
    for (i = 0; i < pl->nFilters; i++)
    {
        fptr                    = pl->filters[i];
        fptr->firstRunRollLines = 0x1;

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
                    fptr->outLinePtr[o][0]      = fptr->linePtrs1stBase[o][0];
                    #ifdef SIPP_F_DUMPS
                    fptr->outLinePtrPrev[o][0]  = fptr->outLinePtr[o][0]; // Initially, set to valid addr
                    #endif
                    fptr->lnToPad[o][0][0]      = fptr->outLinePtr[o][0]; // Initially, set to valid addr
                    fptr->lnToPad[o][1][0]      = fptr->outLinePtr[o][0]; // Initially, set to valid addr
                }
            }
        }
    }

    // Build & open dump file names:
    #ifdef SIPP_F_DUMPS
    sippDbgCreateDumpFiles(pl);
    #endif

}


//###########################################################################
//Starts Shaves, DMAs and SIPP-HW-units that are used Single-Context !
//###########################################################################
// Logic of H - padding: iteration N + 1 can run if all input buffers
// for iteration N are completed. That will be the WAIT.
// At which point , I set H - padding on these filters to be used ?

void sippGenericStartUnits (SippPipeline * pl)
{
    #if defined(SIPP_PC) || defined(MYRIAD2)

    ////////////////////////////////////////////////////////
    // 1) Quick start of all HW Sipp

    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)

    sippGenericStartHWUnits2x5x (pl);

    #ifdef RT_DBG
    SIPP_PIPE_LOG((0xAA000000 | pl->iteration),pl);
    #endif

    #endif
    #endif

    ////////////////////////////////////////////////////////
    // 2) Quick start of Shaves.
    //   Important note: Shaves need to run to do H-Padding, even if
    //                   no SW filters run in initial iterations
    if ((pl->schedInfo[pl->iteration].shaveMask) ||
        (pl->flags & PLF_REQUIRES_SW_PADDING))
    {
        #ifdef MYRIAD2

        pl->svuSyncSem = pl->gi.numShaves;   // Rearm semaphore

        #ifdef SIPP_SW_TIME_EXACT
        DrvTimerGetSystemTicks64(&SwStartTick[pl->iteration]);
        #endif

        //Get Alternate: blocks shaves to get into N+1 iteration
        SET_REG_WORD(pl->svuSyncMtxAddr, pl->svuSyncMtx[1-pl->svuSyncMtxParity] | MTX_REQ_ON_RETRY);
        //Free main, this triggers shaves for current Iteration
        SET_REG_WORD(pl->svuSyncMtxAddr, pl->svuSyncMtx[  pl->svuSyncMtxParity] | MTX_RELEASE);
        //Toggle parity for next run !
        pl->svuSyncMtxParity = 1 - pl->svuSyncMtxParity;

        VCS_PRINT_INT(0x55550000 | pl->iteration);

        #ifdef SIPP_SW_TIME_EXACT
        while (swcLeonReadNoCacheU32 (((u32)&pl->svuSyncSem) | 0x08000000))
        {
            NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;
            NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;
        }
        DrvTimerGetSystemTicks64(&SwStopTick[pl->iteration]);
        #endif
        #else

        // PC sim
        sippKickShaveM1PC (pl);

        #endif
    }

    ////////////////////////////////////////////////////////
    // 3) DMA is very unlikely to be the bottleneck, is likely to finish
    //    very quickly, so start it last
    // In a concurrent pipe scenario - this may not be the case!
    if (pl->schedInfo[pl->iteration].dmaMask)
    {
        #ifdef SIPP_USE_CMXDMA_DRIVER
        sippDrvKickDma (pl);
        #else
        if (pl->flags & PLF_ENABLE_SW_QU_USE)
        {
            sippRunDmaCQ (pl);
        }
        else
        {
            sippKickDma (pl);
        }
        #endif
    }

    ////////////////////////////////////////////////////////
    // 4) Update next Exec Num values in ALTERNATE variables
    //    We cannot update the exeNo directly as all numbers need to be stable during iteration
    //    Shaves samples own or parent "exeNo" to figure out pointer set
    {
        SippFilter ** filters    = pl->filters;
        u32         * nxtExeNo   = pl->nxtExeNo;
        u32           allMask    = pl->schedInfo[pl->iteration].allMask;
        u32           hwWaitMask = pl->schedInfo[pl->iteration].sippHwWaitMask;
        u32           i;

        for (i = 0; i <pl->nFilters; i++)
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

//###########################################################################
// Copy updated run-numbers over current values!
void sippGenericUpdateExecNums ( SippPipeline * pl)
{
    SippFilter ** filters = pl->filters;
    u32         * nxtExeNo = pl->nxtExeNo;
    u32           j;

    for (j = 0; j < pl->nFilters; j++)
        filters[j]->exeNo = nxtExeNo[j];
}

//###########################################################################
//Wait for current iteration to finish !
//Can wait for DMAs, Shave or SIPP-HW(M2 only)
//Fast units are checked first, so that when slowest units are done,
// we just exit.
//###########################################################################

// Used only in synchronous mode

void sippGenericWaitUnits (SippPipeline * pPipe)
{
    #ifdef MYRIAD2
    u32 status, waitMask;
    #endif
    #ifdef SIPP_RUNTIME_PROFILING
    u32 DMAWaitCycles   = 0;
    u32 HWWaitCycles    = 0;
    u32 ShaveWaitCycles = 0;
    #endif

    #if defined(SIPP_F_DUMPS)
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Step 0: on PC, all models are blocking, so can do dumps here.
    // no need to wait() for units to execute
    if (pPipe->dbgLevel > 0)
    {
        sippDbgDumpFilterOuts(pPipe);
    }
    #endif

   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   //Step 1: Then, can also wait for DMA
   //OPT: Could just remove the test and CALL the wait anyway,
   //     as if not called, will return immediately, else wait.
   //     In general, DMAs are used in each iteration, so can rem the test
    #ifndef SIPP_DMA_TIME_EXACT
    if(pPipe->schedInfo[pPipe->iteration].dmaMask)
    {
        #ifdef SIPP_RUNTIME_PROFILING
        DMAWaitCycles = sippWaitDma ();
        #else
        sippWaitDma ();
        #endif
    }
    #endif

    #ifdef MYRIAD2
    // If barrier is nonzero, then we got to wait for something.
    waitMask = pPipe->schedInfo[pPipe->iteration].sippHwWaitMask;

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
    #endif

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Step 3: Then, can also wait for Shave
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

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Final step: this is Leon-End-Of-Iteration-Overhead
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    sippGenericUpdateExecNums (pPipe);
}



//####################################################################################
//Sometimes it's needed to alter params mapped in shave local slices.
// E.g. :
// - update common params, they'll get loaded on HW units in next step:
// - [load pipeline] (shave sections, HW blocks)
// - update slice-params: can only do this after pipe is loaded !
//   would need to update each individual copy of those params for each Shave
// - [process pipeline]

// should we call this from this file or directly from the HWSession and therefore set this up as another
// function which all runtimes must implement

void sippGenericRuntimeLoadPipeline (SippPipeline * pPipe)
{
    u32 regVal, newVals, buffIdx, filtIdx;
    SIPP_PAL_CRIT_STATE uCS;

    // Clear the interrupt barrier
    if (pPipe->useSyncRuntime == 0)
    {
        SET_REG_WORD((SIPP_INT1_BARRIER0_ADR + ((pPipe->HWPipeID)<< 0x2)), 0);
    }

    // Loop thru HW filters
    for (filtIdx = 0, newVals = 0; filtIdx <  pPipe->nFilters; filtIdx++)
    {
        SippFilter * ptrFilt = pPipe->filters[filtIdx];

        if (ptrFilt->unit <= SIPP_MAX_FILTER_ID)
        {
            for (buffIdx = 0; buffIdx < ptrFilt->numOBufs; buffIdx++)
            {
                newVals |= gSipp.uInfo[ptrFilt->unit].hwGetObufIds(ptrFilt, buffIdx);
            }
        }
    }

    sippPalCriticalSectionBegin (&uCS);

    sippGlobalOBFLIncStatus &= ~newVals;

    SET_REG_WORD(SIPP_INT1_CLEAR_ADR, newVals);

    regVal = GET_REG_WORD_VAL(SIPP_INT1_ENABLE_ADR);
    SET_REG_WORD(SIPP_INT1_ENABLE_ADR, (regVal | newVals));

    sippPalCriticalSectionEnd (uCS);

}

//####################################################################################
void sippGenericRuntimeProcessIters (SippPipeline * pPipe, u32 numIters)
{
    sippGenericRuntimeLoadPipeline (pPipe);

    if (pPipe->iteration == 0)
    {
        pPipe->canRunMask = 0;
        sippGenericLinePrepare (pPipe, 0/*iteration*/);
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
        // Figure out how many iterations are left:
        if (pPipe->iteration + numIters > pPipe->nIter)
        {
            pPipe->itersLeft = pPipe->nIter - pPipe->iteration;
        }
        else
        {
            pPipe->itersLeft = numIters;
        }

        //////////////////////////////////////////////////////
        // The MAIN iteration loop - this tends to be quite
        // HW specific so will be implemented in the HW specific files
        sippGenericRuntimeHWProcessIters (pPipe,
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
            extern void sippWait (u32 numx100);
            sippWait (20);
            #endif
        }

    }
    else
    {
        // Using the async runtime

        SIPP_PAL_CRIT_STATE CS;
        pPipe->endIter = pPipe->iteration + numIters;

        sippPalCriticalSectionBegin (&CS);

        // Use critical section protection here to ensure this function
        // is completed in thread context before it is called in ISR
        sippGenericRunNextIter (pPipe);

        sippPalCriticalSectionEnd (CS);

    }
}

///////////////////////////////////////////////////////////////////////
// Public API
//
// The following functions are the common hook functions to be implemented by all
// compatible runtimes called by the session controller

void sippGenericRuntimeClaimHWResource (pSippPipeline pPipe)
{
    #ifndef SIPP_PC
    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    // Sync calls run in isolation - we can safely disable the interrupts therefore
    if (swcWhoAmI() == PROCESS_LEON_OS)
    {
        if (pPipe->useSyncRuntime)
        {
            #if !defined(MULTI_SIPP_DRIVERS)
            DrvIcbDisableIrq (gSippFramework.dynIrqSipp1);
            SET_REG_DWORD(CDMA_INT_EN_ADR,
                          GET_REG_WORD_VAL(CDMA_INT_EN_ADR) & (~(1 << SIPP_CDMA_INT_NO)));
            #endif
        }
        else
        {
            DrvIcbEnableIrq (gSippFramework.dynIrqSipp1);
            SET_REG_DWORD(CDMA_INT_EN_ADR,
                          GET_REG_WORD_VAL(CDMA_INT_EN_ADR) | (1 << SIPP_CDMA_INT_NO));
        }
    }
    else
    {
        if (pPipe->useSyncRuntime)
        {
            #if !defined(MULTI_SIPP_DRIVERS)
            DrvIcbDisableIrq (IRQ_SIPP_1);
            SET_REG_DWORD(CDMA_INT_EN_ADR,
                          GET_REG_WORD_VAL(CDMA_INT_EN_ADR) & (~(1 << SIPP_CDMA_INT_NO)));
            #endif
        }
        else
        {
            DrvIcbEnableIrq (IRQ_SIPP_1);
            SET_REG_DWORD(CDMA_INT_EN_ADR,
                          GET_REG_WORD_VAL(CDMA_INT_EN_ADR) | (1 << SIPP_CDMA_INT_NO));
        }
    }
    #endif
    #endif

}


void sippGenericRuntime (pSippPipeline                    pPipe,
                         eSIPP_ACCESS_SCHEDULER_EVENT     eEvent,
                         SIPP_ACCESS_SCHEDULER_EVENT_DATA pData)
{
    switch (eEvent)
    {
        case eSIPP_ACCESS_SCHEDULER_CMD_PROCESS_ITERS :
        {
            // Copy over the number of iterations
            u32 uNumIters = *(u32 *)pData;

            sippGenericRuntimeProcessIters (pPipe,
                                            uNumIters);

        } break;
        default : break;

    }
}



////////////////////////////////////////////
// Async runtime servicing functions

u32 sippGenericRunIterDone (SippPipeline * pPipe)
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
    sippGenericUpdateHWUnits2x5x (pPipe);
    #endif

    sippGenericUpdateExecNums (pPipe);

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


#ifdef SIPP_PC
#include "wrapperSem.h"
extern Semaphore * cmxDmaSyncSem;
#endif

void sippGenericRunNextIter (pSippPipeline pPipe)
{
    u32 uLocalIteration = pPipe->iteration;
    UNUSED (uLocalIteration); // hush the compiler warning

    //Print something to indicate progress in VCS
    VCS_PRINT_INT(0xABCD0000 + pPipe->iteration);

    #ifdef RT_DBG
    sippPalTrace (SIPP_RUNTIME_TL_INFO, "======== %5d ==========\n", pPipe->iteration);
    #endif

    #ifdef SIPP_RUNTIME_PROFILING
    if (pPipe->pRTStats)
    {
        DrvTimerGetSystemTicks64 (&pPipe->pRTStats->iterTime[(pPipe->iteration * SIPP_RT_PER_ITER_STATS_SIZE) + SIPP_ITER_STAT_ASYNC_START_POS]);
    }
    #endif

    // Start current iteration
    sippGenericStartUnits (pPipe);

    // Setup for next line in background
    sippGenericLinePrepare (pPipe, pPipe->iteration + 1);

    #ifdef SIPP_RUNTIME_PROFILING
    if (pPipe->pRTStats)
    {
        DrvTimerGetSystemTicks64 (&pPipe->pRTStats->iterTime[(pPipe->iteration * SIPP_RT_PER_ITER_STATS_SIZE) + SIPP_ITER_STAT_ASYNC_FW_POS]);
    }
    #endif

    #ifdef SIPP_PC
    if (pPipe->schedInfo[pPipe->iteration].dmaMask)
    {
        cmxDmaSyncSem->Post();
    }
    #endif

    VCS_PRINT_INT(0xDCBA0000 + uLocalIteration);

}
