///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP framework API
///            Platform(s) supported : MA2x5x
///

#include <sipp.h>
#include <sippInternal.h>

////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////

static bool sippAccessSchedulerAssignHWTask (void);

static void sippAccessSchedulerAssignSWTask (void);

static void sippAccessSchedulerPipeUpdate (u32                              uPipeIdx,
                                           eSIPP_ACCESS_SCHEDULER_EVENT     eEvent,
                                           SIPP_ACCESS_SCHEDULER_EVENT_DATA pData);

static u32  sippAccessScheduleAlgo (bool  bSWList,
                                    u32 * puHWPipeIdx);

static void sippAccessScheduleSetPipePend (u32                              uPipeIdx,
                                           eSIPP_ACCESS_SCHEDULER_EVENT     eEvent,
                                           SIPP_ACCESS_SCHEDULER_EVENT_DATA pData);

static bool sippAccessScheduleCapabilityCheck (u32   uPipeIdx,
                                               u32 * puHWPipeIdx);

static bool sippAccessScheduleHWCapabilityCheck (ptSippPipelineSuper pPipe,
                                                 u32 *               puHWPipeIdx);

////////////////////////////////////////////////////
// Global variables
////////////////////////////////////////////////////

extern pSIPP_HW_SESSION pgSippHW;
extern tSippFramework   gSippFramework;

////////////////////////////////////////////////////
// Static variables
////////////////////////////////////////////////////

static SIPP_ACCESS_SCHEDULER  gSippAccessScheduler;

////////////////////////////////////////////////////
// Local Macros
////////////////////////////////////////////////////
#define UPDATE_PEND_LISTS(uPipeIdx) { \
                                        if (gSippAccessScheduler.uPipeEventWrIndex[uPipeIdx] == \
                                            gSippAccessScheduler.uPipeEventRdIndex[uPipeIdx])\
                                        { \
                                            CLR_SW_PEND_LIST(uPipeIdx); \
                                            CLR_HW_PEND_LIST(uPipeIdx); \
                                        } \
                                        else \
                                        {  \
                                            if ( gSippAccessScheduler.ePendActions[uPipeIdx][gSippAccessScheduler.uPipeEventRdIndex[uPipeIdx]] & SIPP_HW_EVENT_FLAG ) \
                                            { \
                                                CLR_SW_PEND_LIST(uPipeIdx); \
                                                SET_HW_PEND_LIST(uPipeIdx); \
                                            } \
                                            else \
                                            { \
                                                SET_SW_PEND_LIST(uPipeIdx); \
                                                CLR_HW_PEND_LIST(uPipeIdx); \
                                            } \
                                        }  \
                                    }

////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippAccessSchedulerInit                                      //
//                                                                            //
//  DESCRIPTION: Initialise the scheduling service                            //
//                                                                            //
//  INPUTS:      None                                                         //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:                                                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

