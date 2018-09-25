#if defined(LRT_PROFILE) || defined(LOS_PROFILE) || defined(SHAVE_FPROFILE)
// Inform the user during the build this is a profiled elf thus the performance impact
#if defined(__clang__)
#pragma clang diagnostic warning "-W#warnings"
#warning Function profiling is activated - expect performance degradation ...
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-Wcpp"
#warning Function profiling is activated - expect performance degradation ...
#pragma GCC diagnostic pop
#endif
#include "functionProfiler.h"

#ifdef __RTEMS__
#include <rtems/score/sysstate.h>
#endif

FunctionProfileEntry_t __fprofInternalBuffer[FPROFILER_BUFFER_SIZE/sizeof(FunctionProfileEntry_t)] FPROFILER_DATA_SECTION;

ProfileBufferEntry_t fProfile FPROFILER_DATA_SECTION;

void
__attribute__((no_instrument_function))
__attribute__((noinline))
__functionProfilerInit() {
    __profilerInit();
    __profileBufferMasterRecord__.fProf = &fProfile;
    fProfile.buffer = fProfile.head = __fprofInternalBuffer;
    fProfile.end = &__fprofInternalBuffer[FPROFILER_BUFFER_SIZE/sizeof(FunctionProfileEntry_t)];
    __builtin_memset(fProfile.buffer, 0, sizeof(__fprofInternalBuffer));
}

// overhead computation
#define noopt() asm volatile("" : : : "memory")
// measure overhead clocks. this can be influenced by dynamic frequency scaling (if the case)
void
__attribute__ ((noinline))
__instrumentedFunction(void) {
    noopt();
}
void
__attribute__ ((noinline))
__attribute__((no_instrument_function))
__notInstrumentedFunction(void) {
    noopt();
}


/**
 * Self measure hooks overhead
 */
void
__attribute__((no_instrument_function))
__computeOverhead() {
   volatile uint64_t ti, tf, e1, x1, e2, x2, cret, rwvar;
/*
 * -> ti (time in)
 * instrumentedDummyFunction() {
 * -> e1 (entry 1)
 * -> x1 (exit 1)
 * }
 * instrumentedDummyFunction() {
 * -> e2 (entry 2)
 * -> x2 (exit 2)
 * }
 * -> tf (time final)
 *
 */
#ifdef __RTEMS__
   System_state_Codes prevState = _System_state_Current;
   _System_state_Current = SYSTEM_STATE_UP;
#endif
    enableProfiler();
    noopt();
    ti = FP_TIME_READ();
    noopt();
    __instrumentedFunction();
    noopt();
    __instrumentedFunction();
    noopt();
    tf = FP_TIME_READ();
    noopt();
    disableProfiler();
#ifdef FP_32BIT_TIMESTAMP_OPT_MODE
    e1 = __fprofInternalBuffer[0].DeltaTimestamp + __profileBufferMasterRecord__.startTime;
    x1 = __fprofInternalBuffer[1].DeltaTimestamp;
    e2 = __fprofInternalBuffer[2].DeltaTimestamp;
    x2 = __fprofInternalBuffer[3].DeltaTimestamp;
#else
    e1 = __fprofInternalBuffer[0].timestamp;
    x1 = __fprofInternalBuffer[1].timestamp;
    e2 = __fprofInternalBuffer[2].timestamp;
    x2 = __fprofInternalBuffer[3].timestamp;
#endif
    // compute call-return overhead
    noopt();
    cret = FP_TIME_READ();
    __notInstrumentedFunction();
    noopt();
    cret = FP_TIME_READ() - cret;
    noopt();
    rwvar = FP_TIME_READ();
    noopt();
    rwvar = FP_TIME_READ() - rwvar;
    noopt();

    // the compiler will make the following expression shorter. we shall keep it expanded just to be a bit more clear
    __profileBufferMasterRecord__.overhead = ((((e2-x1)-(e1-ti)) + ((e2-x1)-(tf-x2)) + ((x2-e2+x1-e1)/2) - cret))/2;
#ifdef __RTEMS__
    _System_state_Current = prevState;
#endif

}
#endif
