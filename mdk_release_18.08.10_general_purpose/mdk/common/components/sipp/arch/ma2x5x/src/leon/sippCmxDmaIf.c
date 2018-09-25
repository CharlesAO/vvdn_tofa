//
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP cmx dma interface - abstracted to this file for ease of switching
///            cmx dma interfacing mechanisms - potentially to use generic driver

/// TODO -     sippDataSectAction uses cmx dma - can we tailor a function??

/////////////////////////////////////////////////////////////////////////////////
//  Header Files
/////////////////////////////////////////////////////////////////////////////////

#include <sipp.h>
#include <sippInternal.h>
#ifdef SIPP_DMA_TIME_EXACT
#include <DrvTimer.h>
#endif

/////////////////////////////////////////////////////////////////////////////////
//  Global Variables
/////////////////////////////////////////////////////////////////////////////////
#ifdef SIPP_DMA_TIME_EXACT
u64 DMAStartTick[3000];
u64 DMAStopTick[3000];
#endif

/////////////////////////////////////////////////////////////////////////////////
//  Global Macros
/////////////////////////////////////////////////////////////////////////////////

#define SIPP_DMA_CQ_ADD_WRITE(quEntry, quNum, address, value64) { \
                                                                    quEntry[quNum].addr  = address; \
                                                                    quEntry[quNum].value = value64; \
                                                                    quNum++; \
                                                                }

#define DMA_TASK_LIST_SZ 16

//////////////////////////////////////////
// Circular DMA task queue

typedef struct
{
    SippPipeline * taskPl[DMA_TASK_LIST_SZ]; // If NULL then slot is empty !
    u32            wPtr;                     // Write pointer : from app space
    u32            rPtr;                     // Read  pointer from ISR side (parsing)

} DmaTaskList;

volatile u32 dmaIdle = 1;
DmaTaskList  dmaTaskList;
extern u32   gSippSliceSz;
extern u32   gSippInitMask;

/////////////////////////////////////////////////////////////////////////////////
//  Function Prototypes
/////////////////////////////////////////////////////////////////////////////////
void sippRunDmaCQDrain (SippPipeline * pl);

/////////////////////////////////////////////////////////////////////////////////
//  Code
/////////////////////////////////////////////////////////////////////////////////

void sippCmxDmaInit ( )
{
    u64 chan2agentMap;

    /////////////////////////////////////////////////////////
    // Enable the CMX-DMA when we init first DMA filter
    // this is a bit risky, move the inits somewhere else !
    if (gSippInitMask & (1 << 0))
    {
        SET_REG_DWORD(CDMA_CTRL_ADR, 0x1LL << 3); // SW reset

        NOP;NOP;NOP;NOP;NOP; NOP;NOP;NOP;NOP;NOP;
        NOP;NOP;NOP;NOP;NOP; NOP;NOP;NOP;NOP;NOP;
        NOP;NOP;NOP;NOP;NOP; NOP;NOP;NOP;NOP;NOP;

        SET_REG_DWORD(CDMA_CTRL_ADR, 0x1LL << 2); // Enable DMA
    }

    SET_REG_DWORD(CDMA_SLICE_SIZE_ADR, gSippSliceSz);

    //  Link the agent SIPP to its corresponding channel
    chan2agentMap  = GET_REG_DWORD_VAL(CDMA_CHTOAGN_ADR);
    chan2agentMap &= (~(0xF << (SIPP_CDMA_AGENT_NO << 0x2)));
    chan2agentMap |= SIPP_CDMA_AGENT_NO << (SIPP_CDMA_AGENT_NO << 0x2);

    SET_REG_DWORD(CDMA_CHTOAGN_ADR, chan2agentMap);

    #ifndef SIPP_PC
    #if SIPP_RTOS == SIPP_NO_RTOS
    #ifndef SIPP_USE_CMXDMA_DRIVER
    DrvIcbIrqClear(IRQ_CMXDMA);
    // Configure CMXDMA interrupt
    #ifdef USE_CMX_DMA_NEW_DRIVER
    DrvIcbSetupIrq (IRQ_CMXDMA, 5, POS_LEVEL_INT, topLevelCmxDmaIrqHandler);
    #else
    DrvIcbSetupIrq (IRQ_CMXDMA, 5, POS_EDGE_INT, topLevelCmxDmaIrqHandler);
    #endif
    DrvIcbEnableIrq(IRQ_CMXDMA);
    #endif
    #endif
    #endif

}

//#################################################################################################
// Always need to put reques in the queue, so that when a transfer is over, we know who finished
// This function may be called from ISR or thread context so use protection

