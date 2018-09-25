///
/// @file      sippApi.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP framework API
///            Platform(s) supported : MA2x5x
///

#include <sipp.h>
#include <sippInternal.h>

////////////////////////////////////////////////////
// Global variables
////////////////////////////////////////////////////

tSippFramework gSippFramework;
u32            gSippInitMask = 0xFFFFFFFF;
u32            gSippSliceSz  = SIPP_DEFAULT_SLICE_SIZE;

extern pSIPP_HW_SESSION pgSippHW;

////////////////////////////////////////////////////
// Static variables
////////////////////////////////////////////////////


////////////////////////////////////////////////////
// Local macros
////////////////////////////////////////////////////

#define SIPP_CHECK_VALID_PIPE(pPipe, bValid) \
{  \
    u32 uIdx; \
    \
    for (uIdx = 0; uIdx < SIPP_MAX_SUPPORTED_PIPELINES; uIdx++) \
    { \
        if (gSippFramework.pPipelines[uIdx] == (ptSippPipelineSuper)pPipe) \
        { \
             bValid = TRUE; \
             break; \
        } \
    } \
}

////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippInit                                                         //
//                                                                                //
//  DESCRIPTION: This function initialises the SIPP Framework                     //
//               more notes to follow when it works...                            //
//                                                                                //
//  INPUTS:      pCfg - Pointer to a static configuration structure for the       //
//                      library  (Maybe - leave this as placeholder)              //
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

