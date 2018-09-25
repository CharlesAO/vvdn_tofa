///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Macros and types used to configure the performance test

/////////////////////////////////////////////////////
// Test specific macros
/////////////////////////////////////////////////////

#ifndef _PERF_TESTBED_CFG_H_
#define _PERF_TESTBED_CFG_H_

// *** Choose a test configuration ***
// See MDK-mz2x5x-SippUSerGuide for test details...

//#define SIPP_PERF_TEST_CFG_A
//#define SIPP_PERF_TEST_CFG_B
//#define SIPP_PERF_TEST_CFG_C
#define SIPP_PERF_TEST_CFG_D

#define MAX_LINE_LENGTH  100
#define FILE_DUMP_BUFFER_MAX_SIZE    50 * 1024

#ifdef SIPP_PERF_TEST_CFG_A
#include "perfTestbedCfg_A.h"
#elif defined SIPP_PERF_TEST_CFG_B
#include "perfTestbedCfg_B.h"
#elif defined SIPP_PERF_TEST_CFG_C
#include "perfTestbedCfg_C.h"
#elif defined SIPP_PERF_TEST_CFG_D
#include "perfTestbedCfg_D.h"
#endif

#endif // _PERF_TESTBED_CFG_H_
