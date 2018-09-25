///
/// @file      logIspL.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Contains all interaction part with Ipipe.
///            This file describes all allowed message interaction with Lrt Pipes side.
///

#ifndef _logIspL_H_
#define _logIspL_H_
#ifdef __cplusplus
extern "C" {
#endif
/**************************************************************************************************
 ~~~ Includes
 **************************************************************************************************/
#include "stdint.h"
/**************************************************************************************************
 ~~~  Exported Functions
 **************************************************************************************************/
void logIspAdd2(uint32_t info1, const char info2[4]);
void logIspAdd3(uint32_t info1, const char info2[4], uint32_t info3);
void logIspAdd4(uint32_t info1, const char info2[4], uint32_t info3, int64_t timeV);

#ifdef LOGISPL_ENABLE
#define logIspAdd(X, Y, Z) logIspAdd3(X, Y, Z)
#define logIspAddFull(X, Y, Z, W) logIspAdd4(X, Y, Z, W)
#else
#define logIspAdd(X, Y, Z) 
#define logIspAddFull(X, Y, Z, W)
#endif

#ifdef TASK_SWTCH_LOG_ENABLE
#define MAX_LOG_THREAD_SWITCH_SIZE (1024*48)
#define MAX_THREAD_NAME_ZS_LOC 16
typedef struct LogThreadSwitchS {
    uint32_t idx;
    uint32_t thrId;
    char     thrName[MAX_THREAD_NAME_ZS_LOC];
    int64_t  ts;
    int32_t  dts;
    uint32_t priority;
    uint32_t info1;
    uint32_t info2;
}LogThreadSwitchT;
extern void my_thread_switch_extension(Thread_Control *executing, Thread_Control *heir);
extern void addToThreadSwtchLogMsg(char* name, uint32_t thrId, int64_t  ts, int32_t  dts);
#endif


#ifdef __cplusplus
}
#endif
#endif //_logIspL_H_
