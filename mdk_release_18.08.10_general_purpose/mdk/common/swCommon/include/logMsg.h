/**
 * @brief Trace Logging Header File
 *
 *
 * ==================================================================
 * Sample usage:
 *
 * // print a simple string
 * LOG_INFO("This is a info string with default level 4\n");
 *
 * // print formatted strings as well. Uses printf-like syntax
 * LOG_ERROR("Now we are printing error: %d\n", errno);
 *
 * // dump binary data
 * LOG_BULK_TRACE(data, size);
 *
 * // send an event
 * LOG_TRACE_EVENT(id, data);
 *
 * ==================================================================
 * Configuration:
 * Add the following defines to the compiler using -D switch or simply #re-define in code
 *
 * MV_UNIT_NAME - Used to set the dynamic log level per unit basis. Define it before including
 * this header. Default is empty
 * When the unit name is defined, a weak int will be generated with <unit name>_traceLogLevel name
 * otherwise will be simply `int _traceLogLevel;`
 *
 * DEFAULT_LOG_LEVEL - Is the default log level. If not set it is LOG_LEVEL_INFO(4)
 * Everything above this value will be ignored
 *
 * MAX_STATIC_LOG_LEVEL - Compile out all messages above this value
 *
 * SINK_FUNCTION - the function used to print printf-like strings
 * void SINK_FUNCTION(const char* __restrict msg);
 * Predefined values:
 * - _printf_clone (default) - use the standard printf function
 * - _trace_print - use the TraceProfiler
 *
 * SINK_BULK - function dumping out bulk messages
 * void SINK_BULK(const int level, void * __restrict data, size_t size);
 * Predefined values:
 * - _bulk_hexdump (default) - write using printf the data as hex values
 *
 * TRACE_BUFFER_SIZE - size in bytes for the TraceProfiler buffer
 *
 * MV_DBG_FMT_STR_SIZE - size in bytes of the formatted string for printf-like functions
 * The default value is 256
 *
 * DBG_ARGx (x=1..6) - prefix used in DBG_LOG()/DBG_ERROR()/...
 * The values are a printf-like argument, and several predefined values can be used
 * The number x represent the position in string. Gaps are allowed. Numbers out-of-range will be
 * silently ignored
 * Predefined values:
 * DBG_PRINT_FILE_LINE
 * DBG_PRINT_MODULE_NAME
 * DBG_PRINT_TIMESTAMP
 * DBG_PRINT_LOG_LEVEL
 * DBG_PRINT_THREAD
 * DBG_PRINT_CORE_ID
 *
 *
 * Example:
 * #define DBG_ARG4 DBG_PRINT_THREAD
 * #define DBG_ARG2 DBG_PRINT_TIMESTAMP
 * gcc -DDBG_ARG6=DBG_PRINT_FILE_LINE
 *
 * SYNC_LOG_MSG - Put sink function in a critical section (mutex for shave/disable interrupts
 * for rtems/set max interrupt level for bm)
 * Unset by default for speed purposes. Define this if you see scrambled messages
 *
 * =====================================
 * For C++ there are extra two functions:
 * static inline void logMessage(int logLevel, const char* __restrict format, ...);
 * static inline void logMessage(int logLevel, const char* __restrict format, va_list args);
 *
 * For conveninece, also can be ostreams like cout (mv_fatal, mv_err, mv_warn, etc):
 * mv_info << "This is info message with value " << 101 << "\n";
 */
#ifndef __PROFILE_H__
#define __PROFILE_H__

// PC compatibility
#if !defined(__leon__) && !defined(__shave__)
#define __PC__
#endif

#include <stdlib.h> // size_t
#include <stdint.h>
#include <stdio.h> // default printing sinks

// for default sink dbgLogEvent()
#include <dbgLogEvents.h> // Event_t

#include <stdarg.h> // until clang will know __builtin_va_arg_pack()

#ifdef __shave__
#include <ShDrvMutex.h>
#endif

// TODO: merge with the one from functionProfiler.h and compute delta between timers
#ifdef __PC__
#include <time.h>
#define _GNU_SOURCE
#include <pthread.h> // link with -lpthread
#else
#include <DrvRegUtils.h>
#include <registersMyriad.h>
#include <swcWhoAmI.h>
#include <DrvSvuDefines.h>
#endif

