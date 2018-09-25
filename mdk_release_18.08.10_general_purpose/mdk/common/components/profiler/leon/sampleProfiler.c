#ifdef SAMPLE_PROFILING

#define SAMPLE_PROFILER_INIT_AVAILABLE
// do not depend on tracing
#define INHIBIT_TRACE_PROFILER_CODE_GEN

///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// Myriad Sampling profiler
///

// System Includes
/// ----------------------------------------------------------------------------
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <bsp.h>
#include <stdint.h>
/// Application Includes
/// ----------------------------------------------------------------------------
#include "DrvTimer.h"
#include "DrvIcb.h"
#include "DrvRegUtilsDefines.h"
#include "DrvSvu.h"
#include "DrvSvuDefines.h"
#include <registersMyriad.h>
#include <swcLeonUtils.h>
#include "theDynContext.h"
#include "DrvDdr.h"

#ifdef __RTEMS__
#include "OsDrvCpr.h"
#include <rtems.h>
#if defined(MA2480) || defined(MA2485)
#include "OsDrvInit.h"
#include "OsDrvTimerInternal.h"
#else
#include "OsDrvTimer.h"
#endif
#endif

#include "profile.h"

// TODO: make a .h for this
void removeShaveFromList(uint32_t shave_nr);
void initSampleProfiler(uint32_t shave_nr);


/// Source Specific #defines and types  (typedef,enum,struct)
/// ----------------------------------------------------------------------------

#ifndef SAMPLE_PROFILER_DATA_SECTION
#define SAMPLE_PROFILER_DATA_SECTION __attribute__((section(".ddr_direct.bss")))
#endif

#ifndef SAMPLE_PERIOD_MICRO
#define SAMPLE_PERIOD_MICRO 10
#endif
#ifndef SAMPLE_DURATION_MS
#define SAMPLE_DURATION_MS 2000
#endif

#ifndef SP_TIMER_INTERRUPT_LEVEL
#define SP_TIMER_INTERRUPT_LEVEL 13
#endif

#define MAX_SAMPLE_COUNT (SAMPLE_DURATION_MS / SAMPLE_PERIOD_MICRO * 1000)
#define SAMPLE_BUFFER_SIZE (MAX_SAMPLE_COUNT)

#ifdef __RTEMS__
#define RTEMS_EVENT_STOP_TIMER  RTEMS_EVENT_1
#define RTEMS_EVENT_START_TIMER RTEMS_EVENT_2

typedef enum
{
    SAMPLE_STATUS_UNITIALIZED = 0,
    SAMPLE_STATUS_INITIALIZED = 1
} SAMPLE_STATUS;
#endif

#ifdef SAMPLE_PROF_COMPUTE_WINDOW
static uint32_t ShaveToLeonAddress(uint32_t shaveNumber,uint32_t address);
#define SHV_ADDR(shv, addr) (ShaveToLeonAddress(shv, addr))
#else
#define SHV_ADDR(shv, addr) (addr)
#endif

#if defined(MA2480) || defined(MA2485)
OsDrvTimerHandler timerHandler;
const uint32_t timerPriority = 11;
#endif

/// Static Local Data
/// ----------------------------------------------------------------------------
/* static uint8_t shave_val[TOTAL_NUM_SHAVES], shaveStartedCount = 0; */
static volatile uint32_t running_shaves = 0;
#define SHAVE_RUN(id) running_shaves |= (1 << (id))
/* __attribute__((no_instrument_function)) */
/* static inline void SHAVE_RUN(uint32_t id) { */
/*     running_shaves |= ((uint32_t)1 << (id)); */
/* } */
#define SHAVE_STOP(id) running_shaves &= ~(1 << (id))
/* __attribute__((no_instrument_function)) */
/* static inline void SHAVE_STOP(uint32_t id) { */
/*     running_shaves &= ~((uint32_t)1 << (id)); */
/* } */

#define FOREACH_RUNNING(svu_id) for (uint32_t r = running_shaves; r; r=r>>1, svu_id++) if (r & 0x1)
__attribute__((unused)) static int spTimerNo;

#ifdef __RTEMS__ //used only if exist rtems
#if !defined(MA2480) && !defined(MA2485)
static osDrvTimerHandler_t osSpTimerHandler;
#endif
static rtems_id taskSPTimer_id;
static rtems_id taskSPMain_id;
#endif


typedef struct  __attribute__ ((packed)) SampleProfileEntry_t {
    uint32_t ip;
    uint8_t shaveNumber;
} SampleProfileEntry_t;
#define PROFILE_ENTRY_DEFINED

