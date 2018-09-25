///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Simple Example showing sample profiler usage.
///            It can be used for testing purposes, too
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include "DrvTimer.h"
#include "DrvSvu.h"
#include "app_config.h"
#include <DrvLeon.h>

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define SHAVE_4         4
#define SHAVE_12        12
#define SHAVES_NUMBER   8

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------


extern u32  spExample4_profile[];
extern u32  spExample5_profile[];
extern u32  spExample6_profile[];
extern u32  spExample7_profile[];
extern u32  spExample8_profile[];
extern u32  spExample9_profile[];
extern u32  spExample10_profile[];
extern u32  spExample11_profile[];

u32* shavesEntry[SHAVES_NUMBER] =
{
    spExample4_profile,
    spExample5_profile,
    spExample6_profile,
    spExample7_profile,
    spExample8_profile,
    spExample9_profile,
    spExample10_profile,
    spExample11_profile
};

// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------
int main(void)
{
    u32 shaveIndex;

    //sample profiler use cpr, timers and DDR, and should be initialized on application side
    initClocksAndMemory();
    DrvTimerInit();

    printf("LRT started..\n");

    for (shaveIndex = SHAVE_4; shaveIndex < SHAVE_12; shaveIndex++)
    {
        printf("Starting Shave %lu..\n", shaveIndex);
        swcResetShave(shaveIndex);
        swcSetAbsoluteDefaultStack(shaveIndex);
        // The function swcStartShave is overridden by the profiler.
        // It will start the profiling automatically whenever this (or any other startShave) is called
        swcStartShave(shaveIndex,(u32)shavesEntry[shaveIndex-SHAVE_4]);
        // The function is overridden by the profiler.
        // It will stop the measurement when this function is called. If the shave was started in async mode,
        // the profiler will also stop the measurement when the shave interrupt arrives.
        swcWaitShave(shaveIndex);
        printf("Shave %lu finished.\n", shaveIndex);
    }

    DrvLeonRTSignalStop();
    return 0;
}
