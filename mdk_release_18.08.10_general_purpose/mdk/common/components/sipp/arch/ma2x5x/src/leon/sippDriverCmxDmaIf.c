//
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP cmx dma driver interface


/////////////////////////////////////////////////////////////////////////////////
//  Header Files
/////////////////////////////////////////////////////////////////////////////////

#include <sipp.h>
#include <sippInternal.h>
#ifdef SIPP_DMA_TIME_EXACT
#include <DrvTimer.h>
#endif

#ifndef SIPP_PC
#include <DrvCmxDma.h>
#endif

#ifdef SIPP_USE_CMXDMA_DRIVER

/////////////////////////////////////////////////////////////////////////////////
//  Global Variables
/////////////////////////////////////////////////////////////////////////////////
#ifdef SIPP_DMA_TIME_EXACT
u64 DMAStartTick[3000];
u64 DMAStopTick[3000];
#endif

/////////////////////////////////////////////////////////////////////////////////
//  Code
/////////////////////////////////////////////////////////////////////////////////

void sippDrvCmxDmaInit ( )
{
    // Enable the CMX-DMA driver
    DrvCmxDmaInitDefault();
}

void sippDrvUpdateDmaAddr (SippFilter * fptr)
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

void drvCmxDmaDoneIrqHandler (dmaTransactionList * transHnd, void * userContext)
{
    SippPipeline * pPipe = (SippPipeline *)userContext;
    UNUSED(transHnd);

    #ifdef SIPP_PC
    if (pl)
    {
    #endif
    #ifdef RT_DBG
    ptSippPipelineSuper ptSuperPipe = (ptSippPipelineSuper)pPipe;
    SIPP_HISTORY_LOG(0xDD000000|ptSuperPipe->uPipeIdx);
    #endif
    sippCheckIterComplete (pPipe, PFL_DMA_DONE);
    #ifdef SIPP_PC
    }
    #endif
}

void dmaDrvKickSequence (SippPipeline *pl)
{
    DmaParam *           dmaDesc = NULL;
    u32                  dmaFiltersToRun[SIPP_MAX_DMA_FILTERS_PER_PIPELINE];
    u32                  nDmaFiltersToRun = 0;
    u32                  runMask = pl->schedInfo[pl->iteration].dmaMask & 0x00FFFFFF;
    u32                  i;

    // Make a list of all DMA filters to run in this iter
    for (i = 0; i < pl->nFiltersDMA; i++)
    {
        if (runMask & (1<<i))
        {
            dmaFiltersToRun[nDmaFiltersToRun++] = i;
        }
    }

    // Copy details from filter descriptors to driver transaction descriptors
    if (nDmaFiltersToRun > 1)
    {
        for (i = 0; i < nDmaFiltersToRun - 1; i++)
        {
            dmaDesc = pl->filtersDMA[dmaFiltersToRun[i]]->params;

            if (pl->linesPerIter < 2)
            {
                u32 *      ptrLL       = (u32 *)&dmaDesc->dmaDsc.dscCtrlLinkAddr;
                DmaParam * nextDmaDesc =  pl->filtersDMA[dmaFiltersToRun[i+1]]->params;
                *ptrLL                 = (u32)nextDmaDesc->drvDesc;
            }
            else
            {
                u32 * ptrLL = (u32 *)&dmaDesc->pLineDesList[pl->linesPerIter-2].dscCtrlLinkAddr;
                *ptrLL      = (u32)pl->filtersDMA[dmaFiltersToRun[i+1]]->params;
                // dmaDesc->pLineDesList[pl->linesPerIter - 2].dscCtrlLinkAddr |= ((u64)((u32)pl->filtersDMA[dmaFiltersToRun[i+1]]->params));
            }

            sippPalMemcpy(dmaDesc->drvDesc, dmaDesc, sizeof(DmaDesc));
        }

        // Zero final linked list entry
        dmaDesc = pl->filtersDMA[dmaFiltersToRun[i]]->params;

        if (pl->linesPerIter < 2)
        {
            u32 * ptrLL = (u32 *)&dmaDesc->dmaDsc.dscCtrlLinkAddr;
            *ptrLL = 0;

        }
        else
        {
            u32 * ptrLL = (u32 *)&dmaDesc->pLineDesList[pl->linesPerIter-2].dscCtrlLinkAddr;
            *ptrLL = 0;
        }

        sippPalMemcpy(dmaDesc->drvDesc, dmaDesc, sizeof(DmaDesc));

    }
    else if (nDmaFiltersToRun == 1)
    {
        dmaDesc = pl->filtersDMA[dmaFiltersToRun[0]]->params;

        u32 * ptrLL = (u32 *)&dmaDesc->dmaDsc.dscCtrlLinkAddr;
        *ptrLL = 0;

        sippPalMemcpy(dmaDesc->drvDesc, dmaDesc, sizeof(DmaDesc));
    }

    dmaDesc = pl->filtersDMA[dmaFiltersToRun[0]]->params;

    DrvCmxDmaStartTaskAsync (dmaDesc->drvDesc, drvCmxDmaDoneIrqHandler, pl);
}