void dmaKickSequenceConcurrent (SippPipeline * pl)
{
    SIPP_PAL_CRIT_STATE uCS;

    sippPalCriticalSectionBegin (&uCS);

    // Buffer overflow protection:
    if (dmaTaskList.taskPl[dmaTaskList.wPtr] != NULL)
    {
        // If slot is already taken then we overflowed the circular queue
        VCS_PRINT_INT(0xDEADBEEF);
        // Error: we're planning to write on a descriptor that was not yet processed
        sippError ((u32 *)NULL, E_CDMA_QU_OVERFLOW);
    }
    else
    {
        dmaTaskList.taskPl[dmaTaskList.wPtr] = pl; // Record who's making the request
        dmaTaskList.wPtr++;                        // Advance WR pointer to next location
        dmaTaskList.wPtr &= 0xF;                   // Circular buffer logic
    }

    if (dmaIdle)
    {
        if (pl->flags & PLF_ENABLE_SW_QU_USE)
        {
            sippRunDmaCQDrain (pl);
        }
        else
        {
            dmaKickSequence (pl);
        }

        dmaIdle = 0;
    }

    sippPalCriticalSectionEnd (uCS);
}


//#################################################################################
u32 sippInitDma (SippFilter * fptr)
{
    SippPipeline * pl = fptr->gi->pl;
    u64            id;
    u32            linesPerIter = fptr->linesPerIter;
    u32            totalBits;

    // Mem for params is already allocated at sippCreateFilter time, so can populate now
    DmaParam * param   = (DmaParam *)fptr->params;
    eDmaMode   dmaMode = param->dmaMode;

    // Scan in the DMA filters and see where we find current filter
    for (id = 0; id < pl->nFiltersDMA; id++)
    {
        if (fptr == pl->filtersDMA[id])
            break;
    }

    if (id == 0)
    {
        // This will be constant for current pipeline for all iterations; no point packing things all the time
        // at each call
        pl->dmaTaskList = (0x1LL                    << 41) |     // Wrap enable
                          (((u64)pl->gi.sliceLast ) << 37) |     // maxSlice
                          (((u64)pl->gi.sliceFirst) << 33) |     // minSlice
                          (0x1LL                    << 32) |     // Start
                          (u64)((u32)pl->filtersDMA[0]->params); // Addr of first descriptor
    }

    param->dmaDsc.dscCtrlLinkAddr = (           0x00000000LL <<  0) | // Link Addr (terminator for now)
                                    (                  0x3LL << 32) | // Transaction type: 2D with chunking
                                    (                  0x0LL << 34) | // Priority
                                    (                  0x4LL << 36) | // Burst Len
                                    (                  0x0LL << 40) | // TransactionID
                                    (((u64)SIPP_CDMA_INT_NO) << 44) | // Interrupt Sel
                                    (                  0x0LL << 48) | // Partition
                                    (                     id << 59);  // Skip

    // No point in programming SRC/DST addresses as we'll do this before every start, for every
    // descriptor that is to be parsed !
    totalBits  = fptr->outputW * fptr->bpp[0];
    totalBits += 0x7;
    totalBits &= 0xFFFFFFF8;
    param->dmaDsc.dscPlanesLen    = ((u64)(fptr->nPlanes[0]-1)<<32) | ((u64)(totalBits >> 3));

    /////////////////////////////////////////////////////
    // DDR to CMX
    if (fptr->nParents == 0)
    {
        if (param->dstChkW == 0)
        {
            totalBits       = fptr->sliceWidth  * fptr->bpp[0];
            totalBits      += 0x7;
            totalBits      &= 0xFFFFFFF8;
            param->dstChkW  = totalBits >> 3;                          // dst in CMX, thus chuked
            param->dstChkS  = pl->sliceSz;                             // chunk stride = slice size
            totalBits       = fptr->planeStride[0] * fptr->bpp[0];
            totalBits      += 0x7;
            totalBits      &= 0xFFFFFFF8;
            param->dstPlS   = totalBits >> 3;
            param->dstLnS   = param->dstChkW;                          // will start in same slice
        }

        if (param->srcChkW == 0)
        {
            totalBits       = fptr->outputW * fptr->bpp[0];
            totalBits      += 0x7;
            totalBits      &= 0xFFFFFFF8;
            param->srcChkW  = totalBits >> 3;                                // src in DDR: thus full line width
            if (dmaMode == DMA_MODE_PARTIAL_LINE)
            {
                param->srcChkS  = (param->extCfg.tPartialCfg.ddrLineStride * fptr->bpp[0]) >> 0x3;
            }
            else
            {
                param->srcChkS  = param->srcChkW;                            // full line, no chunks should exist, thus 0
            }
            totalBits       = (fptr->outputW * fptr->outputH) * fptr->bpp[0];
            totalBits      += 0x7;
            totalBits      &= 0xFFFFFFF8;
            param->srcPlS   = totalBits >> 3;                                // src in DDR, pl stride = full image plane stride
            if (dmaMode == DMA_MODE_PARTIAL_LINE)
            {
                param->srcLnS  = (param->extCfg.tPartialCfg.ddrLineStride * fptr->bpp[0]) >> 0x3;
            }
            else
            {
                param->srcLnS  = param->srcChkW;
            }
        }
    }
    /////////////////////////////////////////////////////
    // CMX to DDR
    else if (fptr->nCons == 0)
    {
        // Note: fptr->planeStride = fptr->lineStride * fptr->nLines;
        //       given a CONSUMER-DMA filter doesn't get an output buffer, thus fptr->nLines = 0
        //       thus planeStride = 0. So use (fptr->parents[0])->planeStride
        if (param->dstChkW == 0)
        {
            totalBits       = fptr->outputW * fptr->bpp[0];
            totalBits      += 0x7;
            totalBits      &= 0xFFFFFFF8;
            param->dstChkW  = totalBits >> 3;                                // dst in DDR, thus line width = full line width
            if (dmaMode == DMA_MODE_PARTIAL_LINE)
            {
                param->dstChkS  = (param->extCfg.tPartialCfg.ddrLineStride * fptr->bpp[0]) >> 0x3;
            }
            else
            {
                param->dstChkS  = param->dstChkW;                            // full line, no chunks should exist, thus 0
            }
            totalBits       = (fptr->outputW * fptr->outputH) * fptr->bpp[0];
            totalBits      += 0x7;
            totalBits      &= 0xFFFFFFF8;
            param->dstPlS  = totalBits >> 3;                                 // src in DDR, pl stride = full image plane stride
            if (dmaMode == DMA_MODE_PARTIAL_LINE)
            {
                param->dstLnS  = (param->extCfg.tPartialCfg.ddrLineStride * fptr->bpp[0]) >> 0x3;
            }
            else
            {
                param->dstLnS  = param->dstChkW;
            }
        }

        if (param->srcChkW == 0)
        {
            totalBits       = fptr->sliceWidth * fptr->bpp[0];
            totalBits      += 0x7;
            totalBits      &= 0xFFFFFFF8;
            param->srcChkW  = totalBits >> 3;                                // src in CMX, thus chunked
            param->srcChkS  = pl->sliceSz;
            totalBits       =  fptr->parents[0]->planeStride[0] * fptr->bpp[0];
            totalBits      += 0x7;
            totalBits      &= 0xFFFFFFF8;
            param->srcPlS   = totalBits >> 3;
            param->srcLnS   = param->srcChkW;
        }
    }

    if (param->dmaDsc.dscSrcStrdWidth == 0)
    {
        param->dmaDsc.dscSrcStrdWidth =  (((u64)param->srcChkS)<<32) | // chunk/line stride
                                         (((u64)param->srcChkW)    );  // chunk/line width
    }

    if (param->dmaDsc.dscDstStrdWidth == 0)
    {
        param->dmaDsc.dscDstStrdWidth =  (((u64)param->dstChkS)<<32) | // chunk/line stride
                                         (((u64)param->dstChkW)    );  // chunk/line width
    }

    param->dmaDsc.dscPlStrides =  (((u64)param->dstPlS)<<32) |
                                  (((u64)param->srcPlS)    );


    // We need to allocate the other descriptors for the remaining lines per iteration
    if (linesPerIter > 1)
    {
        u32 descIdx;

        if (param->pLineDesList == (DmaDesc *)NULL)
        {
            param->pLineDesList = (DmaDesc *)sippMemAlloc (&fptr->pPipe->tHeapMCB,
                                                           vPoolCMXDMADesc,
                                                           sizeof(DmaDesc) * (linesPerIter - 0x1));
        }

        if (param->pLineDesList == (DmaDesc *)NULL)
        {
            sippError (fptr->errorStatus, E_OUT_OF_MEM);
            return 0x1;
        }

        // Update these descriptors
        for (descIdx = 0; descIdx < (linesPerIter - 1); descIdx++)
        {
            param->pLineDesList[descIdx].dscDstStrdWidth = param->dmaDsc.dscDstStrdWidth;
            param->pLineDesList[descIdx].dscSrcStrdWidth = param->dmaDsc.dscSrcStrdWidth;
            param->pLineDesList[descIdx].dscPlStrides    = param->dmaDsc.dscPlStrides;
            param->pLineDesList[descIdx].dscPlanesLen    = param->dmaDsc.dscPlanesLen;
            param->pLineDesList[descIdx].dscCtrlLinkAddr = param->dmaDsc.dscCtrlLinkAddr;

            if (descIdx < (linesPerIter - 2))
            {
                #ifdef RT_DBG
                sippPalTrace (SIPP_API_TL_VERBOSE, "Link to next descriptor : Address = %08x\n", (u32)(&param->pLineDesList[descIdx+1]));
                #endif
                param->pLineDesList[descIdx].dscCtrlLinkAddr  |= (u64)((u32)(&param->pLineDesList[descIdx+1]));
            }
        }

        // Link the first descriptor to the remaining lines per iter descriptor
        param->dmaDsc.dscCtrlLinkAddr |= (u64)((u32)(&param->pLineDesList[0]));
    }

    if (pl->useSyncRuntime == 0)
    {
        sippCmxDmaInitAsync ( );
    }
    else
    {
        // One stream at a time only in sync mode so we can safely disable the
        // sync assigned interrupt bit
        SET_REG_WORD(CDMA_CLR_INTEN_ADR, (1 << SIPP_CDMA_INT_NO));

    }

    return 0x0;
}

