///
/// @file      sippAnalysePipema2x5x.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Examine a pipeline for validity & features pertinent to implementation
///            Platform(s) supported : MA2x5x
///

#include <sipp.h>
#include <sippInternal.h>

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippSetOPipeFilterLatencies                                      //
//                                                                                //
//  DESCRIPTION: Set latency for each output buffer of each HW filter in          //
//               pipeline based on the filter's setup parameters.                 //
//                                                                                //
//  INPUTS:      pPipe     - Pipeline for analysis                                //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       Latency calculated is in terms of lines added to the input       //
//               buffers above the minimum dictated by the max kernel size,       //
//               before the first line is made available in that filter buffer    //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

static void sippSetOPipeFilterLatencies (pSippPipeline pPipe)
{
    SippFilter * ptrFilt;
    u32          filtIdx;

    for (filtIdx = 0; filtIdx < pPipe->nFilters; filtIdx++)
    {
        // Should have a better list with only the HW filters in it to save time
        ptrFilt = pPipe->filters[filtIdx];

        if (ptrFilt->unit <= SIPP_MAX_FILTER_ID)
        {
            gSipp.uInfo[ptrFilt->unit].hwSetObufLatencies(ptrFilt);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippSetBuffsToAlloc                                              //
//                                                                                //
//  DESCRIPTION: Mark all the output buffers which need to be allcoated - that is //
//               which have not been made redundant by an oPipe streaming         //
//               connection                                                       //
//                                                                                //
//  INPUTS:      pPipe     - Pipeline for analysis                                //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       Create a managed buffer list here for oPipe runtime. Remember    //
//               such a runtime will not be used if SW pipes are included so no   //
//               need to check for that here                                      //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

static void sippSetBuffsToAlloc (SippPipeline *pl)
{
    u32              i, o;
    SippFilter *     fptr;
    SippOse *        pOSE;
    #ifdef SIPP_SCRT_ENABLE_OPIPE
    SippOPipeBuf     managedBufList[24];
    u32              managedBufIdx = 0;
    bool             createManagedList = (pl->pfnSippRuntime == sippOPipeRuntime) ? TRUE : FALSE;
    #endif

    for (i = 0; i < pl->nFilters; i++)
    {
        fptr = pl->filters[i];

        // TBD - in reality - this should be "does this output buffer have consumers?"
        // Not does the filter have consumers...
        if (fptr->nCons)
        {
            if (pl->filters[i]->memOSE == 0)
            {
                for (o = 0; o < fptr->numOBufs; o++)
                {
                    fptr->oBufs[o]->allocReq  = 0x1;
                    fptr->oBufs[o]->manageReq = 0x1;
                }
            }
        }
    }

    // Go through OSEs also
    for (i = 0; i < pl->numOSE; i++)
    {
        pOSE = pl->pOSE[i];

        for (o = 0; o < pOSE->numOBufs; o++)
        {
            pOSE->oBufs[o]->allocReq  = 0x1;
            pOSE->oBufs[o]->manageReq = 0x1;
        }
    }


    #ifdef SIPP_SCRT_ENABLE_OPIPE
    // We need a list of all filter buffers here - those
    // which are allocated but not managed

    if (createManagedList)
    {
        for (i = 0; i < pl->nFilters; i++)
        {
            fptr = pl->filters[i];

            for (o = 0; o < fptr->numOBufs; o++)
            {
                if (fptr->oBufs[o]->manageReq)
                {
                    if (managedBufIdx < 24)
                    {
                        managedBufList[managedBufIdx].pBuf          = fptr->oBufs[o];
                        managedBufList[managedBufIdx].pFilt         = fptr;
                        managedBufList[managedBufIdx].linesNextFill = 0x0;
                        managedBufList[managedBufIdx].oBufIdx       = o;
                        managedBufList[managedBufIdx].numClients    = 0x0;

                        #ifdef SIPP_ADD_OPIPE_RT_CHECKS
                        managedBufList[managedBufIdx].oBufCtxReg     = (u32 *)sippBufGetObufCtx (fptr, o);
                        #endif

                        managedBufIdx++;
                    }
                    else
                    {
                        sippPalTrace (SIPP_FW_MGR_TL_ERROR, "Total number of managed buffers exceeded\n");
                    }
                }
            }
        }

        pl->managedBufList = (SippOPipeBuf *)sippMemAlloc (&pl->tHeapMCB,
                                                           vPoolPipeStructs,
                                                           sizeof(SippOPipeBuf) * managedBufIdx);

        if (pl->managedBufList == (SippOPipeBuf *)NULL)
        {
            sippError (pl->errorStatus, E_OUT_OF_MEM);
        }
        else
        {
            sippPalMemcpy ((void *)pl->managedBufList,
                           (void *)managedBufList,
                           sizeof(SippOPipeBuf) * managedBufIdx);

            pl->numManagedBufs = managedBufIdx;

            // Make a backwards connection between the buffer struct and the managed buffer
            // struct for convenience
            for (i = 0, managedBufIdx = 0; i < pl->nFilters; i++)
            {
                fptr = pl->filters[i];
                for (o = 0; o < fptr->numOBufs; o++)
                {
                    if (fptr->oBufs[o]->manageReq)
                    {
                        fptr->oBufs[o]->pManagedBuf = &pl->managedBufList[managedBufIdx++];
                    }
                }
            }
        }
    }
    #endif
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippCheckFilterSetup                                             //
//                                                                                //
//  DESCRIPTION: Once full pipe cfg is known, some filter setup may need updated  //
//                                                                                //
//  INPUTS:      pPipe     - Pipeline for analysis                                //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

static void sippCheckFilterSetup (pSippPipeline pPipe)
{
    SippFilter * ptrFilt;
    u32          filtIdx;
    u32          parentIdx;

    for (filtIdx = 0; filtIdx < pPipe->nFilters; filtIdx++)
    {
    	ptrFilt = pPipe->filters[filtIdx];

    	if (ptrFilt->unit <= SIPP_MAX_FILTER_ID)
        {
            if (gSipp.uInfo[ptrFilt->unit].hwFilterSetupUpdate)
            {
                gSipp.uInfo[ptrFilt->unit].hwFilterSetupUpdate (ptrFilt);
            }

            #ifdef BUGZILLA_22709
            ptrFilt->bug22709Active = (ptrFilt->linesPerIter > 1) ? 0x1 : 0x0;
            #endif
        }
        else
        {
            #ifdef BUGZILLA_22709
            ptrFilt->bug22709Active = 0x0;
            #endif
        }

        // We are now able to setup the pFilter->iBufCtx[parentIdx]->KSDelta here
        // for all input buffers
        for (parentIdx = 0; parentIdx < ptrFilt->nParents; parentIdx++)
        {
            if ((ptrFilt->flags & SIPP_RESIZE) == 0 )
            {
                ptrFilt->iBufCtx[parentIdx]->KSDelta = ptrFilt->linesPerIter - 0x1;

                // Handle irregular consumers
                // Color Combination filter on its chroma channel
                if ((ptrFilt->unit == SIPP_CC_ID) &&
                    (ptrFilt->iBufCtx[parentIdx]->numLinesUsed > 1))
                {
                    ptrFilt->iBufCtx[parentIdx]->KSDelta >>= 0x1;
                }

                // Median Filter when luma blending
                if ((ptrFilt->unit == SIPP_MED_ID) &&
                    (parentIdx == 1))
                {
                    // If we need to sub-sample the luma we need a different buffer
                    // consumption model
                    MedParam * medCfg = (MedParam *)ptrFilt->params;

                    if ((medCfg->cfg >> 28) & 0x1)
                    {
                        ptrFilt->iBufCtx[parentIdx]->KSDelta++;
                        ptrFilt->iBufCtx[parentIdx]->KSDelta <<= 0x1;
                        ptrFilt->iBufCtx[parentIdx]->KSDelta--;
                    }
                }
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippPipeEnsureProducerConsumerAlignment                          //
//                                                                                //
//  DESCRIPTION: For MLPI, ensure a consuming filter is not set to a finer        //
//               granularity than a producer, otherwise the pipe will not be      //
//               successfully scheduled                                           //
//                                                                                //
//               Also try to make sure that any non-resizing parents of an        //
//               upscaler produce at a finer granularity than the upscalar as     //
//               this will avoid cyclic delays in the schedule                    //
//                                                                                //
//  INPUTS:      ptrFilt  - Pointer to filter                                     //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       Called recursively on a modification of linesPerIter             //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

static void sippPipeEnsureProducerConsumerAlignment (SippFilter *  ptrFilt)
{
    u32 connectionIdx;

    SippFilter ** connectionList = ptrFilt->parents;
    u32           maxIdx         = ptrFilt->nParents;

    for (connectionIdx = 0; connectionIdx < maxIdx; connectionIdx++)
    {
        SippFilter * ptrConnectedFilt = connectionList[connectionIdx];

        if (!(ptrFilt->flags & SIPP_RESIZE))
        {
            if (ptrFilt->linesPerIter < ptrConnectedFilt->linesPerIter)
            {
                // This can be acceptable if even though the filter itself is not a resizer,
                // its output is a different vertical size from the particular buffer
                // in this connection
                if (ptrConnectedFilt->outputH <= ptrFilt->outputH)
                {
                    ptrConnectedFilt->linesPerIter = ptrFilt->linesPerIter;

                    // Need to now do this recursively for the parent filter
                    sippPipeEnsureProducerConsumerAlignment (ptrConnectedFilt);
                }
            }
        }
        else
        {
            // The current filter is scaling - check if the parent filter is not a resizer
            if (!(ptrConnectedFilt->flags & SIPP_RESIZE))
            {
                // Check to remove potential schedule cyclic delays
                if (ptrFilt->linesPerIter == ptrConnectedFilt->linesPerIter)
                {
                    u32 filtOutH        = ptrFilt->outputH >> 0x1;
                    u32 filtInH         = ptrConnectedFilt->outputH;

                    // Is the current filter upscaling by at least factor of 2 (only interested vertically)
                    if (filtOutH >= filtInH)
                    {
                        if (ptrConnectedFilt->linesPerIter > 0x1)
                        {
                            ptrConnectedFilt->linesPerIter >>= 0x1;

                            // Need to now do this recursively for the parent filter
                            sippPipeEnsureProducerConsumerAlignment (ptrConnectedFilt);
                        }
                    }
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippPipeSetFilterLinesPerIter                                    //
//                                                                                //
//  DESCRIPTION: Given the pipeline value for lines per iteration, translate      //
//               that to a commensurate value for each filter                     //
//                                                                                //
//  INPUTS:      pPipe     - Pipeline                                             //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       The client sets a target number of lines per iteration for the   //
//               pipeline - if a filter resizes the frame it will be considered   //
//               here to have its number of lines per iteration adjusted w.r.t.   //
//               that for the overall pipeline. This prevents the need to hold    //
//               huge input buffers for filters which aggressively downscale      //
//                                                                                //
//               No non-resizer filter can consume from a parent producing at     //
//               a larger number of lines per iteration (no issue with a parent   //
//               producing at a finer granularity)                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

static void sippPipeSetFilterLinesPerIter (pSippPipeline pPipe)
{
    SippFilter * ptrFilt;
    u32          filtIdx;
    u32          minOutH      = 0xFFFFFFFF;
    u32          localPipeLPI = pPipe->linesPerIter;

    if (pPipe->linesPerIter > 1)
    {
        u32 CheckRule2 = 0;

        // Initial check that the number of lines per iter is not > than the minimum output height of any filter
        for (filtIdx = 0; filtIdx < pPipe->nFilters; filtIdx++)
        {
            ptrFilt = pPipe->filters[filtIdx];

            if (ptrFilt->outputH < minOutH)
            {
                minOutH = ptrFilt->outputH;
            }
        }

        while (localPipeLPI >= minOutH)
        {
            if (localPipeLPI > 0x1)
            {
                localPipeLPI >>= 0x1;
            }
            else
            {
                break;
            }
        }

        pPipe->linesPerIter = localPipeLPI;

        // Rule 1 - If a filter downscales, set the number of lines
        // per iteration to a downscaled amount (avoids excess CMX storage)
        for (filtIdx = 0; filtIdx < pPipe->nFilters; filtIdx++)
        {
            ptrFilt = pPipe->filters[filtIdx];

            if (ptrFilt->unit == SIPP_CC_ID)
            {
                CheckRule2 = 1;
            }

            if (ptrFilt->flags & SIPP_RESIZE)
            {
                if (ptrFilt->nParents)
                {
                    // Establish if the filter is downscaling vertically
                    if (ptrFilt->outputH < ptrFilt->parents[0]->outputH)
                    {
                        // How much are we downscaling?
                        u32 filtOutH        = ptrFilt->outputH;
                        u32 filtInH         = ptrFilt->parents[0]->outputH >> 0x1;
                        u32 downScaleFactor = 0;

                        while (filtOutH <= filtInH)
                        {
                            downScaleFactor++;
                            filtInH >>= 0x1;
                        }

                        // Ideally we now downscale the number of lines per iteration by
                        // 2^downScaleFactor but of-course the pipeline number of lines per iteration
                        // must support this

                        if (pPipe->linesPerIter >> downScaleFactor)
                        {
                            ptrFilt->linesPerIter = (pPipe->linesPerIter >> downScaleFactor);
                        }
                        else
                        {
                            ptrFilt->linesPerIter = 0x1;
                        }
                    }
                    else
                    {
                        // Upscaling or incorrectly flagged!!!
                        ptrFilt->linesPerIter = pPipe->linesPerIter;
                    }
                }
                else
                {
                    ptrFilt->linesPerIter = pPipe->linesPerIter;
                }
            }
            else
            {
                ptrFilt->linesPerIter = pPipe->linesPerIter;
            }
        }

        if (CheckRule2)
        {
            for (filtIdx = 0; filtIdx < pPipe->nFilters; filtIdx++)
            {
                // Rule 2 - If a consumer filter uses a non-regular consumption model
                //          it may be necessary to ensure the producer produces at a lower
                //          granularity - this rule is for Color Comb filter specifically

                ptrFilt = pPipe->filters[filtIdx];

                if (ptrFilt->unit == SIPP_CC_ID)
                {
                    // Check that the chroma parent is producing at a lower granularity (if
                    // color comb has linesPerIter > 1)
                    if (ptrFilt->nParents > 1)
                    {
                        // Probably an unnecessary check below but lets be paranoid for now
                        if (ptrFilt->iBufCtx[(ptrFilt->nParents - 0x1)]->numLinesUsed > 1)
                        {
                            u32 linesPerIter = (ptrFilt->linesPerIter >> 1);

                            if (linesPerIter)
                            {
                                SippFilter * ptrParent = ptrFilt->parents[(ptrFilt->nParents - 0x1)];

                                // This must be fed back until we reach another filter of the same granularity
                                // or a resizer
                                while ((ptrParent->linesPerIter != linesPerIter) &&
                                       (!(ptrParent->flags & SIPP_RESIZE))
                                      )
                                {
                                    ptrParent->linesPerIter = linesPerIter;
                                    if (ptrParent->nParents)
                                    {
                                        ptrParent = ptrParent->parents[0];
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Rule 3 - no non-resizing filter can consume from another filter producing
    // at a coarser granularity
    for (filtIdx = 0; filtIdx < pPipe->nFilters; filtIdx++)
    {
        ptrFilt = pPipe->filters[filtIdx];

        sippPipeEnsureProducerConsumerAlignment (ptrFilt);
    }

    for (filtIdx = 0; filtIdx < pPipe->nFilters; filtIdx++)
    {
        u32 localLinesPerIter;

        ptrFilt                    = pPipe->filters[filtIdx];
        ptrFilt->linesPerIterShift = 0;
        localLinesPerIter          = ptrFilt->linesPerIter;

        while (localLinesPerIter > 0x1)
        {
            localLinesPerIter >>= 0x1;
            ptrFilt->linesPerIterShift++;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippAnalyseAssignSchRT                                           //
//                                                                                //
//  DESCRIPTION: Analyses pipeline info to select the most efficient scheduler /  //
//               runtime combination for that pipe                                //
//                                                                                //
//  INPUTS:      pPipe     - Pipeline for analysis                                //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

static void sippAnalyseAssignSchRT (pSippPipeline pPipe)
{
    #ifdef SIPP_SCRT_ENABLE_OPIPE
    SippFilter * ptrFilt;
    u32          filtIdx, oPipeList = 0;
    u32          useoPipeScheduler  = (pPipe->flags & PLF_CONSIDER_OPIPE_RT) ? 0x1 : 0x0;

    if (useoPipeScheduler)
    {
        for (filtIdx = 0, useoPipeScheduler = 1; filtIdx < pPipe->nFilters; filtIdx++)
        {
            ptrFilt = pPipe->filters[filtIdx];

            // Check no HW filters are used more than once, except DMA
            if (ptrFilt->unit > SIPP_DMA_ID)
            {
                useoPipeScheduler = 0;
                break;
            }
            else
            {
                if (ptrFilt->unit <= SIPP_MAX_ID)
                {
                    if (~(oPipeList & (0x1 << ptrFilt->unit)))
                    {
                        oPipeList |= (0x1 << ptrFilt->unit);
                    }
                    else
                    {
                        useoPipeScheduler = 0;
                    }
                }
            }
        }
    }

    if (useoPipeScheduler == 0x1)
    {
        pPipe->pfnSippSchedule                = sippOPipeSchedule;
        pPipe->pfnSippScheSetBufConsModels    = sippOPipeScheduleSetBufConsModels;
        pPipe->pfnSippRuntimeClaimHWResource  = sippOPipeRuntimeClaimHWResource;
        pPipe->pfnSippRuntime                 = sippOPipeRuntime;
        pPipe->pfnSippRunIterDone             = sippOPipeRunIterDone;
        pPipe->pfnSippRunNextIter             = sippOPipeRunNextIter;
        pPipe->pfnSippRunFrameReset           = sippOPipeRuntimeFrameReset;

        sippPalTrace (SIPP_FW_MGR_TL_INFO, "OPipe Scheduler and RunTime chosen\n");
    }
    else
    #endif
    {
        #ifdef SIPP_SCRT_ENABLE_MLPI
        if (pPipe->linesPerIter > 1)
        {
            pPipe->pfnSippSchedule                = sippMLPISchedule;
            pPipe->pfnSippScheSetBufConsModels    = sippMLPIScheduleSetBufConsModels;
            pPipe->pfnSippRuntimeClaimHWResource  = sippMLPIRuntimeClaimHWResource;
            pPipe->pfnSippRuntime                 = sippMLPIRuntime;
            pPipe->pfnSippRunIterDone             = sippMLPIRunIterDone;
            pPipe->pfnSippRunNextIter             = sippMLPIRunNextIter;
            pPipe->pfnSippRunFrameReset           = sippMLPIRuntimeFrameReset;
            sippPalTrace (SIPP_FW_MGR_TL_INFO, "MLPI Scheduler and RunTime chosen\n");
        }
        else
        #endif
        {
            pPipe->pfnSippSchedule                = sippGenericSchedule;
            pPipe->pfnSippScheSetBufConsModels    = sippGenericScheduleSetBufConsModels;
            pPipe->pfnSippRuntimeClaimHWResource  = sippGenericRuntimeClaimHWResource;
            pPipe->pfnSippRuntime                 = sippGenericRuntime;
            pPipe->pfnSippRunIterDone             = sippGenericRunIterDone;
            pPipe->pfnSippRunNextIter             = sippGenericRunNextIter;
            pPipe->pfnSippRunFrameReset           = sippGenericRuntimeFrameReset;

            sippPalTrace (SIPP_FW_MGR_TL_INFO, "Generic Scheduler and RunTime chosen\n");
        }
    }

    // Now the input buffer consumption models should be assigned for each filter
    pPipe->pfnSippScheSetBufConsModels (pPipe);
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippAnalysePipe2x5x                                              //
//                                                                                //
//  DESCRIPTION: Analyses pipeline for patterns germane to the SIPP processing    //
//               framework                                                        //
//                                                                                //
//  INPUTS:      pPipe     - Pipeline for analysis                                //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippAnalysePipe2x5x (pSippPipeline pPipe)
{
    SippFilter * ptrFilt;
    u32          filtIdx;
    #ifdef SIPP_SCRT_ENABLE_OPIPE
    bool         createManagedList;
    #endif

    // Set the number of lines per iteration for each filter - before OSE checks
    sippPipeSetFilterLinesPerIter (pPipe);

    // Check for updates to the setup required after configuration
    sippCheckFilterSetup (pPipe);

    // Use the information to assign the appropriate scheduler / runtime combination
    sippAnalyseAssignSchRT (pPipe);

    // Set latency for all of the filters output buffers now based on their setup parameters
    // Do this before looking for OSEs as this will need those values to composite latencies
    sippSetOPipeFilterLatencies (pPipe);

    sippIdentifyOPipeSchedulingEntity (pPipe);

    // Modify HW filters with LLB which are nor members of OSEs
    for (filtIdx = 0; filtIdx < pPipe->nFilters; filtIdx++)
    {
        ptrFilt = pPipe->filters[filtIdx];

        if (ptrFilt->unit < SIPP_MAX_FILTER_ID)
        {
            if ((0x1 << ptrFilt->unit) & SIPP_FILTER_LLB_MASK)
            {
                if (ptrFilt->memOSE == 0)
                {

                    // It turns out that if we start a filter with a LLB as soon
                    // as the required data is available in CMX there will be a
                    // throughput issue
                    // The framework wants to increment the input buffer fill level (IBFL)
                    // and then start the filter through decrementing output buffer fill level (OBFL)
                    // then the local line buffer control holds off filter operation from
                    // starting until all lines required by the kernel for the next output line
                    // are available
                    u32 parentIdx;

                    for (parentIdx = 0x0; parentIdx < ptrFilt->nParents; parentIdx++)
                    {
                        ptrFilt->iBufCtx[parentIdx]->numLinesUsed += ptrFilt->linesPerIter;
                    }
                }
            }
        }
    }

    // Work out which of the filters oBufs need to be allocated (since some may now be implemented
    // as direct oPipe connections)
    sippSetBuffsToAlloc (pPipe);

    #ifdef SIPP_SCRT_ENABLE_OPIPE
    createManagedList = (pPipe->pfnSippRuntime == sippOPipeRuntime) ? TRUE : FALSE;
    #endif

    for (filtIdx = 0; filtIdx < pPipe->nFilters; filtIdx++)
    {
        u32 consIdx;

        // Should have a better list with only the HW filters in it to save time
        ptrFilt = pPipe->filters[filtIdx];

        if (ptrFilt->unit <= SIPP_MAX_FILTER_ID)
        {
            u32 oBufIdx;

            // Setup hwOutputBufId for all output buffers
            for (oBufIdx = 0; oBufIdx < ptrFilt->numOBufs; oBufIdx++)
            {
                ptrFilt->oBufs[oBufIdx]->hwOutputBufId = gSipp.uInfo[ptrFilt->unit].hwGetObufIds(ptrFilt, oBufIdx);

                if ((pPipe->linesPerIter != ptrFilt->linesPerIter) &&
                    (ptrFilt->oBufs[oBufIdx]->allocReq))
                {
                    pPipe->startDelta |= ptrFilt->oBufs[oBufIdx]->hwOutputBufId;
                }
            }
        }

        for (consIdx = 0; consIdx < ptrFilt->nCons; consIdx++)
        {
            // Find the consumer iBuf in the oBuf list of the current SE
            SippFilter *    pConsFilt = ptrFilt->cons[consIdx];

            if (pConsFilt->unit <= SIPP_MAX_FILTER_ID)
            {
                psSchLineBuffer consOBuf  = pConsFilt->iBufs[ptrFilt->consIbufIdx[consIdx]];
                u32             oBufIdx;

                for (oBufIdx = 0; oBufIdx < ptrFilt->numOBufs; oBufIdx++)
                {
                    if (ptrFilt->oBufs[oBufIdx] == consOBuf)
                    {
                        if (ptrFilt->oBufs[oBufIdx]->allocReq)
                        {
                            u32 iBufID = gSipp.uInfo[pConsFilt->unit].hwGetIbufIds(pConsFilt, ptrFilt->consIbufIdx[consIdx]);

                            #ifdef SIPP_SCRT_ENABLE_OPIPE
                            if (createManagedList)
                            {
                                if (ptrFilt->oBufs[oBufIdx]->manageReq)
                                {
                                    // Check that this is not a double connection (Dog -> Debayer)
                                    if ((iBufID & ptrFilt->oBufs[oBufIdx]->hwInputBufId) == 0x0)
                                    {
                                        // Create a fast map
                                        ptrFilt->oBufs[oBufIdx]->pManagedBuf->clientCountMap[ptrFilt->oBufs[oBufIdx]->pManagedBuf->numClients]  = iBufID;
                                        ptrFilt->oBufs[oBufIdx]->pManagedBuf->clientCountIncr[ptrFilt->oBufs[oBufIdx]->pManagedBuf->numClients] = (u32)&pConsFilt->irqRatePow;

                                        #ifdef SIPP_ADD_OPIPE_RT_CHECKS
                                        ptrFilt->oBufs[oBufIdx]->pManagedBuf->iBufCtxReg[ptrFilt->oBufs[oBufIdx]->pManagedBuf->numClients] = (u32 *)gSipp.uInfo[pConsFilt->unit].hwGetIbufCtx(pConsFilt, iBufID);
                                        #endif

                                        ptrFilt->oBufs[oBufIdx]->pManagedBuf->numClients++;
                                    }
                                }
                            }
                            #endif

                            // The consumer filter is allowed to set which bits it wants to see set in the input buf incr register
                            // when the current filter's oBufs[oBufIdx] is written to
                            ptrFilt->oBufs[oBufIdx]->hwInputBufId |= iBufID;

                            if (pPipe->linesPerIter != ptrFilt->linesPerIter)
                            {
                                pPipe->ibflIncDelta |= ptrFilt->oBufs[oBufIdx]->hwInputBufId;
                            }
                        }
                    }
                }
            }
            #ifdef SIPP_SCRT_ENABLE_OPIPE
            else if (createManagedList)
            {
                // createManagedList being true infers that the current filter is a HW filter
                // since otherwise the oPipe runtime would not be chosen
                // It also infers that the consumer filter is a HW filter or a DMA for the same
                // reason.
                if (pConsFilt->unit == SIPP_DMA_ID)
                {
                    psSchLineBuffer consOBuf  = pConsFilt->iBufs[ptrFilt->consIbufIdx[consIdx]];
                    u32             oBufIdx;

                    for (oBufIdx = 0; oBufIdx < ptrFilt->numOBufs; oBufIdx++)
                    {
                        if (ptrFilt->oBufs[oBufIdx] == consOBuf)
                        {
                            if (ptrFilt->oBufs[oBufIdx]->manageReq)
                            {
                                // For now, we will not create the mask here but will instead add the filter structs
                                // so that on initialisation of the filters descriptors, this can be sorted out then
                                ptrFilt->oBufs[oBufIdx]->pManagedBuf->clientCountMap[ptrFilt->oBufs[oBufIdx]->pManagedBuf->numClients]  = (u32)pConsFilt;
                                ptrFilt->oBufs[oBufIdx]->pManagedBuf->clientCountIncr[ptrFilt->oBufs[oBufIdx]->pManagedBuf->numClients] = (u32)&pConsFilt->irqRatePow;
                                ptrFilt->oBufs[oBufIdx]->pManagedBuf->numClients++;
                            }
                        }
                    }
                }
            }
            #endif
        }
    }
}
