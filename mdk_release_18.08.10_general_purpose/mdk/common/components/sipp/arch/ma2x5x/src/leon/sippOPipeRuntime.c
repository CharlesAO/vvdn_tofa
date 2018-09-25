///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#ifdef SIPP_SCRT_ENABLE_OPIPE

/////////////////////////////////////////////////////////////////////////////////
//  Header Files
/////////////////////////////////////////////////////////////////////////////////

#include <sipp.h>
#include <sippInternal.h>

#ifndef SIPP_PC
#include <DrvLeonL2C.h>
#include <DrvCpr.h>
#include <DrvIcb.h>
#include <DrvCmxDma.h>
#ifdef USE_CMX_DMA_NEW_DRIVER
#include <DrvCdma.h>
#endif
#endif

/////////////////////////////////////////////////////////////////////////////////
//  Local Macros
/////////////////////////////////////////////////////////////////////////////////

//#define SIPP_OPIPE_RT_DBG_ENABLE

#ifdef SIPP_OPIPE_RT_DBG_ENABLE

#define LOCAL_DBG_ARR_HIST_SIZE 8192
#define LOCAL_DBG_NUM_ARRS      3

u32 localOPipeDbrArr[LOCAL_DBG_NUM_ARRS][LOCAL_DBG_ARR_HIST_SIZE];
u32 localOPipeDbrArrIdx[LOCAL_DBG_NUM_ARRS] = {0,0,0};
u32 localOPipeDbrArrGpIdx                   = (LOCAL_DBG_NUM_ARRS - 1);

#define SIPP_OPIPE_RT_DBG(x) {\
                                 localOPipeDbrArr[localOPipeDbrArrGpIdx][localOPipeDbrArrIdx[localOPipeDbrArrGpIdx]++] = x;\
                                 if (localOPipeDbrArrIdx[localOPipeDbrArrGpIdx] == LOCAL_DBG_ARR_HIST_SIZE) localOPipeDbrArrIdx[localOPipeDbrArrGpIdx] = 0;\
                             }
#else
#define SIPP_OPIPE_RT_DBG(x)
#endif

#define USE_NEW_IBFL
#define USE_NEW_LOOP
#define USE_NEW_OBFL

/////////////////////////////////////////////////////////////////////////////////
//  Global Variables
/////////////////////////////////////////////////////////////////////////////////

extern pSIPP_HW_SESSION pgSippHW;
extern u32              sippGlobalOBFLIncStatus;

/////////////////////////////////////////////////////////////////////////////////
//  Function Prototypes
/////////////////////////////////////////////////////////////////////////////////

#ifdef SIPP_TAPOUTS_ENABLED
void dmaLastSinkIrqHandler (dmaTransactionList * transHnd, void * userContext);
#ifdef USE_CMX_DMA_NEW_DRIVER
void dmaSinkIrqHandler (DrvCmxDmaTransactionHnd * transHnd, void * userContext);
#else
void dmaSinkIrqHandler (dmaTransactionList * transHnd, void * userContext);
#endif
#endif

/////////////////////////////////////////////////////////////////////////////////
//  Code
/////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOPipeRuntimeFrameReset                                       //
//                                                                                //
//  DESCRIPTION: Reset runtime variables                                          //
//                                                                                //
//  INPUTS:      pipeLine - Pipeline                                              //
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

