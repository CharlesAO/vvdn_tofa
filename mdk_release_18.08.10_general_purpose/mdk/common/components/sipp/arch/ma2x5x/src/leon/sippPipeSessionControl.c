///
/// @file      sippPipeSessionControl.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Processes commands from sipp access scheduler
///            Establishes context for and makes calls to sipp HW and shaves
///            This file provides the pipeline aspect to the interface.
///            It is part of a set with SippHWSessionControl.c which provides the HW
///            functionality
///            Platform(s) supported : ma2x5x
///

/////////////////////////////////////////////////////////////////////////////////
//  Header files
/////////////////////////////////////////////////////////////////////////////////

#include <sipp.h>
#include <sippInternal.h>

/////////////////////////////////////////////////////////////////////////////////
//  Global Variables
/////////////////////////////////////////////////////////////////////////////////
extern tSippFramework   gSippFramework;
static eSippPipeState   geControlPipeState = eSIPP_PIPE_WAIT_INIT;
extern pSIPP_HW_SESSION pgSippHW;

/////////////////////////////////////////////////////////////////////////////////
//  Extern Function Prototypes
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
//  Code
/////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippPipeSessionControlInit                                       //
//                                                                                //
//  DESCRIPTION: Initialise the sipp session pipeline interface                   //
//                                                                                //
//  INPUTS:      None.                                                            //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       Called once per session                                          //
//                                                                                //
//  CONTEXT:     This function may be called from non-interrupt context           //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippPipeSessionControlInit ( )
{
    // Initialise the global resource control
    sippCoreResourceInit ();
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippPipeSessionControl                                           //
//                                                                                //
//  DESCRIPTION: Process action requests from the SIPP access scheduler           //
//                                                                                //
//  INPUTS:      eEvent     - Information on the action request                   //
//               uPipeIdx   - SW Pipeline to which the request applies            //
//               uHWPipeIdx - HW Pipeline ID to which this is pipe is to be       //
//                            assigned (ignored if not an event requiring HW )    //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       This function will only be called when the scheduler is sure     //
//               execution of the request may begin immediately                   //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippPipeSessionControl (eSIPP_ACCESS_SCHEDULER_EVENT     eEvent,
                             SIPP_ACCESS_SCHEDULER_EVENT_DATA pData,
                             u32                              uPipeIdx,
                             u32                              uHWPipeIdx)
{
    ptSippPipelineSuper pPipe;
    bool                bCmdComplete = FALSE;
    eSippPipeState      eState;

    pPipe =  gSippFramework.pPipelines[uPipeIdx];

    if (uPipeIdx == SIPP_CONTROL_PIPE_IDX)
    {
        eState = geControlPipeState;
        if (eState != eSIPP_PIPE_WAIT_INIT)
        {
            bCmdComplete = TRUE;
        }
    }
    else
    {
        eState = pPipe->eState;
    }

    sippPalTrace (SIPP_SES_CTRL_TL_VERBOSE, "sippPipeSessionControl : CALL CMD  = %8x\n", eEvent);

    /* Continue processing until command has been dealt with                */
    while (bCmdComplete == FALSE)
    {
        sippPalTrace (SIPP_SES_CTRL_TL_VERBOSE, " sippPipeSessionControl LOOP : CMD= %8x State=%d\n", eEvent, eState);

        // Log current session state
        //sippDiagLogSessionState ( uPipeIdx, pPipe->eState );

        switch (eState)
        {
            // Only enter this state when uPipeIdx == SIPP_CONTROL_PIPE_IDX
            case eSIPP_PIPE_WAIT_INIT :
            {
                // Make sure this is the special pipe used for global HW operations...
                if ((uPipeIdx == SIPP_CONTROL_PIPE_IDX) &&
                    (eEvent == eSIPP_ACCESS_SCHEDULER_CMD_HWINIT))
                {
                    sippHWSessionInit ();
                }

                bCmdComplete = TRUE;

            } break;
            case eSIPP_PIPE_WAIT_FINALISE :
            {
                // Allow teardown of pipeline in this state - will not need to touch HW to do tear down here
                if (eEvent == eSIPP_ACCESS_SCHEDULER_CMD_FINALISE_PIPE)
                {
                    // Hook for pipeline validation
                    if (sippValidatePipe ((pSippPipeline)pPipe))
                    {
                        sippPalTrace (SIPP_SES_CTRL_TL_VERBOSE, "sippValidatePipe is OK\n");

                        if (sippCoreFinalisePipeline ((pSippPipeline) pPipe) == eSIPP_STATUS_OK)
                        {
                            pPipe->eState = eSIPP_PIPE_ACTIVE;
                        }
                        else
                        {
                            sippError (pPipe->tPublicPipe.errorStatus, E_FINALISE_FAIL);
                        }
                    }

                    bCmdComplete = TRUE;

                }
                else if (eEvent == eSIPP_ACCESS_SCHEDULER_CMD_TEARDOWN_PIPE)
                {
                    eState = eSIPP_PIPE_END_SESSION;
                }

            } break;
            case eSIPP_PIPE_ACTIVE :
            {
                // Pipeline now active so free to run - therefore free to accept process iters commands
                if (eEvent == eSIPP_ACCESS_SCHEDULER_CMD_PROCESS_ITERS)
                {
                    pPipe->uHWPipeID = uHWPipeIdx;

                    /* Call the HW aspect of the session controller to setup HW commands */
                    sippHWSessionCommand ((pSippPipeline)pPipe,
                                          pPipe->uHWPipeID,
                                          eEvent,
                                          pData);

                    bCmdComplete    = TRUE;

                }
                // Allow teardown of pipeline in this state - will not need to touch HW to do tear down here
                else if (eEvent == eSIPP_ACCESS_SCHEDULER_CMD_TEARDOWN_PIPE)
                {
                    eState = eSIPP_PIPE_END_SESSION;
                }
                else if (eEvent == eSIPP_ACCESS_SCHEDULER_CMD_RESCHEDULE_PIPE)
                {
                    if (sippCoreReschedulePipeline ((pSippPipeline)pPipe) != eSIPP_STATUS_OK)
                    {

                    }

                    bCmdComplete = TRUE;
                }
                // Update status needs to feed back the status of this to the scheduler
                else if (eEvent == eSIPP_ACCESS_SCHEDULER_PIPE_UPDATE_STATUS)
                {
                    // In theory, if process iters allowed us to still do sub-frames
                    // we should check if it is complete - however for now only
                    // a full frame done gets us here so...
                    // Clear down the current pipes
                    sippHWSessionRemoveActiveLists ((pSippPipeline)pPipe,
                                                    pPipe->uHWPipeID);

                    bCmdComplete = TRUE;

                }


            } break;
            case eSIPP_PIPE_END_SESSION :
            {
                // We need to check that this pipe is removed from the list -
                // in case some
                u32 activePipeIdx;

                for (activePipeIdx = 0; activePipeIdx < SIPP_MAX_SUPPORTED_HW_PIPELINES; activePipeIdx++)
                {
                    if (pgSippHW->pSippLoadedHWPipe[activePipeIdx] == (pSippPipeline)pPipe)
                    {
                        sippHWSessionRemoveActiveLists ((pSippPipeline)pPipe,
                                                        activePipeIdx);
                    }
                }

                bCmdComplete = TRUE;

            } break;
        }

    } /* End of while */

    sippPalTrace ( SIPP_SES_CTRL_TL_VERBOSE, "sippPipeSessionControl LOOP Exit : State = %8x\n", eState );

}

/* End of file */
