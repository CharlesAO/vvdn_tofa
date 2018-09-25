///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mv_types.h>
#include <DrvCpr.h>
#include <swcShaveLoader.h>
#include "app_config.h"
#include <registersMyriad.h>
#include <DrvSvu.h>
#include <DrvTimer.h>
#include <swcTestUtils.h>
#include <swcLeonUtils.h>
#ifdef MYRIAD2
#include <DrvLeonL2C.h>
#include <DrvShaveL2Cache.h>
#endif

typedef enum
{
    FINISH = -2,
    SHAVE_ALL = -1,
    SHAVE0 = 0,
    SHAVE1,
    SHAVE2,
    SHAVE3,
    SHAVE4,
    SHAVE5,
    SHAVE6,
    SHAVE7,
    SHAVE8,
    SHAVE9,
    SHAVE10,
    SHAVE11,
    LEON_OS,
    LEON_RT,
    LEON = LEON_OS
} TargetToRun;

// Modify the SHAVE entry point function
#define kernelAppSV_EntryPoint dummy0_main
extern void*  (kernelAppSV_EntryPoint);

// Variables used for the PC app interaction
u32 shavesEntryPoint[SHAVES_USED] =
{
    (u32)&kernelAppSV_EntryPoint, (u32)&kernelAppSV_EntryPoint, (u32)&kernelAppSV_EntryPoint, (u32)&kernelAppSV_EntryPoint,
    (u32)&kernelAppSV_EntryPoint, (u32)&kernelAppSV_EntryPoint, (u32)&kernelAppSV_EntryPoint, (u32)&kernelAppSV_EntryPoint,
    (u32)&kernelAppSV_EntryPoint, (u32)&kernelAppSV_EntryPoint, (u32)&kernelAppSV_EntryPoint, (u32)&kernelAppSV_EntryPoint,
};

u32 firstShave = 0;
u32 lastShave = 0;
u32 done = 0;
u32 runOnce = -1;
u32 cycleCount;
u32 breakpointCnt;
TargetToRun target = LEON_OS;

void (*leonEntryPoint)();
//--------------------------------------------------------

performanceStruct perfStr[SHAVES_USED];

INIT_END_OF_CODE_AREEA;

void runLeon()
{
    if (leonEntryPoint != NULL)
        leonEntryPoint();
}

void breakpoint()
{
    breakpointCnt++;
}

void startTest()
{
}

void runShaves()
{
    u32 i;

    // Start Shaves
    for (i = firstShave; i <= lastShave; i++)
    {
        swcResetShave(i);
        swcSetAbsoluteDefaultStack(i);
        swcShaveProfStartGathering(i, &perfStr[i]);
        swcStartShave(i, shavesEntryPoint[i]);
    }

    // Wait for Shaves to stop
    while (swcShavesRunning(firstShave, lastShave))
       ;

#ifdef INT_DDR
    DrvShaveL2CachePartitionFlushAndInvalidate(PARTITION_0);
#endif

    // Gather profiling data
    for (i = firstShave; i <= lastShave; i++)
    {
        swcShaveProfStopGathering(i, &perfStr[i]);

        cycleCount += perfStr[i].perfCounterExec;
    }

    // Average cycles across all used Shaves
    cycleCount /= (lastShave - firstShave + 1);
}

int main(void)
{
    // Initialization only run once per test
	initClocksAndMemory();

#ifndef MYRIAD2
    swcLeonDisableDcache();
#endif

    // Test app control loop
    while (done == 0)
    {
        // Choose what to do next
        swcLeonDataCacheFlush(); 
        switch (target)
        {
            case LEON_OS: runLeon(); break;
            case SHAVE_ALL: runShaves(); break;
            case FINISH: done = 1; break;
            default: printf("PC app chose a wrong target (%d). Exiting...\n", target);
                done = 1;
        }

        if (runOnce == 1)
            done = 1;

        // Stop the Myriad app and let the PC app take control
        breakpoint();
    }

    AUTOCHECK_ELF_FILE;

    return 0;
}
