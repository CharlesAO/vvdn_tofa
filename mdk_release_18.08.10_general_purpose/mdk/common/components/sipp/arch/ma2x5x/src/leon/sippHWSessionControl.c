///
/// @file      sippHWSessionControl.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Establishes context for and makes calls to sipp HW and shaves
///            This file provides the hardware facing aspect to the interface.
///            It is part of a set with sippPipeSessionControl.c which provides
///            pipeline funtionality
///            Platform(s) supported : ma2x5x
///

/////////////////////////////////////////////////////////////////////////////////
//  Header files
/////////////////////////////////////////////////////////////////////////////////

#include <sipp.h>
#include <sippInternal.h>

#if defined(__sparc)
#include <DrvSvuDefines.h>
#endif

////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////

static void sippHWSessionInitPipe (pSippPipeline pPipe,
                                   u32           uHWPipeID);

#ifdef SIPP_SUPPORT_PROCESS_ITERS
static void sippHWSessionSaveContext (pSippPipeline pPipe,
                                      bool          bHWCtx);
#endif

static void sippHWSessionSetContext (pSippPipeline pPipe,
                                     u32           uHWPipeID);

static void sippHWSessionConfigSvus (pSippPipeline pPipe);

////////////////////////////////////////////////////
// Global variables
////////////////////////////////////////////////////

#ifdef SIPP_PC
SIPP_HW_SESSION         gSippHW;
#else
static SIPP_HW_SESSION  gSippHW;
#endif
pSIPP_HW_SESSION        pgSippHW;

extern u32              sippGlobalOBFLIncStatus;

