///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///


#include <sipp.h>
#include <sippInternal.h>

////////////////////////////////////////////////////
// Global variables
////////////////////////////////////////////////////

extern int   SVU_SYM(SHAVE_MAIN)(void);

//keep these globals as separate vars to ease debug with moviSim
extern u32   gSippErrCode;
SippFilter * gFakeFptr;       //The fake filter can be used by all pipes
extern u32   gSippSliceSz;
extern u32   gSippInitMask; //1 for Init, 0 to skip initialization

////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippSetSliceSize                                                 //
//                                                                                //
//  DESCRIPTION: Allow the slice size to be modified                              //
//                                                                                //
//  INPUTS:      size - New SIPP slice size                                       //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       When would this be used?? - should it be per pipe?               //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippSetSliceSize (u32 size)
{
    //Add checks !
    gSippSliceSz = size;
}

//####################################################################################
//Call this before sippInitialize to bypass certain initialization steps !
//Mask usage:
//  bit0 : bypasses CMXDMA SW reset (clear-bit to bypass)

// I am assuming this is a real API???
void sippSetInitMask (u32 mask)
{
    gSippInitMask = mask;
}

void sippCoreSetPaddingReqs (pSippPipeline pPipe)
{
    SippFilter * ptrFilt;
    u32          filtIdx, iBufIdx;

    // For every used buffer in the system work it the padding requirements
    // based on the consumer with the largest horizontal kernel size
    for (filtIdx = 0; filtIdx < pPipe->nFilters; filtIdx++)
    {
        ptrFilt = pPipe->filters[filtIdx];

        if ((ptrFilt->unit == SIPP_SVU_ID) || (ptrFilt->unit == SIPP_DMA_ID))
        {
            // For each input buffer
            for (iBufIdx = 0; iBufIdx < ptrFilt->numIBufs; iBufIdx++)
            {
                if ((ptrFilt->iBufCtx[iBufIdx]->hKerSz >> 1) > ptrFilt->iBufs[iBufIdx]->hPadding)
                {
                    ptrFilt->iBufs[iBufIdx]->hPadding = ptrFilt->iBufCtx[iBufIdx]->hKerSz;
                }
            }
        }
    }
}


// By this point there will be data in the new buffer structs which needs to be pulled
// back into the filter structs
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // 1) Copy the number of lines in each buffer to be allocated back to
        //    the filter structure - currently I am doing this for code re-use
        //    purposes - is it architecturally the correct thing to do?
        //    Also copy back the hPadding requirement

