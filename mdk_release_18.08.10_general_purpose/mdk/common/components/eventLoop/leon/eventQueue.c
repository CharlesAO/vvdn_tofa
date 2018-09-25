///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief   
///

/// System Includes
/// ----------------------------------------------------------------------------
#include <stdlib.h>
#include "swcLeonUtils.h"

/// Application Includes
/// ----------------------------------------------------------------------------
#include "eventQueue.h"

/// Source Specific #defines and types  (typedef,enum,struct)
/// ----------------------------------------------------------------------------
#ifndef EVENT_QUEUE_CODE_SECTION
#define EVENT_QUEUE_CODE_SECTION(x) x
#endif

/// Global Data (Only if absolutely necessary)
/// ----------------------------------------------------------------------------

/// Static Local Data
/// ----------------------------------------------------------------------------

/// Static Function Prototypes
/// ----------------------------------------------------------------------------

/// Functions Implementation
/// ----------------------------------------------------------------------------
void EVENT_QUEUE_CODE_SECTION(eventQueueInit)(eventQueue_t *self)
{
    /// Sanity checks
    if (self == NULL)
    {
        return;
    }

    /// Reset the queue
    self->head = NULL;
    self->tail = NULL;
    self->nItems = 0;
}

void eventQueuePush(eventQueue_t *self, eventQueueItem_t *item)
{
    /// Disable interrupts
    int pil = swcLeonSetPIL(15);

    /// Sanity checks
    if (self == NULL || item == NULL)
    {
        /// ReEnable interrupts
        swcLeonSetPIL(pil);
        return;
    }

    /// This element will be the last one
    item->next = NULL;

    if ((self->head == NULL)&& (self->tail == NULL))
    {
        /// First item on the queue
        self->head = item;
        self->tail = item;
    }
    else
    {
        /// Last item on the queue
        self->tail->next = item;
        self->tail = item;
    }

    self->nItems++;

    /// ReEnable interrupts
    swcLeonSetPIL(pil);
}

/// Return item of the head of list and remove it from list
eventQueueItem_t *eventQueuePop(eventQueue_t * self)
{
    //////////////////////
    /// Disable interrupts
    int pil = swcLeonSetPIL(15);

    eventQueueItem_t *item = NULL;
    /// Sanity checks
    if (self == NULL)
    {
        //////////////////////
        // ReEnable interrupts
        swcLeonSetPIL(pil);
        return NULL;
    }

    if (self->head != NULL)
    {
        /// Get head
        item = self->head;
        self->head = item->next;
        if (self->head == NULL)
        {
            self->tail = NULL;
        }
        self->nItems--;
    }

    //////////////////////
    // ReEnable interrupts
    swcLeonSetPIL(pil);

    return item;
}

/// Return item to parent queue
void eventQueueReturnToParent(eventQueueItem_t *item)
{
    /// Sanity check
    if ((item == NULL) || (item->parent == NULL))
    {
        return;
    }

    /// Make sure all the flags are reset when returning the event to it's parent queue
    item->busyFlags = 0;
    item->doneFlags = 0;
    item->requiredFlags = 0;

    /// Give this item back to its parent
    eventQueuePush(item->parent, item);
}