bool sippInitialize (void)
{
    if (gSippFramework.bInit == FALSE)
    {
        // Make all necessary init calls
        // Do SW parts first to init the scheduler as we need this service to provide
        // controlled access to the HW units in order to run their initialisation
        if (sippSWInit ())
        {
            // Should have the option to NOT do the hardware parts
            sippHWInit ();

            gSippFramework.bInit = TRUE;
        }
    }

    return gSippFramework.bInit;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippTerm                                                         //
//                                                                                //
//  DESCRIPTION: Deinitialises SIPP framework                                     //
//                                                                                //
//  INPUTS:      None                                                             //
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

void sippTerm (void)
{
    gSippFramework.bInit = FALSE;

    // Release all held resource - better just to do this here or call a separate function?
    // May need to force release some RTOS resource some thread is waiting on?
    sippTermInternal ();

    // The function above should delete all pipelines
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippCreatePipeline                                               //
//                                                                                //
//  DESCRIPTION: Opens an individual SIPP pipeline instance                       //
//                                                                                //
//  INPUTS:      sliceFirst - first slice / shave to be allocated                 //
//               sliceLast  - last slice / shave to be allocated                  //
//               pmBinImg   - pointer to shave binary                             //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     pPipeline - A handle for the opened SIPP pipeline instance       //
//                                                                                //
//  NOTES:       If pmBinImg is a NULL pointer we do not need the shaves          //
//               associated with the slices selected                              //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

SippPipeline * sippCreatePipeline (u32  sliceFirst,
                                   u32  sliceLast,
                                   u8 * pmBinImg)
{
    ptSippPipelineSuper pPipe;
    u32                 uIdx;
    bool                bAlloc = FALSE;
    SIPP_PAL_CRIT_STATE CS;

    if (gSippFramework.bInit == FALSE)
    {
        sippPalTrace (SIPP_API_TL_ERROR, "SIPP: sippCreatePipeline : Framework was not initialised.\n");
        return (SippPipeline *)0x0;
    }

    sippPalCriticalSectionBegin (&CS);

    for (uIdx = 0; uIdx < SIPP_MAX_SUPPORTED_PIPELINES; uIdx++)
    {
        if (gSippFramework.pPipelines[uIdx] == (ptSippPipelineSuper)NULL)
        {
            pPipe = (ptSippPipelineSuper)sippMemAlloc ((ptSippMCB)NULL,
                                                       vPoolPipeStructs,
                                                       sizeof (tSippPipelineSuper));

            if (pPipe)
            {
                sippPalMemset ((void *)pPipe,
                               0x0,
                               sizeof (tSippPipelineSuper));

                bAlloc                          = TRUE;
                gSippFramework.pPipelines[uIdx] = pPipe;
                pPipe->uPipeIdx                 = uIdx;
            }

            break;
        }
    }

    sippPalCriticalSectionEnd (CS);

    if (bAlloc == FALSE)
    {
        return (SippPipeline *)0x0;
    }

    sippInitPipeline (pPipe,
                      sliceFirst,
                      sliceLast,
                      gSippSliceSz,
                      pmBinImg);

    pPipe->eState = eSIPP_PIPE_WAIT_FINALISE;

    return (SippPipeline *)pPipe;

}


////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippDeletePipeline                                           //
//                                                                            //
//  DESCRIPTION: This function deletes a Pipeline                             //
//                                                                            //
//  INPUTS:      pPipe - The pipeline which is to be deleted                  //
//                                                                            //
//  OUTPUTS:     None.                                                        //
//                                                                            //
//  RETURNS:     None.                                                        //
//                                                                            //
//  NOTES:       pPipe must not be used following a call to this              //
//               function. Any attempt to reuse the handle after closing it   //
//               will result in return code eSIPP_STATUS_BAD_HANDLE           //
//                                                                            //
//  CONTEXT:     Unknown if there are any constraints here as yet             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void sippDeletePipeline (SippPipeline * pPipe)
{
    SIPP_PAL_CRIT_STATE CS;
    u32                 uIdx;
    u8                  found;

    sippPalTrace (SIPP_API_TL_VERBOSE, "SIPP: sippDeletePipeline 0x%08x\n", (u32)pPipe );

    sippPalCriticalSectionBegin (&CS);

    for (uIdx = 0,found = 0; uIdx < SIPP_MAX_SUPPORTED_PIPELINES; uIdx++)
    {
        if (gSippFramework.pPipelines[uIdx] == (ptSippPipelineSuper)pPipe)
        {
            found = 1;
            break;
        }
    }

    sippPalCriticalSectionEnd (CS);

    if (found)
    {
        // Delete ths pipeline's resources completely
        sippFreePipeResource (pPipe);

        // Free the pipeline struct itself
        sippMemFree ((ptSippMCB)NULL,
                     vPoolPipeStructs,
                     (void *)pPipe);

        // Do all of the work above before allowing this slot to be reassigned
        // in the framework struct
        gSippFramework.pPipelines[uIdx] = (ptSippPipelineSuper)NULL;
    }
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippRegisterEventCallback                                        //
//                                                                                //
//  DESCRIPTION: Registers a handler for reporting SIPP pipeline events           //
//                                                                                //
//  INPUTS:      pPipe      - pipe with which to register the callbcaks           //
//               pfCallback - Function ponter to be executed when a SIPP          //
//                            pipeline event is raised                            //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     Unknown if there are any constraints here as yet                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippRegisterEventCallback (SippPipeline *      pPipe,
                                sippEventCallback_t pfCallback )
{
    bool bFound = FALSE;

    SIPP_CHECK_VALID_PIPE(pPipe, bFound);

    if (bFound == FALSE)
    {
        sippPalTrace (SIPP_API_TL_ERROR, "SIPP: sippRegisterEventCallback - Invalid handle.\n");
        return;
    }

    pPipe->pfCallback = pfCallback;

}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippCreateFilter                                                 //
//                                                                                //
//  DESCRIPTION: This function allows the client to create a filter associated    //
//               with a specific SIPP pipeline                                    //
//                                                                                //
//  INPUTS:      pPipe - Pipeline with which the filter is to be associated       //
//               flags -                                                          //
//               out_W -                                                          //
//               out_H -                                                          //
//               num_pl -                                                         //
//               bpp - Really needs to be buffer specific!!!                      //
//               paramSz -                                                        //
//               funcSvuRun -                                                     //
//               name -                                                           //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     SippFilter * - A handle to currently initialised filter          //
//                                                                                //
//  NOTES:       All memory is allocated at this level                            //
//               If this works then a sub function is called to do majority of    //
//               the rest of the creation work                                    //
//                                                                                //
//  CONTEXT:     Unknown if there are any constraints here as yet                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

SippFilter * sippCreateFilter (pSippPipeline  pPipe,
                               u32            flags,
                               u32            out_W,
                               u32            out_H,
                               u32            num_pl,
                               u32            bpp,
                               u32            paramSz,
                               FnSvuRun       funcSvuRun,
                               const char *   name)
{
    pSippFilter pFilter;
    bool        bFound = FALSE;

    SIPP_CHECK_VALID_PIPE(pPipe, bFound);

    if (bFound == FALSE)
    {
        sippPalTrace (SIPP_API_TL_ERROR, "SIPP: sippCreateFilter - Invalid handle.\n");
        return (SippFilter *)0x0;
    }

    if (pPipe->nFilters == SIPP_MAX_FILTERS_PER_PIPELINE)
    {
        return (SippFilter *)0x0;
    }

    pPipe->filters[pPipe->nFilters] = (SippFilter *)sippMemAlloc (&pPipe->tHeapMCB,
                                                                  vPoolPipeStructs,
                                                                  sizeof(SippFilter));

    pFilter                         = pPipe->filters[pPipe->nFilters];

    if (pFilter == (pSippFilter)NULL)
    {
        sippError (pPipe->errorStatus, E_OUT_OF_MEM);
        return (SippFilter *)0x0;
    }
    else
    {
        sippPalMemset ((void *)pFilter,
                       0x0,
                       sizeof (SippFilter));
    }


    // Param buffers
    if (paramSz == 0)
    {
        pFilter->params = NULL;
    }
    else
    {
        u32 unitNo = (u32)funcSvuRun;

        if (unitNo < SIPP_SVU_ID)
        {
            // Force param size for known HW units !
            paramSz = gSipp.uInfo[unitNo].paramSz;
        }

        pFilter->params = (void *)sippMemAlloc (&pPipe->tHeapMCB,
                                                (unitNo == SIPP_DMA_ID) ? vPoolCMXDMADesc : vPoolPipeStructs,
                                                paramSz);

        if (pFilter->params == (void *)NULL)
        {
            sippError (pPipe->errorStatus, E_OUT_OF_MEM);
            return (SippFilter *)0x0;
        }

        sippPalMemset (pFilter->params,
                       0x0,
                       paramSz);
    }

    pFilter->oBufs[0] = (psSchLineBuffer)sippMemAlloc (&pPipe->tHeapMCB,
                                                       vPoolPipeStructs,
                                                       sizeof(sSchLineBuffer));

    if (pFilter->oBufs[0] == (psSchLineBuffer)NULL)
    {
        sippError (pPipe->errorStatus, E_OUT_OF_MEM);
        return (SippFilter *)0x0;
    }
    else
    {
        sippPalMemset (pFilter->oBufs[0],
                       0x0,
                       sizeof(sSchLineBuffer));
    }

    sippAddFilterToPipe (pPipe,
                         pFilter,
                         flags,
                         out_W,
                         out_H,
                         num_pl,
                         bpp,
                         funcSvuRun,
                         name);

    return pFilter;
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippFilterSetBufBitsPP                                           //
//                                                                                //
//  DESCRIPTION: Sets bits per pixel for a filter                                 //
//               Needed for packed10 and packed12 formats                         //
//                                                                                //
//  INPUTS:      pFilter - Filter                                                 //
//               oBufIdx                                                          //
//               bitsPerPixel                                                     //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     Unknown if there are any constraints here as yet                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippFilterSetBufBitsPP (pSippFilter pFilter,
                             u32         oBufIdx,
                             u32         bitsPerPixel)
{
    pFilter->bpp[oBufIdx] = bitsPerPixel;
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippLinkFilter                                                   //
//                                                                                //
//  DESCRIPTION: Create bidirectional link between a filter and its parent        //
//                                                                                //
//  INPUTS:      pFilter - Filter                                                 //
//               pParent - Parent filter                                          //
//               vKerSz  - Vertical kernel size (of the child filter)             //
//               hKerSz  - Horizontal kernel size (of the child filter)           //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     Unknown if there are any constraints here as yet                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippLinkFilter (pSippFilter pFilter,
                     pSippFilter pParent,
                     u32         vKerSz,
                     u32         hKerSz)
{
    pFilter->parents[pFilter->nParents]       = pParent;
    pFilter->nLinesUsed[pFilter->nParents]    = vKerSz;
    pFilter->iBufs[pFilter->nParents]         = pParent->oBufs[0];   // Always default to parent output buffer 0
    pFilter->parentOBufIdx[pFilter->nParents] = 0;                   // Always default to parent output buffer 0
    pParent->consIbufIdx[pParent->nCons]      = pFilter->nParents;

    // Allocate and initialise iBufCtx
    pFilter->iBufCtx[pFilter->nParents] = (SchedIBufUsageInfo *)sippMemAlloc (&pFilter->pPipe->tHeapMCB,
                                                                              vPoolPipeStructs,
                                                                              sizeof(SchedIBufUsageInfo));

    if (pFilter->iBufCtx[pFilter->nParents])
    {
        pFilter->iBufCtx[pFilter->nParents]->runFullSearch      = (pFilter->flags & SIPP_RESIZE) ? 1: 0;
        pFilter->iBufCtx[pFilter->nParents]->consumptionLatency = 0;
        pFilter->iBufCtx[pFilter->nParents]->lastAskLineNo      = 0xFFFFFFFF;
        pFilter->iBufCtx[pFilter->nParents]->numLinesUsed       = vKerSz;
        pFilter->iBufCtx[pFilter->nParents]->hKerSz             = hKerSz;
        pFilter->iBufCtx[pFilter->nParents]->outputHeight       = pFilter->outputH;
        pFilter->iBufCtx[pFilter->nParents]->params             = (void *)pFilter->params; // This is a bit of duplication
                                                                                           // but enables neater code in funcAsk
        pFilter->iBufCtx[pFilter->nParents]->KSDelta             = 0;
        pFilter->iBufCtx[pFilter->nParents]->iPadLines           = 0;

        // Prepare for next link (to next parent)
        pFilter->nParents++;
        pFilter->numIBufs++;

        if (pFilter->nParents > SIPP_FILTER_MAX_PARENTS)
        {
            sippError (pFilter->errorStatus, E_TOO_MANY_PARENTS);
            return;
        }
        else if (pFilter->unit == SIPP_SVU_ID)
        {
            pParent->oBufs[0]->numSWConsumers++;
        }

        if ((pFilter->unit == SIPP_SVU_ID) || (pFilter->unit == SIPP_DMA_ID))
        {
            // Parent must keep a SW buffer view as it's got at least 1 SW consummer
            pParent->flags |= SIPP_REQ_SW_VIEW;  // TBD now this SHOULD be buffer specific also
        }

        pParent->cons[pParent->nCons] = pFilter;
        pParent->nCons++;

        if ( pParent->nCons > SIPP_FILTER_MAX_CONSUMERS)
        {
            sippError (pFilter->errorStatus, E_TOO_MANY_CONSUMERS);
            return;
        }
    }
    else
    {
        sippError (pFilter->errorStatus, E_OUT_OF_MEM);
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippLinkFilterSetOBuf                                            //
//                                                                                //
//  DESCRIPTION: Modifies the parent output buffer which a consumer filter        //
//               uses as its input buffer - note the two filters must have been   //
//               previously linked thru a call to sippLinkFilter                  //
//                                                                                //
//  INPUTS:      pFilter - Filter                                                 //
//               pParent - Parent filter                                          //
//               parentOBufIdx  - Parenmt output buffer index we wish to link     //
//                                this filter to                                  //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       This will only work so long as the filter is only linked to      //
//               no more than one of any parent's output buffers. This function   //
//               would need to be expanded if that became a realistic situation   //
//               in the future but for now no need to code bloat without purpose  //
//               It does handle multiple connections to the same buffer within    //
//               the same parent also - Assumes this function is called before    //
//               any subsequent call to sippLinkFilter for the same 2 filters     //
//                                                                                //
//  CONTEXT:     Unknown if there are any constraints here as yet                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippLinkFilterSetOBuf (pSippFilter pFilter,
                            pSippFilter pParent,
                            u32         parentOBufIdx)
{
    s32 parentIdx;

    if (pFilter->nParents)
    {
        // The parent list is accessed from most recent connection as the same parent
        // could appear multiple times in the same list (eg DoGLTM to Debayer could be
        // connected twice)
        for (parentIdx = (pFilter->nParents - 0x1); parentIdx >= 0; parentIdx--)
        {
            if (pFilter->parents[parentIdx] == pParent)
            {
                pFilter->iBufs[parentIdx]         = pParent->oBufs[parentOBufIdx];
                pFilter->parentOBufIdx[parentIdx] = parentOBufIdx;
                if (pFilter->unit == SIPP_SVU_ID)
                {
                    pParent->oBufs[parentOBufIdx]->numSWConsumers++;
                    pParent->oBufs[0]->numSWConsumers--;
                }
                return;
            }
        }
    }

    sippError (pFilter->errorStatus, E_PAR_NOT_FOUND);

}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippFilterAddOBuf                                                //
//                                                                                //
//  DESCRIPTION: Adds an output buffer to a specified filter                      //
//                                                                                //
//  INPUTS:      pFilter - Filter                                                 //
//               pParent - Parent filter                                          //
//               parentOBufIdx  - Parent output buffer index we wish to link      //
//                                this filter to                                  //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       In order to keep the sippCreateFilter() API fully backwards      //
//               compatible only one value of number of planes and bits per pixel //
//               is passed as a parameter. The v3 framework requires these values //
//               to be buffer specific since filters may now have more than one   //
//               output buffer                                                    //
//               This API needs to be called before sippLinkFilter is called to   //
//               create a consumer link to the current filter as the parent       //
//                                                                                //
//  CONTEXT:     Unknown if there are any constraints here as yet                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippFilterAddOBuf (pSippFilter pFilter,
                        u32         numPlanes,
                        u32         bpp)
{
    u32 oBufIdx = pFilter->numOBufs;

    pFilter->oBufs[oBufIdx] = (psSchLineBuffer)sippMemAlloc (&pFilter->pPipe->tHeapMCB,
                                                             vPoolPipeStructs,
                                                             sizeof(sSchLineBuffer));

    if (pFilter->oBufs[oBufIdx])
    {
        pFilter->oBufs[oBufIdx]->numLines              = BUFF_HUGE_SZ;
        pFilter->oBufs[oBufIdx]->numSWConsumers        = 0;
        pFilter->oBufs[oBufIdx]->filtOutHeight         = pFilter->outputH;
        pFilter->oBufs[oBufIdx]->mostRecentLine        = -1;
        pFilter->oBufs[oBufIdx]->hwOutputBufId         = 0;
        pFilter->oBufs[oBufIdx]->hwInputBufId          = 0;
        pFilter->oBufs[oBufIdx]->bottomLineReplication = 0;

        pFilter->nLines[oBufIdx]                = 0;
        pFilter->oBufAlloc[oBufIdx]             = 0;
        pFilter->outputBuffer[oBufIdx]          = NULL;

        pFilter->bpp[oBufIdx]                   = bpp << 3;
        pFilter->bytesPerPix                    = bpp >> 1; // 1Bpp:0  2Bpp:1 4Bpp:2
        pFilter->planeStride[oBufIdx]           = 0;
        pFilter->hPadding[oBufIdx]              = 0;
        pFilter->outOff[oBufIdx]                = NULL;
        pFilter->nPlanes[oBufIdx]               = numPlanes;
        pFilter->outLineOffset[oBufIdx]         = 0;

        pFilter->numOBufs++;
    }
    else
    {
        sippError (pFilter->errorStatus, E_OUT_OF_MEM);
    }
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippReschedulePipeline                                           //
//                                                                                //
//  DESCRIPTION: Re-run the schedule derivation process for a SIPP pipeline       //
//                                                                                //
//  INPUTS:      pPipe - Pipeline to reschedule                                   //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     Unknown if there are any constraints here as yet                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippReschedulePipeline (pSippPipeline pPipe)
{
    bool bFound = FALSE;

    SIPP_CHECK_VALID_PIPE(pPipe, bFound);

    if (bFound == FALSE)
    {
        sippPalTrace (SIPP_API_TL_ERROR, "SIPP: sippReschedulePipeline - Invalid handle.\n");
        return;
    }

    sippRescheduleRequest (pPipe);

}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippFinalizePipeline                                             //
//                                                                                //
//  DESCRIPTION: Lock down and validate a pipeline + create its schedule          //
//                                                                                //
//  INPUTS:      pPipe - pipeline to process                                      //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     Unknown if there are any constraints here as yet                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippFinalizePipeline (pSippPipeline pPipe)
{
    bool bFound = FALSE;

    SIPP_CHECK_VALID_PIPE(pPipe, bFound);

    if (bFound == FALSE)
    {
        sippPalTrace (SIPP_API_TL_ERROR, "SIPP: sippFinalizePipeline - Invalid handle.\n");
        return;
    }

    if(!(pPipe->flags & PLF_IS_FINALIZED))
    {
        sippElaboratePipeline (pPipe);
    }
}

#if SIPP_RTOS == SIPP_NO_RTOS

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippProcessFrame                                                 //
//                                                                                //
//  DESCRIPTION: Process one frame of data from specified type                    //
//                                                                                //
//  INPUTS:      pPipe - Pipeline to process                                      //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       Should only call this blocking synchronous API if no commands    //
//               are pending via the asyncronous interface                        //
//                                                                                //
//  CONTEXT:     Unknown if there are any constraints here as yet                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////
#ifndef SIPP_ENABLE_RT_COEXISTENCE
void sippProcessFrame (pSippPipeline pPipe)
{
    eSIPP_STATUS eRetCode = eSIPP_STATUS_OK;
    bool         bFound   = FALSE;

    SIPP_CHECK_VALID_PIPE(pPipe, bFound);

    if (bFound == FALSE)
    {
        sippPalTrace (SIPP_API_TL_ERROR, "SIPP: sippProcessFrame - Invalid handle.\n");
        return;
    }

    pPipe->iteration        = 0;
    pPipe->useSyncRuntime   = 1;
    pPipe->firstUseMask     = 0;
    pPipe->firstIbflUseMask = 0;

    if(!(pPipe->flags & PLF_IS_FINALIZED))
    {
        eRetCode = sippElaboratePipeline (pPipe);
    }
    else
    {
        sippResetFilterVariables (pPipe);
    }

    if ( eRetCode == eSIPP_STATUS_OK )
    {
        SIPP_PAL_CRIT_STATE CS;
        // Only allow a sync operation if no other operations
        // are outstanding or are in progress as this needs to
        // be a blocking call

        // To check this, force a call to sippAccessSchedulerControl
        sippAccessSchedulerControl ( );

        sippPalCriticalSectionBegin (&CS);

        if ((sippAccessScheduleCheckPending () == 0) &&
            (pgSippHW->uNumCurrHwPipes == 0))
        {
            sippPalCriticalSectionEnd (CS);

            /* Call scheduler */
            sippRunItersRequest (pPipe, pPipe->nIter);
        }
        else
        {
            sippPalCriticalSectionEnd (CS);
            sippPalTrace (SIPP_API_TL_ERROR, "SIPP: sippProcessFrame - Outstanding or in progress HW operations remain - blocking call to sippProcessFrame rejected.\n");
            sippError (pPipe->errorStatus, E_BLOCK_CALL_REJECTED);
            return;
        }
    }
}
#endif

#endif

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippProcessFrameNB                                               //
//                                                                                //
//  DESCRIPTION: Process one frame of data from specified type asynchronously     //
//                                                                                //
//  INPUTS:      pPipe - Pipeline to process                                      //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:     Unknown if there are any constraints here as yet                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippProcessFrameNB (pSippPipeline pPipe)
{
    eSIPP_STATUS eRetCode = eSIPP_STATUS_OK;
    bool         bFound   = FALSE;

    SIPP_CHECK_VALID_PIPE(pPipe, bFound);

    if (bFound == FALSE)
    {
        sippPalTrace (SIPP_API_TL_ERROR, "SIPP: sippProcessFrameNB - Invalid handle.\n");
        return;
    }

    pPipe->iteration        = 0;
    pPipe->useSyncRuntime   = 0;
    pPipe->firstUseMask     = 0;
    pPipe->firstIbflUseMask = 0;

    if (!(pPipe->flags & PLF_IS_FINALIZED))
    {
        eRetCode = sippElaboratePipeline (pPipe);
    }
    else
    {
        sippResetFilterVariables (pPipe);
    }

    if (eRetCode == eSIPP_STATUS_OK)
    {
        /* Call scheduler */
        sippRunItersRequest (pPipe, pPipe->nIter);
    }
    else
    {
        sippPalTrace (SIPP_API_TL_ERROR, "SIPP: sippProcessFrameNB - Pipeline finalising failed - frame processing will not be scheduled\n");

        // Make a callback signifying pipeline elaboration failed

    }
}

#if SIPP_RTOS == SIPP_NO_RTOS

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippFrameworkRun                                                 //
//                                                                                //
//  DESCRIPTION: This is the thread function call for the SIPP framework          //
//                                                                                //
//  INPUTS:      None.                                                            //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       This call is needed to support the internal access scheduler's   //
//               response to event posts from irq context                         //
//                                                                                //
//  CONTEXT:     Call from thread context - call made by client not from within   //
//               framework unless an RTOS is enabled and SIPP framework uses its  //
//               own thread                                                       //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippFrameworkRun ( )
{
    /* Call the sippAccessScheduler */
    sippAccessSchedulerControl ( );
}

#endif

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippPipeSetLinesPerIter                                          //
//                                                                                //
//  DESCRIPTION: Set the number of lines per scedhuling iteration to run for a    //
//               specific pipeline                                                //
//                                                                                //
//  INPUTS:      pPipe - Pipeline to process                                      //
//               linesPerIter - must be 1/2/4/8 or 16                             //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       Larger lines per iter should give a speedup at the expense of    //
//               requiring more line buffer memory (usually in CMX)               //
//                                                                                //
//  CONTEXT:     Unknown if there are any constraints here as yet                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippPipeSetLinesPerIter (pSippPipeline pPipe,
                              u32           linesPerIter)
{
    bool         bFound   = FALSE;

    SIPP_CHECK_VALID_PIPE(pPipe, bFound);

    if (bFound == FALSE)
    {
        sippPalTrace (SIPP_API_TL_ERROR, "SIPP: sippPipeSetLinesPerIter - Invalid handle.\n");
        return;
    }

    if ((linesPerIter) &&
         linesPerIter < 17)
    {
        u32 localLinesPerIter = linesPerIter;

        do
        {
            if (localLinesPerIter & 0x1)
            {
                localLinesPerIter >>= 0x1;
                if (localLinesPerIter)
                {
                    sippPalTrace (SIPP_API_TL_ERROR, "SIPP: sippPipeSetLinesPerIter - linesPerIter must be 1/2/4/8 or 16\n");
                }
            }
            else
            {
            	localLinesPerIter >>= 0x1;
            }

        }while (localLinesPerIter);

        pPipe->linesPerIter = linesPerIter;
    }
    else
    {
        sippPalTrace (SIPP_API_TL_ERROR, "SIPP: sippPipeSetLinesPerIter - linesPerIter must be 1/2/4/8 or 16\n");
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippPipeSetNumLinesPerBuf                                        //
//                                                                                //
//  DESCRIPTION: Set the number of lines per output buffer in direct streaming    //
//               runtime                                                          //
//                                                                                //
//  INPUTS:      pFilter - filter to process                                      //
//               oBufIdx - output buffer index of filter for which we wish to     //
//                         set number of lines                                    //
//               numLines - number of lines                                       //                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       Larger number of lines per output buffer should give a speedup   //
//               at the expense of requiring more line buffer memory (usually     //
//               in CMX)                                                          //
//                                                                                //
//  CONTEXT:     Unknown if there are any constraints here as yet                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippPipeSetNumLinesPerBuf (pSippFilter pFilter,
                                u32         oBufIdx,
                                u32         numLines)
{
    pFilter->nLines[oBufIdx]          = numLines;
    pFilter->oBufs[oBufIdx]->numLines = numLines;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippDynRouteIrq                                                  //
//                                                                                //
//  DESCRIPTION: Set IRQ sources for dynamically route IRQ from LeonRT to LeonOS  //
//                                                                                //
//  INPUTS:      pPipe - Pipeline to process                                      //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       When running async runtime on LeonOS dynamic interrupts          //
//               will be used                                                     //
//                                                                                //
//  CONTEXT:     Unknown if there are any constraints here as yet                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippDynRouteIrq (u32 dynamic_irqSipp0, u32 dynamic_irqSipp1, u32 dynamic_irqSipp2)
{

    #if defined(__sparc)
    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    if (swcWhoAmI() == PROCESS_LEON_OS)
    {
        DrvIcbDynamicIrqConfig (LRT_TO_LOS, IRQ_SIPP_0, gSippFramework.dynIrqSipp0, 0x0);
        DrvIcbDynamicIrqConfig (LRT_TO_LOS, IRQ_SIPP_1, gSippFramework.dynIrqSipp1, 0x0);
        DrvIcbDynamicIrqConfig (LRT_TO_LOS, IRQ_SIPP_2, gSippFramework.dynIrqSipp2, 0x0);

        DrvIcbIrqUnsetup (gSippFramework.dynIrqSipp0);
        DrvIcbIrqUnsetup (gSippFramework.dynIrqSipp1);
        DrvIcbIrqUnsetup (gSippFramework.dynIrqSipp2);
    }
    #endif
    #endif

    gSippFramework.dynIrqSipp0 = dynamic_irqSipp0;
    gSippFramework.dynIrqSipp1 = dynamic_irqSipp1;
    gSippFramework.dynIrqSipp2 = dynamic_irqSipp2;

    sippIsrSetup ( );
}

// End of file
