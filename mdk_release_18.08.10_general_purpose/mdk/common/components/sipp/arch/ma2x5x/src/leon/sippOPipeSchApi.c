///
/// @file      sippOPipeSchedApi.c
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP framework API
///            Platform(s) supported : MA2x5x
///

#ifdef SIPP_SCRT_ENABLE_OPIPE

////////////////////////////////////////////////////
// Header files
////////////////////////////////////////////////////

#include <sipp.h>
#include <sippInternal.h>

////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOPipeGetLLBcapacity                                          //
//                                                                                //
//  DESCRIPTION: Gets LLB capacity for filters that have an LLB                   //
//                                                                                //
//  INPUTS:      pipeLine -                                                       //
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

u32 sippOPipeGetLLBcapacity (SippFilter * ptrFilt)
{
    u32 LLBcapacity = 0;

    switch (ptrFilt->unit)
    {
        case SIPP_SIGMA_ID:
            LLBcapacity = SIGMA_KERNEL_SIZE; break;

        case SIPP_RAW_ID:
            LLBcapacity = RAW_KERNEL_SIZE; break;

        case SIPP_DBYR_ID:
            LLBcapacity = DBYR_KERNEL_SIZE; break;

        case SIPP_DOGL_ID:
            LLBcapacity = DOGLTM_KERNEL_SIZE; break;

        case SIPP_LUMA_ID:
            LLBcapacity = LUMA_REF_KERNEL_SIZE; break;

        case SIPP_SHARPEN_ID:
            LLBcapacity = SHARPEN_KERNEL_SIZE; break;

        case SIPP_CGEN_ID:
            LLBcapacity = GEN_CHROMA_KERNEL_SIZE; break;

        case SIPP_MED_ID:
            LLBcapacity = MED_KERNEL_SIZE; break;

        case SIPP_CHROMA_ID:
            LLBcapacity = CHROMA_V_KERNEL_SIZE; break;

        case SIPP_CC_ID:
            LLBcapacity = CC_CHROMA_KERNEL_SIZE; break;
    }

    return LLBcapacity;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOPipeSetBufLatencies                                         //
//                                                                                //
//  DESCRIPTION: Allocate and populate temporary scheudling structs for each      //
//               filter detailing the latency of their outputs - that is the      //
//               expected number of lines that will be fed into the head of the   //
//               pipeline before a line is expected in the output buffer          //
//                                                                                //
//  INPUTS:      pipeLine - The pipeline under execution                          //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     0x0 if success - 1 if processing / allocation failure            //
//                                                                                //
//  NOTES:       Allocate from vPoolScheduleTemp as this info is ONLY needed in   //
//               order to size the buffers in the situation where the oPipe       //
//               scheduler / runtime is chosen and therefore no formal schedule   //
//               is created                                                       //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippOPipeSetBufLatencies (pSippPipeline pipeLine)
{
    SippFilter    * ptrFilt;
    u32             i, worstInputLatency, parentIdx, bufIdx;
    bool            progressThisLoop = TRUE;

    // Allocate the strucutres we need from the scheduling (temporary) virtual pool
    for (i = 0; i < pipeLine->nFilters; i++)
    {
        ptrFilt            = pipeLine->filters[i];
        ptrFilt->pOpipeSch = (SippFilterSchedInfo *)sippMemAlloc (&pipeLine->tHeapMCB,
                                                                  vPoolScheduleTemp,
                                                                  sizeof(SippFilterSchedInfo));

        if (ptrFilt->pOpipeSch == (SippFilterSchedInfo *)NULL)
        {
            sippError (ptrFilt->errorStatus, E_OUT_OF_MEM );
            sippError (pipeLine->errorStatus, E_OUT_OF_MEM );
            return 0x1;
        }

        sippPalMemset (ptrFilt->pOpipeSch, 0, sizeof(SippFilterSchedInfo));

        if (ptrFilt->linesPerIter == 0)
        {
            ptrFilt->pOpipeSch->subSampleScale = 0x1;
        }

        for (parentIdx = 0; parentIdx < ptrFilt->nParents; parentIdx++)
        {
            ptrFilt->pOpipeSch->nlinesUsedParent[parentIdx] = (ptrFilt->iBufCtx[parentIdx]->numLinesUsed >> 1);

            if (ptrFilt->pOpipeSch->subSampleScale)
            {
                ptrFilt->pOpipeSch->nlinesUsedParent[parentIdx] <<= 0x1;
            }
        }
    }

    // Need latencies for all buffers (allocated and non-allocated - otherwise how do we get the Sharpen-> color_comb buffer correct without
    // knowing the latency at the unallocated chroma denoise boundary)
    // We can take advantage of the OSE latency also where we can (as that already has some subsample scaling methodolgy employed)

    do
    {
        progressThisLoop = FALSE;

        for (i = 0; i < pipeLine->nFilters; i++)
        {
            ptrFilt = pipeLine->filters[i];

            if (ptrFilt->pOpipeSch->latenciesKnown == FALSE)
            {
                ptrFilt->pOpipeSch->parentLatenciesKnown = TRUE;

                // Check all parent latencies known
                for (parentIdx = 0, worstInputLatency = 1; parentIdx < ptrFilt->nParents; parentIdx++)
                {
                    if (ptrFilt->parents[parentIdx]->pOpipeSch->latenciesKnown == FALSE)
                    {
                        ptrFilt->pOpipeSch->parentLatenciesKnown = FALSE;
                    }
                    else
                    {
                        u32 parentOBufIdx = ptrFilt->parentOBufIdx[parentIdx];
                        u32 oBufLatency   = ptrFilt->parents[parentIdx]->pOpipeSch->oBufLatency[parentOBufIdx];

                        if ((oBufLatency + ptrFilt->pOpipeSch->nlinesUsedParent[parentIdx])
                            > worstInputLatency)
                        {
                            worstInputLatency = oBufLatency + ptrFilt->pOpipeSch->nlinesUsedParent[parentIdx];
                        }
                    }
                }

                if (ptrFilt->pOpipeSch->parentLatenciesKnown)
                {
                    // Set our own latencies
                    // What is the worst case latency + num lines used fir the current filter
                    for (bufIdx = 0; bufIdx < ptrFilt->numOBufs; bufIdx++)
                    {
                        ptrFilt->pOpipeSch->oBufLatency[bufIdx] = worstInputLatency + (ptrFilt->OBufProdLatency[bufIdx] << ptrFilt->pOpipeSch->subSampleScale);
                        if (ptrFilt->oBufs[bufIdx])
                        {
                            if (ptrFilt->oBufs[bufIdx]->allocReq) ptrFilt->pOpipeSch->hasBuffers = TRUE;
                        }
                    }

                    ptrFilt->pOpipeSch->latenciesKnown = TRUE;
                    progressThisLoop                   = TRUE;
                }
            }
        }

    } while (progressThisLoop);

    #if defined RT_DBG || defined SIPP_PC
    for (i = 0; i < pipeLine->nFilters; i++)
    {
        ptrFilt = pipeLine->filters[i];

        if (ptrFilt->pOpipeSch->latenciesKnown == FALSE)
        {
            sippError (pipeLine->errorStatus, E_RUN_DON_T_KNOW);
            return 0x1;
        }
    }
    #endif

    return 0x0;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOPipeAllocBuffers                                            //
//                                                                                //
//  DESCRIPTION: Allocate all line buffers in the system                          //
//                                                                                //
//  INPUTS:      pipeLine -                                                       //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     0x0 if success - 1 if processing / allocation failure            //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippOPipeAllocBuffers (pSippPipeline pipeLine)
{
    SippFilter    * ptrFilt, * ptrCons, * parForCons, *ptrWorstCons;
    SippFilter    * oBufConsList[SIPP_FILTER_MAX_OBUFS][SIPP_FILTER_MAX_CONSUMERS];
    u32             consBufIdx[SIPP_FILTER_MAX_OBUFS];
    u32             i, j, consIdx, oBufIdx, parIdx, filterOBufIdx, inBufferLatency, bufferLatency;
    u32             worstConsumerLatency, consumerLatency, LLBcapacity = 0;
    u32             nLines = 0;

    // Now check for all buffers which need to be allocated
    // Using the worst case latencies of the input buffers of all consumers
    // as well as the vertical kernel sizes, LLB sizes and IRQ_RATE
    // calculate
    for (i = 0; i < pipeLine->nFilters; i++)
    {
        ptrFilt = pipeLine->filters[i];

        // For every output buffer which needs allocated
        if (ptrFilt->pOpipeSch->hasBuffers)
        {
            ptrWorstCons = (SippFilter *)NULL;

            // Reset consBufIdx[SIPP_FILTER_MAX_OBUFS];
            for (j = 0; j < SIPP_FILTER_MAX_OBUFS; j++)
            {
                consBufIdx[j] = 0;
            }

            // Sort the consumers into a list of consumers for each oBuf
            for (consIdx = 0; consIdx < ptrFilt->nCons; consIdx++)
            {
                // Find the consumer iBuf in the oBuf list of the current filter
                ptrCons = ptrFilt->cons[consIdx];
                oBufIdx = ptrCons->parentOBufIdx[ptrFilt->consIbufIdx[consIdx]];
                //consOBuf = ptrCons->iBufs[ptrFilt->consIbufIdx[consIdx]];

                // Add the consumer to the list of consumers for this oBuf
                oBufConsList[oBufIdx][consBufIdx[oBufIdx]++] = ptrCons;

                if (oBufIdx == ptrFilt->numOBufs)
                {
                    sippError (pipeLine->errorStatus, E_DATA_NOT_FOUND);
                }
            }

            // Compute optimal buffer size for each oBuf based on latencies,
            // vertical kernel sizes, LLB capacities and IRQ_RATE
            for (oBufIdx = 0; oBufIdx < ptrFilt->numOBufs; oBufIdx++)
            {
                if (ptrFilt->oBufs[oBufIdx]->allocReq)
                {
                    worstConsumerLatency = 1;

                    for (consIdx = 0; consIdx < consBufIdx[oBufIdx]; consIdx++)
                    {
                        consumerLatency = 1;
                        ptrCons         = oBufConsList[oBufIdx][consIdx];

                        // Get the latency of all the buffers this consumer filter consumes
                        for (parIdx = 0; parIdx < ptrCons->nParents; parIdx++)
                        {
                            parForCons      = ptrCons->parents[parIdx];
                            filterOBufIdx   = ptrCons->parentOBufIdx[parIdx];
                            inBufferLatency = parForCons->pOpipeSch->oBufLatency[filterOBufIdx];

                            bufferLatency   = inBufferLatency + (ptrCons->pOpipeSch->nlinesUsedParent[parIdx] << 1);

                            if (bufferLatency > consumerLatency)
                            {
                                consumerLatency = bufferLatency;
                            }
                        }

                        if (consumerLatency > worstConsumerLatency)
                        {
                            worstConsumerLatency = consumerLatency;
                            ptrWorstCons         = ptrCons;
                        }
                    }

                    // So we know the worst case consumer latency for this filter
                    nLines      = worstConsumerLatency - ptrFilt->pOpipeSch->oBufLatency[oBufIdx];
                    if (ptrWorstCons)
                    {
                        LLBcapacity = sippOPipeGetLLBcapacity (ptrWorstCons);
                    }

                    if (nLines > LLBcapacity) nLines -= LLBcapacity;
                    else                      nLines  = 0;

                    nLines += ((1 << ptrFilt->irqRatePow) * 3);
                    nLines &= (~((1 << ptrFilt->irqRatePow) - 1));
                    ptrFilt->oBufs[oBufIdx]->numLines = nLines;
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOPipeAllocCdmaDescs                                          //
//                                                                                //
//  DESCRIPTION: Allocate all line buffers in the system                          //
//                                                                                //
//  INPUTS:      pipeLine -                                                       //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     0x0 if success - 1 if processing / allocation failure            //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippOPipeAllocCdmaDescs (pSippPipeline pipeLine)
{
    u32          dmaFiltIdx;
    DmaParam *   param;
    SippFilter * ptrFilt;

    for (dmaFiltIdx = 0; dmaFiltIdx < pipeLine->nFiltersDMA; dmaFiltIdx++)
    {
        ptrFilt = pipeLine->filtersDMA[dmaFiltIdx];

        param   = (DmaParam *)ptrFilt->params;

        param->pTransList = (DMATransDesc *)sippMemAlloc (&pipeLine->tHeapMCB,
                                                          vPoolCMXDMADesc,
                                                          sizeof(DMATransDesc));

        if (param->pTransList == (DMATransDesc *)NULL)
        {
            sippError (ptrFilt->errorStatus, E_OUT_OF_MEM);
            return 0x1;
        }
    }

    return 0x0;
}

#ifndef SIPP_PC

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOPipeInitCdmaDescs                                           //
//                                                                                //
//  DESCRIPTION: Initialise the cdma descriptors allocated to the pipe with the   //
//               information which will be static over the course of execution    //
//                                                                                //
//  INPUTS:      pipeLine - Pipeline whose descriptors are to be initialised      //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     0x0 if successful, 0x1 if processing error                       //
//                                                                                //
//  NOTES:       CDMA driver transaction handles are setup by this function       //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippOPipeInitCdmaDescs (pSippPipeline pipeLine, u32 reschedPipe)
{
    u32           dmaFiltIdx, dmaTransIdx, parentIdx, managedBufClientIdx;
    DmaParam *    param;
    SippFilter *  ptrFilt;

    for (dmaFiltIdx = 0; dmaFiltIdx < pipeLine->nFiltersDMA; dmaFiltIdx++)
    {
        ptrFilt = pipeLine->filtersDMA[dmaFiltIdx];

        param   = (DmaParam *)ptrFilt->params;

        for (dmaTransIdx = 0; dmaTransIdx < SIPP_NUM_DESCS_PER_CDMA; dmaTransIdx++)
        {
            #ifdef USE_CMX_DMA_NEW_DRIVER
            DrvCmxDmaTransactionHnd * dmaH = &param->pTransList->dmaHnd[dmaTransIdx];
            DrvCmxDmaTransaction    * dmaD = &param->pTransList->sippDmaDsc[dmaTransIdx].dmaDsc;
            #else
            dmaTransactionList * dmaD   = &param->pTransList->sippDmaDsc[dmaTransIdx].dmaDsc;
            #endif
            u32                  totalBits;

            // DDR to CMX
            if (ptrFilt->nParents == 0)
            {
                totalBits       = ptrFilt->lineStride[0]  * ptrFilt->bpp[0];
                totalBits      += 0x7;
                totalBits      &= 0xFFFFFFF8;
                dmaD->dst_stride       = totalBits >> 3;                                  // dst in CMX, thus chuked
                dmaD->dst_plane_stride = ptrFilt->oBufs[0]->numLines * dmaD->dst_stride;

                if (param->dmaMode == DMA_MODE_PARTIAL_LINE)
                {
                    totalBits       = param->extCfg.tPartialCfg.ddrLineStride * ptrFilt->bpp[0];
                }
                else
                {
                    totalBits       = ptrFilt->outputW * ptrFilt->bpp[0];
                }
                totalBits      += 0x7;
                totalBits      &= 0xFFFFFFF8;
                dmaD->src_stride   = totalBits >> 3;                                      // src in DDR: thus full line width
                if (param->dmaMode == DMA_MODE_PARTIAL_LINE)
                {
                    totalBits       = (param->extCfg.tPartialCfg.ddrLineStride * ptrFilt->outputH) * ptrFilt->bpp[0];
                }
                else
                {
                    totalBits       = (ptrFilt->outputW * ptrFilt->outputH) * ptrFilt->bpp[0];
                }
                totalBits      += 0x7;
                totalBits      &= 0xFFFFFFF8;
                dmaD->src_plane_stride   = totalBits >> 3;                                // src in DDR, pl stride = full image plane stride
                dmaD->src_width = (ptrFilt->outputW * ptrFilt->bpp[0]) >> 3;              // line_size
                dmaD->dst_width = dmaD->src_width;
            }
            // CMX to DDR
            else if (ptrFilt->nCons == 0)
            {
                SippDmaDesc * pSippDMA = (SippDmaDesc *)dmaD;

                // Note: Given a CONSUMER-DMA filter doesn't get an output buffer, thus ptrFilt->nLines = 0,
                //       so use ptrFilt->parents[0]->oBufs[0]->numLines instead

                if (param->dmaMode == DMA_MODE_PARTIAL_LINE)
                {
                    totalBits       = param->extCfg.tPartialCfg.ddrLineStride * ptrFilt->bpp[0];
                }
                else
                {
                    totalBits       = ptrFilt->outputW * ptrFilt->bpp[0];
                }
                totalBits      += 0x7;
                totalBits      &= 0xFFFFFFF8;
                dmaD->dst_stride  = totalBits >> 3;                                       // dst in DDR, thus line width = full line width
                if (param->dmaMode == DMA_MODE_PARTIAL_LINE)
                {
                    totalBits       = (param->extCfg.tPartialCfg.ddrLineStride * ptrFilt->outputH) * ptrFilt->bpp[0];
                }
                else
                {
                    totalBits       = (ptrFilt->outputW * ptrFilt->outputH) * ptrFilt->bpp[0];
                }
                totalBits      += 0x7;
                totalBits      &= 0xFFFFFFF8;
                dmaD->dst_plane_stride  = totalBits >> 3;                                 // src in DDR, pl stride = full image plane stride
                totalBits       = ptrFilt->parents[0]->lineStride[0] * ptrFilt->bpp[0];
                totalBits      += 0x7;
                totalBits      &= 0xFFFFFFF8;
                dmaD->src_stride   = totalBits >> 3;                                      // src in CMX, thus chunked
                dmaD->src_plane_stride       =  ptrFilt->parents[0]->oBufs[0]->numLines * dmaD->src_stride;
                dmaD->src_width = (ptrFilt->parents[0]->outputW * ptrFilt->bpp[0]) >> 3;  // line_size
                dmaD->dst_width = dmaD->src_width;

                if (reschedPipe == false)
                {
                    if (dmaTransIdx == 0)
                    {
                        // Now we need to fill in an entry managedBufClientIdx which tells us ewhich

                        // Do we want to support multiple parents yet for DMAs?
                        // Lets assume yes at this stage
                        for (parentIdx = 0x0; parentIdx < ptrFilt->nParents; parentIdx++)
                        {
                            u32             oBufIdx;
                            psSchLineBuffer oBuf       = ptrFilt->iBufs[parentIdx];
                            SippFilter *    parentFilt = ptrFilt->parents[parentIdx];

                            for (oBufIdx = 0; oBufIdx < parentFilt->numOBufs; oBufIdx++)
                            {
                                if (parentFilt->oBufs[oBufIdx] == oBuf)
                                {
                                    if (parentFilt->oBufs[oBufIdx]->manageReq)
                                    {
                                        // Look for the DMA filter in this list
                                        for (managedBufClientIdx = 0; managedBufClientIdx < parentFilt->oBufs[oBufIdx]->pManagedBuf->numClients; managedBufClientIdx++)
                                        {
                                            if ((SippFilter *)parentFilt->oBufs[oBufIdx]->pManagedBuf->clientCountMap[managedBufClientIdx] == ptrFilt)
                                            {
                                                pSippDMA->managedBufClientIdx = managedBufClientIdx;
                                                parentFilt->oBufs[oBufIdx]->pManagedBuf->clientCountMap[managedBufClientIdx] = (0x1 << SIPP_DMA_ID);
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        // Copy the details
                        pSippDMA->managedBufClientIdx = managedBufClientIdx;
                    }
                }

                pSippDMA->pFilt = ptrFilt;
            }

            #ifdef USE_CMX_DMA_NEW_DRIVER
            // Initialise all transactions to fixed set of parameters.
            // The src, dst and size parameters may be updated by the runtime
            // prior to use but set non-zero so driver actually creates transaction!
            DrvCmxDmaCreateStrideTransaction (dmaH,
                                              dmaD,
                                              (uint8_t *)0x10, // src - 0x1
                                              (uint8_t *)0x10, // dst
                                              dmaD->src_width,
                                              dmaD->dst_width,
                                              dmaD->src_stride,
                                              dmaD->dst_stride,
                                              0x1) ;// size;

            dmaD->planes_no = ptrFilt->nPlanes[0] - 1;
            #else
            DrvCmxDmaTransactionBriefInit (DrvCmxDmaInitRequesterOnAgent(1, 0), //DrvCmxDmaInitRequester(SIPP_OPIPE_CDMA_AGENT_NO),
                                           DMA_2D_TRANSACTION,
                                           dmaD);
            dmaD->no_planes = ptrFilt->nPlanes[0] - 1;
            #endif
        }

        param->pTransList->curDsc = 0;

    }
}

#endif

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOPipeCheckSize                                               //
//                                                                                //
//  DESCRIPTION: Check if buffer sizes are acceptable - each buffer must fit in   //
//               a slice - if too large reduce IRQ rate control                   //
//                                                                                //
//  INPUTS:      pipeLine - Pipeline                                              //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       0x0 if success, 0x1 if too large                                 //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippOPipeCheckSize (pSippPipeline pipeLine)
{
    SippFilter * fptr;
    u32          i, o, size, totalSize;
    u32          numSlices     = pipeLine->gi.sliceLast - pipeLine->gi.sliceFirst + 0x1;
    bool         bComplete     = FALSE;
    u32          totalAvailMem = (pipeLine->gi.sliceSize * numSlices);

    if (pipeLine->useCmxRegMap)
    {
        totalAvailMem += pipeLine->pCmxMap->totalMem;
    }

    while (bComplete == FALSE)
    {
        for (i = 0, totalSize = 0; i < pipeLine->nFilters; i++)
        {
            fptr = pipeLine->filters[i];

            for (o = 0; o < fptr->numOBufs; o++)
            {
                // Check size for every output buffer which needs allocated
                if (fptr->oBufs[o]->allocReq)
                {
                    if (fptr->flags & SIPP_SHIFT_PLANES)
                    {
                        size  = fptr->lineStride[o] * fptr->oBufs[o]->numLines * fptr->bpp[o];
                    }
                    else
                    {
                        size  = fptr->lineStride[o] * fptr->oBufs[o]->numLines * fptr->nPlanes[o] * fptr->bpp[o];
                    }

                    size += 0x7;
                    size &= 0xFFFFFFF8;

                    totalSize += (size >> 3);
                }
            }
        }

        if (totalSize > totalAvailMem)
        {
            bool bModsMade = FALSE;

            for (i = 0; i < pipeLine->nFilters; i++)
            {
                fptr = pipeLine->filters[i];
                if (fptr->irqRatePow > 1)
                {
                    fptr->irqRatePow--;
                    bModsMade = TRUE;
                }
            }

            if (bModsMade)
            {
                sippOPipeAllocBuffers (pipeLine);
            }
            else
            {
                sippError (pipeLine->errorStatus, E_OUT_OF_MEM);
                return 0x1;
            }
        }
        else
        {
            bComplete = TRUE;
        }
    }

    return 0x0;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOPipeCheckSizePrecomSched                                    //
//                                                                                //
//  DESCRIPTION: Sanity checks on precompiled settings for line buffer sizes      //
//                                                                                //
//  INPUTS:      pipeLine - Pipeline                                              //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       0x0 if success, 0x1 if checks fail                               //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippOPipeCheckSizePrecomSched (pSippPipeline pPipe)
{
    SippFilter * fptr, *cons;
    u32 i, o, c;
    u32 linesUsed = 0;

    for (i = 0; i < pPipe->nFilters; i++)
    {
        fptr = pPipe->filters[i];

        for (o = 0; o < fptr->numOBufs; o++)
        {
            // Check size for every output buffer which needs allocated
            if (fptr->oBufs[o]->allocReq)
            {
                // Buffer size needs to be bigger than irq rate control
                if ((int)fptr->oBufs[o]->numLines < (1 << fptr->irqRatePow))
                {
                    sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "sippOPipeSchedule : Buffer size for filter %u too small\n", fptr->unit);
                    return 0x1;
                }
                // Buffer size needs to be bigger than what consumers need
                for (c = 0; c < fptr->nCons; c++)
                {
                    cons      = fptr->cons[c];
                    linesUsed = cons->nLinesUsed[fptr->consIbufIdx[c]];

                    if (fptr->oBufs[o]->numLines < linesUsed)
                    {
                        sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "sippOPipeSchedule : Buffer size for filter %u too small\n", fptr->unit);
                        return 0x1;
                    }
                }
            }
        }
    }

    return 0x0;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOPipeUpdateMgdBufCountIncrs                                  //
//                                                                                //
//  DESCRIPTION: Now all sizes and irq rates have been set, the client count      //
//               increment (how many lines to incrememt for each isr may be       //
//               filled in)                                                       //
//                                                                                //
//  INPUTS:      pipeLine - Pipeline                                              //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       At creation time fo rth emanged buffer, while all the filters    //
//               are being added to its list of clients all the cross-referncing  //
//               info is already available to map a client count increment        //
//               Unfortunately at that point, the irq rates for each filter are   //
//               not finalised. So in order to avoid going through the relatively //
//               complex process of identifying the filter clients for each       //
//               managed buffer again, sippAnalysePipe2x5x simply leaves behind a //
//               pointer to where the desired data will eventually be held in     //
//               place of the clientCountIncr. That pointer is dereferenced here  //
//               so that this simple function may be called to update the data    //
//               once each buffer's irq rate has been finalised                   //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippOPipeUpdateMgdBufCountIncrs (pSippPipeline pipeLine)
{
    u32 managedBufIdx;
    u32 clientIdx;

    for (managedBufIdx = 0; managedBufIdx < pipeLine->numManagedBufs; managedBufIdx++)
    {
        pSippOPipeBuf pManagedBuf = &pipeLine->managedBufList[managedBufIdx];

        for (clientIdx = 0; clientIdx < pManagedBuf->numClients; clientIdx++)
        {
            u8 irqRatePow2 = (*(u8 *)(pManagedBuf->clientCountIncr[clientIdx]));
            pManagedBuf->clientCountIncr[clientIdx] = (0x1 << irqRatePow2);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// Scheduler interface API

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOPipeSchedule                                                //
//                                                                                //
//  DESCRIPTION: Main API function for framework to cause creation of a schedule  //
//               suitable for a runtime to follow to process one frame on the     //
//               specified pipeline                                               //
//                                                                                //
//  INPUTS:      pipeLine - Pipeline whose per frame operation is to scheduled    //
//               allocMem - Set TRUE to allocate space for the runtime schedule   //
//                          Otherwsie assuem this space is already available      //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     0x0 if successful, 0x1 if processing error                       //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippOPipeSchedule (pSippPipeline pipeLine, bool allocMem, bool reschedPipe)
{
    if (allocMem)
    {
        sippOPipeSetBuffLineStrides (pipeLine);

        if (!sippUsingPrecompSched(pipeLine))
        {
            // First set buffer latencies for each buffer in the system
            if (sippOPipeSetBufLatencies (pipeLine))
            {
                return 0x1;
            }

            if (reschedPipe == false)
            {
                // Call sippOPipeAllocBuffers to change the line buffer sizes from BUFF_HUGE_SZ to the
                // minimum that the output buffers need to hold
                sippOPipeAllocBuffers (pipeLine);
            }

            // Check if buffer sizes are acceptable - if too large consider reducing IRQ_RATE and
            // trying again
            if (sippOPipeCheckSize (pipeLine))
            {
                return 0x1;
            }
        }
        else
        {
            // Check if buffer sizes given by user are minimum needed
            if (sippOPipeCheckSizePrecomSched (pipeLine))
            {
                return 0x1;
            }
        }

        if (reschedPipe == false)
        {
            if (sippOPipeAllocCdmaDescs (pipeLine))
            {
                return 0x1;
            }
        }

        #ifndef SIPP_PC
        sippOPipeInitCdmaDescs (pipeLine, reschedPipe);
        #endif

        if (reschedPipe == false)
        {
            // Now all sizes and irq rates have been set, the client count increment (how many lines to incrememt for each isr may be filled in)
            sippOPipeUpdateMgdBufCountIncrs (pipeLine);
        }
    }

    return 0x0;
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOPipeScheduleSetBufConsModels                                //
//                                                                                //
//  DESCRIPTION: Main API function for framework to set the input buffer          //
//               consumption models for all filter buffers                        //
//                                                                                //
//  INPUTS:      pipeLine - Pipeline                                              //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       Not needed for oPipe SchRT                                       //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippOPipeScheduleSetBufConsModels (pSippPipeline pipeLine)
{
    UNUSED (pipeLine);
}

#endif /* SIPP_SCRT_ENABLE_OPIPE */

// End of file