#ifdef __cplusplus
// TODO: provide better C++ API
extern "C++" {
#include <string> // to_string
}
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
// configurable macros
//////////////////////////////////////////////////////////////////////////

#ifndef SINK_FUNCTION
#define SINK_FUNCTION _printf_clone
#endif
void SINK_FUNCTION(const char* __restrict msg);

#ifndef SINK_BULK
#define SINK_BULK _bulk_hexdump
#endif
void SINK_BULK(void * __restrict data, size_t size);

// TODO: generic buffer for dynamic loading
#ifndef TRACE_BUFFER_SIZE
#define TRACE_BUFFER_SIZE (1024*1024)
#endif

#ifdef __leon__
#ifdef __RTEMS__
#define CRITICAL_SECTION_ENTER do { \
    rtems_interrupt_level interrupts = 0; \
    rtems_interrupt_disable(interrupts);
#else
#define CRITICAL_SECTION_ENTER do { \
    uint32_t pil = swcLeonSetPIL(15);
#endif
#ifdef __RTEMS__
#define CRITICAL_SECTION_EXIT \
    rtems_interrupt_enable(interrupts); \
    } while (0);
#else
#define CRITICAL_SECTION_EXIT \
    swcLeonSetPIL(pil); \
    } while (0);
#endif
#else // shave
#define CRITICAL_SECTION_ENTER do { \
    ShDrvMutexRequest(4);
#define CRITICAL_SECTION_EXIT \
    ShDrvMutexRelease(4); \
    } while (0);
#endif


//////////////////////////////////////////////////////////////////////////
// constants
//////////////////////////////////////////////////////////////////////////

#define STR_IMPL_(x) #x
#define STR(x) STR_IMPL_(x)

#define LOG_LEVEL_FATAL     1
#define LOG_LEVEL_ERROR     2
#define LOG_LEVEL_WARNING   3
#define LOG_LEVEL_INFO      4
#define LOG_LEVEL_DEBUG     5
#define LOG_LEVEL_TRACE     6

#define DBG_FATAL "<" STR(LOG_LEVEL_FATAL) ">"
#define DBG_ERROR "<" STR(LOG_LEVEL_ERROR) ">"
#define DBG_WARNING "<" STR(LOG_LEVEL_WARNING) ">"
#define DBG_INFO "<" STR(LOG_LEVEL_INFO) ">" /* default */
#define DBG_DEBUG "<" STR(LOG_LEVEL_DEBUG) ">"
#define DBG_TRACE "<" STR(LOG_LEVEL_TRACE) ">"

#define DBG_CHAR_LOG_TYPE(lvl) ('0' + lvl)
#define DBG_MAX_LEVEL DBG_CHAR_LOG_TYPE(LOG_LEVEL_TRACE)

#ifndef DEFAULT_LOG_LEVEL
#define DEFAULT_LOG_LEVEL DBG_CHAR_LOG_TYPE(LOG_LEVEL_INFO)
#endif

// max level to include. what is above will be compiled-out
#ifndef MAX_STATIC_LOG_LEVEL
#define MAX_STATIC_LOG_LEVEL LOG_LEVEL_TRACE
#endif

/*
Static allocated buffer size for printf-like conversion string
*/
#ifndef MV_DBG_FMT_STR_SIZE
#define MV_DBG_FMT_STR_SIZE 256u
#endif

#ifndef MV_UNIT_NAME
#define MV_UNIT_NAME _
#endif
#define _MV_LOG_LEVEL(UNIT) UNIT ## _traceLogLevel
#define MV_LOG_LEVEL(UNIT) _MV_LOG_LEVEL(UNIT)
// can be replaced by a strong symbol, and/or modified at runtime
// FIXME: ugly hack until moviCompile will support weak variables
__attribute__((weak))
#ifdef __shave__
int MV_LOG_LEVEL(MV_UNIT_NAME)() {
    static int MV_LOG_LEVEL(MV_UNIT_NAME) = DEFAULT_LOG_LEVEL;
    return MV_LOG_LEVEL(MV_UNIT_NAME);
}
#define _traceLogLevel MV_LOG_LEVEL(MV_UNIT_NAME)()
#else
int MV_LOG_LEVEL(MV_UNIT_NAME) = DEFAULT_LOG_LEVEL;
#define _traceLogLevel MV_LOG_LEVEL(MV_UNIT_NAME)
#endif