//#################################################################################
void sippUpdateDmaAddr (SippFilter * fptr)
{
    u32 linesPerIter = fptr->linesPerIter;
    u32 runNo        = fptr->exeNo * linesPerIter;
    u32 srcAddr      = 0xDEADBEEF;
    u32 dstAddr      = 0xDEADBEEF;
    #ifdef SIPP_SLICE0_ADDRESS
    u32 sliceSz      = fptr->gi->sliceSize;
    #endif

    DmaParam * cfg = (DmaParam *)fptr->params;

    /////////////////////////////////////////
    //// DDR to CMX
    if (fptr->nParents == 0)
    {
        srcAddr = (u32)cfg->ddrAddr + (runNo * cfg->srcLnS);
        #ifdef SIPP_SLICE0_ADDRESS
        dstAddr = (u32)fptr->dbLineOut[0][fptr->exeNo&1][0] + fptr->firstOutSlc * sliceSz;
        #else
        dstAddr = (u32)fptr->dbLineOut[0][fptr->exeNo&1][0];
        #endif
    }
    /////////////////////////////////////////
    //// CMX to DDR
    else if (fptr->nCons == 0)
    {
        #ifdef SIPP_SLICE0_ADDRESS
        srcAddr = (u32)fptr->dbLinesIn[0][fptr->exeNo&1][0][0] + fptr->parents[0]->firstOutSlc * sliceSz;
        #else
        srcAddr = (u32)fptr->dbLinesIn[0][fptr->exeNo&1][0][0];
        #endif
        dstAddr = (u32)cfg->ddrAddr + (runNo * cfg->dstLnS); // Proper line
    }

    // Update the descriptor SRC and DST addresses:
    cfg->dmaDsc.dscDstSrcAddr = ((u64)dstAddr << 32) | (srcAddr);

    if (linesPerIter > 1)
    {
        u32 descIdx;
        for (descIdx = 0; descIdx < (linesPerIter - 1); descIdx++)
        {
            // Add checks here for bottom replication lines
            // If it is bottom replication line we run the dma again with the values
            // for the previous line but this eliminates the need to recalculate
            // irqFreq in dmaKickSequence as if we skipped these dma iters then
            // that value would have to be adjusted
            if ((runNo + descIdx + 1) < fptr->outputH)
            {
                /////////////////////////////////////////
                //// DDR to CMX
                if (fptr->nParents == 0)
                {
                    srcAddr += cfg->srcLnS;
                    #ifdef SIPP_SLICE0_ADDRESS
                    dstAddr  = (u32)fptr->dbLineOut[0][fptr->exeNo&1][descIdx+1] + fptr->firstOutSlc * sliceSz;
                    #else
                    dstAddr  = (u32)fptr->dbLineOut[0][fptr->exeNo&1][descIdx+1];
                    #endif
                }
                /////////////////////////////////////////
                //// CMX to DDR
                else if (fptr->nCons == 0)
                {
                    #ifdef SIPP_SLICE0_ADDRESS
                    srcAddr  = (u32)fptr->dbLinesIn[0][fptr->exeNo&1][descIdx+1][0] + fptr->parents[0]->firstOutSlc * sliceSz;
                    #else
                    srcAddr  = (u32)fptr->dbLinesIn[0][fptr->exeNo&1][descIdx+1][0];
                    #endif
                    dstAddr += cfg->dstLnS; // Proper line
                }
            }
            cfg->pLineDesList[descIdx].dscDstSrcAddr = ((u64)dstAddr << 32) | (srcAddr);
        }
    }
}