bool sippAccessSchedulerInit ( )
{
    u32 uPipeIdx;

    gSippAccessScheduler.SippQu.uWrIdx      = 0;
    gSippAccessScheduler.SippQu.uRdIdx      = 0;
    gSippAccessScheduler.uHWPendList        = 0;
    gSippAccessScheduler.eControlPipeAction = eSIPP_ACCESS_SCHEDULER_NULL;

    for (uPipeIdx = 0; uPipeIdx < SIPP_MAX_SUPPORTED_PIPELINES; uPipeIdx++)
    {
        CLR_HW_WAIT_IVAL(uPipeIdx);
    }

    #if SIPP_RTOS != SIPP_NO_RTOS
    // Create the access scheduler qu
    if (eSIPP_STATUS_OK != sippPalQuCreate (SIPP_ACCESS_SCHEDULER_QU_SIZE,
                                            SIPP_ACCESS_SCHEDULER_QU_NAME,
                                            &gSippAccessScheduler.SippMsgQu))
    {
        return FALSE;
    }
    #endif

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippAccessSchedulerQuPush                                    //
//                                                                            //
//  DESCRIPTION: Adds event to the scheduler queue                            //
//                                                                            //
//  INPUTS:      ...                                                          //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     TRUE if push successful                                      //
//                                                                            //
//  NOTES:       May be called from any context                               //
//               Provides obvious RTOS delimiter for the whole process        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

bool sippAccessSchedulerQuPush (u32                              uPipeIdx,
                                eSIPP_ACCESS_SCHEDULER_EVENT     eEvent,
                                SIPP_ACCESS_SCHEDULER_EVENT_DATA pData)
{
    SIPP_PAL_CRIT_STATE              uCS;
    SIPP_ACCESS_SCHEDULER_QU_ENTRY * pQuEntry;
    bool                             bRetVal = FALSE;

    sippPalCriticalSectionBegin (&uCS);

    if (gSippAccessScheduler.SippQu.uSize == SIPP_ACCESS_SCHEDULER_QU_SIZE)
    {
        sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "sippAccessSchedulerQuPush - QU FULL \n");

        sippPalCriticalSectionEnd ( uCS );
    }
    else
    {
        pQuEntry = (SIPP_ACCESS_SCHEDULER_QU_ENTRY *)&gSippAccessScheduler.SippQu.Qu[gSippAccessScheduler.SippQu.uWrIdx];

        pQuEntry->eEvent     = eEvent;
        pQuEntry->pEventData = pData;
        pQuEntry->uPipeIdx   = uPipeIdx;

        gSippAccessScheduler.SippQu.uWrIdx++;
        if (gSippAccessScheduler.SippQu.uWrIdx == SIPP_ACCESS_SCHEDULER_QU_SIZE)
        {
            gSippAccessScheduler.SippQu.uWrIdx = 0;
        }

        gSippAccessScheduler.SippQu.uSize++;

        sippPalCriticalSectionEnd ( uCS );

        #if SIPP_RTOS != SIPP_NO_RTOS
        if (sippPalQuPost (&gSippAccessScheduler.SippMsgQu,
                           (void *)&pQuEntry) == eSIPP_STATUS_OK)
        {
            bRetVal = TRUE;
        }
        #else
        bRetVal = TRUE;
        #endif
    }

    return bRetVal;

}

#if SIPP_RTOS == SIPP_NO_RTOS

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippAccessSchedulerQuPull                                    //
//                                                                            //
//  DESCRIPTION: Pulls event from the scheduler queue                         //
//                                                                            //
//  INPUTS:      None                                                         //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:                                                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

static bool sippAccessSchedulerQuPull (u32 *                              puPipeIdx,
                                       eSIPP_ACCESS_SCHEDULER_EVENT *     peEvent,
                                       SIPP_ACCESS_SCHEDULER_EVENT_DATA * ppData)
{
    bool bEntryAvail = FALSE;

    SIPP_PAL_CRIT_STATE uCS;
    SIPP_ACCESS_SCHEDULER_QU_ENTRY * pQuEntry;

    sippPalCriticalSectionBegin ( &uCS );

    if ( gSippAccessScheduler.SippQu.uSize == 0 )
    {
        *peEvent = eSIPP_ACCESS_SCHEDULER_NULL;

        sippPalCriticalSectionEnd ( uCS );
        return bEntryAvail;
    }

    pQuEntry   = ( SIPP_ACCESS_SCHEDULER_QU_ENTRY * )&gSippAccessScheduler.SippQu.Qu[gSippAccessScheduler.SippQu.uRdIdx];

    *peEvent   = pQuEntry->eEvent;
    *ppData    = pQuEntry->pEventData;
    *puPipeIdx = pQuEntry->uPipeIdx;

    gSippAccessScheduler.SippQu.uRdIdx++;

    if ( gSippAccessScheduler.SippQu.uRdIdx == SIPP_ACCESS_SCHEDULER_QU_SIZE )
    {
        gSippAccessScheduler.SippQu.uRdIdx = 0;
    }

    gSippAccessScheduler.SippQu.uSize--;

    bEntryAvail = TRUE;

    sippPalCriticalSectionEnd ( uCS );

    return bEntryAvail;

}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippAccessSchedulerControl                                   //
//                                                                            //
//  DESCRIPTION: Process inputs to scheduler and make decision on subsequent  //
//               actions on a per pipeline basis - including enabling access  //
//               to HW                                                        //
//                                                                            //
//  INPUTS:      None                                                         //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       Call from thread context.                                    //
//               In an RTOS situation we could consider using this qu as      //
//               the appropriate thread sync primitive                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifdef SIPP_ACCESS_SCHEDULE_DEBUG
u32 uCmdList[SIPP_MAX_SUPPORTED_PIPELINES][20];
#if SIPP_MAX_SUPPORTED_PIPELINES > 1
u32 uCmdListIdx[SIPP_MAX_SUPPORTED_PIPELINES] = {{0}};
#else
u32 uCmdListIdx[SIPP_MAX_SUPPORTED_PIPELINES] = {0};
#endif
#endif

