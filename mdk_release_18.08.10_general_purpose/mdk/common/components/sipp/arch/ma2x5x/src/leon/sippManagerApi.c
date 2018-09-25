///
/// @file      sippManagerApi.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP framework API
///            Platform(s) supported : ma2x5x
///

////////////////////////////////////////////////////
// Public includes
////////////////////////////////////////////////////

#include <sipp.h>
#include <sippInternal.h>

////////////////////////////////////////////////////
// Global variables
////////////////////////////////////////////////////

extern tSippFramework gSippFramework;
static u32 gSippItersData[SIPP_MAX_SUPPORTED_PIPELINES];
static u32 gCallbackDataIdx[SIPP_MAX_SUPPORTED_PIPELINES];
static u32 gCallbackData[SIPP_MAX_SUPPORTED_PIPELINES][SIPP_MAX_EVENTS_PER_PIPE];

////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippCallbackInit                                                 //
//                                                                                //
//  DESCRIPTION: Initialisation of static globals for this mechanism              //
//                                                                                //
//  INPUTS:      None                                                             //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     Will be called by control layer                                  //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippCallbackInit ( void )
{
    u32 uPipeIdx;

    for (uPipeIdx = 0; uPipeIdx < SIPP_MAX_SUPPORTED_PIPELINES; uPipeIdx++)
    {
        gCallbackDataIdx[uPipeIdx] = 0;
    }
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippIssueCommand                                                 //
//                                                                                //
//  DESCRIPTION: Internal function to process commands which need HW and/or       //
//               SHAVE support to be completed                                    //
//                                                                                //
//  INPUTS:      pPipe     - Pipeline command                                     //
//               eSippCmd  - The command to be executed                           //
//               pCmdData  - Pointer to any data associated with the command      //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     SIPP_STATUS                                                      //
//                  OK             - Success.                                     //
//                  ???                                                           //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     Called from SIPP API context - will vary dependent on system     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

eSIPP_STATUS sippIssueCommand (pSippPipeline   pPipe,
                               eSippCommand    eSippCmd,
                               SippCommandData pCmdData)
{
    SIPP_ACCESS_SCHEDULER_EVENT_DATA pEventData = (SIPP_ACCESS_SCHEDULER_EVENT_DATA)NULL;
    ptSippPipelineSuper              ptSPipe    = (ptSippPipelineSuper)pPipe;
    eSIPP_ACCESS_SCHEDULER_EVENT     eSchedulerEvent;
    u32                              uPipeIdx;

    /* Convert Handle to a stream index */
    uPipeIdx = ( eSippCmd == eSIPP_CMD_INIT_HW ) ? SIPP_CONTROL_PIPE_IDX : ptSPipe->uPipeIdx;

    switch ( eSippCmd )
    {
        case eSIPP_CMD_INIT_HW                : {
                                                    eSchedulerEvent = eSIPP_ACCESS_SCHEDULER_CMD_HWINIT;
                                                } break;
        case eSIPP_CMD_PROCESS_ITERS_HW       : {
                                                    sippPalMemcpy (( void * )&gSippItersData[uPipeIdx],
                                                                   ( void * )pCmdData,
                                                                   sizeof( u32 ));

                                                    eSchedulerEvent = eSIPP_ACCESS_SCHEDULER_CMD_PROCESS_ITERS;
                                                    pEventData      = ( SIPP_ACCESS_SCHEDULER_EVENT_DATA )&gSippItersData[uPipeIdx];
                                                } break;
        case eSIPP_CMD_FINALISE_PIPE_SW       : {
                                                    eSchedulerEvent = eSIPP_ACCESS_SCHEDULER_CMD_FINALISE_PIPE;
                                                } break;
        case eSIPP_CMD_RESCHEDULE_PIPE_SW     : {
                                                    eSchedulerEvent = eSIPP_ACCESS_SCHEDULER_CMD_RESCHEDULE_PIPE;
                                                } break;
        case eSIPP_CMD_DELETE_PIPE_SW         : {
                                                    eSchedulerEvent = eSIPP_ACCESS_SCHEDULER_CMD_TEARDOWN_PIPE;
                                                } break;
        default                               : eSchedulerEvent = eSIPP_ACCESS_SCHEDULER_CMD_RESET; break;
    }

    if ( FALSE == sippAccessSchedulerQuPush (uPipeIdx,
                                             eSchedulerEvent,
                                             pEventData))
    {
        sippError (pPipe->errorStatus, E_SCHEDULING_OVF);
        return eSIPP_STATUS_FULL;
    }
    #if ( SIPP_RTOS == SIPP_NO_RTOS )
    else
    {
        /* Call the sippAccessScheduler */
        sippAccessSchedulerControl ( );
    }
    #endif

    return eSIPP_STATUS_OK;

}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippEventNotify                                                  //
//                                                                                //
//  DESCRIPTION: Used by SIPP framework to signal events                          //
//                                                                                //
//  INPUTS:      uPipeIdx - Pipeline Index within framework strucutre             //
//               eEvent   - The event to report                                   //
//               pData    - Pointer to any data associated with the event         //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     Will be called in ISR or thread context                          //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippEventNotify (pSippPipeline              pPipe,
                      eSIPP_PIPELINE_EVENT       eEvent,
                      SIPP_PIPELINE_EVENT_DATA * pData)
{
    ptSippPipelineSuper ptSPipe;
    SIPP_PAL_CRIT_STATE uCS;

    ptSPipe = (ptSippPipelineSuper)pPipe;

    if ((eEvent == eSIPP_PIPELINE_FRAME_DONE) ||
        (eEvent == eSIPP_PIPELINE_ITERS_DONE) ||
        (eEvent == eSIPP_PIPELINE_SYNC_OP_DONE))
    {
        u32 uPipeIdx = ptSPipe->uPipeIdx;

        sippPalCriticalSectionBegin (&uCS);

        // The data associated with the release message will simply be the pipeline struct
        // so that it may be removed from the list of active pipes
        gCallbackData[uPipeIdx][gCallbackDataIdx[uPipeIdx]] = (u32)ptSPipe;

        if (FALSE == sippAccessSchedulerQuPush (uPipeIdx,
                                                eSIPP_ACCESS_SCHEDULER_PIPE_UPDATE_STATUS,
                                                (SIPP_ACCESS_SCHEDULER_EVENT_DATA)&gCallbackData[uPipeIdx][gCallbackDataIdx[uPipeIdx]]))
        {
            sippError (pPipe->errorStatus, E_SCHEDULING_OVF);
        }
        else
        {
            // We would need to note that a call to the scheduler is required...
        }

        gCallbackDataIdx[uPipeIdx]++;

        if (gCallbackDataIdx[uPipeIdx] == SIPP_MAX_EVENTS_PER_PIPE) gCallbackDataIdx[uPipeIdx] = 0;

        sippPalCriticalSectionEnd (uCS);
    }


    if (eEvent != eSIPP_PIPELINE_SYNC_OP_DONE)
    {
        if (ptSPipe->tPublicPipe.pfCallback)
        {
            ptSPipe->tPublicPipe.pfCallback (pPipe,
                                             eEvent,
                                             pData);
        }
    }
}

