///
/// @file      logIspL.c
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     
///            
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <rtems.h>
#include <assert.h>
#include <rtems/bspIo.h>
#include "logIspL.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#ifndef LOGISP_MAX_BUF_SIZE
#define LOGISP_MAX_BUF_SIZE (1024*16)
#endif
#define LOGISP_MAX_STRING_ZS 16
typedef struct {
    uint32_t idx;
    uint32_t info1;
    int64_t  ts;
    char  info2[LOGISP_MAX_STRING_ZS];
    uint32_t info3;
    int64_t  info4;
    uint32_t info5;
}LogIspT;


// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
volatile LogIspT  logIspB[LOGISP_MAX_BUF_SIZE] __attribute__((section(".ddr_direct.data")));
volatile uint32_t logIspI = 0;

void logIspAdd2(uint32_t info1, const char *info2) {
    uint32_t idx;
    int intLevel;
    struct timeval tv;
    int ret;
    rtems_interrupt_disable (intLevel);
    idx = logIspI; 
    logIspI++;
    if(LOGISP_MAX_BUF_SIZE == logIspI) {
        logIspI = 0;
        printk("logIspBufOverwrite\n");
    }
    ret = gettimeofday(&tv, NULL);
    rtems_interrupt_enable (intLevel);
    logIspB[idx].idx = idx;
    logIspB[idx].info1 = info1;
    strlcpy((char*)logIspB[idx].info2, info2, LOGISP_MAX_STRING_ZS);
    logIspB[idx].info3 = 0;
    assert(0==ret);
    logIspB[idx].ts = (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
}

void logIspAdd3(uint32_t info1, const char info2[4], uint32_t info3) {
    uint32_t idx;
    int intLevel;
    struct timeval tv;
    int ret;
    rtems_interrupt_disable (intLevel);
    idx = logIspI; 
    logIspI++;
    if(LOGISP_MAX_BUF_SIZE == logIspI) {
        logIspI = 0;
        printk("logIspBufOverwrite\n");
    }
    ret = gettimeofday(&tv, NULL);
    rtems_interrupt_enable (intLevel);
    logIspB[idx].idx = idx;
    logIspB[idx].info1 = info1;
    strlcpy((char*)logIspB[idx].info2, info2, LOGISP_MAX_STRING_ZS);
    logIspB[idx].info3 = info3; 
    logIspB[idx].info4 = 0;
    assert(0==ret);
    logIspB[idx].ts = (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
}

void logIspAdd4(uint32_t info1, const char info2[4], uint32_t info3, int64_t timeV) {
    uint32_t idx;
    int intLevel;
    struct timeval tv;
    int ret;
    rtems_interrupt_disable (intLevel);
    idx = logIspI; 
    logIspI++;
    if(LOGISP_MAX_BUF_SIZE == logIspI) {
        logIspI = 0;
        printk("logIspBufOverwrite\n");
    }
    ret = gettimeofday(&tv, NULL);
    rtems_interrupt_enable (intLevel);
    logIspB[idx].idx = idx;
    logIspB[idx].info1 = info1;
    strlcpy((char*)logIspB[idx].info2, info2, LOGISP_MAX_STRING_ZS);
    logIspB[idx].info3 = info3; 
    logIspB[idx].info4 = timeV; 
    
    assert(0==ret);
    logIspB[idx].ts = (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
}


#ifdef TASK_SWTCH_LOG_ENABLE
#include "CmdGen.h"
#include <rtems/bspIo.h>
#include <assert.h>
#include <rtems/score/objectimpl.h>
#include "pthread.h"

LogThreadSwitchT logThreadSwitch[MAX_LOG_THREAD_SWITCH_SIZE]
                                 __attribute__((section(".ddr_direct.data")));
int32_t __attribute__((section(".ddr_direct.data"))) logThreadSwitchIdx = 0;

static inline void extractThreadName (char *name, Thread_Control *heir) {
    Objects_Information *information;
    information  = _Objects_Get_information_id( heir->Object.id );
    if((heir->Join_queue.Queue.name) && heir->Join_queue.Queue.name[0]) {
        memcpy(name,
                heir->Join_queue.Queue.name, MAX_THREAD_NAME_ZS_LOC-1);
    }
    else {
        if ((information->is_string) && (heir->Object.name.name_p)) {
            memcpy(name,
                    heir->Object.name.name_p, MAX_THREAD_NAME_ZS_LOC-1);
        }
        else {
            char* ss = (char*)&heir->Object.name.name_u32;
            name[0] = ss[3];
            name[1] = ss[2];
            name[2] = ss[1];
            name[3] = ss[0];
            name[4] = 0;
        }
    }
    name[MAX_THREAD_NAME_ZS_LOC-1] = 0;
}

void addToThreadSwtchLogMsg(char* name, uint32_t thrId, int64_t  ts, int32_t  dts) {

    logThreadSwitchIdx++;
    int32_t idx = logThreadSwitchIdx-1;
    if(logThreadSwitchIdx == MAX_LOG_THREAD_SWITCH_SIZE) {
        logThreadSwitchIdx = 0;
        printk("Thread Log overflow. \n");
    }
    logThreadSwitch[idx].idx = idx;
    if(name) {
        memcpy(logThreadSwitch[idx].thrName, name, MAX_THREAD_NAME_ZS_LOC-1);
        logThreadSwitch[idx].thrName[MAX_THREAD_NAME_ZS_LOC-1] = 0;
    }
    else {
        logThreadSwitch[idx].thrName[0] = 0;
    }
    logThreadSwitch[idx].thrId = thrId;
    logThreadSwitch[idx].dts = dts;
    logThreadSwitch[idx].ts  = ts;
}

void my_thread_switch_extension(Thread_Control *executing, Thread_Control *heir) {
    logThreadSwitchIdx++;
    int32_t idx = logThreadSwitchIdx-1;
    if(logThreadSwitchIdx == MAX_LOG_THREAD_SWITCH_SIZE) {
        logThreadSwitchIdx = 0;
        printk("Thread Log overflow. \n");
    }
    static int64_t oldTs;
    int64_t ts = 0;
    getTimeStamps(&ts);

    pthread_t           thread = executing->Object.id;
    int  policy;
    struct sched_param param;
    pthread_getschedparam(thread, &policy, &param);
    logThreadSwitch[idx].idx = idx;
    extractThreadName(logThreadSwitch[idx].thrName, executing);
    logThreadSwitch[idx].thrId = executing->Object.id;
    logThreadSwitch[idx].dts = (int32_t)(ts-oldTs);
    logThreadSwitch[idx].ts  = ts;
    logThreadSwitch[idx].priority  = (policy << 16) | param.sched_priority;
    logThreadSwitch[idx].info1     = heir->Object.id;
    oldTs = ts;
 }
#endif

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