//////////////////////////////////////////////////////////////////////////
// public API
//////////////////////////////////////////////////////////////////////////

#define TIMER_ADDR TIM0_BASE_ADR

#ifdef __PC__
#define FP_TIME_READ() time(NULL)
#else
#ifdef MA2X8X
#define FP_TIME_READ() ({ \
    uint64_t ___low  = (uint64_t)GET_REG_WORD_VAL(TIMER_ADDR + TIM_FREE_CNT0_OFFSET); \
    uint64_t ___high  = (uint64_t)GET_REG_WORD_VAL(TIMER_ADDR + TIM_FREE_CNT1_OFFSET); \
    uint64_t ___full = (___high  << 32) | ___low; \
    ___full; \
})
#else
#define FP_TIME_READ() ({ \
    uint64_t ___high  = (uint64_t)GET_REG_WORD_VAL(TIMER_ADDR + TIM_FREE_CNT1_OFFSET); \
    uint64_t ___low  = (uint64_t)GET_REG_WORD_VAL(TIMER_ADDR + TIM_FREE_CNT0_OFFSET); \
    uint64_t ___full = (___high  << 32) | ___low; \
    ___full; \
})
#endif
#endif

__attribute__((no_instrument_function))
static inline char* getMyThreadName(void) {
    static char buf[16]; // FIXME
    int	pthread_getname_np(pthread_t, char *, size_t);
    pthread_t pthread_self(void);
    if (pthread_getname_np(pthread_self(), buf, sizeof(buf)) != 0){
        __builtin_strcpy(buf, "<unknown>");
    }
    return buf;
}

#define DBG_PRINT_FILE_LINE "[File: %s, Line: %d]\t", __FILE__, __LINE__
#define DBG_PRINT_MODULE_NAME "[Module: " STR(MV_UNIT_NAME) "]\t"
#define DBG_PRINT_TIMESTAMP "[Timestamp: %lld]\t", FP_TIME_READ()
#define DBG_PRINT_LOG_LEVEL "[Severity: %c]\t", _traceLogLevel
#if defined(__RTEMS__)
pthread_t pthread_self(void);
#define DBG_PRINT_THREAD "[Thread: %s, Id: 0x%lx]\t", getMyThreadName(), pthread_self()
#elif defined(__PC__)
#define DBG_PRINT_THREAD "[Thread: %s, Id: 0x%lx]\t", getMyThreadName(), pthread_self()
#else
#define DBG_PRINT_THREAD
#endif
#ifdef __PC__
#define DBG_PRINT_CORE_ID "[CPU: x86]\t"
#else
#define DBG_PRINT_CORE_ID "[Core: %d]\t", swcWhoAmI()
#endif

#define _FIRST_ARG(a,...) a
#define FIRST_ARG(...) _FIRST_ARG(__VA_ARGS__)
#define _SECOND_ARG(a, ...) , ##__VA_ARGS__
#define SECOND_ARG(...) _SECOND_ARG(__VA_ARGS__)

#ifdef DBG_ARG1
#define FL_STR1 FIRST_ARG(DBG_ARG1)
#define FL_ARG1 SECOND_ARG(DBG_ARG1)
#else
#define FL_STR1
#define FL_ARG1
#endif
#ifdef DBG_ARG2
#define FL_STR2 FIRST_ARG(DBG_ARG2)
#define FL_ARG2 SECOND_ARG(DBG_ARG2)
#else
#define FL_STR2
#define FL_ARG2
#endif
#ifdef DBG_ARG3
#define FL_STR3 FIRST_ARG(DBG_ARG3)
#define FL_ARG3 SECOND_ARG(DBG_ARG3)
#else
#define FL_STR3
#define FL_ARG3
#endif
#ifdef DBG_ARG4
#define FL_STR4 FIRST_ARG(DBG_ARG4)
#define FL_ARG4 SECOND_ARG(DBG_ARG4)
#else
#define FL_STR4
#define FL_ARG4
#endif
#ifdef DBG_ARG5
#define FL_STR5 FIRST_ARG(DBG_ARG5)
#define FL_ARG5 SECOND_ARG(DBG_ARG5)
#else
#define FL_STR5
#define FL_ARG5
#endif
#ifdef DBG_ARG6
#define FL_STR6 FIRST_ARG(DBG_ARG6)
#define FL_ARG6 SECOND_ARG(DBG_ARG6)
#else
#define FL_STR6
#define FL_ARG6
#endif