void sippAccessSchedulerControl ( )
{
    u32                              uPipeIdx = 0;
    eSIPP_ACCESS_SCHEDULER_EVENT     eEvent;
    SIPP_ACCESS_SCHEDULER_EVENT_DATA pData = NULL;

    // Always consume all commands as some may usurp others!
    // For example I can envisage a situation where we may wish to stop a pipeline
    // after having issued a process frame command
    while (sippAccessSchedulerQuPull (&uPipeIdx, &eEvent, &pData))
    {
        #ifdef SIPP_ACCESS_SCHEDULE_DEBUG
        uCmdList[uPipeIdx][uCmdListIdx[uPipeIdx]++] = eEvent;
        if ( uCmdListIdx[uPipeIdx] == 20 ) uCmdListIdx[uPipeIdx] = 0;
        #endif

        /* Pass detail to a pipe specific function               */
        sippAccessSchedulerPipeUpdate (uPipeIdx, eEvent, pData);

        // This should also be protected as if it needs to generate a schedule it needs to be
        // checked against the active pipelines for compatibility in terms of CMX memory
        while ( SIPP_SW_CMD_PENDING ) sippAccessSchedulerAssignSWTask ();

        while (sippAccessSchedulerAssignHWTask ( ));

    }
}

#else

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippAccessSchedulerQuPull                                    //
//                                                                            //
//  DESCRIPTION: Pulls event from the scheduler queue                         //
//                                                                            //
//  INPUTS:      None                                                         //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:                                                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

static bool sippAccessSchedulerQuPull (SIPP_ACCESS_SCHEDULER_QU_ENTRY *   pQuEntry,
                                       u32 *                              puPipeIdx,
                                       eSIPP_ACCESS_SCHEDULER_EVENT *     peEvent,
                                       SIPP_ACCESS_SCHEDULER_EVENT_DATA * ppData)
{
    SIPP_PAL_CRIT_STATE uCS;
    bool                bEntryAvail = FALSE;

    sippPalCriticalSectionBegin ( &uCS );

    if (gSippAccessScheduler.SippQu.uSize == 0)
    {
        *peEvent = eSIPP_ACCESS_SCHEDULER_NULL;

        sippPalCriticalSectionEnd ( uCS );
        return bEntryAvail;
    }

    *peEvent   = pQuEntry->eEvent;
    *ppData    = pQuEntry->pEventData;
    *puPipeIdx = pQuEntry->uPipeIdx;

    gSippAccessScheduler.SippQu.uRdIdx++;

    if ( gSippAccessScheduler.SippQu.uRdIdx == SIPP_ACCESS_SCHEDULER_QU_SIZE )
    {
        gSippAccessScheduler.SippQu.uRdIdx = 0;
    }

    gSippAccessScheduler.SippQu.uSize--;

    bEntryAvail = TRUE;

    sippPalCriticalSectionEnd ( uCS );

    return bEntryAvail;

}

