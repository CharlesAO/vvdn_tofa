////
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP framework API
///            Platform(s) supported : MA2x5x
///

#ifndef _SIPP_ACCESS_SCHEDULER_TYPES_H_
#define _SIPP_ACCESS_SCHEDULER_TYPES_H_

/////////////////////////////////////////////////////////////////////////////////
//  Header files
/////////////////////////////////////////////////////////////////////////////////

#include "sippCfg.h"
#include <sippPalTypes.h>

/////////////////////////////////////////////////////////////////////////////////
//  Global Macros
/////////////////////////////////////////////////////////////////////////////////

#define SIPP_SW_CMD_PENDING             gSippAccessScheduler.uSWPendList
#define SET_SW_PEND_LIST(uPipeIdx)      gSippAccessScheduler.uSWPendList |= ( 0x1 << uPipeIdx)
#define CLR_SW_PEND_LIST(uPipeIdx)      gSippAccessScheduler.uSWPendList &= (~( 0x1 << uPipeIdx))
#define SET_HW_PEND_LIST(uPipeIdx)      gSippAccessScheduler.uHWPendList |= ( 0x1 << uPipeIdx)
#define CLR_HW_PEND_LIST(uPipeIdx)      gSippAccessScheduler.uHWPendList &= (~( 0x1 << uPipeIdx))
#define CHK_HW_PEND_LIST(uPipeIdx)      gSippAccessScheduler.uHWPendList & ( 0x1 << uPipeIdx)
#define CLR_HW_WAIT_IVAL(uPipeIdx)      gSippAccessScheduler.uHWWaitList[uPipeIdx] = 0
#define SIPP_HW_EVENT_FLAG              0x10000000


/////////////////////////////////////////////////////////////////////////////////
//  Global types / structure definitions
////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
// Scheduler event

typedef enum
{
    eSIPP_ACCESS_SCHEDULER_CMD_RESET = 0x0,
    eSIPP_ACCESS_SCHEDULER_CMD_FINALISE_PIPE,
    eSIPP_ACCESS_SCHEDULER_CMD_RESCHEDULE_PIPE,
    eSIPP_ACCESS_SCHEDULER_CMD_TEARDOWN_PIPE,

    eSIPP_ACCESS_SCHEDULER_CMD_HWINIT        = 0x10000000,
    eSIPP_ACCESS_SCHEDULER_CMD_PROCESS_ITERS,

    eSIPP_ACCESS_SCHEDULER_PIPE_UPDATE_STATUS= 0x80000000,
    eSIPP_ACCESS_SCHEDULER_NULL

} eSIPP_ACCESS_SCHEDULER_EVENT;


//////////////////////////////////////////////////////////////
// Scheduler Event info structure

typedef void * SIPP_ACCESS_SCHEDULER_EVENT_DATA;

//////////////////////////////////////////////////////////
// Schedule Queue Entry structure

typedef struct
{
    eSIPP_ACCESS_SCHEDULER_EVENT      eEvent;
    SIPP_ACCESS_SCHEDULER_EVENT_DATA  pEventData;
    u32                               uPipeIdx;

} SIPP_ACCESS_SCHEDULER_QU_ENTRY;

//////////////////////////////////////////////////////////////
// Scheduler Queue

typedef struct
{
    SIPP_ACCESS_SCHEDULER_QU_ENTRY  Qu[SIPP_ACCESS_SCHEDULER_QU_SIZE];
    u32                             uWrIdx;
    u32                             uRdIdx;
    u32                             uSize;

} SIPP_ACCESS_SCHEDULER_QU;

//////////////////////////////////////////////////////////////
// SIPP Scheduler

typedef struct
{

    #if SIPP_RTOS != SIPP_NO_RTOS
    SIPP_PAL_QU                      SippMsgQu;
    #endif

    SIPP_ACCESS_SCHEDULER_QU         SippQu;

    eSIPP_ACCESS_SCHEDULER_EVENT     ePendActions[SIPP_MAX_SUPPORTED_PIPELINES][SIPP_MAX_EVENTS_PER_PIPE];
    SIPP_ACCESS_SCHEDULER_EVENT_DATA pPendData[SIPP_MAX_SUPPORTED_PIPELINES][SIPP_MAX_EVENTS_PER_PIPE];
    eSIPP_ACCESS_SCHEDULER_EVENT     eControlPipeAction;
    SIPP_ACCESS_SCHEDULER_EVENT_DATA pControlPipePendData;

    volatile u32                     uSWPendList;
    volatile u32                     uHWPendList;
    bool                             bWaitLongest[SIPP_MAX_SUPPORTED_PIPELINES];
    u32                              uHWWaitList[SIPP_MAX_SUPPORTED_PIPELINES];
    u32                              uPipeEventWrIndex[SIPP_MAX_SUPPORTED_PIPELINES];
    u32                              uPipeEventRdIndex[SIPP_MAX_SUPPORTED_PIPELINES];
    u32                              uPipeHWEventCount[SIPP_MAX_SUPPORTED_PIPELINES];

} SIPP_ACCESS_SCHEDULER;

#endif /* _SIPP_ACCESS_SCHEDULER_TYPES_H_ */

/* End of file */