//#################################################################################
void dmaKickSequence (SippPipeline *pl)
{
    u32 runMask = pl->schedInfo[pl->iteration].dmaMask &  0x00FFFFFF;
    u32 irqFreq = pl->schedInfo[pl->iteration].dmaMask >> 24;

    SET_REG_DWORD(CDMA_SKIP_DES_ADR,                          (~runMask) & 0x00FFFFFF);
    SET_REG_DWORD(CDMA_INT_FREQ0_ADR + (8*SIPP_CDMA_INT_NO),   irqFreq);
    SET_REG_DWORD(CDMA_LINK_CFG0_ADR + (8*SIPP_CDMA_AGENT_NO), pl->dmaTaskList);

}


//#################################################################################
void sippKickDma (SippPipeline * pl)
{
    SippFilter ** filters  = pl->filtersDMA;
    u32           nFilters = pl->nFiltersDMA;
    u32           runMask  = pl->schedInfo[pl->iteration].dmaMask &  0x00FFFFFF;
    u32           i;

    #ifdef SIPP_DMA_TIME_EXACT
    DrvTimerGetSystemTicks64(&DMAStartTick[pl->iteration]);
    #endif

    // 1) Update SRC and DST addresses for all DMA filters that are to run in this iter
    for (i = 0; i < nFilters; i++)
    {
        if (runMask & (1 << i))
        {
            sippUpdateDmaAddr (filters[i]);
        }
    }

    if (pl->useSyncRuntime == 0)
    {
        dmaKickSequenceConcurrent (pl);
    }
    else
    {
        // Non blocking call - pipeline will be running by itself
        dmaKickSequence (pl);
    }

    #ifdef SIPP_DMA_TIME_EXACT
    sippWaitDma ();
    DrvTimerGetSystemTicks64(&DMAStopTick[pl->iteration]);
    #endif

}

