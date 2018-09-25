///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @defgroup eventQueue Event Queue
/// @ingroup eventQueue
/// @{
/// @brief    Event Queue functions API
///
/// Event Queue API and structs to aid the event loop component
///
#ifndef __EVENT_QUEUE_H__
#define __EVENT_QUEUE_H__

#include "EventType.h"

#ifdef __cplusplus
extern "C" {
#endif

// System Includes
// ----------------------------------------------------------------------------

// Application Includes
// ----------------------------------------------------------------------------

// Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------


struct eventQueue_t;

///  @brief An event queue item type
typedef struct eventQueueItem_t
{
    ///  @brief Type of this event
    volatile eventType_t type;

    ///  @brief Private data for this event
    void *data;

    ///  @brief Timestamp of this event
    u64 timestamp;

    /// @name Event Flags
    ///
    /// The following three flags should be modified through eventLoop interfaces only (check eventLoop.h)
    /// @{

    /// @brief Flags to indicate required callbacks for the event processing
    volatile u8 requiredFlags;
    /// @brief Flags to indicate event is busy with a specific event handling
    volatile u8 busyFlags;
    /// @brief Flags to indicate that a specific event handling is done
    volatile u8 doneFlags;
    /// @brief Flags to indicate that a specific event was dropped by one of the callbacks
    volatile u8 dropFlags;
    ///@}

    /// @brief Next pointer for event queue
    struct eventQueueItem_t *next;

    /// @brief Which list we are in
    struct eventQueue_t *parent;
} eventQueueItem_t;

/// @brief An event queue type
typedef struct eventQueue_t
{
	/// @brief Event queue item head
    eventQueueItem_t *head;
	/// @brief Event queue item tail
    eventQueueItem_t *tail;
	/// @brief Number of items
    u32 nItems;
} eventQueue_t;

/// @brief Get system timestamp in clock-ticks
u64 eventLoopTimestamp();

/// @brief Initialize an event queue
/// @param[in] self - event queue to be initialised
/// @return void
///
void eventQueueInit(eventQueue_t *self);

/// @brief Push a new item on the event queue
/// param[in] self - event queue to which will be added a new item
/// param[in] item - new event queue item
/// @return void
///
void eventQueuePush(eventQueue_t *self, eventQueueItem_t *item);

/// @brief Get item of the head of list and remove it from list
/// @param[in] self - event queue whose head is to be removed
/// @return Head of the event queue
///
eventQueueItem_t *eventQueuePop(eventQueue_t * self);

/// @brief Return item to parent queue
/// @param[in] item - event queue item to be added back to the parent queue
/// @return void
///
void eventQueueReturnToParent(eventQueueItem_t *item);

/// @}
#ifdef __cplusplus
}
#endif


#endif /// __EVENT_QUEUE_H__