void * sippAccessSchedulerControl (void * pArgV)
{
    u32                              pipeIdx = 0;
    eSIPP_ACCESS_SCHEDULER_EVENT     eEvent;
    SIPP_ACCESS_SCHEDULER_EVENT_DATA pData = NULL;

    UNUSED(pArgV);

    sippPalQuAttach (SIPP_ACCESS_SCHEDULER_QU_NAME,
                     &gSippAccessScheduler.SippMsgQu);

    while (1)
    {
        SIPP_PAL_QU_MSG message;
        if (eSIPP_STATUS_OK == sippPalQuReceive (&gSippAccessScheduler.SippMsgQu,
                                                 &message))
        {
            SIPP_ACCESS_SCHEDULER_QU_ENTRY * pQuEntry = *(SIPP_ACCESS_SCHEDULER_QU_ENTRY **)message;

            sippAccessSchedulerQuPull (pQuEntry,
                                       &pipeIdx,
                                       &eEvent,
                                       &pData);

            // Pass detail to a pipe specific function
            sippAccessSchedulerPipeUpdate (pipeIdx,
                                           eEvent,
                                           pData);

            // This should also be protected as if it needs to generate a schedule it needs to be
            // checked against the active pipelines for compatibility in terms of CMX memory
            while ( SIPP_SW_CMD_PENDING ) sippAccessSchedulerAssignSWTask ();

            while (sippAccessSchedulerAssignHWTask ( ));


            // We need a terminate message.... so that an API could post this command
        }
    }

    return NULL;
}

#endif

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippAccessScheduleCheckPending                               //
//                                                                            //
//  DESCRIPTION: returns if any HW instructions are pending for any pipe      //
//                                                                            //
//  INPUTS:      None                                                         //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     Pending List                                                 //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

u32 sippAccessScheduleCheckPending ( )
{
    return gSippAccessScheduler.uHWPendList;
}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippAccessSchedulerAssignHWTask                              //
//                                                                            //
//  DESCRIPTION: Examine waiting items which require HW execution             //
//               Select next for processing                                   //
//                                                                            //
//  INPUTS:      None                                                         //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       May be called from thread context                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

static bool sippAccessSchedulerAssignHWTask ( )
{
    u32  uPipe2Schedule = SIPP_NULL_PIPELINE_ID;
    bool bRetVal        = FALSE;
    u32  uHWPipeIdx     = 0xFF;

    /* Call the scheduler algo    */
    uPipe2Schedule = sippAccessScheduleAlgo (FALSE,
                                             &uHWPipeIdx);

    if (uPipe2Schedule != SIPP_NULL_PIPELINE_ID)
    {
        eSIPP_ACCESS_SCHEDULER_EVENT     ePendAction;
        SIPP_ACCESS_SCHEDULER_EVENT_DATA pPendData;

        if (uPipe2Schedule != SIPP_CONTROL_PIPE_IDX)
        {
            ePendAction = gSippAccessScheduler.ePendActions[uPipe2Schedule][gSippAccessScheduler.uPipeEventRdIndex[uPipe2Schedule]];
            pPendData   = gSippAccessScheduler.pPendData[uPipe2Schedule][gSippAccessScheduler.uPipeEventRdIndex[uPipe2Schedule]];

            gSippAccessScheduler.uPipeEventRdIndex[uPipe2Schedule]++;
            if (gSippAccessScheduler.uPipeEventRdIndex[uPipe2Schedule] == SIPP_MAX_EVENTS_PER_PIPE)
            {
                gSippAccessScheduler.uPipeEventRdIndex[uPipe2Schedule] = 0x0;
            }

            /* Update this pending lists */
            UPDATE_PEND_LISTS(uPipe2Schedule);

            /* Reset its wait time */
            CLR_HW_WAIT_IVAL(uPipe2Schedule);

        }
        else
        {
            ePendAction = gSippAccessScheduler.eControlPipeAction;
            pPendData   = gSippAccessScheduler.pControlPipePendData;

            gSippAccessScheduler.eControlPipeAction = eSIPP_ACCESS_SCHEDULER_NULL;
        }

        /* Set up the command */
        sippPipeSessionControl (ePendAction,
                                pPendData,
                                uPipe2Schedule,
                                uHWPipeIdx);

        bRetVal = TRUE;
    }

    return bRetVal;

}