void sippOPipeRuntimeFrameReset (SippPipeline * pl)
{
    u32 i, o;

    for (i = 0; i < pl->numManagedBufs; i++)
    {
        pSippOPipeBuf managedBuf            = &pl->managedBufList[i];
        managedBuf->pBuf->internalFillLevel = 0;
        managedBuf->linesNextFill           = 0;

        #ifdef SIPP_ADD_OPIPE_RT_CHECKS
        managedBuf->lastOutLineServiced     = 0x0;
        managedBuf->totalLinesServiced      = 0x0;
        #endif

        for (o = 0; o < managedBuf->numClients; o++)
        {
            managedBuf->clientCount[o]      = 0x0;
            managedBuf->sinkLineCount[o]    = 0x0;
            #ifdef SIPP_ADD_OPIPE_RT_CHECKS
            managedBuf->lastLineServiced[o] = 0x0;
            #endif
        }
    }

    #ifdef SIPP_OPIPE_RT_DBG_ENABLE
    localOPipeDbrArrGpIdx++;
    if (localOPipeDbrArrGpIdx == LOCAL_DBG_NUM_ARRS) localOPipeDbrArrGpIdx = 0;
    localOPipeDbrArrIdx[localOPipeDbrArrGpIdx] = 0;
    #endif
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOPipeFinished                                                //
//                                                                                //
//  DESCRIPTION: Check for pipeline frame completion                              //
//                                                                                //
//  INPUTS:      pipeLine - Pipeline                                              //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       A pipe is considered finished when all its dma sinks have copied //
//               all lines to destination.                                        //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippOPipeFinished (SippPipeline * pPipe)
{
    u32 managedBufIdx, clientIdx;

    for (managedBufIdx = 0; managedBufIdx < pPipe->numManagedBufs; managedBufIdx++)
    {
        pSippOPipeBuf managedBuf = &pPipe->managedBufList[managedBufIdx];

        if (managedBuf->pFilt->unit != SIPP_DMA_ID)
        {
            for (clientIdx = 0; clientIdx < managedBuf->numClients; clientIdx++)
            {
                if (managedBuf->clientCountMap[clientIdx] == (0x1 << SIPP_DMA_ID))
                {
                    // This should really be the height of the DMA output, not managedBuf->pFilt->outputH -
                    // what if it was cropping
                    if (managedBuf->sinkLineCount[clientIdx] < managedBuf->pFilt->outputH)
                    {
                        return 0;
                    }
                }
            }
        }
    }

    return 0x1;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOpipeIbflDecAllClients                                       //
//                                                                                //
//  DESCRIPTION: Check that all clients of a managed buffer have completed a      //
//               certain amount of consumption                                    //
//                                                                                //
//  INPUTS:      pManagedBuf - Pointger to managed buffer                         //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     The number of lines to be made free in a buffer.                 //
//                                                                                //
//  NOTES:       For simplicity, this function only triggers calls when all       //
//               clients have onsumed a number of lines equal to the irq rate of  //
//               the producing filter.                                            //
//                                                                                //
//  CONTEXT:     Only called in irq context (assuming dma callbacks are in irq    //
//               context?)                                                        //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32  sippOpipeIbflDecAllClients (pSippOPipeBuf pManagedBuf)
{
    u32 retVal;
    u32 triggerVal;
    u32 clientIdx;
    SIPP_PAL_CRIT_STATE uCS;

    sippPalCriticalSectionBegin (&uCS);

    retVal = triggerVal = (1 << pManagedBuf->pFilt->irqRatePow);

    for (clientIdx = 0x0; (clientIdx < pManagedBuf->numClients) && (retVal); clientIdx++)
    {
        if (pManagedBuf->clientCount[clientIdx] < triggerVal) retVal = 0x0;
    }

    if (retVal)
    {
        for (clientIdx = 0x0; clientIdx < pManagedBuf->numClients; clientIdx++)
        {
            pManagedBuf->clientCount[clientIdx] -= triggerVal;
        }
    }

    sippPalCriticalSectionEnd (uCS);

    return retVal;
}

#ifndef SIPP_PC
#ifdef USE_CMX_DMA_NEW_DRIVER

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGetNextDmaDsc                                                //
//                                                                                //
//  DESCRIPTION: Return next descriptor & handler in circular fashion             //
//                                                                                //
//  INPUTS:      ptrFilt - DMA filter                                             //
//                                                                                //
//  OUTPUTS:     ppDmaH - pointer to a DMA transaction handle                     //
//                                                                                //
//  RETURNS:     DrvCmxDmaTransaction * - pointer to head of a linked list of     //
//               transactions.                                                    //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

DrvCmxDmaTransaction * sippGetNextDmaDsc (SippFilter *               ptrFilt,
                                          DrvCmxDmaTransactionHnd ** ppDmaH)
{
    DmaParam *             param      = (DmaParam *)ptrFilt->params;
    DMATransDesc *         pTransList = param->pTransList;
    DrvCmxDmaTransaction * ret        = &pTransList->sippDmaDsc[pTransList->curDsc].dmaDsc;

    *ppDmaH                           = &pTransList->dmaHnd[pTransList->curDsc];

    pTransList->curDsc++;
    if (pTransList->curDsc == SIPP_NUM_DESCS_PER_CDMA)
    {
        pTransList->curDsc  = 0;
    }

    return ret;
}

#else

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGetNextDmaDsc                                                //
//                                                                                //
//  DESCRIPTION: Return next descriptor & handler in cicular fashion              //
//                                                                                //
//  INPUTS:      ptrFilt - DMA filter                                             //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     dmaTransactionList * - pointer to head of a linked list of       //
//               transactions.                                                    //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

dmaTransactionList * sippGetNextDmaDsc (SippFilter  * ptrFilt)
{
    DmaParam *           param       = (DmaParam *)ptrFilt->params;
    DMATransDesc *       pTransList  = param->pTransList;
    u32                  cacheAddr   = (u32)&pTransList->sippDmaDsc[pTransList->curDsc].dmaDsc;
    dmaTransactionList * ret         = (dmaTransactionList *)(cacheAddr | 0x8000000);

    pTransList->curDsc++;
    if (pTransList->curDsc == SIPP_NUM_DESCS_PER_CDMA)
    {
        pTransList->curDsc  = 0;
    }

    return ret;
}

#endif
#endif

#ifndef SIPP_PC

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    dmaSourceIrqHandler                                              //
//                                                                                //
//  DESCRIPTION: CMX DMA driver transaction complete callback for source filters  //
//                                                                                //
//  INPUTS:      transHnd - DMA transaction handle                                //
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

#ifdef USE_CMX_DMA_NEW_DRIVER
void dmaSourceIrqHandler (DrvCmxDmaTransactionHnd * transHnd, void * userContext)
#else
void dmaSourceIrqHandler (dmaTransactionList * transHnd, void * userContext)
#endif
{
    pSippOPipeBuf pManagedBuf  = (pSippOPipeBuf)userContext;
    #ifdef USE_CMX_DMA_NEW_DRIVER
    SippDmaDesc * pSippDmaDesc = (SippDmaDesc *)transHnd->head;
    #else
    SippDmaDesc * pSippDmaDesc = (SippDmaDesc *)transHnd;
    #endif
    u32           i;

    for(i = 0; i < pSippDmaDesc->linesInTransaction; i++)
    {
        SET_REG_WORD(SIPP_IBFL_INC_ADR, pManagedBuf->pBuf->hwInputBufId);
    }

    #ifdef SIPP_TAPOUTS_ENABLED
    // In this case we allow a dma in to feed a dma out...
    {
        SippFilter *    fptr = pManagedBuf->pFilt;
        psSchLineBuffer pBuf = pManagedBuf->pBuf;
        u32 consIdx;

        for (consIdx = 0; consIdx < fptr->nCons; consIdx++)
        {
            SippFilter * cons = fptr->cons[consIdx];

            // Consumer is a DMA out buffer: CMX->DDR transfer
            // Also check DMA consumer is actually consuming from the buffer under inspection
            if ((cons->nCons == 0) &&
                (pBuf == cons->iBufs[0x0]))
            {
                SIPP_PAL_CRIT_STATE uCS;
                SippDmaDesc * pSippDmaDescTap;

                sippPalCriticalSectionBegin (&uCS);

                #ifdef USE_CMX_DMA_NEW_DRIVER
                DrvCmxDmaTransactionHnd * dmaH;
                DrvCmxDmaTransaction *    dmaD;

                dmaD = sippGetNextDmaDsc (cons,
                                          &dmaH);
                #else
                dmaTransactionList * dmaD = sippGetNextDmaDsc (cons);
                #endif
                pSippDmaDescTap = (SippDmaDesc *)dmaD;

                // As per in sippOPipeFinished this should really use the consumer filter height, not that of
                // the line buffer producer
                if (pManagedBuf->sinkLineCount[pSippDmaDescTap->managedBufClientIdx] < pManagedBuf->pFilt->outputH)
                {
                    #ifndef SIPP_PC
                    u32           srcAddr;
                    u32           dstAddr;
                    DmaParam *    param = (DmaParam *)cons->params;
                    bool          bLastTrans = FALSE;

                    srcAddr = (u32)pSippDmaDesc->dmaDsc.dst;
                    dstAddr = (u32)param->ddrAddr + ((cons->outputW * pManagedBuf->sinkLineCount[pSippDmaDescTap->managedBufClientIdx]) << cons->bytesPerPix);

                    dmaD->length = pSippDmaDesc->dmaDsc.length;
                    dmaD->src    = (void *)srcAddr;
                    dmaD->dst    = (void *)dstAddr;

                    pSippDmaDescTap->linesInTransaction = pSippDmaDesc->linesInTransaction;

                    if ((pManagedBuf->sinkLineCount[pSippDmaDescTap->managedBufClientIdx] + pSippDmaDescTap->linesInTransaction) >=
                        pManagedBuf->pFilt->outputH
                       )
                    {
                        bLastTrans = TRUE;
                    }

                    #ifdef USE_CMX_DMA_NEW_DRIVER
                    if (MYR_DRV_ERROR == DrvCmxDmaStartTransfer  (dmaH,
                                                                  dmaSinkIrqHandler,
                                                                  pManagedBuf))
                    #else
                    if (CDMA_TASK_QUEUE_FULL <= DrvCmxDmaStartTaskAsync (dmaD,
                                                                         dmaSinkIrqHandler,
                                                                         pManagedBuf))
                    #endif
                    {
                        sippPalTrace (SIPP_RUNTIME_TL_CATASTROPHE, "CMDA Error 02\n");
                    }

                }

                sippPalCriticalSectionEnd (uCS);
                #endif
            }
        }
    }

    #endif


    SIPP_OPIPE_RT_DBG (0xEE00);
    SIPP_OPIPE_RT_DBG (pSippDmaDesc->linesInTransaction);
}

#endif

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOpipeHandleIbflDec                                           //
//                                                                                //
//  DESCRIPTION: SIPP input buffer decrement ISR handler                          //
//                                                                                //
//  INPUTS:      status - sipp Interupt 0 source status                           //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippOpipeHandleIbflDec (SippPipeline * pPipe, u32 status)
{
    SippFilter * fptr;
    u32          idx;
    u32          retVal = 0;

    SIPP_OPIPE_RT_DBG (0xEE01);

    for (idx = 0; idx < pPipe->numManagedBufs; idx++)
    {
        pSippOPipeBuf managedBuf   = &pPipe->managedBufList[idx];
        u32           bufsToHandle = status & managedBuf->pBuf->hwInputBufId;

        if (bufsToHandle)
        {
            u32 clientIdx;
            fptr =  managedBuf->pFilt;

            SIPP_OPIPE_RT_DBG (idx);
            SIPP_OPIPE_RT_DBG (bufsToHandle);

            // Clear flag - do this before working out how many lines may have
            // been signalled so we don't create timing hole
            SET_REG_WORD(SIPP_INT0_CLEAR_ADR, bufsToHandle);

            retVal |= bufsToHandle;

            // Analyse each entry in turn in the client count map list
            // If it is signalled now, increment the client count by the appropriate amount
            // (the irq rate for the generating filter)

            // Note, not using SIPP_ADD_OPIPE_RT_CHECKS assumes system is able to uniquely service
            // every ibfl interrupt from the same interrupt source - if there was a delay somehow
            // a filter may be able to say decrement my input buffer twice.
            // Only enabling SIPP_ADD_OPIPE_RT_CHECKS can catch that situation
            SIPP_PAL_CRIT_STATE uCS;

            sippPalCriticalSectionBegin (&uCS);

            for (clientIdx = 0x0; clientIdx < managedBuf->numClients; clientIdx++)
            {
                if (managedBuf->clientCountMap[clientIdx] & bufsToHandle)
                {
                    #ifdef SIPP_ADD_OPIPE_RT_CHECKS
                    #ifdef USE_NEW_IBFL
                    // We should read the IBUF.CTX.LI bits and compare with last good read
                    // to establish if we have potentially had > 1 interrupt since we last ran this
                    // check - This can happen if the buffer size is >= 2 * irq rate of the consumer
                    u16 lastLineCount = (u16)(*managedBuf->iBufCtxReg[clientIdx] & 0xFFFF);
                    u16 linesToService;

                    // Rnd this down to a multiple of irq rate power
                    lastLineCount &= (~(managedBuf->clientCountIncr[clientIdx] - 0x1));

                    if (lastLineCount >= managedBuf->lastLineServiced[clientIdx])
                    {
                        linesToService                          = lastLineCount - managedBuf->lastLineServiced[clientIdx];
                        managedBuf->lastLineServiced[clientIdx] = lastLineCount;
                    }
                    else
                    {
                        linesToService                          = fptr->outputH - managedBuf->lastLineServiced[clientIdx];
                        managedBuf->lastLineServiced[clientIdx] = fptr->outputH;
                    }

                    #ifdef SIPP_OPIPE_RT_DBG_ENABLE
                    if (linesToService > (u32)managedBuf->clientCountIncr[clientIdx])
                    {
                        sippPalTrace (SIPP_RUNTIME_TL_CATASTROPHE, "linesToService 0x%04x 0x%04lx %04lx lastLineCount %lu bufIdx %lu, clientIdx %lu\n", linesToService, managedBuf->clientCountIncr[clientIdx], managedBuf->lastLineServiced[clientIdx], lastLineCount, (u32)idx, clientIdx);
                    }
                    #endif
                    SIPP_OPIPE_RT_DBG (linesToService);

                    while (linesToService)
                    {
                        managedBuf->clientCount[clientIdx] += managedBuf->clientCountIncr[clientIdx];
                        linesToService                     -= managedBuf->clientCountIncr[clientIdx];
                    }


                    #else
                    managedBuf->clientCount[clientIdx] += managedBuf->clientCountIncr[clientIdx];
                    #endif
                    #else
                    managedBuf->clientCount[clientIdx] += managedBuf->clientCountIncr[clientIdx];
                    #endif
                }
            }

            // Call decrement check function
            // for simplicity, this function only should trigger calls when all clients have
            // consumed a number of lines equal to the irq rate of the producing filter
            #ifdef SIPP_ADD_OPIPE_RT_CHECKS
            #ifdef USE_NEW_LOOP
            while (sippOpipeIbflDecAllClients (managedBuf))
            #else
            if (sippOpipeIbflDecAllClients (managedBuf))
            #endif
            #else
            if (sippOpipeIbflDecAllClients (managedBuf))
            #endif
            {
                // DMA-in output buffer: DDR->CMX transfer
                if (fptr->nParents == 0)
                {
                    #ifndef SIPP_PC
                    u32        srcAddr       = 0xDEADBEEF;
                    u32        dstAddr       = 0xDEADBEEF;
                    u32        fillStartLine = managedBuf->pBuf->internalFillLevel % fptr->nLines[managedBuf->oBufIdx];
                    DmaParam * param         = (DmaParam *)fptr->params;
                    u32        cdmaRetVal;
                    u32        srcStride;

                    if (managedBuf->pBuf->internalFillLevel < fptr->outputH)
                    {
                        managedBuf->linesNextFill = fptr->outputH - managedBuf->pBuf->internalFillLevel;
                        if (managedBuf->linesNextFill > (1 << fptr->irqRatePow))
                        {
                            managedBuf->linesNextFill = 1 << fptr->irqRatePow;
                        }

                        srcStride = (param->dmaMode == DMA_MODE_PARTIAL_LINE) ? param->extCfg.tPartialCfg.ddrLineStride : fptr->outputW;

                        srcAddr = (u32)(param->ddrAddr + ((managedBuf->pBuf->internalFillLevel * srcStride) << fptr->bytesPerPix));
                        dstAddr = (u32)fptr->outputBuffer[managedBuf->oBufIdx] + ((fillStartLine * fptr->lineStride[managedBuf->oBufIdx]) << fptr->bytesPerPix);

                        #ifdef USE_CMX_DMA_NEW_DRIVER
                        DrvCmxDmaTransactionHnd * dmaH;
                        DrvCmxDmaTransaction    * dmaD    = sippGetNextDmaDsc (fptr,
                                                                               &dmaH);
                        #else
                        dmaTransactionList      * dmaD    = sippGetNextDmaDsc (fptr);
                        #endif
                        SippDmaDesc *        pSippDmaDesc = (SippDmaDesc *)dmaD;

                        dmaD->length = ((fptr->outputW * managedBuf->linesNextFill) << fptr->bytesPerPix); // This should use bpp and calculate properly
                        dmaD->src    = (void *)srcAddr;
                        dmaD->dst    = (void *)dstAddr;

                        managedBuf->pBuf->internalFillLevel += managedBuf->linesNextFill;
                        pSippDmaDesc->linesInTransaction     = managedBuf->linesNextFill;

                        #ifdef USE_CMX_DMA_NEW_DRIVER
                        cdmaRetVal = DrvCmxDmaStartTransfer  (dmaH,
                                                              dmaSourceIrqHandler,
                                                              managedBuf);
                        #else
                        cdmaRetVal = DrvCmxDmaStartTaskAsync (dmaD,
                                                              dmaSourceIrqHandler,
                                                              managedBuf);
                        #endif

                        SIPP_OPIPE_RT_DBG (cdmaRetVal);

                        #ifdef USE_CMX_DMA_NEW_DRIVER
                        if (MYR_DRV_ERROR == cdmaRetVal)
                        #else
                        if (CDMA_TASK_QUEUE_FULL <= cdmaRetVal)
                        #endif
                        {
                            sippPalTrace (SIPP_RUNTIME_TL_CATASTROPHE, "CMDA Error 03\n");
                        }
                    }
                    #endif
                }
                else
                {
                    if (managedBuf->pBuf->internalFillLevel < fptr->outputH)
                    {
                        for (int j = 0; j < (1 << fptr->irqRatePow); j++)
                        {
                            SET_REG_WORD(SIPP_OBFL_DEC_ADR, managedBuf->pBuf->hwOutputBufId);
                        }
                    }
                }
            }
            sippPalCriticalSectionEnd (uCS);
        }
    }

    return retVal;
}

#ifndef SIPP_PC

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    dmaSinkIrqHandler                                                //
//                                                                                //
//  DESCRIPTION: CMX DMA driver transaction complete callback for sink filters    //
//                                                                                //
//  INPUTS:      transHnd - DMA transaction handle                                //
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

#ifdef USE_CMX_DMA_NEW_DRIVER
void dmaSinkIrqHandler (DrvCmxDmaTransactionHnd * transHnd, void * userContext)
#else
void dmaSinkIrqHandler (dmaTransactionList * transHnd, void * userContext)
#endif
{
    u32 i;
    pSippOPipeBuf managedBuf   = (pSippOPipeBuf)userContext;
    // Re-cast transHnd in order to get access to some additional info
    #ifdef USE_CMX_DMA_NEW_DRIVER
    SippDmaDesc * pSippDmaDesc = (SippDmaDesc *)transHnd->head;
    #else
    SippDmaDesc * pSippDmaDesc = (SippDmaDesc *)transHnd;
    #endif
    SippFilter *  ptrFilt      = (SippFilter *)managedBuf->pFilt;

    SIPP_OPIPE_RT_DBG (0xEE03);

    // Increment the completes for all transactions just completed
    while (pSippDmaDesc)
    {
        managedBuf->clientCount[pSippDmaDesc->managedBufClientIdx]   += pSippDmaDesc->linesInTransaction;
        managedBuf->sinkLineCount[pSippDmaDesc->managedBufClientIdx] += pSippDmaDesc->linesInTransaction;

        // Move to next
        #ifdef USE_CMX_DMA_NEW_DRIVER
        pSippDmaDesc = pSippDmaDesc->dmaDsc.link_address;
        #else
        pSippDmaDesc = pSippDmaDesc->dmaDsc.linkAddress;
        #endif
    }

    #ifdef SIPP_ADD_OPIPE_RT_CHECKS
    #ifdef USE_NEW_LOOP
    while (sippOpipeIbflDecAllClients (managedBuf))
    #else
    if (sippOpipeIbflDecAllClients (managedBuf))
    #endif
    #else
    if (sippOpipeIbflDecAllClients (managedBuf))
    #endif
    {
        for(i = 0; i < (u32)(1 << ptrFilt->irqRatePow); i++)
        {
            SET_REG_WORD(SIPP_OBFL_DEC_ADR, managedBuf->pBuf->hwOutputBufId);
        }
    }

    // transHnd may now be a linked list of multiple transactions - reset it to one
    // for the next frame
    #ifdef USE_CMX_DMA_NEW_DRIVER
    transHnd->tail = transHnd->head;
    #else
    transHnd->linkAddress = 0;
    #endif

}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    dmaLastSinkIrqHandler                                            //
//                                                                                //
//  DESCRIPTION: CMX DMA driver transaction complete callback for sink filters    //
//               when carrying out their final transactions for a frame           //
//                                                                                //
//  INPUTS:      transHnd - DMA transaction handle                                //
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

#ifdef USE_CMX_DMA_NEW_DRIVER
void dmaLastSinkIrqHandler (DrvCmxDmaTransactionHnd * transHnd, void * userContext)
#else
void dmaLastSinkIrqHandler (dmaTransactionList * transHnd, void * userContext)
#endif
{
    u32 i;
    pSippOPipeBuf managedBuf   = (pSippOPipeBuf)userContext;
    // Re-cast transHnd in order to get access to some additional info
    #ifdef USE_CMX_DMA_NEW_DRIVER
    SippDmaDesc * pSippDmaDesc = (SippDmaDesc *)transHnd->head;
    #else
    SippDmaDesc * pSippDmaDesc = (SippDmaDesc *)transHnd;
    #endif
    SippFilter *  ptrFilt      = (SippFilter *)managedBuf->pFilt;
    SIPP_PAL_CRIT_STATE uCS;

    SIPP_OPIPE_RT_DBG (0xEE04);

    sippPalCriticalSectionBegin (&uCS);

    // Increment the counts for all transactions just completed
    while (pSippDmaDesc)
    {
        managedBuf->clientCount[pSippDmaDesc->managedBufClientIdx]   += pSippDmaDesc->linesInTransaction;
        managedBuf->sinkLineCount[pSippDmaDesc->managedBufClientIdx] += pSippDmaDesc->linesInTransaction;

        // Move to next
        #ifdef USE_CMX_DMA_NEW_DRIVER
        pSippDmaDesc = pSippDmaDesc->dmaDsc.link_address;
        #else
        pSippDmaDesc = pSippDmaDesc->dmaDsc.linkAddress;
        #endif
    }

    #ifdef SIPP_ADD_OPIPE_RT_CHECKS
    #ifdef USE_NEW_LOOP
    while (sippOpipeIbflDecAllClients (managedBuf))
    #else
    if (sippOpipeIbflDecAllClients (managedBuf))
    #endif
    #else
    if (sippOpipeIbflDecAllClients (managedBuf))
    #endif
    {
        for(i = 0; i < (u32)(1 << ptrFilt->irqRatePow); i++)
        {
            SET_REG_WORD(SIPP_OBFL_DEC_ADR, managedBuf->pBuf->hwOutputBufId);
        }
    }
    sippPalCriticalSectionEnd (uCS);

    // If current sink finished, check for pipe finish
    if (sippOPipeFinished (ptrFilt->pPipe))
    {
        SIPP_OPIPE_RT_DBG (0xEE06);

        ptSippPipelineSuper pLocSPipe = (ptSippPipelineSuper)ptrFilt->pPipe;
        // End of frame callback
        // Could return some stats as the data
        sippEventNotify (ptrFilt->pPipe,
                         eSIPP_PIPELINE_FRAME_DONE,
                         (SIPP_PIPELINE_EVENT_DATA *)0);

        // Remove from the loaded list
        sippHWSessionRemoveLoadedPipe (ptrFilt->pPipe);
        sippHWSessionRemoveActiveLists (ptrFilt->pPipe,
                                        pLocSPipe->uHWPipeID);

    }

    // transHnd may now be a linked list of two transactions - reset it to one
    // for the next frame
    #ifdef USE_CMX_DMA_NEW_DRIVER
    transHnd->tail = transHnd->head;  // Does not work for new multi input case....
                                      // but Will we ever extend the multi-input case to support
                                      // dma sinks in sipp pipelines?
    #else
    transHnd->linkAddress = 0;
    #endif
}

#endif

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOPipeSendDMATransaction                                      //
//                                                                                //
//  DESCRIPTION:                                                                  //
//                                                                                //
//  INPUTS:                                                                       //
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

void sippOPipeSendDMATransaction (SippFilter *  fptr,
                                  SippFilter *  cons,
                                  pSippOPipeBuf managedBuf,
                                  bool          bLastTrans)
{
    #ifdef SIPP_PC
    UNUSED(fptr);
    UNUSED(cons);
    UNUSED(managedBuf);
    #else

    // Check if the transaction is larger than one irq rate - if not there is no need to check
    // for output buffer looping within the transaction
    if ((managedBuf->linesNextFill >> (fptr->irqRatePow + 0x1)) == 0)
    {
        #ifndef SIPP_PC
        u32           srcAddr;
        u32           dstAddr;
        SippDmaDesc * pSippDmaDesc;
        DmaParam *    param = (DmaParam *)cons->params;
        SIPP_PAL_CRIT_STATE uCS;

        sippPalCriticalSectionBegin (&uCS);

        u32 dstStride = (param->dmaMode == DMA_MODE_PARTIAL_LINE) ? param->extCfg.tPartialCfg.ddrLineStride : cons->outputW;

        srcAddr = (u32)fptr->outputBuffer[managedBuf->oBufIdx] + (((managedBuf->pBuf->internalFillLevel % fptr->nLines[managedBuf->oBufIdx]) * fptr->lineStride[managedBuf->oBufIdx]) << fptr->bytesPerPix);
        dstAddr = (u32)param->ddrAddr + ((dstStride * managedBuf->pBuf->internalFillLevel) << cons->bytesPerPix);

        #ifdef USE_CMX_DMA_NEW_DRIVER
        DrvCmxDmaTransaction    * dmaD;
        DrvCmxDmaTransactionHnd * dmaH;

        dmaD = sippGetNextDmaDsc (cons,
                                  &dmaH);
        #else
        dmaTransactionList * dmaD = sippGetNextDmaDsc (cons);
        #endif

        dmaD->length = ((cons->outputW * managedBuf->linesNextFill) << cons->bytesPerPix); // This should use bpp and calculate properly
        dmaD->src    = (void *)srcAddr;
        dmaD->dst    = (void *)dstAddr;

        pSippDmaDesc                     = (SippDmaDesc *)dmaD;
        pSippDmaDesc->linesInTransaction = managedBuf->linesNextFill;

        #ifdef USE_CMX_DMA_NEW_DRIVER
        if (MYR_DRV_ERROR == DrvCmxDmaStartTransfer (dmaH,
                                                     bLastTrans ? dmaLastSinkIrqHandler : dmaSinkIrqHandler,
                                                     managedBuf))
        #else
        if (CDMA_TASK_QUEUE_FULL <= DrvCmxDmaStartTaskAsync (dmaD,
                                                             bLastTrans ? dmaLastSinkIrqHandler : dmaSinkIrqHandler,
                                                             managedBuf))
        #endif
        {
            sippPalTrace (SIPP_RUNTIME_TL_CATASTROPHE, "CMDA Error 05\n");
        }

        sippPalCriticalSectionEnd (uCS);
        #endif
    }
    else
    {
        u32 firstTrans    = 0;
        u32 secondTrans   = 0;
        u32 obufIdx       = managedBuf->oBufIdx;
        u32 fillStartLine = managedBuf->pBuf->internalFillLevel % fptr->nLines[obufIdx];

        u32 srcAddr = 0xDEADBEEF;
        u32 dstAddr = 0xDEADBEEF;

        SIPP_OPIPE_RT_DBG (0xEE08);

        firstTrans = fptr->nLines[obufIdx] - fillStartLine;

        if (firstTrans > managedBuf->linesNextFill)
        {
            firstTrans = managedBuf->linesNextFill;
        }

        if (firstTrans)
        {
            DmaParam *    param     = (DmaParam *)cons->params;
            u32           dstStride = (param->dmaMode == DMA_MODE_PARTIAL_LINE) ? param->extCfg.tPartialCfg.ddrLineStride : cons->outputW;
            SippDmaDesc * pSippDmaDesc;
            SIPP_PAL_CRIT_STATE uCS;

            sippPalCriticalSectionBegin (&uCS);

            srcAddr = (u32)fptr->outputBuffer[obufIdx] + ((fillStartLine * fptr->lineStride[managedBuf->oBufIdx]) << fptr->bytesPerPix);
            dstAddr = (u32)param->ddrAddr + ((managedBuf->pBuf->internalFillLevel * dstStride) << fptr->bytesPerPix);

            #ifdef USE_CMX_DMA_NEW_DRIVER
            DrvCmxDmaTransactionHnd * dmaH0;
            DrvCmxDmaTransaction    * dmaD0 = sippGetNextDmaDsc (cons,
                                                                 &dmaH0);
            #else
            dmaTransactionList *dmaD0   = sippGetNextDmaDsc (cons);
            #endif

            dmaD0->length = ((cons->outputW * firstTrans) << cons->bytesPerPix); // This should use bpp and calculate properly
            dmaD0->src    = (void *)srcAddr;
            dmaD0->dst    = (void *)dstAddr;

            pSippDmaDesc                     = (SippDmaDesc *)dmaD0;
            pSippDmaDesc->linesInTransaction = firstTrans;

            if (firstTrans != managedBuf->linesNextFill)
            {
                // A second transaction is required
                secondTrans   = managedBuf->linesNextFill - firstTrans;

                // Increment src/dst buffers' address for last transfer
                srcAddr  = (u32)fptr->outputBuffer[obufIdx];
                dstAddr += ((dstStride * firstTrans) << fptr->bytesPerPix);

                #ifdef USE_CMX_DMA_NEW_DRIVER
                DrvCmxDmaTransactionHnd * dmaH1;
                DrvCmxDmaTransaction    * dmaD1 = sippGetNextDmaDsc (cons,
                                                                     &dmaH1);
                #else
                dmaTransactionList * dmaD1   = sippGetNextDmaDsc (cons);
                #endif

                dmaD1->length = ((cons->outputW * secondTrans) << cons->bytesPerPix); // This should use bpp and calculate properly
                dmaD1->src    = (void *)srcAddr;
                dmaD1->dst    = (void *)dstAddr;

                pSippDmaDesc                     = (SippDmaDesc *)dmaD1;
                pSippDmaDesc->linesInTransaction = secondTrans;

                #ifdef USE_CMX_DMA_NEW_DRIVER
                DrvCmxDmaLinkTransactions (dmaH0, 2, dmaH0, dmaH1);
                #else
                DrvCmxDmaLinkTasks (dmaD0, 1, dmaD1);
                #endif
            }

            #ifdef USE_CMX_DMA_NEW_DRIVER
            if (MYR_DRV_ERROR == DrvCmxDmaStartTransfer (dmaH0,
                                                         bLastTrans ? dmaLastSinkIrqHandler : dmaSinkIrqHandler,
                                                         managedBuf))
            #else
            if (CDMA_TASK_QUEUE_FULL <= DrvCmxDmaStartTaskAsync (dmaD0,
                                                                 bLastTrans ? dmaLastSinkIrqHandler : dmaSinkIrqHandler,
                                                                 managedBuf))
            #endif
            {
                sippPalTrace (SIPP_RUNTIME_TL_CATASTROPHE, "CMDA Error 00\n");
            }

            sippPalCriticalSectionEnd (uCS);
        }
    }

    #endif
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOpipeHandleObflInc                                           //
//                                                                                //
//  DESCRIPTION: Main handler for output buffer fill level increment interrupt    //
//                                                                                //
//  INPUTS:      pPipe  - Pipeline to consider                                    //
//               status - SIPP INT1 status                                        //
//               eof    - called from an end of frame interrupt                   //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       This is a multi-pipe capable function                            //
//                                                                                //
//  CONTEXT:     ISR context                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippOpipeHandleObflInc (SippPipeline * pPipe, u32 status, u32 eof)
{
    SippFilter *    fptr;
    psSchLineBuffer pBuf;
    u32             idx, consIdx;
    u32             retVal = 0;

    SIPP_OPIPE_RT_DBG (0xEE02);
    SIPP_OPIPE_RT_DBG (eof);
    SIPP_OPIPE_RT_DBG (status);

    for (idx = 0; idx < pPipe->numManagedBufs; idx++)
    {
        pSippOPipeBuf managedBuf   = &pPipe->managedBufList[idx];
        u32           bufsToHandle = status & managedBuf->pBuf->hwOutputBufId;

        if (bufsToHandle)
        {
            fptr = managedBuf->pFilt;
            pBuf = managedBuf->pBuf;

            // Clear flag - do this before working out how many lines may have
            // been signalled so we don't create timing hole
            if (eof == 0)
            {
                SET_REG_WORD(SIPP_INT1_CLEAR_ADR, bufsToHandle);
                retVal |= bufsToHandle;
            }

            #ifdef SIPP_ADD_OPIPE_RT_CHECKS
            #ifdef USE_NEW_OBFL
            // We should read the OBUF.CTX.LO bits and compare with last good read
            // to establish if we have potentially had > 1 interrupt since we last ran this
            // check - This can happen if the buffer size is >= 2 * irq rate of the consumer
            u16 lastLineCount = (u16)(((*managedBuf->oBufCtxReg) >> 16) & 0x3FF) ;
            u32 numLineIncs   = 0;

            if (managedBuf->totalLinesServiced < managedBuf->pFilt->outputH)
            {

                if (lastLineCount > managedBuf->lastOutLineServiced)
                {
                    numLineIncs = (lastLineCount - managedBuf->lastOutLineServiced);
                }
                else if (lastLineCount < managedBuf->lastOutLineServiced)
                {
                    // Wrapped buffer
                    numLineIncs = (pBuf->numLines - managedBuf->lastOutLineServiced) + lastLineCount;
                }
                else
                {
                    // Equal - check OBFL - if we are full - assume we wrapped, if we are empty
                    // this must be a residual interrupt already serviced
                    volatile u32 * obflRegAddr = (volatile u32 *)((u32)managedBuf->oBufCtxReg - 0x4);
                    u32 notEmpty               = *obflRegAddr & 0x3FF;

                    #ifdef SIPP_OPIPE_RT_DBG_ENABLE
                    sippPalTrace (SIPP_RUNTIME_TL_CATASTROPHE, "notEmpty 0x%04x (0x%08lx), managedBuf->totalLinesServiced %lu eof %lu 0x%08lx 0x%08lx\n",
                                                                (u16)notEmpty,
                                                                *obflRegAddr,
                                                                managedBuf->totalLinesServiced,
                                                                eof,
                                                                status,
                                                                managedBuf->pBuf->hwOutputBufId);
                    #endif

                    if (notEmpty)
                    {
                        numLineIncs = pBuf->numLines;
                    }
                }
            }

            // POSSIBLE ENHANCE :  doing this to the algo forces the use of the EOF interrupt to know that we have finished in all circumstances
            // (i.e. when the final incr is not a full set of irq Rate lines)
            // Possibly we should do this after we make a local decision on EOF based on the obfl inc only
            if (eof == 0)
            {
                // Rnd this down to a multiple of irq rate power
                numLineIncs &= (~((0x1 << fptr->irqRatePow) - 0x1));

                #ifdef SIPP_OPIPE_RT_DBG_ENABLE
                if (numLineIncs > (u32)(1 << fptr->irqRatePow))
                {
                    sippPalTrace (SIPP_RUNTIME_TL_CATASTROPHE, "lastLineCount 0x%04x 0x%04x 0x%08lx, numLineIncs %lu\n", lastLineCount, managedBuf->lastOutLineServiced, (u32)(*managedBuf->oBufCtxReg), numLineIncs);
                }
                #endif
            }

            managedBuf->lastOutLineServiced += numLineIncs;
            managedBuf->totalLinesServiced  += numLineIncs;

            if (managedBuf->lastOutLineServiced >= pBuf->numLines)
            {
                managedBuf->lastOutLineServiced -= pBuf->numLines;
            }

            #else
            u32 numLineIncs = (eof) ? (fptr->outputH - (u32)pBuf->internalFillLevel) : (u32)(1 << fptr->irqRatePow);
            #endif
            #else
            u32 numLineIncs = (eof) ? (fptr->outputH - (u32)pBuf->internalFillLevel) : (u32)(1 << fptr->irqRatePow);
            #endif

            // Adding this check for full belt and braces approach
            // in case we also manmage to get a numLineIncs == 0 in all this
            if (numLineIncs)
            {
                for (consIdx = 0; consIdx < fptr->nCons; consIdx++)
                {
                    SippFilter * cons = fptr->cons[consIdx];

                    // Consumer is a DMA out buffer: CMX->DDR transfer
                    // Also check DMA consumer is actually consuming from the buffer under inspection
                    if ((cons->nCons == 0) &&
                        (pBuf == cons->iBufs[0x0]))
                    {
                        bool bLastTrans           = FALSE;
                        managedBuf->linesNextFill = 0;

                        // Check if data is still to be fed
                        if (pBuf->internalFillLevel < fptr->outputH)
                        {
                            managedBuf->linesNextFill = fptr->outputH - pBuf->internalFillLevel;

                            if (managedBuf->linesNextFill > (numLineIncs))
                            {
                                managedBuf->linesNextFill = numLineIncs;
                            }
                            else if (managedBuf->linesNextFill + pBuf->internalFillLevel == fptr->outputH)
                            {
                                bLastTrans = TRUE;
                            }
                        }

                        // Whether this is the last transaction or not, previous code was predicated on the fact that
                        // we would only ever send a length of irq rate max (i.e. managedBuf->linesNextFill would be at most
                        // 1 << fptr->irqRatePow). This meant we could avoid checking for loops in the buffer - but now we
                        // are checking for missed interruts so the number of lines could be a a greater than one multiple of
                        // 1 <<  fptr->irqRatePow - in that situation we must check for loops...
                        if (managedBuf->linesNextFill)
                        {
                            sippOPipeSendDMATransaction (fptr,
                                                         cons,
                                                         managedBuf,
                                                         bLastTrans);
                        }
                    }
                }

                if (pBuf->internalFillLevel < fptr->outputH)
                {
                    // Buffer between HW filters: increment the other HW filter's input buffer level
                    if (pBuf->hwInputBufId)
                    {
                        for(u32 j = 0; j < numLineIncs; j++)
                        {
                            SET_REG_WORD(SIPP_IBFL_INC_ADR, pBuf->hwInputBufId);
                        }
                    }

                    pBuf->internalFillLevel += numLineIncs;
                }
            }
        }
    }

    return retVal;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOPipeEofIsr                                                  //
//                                                                                //
//  DESCRIPTION: End of frame ISR                                                 //
//                                                                                //
//  INPUTS:      irqSrc                                                           //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       This is a multi-pipe capable function                            //
//                                                                                //
//  CONTEXT:     ISR context                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippOPipeEofIsr (SippPipeline * pPipe,
                      u32            status)
{
    sippOpipeHandleObflInc (pPipe, status, 0x1);
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOPipeRuntimeProcessIters                                     //
//                                                                                //
//  DESCRIPTION: Load pipeline to runtime and HW                                  //
//                                                                                //
//  INPUTS:      pPipe - pipeline to 'load' to runtime                            //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:     Thread context                                                   //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippOPipeRuntimeProcessIters (SippPipeline *pPipe, u32 numIters)
{
    u32                 filtIdx;
    SippFilter  *       fptr;
    pSippOPipeBuf       managedBuf;
    SIPP_PAL_CRIT_STATE uCS;

    UNUSED (numIters);

    for (filtIdx = 0; filtIdx < pPipe->nFiltersDMA; filtIdx++)
    {
        fptr = pPipe->filtersDMA[filtIdx];

        // DMA-in case: DDR->CMX transaction
        if (fptr->nParents == 0)
        {
            #ifndef SIPP_PC
            u32        srcAddr;
            u32        dstAddr;
            u32        transLength;
            DmaParam * param = (DmaParam *)fptr->params;

            srcAddr     = (u32)param->ddrAddr;
            dstAddr     = (u32)fptr->outputBuffer[0x0];
            transLength = ((fptr->outputW * fptr->nLines[0x0]) << fptr->bytesPerPix);

            // First run: fill in whole input buffer
            managedBuf = fptr->oBufs[0]->pManagedBuf;
            managedBuf->linesNextFill = fptr->nLines[0x0];

			sippPalCriticalSectionBegin (&uCS);

            #ifdef USE_CMX_DMA_NEW_DRIVER
            DrvCmxDmaTransactionHnd * dmaH;
            DrvCmxDmaTransaction    * dmaD   = sippGetNextDmaDsc (fptr,
                                                                  &dmaH);
            #else
            dmaTransactionList *dmaD         = sippGetNextDmaDsc (fptr);
            #endif
            SippDmaDesc *       pSippDmaDesc = (SippDmaDesc *)dmaD;

            pSippDmaDesc->linesInTransaction = managedBuf->linesNextFill;

            dmaD->length = transLength;
            dmaD->src    = (void *)srcAddr;
            dmaD->dst    = (void *)dstAddr;

            managedBuf->pBuf->internalFillLevel += managedBuf->linesNextFill;

            #ifdef USE_CMX_DMA_NEW_DRIVER
            if (MYR_DRV_ERROR == DrvCmxDmaStartTransfer (dmaH, dmaSourceIrqHandler, managedBuf))
            #else
            if (CDMA_TASK_QUEUE_FULL <= DrvCmxDmaStartTaskAsync (dmaD, dmaSourceIrqHandler, managedBuf))
            #endif
            {
                sippPalTrace (SIPP_RUNTIME_TL_CATASTROPHE, "CMDA Error 01\n");
            }

            sippPalCriticalSectionEnd (uCS);

            #endif
        }
    }
}

///////////////////////////////////////////////////////////////////////
// Public API
//
// The following functions are the common hook functions to be implemented by all
// compatible runtimes called by the session controller

void sippOPipeRuntimeClaimHWResource (pSippPipeline pPipe)
{
    u32 regVal, intMask0, intMask12, idx;
    SIPP_PAL_CRIT_STATE uCS;

    // Loop thru managed buffer list
    for (idx = 0, intMask0 = 0, intMask12 = 0; idx < pPipe->numManagedBufs; idx++)
    {
        pSippOPipeBuf managedBuf   = &pPipe->managedBufList[idx];

        intMask0  |= managedBuf->pBuf->hwInputBufId;
        intMask12 |= managedBuf->pBuf->hwOutputBufId;

    }

    sippPalCriticalSectionBegin (&uCS);

    sippGlobalOBFLIncStatus &= ~intMask12;

    SET_REG_WORD(SIPP_INT0_CLEAR_ADR, intMask0);
    SET_REG_WORD(SIPP_INT1_CLEAR_ADR, intMask12);
    SET_REG_WORD(SIPP_INT2_CLEAR_ADR, intMask12);

    regVal = GET_REG_WORD_VAL(SIPP_INT0_ENABLE_ADR);
    SET_REG_WORD(SIPP_INT0_ENABLE_ADR, (regVal | intMask0));

    regVal = GET_REG_WORD_VAL(SIPP_INT1_ENABLE_ADR);
    SET_REG_WORD(SIPP_INT1_ENABLE_ADR, (regVal | intMask12));

    regVal = GET_REG_WORD_VAL(SIPP_INT2_ENABLE_ADR);
    SET_REG_WORD(SIPP_INT2_ENABLE_ADR, (regVal | intMask12));

    sippPalCriticalSectionEnd (uCS);
}


void sippOPipeRuntime (pSippPipeline                    pPipe,
                       eSIPP_ACCESS_SCHEDULER_EVENT     eEvent,
                       SIPP_ACCESS_SCHEDULER_EVENT_DATA pData)
{
    switch (eEvent)
    {
        case eSIPP_ACCESS_SCHEDULER_CMD_PROCESS_ITERS :
        {
            // Copy over the number of iterations
            u32 uNumIters = *(u32 *)pData;

            sippOPipeRuntimeProcessIters (pPipe,
                                          uNumIters);

        } break;
        default : break;
    }
}

////////////////////////////////////////////
// Async runtime servicing functions

u32 sippOPipeRunIterDone (SippPipeline * pPipe)
{
    UNUSED (pPipe);

    return 0;
}

void sippOPipeRunNextIter (pSippPipeline pPipe)
{
    UNUSED (pPipe);
}

#endif /* SIPP_SCRT_ENABLE_OPIPE */