//#################################################################################
u32 sippUpdateDmaAddrCQ (sSippCdmaQuEntry * pQuEntry,
                         SippFilter *       fptr)
{
    u32 linesPerIter = fptr->linesPerIter;
    u32 actExeNo     = fptr->schNo - 1;
    u32 runNo        = actExeNo * linesPerIter; // fptr->exeNo will be wrong at time of calling this function - use fptr->schNo - 1
    u32 srcAddr      = 0xDEADBEEF;
    u32 dstAddr      = 0xDEADBEEF;
    #ifdef SIPP_SLICE0_ADDRESS
    u32 sliceSz      = fptr->gi->sliceSize;
    #endif
    u32 numWrites    = 0;

    DmaParam * cfg = (DmaParam *)fptr->params;

    /////////////////////////////////////////
    //// DDR to CMX
    if (fptr->nParents == 0)
    {
        srcAddr = (u32)cfg->ddrAddr + (runNo * cfg->srcLnS);
        #ifdef SIPP_SLICE0_ADDRESS
        dstAddr = (u32)fptr->dbLineOut[0][actExeNo&1][0] + fptr->firstOutSlc * sliceSz;
        #else
        dstAddr = (u32)fptr->dbLineOut[0][actExeNo&1][0];
        #endif
    }
    /////////////////////////////////////////
    //// CMX to DDR
    else if (fptr->nCons == 0)
    {
        #ifdef SIPP_SLICE0_ADDRESS
        srcAddr = (u32)fptr->dbLinesIn[0][actExeNo&1][0][0] + fptr->parents[0]->firstOutSlc * sliceSz;
        #else
        srcAddr = (u32)fptr->dbLinesIn[0][actExeNo&1][0][0];
        #endif
        dstAddr = (u32)cfg->ddrAddr + (runNo * cfg->dstLnS); // Proper line
    }

    // Update the descriptor SRC and DST addresses:
    SIPP_DMA_CQ_ADD_WRITE (pQuEntry, numWrites, ((u64)((u32)&cfg->dmaDsc.dscDstSrcAddr)), ((u64)dstAddr << 32) | (srcAddr));

    if (linesPerIter > 1)
    {
        u32 descIdx;
        for (descIdx = 0; descIdx < (linesPerIter - 1); descIdx++)
        {
            // Add checks here for bottom replication lines
            // If it is bottom replication line we run the dma again with the values
            // for the previous line but this eliminates the need to recalculate
            // irqFreq in dmaKickSequence as if we skipped these dma iters then
            // that value would have to be adjusted
            if ((runNo + descIdx + 1) < fptr->outputH)
            {
                /////////////////////////////////////////
                //// DDR to CMX
                if (fptr->nParents == 0)
                {
                    srcAddr += cfg->srcLnS;
                    #ifdef SIPP_SLICE0_ADDRESS
                    dstAddr  = (u32)fptr->dbLineOut[0][actExeNo&1][descIdx+1] + fptr->firstOutSlc * sliceSz;
                    #else
                    dstAddr  = (u32)fptr->dbLineOut[0][actExeNo&1][descIdx+1];
                    #endif
                }
                /////////////////////////////////////////
                //// CMX to DDR
                else if (fptr->nCons == 0)
                {
                    #ifdef SIPP_SLICE0_ADDRESS
                    srcAddr  = (u32)fptr->dbLinesIn[0][actExeNo&1][descIdx+1][0] + fptr->parents[0]->firstOutSlc * sliceSz;
                    #else
                    srcAddr  = (u32)fptr->dbLinesIn[0][actExeNo&1][descIdx+1][0];
                    #endif
                    dstAddr += cfg->dstLnS; // Proper line
                }
            }
            SIPP_DMA_CQ_ADD_WRITE (pQuEntry, numWrites, ((u64)((u32)&cfg->pLineDesList[descIdx].dscDstSrcAddr)), ((u64)dstAddr << 32) | (srcAddr));
            // cfg->pLineDesList[descIdx].dscDstSrcAddr = ((u64)dstAddr << 32) | (srcAddr);

        }
    }

    return numWrites;
}

