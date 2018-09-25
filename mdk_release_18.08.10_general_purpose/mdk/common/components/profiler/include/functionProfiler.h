/*
 * functionProfiler.h
 */

#ifndef COMPONENTS_PROFILER_INCLUDE_PROFILE_H_
#define COMPONENTS_PROFILER_INCLUDE_PROFILE_H_

#include <stdint.h>

#include <DrvRegUtils.h>
#include <registersMyriad.h>

#include "profile.h"

// FIXME: FP_TIME_READ()
#ifndef FP_TIME_READ
#define TIMER_ADDR TIM0_BASE_ADR
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


typedef enum FunctionProfileEntryType
{
    HOOK_FUNC_ENTRY = 0x1, /*function entry hook*/
    HOOK_FUNC_EXIT,         /*function exit hook*/
    HOOK_TASK_SWITCH,        /*task entry hook*/
    HOOK_ISR_ENTRY,         /*ISR entry hook*/
    HOOK_ISR_EXIT,           /*ISR exit  hook*/
    HOOK_MEM_VALUE          /*used to log 64 bit value variables, registers, etc*/
} FunctionProfileEntryType;

typedef struct  __attribute__ ((packed)) FunctionProfileEntry_t {
    uint32_t addr_func;
    uint32_t context;
    uint64_t timestamp;
} FunctionProfileEntry_t;


#ifdef __cplusplus
extern "C" {
#endif

void __functionProfilerInit();
void __computeOverhead();
extern FunctionProfileEntry_t __fprofInternalBuffer[FPROFILER_BUFFER_SIZE/sizeof(FunctionProfileEntry_t)];
extern ProfileBufferEntry_t fProfile;

extern uint64_t oldTimestamp;

#ifdef FP_32BIT_TIMESTAMP_OPT_MODE

#define FP_TIME_READ() ( { uint64_t prevTimestamp = oldTimestamp; \
                           oldTimestamp = FP_TIME_READ64(); \
                           if ( unlikely(__profileBufferMasterRecord__.startTime == 0) ){ __profileBufferMasterRecord__.startTime = prevTimestamp = oldTimestamp; } \
                           (uint32_t)(oldTimestamp-prevTimestamp); \
                        })
#endif

/*
======================================
FunctionProfileEntry_t structure memory layout
======================================

+===========+=========+===========+
| addr_func | context | timestamp |
+===========+=========+===========+
| 32 bit    | 32 bit  | 64 bit    |
+-----------+---------+-----------+


context field (32 bit)
+============+===========+===========+============+
| Thread API | Thread ID | Reserved  | Entry Type |
+============+===========+===========+============+
|   31 .. 29 |  28 .. 25 |  24 .. 4  |     3 .. 0 |
+------------+-----------+-----------+------------+
If is ISR context, then Thread ID is the interrupt
source (ICB.SRC) and thread API is 111 (0x7)
*/

#define IN(func) profilePushEntry(FunctionProfileEntry_t, fProfile, \
    (uint32_t)func, \
    HOOK_FUNC_ENTRY, \
    FP_TIME_READ())
#define OUT(func) profilePushEntry(FunctionProfileEntry_t, fProfile, \
    (uint32_t)func, \
    HOOK_FUNC_EXIT, \
    FP_TIME_READ())

/*
no need to send the tid for every function entry/exit since the CTX_SWITCH will do the job
just fine
TODO:
f_addr 32 bit aligned (lat 2 bits always 0 - use the location to save type)
f entry: f_addr | 0x1, timestamp
f exit : f_addr | 0x2, timestamp
ctx sw : (old/new ctx) << 2 | 0x3, timestamp
memory : (addr & ~0x3), timestamp
*/
// Since one can use more API in one app, TID contains also API: 15:13 is API while 12:0 is index
#define RTEMS_IN(func, tid) profilePushEntry(FunctionProfileEntry_t, fProfile, \
    (uint32_t)func, \
    ((tid&0x07000000)<<5) | ((tid&0x1FFF)<<16) | HOOK_FUNC_ENTRY, \
    FP_TIME_READ())
#define RTEMS_OUT(func, tid) profilePushEntry(FunctionProfileEntry_t, fProfile, \
    (uint32_t)func, \
    ((tid&0x07000000)<<5) | ((tid&0x1FFF)<<16) | HOOK_FUNC_EXIT, \
    FP_TIME_READ())
#define RTEMS_SWITCH(executing, heir) \
    profilePushEntry(FunctionProfileEntry_t, fProfile, \
        ((((executing&0x07000000)>>11)&0xe000) | (executing&0x1fff)) & 0xffff,\
                    ((heir&0x07000000)<<5) | ((heir&0x1FFF)<<16) | HOOK_TASK_SWITCH,\
                    FP_TIME_READ());


// thread switch or irq enter/irq exit
// FIXME: the context is a thread id, or a irq number, must be already shifted 
#define CTX_SWITCH(oldCtx, newCtx) profilePushEntry(FunctionProfileEntry_t, fProfile, \
    oldCtx, \
    newCtx | HOOK_TASK_SWITCH, \
    FP_TIME_READ())

// TODO: optimize memory. compact data
#define PUSH_MEM(address, memValue) profilePushEntry(FunctionProfileEntry_t, fProfile, \
    (uint32_t)address, HOOK_MEM_VALUE, memValue)


#ifdef __cplusplus
} // extern "C"
#endif

#endif /* COMPONENTS_PROFILER_INCLUDE_PROFILE_H_ */
