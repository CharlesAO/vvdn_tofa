///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

/// System Includes
/// ----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "DrvTimer.h"
#include "swcLeonUtils.h"

/// Application Includes
/// ----------------------------------------------------------------------------
#include "eventLoop.h"

/// Source Specific #defines and types  (typedef,enum,struct)
/// ----------------------------------------------------------------------------
#ifndef EVENT_LOOP_DATA_SECTION
#define EVENT_LOOP_DATA_SECTION(x) x
#endif

#ifndef EVENT_LOOP_CODE_SECTION
#define EVENT_LOOP_CODE_SECTION(x) x
#endif

#define EVENT_LOOP_MAX_CALLBACKS 3

#define EVENT_LOOP_RUNNING 0xABCD

#define CALLBACK_NOT_FOUND 0xFFFF
/// This is the number of consecutive times event_loop_pop must return NULL
/// before the event loop will enter a low power state
/// Steve's wild ass guess is 33 ms * (180 ms / tick) / (~180 instructions)
#define EVENT_LOOP_IDLE_COUNT_US (33000)

/// Debug structures
/// MAX_LOOP_DEBUG must be power of 2
#define MAX_LOOP_DEBUG 64
#define MAX_LOOP_ERROR_COUNT 128

typedef struct eventLoopLog_t{
    u32 time;
    u32 timestamp;
    u32 rvalEvent;
    u32 dt;
} eventLoopLog_t;

/// Global Data (Only if absolutely necessary)
/// ----------------------------------------------------------------------------

/// Static Local Data
/// ----------------------------------------------------------------------------
eventLoopLog_t EVENT_LOOP_DATA_SECTION(eventLoopStatus)[MAX_LOOP_DEBUG];
eventLoopLog_t EVENT_LOOP_DATA_SECTION(eventLoopReport)[MAX_LOOP_DEBUG];
eventLoopLog_t EVENT_LOOP_DATA_SECTION(eventLoopErrors)[MAX_LOOP_ERROR_COUNT];


/// Debug variables, ending with Dbg
volatile s32 eventLoopDbg = 0;
static volatile u32 eventLoopLowPowerCountDbg = 0;
static volatile u32 eventLoopCountDbg = 0;
static u32 eventLoopIndexDbg = 0;
static u32 eventLoopErrorIndexDbg = 0;
/// This is the major event loop
static eventQueue_t eventLoop;

/// This is the command event loop
/// Command event loop is not cleared by start/stop
static eventQueue_t cmdQueue;

/// Event loop callbacks
/// This array of function pointers is called when it's event is received
static eventLoopCallback_t eventLoopCallbacks[EVENT_TYPE_LAST][EVENT_LOOP_MAX_CALLBACKS];
static eventLoopCallback_t eventLoopReleaseCallbacks[EVENT_TYPE_LAST];

/// System wide timestamper
static tyTimeStamp eventLoopInitTime;

/// Static Function Prototypes
/// ----------------------------------------------------------------------------

/// Functions Implementation
/// ----------------------------------------------------------------------------

static void EVENT_LOOP_CODE_SECTION(eventLoopDebug)()
{
    static int i, j;
    j = eventLoopIndexDbg;
    i = swcLeonReadNoCacheI32(&eventLoopDbg);

    if (i == 0)
    {
        return;
    }

    for (i = 0; i < MAX_LOOP_DEBUG; i++)
    {
        swcLeonMemCpy(&eventLoopReport[j],
                le_pointer, &eventLoopStatus[j],
                le_pointer, sizeof(eventLoopStatus[j]));
        j = (j + 1) & ((MAX_LOOP_DEBUG - 1));
    }

    swcLeonWriteNoCache32(&eventLoopDbg, 0);
}

/// Initialize the event loop queues
void EVENT_LOOP_CODE_SECTION(eventLoopInit)()
{
    /// Clear the event loop callbacks
    memset(eventLoopCallbacks, 0, sizeof(eventLoopCallbacks));
    memset(eventLoopReleaseCallbacks, 0, sizeof(eventLoopReleaseCallbacks));

    /// Clear the event queue
    eventQueueInit(&eventLoop);

    /// Clear the command queue
    eventQueueInit(&cmdQueue);
        }
