///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup dbgTracerApi Debug Tracer
/// @ingroup dbgTracerApi
/// @{
/// @brief Debug Tracer module API
///
/// Header abstract API for debug trace logging
///

#ifndef DBG_TRACER_API_H
#define DBG_TRACER_API_H

#if 1
#define DEBUG_LOG_LEVEL_LOW 	LOG_LEVEL_INFO
#define DEBUG_LOG_LEVEL_MEDIUM 	LOG_LEVEL_WARNING
#define DEBUG_LOG_LEVEL_HIGH 	LOG_LEVEL_ERROR
#include "logMsg.h"
#else

#include <dbgLogEvents.h>
#include <logMsg.h>

#include <stdint.h>
#include <stddef.h> // size_t

// TODO: remove those. obsolete
#define DEBUG_LOG_LEVEL_LOW 	LOG_LEVEL_INFO
#define DEBUG_LOG_LEVEL_MEDIUM 	LOG_LEVEL_WARNING
#define DEBUG_LOG_LEVEL_HIGH 	LOG_LEVEL_ERROR

#ifdef TRACE_PROFILER_ENABLED
#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////
/// @brief initTraceProfiler - Init function
///
/// @param buffer - trace profiler buffer. If NULL it will disable logging
/// @param size - trace buffer size
/// @param level - logging level. if set to 0, then profiling will be disabled
////////////////////////////////////////////////////////////////////////////
void initTraceProfiler(void* buffer, size_t size, uint32_t level);

/// @brief Logs Event from the system by a specific logger method
///
/// Components that are going to be logged are: Timestamp, eventID and 
/// data value (if any) to a (ring) buffer
/// @param[in] eventId    - ID of the event that will be logged
/// @param[in] data       - Data field containing custom info
/// @param[in] eventLevel - Debug Level for the current eventId to determine if
///            the event would be traced or not.
/// @return void
///
void dbgLogEvent(Event_t eventId, uint32_t data, uint32_t eventLevel);

/// @brief Log a formatted string message
/// @param as in printf family
void dbgLogMessage(const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#else // TRACE_PROFILER_ENABLED

#define eat_arg(...)

#ifndef initTraceProfiler
#define initTraceProfiler(a,b,c) (void)(a);(void)(b);(void)(c)
#endif

#ifndef dbgLogEvent
#define dbgLogEvent(a,b,c) (void)(a);(void)(b);(void)(c)
#endif

#ifndef dbgLogMessage
#define dbgLogMessage(f, ...) eat_arg(f, __VA_ARGS__)
#endif

#endif // TRACE_PROFILER_ENABLED
/// @}
#endif
#endif //_DBG_TRACER_API_H_