//#################################################################################
u32 dmaKickSequenceCQ (sSippCdmaQuEntry * pQuEntry,
                       SippPipeline *     pl,
                       u32                iteration)
{
    u32 runMask   = pl->schedInfo[iteration].dmaMask &  0x00FFFFFF;
    u32 irqFreq   = pl->schedInfo[iteration].dmaMask >> 24;
    u32 numWrites = 0;

    SIPP_DMA_CQ_ADD_WRITE (pQuEntry, numWrites, (u64)CDMA_SKIP_DES_ADR, (~runMask) & 0x00FFFFFF);
    SIPP_DMA_CQ_ADD_WRITE (pQuEntry, numWrites, (u64)CDMA_INT_FREQ0_ADR + (8*SIPP_CDMA_INT_NO), irqFreq);
    SIPP_DMA_CQ_ADD_WRITE (pQuEntry, numWrites, (u64)CDMA_LINK_CFG0_ADR + (8*SIPP_CDMA_AGENT_NO), pl->dmaTaskList);

    return numWrites;
}

//#################################################################################
void sippKickDmaCQ (SippPipeline * pl,
                    u32            iteration)
{
    SippFilter ** filters   = pl->filtersDMA;
    u32           nFilters  = pl->nFiltersDMA;
    u32           runMask   = pl->schedInfo[iteration].dmaMask &  0x00FFFFFF;
    u32           i;
    psSippCdmaQu  pQu       = &pl->tCmxDmaQu[iteration & 0x1];

    // 1) Update SRC and DST addresses for all DMA filters that are to run in this iter
    for (i = 0; i < nFilters; i++)
    {
        if (runMask & (1 << i))
        {
            pQu->numEntries += sippUpdateDmaAddrCQ (&pQu->entry[pQu->numEntries],
                                                    filters[i]);
        }
    }

    // Non blocking call - pipeline will be running by itself
    pQu->numEntries += dmaKickSequenceCQ (&pQu->entry[pQu->numEntries],
                                          pl,
                                          iteration);

    if (pQu->numEntries > pQu->size)
    {
        // Note we may already have done some other damage by this time!!
        // but this is a debug only check - at runtime expect to be removed
        sippError (pl->errorStatus, E_CDMA_QU_OVERFLOW);
        // Change the error code sometime
    }
}


void sippRunDmaCQDrain (SippPipeline * pl)
{
    sSippCdmaQu * pQu       =  &pl->tCmxDmaQu[pl->iteration & 0x1];
    u32           writeIdx;

    for (writeIdx = 0; writeIdx < pQu->numEntries; writeIdx++)
    {
        SET_REG_DWORD(pQu->entry[writeIdx].addr, pQu->entry[writeIdx].value);
    }

    // Reset the number of qu entries
    pQu->numEntries = 0;
}


