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

extern tSippFramework gSippFramework;

////////////////////////////////////////////////////
// Static variables
////////////////////////////////////////////////////

////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippHWInit                                                       //
//                                                                                //
//  DESCRIPTION: Sipp framework HW system initialise                              //
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

void sippHWInit (void)
{
    #ifdef SIPP_PC
    extern void initPCHwModels ( );
    initPCHwModels ( );
    #endif

    #ifndef SIPP_PC
    gSippFramework.dynIrqSipp0 = DYNAMIC_IRQ_SIPP_0;
    gSippFramework.dynIrqSipp1 = DYNAMIC_IRQ_SIPP_1;
    gSippFramework.dynIrqSipp2 = DYNAMIC_IRQ_SIPP_2;
    #endif

    sippIssueCommand ((pSippPipeline)NULL,
                      eSIPP_CMD_INIT_HW,
                      (SippCommandData)NULL);

}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippSWInit                                                       //
//                                                                                //
//  DESCRIPTION: Sipp framework - initialise SW aspects                           //
//                                                                                //
//  INPUTS:      None.                                                            //
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

bool sippSWInit (void)
{

    // Initialise the access scheduler
    if (sippAccessSchedulerInit ( ))
    {
        // Initialise the callback mechanism
        sippCallbackInit ( );

        // Initialise the resource manager

        // Initialise Pipeline validation

        // Initialise the pipeline session controller
        sippPipeSessionControlInit ( );

        // Initialise trace
        sippPalTraceInit ( );

        // Initialise error recording
        sippErrorInit ( );

        #if SIPP_RTOS == SIPP_NO_RTOS

        return TRUE;

        #else

        // Create the main thread
        if (sippThreadCreate ())
        {
            return TRUE;
        }

        #endif
    }

    return FALSE;

}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippInitPipeline                                                 //
//                                                                                //
//  DESCRIPTION: Internal call to initialise a sipp pipeline strucutre            //
//                                                                                //
//  INPUTS:      ptSPipe - pointer to the super pipe                              //
//               sliceFirst                                                       //
//               sliceLast                                                        //
//               sliceSize                                                        //
//               mbinImg                                                          //
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

