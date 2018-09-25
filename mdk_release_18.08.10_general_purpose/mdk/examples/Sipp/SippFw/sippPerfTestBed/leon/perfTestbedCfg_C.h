///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Macros and types used to configure the performance test

/////////////////////////////////////////////////////
// Test specific macros
/////////////////////////////////////////////////////

#ifndef _PERF_TESTBED_CFG_C_H_
#define _PERF_TESTBED_CFG_C_H_

#define SIPP_RUN_ASYNC
//#define DISABLE_OPIPE_CONNECTIONS
//#define DISABLE_SIPP_SW_QU
#define SIPP_RT_STATS_PRINT_CTRL         0


#ifndef SIPP_RUN_ASYNC
#define PERF_TESTBED_NUM_PIPELINES       0x1   // DO NOT CHANGE - Only one pipe may run at any time in sync mode
#else
#define PERF_TESTBED_NUM_PIPELINES       0x2
#endif

#define PERF_TESTBED_FRAMES_PER_PIPE     0x10

// Pipeline 0 Setup
#define PERF_TESTBED_PIPE0_WIDTH          1280
#define PERF_TESTBED_PIPE0_HEIGHT          720
#define PERF_TESTBED_PIPE0_NUM_PLANES        1
#define PERF_TESTBED_PIPE0_LINES_PER_ITER    1
typedef u8  t_Pipe0;

// Pipeline 1 Setup
#define PERF_TESTBED_PIPE1_WIDTH          1280
#define PERF_TESTBED_PIPE1_HEIGHT          720
#define PERF_TESTBED_PIPE1_NUM_PLANES        1
#define PERF_TESTBED_PIPE1_LINES_PER_ITER    1
typedef u8  t_Pipe1;

// Pipeline 2 Setup
#define PERF_TESTBED_PIPE2_WIDTH          1280
#define PERF_TESTBED_PIPE2_HEIGHT          720
#define PERF_TESTBED_PIPE2_NUM_PLANES        1
#define PERF_TESTBED_PIPE2_LINES_PER_ITER    4
typedef u8  t_Pipe2;

// Pipeline 3 Setup
#define PERF_TESTBED_PIPE3_WIDTH          1280
#define PERF_TESTBED_PIPE3_HEIGHT          720
#define PERF_TESTBED_PIPE3_NUM_PLANES        1
#define PERF_TESTBED_PIPE3_LINES_PER_ITER    4
typedef u8  t_Pipe3;

#endif // _PERF_TESTBED_CFG_C_H_