////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippAccessSchedulerAssignSWTask                              //
//                                                                            //
//  DESCRIPTION: Process the highest priority pending SW tasks                //
//                                                                            //
//  INPUTS:      None                                                         //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     Stream Index                                                 //
//                                                                            //
//  NOTES:       This function does not care if one of the SW commands        //
//               applies to a pipe which is currently active in HW            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/* In a RTOS situation, we could post to threads of differing priority here */
/* in order to have these events serviced                                   */

static void sippAccessSchedulerAssignSWTask ( )
{
    u32 uPipe2Schedule = SIPP_NULL_PIPELINE_ID;
    u32 uHWPipeIdx     = 0xFF;

    /* Call the scheduler algo    */
    uPipe2Schedule = sippAccessScheduleAlgo (TRUE,
                                             &uHWPipeIdx);

    /* If this is a proper pipe */
    if (uPipe2Schedule != SIPP_NULL_PIPELINE_ID)
    {
        eSIPP_ACCESS_SCHEDULER_EVENT     ePendAction;
        SIPP_ACCESS_SCHEDULER_EVENT_DATA pPendData;

        ePendAction = gSippAccessScheduler.ePendActions[uPipe2Schedule][gSippAccessScheduler.uPipeEventRdIndex[uPipe2Schedule]];
        pPendData   = gSippAccessScheduler.pPendData[uPipe2Schedule][gSippAccessScheduler.uPipeEventRdIndex[uPipe2Schedule]];

        gSippAccessScheduler.uPipeEventRdIndex[uPipe2Schedule]++;
        if (gSippAccessScheduler.uPipeEventRdIndex[uPipe2Schedule] == SIPP_MAX_EVENTS_PER_PIPE)
        {
            gSippAccessScheduler.uPipeEventRdIndex[uPipe2Schedule] = 0x0;
        }

        /* Update this pending lists */
        UPDATE_PEND_LISTS(uPipe2Schedule);

        /* Reset its wait time */
        CLR_HW_WAIT_IVAL(uPipe2Schedule);

        /* Set up the command */
        sippPipeSessionControl (ePendAction,
                                pPendData,
                                uPipe2Schedule,
                                uHWPipeIdx);

    }
}


////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippAccessSchedulerPipeUpdate                                //
//                                                                            //
//  DESCRIPTION: Process an event in the context of the pipe only             //
//                                                                            //
//  INPUTS:      None                                                         //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None.                                                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

static void sippAccessSchedulerPipeUpdate (u32                              uPipeIdx,
                                           eSIPP_ACCESS_SCHEDULER_EVENT     eEvent,
                                           SIPP_ACCESS_SCHEDULER_EVENT_DATA pData)
{

    if ( eEvent == eSIPP_ACCESS_SCHEDULER_PIPE_UPDATE_STATUS )
    {
        /* Since this is a update status, we know this pipe is */
        /* currently active on some HW resource                */
        sippPipeSessionControl (eSIPP_ACCESS_SCHEDULER_PIPE_UPDATE_STATUS,
                                pData,
                                uPipeIdx,
                                0x0);
    }
    else
    {
        sippAccessScheduleSetPipePend (uPipeIdx,
                                       eEvent,
                                       pData);

    }
}