//#################################################################################
void sippRunDmaCQ (SippPipeline * pl)
{
    if (pl->useSyncRuntime == 0)
    {
        dmaKickSequenceConcurrent (pl);
    }
    else
    {
        sippRunDmaCQDrain (pl);
    }
}

u32 sippDmaCQInit (SippPipeline * pPipe)
{
    if (pPipe->nFiltersDMA)
    {
        u32 numEntries = (pPipe->nFiltersDMA * pPipe->linesPerIter) + 0x5;

        pPipe->tCmxDmaQu[0].entry = (psSippCdmaQuEntry)sippMemAlloc (&pPipe->tHeapMCB,
                                                                     vPoolPipeStructs,
                                                                     numEntries * sizeof(sSippCdmaQuEntry));

        if (pPipe->tCmxDmaQu[0].entry == (psSippCdmaQuEntry)0x0)
        {
            sippError (pPipe->errorStatus, E_OUT_OF_MEM);
            return 0x1;
        }
        else
        {
            pPipe->tCmxDmaQu[0].size       = numEntries;
            pPipe->tCmxDmaQu[0].numEntries = 0;
        }
        pPipe->tCmxDmaQu[1].entry = (psSippCdmaQuEntry)sippMemAlloc (&pPipe->tHeapMCB,
                                                                     vPoolPipeStructs,
                                                                     numEntries * sizeof(sSippCdmaQuEntry));

        if (pPipe->tCmxDmaQu[1].entry == (psSippCdmaQuEntry)0x0)
        {
            sippError (pPipe->errorStatus, E_OUT_OF_MEM);
            return 0x1;
        }
        else
        {
            pPipe->tCmxDmaQu[1].size       = numEntries;
            pPipe->tCmxDmaQu[1].numEntries = 0;
        }
    }

    return 0;
}

//#################################################################################

u32  sippWaitDma ()
{
    #ifdef MYRIAD2
    #define WAIT_MASK (1<<SIPP_CDMA_INT_NO)
    u32 loopCount = 0;
    u64 status;

    while(1)
    {
        status = swcLeonReadNoCacheU64 (CDMA_INT_STATUS_ADR | 0x08000000);

        if ((status & WAIT_MASK) != WAIT_MASK)
        {
            NOP; NOP; NOP; NOP; NOP;
            NOP; NOP; NOP; NOP; NOP;
            NOP; NOP; NOP; NOP; NOP;

            // After a few nops, continue the polling
        }
        else
        {
            break;
        }

        loopCount++;

    }

    // Clear status bit for next iteration...
    SET_REG_DWORD(CDMA_INT_RESET_ADR, (u64)WAIT_MASK);

    return loopCount;

    #else

    // PC and DMA copies are blocking, so nothing to be done
    return 0;
    #endif
}

//#################################################################################
// MUST be done once entire pipeline is defined !
// If there is a single DMA in the system, it's dscCtrlLinkAddr is already null
// from constructor, so nothing will be done