////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippHWSessionInit                                            //
//                                                                            //
//  DESCRIPTION: Initialise SIPP HW                                           //
//                                                                            //
//  INPUTS:      None                                                         //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       Turns on balanced arbitration between AMC ports              //
//               Soft reset & enable SIPP units                               //
//               Put SIPP units in sync mode - deprecated                     //
//               Increase Leon's priority when accessing CMX-slices           //
//               Reset & enable CMX-DMA                                       //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void sippHWSessionInit ()
{
    u32 uIdx;

    #if defined(MYRIAD2)
    //(BB)Set bit per Slice, turns on balanced arbitration between AMC ports
    SET_REG_WORD(CMX_SLICE_ARB_CTRL,    0x0000ffff);
    #endif

    #if defined(MYRIAD2) || defined(SIPP_PC)

    //SOFTRST doesn't reset properly the LSC MeshReadClient
    //        so beter not use it !
    //Reset SIPP units:
    SET_REG_WORD(SIPP_SOFTRST_ADR,     0xFFFFFFFF);
    NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
    NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
    // Disable all SIPP units:
    SET_REG_WORD(SIPP_CONTROL_ADR,     0x00000000);

    #endif

    #if defined(MYRIAD2)

    // TODO - Analyse what to do here */
    //Critical to avoid huge (thousands of c.c.) Leon STALL:
    //each 16bit field shows priority from other 15 slices (1 bit is don't care there...)
    SET_REG_WORD(CMX_SLICE_ISI_PRIORITY0_CTRL, 0x80008000); //hi=slice_1, lo=slice_0
    SET_REG_WORD(CMX_SLICE_ISI_PRIORITY1_CTRL, 0x80008000); //hi=slice_3, lo=slice_2
    SET_REG_WORD(CMX_SLICE_ISI_PRIORITY2_CTRL, 0x80008000);
    SET_REG_WORD(CMX_SLICE_ISI_PRIORITY3_CTRL, 0x80008000);
    SET_REG_WORD(CMX_SLICE_ISI_PRIORITY4_CTRL, 0x80008000);
    SET_REG_WORD(CMX_SLICE_ISI_PRIORITY5_CTRL, 0x80008000);
    SET_REG_WORD(CMX_SLICE_ISI_PRIORITY6_CTRL, 0x80008000);
    SET_REG_WORD(CMX_SLICE_ISI_PRIORITY7_CTRL, 0x80008000);
    #endif

    #ifdef SIPP_USE_CMXDMA_DRIVER
    sippDrvCmxDmaInit ( );
    #else
    sippCmxDmaInit ( );
    #endif

    pgSippHW         = &gSippHW;
    pgSippHW->eState = eSIPP_HW_INACTIVE;
    sippIsrSetup ( );
    sippIntBarrierSetup (0x1);

    for (uIdx = 0x0; uIdx < SIPP_MAX_SUPPORTED_HW_PIPELINES; uIdx++)
    {
        pgSippHW->pSippCurrHWPipe[uIdx]   = (pSippPipeline)NULL;
    }

    for (uIdx = 0x0; uIdx < SIPP_NUM_SVUS; uIdx++)
    {
        pgSippHW->pSippCurrSvuPipe[uIdx] = (pSippPipeline)NULL;
    }

    pgSippHW->uNumCurrHwPipes = 0x0;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippHWSessionAddActiveLists                                      //
//                                                                                //
//  DESCRIPTION: Add a pipeline to the list of active pipes on its associaited    //
//               resources                                                        //
//                                                                                //
//  INPUTS:      pPipe     - Pointer to pipeline to be added                      //
//               uHWPipeID - HW pipe ID as chosen by access scheduler - Note this //
//                           is SIPP_HW_INVALID_PIPE_ID if pipeline does not use  //
//                           SIPP HW acceleration                                 //
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

void sippHWSessionAddActiveLists (pSippPipeline pPipe,
                                  u32           uHWPipeID)
{

    if (uHWPipeID != SIPP_INVALID_HW_PIPE_ID)
    {
        SIPP_PAL_CRIT_STATE CS;
        u32                 uIdx;

        sippPalCriticalSectionBegin (&CS);

        for (uIdx = pPipe->gi.sliceFirst; uIdx <= pPipe->gi.sliceLast; uIdx++)
        {
            pgSippHW->pSippCurrSvuPipe[uIdx] = pPipe;
        }

        pgSippHW->pSippCurrHWPipe[uHWPipeID]   = pPipe;
        pgSippHW->pSippLoadedHWPipe[uHWPipeID] = pPipe;
        pgSippHW->uNumCurrHwPipes++;

        for (uIdx = 0; uIdx < SIPP_HW_FILTER_MASK_SIZE; uIdx++)
        {
            pgSippHW->uHWFilterUsageBitMask[uIdx] |= pPipe->uHWFilterUsageBitMask[uIdx];
            // Note - it is only at this point that the scheduler should be free to run again!
        }

        sippPalCriticalSectionEnd (CS);
    }
    #ifdef SIPP_PC
    else
    {
        pgSippHW->pSippLoadedHWPipe[0] = pPipe;
    }
    #endif

    pPipe->HWPipeID = uHWPipeID;
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippHWSessionRemoveActiveLists                                   //
//                                                                                //
//  DESCRIPTION: Remove a pipeline from the list of active pipes on its           //
//               associated resources                                             //
//                                                                                //
//  INPUTS:      pPipe     - Pointer to pipeline to be removed                    //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       This should be called on a STOP command - or a pipeline complete //
//               command - i.e. free this HW                                      //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippHWSessionRemoveActiveLists (pSippPipeline pPipe,
                                     u32           uHWPipeID)
{
    u32 uIdx;

    if (uHWPipeID != SIPP_INVALID_HW_PIPE_ID)
    {
        SIPP_PAL_CRIT_STATE CS;

        sippStopSvus (pPipe);

        sippPalCriticalSectionBegin (&CS);

        // Backup the last pipes to run on each SHAVE
        sippPalMemcpy (pgSippHW->pSippPrevSvuPipe,
                       pgSippHW->pSippCurrSvuPipe,
                       sizeof(pSippPipeline) * SIPP_NUM_SVUS);

        for (uIdx = pPipe->gi.sliceFirst; uIdx <= pPipe->gi.sliceLast; uIdx++)
        {
            pgSippHW->pSippCurrSvuPipe[uIdx] = (pSippPipeline)NULL;
        }

        for (uIdx = 0x0; uIdx < SIPP_MAX_SUPPORTED_HW_PIPELINES; uIdx++)
        {
            if (pgSippHW->pSippCurrHWPipe[uIdx] == pPipe)
            {
                u32 uUsageIdx = 0;
                u32 regVal;

                pgSippHW->pSippCurrHWPipe[uIdx] = (pSippPipeline)NULL;

                for (uUsageIdx = 0; uUsageIdx < SIPP_HW_FILTER_MASK_SIZE; uUsageIdx++)
                {
                    pgSippHW->uHWFilterUsageBitMask[uUsageIdx] &= (~pPipe->uHWFilterUsageBitMask[uUsageIdx]);

                    regVal  = GET_REG_WORD_VAL(SIPP_INT0_ENABLE_ADR);
                    regVal &= ~pPipe->uHWFilterUsageBitMask[uUsageIdx];
                    regVal &= ~pPipe->uHWFilterIBufUsageMask;
                    SET_REG_WORD(SIPP_INT0_ENABLE_ADR, regVal);
                    regVal  = GET_REG_WORD_VAL(SIPP_INT1_ENABLE_ADR);
                    regVal &= ~pPipe->uHWFilterUsageBitMask[uUsageIdx];
                    regVal &= ~pPipe->uHWFilterOBufUsageMask;
                    SET_REG_WORD(SIPP_INT1_ENABLE_ADR, regVal);
                    regVal  = GET_REG_WORD_VAL(SIPP_INT2_ENABLE_ADR);
                    regVal &= ~pPipe->uHWFilterUsageBitMask[uUsageIdx];
                    regVal &= ~pPipe->uHWFilterOBufUsageMask;
                    SET_REG_WORD(SIPP_INT2_ENABLE_ADR, regVal);

                    SET_REG_WORD(SIPP_INT0_CLEAR_ADR, pPipe->uHWFilterUsageBitMask[uUsageIdx]);
                    SET_REG_WORD(SIPP_INT0_CLEAR_ADR, pPipe->uHWFilterIBufUsageMask);
                    SET_REG_WORD(SIPP_INT1_CLEAR_ADR, pPipe->uHWFilterUsageBitMask[uUsageIdx]);
                    SET_REG_WORD(SIPP_INT1_CLEAR_ADR, pPipe->uHWFilterOBufUsageMask);
                    SET_REG_WORD(SIPP_INT2_CLEAR_ADR, pPipe->uHWFilterUsageBitMask[uUsageIdx]);
                    SET_REG_WORD(SIPP_INT2_CLEAR_ADR, pPipe->uHWFilterOBufUsageMask);

                    sippGlobalOBFLIncStatus &= ~pPipe->uHWFilterUsageBitMask[uUsageIdx];
                    sippGlobalOBFLIncStatus &= ~pPipe->uHWFilterOBufUsageMask;

                }

                pgSippHW->uNumCurrHwPipes--;

                break;

            }
        }

        sippPalCriticalSectionEnd (CS);
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippHWSessionRemoveLoadedPipe                                    //
//                                                                                //
//  DESCRIPTION: Remove a pipeline from the list of HW loaded pipes               //
//               associated resources                                             //
//                                                                                //
//  INPUTS:      pPipe     - Pointer to pipeline to be removed                    //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       The pSippLoadedHWPipe is checked by the ISR handler              //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippHWSessionRemoveLoadedPipe (pSippPipeline pPipe)
{
    u32                 uIdx;
    SIPP_PAL_CRIT_STATE CS;

    sippPalCriticalSectionBegin (&CS);

    for (uIdx = 0x0; uIdx < SIPP_MAX_SUPPORTED_HW_PIPELINES; uIdx++)
    {
        if (pgSippHW->pSippLoadedHWPipe[uIdx] == pPipe)
        {
            pgSippHW->pSippLoadedHWPipe[uIdx] = (pSippPipeline)NULL;
        }
    }

    sippPalCriticalSectionEnd (CS);
}

#ifdef RT_DBG

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippHWGetPipeIdx                                                 //
//                                                                                //
//  DESCRIPTION: Remove a pipeline from the list of HW loaded pipes               //
//               associated resources                                             //
//                                                                                //
//  INPUTS:      pPipe     - Pointer to pipeline to be removed                    //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       The pSippLoadedHWPipe is checked by the ISR handler              //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippHWGetPipeIdx (pSippPipeline pPipe)
{
    u32                 uIdx;
    SIPP_PAL_CRIT_STATE CS;

    sippPalCriticalSectionBegin (&CS);

    for (uIdx = 0x0; uIdx < SIPP_MAX_SUPPORTED_HW_PIPELINES; uIdx++)
    {
        if (pgSippHW->pSippLoadedHWPipe[uIdx] == pPipe)
        {
            break;
        }
    }

    sippPalCriticalSectionEnd (CS);

    return uIdx;
}

#endif

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippHWSessionCommand                                             //
//                                                                                //
//  DESCRIPTION: Send a command to be carried out on SIPP HW and/or SHAVE         //
//               resource                                                         //
//                                                                                //
//  INPUTS:      uHWPipeID - ID within active HW array to be used                 //
//               eEvent    - Command type                                         //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       If we re-use a shave we would assume that the old pipeline       //
//               associated with that shave is terminated so if it had any        //
//               resource that should be released. However, that is for the pipe  //
//               session aspect of this controller - we do not care about that    //
//               here, bar noting that a re-use of the pSippPipeline structure    //
//               for a new pipeline will not cause any issues so long as          //
//               sippHWSessionRemoveActiveLists was called for the previous       //
//               incarnation of the pipe first                                    //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippHWSessionCommand (pSippPipeline                    pPipe,
                           u32                              uHWPipeID,
                           eSIPP_ACCESS_SCHEDULER_EVENT     eEvent,
                           SIPP_ACCESS_SCHEDULER_EVENT_DATA pData)
{
    #ifdef SIPP_SUPPORT_PROCESS_ITERS

    u32 uShaveIdx, uHWFiltIdx;

    // First step is check the shaves - remember we may have to do more than one
    // context save ...
    for (uShaveIdx = pPipe->gi.sliceFirst; uShaveIdx <= pPipe->gi.sliceLast; uShaveIdx++)
    {
        if (pgSippHW->pSippPrevSvuPipe[uShaveIdx])
        {
            if (pgSippHW->pSippPrevSvuPipe[uShaveIdx] != pPipe)
            {
                sippHWSessionSaveContext (pgSippHW->pSippPrevSvuPipe[uShaveIdx],
                                          FALSE);
            }
        }
    }

    // Next check the HW resource
    // The access scheduler will have specified which HW Pipe ID to use so only that
    // needs to be checked (this pipe must be capable of running alongside other active HW pipes
    // so they must not be sharing hardware resource and thus those pipes need not have their
    // context saved)

    if (uHWPipeID != SIPP_INVALID_HW_PIPE_ID)
    {
        for (uHWFiltIdx = 0; uHWFiltIdx < SIPP_MAX_FILTER_ID; uHWFiltIdx++)
        {
            if ((pgSippHW->pSippFilterLoadedPipe[uHWFiltIdx]) &&
                (pgSippHW->pSippFilterLoadedPipe[uHWFiltIdx] != pPipe))
            {
                sippHWSessionSaveContext (pgSippHW->pSippFilterLoadedPipe[uHWPipeID],
                                          TRUE);
                break;
            }
        }
    }

    #endif // SIPP_SUPPORT_PROCESS_ITERS

    sippHWSessionInitPipe (pPipe,
                           uHWPipeID);

    // Establish context for this new pipeline
    sippHWSessionSetContext (pPipe,
                             uHWPipeID);

    // Make a call to the runtime
    // A blocking runtime will demand that this call does not return until
    // the work is done!
    pPipe->pfnSippRuntime (pPipe,
                           eEvent,
                           pData);

}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippHWSessionInitPipe                                            //
//                                                                                //
//  DESCRIPTION: Call any special inits of the SIPP HW flagged for the specified  //
//               pipeline                                                         //
//                                                                                //
//  INPUTS:      pPipe - Pointer to pipeline struct                               //
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

void sippHWSessionInitPipe (pSippPipeline pPipe,
                            u32           uHWPipeID)
{
    UNUSED(uHWPipeID); //hush the compiler warning
    #ifdef MYRIAD2
    SIPP_PAL_CRIT_STATE CS;

    pPipe->pfnSippRuntimeClaimHWResource (pPipe);

    // Reset used filters
    sippPalCriticalSectionBegin (&CS);
    CLR_REG_BITS_MASK(MSS_SIPP_RSTN_CTRL_ADR, pPipe->uHWFilterUsageBitMask[0]);
    SET_REG_BITS_MASK(MSS_SIPP_RSTN_CTRL_ADR, pPipe->uHWFilterUsageBitMask[0]);
    sippPalCriticalSectionEnd (CS);


    #ifdef SIPP_SCRT_ENABLE_OPIPE

    if ((pPipe->pfnSippRuntime == sippGenericRuntime) ||
        (pPipe->pfnSippRuntime == sippMLPIRuntime))
    {
        // By default such a pipe wants to use interrupt barriers for performance
        #ifdef SIPP_ENABLE_RT_COEXISTENCE
        pgSippHW->useIntBar[uHWPipeID] = pgSippHW->noIntBarSwitchPend ? 0x0 : 0x1;

        // TBD : Qualify this by deciding if there are HW filters - and in enough
        //       numbers to justify this - we could do this in analysePipe up front

        u32 pipeIdx;
        for (pipeIdx = 0; pipeIdx < SIPP_MAX_SUPPORTED_HW_PIPELINES; pipeIdx++)
        {
            if (pgSippHW->pSippLoadedHWPipe[pipeIdx])
            {
                if (pgSippHW->useIntBar[pipeIdx] == 0)
                {
                    // Set pipe to not use interrupt barriers
                    pgSippHW->useIntBar[uHWPipeID] = 0x0;
                }
            }
        }

        #else
        pgSippHW->useIntBar[uHWPipeID] = 0x1;
        #endif
    }
    else
    {
        pgSippHW->useIntBar[uHWPipeID] = 0x0;
    }

    sippPalTrace (SIPP_RUNTIME_TL_INFO, "Pipe %08lx intsetup %lu\n", (u32)pPipe, pgSippHW->useIntBar[uHWPipeID]);

    if (pgSippHW->isrSetup != pgSippHW->useIntBar[uHWPipeID])
    {
        sippIntBarrierSetup (pgSippHW->useIntBar[uHWPipeID]);
    }

    #endif
    #endif

}

#ifdef SIPP_SUPPORT_PROCESS_ITERS

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippHWSessionSaveContext                                         //
//                                                                                //
//  DESCRIPTION: Save the SIPP HW for the specified pipeline                      //
//                                                                                //
//  INPUTS:      pPipe - Pointer to pipeline struct                               //
//               bHWCtx - Set TRUE to save off HW filter aspects - otherwise      //
//                        save SHAVE aspects                                      //
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

void sippHWSessionSaveContext (pSippPipeline pPipe,
                               bool          bHWCtx)
{

    #if defined(MYRIAD2) || defined(SIPP_PC)

    // Check if this is a HW context save
    // This will save off all the filter contexts - not just those which overlap with the new pipeline
    // though I assume that is ok at the moment
    if (bHWCtx)
    {
        sippCoreHwInitialSave (pPipe);
    }
    else
    {
        // If prev pipeline finished its execution, it gets (->iteration = 0),
        // so all it's relevant output is back in DDR, no need to save back the CMX buffers!
        if (pPipe->iteration != 0)
        {
            sippDataSectAction (pPipe, pPipe->pushCmd);
        }
    }
    #endif

}

#endif // SIPP_SUPPORT_PROCESS_ITERS

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippHWSessionSetContext                                          //
//                                                                                //
//  DESCRIPTION: Set-up the SIPP HW for the specified pipeline                    //
//                                                                                //
//  INPUTS:      pPipe - Pointer to pipeline struct                               //
//               bCtxSwitch - Set TRUE if this operation is part of a cintext     //
//                            switch                                              //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippHWSessionSetContext (pSippPipeline pPipe,
                              u32           uHWPipeID)
{

    sippHWSessionAddActiveLists (pPipe,
                                 uHWPipeID);

    if (uHWPipeID != SIPP_INVALID_HW_PIPE_ID)
    {
        // What is this actually doing - setting the slice size for only a specified group of slices?
        // This needs very closely checked out
        // If it effects other simultaneous pipelines then the access scheduler should have taken
        // this into account before deciding that they could run together

        #ifdef SIPP_SLICE0_ADDRESS
        u32 value = pPipe->sliceSz |
                    (pPipe->gi.sliceFirst<<24) |
                    (pPipe->gi.sliceLast <<28);
        #else
        u32 value = pPipe->sliceSz |
                    ((SIPP_NUM_SVUS - 1)<<28);

        #endif

        SET_REG_WORD(SIPP_SLC_SIZE_ADR, value);
    }

    // Bring new code and new win-reg settings if needed
    sippHWSessionConfigSvus (pPipe);

    // Clear all execNo, as LUT HW load looks at exec number !
    if (pPipe->iteration == 0)
    {
        pPipe->pfnSippRunFrameReset (pPipe);
    }

    if (uHWPipeID != SIPP_INVALID_HW_PIPE_ID)
    {
        sippCoreHwInitialLoad (pPipe);
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippHWSessionConfigSvus                                          //
//                                                                                //
//  DESCRIPTION: Set-up a pipeline's presence on the SHAVE suite                  //
//                                                                                //
//  INPUTS:      pPipe - Pointer to pipeline struct                               //
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

void sippHWSessionConfigSvus (pSippPipeline pPipe)
{
    pSippPipeline pStopPipeList[SIPP_NUM_SVUS] = {0};
    u32           uNumPipesToStop;
    u32           uPipeIdx;
    bool          bLoadNewImage, bPrevImageLoaded;

    // Build a list of the previously effected pipes
    for (uPipeIdx = pPipe->gi.sliceFirst, uNumPipesToStop = 0x0, bLoadNewImage = FALSE, bPrevImageLoaded = FALSE; uPipeIdx <= pPipe->gi.sliceLast; uPipeIdx++)
    {
        pSippPipeline prevPl = pgSippHW->pSippPrevSvuPipe[uPipeIdx];
        if (prevPl)
        {
            if (prevPl->nFiltersSvu)
            {
                u32 uStopIdx;
                bool   bFoundInList;

                for (uStopIdx = 0x0, bFoundInList = FALSE; uStopIdx < uNumPipesToStop; uStopIdx++)
                {
                    if (pStopPipeList[SIPP_NUM_SVUS - 1] == prevPl)
                    {
                        bFoundInList = TRUE;
                        break;
                    }
                }

                if (bFoundInList == FALSE)
                {
                    pStopPipeList[uNumPipesToStop++] = prevPl;
                }
            }

            if (prevPl->mbinImg != pPipe->mbinImg)
            {
                bLoadNewImage = TRUE;
                if (prevPl->mbinImg)
                {
                    bPrevImageLoaded = TRUE;
                }
            }
        }
    }

    //1st: stop shaves (if prev pipeline exists and had SW filters)
    //     if prev pipeline does not exist no need to stop SHAVEs
    if (uNumPipesToStop)
    {
        for (uPipeIdx = 0x0; uPipeIdx < uNumPipesToStop; uPipeIdx++)
        {
            sippStopSvus (pStopPipeList[uPipeIdx]);
        }
    }

    // If new pipeline has SW filters:
    if ((pPipe->nFiltersSvu) ||
        (pPipe->flags & PLF_REQUIRES_SW_PADDING))
    {
        //2nd: if old pipe had SW filters and was different shave img,
        //     it means we need to load new image
        if (bPrevImageLoaded == FALSE)
        {
            sippSetupSvus (pPipe);
        }
        else if (bLoadNewImage)
        {   //meaning && (prevPl != NULL)
            sippSetupSvus (pPipe);
        }

        // 3rd: Leon get sync mutexes
        sippInitSyncMutexes (pPipe);

        //Invalidate L1-instr caches always ! TBD: update this...
        #ifdef MYRIAD2 //sparc-Myriad2
        void invalidateSvuInstL1Cache(u32 first, u32 last);
        invalidateSvuInstL1Cache(pPipe->gi.sliceFirst, pPipe->gi.sliceLast);
        #endif

        // 4th: update Shave's sipp_pl reference, and start again
        sippKickSvus (pPipe);
    }
}


