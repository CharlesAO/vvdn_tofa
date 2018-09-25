///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains basic declarations for the SIPP Performance TestBed
///

#ifndef __PERF_TESTBED_H__
#define __PERF_TESTBED_H__

/////////////////////////////////////////////////////////////////////////////////
//  Headers
/////////////////////////////////////////////////////////////////////////////////
#include "sipp.h"
#include "perfTestbedTypes.h"
#include "perfTestbedCfg.h"

///////////////////////////////////////////////////////////////////////////////
// Macros
#define PERF_TB_DUMP_LINE(s_, f_, ...)  { \
                                            u32 u32NbChars = sprintf ((s_), (f_), ## __VA_ARGS__);  \
                                            perfTestbedAddToFile (&u32NbChars, s_); \
                                        }

///////////////////////////////////////////////////////////////////////////////
// Utility APIs

void perfTestbedSetFileBuffer (u8 * pFileBuffer);
void perfTestbedAddToFile (u32 * pu32NbChars, char * pLineBuffer);
void perfTestbedDumpFile (char * fileDumpName);

///////////////////////////////////////////////////////////////////////////////
// Globals
extern PerfTestbedPipe perfTestbedPipeList[PERF_TESTBED_NUM_PIPELINES];

extern u32 sliceFirst[PERF_TESTBED_NUM_PIPELINES];
extern u32 sliceLast[PERF_TESTBED_NUM_PIPELINES];


#endif // __PERF_TESTBED_H__

// End of file