void sippChainDmaDesc (SippPipeline * pl)
{
    s32 i;

    for (i = 0; i < (s32)pl->nFiltersDMA - 1; i++)
    {
        DmaParam * paramCur;
        DmaParam * paramNxt;
        u32        linesPerIter = pl->filtersDMA[i  ]->linesPerIter;

        paramCur = (DmaParam *)pl->filtersDMA[i  ]->params;
        paramNxt = (DmaParam *)pl->filtersDMA[i+1]->params;

        if (linesPerIter < 2)
        {
            paramCur->dmaDsc.dscCtrlLinkAddr = paramCur->dmaDsc.dscCtrlLinkAddr | ((u64)((u32)paramNxt));
        }
        else
        {
            paramCur->pLineDesList[linesPerIter - 2].dscCtrlLinkAddr  |= ((u64)((u32)paramNxt));
        }
    }
    // Last DMA: already assumes nothing follows after, so nothing else tbd...
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippCmxDmaDoneIrqHandler                                         //
//                                                                                //
//  DESCRIPTION: sipp CMX DMA interrupt service routine                           //
//                                                                                //
//  INPUTS:      None.                                                            //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     Called as an ISR from the CMX DMA interrupt                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_CMX_DMA_NEW_DRIVER
void sippCmxDmaDoneIrqHandler (bool bClear)
#else
void sippCmxDmaDoneIrqHandler ()
#endif
{
    SippPipeline *      pPipe;
    #ifdef RT_DBG
    ptSippPipelineSuper ptSuperPipe;
    #endif

    SET_REG_DWORD(CDMA_INT_RESET_ADR, (1<<SIPP_CDMA_INT_NO));  // Clear bit of interest

    #ifdef MYRIAD2
    #ifdef USE_CMX_DMA_NEW_DRIVER
    if (bClear)
    {
        DrvIcbIrqClear (IRQ_CMXDMA);
    }
    #endif
    #endif

    pPipe = dmaTaskList.taskPl[dmaTaskList.rPtr];              // Get a ref to the pipeline that finished

    dmaTaskList.taskPl[dmaTaskList.rPtr] = NULL;     // Transaction done, mark slot as FREE
    dmaTaskList.rPtr++;                              // Go to next entry
    dmaTaskList.rPtr &= 0xF;                         // Circular buffer logic

    #ifdef SIPP_RUNTIME_PROFILING
    if (pPipe->pRTStats)
    {
        DrvTimerGetSystemTicks64(&pPipe->pRTStats->iterTime[(pPipe->iteration * SIPP_RT_PER_ITER_STATS_SIZE) + SIPP_ITER_STAT_ASYNC_CDMA_POS]);
    }
    #endif

    // Check if got more things to do:
    if (dmaTaskList.taskPl[dmaTaskList.rPtr] == NULL)
    {
        // Nothing else to do...
        dmaIdle = 1;
    }
    else
    {
        // Get ref to new Task
        SippPipeline * pNextDMAPipe = dmaTaskList.taskPl[dmaTaskList.rPtr];

        // Since we're in DMA-done ISR, definitely DMA is idle, so can restart it here:
        if (pNextDMAPipe->flags & PLF_ENABLE_SW_QU_USE)
        {
            sippRunDmaCQDrain (pNextDMAPipe);
        }
        else
        {
            dmaKickSequence (pNextDMAPipe);
        }

        dmaIdle = 0;
    }

    #ifdef SIPP_PC
    if (pPipe)
    {
    #endif
    #ifdef RT_DBG
    ptSuperPipe = (ptSippPipelineSuper)pPipe;
    SIPP_HISTORY_LOG(0xDD000000|ptSuperPipe->uPipeIdx);
    #endif
    sippCheckIterComplete (pPipe, PFL_DMA_DONE);
    #ifdef SIPP_PC
    }
    #endif
}




#ifndef SIPP_PC
#if SIPP_RTOS == SIPP_NO_RTOS

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    topLevelCmxDmaIrqHandler                                         //
//                                                                                //
//  DESCRIPTION: Top level CMX DMA interrupt service routine                      //
//                                                                                //
//  INPUTS:      None.                                                            //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     Called as an ISR from the CMX DMA interrupt                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void topLevelCmxDmaIrqHandler (u32 irqSource)
{
    u32 intState;

    // Read interrupt status bits
    intState = GET_REG_WORD_VAL (CDMA_INT_STATUS_ADR);

    #ifdef USE_CMX_DMA_NEW_DRIVER
    DrvCmxDmaIrqHandler (irqSource);
    #endif

    if (intState & (0x1 << SIPP_CDMA_INT_NO))
    {
        #ifdef MYRIAD2
        #ifdef USE_CMX_DMA_NEW_DRIVER
        bool bClearICB = FALSE;

        if ((DrvCmxDmaGetIrqMask() & intState) == 0x0)
        {
            bClearICB = TRUE;
        }

        // Checks the INT-bit associated to SIPP, and only treats that one !
        sippCmxDmaDoneIrqHandler (bClearICB);

        #else
        sippCmxDmaDoneIrqHandler ();
        #endif
        #endif

    }

    #ifndef USE_CMX_DMA_NEW_DRIVER
    DrvCmxDmaIrqHandlerFunc (irqSource);
    #endif
}

#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippCmxDmaInitAsync                                              //
//                                                                                //
//  DESCRIPTION: Ensure CMX DMA HW is setup for sipp async mode                   //
//               if we need to use it                                             //
//                                                                                //
//  INPUTS:      None.                                                            //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       Possibly have an undo function?                                  //
//                                                                                //
//  CONTEXT:     None.                                                            //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippCmxDmaInitAsync ( )
{
    #ifndef SIPP_PC
    if (PROCESS_LEON_OS == swcWhoAmI())
    {
        SET_REG_DWORD(CDMA_CLR_IMASK_ADR, (1 << SIPP_CDMA_INT_NO));
    }
    else
    {
        SET_REG_DWORD(CDMA_SET_IMASK_ADR, (1 << SIPP_CDMA_INT_NO));
    }
    #endif

    SET_REG_WORD(CDMA_SET_INTEN_ADR, (1 << SIPP_CDMA_INT_NO));

}


// End of file
