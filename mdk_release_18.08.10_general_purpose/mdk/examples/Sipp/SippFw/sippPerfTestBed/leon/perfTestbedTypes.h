///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains basic declarations for the SIPP concurrency application
///            app layer types
///

#ifndef __PERF_TESTBED_TYPES_H__
#define __PERF_TESTBED_TYPES_H__

/////////////////////////////////////////////////////////////////////////////////
//  Headers
/////////////////////////////////////////////////////////////////////////////////

#include "sipp.h"
#ifndef SIPP_PC
#include "brdMv0198.h"
#endif

/////////////////////////////////////////////////////////////////////////////////
//  Macros
/////////////////////////////////////////////////////////////////////////////////
#define PERF_TESTBED_EVENT_HISTORY_SIZE 0x400
#define PERF_TESTBED_EVENT_PIPE_HISTORY_SIZE 0x8

/////////////////////////////////////////////////////////////////////////////////
//  Types
/////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// Pipeline state enum

typedef enum
{
    PERF_TB_PIPE_STATE_UNINIT            = 0x00,
    PERF_TB_PIPE_STATE_IDLE              = 0x01,
    PERF_TB_PIPE_STATE_RUNNING           = 0x02,
    PERF_TB_PIPE_STATE_COMPLETE          = 0x03

} PERF_TB_PIPE_STATE;

//////////////////////////////////////////////////////////////
// Pipeline state enum

typedef enum
{
    PERF_TB_PIPE_EVENT_NONE              = 0x00,
    PERF_TB_PIPE_EVENT_FRAME_DONE        = 0x01,
    PERF_TB_PIPE_EVENT_FRAME_STARTED     = 0x02,
    PERF_TB_PIPE_POWER_MEAS_STARTED      = 0x03,
    PERF_TB_PIPE_POWER_MEAS_DONE         = 0x04

} PERF_TB_PIPE_EVENT;


//////////////////////////////////////////////////////////////
// Perf Testbed pipeline definition

typedef struct
{
    PERF_TB_PIPE_STATE eState;
    SippPipeline *     sippFwHandle;

    SippFilter * dmaIn;
    SippFilter * dmaOut;
    SippFilter * HWFilter;
    SippFilter * SWFilter;

    u32 framesComplete;

    PERF_TB_PIPE_EVENT eventList[PERF_TESTBED_EVENT_PIPE_HISTORY_SIZE];
    u32                eventListWrPtr;
    u32                eventListRdPtr;

} PerfTestbedPipe, * pPerfTestbedPipe;


//////////////////////////////////////////////////////////////
// App event data type

typedef struct
{
    u8                 pipeIdx;
    PERF_TB_PIPE_EVENT event;
    u64                eventData;

} PerfTestEvent;

#ifndef SIPP_PC
typedef struct
{
    tyAdcResultAllRails power;
    fp32 avgmW;
    fp32 avgmA;

} PerfTestPowerStat;

typedef struct
{
    fp32 temperature;
    fp32 CoremW;
    fp32 DDRmW;

} PerfTestPowerSnapShot;

#endif
#endif // __PERF_TESTBED_TYPES_H__