#define FL_STR  FL_STR1 FL_STR2 FL_STR3 FL_STR4 FL_STR5 FL_STR6
#define FL_ARGS FL_ARG1 FL_ARG2 FL_ARG3 FL_ARG4 FL_ARG5 FL_ARG6

#if MAX_STATIC_LOG_LEVEL >= LOG_LEVEL_TRACE
#define LOG_TRACE(fmt, ...) logMsg(DBG_TRACE FL_STR fmt FL_ARGS, ##__VA_ARGS__)
#define LOG_BULK_TRACE(data, size) logBulk(LOG_LEVEL_TRACE, data, size)
#define LOG_TRACE_EVENT(id, data)   dbgLogEvent(id, data, LOG_LEVEL_TRACE)
#else
#define LOG_TRACE(...)
#define LOG_BULK_TRACE(data, size) (void)data;(void)size
#define LOG_TRACE_EVENT(id, data)   (void)id;(void)data
#endif

#if MAX_STATIC_LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_DEBUG(fmt, ...) logMsg(DBG_DEBUG FL_STR fmt FL_ARGS, ##__VA_ARGS__)
#define LOG_BULK_DEBUG(data, size) logBulk(LOG_LEVEL_DEBUG, data, size)
#define LOG_DEBUG_EVENT(id, data)   dbgLogEvent(id, data, LOG_LEVEL_DEBUG)
#else
#define LOG_DEBUG(...)
#define LOG_BULK_DEBUG(data, size) (void)data;(void)size
#define LOG_DEBUG_EVENT(id, data)   (void)id;(void)data
#endif

#if MAX_STATIC_LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG_INFO(fmt, ...) logMsg(DBG_INFO FL_STR fmt FL_ARGS, ##__VA_ARGS__)
#define LOG_BULK_INFO(data, size) logBulk(LOG_LEVEL_INFO, data, size)
#define LOG_INFO_EVENT(id, data)    dbgLogEvent(id, data, LOG_LEVEL_INFO)
#else
#define LOG_INFO(...)
#define LOG_BULK_INFO(data, size) (void)data;(void)size
#define LOG_INFO_EVENT(id, data)    (void)id;(void)data
#endif

#if MAX_STATIC_LOG_LEVEL >= LOG_LEVEL_WARNING
#define LOG_WARNING(fmt, ...) logMsg(DBG_WARNING FL_STR fmt FL_ARGS, ##__VA_ARGS__)
#define LOG_BULK_WARNING(data, size) logBulk(LOG_LEVEL_WARNING, data, size)
#define LOG_WARNING_EVENT(id, data) dbgLogEvent(id, data, LOG_LEVEL_WARNING)
#else
#define LOG_WARNING(...)
#define LOG_BULK_WARNING(data, size) (void)data;(void)size
#define LOG_WARNING_EVENT(id, data) (void)id;(void)data
#endif

#if MAX_STATIC_LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOG_ERROR(fmt, ...) logMsg(DBG_ERROR FL_STR fmt FL_ARGS, ##__VA_ARGS__)
#define LOG_BULK_ERROR(data, size) logBulk(LOG_LEVEL_ERROR, data, size)
#define LOG_ERROR_EVENT(id, data)   dbgLogEvent(id, data, LOG_LEVEL_ERROR)
#else
#define LOG_ERROR(...)
#define LOG_BULK_ERROR(data, size) (void)data;(void)size
#define LOG_ERROR_EVENT(id, data)   (void)id;(void)data
#endif