SAMPLE_PROFILER_DATA_SECTION SampleProfileEntry_t newspBuffer___[SAMPLE_BUFFER_SIZE];
SAMPLE_PROFILER_DATA_SECTION static ProfileBufferEntry_t sProfile = {NULL, NULL, NULL};

/// Functions Implementation
/// ----------------------------------------------------------------------------
__attribute__((no_instrument_function))
static inline 
void timerStopOperation()
{
#ifdef __RTEMS__
    rtems_status_code status;
    status = rtems_event_send(taskSPTimer_id,RTEMS_EVENT_STOP_TIMER);
    if(status != RTEMS_SUCCESSFUL)

    {
        LOG_INFO("Error: rtems event send, status = %u\n",status);
    }
#else
    s32 status;
    status = DrvTimerStopOperation(spTimerNo);
    if(status!= MYR_DRV_SUCCESS)
    {
        LOG_INFO("Error: DrvTimerStopOperation, status = %ld\n",status);
    }
#endif
}

static void sendTimestamp(uint8_t shave_id) {
    uint32_t headerFlag = -1;
    uint8_t info_unused_ = 0;
    uint64_t timestamp = FP_TIME_READ();
    uint32_t lowTimestamp = (uint32_t)timestamp;
    uint32_t hiTimestamp = (timestamp >> 32);
    profilePushEntry(SampleProfileEntry_t, sProfile, headerFlag, shave_id);
    profilePushEntry(SampleProfileEntry_t, sProfile, lowTimestamp, info_unused_);
    profilePushEntry(SampleProfileEntry_t, sProfile, hiTimestamp, info_unused_);
}

// TODO: common/components/Debug/SvuDebugMyriad2/leon/SvuDebugMyriad2.c regarding shave window
__attribute__((no_instrument_function))
static uint32_t sampleProfilerTimerCb(uint32_t timerNumber, uint32_t param2)
{
    UNUSED(timerNumber);
    UNUSED(param2);
    uint32_t svu_id = 0;
    uint32_t processedShaves = 0;
    FOREACH_RUNNING(svu_id) {
        uint32_t ip = GET_REG_WORD_VAL(DCU_SVU_PTR(svu_id));
        if (!ip || DrvSvuSwiHalted(svu_id)) {
            removeShaveFromList(svu_id);
            continue;
        }
        ip = SHV_ADDR(svu_id, ip);
        profilePushEntry(SampleProfileEntry_t, sProfile, ip, svu_id); // shave start from 0, but core id is +2
        processedShaves++;
    }
    return 1 /*processedShaves*/;
}

#if defined(MA2480) || defined(MA2485)
__attribute__((no_instrument_function))
static bool _tmCbWrapper(void* ctx) {
    UNUSED(ctx);
    if (sampleProfilerTimerCb(0,0) == 0) {
        return false; // stop the timer
    }
    return true;
}

__attribute__((no_instrument_function))
static void allocate_timer() {
    uint32_t sysClk; // kHz
    OsDrvCprGetClockFrequency(OS_DRV_CPR_CLK_PLL0,&sysClk);
    uint32_t sampleTicks = (SAMPLE_PERIOD_MICRO*sysClk)/1000; // ticks per micro
    /* printf("tps = %lu\nOsDrvTimerAllocate(%p, %p, NULL, %lu, %lu)\n", */
    /*         sysClk, */
    /*         &timerHandler, _tmCbWrapper, timerPriority, sampleTicks); */
    rtems_status_code res = OsDrvTimerAllocate(&timerHandler, _tmCbWrapper, NULL, timerPriority, sampleTicks);
    assert(res == RTEMS_SUCCESSFUL);
}
#endif

