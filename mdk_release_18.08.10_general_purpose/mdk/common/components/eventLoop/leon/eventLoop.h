///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @defgroup eventLoopApi Event Loop API
/// @ingroup  eventLoopApi
/// @{
/// @brief Event Loop functions API
///
/// API to implement simple scheduler in order to avoid interrupts on chip
///

#ifndef __EVENT_LOOP_H__
#define __EVENT_LOOP_H__

// System Includes
// ----------------------------------------------------------------------------

// Application Includes
// ----------------------------------------------------------------------------
#include "eventQueue.h"

#ifdef __cplusplus
extern "C" {
#endif

// Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
/// @brief Type of event loop callback for each event type;
typedef s32 (*eventLoopCallback_t)(eventQueueItem_t *event);

/// @brief Enum of different states of an event
typedef enum eventState_t
{
    BUSY,
    DONE,
    REQUIRED,
    DROPPED
} eventState_t;

// Global function prototypes
// ----------------------------------------------------------------------------

/// @brief  Initialize the event loop queuese
/// @return void
///
void eventLoopInit();

/// @brief Add an event loop callback for an event type
/// @param[in] event - type of event
/// @param[in] cb    - Type of event loop callback for each event type
/// @return 1
///
s32 eventLoopSetCallback(eventType_t event, eventLoopCallback_t cb);

/// @brief Add an event to the command queue
/// @param[in] event - event queue item
/// @return void
///
void eventLoopPushCmd(eventQueueItem_t *event);

/// @brief Add an event to the event loop
/// @param[in] event - event queue item
/// @return void
///
void eventLoopPush(eventQueueItem_t *event);

/// @brief Run the event loop
/// @return void
///
void eventLoopRun();

/// @brief Clear the event loop
/// @return void
///
void eventLoopReset();

/// @brief Start event loop timer
/// @return void
///
void eventLoopStartTimer();

/// @brief Set the event state
/// @param[in] evState       - event state to be set
/// @param[in] event         - event whose state is to be changed
/// @param[in] evCallbackPtr - pointer to event loop callback
/// @return void
///
void eventLoopSetEventState(eventState_t evState, eventQueueItem_t *event,
        eventLoopCallback_t evCallbackPtr);

/// @brief 	Clear the busy callback flag
/// @param[in] event 		 - event queue item
/// @param[in] evCallbackPtr - pointer to event loop callback
/// @return void
///
void eventLoopClearBusyFlag(eventQueueItem_t *event,
                            eventLoopCallback_t evCallbackPtr);

/// @brief Change the event type to newEventType.
/// @param[in] event         - event queue item
/// @param[in] newEventType  - event type to which the event item should be changed
/// @note  Changing the event type is a bad practice.
/// This function is usually used to avoid event copying (if the data is big)
/// Use this function only if you know the event loop well.
/// @return void
///
void eventLoopChangeEventType(eventQueueItem_t *event,
                              eventType_t newEventType);

/// @brief Drop an event from the loop
/// @param[in] event         - event queue item
/// @param[in] evCallbackPtr - pointer to event loop callback
/// @return void
///
void eventLoopDropEvent(eventQueueItem_t *event, eventLoopCallback_t evCallbackPtr);

/// @brief Set event done callback
/// @param[in] event         - event queue item
/// @param[in] onReleaseCb   - pointer to event loop callback
/// @return void
///
void eventLoopSetReleaseCallback(eventType_t event, eventLoopCallback_t onReleaseCb);

/// @brief Mark event done or free it, depending on the event state
/// @param[in] event         - event queue item
/// @param[in] evCallbackPtr - pointer to event loop callback
/// @return void
///
void eventLoopReleaseEvent(eventQueueItem_t *event, eventLoopCallback_t evCallbackPtr);

#ifdef __cplusplus
}
#endif


#endif /// __EVENT_LOOP_H__