void eventLoopSetReleaseCallback(eventType_t event, eventLoopCallback_t onReleaseCb)
{
    eventLoopReleaseCallbacks[event] = onReleaseCb;
}

/// Set an event loop callback
s32 EVENT_LOOP_CODE_SECTION(eventLoopSetCallback)(eventType_t event, eventLoopCallback_t cb)
{
    u32 i;

    /// Assert we aren't attaching bad callbacks
    assert(event < EVENT_TYPE_LAST);

    /// Search first available callback slot and return if callback already here
    for(i = 0 ; i < EVENT_LOOP_MAX_CALLBACKS && eventLoopCallbacks[event][i] != NULL; i++)
    {
        if(eventLoopCallbacks[event][i] == cb)
        {
            return 1;
        }
    }

    /// Save the callback for this event in the callback array
    if(i < EVENT_LOOP_MAX_CALLBACKS)
    {
        eventLoopCallbacks[event][i] = cb;
        return 0;
    }
    else
    {
        return -1;
    }
}

/// Add an event to the command queue
void eventLoopPushCmd(eventQueueItem_t *event)
{
    eventQueuePush(&cmdQueue, event);
}

/// Add an event to the event loop
void eventLoopPush(eventQueueItem_t *event)
{
    eventQueuePush(&eventLoop, event);
}

/// Reset the event loop
void EVENT_LOOP_CODE_SECTION(eventLoopReset)()
{
    /// Reset the event loop
    eventQueueInit(&eventLoop);
}

void EVENT_LOOP_CODE_SECTION(eventLoopStartTimer)()
{
    DrvTimerStartTicksCount(&eventLoopInitTime);
}

u64 eventLoopTimestamp()
{
    u64 ticks;
    DrvTimerGetElapsedTicks(&eventLoopInitTime, &ticks);
    return ticks;
}

static u16 eventLoopGetCallbackIndex(eventType_t evType, eventLoopCallback_t evCallbackPtr)
{
    u32 evIndex;

    assert (evType < EVENT_TYPE_LAST);

    for(evIndex = 0 ; evIndex < EVENT_LOOP_MAX_CALLBACKS ; evIndex++)
    {
        if(eventLoopCallbacks[evType][evIndex] == evCallbackPtr)
        {
            return evIndex;
        }
    }

    return CALLBACK_NOT_FOUND;
}

/// The function modifies the event type to newEventType. It checks all callbacks.
/// The following situations are handled for every callback:
///     - If the new event type has the same callback as the current event type,
///       the flags (required, busy, done) will be copied to the corresponding location
///     - If the new event type doesn't have the same callback as the current event type,
///       but the callback is already handled (is required and is done) the event type can be changed
///     - If the new event type doesn't have the same callback as the current event type,
///       and the callback is not handled (is required and is not done) the event type can't be changed.
///       The function will block in an assert.
///  Changing the event type is a bad practice. Use this function only if you know the event loop well.
void eventLoopChangeEventType(eventQueueItem_t *event,
                              eventType_t newEventType)
{
    /// Sanity checks
    if((event == NULL) || (event->type == newEventType))
    {
        return;
    }
    assert(newEventType < EVENT_TYPE_LAST);
    u32 cbIndex;
    u8 busyFlags = event->busyFlags;
    u8 requiredFlags = event->requiredFlags;
    u8 doneFlags = event->doneFlags;
    /// Reset the flags for the new event. We will copy the needed flags.
    event->busyFlags = 0;
    event->requiredFlags = 0;
    event->doneFlags = 0;

    /// Check all the callbacks for the current event type
    for (cbIndex = 0 ; cbIndex < EVENT_LOOP_MAX_CALLBACKS ; cbIndex++)
    {
        eventLoopCallback_t callback = eventLoopCallbacks[event->type][cbIndex];
        if( callback != NULL)
        {
            u16 newCallbackIndex = eventLoopGetCallbackIndex(newEventType,callback);
            if (newCallbackIndex != CALLBACK_NOT_FOUND)
            {
                /// The callback exists for the new event type too. Copy the callback flags to the corresponding position
                event->busyFlags |= (((busyFlags & (1 << cbIndex)) >> cbIndex) << newCallbackIndex);
                event->doneFlags |= (((doneFlags & (1 << cbIndex)) >> cbIndex) << newCallbackIndex);
                event->requiredFlags |= (((requiredFlags & (1 << cbIndex)) >> cbIndex) << newCallbackIndex);
            }
            else
            {
                /// The callback does not exist for the new event type
                /// If this callback is not done, we can't change the event type
                if (event->requiredFlags & (1 << cbIndex))
                {
                    assert(doneFlags & (1 << cbIndex));
                }
            }
        }
    }
    event->type = newEventType;

}
void eventLoopSetEventState(eventState_t evState, eventQueueItem_t *event,
        eventLoopCallback_t evCallbackPtr)
{
    if(event == NULL)
    {
        return;
    }

    u16 eventCallbackFlag = eventLoopGetCallbackIndex(event->type, evCallbackPtr);
    assert(eventCallbackFlag != CALLBACK_NOT_FOUND);
    eventCallbackFlag = 1 << eventCallbackFlag;
    switch (evState)
    {
    case BUSY:
        event->busyFlags |= eventCallbackFlag;
        break;
    case DONE:
        event->doneFlags |= eventCallbackFlag;
        break;
    case REQUIRED:
        event->requiredFlags |= eventCallbackFlag;
        break;
    case DROPPED:
        event->dropFlags |= eventCallbackFlag;
        break;
    default:
        assert(0);
    }
}