#ifdef __RTEMS__
__attribute__((no_instrument_function))
static rtems_task SampleProfiler_task(rtems_task_argument unused)
{
    UNUSED(unused);
    static SAMPLE_STATUS sampleProfilerStatus = SAMPLE_STATUS_UNITIALIZED;
    rtems_event_set event;
    rtems_status_code rtemsStatusCode;
#if  !defined(MA2480) && !defined(MA2485)
    s32 intStatus;
#endif
    //only for the first call, init cpr, timer and DDR
    if(sampleProfilerStatus == SAMPLE_STATUS_UNITIALIZED)
    {
        sampleProfilerStatus = SAMPLE_STATUS_INITIALIZED;

#if defined(MA2480) || defined(MA2485)
        allocate_timer();
#else
        intStatus = OsDrvCprInit();
        if (intStatus != OS_MYR_DRV_SUCCESS)
        {
            LOG_INFO("Error: CPR init, status = %ld\n", intStatus);
        }

        intStatus = OsDrvCprOpen();
        if (intStatus != OS_MYR_DRV_SUCCESS)
        {
            LOG_INFO("Error: CPR open, status = %ld\n", intStatus);
        }

        intStatus = OsDrvTimerInit();
        if (intStatus != OS_MYR_DRV_SUCCESS)
        {
            LOG_INFO("Error: timer init, status = %ld\n", intStatus);
        }

        DrvDdrInitialise(NULL);
#endif
    }

    taskSPTimer_id = rtems_task_self();

#if  !defined(MA2480) && !defined(MA2485)
    intStatus = OsDrvTimerOpen(&osSpTimerHandler, 20000, OS_MYR_PROTECTION_SEM);
    if (intStatus != OS_MYR_DRV_SUCCESS)
    {
        LOG_INFO("Error: timer open status %ld\n", intStatus);
    }

    intStatus = OsDrvTimerStartTimer(&osSpTimerHandler, SAMPLE_PERIOD_MICRO,
                                  sampleProfilerTimerCb, REPEAT_FOREVER,
                                  SP_TIMER_INTERRUPT_LEVEL);

    if (intStatus != OS_MYR_DRV_SUCCESS)
    {
        LOG_INFO("Error: timer start status = %ld\n", intStatus);
    }
#else
    rtemsStatusCode = OsDrvTimerStart(&timerHandler);
    assert(rtemsStatusCode == RTEMS_SUCCESSFUL);
#endif
    //send event to inform that timer just start
    rtemsStatusCode = rtems_event_send(taskSPMain_id, RTEMS_EVENT_START_TIMER);
    if(rtemsStatusCode != RTEMS_SUCCESSFUL)
    {
        LOG_INFO("Error: rtems event send, status = %u\n", rtemsStatusCode);
    }

    //wait for signal to stop timer
    rtemsStatusCode = rtems_event_receive(
                                 RTEMS_EVENT_STOP_TIMER,
                                 RTEMS_WAIT,
                                 RTEMS_NO_TIMEOUT,
                                 &event
                                 );

    if(rtemsStatusCode != RTEMS_SUCCESSFUL)
    {
        LOG_INFO("Error: event receive, status = %u\n", rtemsStatusCode);
    }
#if  !defined(MA2480) && !defined(MA2485)
    intStatus = OsDrvTimerStopTimer(&osSpTimerHandler);
    if (intStatus != OS_MYR_DRV_SUCCESS)
    {
        LOG_INFO("Error: timer stop, status = %ld\n", intStatus);
    }

    intStatus = OsDrvTimerCloseTimer(&osSpTimerHandler);
    if (intStatus != OS_MYR_DRV_SUCCESS)
    {
        LOG_INFO("Error: timer close, status = %ld\n", intStatus);
    }

#else
    rtemsStatusCode = OsDrvTimerStop(&timerHandler);
    assert(rtemsStatusCode == RTEMS_SUCCESSFUL);
    /* rtemsStatusCode = OsDrvTimerDeallocate(&timerHandler); */
    /* assert(rtemsStatusCode == RTEMS_SUCCESSFUL); */
#endif
    rtemsStatusCode = rtems_task_delete(RTEMS_SELF);
    if(rtemsStatusCode != RTEMS_SUCCESSFUL)
    {
        LOG_INFO("Error: event receive, status = %u\n", rtemsStatusCode);
    }
}
#endif

