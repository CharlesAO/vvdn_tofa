///
/// Header for Event ID list
///

#ifndef LOCAL_LOG_EVENTS_H
#define LOCAL_LOG_EVENTS_H

#include <dbgLogEvents.h>

//List of event IDs for the local events
typedef enum
{
    LOG_EVENT_MY_APP_RUN = LOG_EVENT_LAST_EVENT + 1
} LocalEvent_t;

#endif //_LOG_EVENTS_H_