void dmaDrvKickSequenceConcurrent (SippPipeline * pl)
{
    SIPP_PAL_CRIT_STATE uCS;

    sippPalCriticalSectionBegin (&uCS);
    dmaDrvKickSequence (pl);
    sippPalCriticalSectionEnd (uCS);
}


//#################################################################################
void sippDrvKickDma (SippPipeline * pl)
{
    SippFilter **filters = pl->filtersDMA;
    u32          nFilters = pl->nFiltersDMA;
    u32          runMask  = pl->schedInfo[pl->iteration].dmaMask &  0x00FFFFFF;
    u32          i;

    #ifdef SIPP_DMA_TIME_EXACT
    DrvTimerGetSystemTicks64(&DMAStartTick[pl->iteration]);
    #endif

    // 1) Update SRC and DST addresses for all DMA filters that are to run in this iter
    for (i = 0; i < nFilters; i++)
    {
        if (runMask & (1 << i))
        {
            sippDrvUpdateDmaAddr (filters[i]);
        }
    }

    if (pl->useSyncRuntime == 0)
    {
        dmaDrvKickSequenceConcurrent (pl);
    }
    else
    {
        // Non blocking call - pipeline will be running by itself
        // dmaDrvKickSequence (pl);
    }

    #ifdef SIPP_DMA_TIME_EXACT
    // sippWaitDma ();
    DrvTimerGetSystemTicks64(&DMAStopTick[pl->iteration]);
    #endif

}

u32 sippInitDrvDesc (SippPipeline * pl)
{
    u32          dmaFiltIdx;
    DmaParam *   param;
    SippFilter * ptrFilt;

    // Allocate and initialize driver transaction descriptors
    for (dmaFiltIdx = 0; dmaFiltIdx < pl->nFiltersDMA; dmaFiltIdx++)
    {
        ptrFilt = pl->filtersDMA[dmaFiltIdx];
        param   = (DmaParam *)ptrFilt->params;
        param->drvDesc = (dmaTransactionList *)sippMemAlloc(&pl->tHeapMCB,
                                                     vPoolCMXDMADesc,
                                                     sizeof(dmaTransactionList));

        if (param->drvDesc == (dmaTransactionList *)NULL)
        {
            sippError (ptrFilt->errorStatus, E_OUT_OF_MEM);
            return 0x1;
        }

        DrvCmxDmaTransactionBriefInit (DrvCmxDmaInitRequesterOnAgent(1, 0),
                                       DMA_2D_TRANSACTION,
                                       param->drvDesc);
    }

    return 0x0;
}

#endif // SIPP_USE_CMXDMA_DRIVER