__attribute__((no_instrument_function))
static inline
void timerStartOperation()
{
#ifdef __RTEMS__
    rtems_name task_name;
    rtems_status_code status;
    rtems_event_set event;

    // Create New Task
    task_name = rtems_build_name('S', 'A', 'M', 'P');

    // Task creation
    status = rtems_task_create(
                    task_name,
                    1, RTEMS_MINIMUM_STACK_SIZE * 2,
                    RTEMS_DEFAULT_MODES,
                    RTEMS_DEFAULT_ATTRIBUTES, &taskSPTimer_id);

    if(status != RTEMS_SUCCESSFUL)
    {
        LOG_INFO("Error: task create, status = %u\n",status);
    }

    taskSPMain_id = rtems_task_self();

    // Start Task
    status = rtems_task_start(taskSPTimer_id, SampleProfiler_task, 1);
    if(status != RTEMS_SUCCESSFUL)
    {
        LOG_INFO("Error: task start, status =  %u\n",status);
    }

    //wait for task to start timer
    status = rtems_event_receive(
                    RTEMS_EVENT_START_TIMER,
                    RTEMS_WAIT,
                    RTEMS_NO_TIMEOUT,
                    &event
    );

    if(status != RTEMS_SUCCESSFUL)
    {
        LOG_INFO("Error: event recevive, status =  %u\n",status);
    }
#else
    int status;
    // FIXME: time is actually SAMPLE_PERIOD_MICRO + ISR overhead
    status = DrvTimerStartOperation(SAMPLE_PERIOD_MICRO,
                           sampleProfilerTimerCb,
                           REPEAT_FOREVER,
                           SP_TIMER_INTERRUPT_LEVEL,
                           &spTimerNo);
    if (status != MYR_DRV_SUCCESS)
    {
        LOG_INFO("Error: DrvTimerStartOperation, status =  %u\n",status);
    }
#endif
}

#ifdef SAMPLE_PROF_COMPUTE_WINDOW
__attribute__((no_instrument_function))
static uint32_t ShaveToLeonAddress(uint32_t shaveNumber,uint32_t address)
{
    uint32_t window;
    uint32_t windowBase;
    uint32_t leonAddress;
    //Calculate address of the WindowA register
    uint32_t base = SHAVE_0_BASE_ADR + SLC_TOP_OFFSET_WIN_A + SVU_SLICE_OFFSET * shaveNumber;

    window = (address >> 24) & 0xFF;

    switch (window) {
    case 0x1c:
        windowBase  = GET_REG_WORD_VAL(base + 0x0);
        leonAddress = windowBase + (address & 0x00FFFFFF);
        break;
    case 0x1d:
        windowBase  = GET_REG_WORD_VAL(base + 0x4);
        leonAddress = windowBase + (address & 0x00FFFFFF);
        break;
    case 0x1e:
        windowBase  = GET_REG_WORD_VAL(base + 0x8);
        leonAddress = windowBase + (address & 0x00FFFFFF);
        break;
    case 0x1f:
        windowBase  = GET_REG_WORD_VAL(base + 0xC);
        leonAddress = windowBase + (address & 0x00FFFFFF);
        break;
    default:
        // If we aren't in a window the address isn't modified
        leonAddress = address;
        break;
    }
    return leonAddress;
}
#endif

/**
 * public functions
 */

/* #define one_bit_set(b) (b && !(b & (b-1))) */

__attribute__((no_instrument_function))
void initSampleProfiler(uint32_t shave_nr)
{
    /* printf("init %ld\n", shave_nr); */
    if (TOTAL_NUM_SHAVES <= shave_nr) {
        LOG_ERROR("Invalid id %ld\n", shave_nr);         
        return;
    }
	if (!__profileBufferMasterRecord__.sProf) {
        __profilerInit();
		sProfile.buffer = sProfile.head = newspBuffer___;
        sProfile.end = ((SampleProfileEntry_t*)sProfile.buffer) + SAMPLE_BUFFER_SIZE;
		__profileBufferMasterRecord__.sProf = &sProfile;
		__builtin_memset(newspBuffer___, 0, sizeof(newspBuffer___));
        enableProfiler(); // TODO: specify in an bitmask argument which profiler to enable (sample)
	}
    int startTimer = 0;
    CRITICAL_SECTION_ENTER
    if (!running_shaves) startTimer = 1;
    SHAVE_RUN(shave_nr);

    if (startTimer) {
        /* LOG_INFO("start on: %lx\n", running_shaves); */
        sendTimestamp(shave_nr);
        timerStartOperation();
    } 
    CRITICAL_SECTION_EXIT
    /* LOG_INFO("running shaves: %lx\n", running_shaves); */
}

__attribute__((no_instrument_function))
void removeShaveFromList(uint32_t shave_nr)
{
    if (TOTAL_NUM_SHAVES <= shave_nr) {
        LOG_ERROR("Invalid id %ld\n", shave_nr);         
        return;
    }
    if (!running_shaves) {
        // this is happening alot since we check from the ISR cb if shave is
        // finished
        /* LOG_ERROR("Already cleared!\n"); */
        return;
    }
    SHAVE_STOP(shave_nr);
    if(!running_shaves)
    {
        /* printf("stopping timer\n"); */
        timerStopOperation();
    }
}

#endif