#if MAX_STATIC_LOG_LEVEL >= LOG_LEVEL_FATAL
#define LOG_FATAL(fmt, ...) logMsg(DBG_FATAL FL_STR fmt FL_ARGS, ##__VA_ARGS__)
#define LOG_BULK_FATAL(data, size) logBulk(LOG_LEVEL_FATAL, data, size)
#define LOG_FATAL_EVENT(id, data)   dbgLogEvent(id, data, LOG_LEVEL_FATAL)
#else
#define LOG_FATAL(...)
#define LOG_BULK_FATAL(data, size) (void)data;(void)size
#define LOG_FATAL_EVENT(id, data)   (void)id;(void)data
#endif
// TODO: replace everywhere dbgLogEvent with LOG_INFO_EVENT, then remove from interface dbgLogEvent

//////////////////////////////////// Implementation /////////////////////////////////////
void _printf_clone(const char* __restrict msg);
void logBulk(const int level, void * __restrict data, size_t size);

// backends here

/////////////////////////////////////////////////////////////////////
// start of event
////////////////////////////////////////////////////////////////////
#ifdef TRACE_PROFILER_ENABLED
#include <profile.h>

#ifdef INHIBIT_TRACE_PROFILER_CODE_GEN
void dbgLogEvent(Event_t eventId, uint32_t data, int eventLevel);
#else

typedef struct  __attribute__ ((packed)) TraceProfileEntry_t {
    // if eventId is -1, then is variable length
    // for variable length, the data is the payload, and the following 'data' bytes
    // from the following records represent the message
    uint32_t eventId;   // TODO: can be uit16_t (or less)
    uint32_t data;      // TODO: can be optional (based on id)
    uint32_t caller;    //
    uint64_t timestamp; // TODO: use delta
} TraceProfileEntry_t;

// !!!! IMPORTANT !!!
// compile this with -fdata-sections -ffunction-sections
// link with --gc-sections
__attribute__((weak, no_instrument_function))
ProfileBufferEntry_t* _getTraceProfileBuffer() {
    static ProfileBufferEntry_t tProfile __attribute__((section(".ddr_direct.bss"))) = {NULL, NULL, NULL};
    return &tProfile;
}

__attribute__((weak, no_instrument_function))
__attribute__((constructor))
void _init_code__() {
    if (__profileBufferMasterRecord__.tProf) return;
    static char traceBuffer[TRACE_BUFFER_SIZE] __attribute__((section(".ddr_direct.bss")));
    // TODO: when someone will figure out that trace profiler is working for shaves as well
    // make an init function for shaves
    ProfileBufferEntry_t* tProfile = _getTraceProfileBuffer();
    tProfile->buffer = tProfile->head = traceBuffer;
    tProfile->end = &((TraceProfileEntry_t*)tProfile->buffer)[TRACE_BUFFER_SIZE / sizeof(TraceProfileEntry_t)];
    __profileBufferMasterRecord__.tProf = tProfile;
    __profileBufferMasterRecord__.coreId = swcWhoAmI();

    __builtin_memset(traceBuffer, 0, TRACE_BUFFER_SIZE);

//    printf("Init done for %d\n", __profileBufferMasterRecord__.coreId);

}

// sample profiler hooks
__attribute__((weak, no_instrument_function))
void initSampleProfiler(uint32_t __attribute__((unused)) shaveId){}
__attribute__((weak, no_instrument_function))
void removeShaveFromList(uint32_t __attribute__((unused)) shaveId){}

// TODO: disable Wframe-address warning
__attribute__((weak, no_instrument_function))
void dbgLogEvent(Event_t eventId, uint32_t data, int eventLevel)
{
    ProfileBufferEntry_t* tProfile = _getTraceProfileBuffer();
    // TODO: make this generic when others will be interested in settings callbacks on specific events
    if (LOG_EVENT_SHAVE_0_RUN <= eventId && eventId < LOG_EVENT_SHAVE_0_RUN + TOTAL_NUM_SHAVES) {
        uint32_t shaveNum = eventId - LOG_EVENT_SHAVE_0_RUN;
        if (data)
            initSampleProfiler(shaveNum);
        else
            removeShaveFromList(shaveNum);
    }
#ifdef __shave__
    // TODO: remove this when shave will call constructors
    _init_code__();
#endif
    if (tProfile->head && eventLevel <= _traceLogLevel) {
        CRITICAL_SECTION_ENTER
        profilePushEntry(TraceProfileEntry_t, (*tProfile), eventId, data,
                         (uint32_t)__builtin_extract_return_addr(__builtin_return_address(0)),
                         FP_TIME_READ());
        CRITICAL_SECTION_EXIT
    }
}