////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippAccessScheduleAlgo                                       //
//                                                                            //
//  DESCRIPTION: Selects a new pipe for processing with HW                    //
//                                                                            //
//  INPUTS:      bSWList - Set true if this is for SW processing              //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     Stream Index                                                 //
//                                                                            //
//  NOTES:                                                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


static u32 sippAccessScheduleAlgo (bool  bSWList,
                                   u32 * puHWPipeIdx)
{
    u32 uPipeIdx;
    u32 uLongWaitServicable = 0;
    u32 uLongWaitOverall    = 0;
    u32 uChosenPipe         = SIPP_NULL_PIPELINE_ID;
    u32 uHWPipeIdx          = SIPP_INVALID_HW_PIPE_ID;
    u32 uChosenHWPipeIdx    = SIPP_INVALID_HW_PIPE_ID;

    if (bSWList == FALSE)
    {
        /* Employing a basic round robin algo for HW */
        if (gSippAccessScheduler.eControlPipeAction != eSIPP_ACCESS_SCHEDULER_NULL)
        {
            uChosenPipe = SIPP_CONTROL_PIPE_IDX;
        }
        else if (gSippAccessScheduler.uHWPendList)
        {
            for (uPipeIdx = 0; uPipeIdx < SIPP_MAX_SUPPORTED_PIPELINES; uPipeIdx++)
            {
                if (CHK_HW_PEND_LIST(uPipeIdx))
                {
                    gSippAccessScheduler.uHWWaitList[uPipeIdx]++;

                    if (gSippAccessScheduler.uHWWaitList[uPipeIdx] > uLongWaitServicable)
                    {
                        /* Is current free resource capable of executing the instruction? */
                        if (sippAccessScheduleCapabilityCheck (uPipeIdx,
                                                               &uHWPipeIdx))
                        {
                            uLongWaitServicable  = gSippAccessScheduler.uHWWaitList[uPipeIdx];
                            uChosenPipe          = uPipeIdx;
                            uChosenHWPipeIdx     = uHWPipeIdx;
                        }

                        if (gSippAccessScheduler.uHWWaitList[uPipeIdx] > uLongWaitOverall)
                        {
                            uLongWaitOverall = gSippAccessScheduler.uHWWaitList[uPipeIdx];
                        }
                    }
                }
            }
        }
    }
    else
    {
        /* Simpler find highest bit algo for SW      */
        /* All SW commands will be processed before  */
        /* scheduler returns anyway                  */
        if (gSippAccessScheduler.uSWPendList)
        {
            uChosenPipe = sippPalFindHighestBit (gSippAccessScheduler.uSWPendList);
        }
    }

    if (uChosenPipe != SIPP_NULL_PIPELINE_ID)
    {
        if (uChosenPipe != SIPP_CONTROL_PIPE_IDX)
        {
            CLR_HW_WAIT_IVAL(uChosenPipe);
        }
        else
        {
            CLR_HW_PEND_LIST(uChosenPipe);
        }

        if (bSWList == FALSE)
        {
            /* Claim the required HW resource */
            *puHWPipeIdx = uChosenHWPipeIdx;
        }
    }

    return uChosenPipe;
}


////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippAccessScheduleSetPipePend                                //
//                                                                            //
//  DESCRIPTION: Sets a command for a pipe in the appropriate pending list(s) //
//                                                                            //
//  INPUTS:      None                                                         //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     Stream Index                                                 //
//                                                                            //
//  NOTES:       Called only from sippAccessSchedulerPipeUpdate ( )           //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