void sippProcessSchedData (pSippPipeline pPipe)
{
    u32 filtIdx, o;
    // Scheduling entities are only used within the scheduler. Any info pertinent
    //
    // Populate the filter structs with required info
    //    Set up
    //    1) nLines
    //    2) hPadding  - should do this somewhere else as it is a function of the new output buf structs not the scheduler
    //    3) parentKS
    for (filtIdx = 0; filtIdx < pPipe->nFilters; filtIdx++)
    {
        SippFilter * pFilter = pPipe->filters[filtIdx];

        for (o = 0; o < pFilter->numOBufs; o++)
        {
            pFilter->nLines[o]   = (pFilter->oBufs[o]->allocReq ) ? pFilter->oBufs[o]->numLines : 0;
            pFilter->hPadding[o] = pFilter->oBufs[o]->hPadding;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////////////
// Public functions


//####################################################################################
// - compute frame schedule
// - allocate CMX line buffers
// - compute padding values & helper data members
// Called from the SW scheduler so no touching hardware

eSIPP_STATUS sippCoreFinalisePipeline (pSippPipeline pPipe)
{
    eSIPP_STATUS eRetCode     = eSIPP_STATUS_OK;
    #ifdef SIPP_RUNTIME_PROFILING
    u8           collectStats = (pPipe->pRTStats) ? 0x1 : 0x0;
    u64          schStartTime;
    u64          schStopTime;
    #endif

    if (pPipe->pCmxMap)
    {
        #ifdef SIPP_SCRT_ENABLE_OPIPE
        if (pPipe->pfnSippRuntime == sippOPipeRuntime)
        {
            // Consolidation of any contiguous regions
            // into one single region
            sippMemLBConsolidateRegions (pPipe->pCmxMap);
        }
        else
        #endif
        {
            // If chunking is employed - match up the regions
            u32 numChunks = pPipe->gi.sliceLast - pPipe->gi.sliceFirst + 0x1;

            sippMemLBMatchRegionsToChunks (pPipe->pCmxMap,
                                           pPipe->gi.sliceSize,
                                           numChunks);
        }
    }

    sippCoreSetPaddingReqs (pPipe);

    // Mark mbin as null if got no filters; this helps ctx-switch !
    // TBD - I need to feed something like this into my access scheduling algo
    if (pPipe->nFiltersSvu == 0)
    {
        pPipe->mbinImg = NULL;
    }

    sippComputeSliceLayout (pPipe);

    #if defined(MYRIAD2) || defined(SIPP_PC)
    sippGetCtxOrder (pPipe); // Determine if at least one filter requires context switch
    #endif

    if (!sippUsingPrecompSched(pPipe))
    {
        // Compute the main schedule
        #ifdef SIPP_RUNTIME_PROFILING
        if (collectStats) DrvTimerGetSystemTicks64 (&schStartTime);

        if (pPipe->flags & PLF_PROVIDE_RT_STATS)
        {
            pPipe->pRTStats = (ptRTStats)sippMemAlloc (&pPipe->tHeapMCB,
                                                       vPoolGeneral,
                                                       sizeof(tRTStats));

            // No check for fail here - stats simply won't be collected
            if (pPipe->pRTStats == (ptRTStats)NULL)
            {
                sippPalTrace (SIPP_API_TL_ERROR, "Failed to alloc stats area\n");
            }
        }
        else
        {
            pPipe->pRTStats = (ptRTStats)NULL;
        }

        #endif

        if (pPipe->pfnSippSchedule (pPipe,
                                    true,
                                    false) == 0)
        {

            #ifdef SIPP_RUNTIME_PROFILING
            if (collectStats)
            {
                DrvTimerGetSystemTicks64 (&schStopTime);
                pPipe->pRTStats->ScheduleCycles = (u32)(schStopTime - schStartTime);
            }
            #endif

            sippProcessSchedData (pPipe);

            sippGetFirstHwFiltIdx (pPipe);

            // Dump pipeline graph now that line buffer sizes are known
            #if defined(SIPP_PC)
            sippDbgDumpGraph (pPipe, "pipeline.dot");
            #endif

            #if defined(MYRIAD2) || defined(SIPP_PC)
            if (pPipe->dbgLevel > 0)
            {
                sippDbgDumpSchedForVcsCArr (pPipe);

                // This is done here as the schedule is needed for the
                // offsets within SchedInfo struct which are also saved off
                sippDbgDumpAsmOffsets (pPipe);
            }
            #endif
        }
        else
        {
            sippPalTrace (SIPP_API_TL_ERROR, "SIPP: sippSchedPipeline fails\n");
            return eSIPP_STATUS_INTERNAL_ERROR;
        }
    }
    else
    {
        // If using a precomputed header, we still need to know this:
        sippGetFirstHwFiltIdx (pPipe);

        #ifdef SIPP_SCRT_ENABLE_OPIPE
        if (pPipe->pfnSippRuntime == sippOPipeRuntime)
        {
            if (pPipe->pfnSippSchedule (pPipe, true, false))
            {
                sippPalTrace (SIPP_API_TL_ERROR, "SIPP: sippSchedPipeline fails\n");
                return eSIPP_STATUS_INTERNAL_ERROR;
            }
        }
        #endif
    }

    // Alloc line buffers in CMX, compute padding strides etc...
    #ifdef SIPP_SCRT_ENABLE_OPIPE
    if (pPipe->pfnSippRuntime != sippOPipeRuntime)
    {
    #endif
        if (sippBuildLnBuffs (pPipe) != 0)
        {
            sippPalTrace (SIPP_API_TL_ERROR, "SIPP: sippBuildLnBuffs fails\n");
            sippError (pPipe->errorStatus, E_OUT_OF_MEM );
            return 0x1;
        }
    #ifdef SIPP_SCRT_ENABLE_OPIPE
    }
    else
    {
        if (sippInitLnMemPoolSlices (&pPipe->tHeapMCB,
                                     pPipe->gi.sliceFirst,
                                     pPipe->gi.sliceLast) != 0)
        {
            sippPalTrace (SIPP_API_TL_ERROR, "SIPP: sippInitLnMemPoolSlices fails\n");
            return eSIPP_STATUS_INTERNAL_ERROR;
        }

        if (sippBuildLnBuffsOPipe (pPipe) != 0)
        {
            sippPalTrace (SIPP_API_TL_ERROR, "SIPP: sippBuildLnBuffsOPipe fails\n");
            sippError (pPipe->errorStatus, E_OUT_OF_MEM );
            return 0x1;
        }
    }
    #endif

    // Only load code and set win-regs if at least one SW filter exits
    if (pPipe->nFiltersSvu)
    {
        #if defined(__sparc)
        //If got shave filters, but shave image is NULL => fatal error
        if (pPipe->mbinImg == NULL)
        {
            sippPalTrace (SIPP_API_TL_ERROR, "SIPP: sippBuildLnBuffs fails\n");
            sippError (pPipe->errorStatus, E_MISSING_SHAVE_IMAGE);
            return eSIPP_STATUS_INTERNAL_ERROR;
        }
        #endif
    }

    // Compute hard constants for all HW-SIPP filters, this needs to be done AFTER
    // all line buffers are allocated
    // Pack internal data structures
    if (sippIniHwFilters (pPipe))
    {
        return eSIPP_STATUS_INTERNAL_ERROR;
    }

    #ifdef SIPP_USE_CMXDMA_DRIVER
    if (sippInitDrvDesc (pPipe))  // Allocate and initialize driver transaction descriptors
    {
        return 1;
    }
    #else
    sippChainDmaDesc (pPipe); // Chain DMA descriptors when using direct acces to CDMA
    #endif

    // Skip this part in oPipe SchRT
    #ifdef SIPP_SCRT_ENABLE_OPIPE
    if (pPipe->pfnSippRuntime != sippOPipeRuntime)
    #endif
    {
        sippDumpHtmlMap (pPipe); // DEBUG
        sippComputePaddingOffsets(pPipe);
        sippComputeSwOutCt(pPipe);
        sippAsmOptSetup(pPipe);   //compute additional data structs for Shave optimizations
    }

    #ifndef SIPP_USE_CMXDMA_DRIVER
	if (pPipe->flags & PLF_ENABLE_SW_QU_USE)
    {
        sippDmaCQInit (pPipe); // This is not really related to the schedule so should be allocated out of the cmx virtual pool
        sippCQInit (pPipe); // This is related to the schedule so should be allocated out of the schedule virtual pool
                            // and be reallocated as part of the reschedule process
    }
    #endif

    #if defined(MYRIAD2)
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Differentiate OS/RT Leons here and get MTX access addr
    if (PROCESS_LEON_OS == swcWhoAmI())
    {
        pPipe->svuSyncMtxAddr = CMX_MTX_BASE + CMX_MTX_LOS_GET;
    }
    else
    {
        pPipe->svuSyncMtxAddr = CMX_MTX_BASE + CMX_MTX_LRT_GET;
    }
    #endif

    //====================================================================================
    //storage and dma descriptor for CMX data sections Push-Pop operations in ddr
    //(useful for sippProcessIters in conjunctions with other pipelines)
    #if defined(MYRIAD2) || defined(SIPP_PC)
    // TODO - Below, remove or keep?
    if(pPipe->flags & PLF_RUNS_ITER_GROUPS)
    {
        // Interrogate ln-mempool to see utilization (i.e. request 0 bytes :) - this will not work if
        // this pool has been re-mapped to a heap...)
        u32 curPos = (u32)sippMemAlloc (&pPipe->tHeapMCB,
                                        vPoolFilterLineBuf,
                                        0);
        DmaDesc *dmaDesc;
        //allocate a buffer in DDR to hold: 1)shaves-static-data, 2)stack and 3)dynamic-ln-pool
        //(i.e. data sections that typically lie in slice
        pPipe->ddrCmxBackupLen = curPos - pPipe->svuWinRegs[0];
        pPipe->ddrCmxBackupAdr = (u8*)sippMemAlloc (&pPipe->tHeapMCB,
                                                    vPoolGeneral,
                                                    pPipe->ddrCmxBackupLen * pPipe->gi.numShaves);

        // What to do here? This is a lot of memory to allocate so we do not simply wish to do it by default
        // so when should we decide to do it - should we insist on a flag again which would let us know?

        //-----------------------------------------------------------------------------
        //CMX to DDR descriptor (covers all slices)
        dmaDesc = &pPipe->dmaCmxPush;
        pPipe->pushCmd        =   (0x0LL                       <<41)  //Wrap enable
                                | (((u64)pPipe->gi.sliceLast ) <<37)  //maxSlice
                                | (((u64)pPipe->gi.sliceFirst) <<33)  //minSlice
                                |  (0x1LL                      <<32)  //Start
                                | (u64)((u32)&pPipe->dmaCmxPush ); //Addr of first descriptor

        //Build the descriptor !
        dmaDesc->dscCtrlLinkAddr = (              0x00000000LL << 0)  //Link Addr (NULL terminator)
                                  |(                     0x3LL <<32)  //Transaction type: 2D with chunking
                                  |(                     0x4LL <<36)  //Burst Len
                                  |(                       0LL <<40)  //TransactionID
                                  |(((u64)SIPP_CDMA_INT_NO) <<44)  //Interrupt Sel
                                  |(                       0LL <<59); //skip
        dmaDesc->dscDstSrcAddr   = (((u64)(u32)pPipe->ddrCmxBackupAdr)<<32)
                                  |(pPipe->svuWinRegs[0] + (pPipe->gi.sliceFirst * pPipe->sliceSz));
        dmaDesc->dscPlanesLen    = pPipe->ddrCmxBackupLen * pPipe->gi.numShaves; //1 plane
        dmaDesc->dscSrcStrdWidth = (((u64)pPipe->sliceSz)<<32)
                                   |((u64)pPipe->ddrCmxBackupLen);
        dmaDesc->dscDstStrdWidth = (((u64)pPipe->ddrCmxBackupLen)<<32)
                                   |((u64)pPipe->ddrCmxBackupLen);
        dmaDesc->dscPlStrides    = 0;

        //-----------------------------------------------------------------------------
        //DDR to CMX descriptor (covers all slices)
        dmaDesc = &pPipe->dmaCmxPop;
        pPipe->popCmd         =   (0x0LL                      <<41)  //Wrap enable
                                | (((u64)pPipe->gi.sliceLast ) <<37)  //maxSlice
                                | (((u64)pPipe->gi.sliceFirst) <<33)  //minSlice
                                |  (0x1LL                      <<32)  //Start
                                | (u64)((u32)&pPipe->dmaCmxPop  ); //Addr of first descriptor

        //Build the descriptor !
        dmaDesc->dscCtrlLinkAddr = (             0x00000000LL << 0)  //Link Addr (NULL terminator)
                                 |(                     0x3LL <<32)  //Transaction type: 2D with chunking
                                 |(                     0x4LL <<36)  //Burst Len
                                 |(                       0LL <<40)  //TransactionID
                                 |(((u64)SIPP_CDMA_INT_NO) <<44)  //Interrupt Sel
                                 |(                       0LL <<59); //skip
        dmaDesc->dscDstSrcAddr   = ((u64)(pPipe->svuWinRegs[0] + (pPipe->gi.sliceFirst * pPipe->sliceSz)))<<32
                                  |((u64)((u32)pPipe->ddrCmxBackupAdr));
        dmaDesc->dscPlanesLen    = pPipe->ddrCmxBackupLen * pPipe->gi.numShaves; //1 plane
        dmaDesc->dscSrcStrdWidth = (((u64)pPipe->ddrCmxBackupLen)<<32)
                                  |((u64)pPipe->ddrCmxBackupLen);
        dmaDesc->dscDstStrdWidth = (((u64)pPipe->sliceSz)<<32)
                                  |((u64)pPipe->ddrCmxBackupLen);
        dmaDesc->dscPlStrides    = 0;
    }
    #endif

    pPipe->flags |= PLF_IS_FINALIZED;

    // Need a callback to ensure client knows this is complete
    sippEventNotify (pPipe,
                     eSIPP_PIPELINE_FINALISED,
                     (SIPP_PIPELINE_EVENT_DATA *)NULL);

    return eRetCode;
}


eSIPP_STATUS sippCoreResourceInit()
{
    sippInitPhysicalPoolGlobal ();
    sippMemInitVirtPhysMaps ();

    return eSIPP_STATUS_OK;
}

void sippCoreReInitLineBuffers (pSippPipeline pPipe)
{
    u32          filtIdx, idx;
    u32          bufIdx;
    SippFilter * ptrFilt;


    for (idx = 0; idx < pPipe->nFiltersDMA; idx++)
    {
        ptrFilt = pPipe->filtersDMA[idx];

        DmaParam * param   = (DmaParam *)ptrFilt->params;
        param->dstChkW     = 0;
        param->srcChkW     = 0;
    }

    for (filtIdx = 0; filtIdx < pPipe->nFilters; filtIdx++)
    {
        ptrFilt = pPipe->filters[filtIdx];

        for (bufIdx = 0; bufIdx < ptrFilt->numOBufs; bufIdx++)
        {
            ptrFilt->oBufs[bufIdx]->mostRecentLine        = -1;
            ptrFilt->oBufs[bufIdx]->bottomLineReplication = 0;
            ptrFilt->oBufs[bufIdx]->internalFillLevel     = 0;
            ptrFilt->oBufs[bufIdx]->filtOutHeight         = ptrFilt->outputH;
        }
        for (bufIdx = 0; bufIdx < ptrFilt->nParents; bufIdx++)
        {
            ptrFilt->iBufCtx[bufIdx]->runFullSearch = (ptrFilt->flags & SIPP_RESIZE) ? 1: 0;
            ptrFilt->iBufCtx[bufIdx]->lastAskLineNo = 0xFFFFFFFF;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////
// MANDATED SCHEDULE CREATION APIS
////////////////////////////////////////////////////////////////////////////////////////

eSIPP_STATUS sippCoreReschedulePipeline (pSippPipeline pPipe)
{
    eSIPP_STATUS eRetCode = eSIPP_STATUS_OK;

    pPipe->numSE            = 0;
    pPipe->nPadFilters      = 0;
    pPipe->schedInfoEntries = 0;

    #ifdef SIPP_SCRT_ENABLE_OPIPE
    u32  useoPipeScheduler  = (pPipe->flags & PLF_CONSIDER_OPIPE_RT) ? 0x1 : 0x0;
    #endif

    sippCoreReInitLineBuffers (pPipe);

    sippInitLnMemPool (&pPipe->tHeapMCB,
                       pPipe->lineMemPoolBase);

    // Free line buffers and schedule info - note these calls will only achive something
    // if these virtual pools were mapped to a communally used heap
    sippMemFreeList (&pPipe->tHeapMCB,
                     vPoolFilterLineBuf);
    sippMemFreeList (&pPipe->tHeapMCB,
                     vPoolSchedule);
    sippMemFreeList (&pPipe->tHeapMCB,
                     vPoolScheduleTemp);

    // Schedule the pipeline
    if (pPipe->pfnSippSchedule (pPipe,
                                true,
                                true) == 0)
    {
        sippProcessSchedData (pPipe);

        // Alloc line buffers in CMX, compute padding strides etc...
        #ifdef SIPP_SCRT_ENABLE_OPIPE
        if (useoPipeScheduler == 0x1)
        {
            if (sippBuildLnBuffsOPipe (pPipe) != 0)
            {
                sippPalTrace (SIPP_API_TL_ERROR, "SIPP: sippBuildLnBuffsOPipe fails\n");
                sippError (pPipe->errorStatus, E_OUT_OF_MEM );
                return 0x1;
            }

            // Update DMA descriptors, rebuild buffer data structs
            sippIniHwFilters (pPipe);
        }
        else
        #endif
        {
            if (sippBuildLnBuffs (pPipe) != 0)
            {
                sippPalTrace (SIPP_API_TL_ERROR, "SIPP: sippBuildLnBuffs fails\n");
                sippError (pPipe->errorStatus, E_OUT_OF_MEM );
                return 0x1;
            }

            if(pPipe->dbgLevel > 0)
            {
                sippDbgDumpSchedForVcsCArr (pPipe);
                sippDbgDumpAsmOffsets (pPipe);
            }

            // Recompute chunk width, strides
            sippComputeBufferProps (pPipe);

            // Initialise line pointers given new sizes
            sippInitBufferLnPointers (pPipe);

            // Update DMA descriptors, rebuild buffer data structs
            sippIniHwFilters (pPipe);

            // When DMA params get recomputed, link is broken, so must relink
            sippChainDmaDesc (pPipe);

            // Rebuild helper constants for shaves (rely on plane stride which may have been altered through rescheduling)
            sippComputeSwOutCt (pPipe);

            // Compute additional data structs for Shave optimizations
            sippAsmOptSetup (pPipe);
        }

        // Need a callback to ensure client knows this is complete
        sippEventNotify (pPipe,
                         eSIPP_PIPELINE_RESCHEDULED,
                         (SIPP_PIPELINE_EVENT_DATA *)NULL);
    }
    else
    {
        eRetCode = eSIPP_STATUS_INTERNAL_ERROR;
    }

    return eRetCode;
}