void sippInitPipeline (ptSippPipelineSuper ptSPipe,
                       u32                 sliceFirst,
                       u32                 sliceLast,
                       u32                 sliceSize,
                       u8 *                mbinImg)
{
    u32           uIdx;
    u32           localSliceFirst, localSliceLast;
    pSippPipeline pPipe = (pSippPipeline)ptSPipe;

    localSliceFirst      = sliceFirst;
    localSliceLast       = sliceLast;

    if (localSliceFirst > (SIPP_NUM_SVUS - 1))
    {
        sippPalTrace (SIPP_API_TL_ERROR, "SIPP: sippInitPipeline : Bad value of first slice %d, setting to %d\n", localSliceFirst, SIPP_NUM_SVUS - 1);
        localSliceFirst = SIPP_NUM_SVUS - 1;
        if (localSliceFirst > localSliceLast)
        {
            sippPalTrace (SIPP_API_TL_ERROR, "SIPP: sippInitPipeline : Modifying last slice to %d due to change in first slice\n", localSliceFirst);
            localSliceLast = localSliceFirst;
        }
    }
    if (localSliceLast > (SIPP_NUM_SVUS - 1))
    {
        sippPalTrace (SIPP_API_TL_ERROR, "SIPP: sippInitPipeline : Bad value of last slice %d, setting to %d\n", localSliceLast, SIPP_NUM_SVUS - 1);
        localSliceLast = SIPP_NUM_SVUS - 1;
    }

    pPipe->svuCmd        = 0; // Tell Master-Shave to stay IDLE

    // Reset mem pool before we start creating the pipeline !
    pPipe->tHeapMCB.pipeIdx   = ptSPipe->uPipeIdx;
    pPipe->tHeapMCB.numVPools = vPoolFilterLineBuf0;

    sippInitPhysicalPoolPipe (&pPipe->tHeapMCB);
    sippInitSchedPool (&pPipe->tHeapMCB,
                       localSliceFirst,
                       localSliceLast);

    pPipe->svuSyncMtx[0]    = (ptSPipe->uPipeIdx << 0x1) + 0;
    pPipe->svuSyncMtx[1]    = (ptSPipe->uPipeIdx << 0x1) + 1;

    pPipe->linesPerIter     = 1;
    pPipe->setIterMask      = 1;

    for (uIdx = 0; uIdx < EXE_NUM; uIdx++)
    {
        pPipe->hwSippFirst [uIdx] = -1; // index
    }

    pPipe->sliceSz          = sliceSize; // sliceSize is duplicated, but allows faster access
    pPipe->gi.sliceSize     = sliceSize; // from both SippPipeline and SippFitler data structure
    pPipe->gi.sliceFirst    = localSliceFirst;
    pPipe->gi.sliceLast     = localSliceLast;
    pPipe->gi.pl            = pPipe;
    #ifdef SIPP_PC
    pPipe->gi.numShaves     = localSliceLast - localSliceFirst + 1;
    #else
    pPipe->gi.numShaves     = (mbinImg) ? localSliceLast - localSliceFirst + 1 : 0;
    #endif

    pPipe->mbinImg          = mbinImg;
    pPipe->pCmxMap          = (pSippCmxBufferMap)NULL;

    #ifdef SIPP_PC
    pPipe->dbgLevel         = 1;
    #else
    pPipe->dbgLevel         = 0;
    #endif

    ptSPipe->eState         = eSIPP_PIPE_WAIT_FINALISE;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippAddFilterToPipe                                              //
//                                                                                //
//  DESCRIPTION: Add filter to specified pipeline                                 //
//                                                                                //
//  INPUTS:      pPipe - pointer to the pipeline                                  //
//               sliceFirst                                                       //
//               sliceLast                                                        //
//               sliceSize                                                        //
//               mbinImg                                                          //
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

void sippAddFilterToPipe (pSippPipeline pPipe,
                          pSippFilter   pFilter,
                          u32           flags,
                          u32           out_W,
                          u32           out_H,
                          u32           num_pl,
                          u32           bpp,
                          FnSvuRun      funcSvuRun,
                          const char *  name)
{
    UNUSED (name); //hush the compiler warning

    u32 unitNo = (u32)funcSvuRun;

    // Const params
    pFilter->pPipe            = pPipe;
    pFilter->gi               = &pPipe->gi;
    pFilter->id               = pPipe->nFilters; // Attach a filter number (debug mostly)
    pFilter->outputW          = out_W;
    pFilter->outputH          = out_H;
    pFilter->nPlanes[0]       = num_pl;
    pFilter->bpp[0]           = bpp << 3;
    pFilter->bytesPerPix      = bpp >> 1; //1Bpp:0  2Bpp:1 4Bpp:2
    pFilter->linesPerIter     = 1;
    pFilter->linesThisIter[0] = 1;
    pFilter->linesThisIter[1] = 1;

    pFilter->firstOutSlc  = pPipe->gi.sliceFirst; //default first output chunk to FIRST_SLICE

    if (unitNo < SIPP_SVU_ID)
    {
        // Then it's a HW block (SW filters will send a ref to call function)
        pFilter->unit = unitNo;

        // DMA special checks
        if (pFilter->unit == SIPP_DMA_ID)
        {
            pPipe->filtersDMA[pPipe->nFiltersDMA] = pFilter;
            pPipe->nFiltersDMA++;
            if (pPipe->nFiltersDMA > SIPP_MAX_DMA_FILTERS_PER_PIPELINE)
            {
                sippError (pPipe->errorStatus, E_TOO_MANY_DMAS);
                return;
            }
        }
        else
        {
            // Special code to deal with legacy use of the SIPP_UPFIRDN_ID macro
            if (pFilter->unit == SIPP_UPFIRDN_ID)
            {
                if (pPipe->hwSippFltCnt[pFilter->unit])
                {
                    if (pPipe->hwSippFltCnt[SIPP_UPFIRDN1_ID] == 0)
                    {
                        pFilter->unit = SIPP_UPFIRDN1_ID;
                    }
                    else if (pPipe->hwSippFltCnt[SIPP_UPFIRDN2_ID] == 0)
                    {
                        pFilter->unit = SIPP_UPFIRDN2_ID;
                    }
                }
            }

            pPipe->hwSippFltCnt[pFilter->unit]++; // Increment number of filters on this unit !

            #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
            // All HW filter blocks can only be used single-instance
            if (pFilter->unit <= SIPP_MAX_FILTER_ID)
            {
                if (pPipe->hwSippFltCnt[pFilter->unit] > 1)
                {
                    sippError (pPipe->errorStatus, E_INVLD_MULTI_INSTANCE);
                    return;
                }
            }
            #endif
        }
    }
    else
    {
        // Shave filter
        pFilter->unit       = SIPP_SVU_ID;
        pFilter->funcSvuRun = funcSvuRun;

        // IMPORTANT: "paramSz" stays as defined by USER
        // Add to Shave filter list
        pPipe->filtersSvu[pPipe->nFiltersSvu] = pFilter;
        pPipe->nFiltersSvu++;
    }

    pFilter->flags = flags;

    #ifndef RT_DBG
    if ((pFilter->unit == SIPP_SVU_ID) ||
        (pFilter->unit == SIPP_DMA_ID))
    #endif
    {
        pFilter->flags |= SIPP_REQ_SW_VIEW;
    }

    #if defined(SIPP_F_DUMPS)
    if (name)
    {
        strcpy(pPipe->filtName[pFilter->id], name);
    }
    #endif

    pFilter->numOBufs   = 1;

    // Allocate oBufs
    pFilter->oBufs[0]->numLines              = BUFF_HUGE_SZ;
    pFilter->oBufs[0]->filtOutHeight         = pFilter->outputH;
    pFilter->oBufs[0]->mostRecentLine        = -1;

    // Increment number of filters !
    pPipe->nFilters++;

    #ifdef SIPP_SCRT_ENABLE_OPIPE
    // Set default IRQ rate control to 2^3 = 8 lines
    pFilter->irqRatePow = 3;
    #endif

}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippElaboratePipeline                                            //
//                                                                                //
//  DESCRIPTION: Validate a pipeline, allocate its line buffers, compute padding  //
//               values and helper data members and create its schedule           //
//                                                                                //
//  INPUTS:      pPipe - Pipeline to elaborate                                    //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       Since this causes creation of a schedule (taking a lot of time)  //
//               it is handled in a manner which could be non-blocking if an RTOS //
//               was used                                                         //
//                                                                                //
//  CONTEXT:     Unknown if there are any constraints here as yet                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

eSIPP_STATUS sippElaboratePipeline (pSippPipeline pPipe)
{
    eSIPP_STATUS        eRetCode   = eSIPP_STATUS_OK;
    ptSippPipelineSuper pSuperPipe = (ptSippPipelineSuper)pPipe;

    sippIssueCommand (pPipe,
                      eSIPP_CMD_FINALISE_PIPE_SW,
                      (SippCommandData)NULL);

    // Need to check if finalising the pipe actually worked
    if (pSuperPipe->eState != eSIPP_PIPE_ACTIVE )
    {
        eRetCode = eSIPP_STATUS_INTERNAL_ERROR;
    }

    return eRetCode;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippTermInternal                                                 //
//                                                                                //
//  DESCRIPTION: Teardown the sipp framework freeing all its resource             //
//                                                                                //
//  INPUTS:      None                                                             //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       None                                                             //
//                                                                                //
//  CONTEXT:     Unknown if there are any constraints here as yet                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

eSIPP_STATUS sippTermInternal (void)
{
    eSIPP_STATUS eRetCode = eSIPP_STATUS_OK;

    return eRetCode;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippFreePipeResource                                             //
//                                                                                //
//  DESCRIPTION: Freeing all resource allocated to specified pipeline             //
//                                                                                //
//  INPUTS:      pPipe - Pipeline to free resource from                           //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       None                                                             //
//                                                                                //
//  CONTEXT:     Unknown if there are any constraints here as yet                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

eSIPP_STATUS sippFreePipeResource (pSippPipeline pPipe)
{
    eSIPP_STATUS eRetCode = eSIPP_STATUS_OK;
    u32          vPoolIdx;

    // Free all allocations through virtual pools onto heaps
    for (vPoolIdx = 0; vPoolIdx < (u32)pPipe->tHeapMCB.numVPools; vPoolIdx++)
    {
        sippMemFreeList (&pPipe->tHeapMCB,
                         (SippVirtualPool)vPoolIdx);
    }

    // In the future, check if mutexes or other shared resources may need to be freed here
    // If this pipe is currently loaded on the HW, we need to remove it
    sippIssueCommand ((pSippPipeline)pPipe,
                      eSIPP_CMD_DELETE_PIPE_SW,
                      (SippCommandData)NULL);

    return eRetCode;
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippRescheduleRequest                                            //
//                                                                                //
//  DESCRIPTION: Create a new schedule for specified pipeline                     //
//                                                                                //
//  INPUTS:      pPipe - Pipeline to reschedule                                   //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       Since this causes creation of a schedule (taking a lot of time)  //
//               it is handled in a manner which could be non-blocking if an RTOS //
//               was used                                                         //
//                                                                                //
//  CONTEXT:     Unknown if there are any constraints here as yet                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

eSIPP_STATUS sippRescheduleRequest (pSippPipeline pPipe)
{
    eSIPP_STATUS eRetCode = eSIPP_STATUS_OK;
    ptSippPipelineSuper pSuperPipe = (ptSippPipelineSuper)pPipe;

    sippIssueCommand ((pSippPipeline)pPipe,
                      eSIPP_CMD_RESCHEDULE_PIPE_SW,
                      (SippCommandData)NULL);

    // Need to check if finalising the pipe actually worked
    if (pSuperPipe->eState != eSIPP_PIPE_ACTIVE )
    {
        eRetCode = eSIPP_STATUS_INTERNAL_ERROR;
    }

    return eRetCode;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippRunItersRequest                                              //
//                                                                                //
//  DESCRIPTION: Internal function to call HW filters and shaves to run a number  //
//               of iterations of the pipeline                                    //
//                                                                                //
//  INPUTS:      pPipe - Pipeline to reschedule                                   //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       Since this needs to be handled along with competing requests     //
//               issue a command to the access control scheduler                  //
//                                                                                //
//  CONTEXT:     Unknown if there are any constraints here as yet                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

eSIPP_STATUS sippRunItersRequest (pSippPipeline pPipe,
                                  u32           uNumIters)
{
    eSIPP_STATUS eRetCode       = eSIPP_STATUS_OK;
    u32          uLocalNumIters = uNumIters;

    sippIssueCommand ((pSippPipeline)pPipe,
                      eSIPP_CMD_PROCESS_ITERS_HW,
                      (SippCommandData)&uLocalNumIters);

    return eRetCode;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippResetFilterVariables                                         //
//                                                                                //
//  DESCRIPTION: Reset some filter varables modified during the porcessing of a   //
//               frame to their correct initial values                            //
//                                                                                //
//  INPUTS:      pPipe - Pipeline                                                 //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       None                                                             //
//                                                                                //
//  CONTEXT:     Unknown if there are any constraints here as yet                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

eSIPP_STATUS sippResetFilterVariables (pSippPipeline pPipe)
{
    eSIPP_STATUS eRetCode = eSIPP_STATUS_OK;
    u32          filtIdx  = 0;
    SippFilter * ptrFilt;

    for (filtIdx = 0; filtIdx < pPipe->nFilters; filtIdx++)
    {
        ptrFilt = pPipe->filters[filtIdx];

        ptrFilt->firstRunRollLines = ptrFilt->firstRunNumLines = ptrFilt->firstIterLines;
    }

    return eRetCode;

}