void eventLoopClearBusyFlag(eventQueueItem_t *event,
                            eventLoopCallback_t evCallbackPtr)
{
    u16 eventCallbackFlag = eventLoopGetCallbackIndex(event->type, evCallbackPtr);
    assert(eventCallbackFlag != CALLBACK_NOT_FOUND);
    eventCallbackFlag = ~(1 << eventCallbackFlag);
    event->busyFlags &= eventCallbackFlag;

}
static u32 eventLoopIsEventCbUnhandled(eventQueueItem_t *event, eventLoopCallback_t evCallbackPtr)
{
    u16 eventCallbackFlag = eventLoopGetCallbackIndex(event->type, evCallbackPtr);
    assert(eventCallbackFlag != CALLBACK_NOT_FOUND);
    eventCallbackFlag = 1 << eventCallbackFlag;
    if((event->doneFlags & eventCallbackFlag) ||
            (event->busyFlags & eventCallbackFlag))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void EVENT_LOOP_CODE_SECTION(eventLoopDropEvent)(eventQueueItem_t *event, eventLoopCallback_t evCallbackPtr)
{
    if(event == NULL)
    {
        return;
    }
    s32 pil = swcLeonSetPIL(15);

    /// Simulate the event was busy when dropping it
    eventLoopSetEventState(BUSY, event, evCallbackPtr);
    eventLoopSetEventState(DROPPED, event, evCallbackPtr);

    eventLoopReleaseEvent(event, evCallbackPtr);
    swcLeonSetPIL(pil);

}

void eventLoopReleaseEvent(eventQueueItem_t *event, eventLoopCallback_t evCallbackPtr)
{
    s32 pil = swcLeonSetPIL(15);
    //// Register the event as done
    eventLoopSetEventState(DONE, event, evCallbackPtr);

    /// Check that all the started callbacks returned successfully
    if(event->requiredFlags != event->doneFlags)
    {
        swcLeonSetPIL(pil);
        return;
    }

    if (eventLoopReleaseCallbacks[event->type] != NULL)
        eventLoopReleaseCallbacks[event->type](event);
    /// If all is done, mark the event as done and free it
    event->requiredFlags = event->busyFlags = event->doneFlags = event->dropFlags = 0;
    eventQueueReturnToParent(event);
    swcLeonSetPIL(pil);
}


void eventAppConfigLeonPowerDown(void)
{
    asm("wr %g0, %asr19");
}


/// Process the events
void eventLoopRun()
{
    u32 lowPowerRequests = 0;
    u64 clockIn, clockOut;

    eventQueueItem_t *event = NULL;

    /// Clear the loop_debugger
    memset(eventLoopStatus, 0, sizeof(eventLoopStatus));
    eventLoopIndexDbg = 0;

    /// Start the clock
    DrvTimerStartTicksCount(&eventLoopInitTime);

    while (1)
    {
        s32 rval = 0;
        u32 evCbIndex;
        enum { HANDLED, UNHANDLED } evHandlingCheck;

        /// Get a timestamp for profiling
        clockIn = eventLoopTimestamp();

        /// Optionally debug the loop
        eventLoopDebug();

        /// Get an item from the cmd_queue
        event = eventQueuePop(&cmdQueue);

        /// If the command queue is empty
        if (event == NULL)
        {
            /// Get a new item from the event queue
            event = eventQueuePop(&eventLoop);
        }

        /// Count reguardless
        eventLoopCountDbg++;

        /// If both are empty
        if (event == NULL)
        {
            /// If the queue is empty, only interrupts can push new items
            lowPowerRequests++;
            /// Steve's magic number
            if (lowPowerRequests > EVENT_LOOP_IDLE_COUNT_US)
            {
                /// Count the number of times we enter low power mode
                eventLoopLowPowerCountDbg++;
                /// Put the CPU to sleep until a new item is pushed
               // eventAppConfigLeonPowerDown();
            }
            /// Do not put a sleep here.
            /// Sleep just busy loops anyway.
            /// So why not keep our latency down.
            continue;
        }
        /// When we have something to do, reset the low_power_counter
        lowPowerRequests = 0;

        /// Handle the event
        assert(event->type < EVENT_TYPE_LAST);

        evHandlingCheck = UNHANDLED;
        for(evCbIndex = 0 ; evCbIndex < EVENT_LOOP_MAX_CALLBACKS; evCbIndex++)
        {
            if(eventLoopCallbacks[event->type][evCbIndex] != NULL && /// check that we have a valid callback
                    eventLoopIsEventCbUnhandled(event, eventLoopCallbacks[event->type][evCbIndex])) /// check that the callback is neither busy nor done
            {
                /// Valid callback found, marking as required
                eventLoopSetEventState(REQUIRED, event, eventLoopCallbacks[event->type][evCbIndex]);

                /// Record the event type and mark as running
                eventLoopStatus[eventLoopIndexDbg].rvalEvent = (EVENT_LOOP_RUNNING << 16) | event->type;

                /// Save this event start timestamp before the callback
                eventLoopStatus[eventLoopIndexDbg].dt = 0;
                eventLoopStatus[eventLoopIndexDbg].time = (u32)(clockIn);
                eventLoopStatus[eventLoopIndexDbg].timestamp = (u32)(event->timestamp);

                /// Run the callback function registered
                rval = (*eventLoopCallbacks[event->type][evCbIndex])(event);

                /// Profile this step
                DrvTimerGetElapsedTicks(&eventLoopInitTime, &clockOut);
                eventLoopStatus[eventLoopIndexDbg].dt = (u32) (clockOut - clockIn);
                /// Log the event return value
                eventLoopStatus[eventLoopIndexDbg].rvalEvent = ((rval << 16) |
                        (eventLoopStatus[eventLoopIndexDbg].rvalEvent & 0xFFFF));
                if (rval < 0)
                {
                    eventLoopErrors[eventLoopErrorIndexDbg].dt = eventLoopStatus[eventLoopIndexDbg].dt;
                    eventLoopErrors[eventLoopErrorIndexDbg].time = eventLoopStatus[eventLoopIndexDbg].time;
                    eventLoopErrors[eventLoopErrorIndexDbg].timestamp = eventLoopStatus[eventLoopIndexDbg].timestamp;
                    eventLoopErrors[eventLoopErrorIndexDbg].rvalEvent = eventLoopStatus[eventLoopIndexDbg].rvalEvent;
                    /// Increment the error index
                    eventLoopErrorIndexDbg = (eventLoopErrorIndexDbg + 1) & (MAX_LOOP_ERROR_COUNT - 1);
                }

                /// Increment the loop_index
                eventLoopIndexDbg = (eventLoopIndexDbg + 1) & (MAX_LOOP_DEBUG - 1);

                evHandlingCheck = HANDLED;
            }
        }

        /// Check if event has been properly handled
        assert(evHandlingCheck == HANDLED);
    }
}