__attribute__((weak, no_instrument_function))
void _dbgLogPlainMessage(const char* __restrict msg, size_t msgLen) {
    // variable length
    ProfileBufferEntry_t* tProfile = _getTraceProfileBuffer();
    profilePushEntry(TraceProfileEntry_t, (*tProfile), (Event_t)-1, (uint32_t)msgLen,
                     (uint32_t)__builtin_extract_return_addr(__builtin_return_address(0)),
                     FP_TIME_READ());
    // compute mesage size that should be multiple of TraceProfileEntry_t
    msgLen = ((msgLen + sizeof(TraceProfileEntry_t) - 1) / sizeof(TraceProfileEntry_t)) *
             sizeof(TraceProfileEntry_t);
    while (msgLen) {
        profilePushEntry(TraceProfileEntry_t, (*tProfile),
                         (uint32_t) ((uint32_t)msg[0] | (uint32_t)msg[1] << 8 | (uint32_t)msg[2] << 16 | (uint32_t)msg[3] << 24),
                         (uint32_t) ((uint32_t)msg[4] | (uint32_t)msg[5] << 8 | (uint32_t)msg[6] << 16 | (uint32_t)msg[7] << 24),
                         (uint32_t) ((uint32_t)msg[8] | (uint32_t)msg[9] << 8 | (uint32_t)msg[10] << 16 | (uint32_t)msg[11] << 24),
                         (uint64_t) ((uint64_t)msg[12] | (uint64_t)msg[13] << 8 | (uint64_t)msg[14] << 16 | (uint64_t)msg[15] << 24 | (uint64_t)msg[16] << 32 | (uint64_t)msg[17] << 40 | (uint64_t)msg[18] << 48 | (uint64_t)msg[19] << 56)
        );
        msgLen -= sizeof(TraceProfileEntry_t);
        msg += sizeof(TraceProfileEntry_t);
    }
}
/**
 * @brief Print using the TraceProfiler as backend
 * @param msg  null terminated string to be logged in trace profiler
 */
__attribute__((weak, no_instrument_function))
void _trace_print(const char* __restrict msg){
    _dbgLogPlainMessage(msg, __builtin_strlen(msg)); // TODO: strlen can be precomputed from vsnprintf
}

#endif // INHIBIT_TRACE_PROFILER_CODE_GEN
#else

#define dbgLogEvent(a,b,c) (void)(a);(void)(b);(void)(c)
#define _dbgLogPlainMessage(a,b) (void)(a);(void)(b)

#endif // TRACE_PROFILER_ENABLED

////////////////////////////////////////////////////////////////////
// end of event
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
// SINK functions
// what are you sinkin' about?
////////////////////////////////////////////////////////////////////

__attribute__((weak, no_instrument_function))
void _printf_clone(const char* __restrict msg){
    /* __builtin_puts(msg); */
   printf("%s\n", msg);
   UNUSED(msg); // useful in case printf is an empty macro
}

__attribute__((weak, no_instrument_function))
void _bulk_hexdump(void * __restrict data, size_t size) {
    for (unsigned i=0; i<size; i++) {
        uint8_t _byte = ((uint8_t*)data)[i];
        printf("0x%x%c", _byte, i%10==9?'\n':' ');
        UNUSED(_byte); // useful in case printf is an empty macro
    };
    printf("\n");
}


// I feel a great disturbance in the Source

//__attribute__ ((format (printf, 1, 2), ifunc ("resolve_dbgMessage")))

