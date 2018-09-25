////
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP framework API
///            Platform(s) supported : MA2x5x
///

#ifndef _SIPP_ACCESS_SCHEDULER_H_
#define _SIPP_ACCESS_SCHEDULER_H_

/////////////////////////////////////////////////////////////////////////////////
//  Header files
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//  Function Prototypes
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Public

bool sippAccessSchedulerInit (void);

bool sippAccessSchedulerQuPush (u32                           uPipeIdx,
                                eSIPP_ACCESS_SCHEDULER_EVENT     eEvent,
                                SIPP_ACCESS_SCHEDULER_EVENT_DATA pData );

#if SIPP_RTOS != SIPP_NO_RTOS
void * sippAccessSchedulerControl (void * pArgV);
#else
void sippAccessSchedulerControl (void);
#endif

u32 sippAccessScheduleCheckPending (void);

#endif /* _SIPP_ACCESS_SCHEDULER_H_ */

/* End of file */
