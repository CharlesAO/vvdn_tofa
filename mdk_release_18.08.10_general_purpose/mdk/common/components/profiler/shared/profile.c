#include <stdlib.h> // atexit

#include "profile.h"

#include <swcWhoAmI.h>
#ifndef __RTEMS__
#include "DrvCpr.h"
#else
#include "OsDrvCpr.h"
#endif

volatile ProfileBufferMasterRecord_t __profileBufferMasterRecord__
//__attribute__((weak))
__attribute__((section(".ddr_direct.data")))
= {
    .magic=0x666f7270,
    .version=MV_PROF_VERSION,
    .fProf=0,
    .sProf=0,
    .tProf=0,
    .pProf=0,
    .coreId=-1, // during init filled with swcWhoAmI()
    .clockSpeed=-1,
    .myriadChipVersion=
#if defined(MA2450)
    "MA2450",
#elif defined(MA2150)
    "MA2150",
#elif defined(MA2155)
    "MA2155",
#elif defined(MA2455)
    "MA2455",
#elif defined(MA2480)
    "MA2480",
#else
    "unknown",
#endif
    .overhead=0
};

uint64_t oldTimestamp = 0;

int __profileEnable = 0;

int bufferCollected = 0;
void __attribute__((no_instrument_function))
collectProfileData(ProfileBufferEntry_t* pProf) {
    // the debugger set the value of bufferCollected to 1 when finished, afterwards the execution is resumed
    // if is 0, it may be that something went wrong. do not flush or change
    if (!bufferCollected) return;

    // const char* todo = __func__;
    // todo = __PRETTY_FUNCTION__;
    // TODO: double buffering for live session
    // init buffers
    pProf->head = pProf->buffer;
    __builtin_memset(pProf->head, 0, ((char*)pProf->end - (char*)pProf->buffer));

    bufferCollected = 0;
}

// ###andoniu###: tmp
#include <stdio.h>

static void
__attribute__((no_instrument_function))
__attribute__((noinline))
__profilerEnd() {
    int32_t freq = 0;
    disableProfiler();
#ifndef SHAVE_FPROFILE
#ifdef __RTEMS__
#if defined(MA2480) || defined(MA2485)
    OsDrvCprGetClockFrequency(OS_DRV_CPR_CLK_PLL0,(uint32_t*)&freq);
    int clks = rtems_clock_get_ticks_per_second();
    printf("RTEMS freq = %d\n", clks);
#else
    OsDrvCprGetClockFreqKhz(SYS_CLK, NULL, &freq);
#endif
#else
    freq = DrvCprGetClockFreqKhz(SYS_CLK, NULL);
#endif
#endif
    __profileBufferMasterRecord__.clockSpeed = freq;
}

void
__attribute__((no_instrument_function))
__attribute__((noinline))
__profilerInit() {
    if ( __profileBufferMasterRecord__.coreId != 0xFF )
        return;
    __profileBufferMasterRecord__.coreId = swcWhoAmI();
    __profileBufferMasterRecord__.startTime = oldTimestamp = 0;

    atexit(__profilerEnd);
}