#ifdef __cplusplus
extern "C++" {
static inline void logMessage(int logLevel, const char* __restrict format, ...) {
    if (_traceLogLevel < logLevel) return;
    va_list ap;
    char buffer[MV_DBG_FMT_STR_SIZE];
    va_start(ap, format);
    int nbBytes = __builtin_vsnprintf(buffer, MV_DBG_FMT_STR_SIZE, format, ap);
    va_end(ap);
    if ( nbBytes < 0 ) return;
    SINK_FUNCTION(buffer);
}
static inline void logMessage(int logLevel, const char* __restrict format, va_list args) {
    if (_traceLogLevel < logLevel) return;
    char buffer[MV_DBG_FMT_STR_SIZE];
    int nbBytes = __builtin_vsnprintf(buffer, MV_DBG_FMT_STR_SIZE, format, args);
    if ( nbBytes < 0 ) return;
    SINK_FUNCTION(buffer);
}
class MvLogMessage {
    const int _severity;
public:
    MvLogMessage(int severity) : _severity(severity) {
    }

    template <typename T>
    MvLogMessage& operator << (const T& t) {
        write(t);
        return *this;
    }

    template <typename T>
    void write(const T& t) {
        write(std::to_string(t).c_str());
    }
    void write(const char* msg){
        if (_traceLogLevel < _severity) return;
        SINK_FUNCTION(msg);
    }
    void write(const char* msg, size_t size){
        if (_traceLogLevel < _severity) return;
        char buffer[MV_DBG_FMT_STR_SIZE];
        __builtin_strncpy(buffer, msg, size);
        buffer[size] = '\0';
        SINK_FUNCTION(buffer);
    }
};
__attribute__((weak))
MvLogMessage mv_fatal(LOG_LEVEL_FATAL), mv_err(LOG_LEVEL_ERROR), mv_warn(LOG_LEVEL_WARNING),
             mv_info(LOG_LEVEL_INFO), mv_debug(LOG_LEVEL_DEBUG), mv_trace(LOG_LEVEL_TRACE);
}
#else
// no alternative for C. sorry!
#endif

/**
 * Log formatted string messages
 * @param format
 * @param ...
 * static because __builtin_va_arg_pack()
 */
__attribute__ ((format (printf, 1, 2), nonnull(1), no_instrument_function))
#if (__leon__)
__attribute__((always_inline)) // for __builtin_va_arg
#endif
static inline void logMsg(const char* __restrict format, ...) {
    /* check level */
    if (format[0] == '<' &&
        format[2] == '>' &&
        '0' < format[1] &&
        format[1] <= DBG_MAX_LEVEL ) {
        /* have log level embedded in string */
        if (_traceLogLevel < format[1])
            return; // do not log this message
        format += 3;
    } else {
        // no log level provided, consider the default
        if (_traceLogLevel < DEFAULT_LOG_LEVEL)
            return;
    }
#if (__leon__)
    // clang does not support ___builtin_va_arg_pack_len functions
    if ( __builtin_va_arg_pack_len() == 0 ) {
        // no arguments. fast speed ahead
        SINK_FUNCTION(format);
    } else {
        char buffer[MV_DBG_FMT_STR_SIZE];
        int nBytes = __builtin_snprintf(buffer, MV_DBG_FMT_STR_SIZE, format, __builtin_va_arg_pack());
        if ( nBytes < 0){
            // error
            return;
        }
        SINK_FUNCTION(buffer);
    }
#else
    va_list ap;
    // TODO: for sink TraceProfile, use directly the trace prof internal buffer
    char buffer[MV_DBG_FMT_STR_SIZE];
    va_start(ap, format);
    int nbBytes = __builtin_vsnprintf(buffer, MV_DBG_FMT_STR_SIZE, format, ap);
    if ( nbBytes < 0){
        // error
        return;
    }
    va_end(ap);
#ifdef SYNC_LOG_MSG
    CRITICAL_SECTION_ENTER
#endif
    SINK_FUNCTION(buffer);
#ifdef SYNC_LOG_MSG
    CRITICAL_SECTION_EXIT
#endif
#endif
}


/**
 * @brief Log bulk data
 *
 * All bulk data will be dumped in a file on disk by moviProf when
 * an appropriate sink is used
 *
 * logBulk symol is weak in order for clients to be able to replace it with
 * their own implementation
 *
 * @param level log level of the data
 * @param data is the binary message to be logged
 * @param size data size
 */
__attribute__((weak, no_instrument_function))
void logBulk(const int level, void * __restrict data, size_t size) {
    if (_traceLogLevel < level) return;
    SINK_BULK(data, size);
}


#ifdef __cplusplus
}
#endif

#endif