static void sippAccessScheduleSetPipePend (u32                              uPipeIdx,
                                           eSIPP_ACCESS_SCHEDULER_EVENT     eEvent,
                                           SIPP_ACCESS_SCHEDULER_EVENT_DATA pData)
{
    SIPP_PAL_CRIT_STATE uCS;
    bool                bIncrementWrPtr = TRUE;

    sippPalCriticalSectionBegin (&uCS);

    switch ( eEvent )
    {
        case eSIPP_ACCESS_SCHEDULER_CMD_RESET :
        case eSIPP_ACCESS_SCHEDULER_CMD_RESCHEDULE_PIPE :
        case eSIPP_ACCESS_SCHEDULER_CMD_TEARDOWN_PIPE :
        case eSIPP_ACCESS_SCHEDULER_CMD_FINALISE_PIPE :
        {
            if (gSippAccessScheduler.uPipeEventWrIndex[uPipeIdx] ==
                gSippAccessScheduler.uPipeEventRdIndex[uPipeIdx])
            {
                SET_SW_PEND_LIST(uPipeIdx);
            }

            gSippAccessScheduler.ePendActions[uPipeIdx][gSippAccessScheduler.uPipeEventWrIndex[uPipeIdx]] = eEvent;
            gSippAccessScheduler.pPendData[uPipeIdx][gSippAccessScheduler.uPipeEventWrIndex[uPipeIdx]]    = pData;


        } break;
        case eSIPP_ACCESS_SCHEDULER_CMD_HWINIT :
        {
            bIncrementWrPtr                           = FALSE;
            gSippAccessScheduler.eControlPipeAction   = eEvent;
            gSippAccessScheduler.pControlPipePendData = pData;
            SET_HW_PEND_LIST(uPipeIdx);
        } break;
        case eSIPP_ACCESS_SCHEDULER_CMD_PROCESS_ITERS :
        {
            if (gSippAccessScheduler.uPipeEventWrIndex[uPipeIdx] ==
                gSippAccessScheduler.uPipeEventRdIndex[uPipeIdx])
            {
                SET_HW_PEND_LIST(uPipeIdx);
            }

            gSippAccessScheduler.ePendActions[uPipeIdx][gSippAccessScheduler.uPipeEventWrIndex[uPipeIdx]] = eEvent;
            gSippAccessScheduler.pPendData[uPipeIdx][gSippAccessScheduler.uPipeEventWrIndex[uPipeIdx]]    = pData;

        } break;

        default         :
        {
            /* Set a pending event on the HW list       */
            SET_HW_PEND_LIST(uPipeIdx);
            gSippAccessScheduler.ePendActions[uPipeIdx][gSippAccessScheduler.uPipeEventWrIndex[uPipeIdx]] = eEvent;
            gSippAccessScheduler.pPendData[uPipeIdx][gSippAccessScheduler.uPipeEventWrIndex[uPipeIdx]]    = pData;

        } break;
    }

    if ( bIncrementWrPtr )
    {
        gSippAccessScheduler.uPipeEventWrIndex[uPipeIdx]++;
        if ( gSippAccessScheduler.uPipeEventWrIndex[uPipeIdx] == SIPP_MAX_EVENTS_PER_PIPE ) gSippAccessScheduler.uPipeEventWrIndex[uPipeIdx] = 0x0;
    }

    sippPalCriticalSectionEnd (uCS);

}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippAccessScheduleCapabilityCheck                            //
//                                                                            //
//  DESCRIPTION: Checks pending instruction for a pipe against resources      //
//               to decide if can be processed at the current time            //
//                                                                            //
//  INPUTS:      uPipeIdx      - The pipe index                               //
//                                                                            //
//  OUTPUTS:                                                                  //
//                                                                            //
//  RETURNS:     TRUE if available HW resource is capable of executing        //
//               instruction                                                  //
//                                                                            //
//  NOTES:       Called only from sippAccessSchedulerAssignHWTask ( )         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

static bool sippAccessScheduleCapabilityCheck (u32   uPipeIdx,
                                               u32 * puHWPipeIdx)
{
    ptSippPipelineSuper pPipe;

    pPipe =  gSippFramework.pPipelines[uPipeIdx];

    // Check if pipe's required HW resource is available
    return (sippAccessScheduleHWCapabilityCheck (pPipe,
                                                 puHWPipeIdx));
}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippAccessScheduleHWCapabilityCheck                          //
//                                                                            //
//  DESCRIPTION: Checks pending instruction for a pipe against HW resource    //
//               to decide if can be processed on that HW unit                //
//                                                                            //
//  INPUTS:      uPipeIdx      - The pipe index                               //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     TRUE if HW resource required is available                    //
//                                                                            //
//  NOTES:       Called only from sippAccessScheduleCapabilityCheck()         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

static bool sippAccessScheduleHWCapabilityCheck (ptSippPipelineSuper pPipe,
                                                 u32 *               puHWPipeIdx)
{
    u32                 usageIdx;
    bool                bResourceNotAvail = FALSE;
    SIPP_PAL_CRIT_STATE CS;

    sippPalCriticalSectionBegin (&CS);

    if (pgSippHW->uNumCurrHwPipes < SIPP_MAX_SUPPORTED_HW_PIPELINES)
    {
        // Check that the pipes requested HW Filters are free
        for (usageIdx = 0; usageIdx < SIPP_HW_FILTER_MASK_SIZE; usageIdx++)
        {
            if (pgSippHW->uHWFilterUsageBitMask[usageIdx] & pPipe->tPublicPipe.uHWFilterUsageBitMask[usageIdx])
            {
                bResourceNotAvail = TRUE;
            }
        }

        // Check that the pipes requested SHAVEs are free
        for (usageIdx = pPipe->tPublicPipe.gi.sliceFirst; usageIdx <= pPipe->tPublicPipe.gi.sliceLast; usageIdx++)
        {
            if (pgSippHW->pSippCurrSvuPipe[usageIdx] != (pSippPipeline)NULL)
            {
                bResourceNotAvail = TRUE;
            }
        }

        #ifdef SIPP_SCRT_ENABLE_OPIPE
        if (bResourceNotAvail == FALSE)
        {
            if (pgSippHW->uNumCurrHwPipes)
            {
                // There are active pipes
                // For opipe runtime, check if we have a contrary ISR Setup
                // to what we need (no interrupt barriers) - if so, delay
                if (pPipe->tPublicPipe.pfnSippRuntime == sippOPipeRuntime)
                {
                    if (pgSippHW->isrSetup)
                    {
                        bResourceNotAvail = TRUE;


                        #ifdef SIPP_ENABLE_RT_COEXISTENCE
                        // We also need a means to force any subsequent session start
                        // not involving this pipe to adopt a no interrupt barrier policy
                        // until we get this pipe launched.

                        pgSippHW->noIntBarSwitchPend = TRUE;
                        #endif
                    }
                    #ifdef SIPP_ENABLE_RT_COEXISTENCE
                    else
                    {
                        pgSippHW->noIntBarSwitchPend = FALSE;
                    }
                    #endif
                }
            }
        }
        #endif
    }
    else
    {
        *puHWPipeIdx      = SIPP_INVALID_HW_PIPE_ID;
        bResourceNotAvail = TRUE;
    }

    // In a multi-threaded (RTOS) situation - successful checks on uHWFilterUsageBitMask & Shave usage
    // should be protected until this mask gets set to prevent multiple threads assuming the resource is
    // available
    sippPalCriticalSectionEnd (CS);

    if (bResourceNotAvail == FALSE)
    {
        u32 uIdx;

        for (uIdx = 0x0; uIdx < SIPP_MAX_SUPPORTED_HW_PIPELINES; uIdx++)
        {
            if (pgSippHW->pSippCurrHWPipe[uIdx] == (pSippPipeline)NULL)
            {
                break;
            }
        }

        *puHWPipeIdx = uIdx;
    }

    return !bResourceNotAvail;
}

/* End of File */
