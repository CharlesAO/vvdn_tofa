///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Main Leon CSS file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include <rtems.h>
#include <rtems/bspIo.h>
#include <OsDrvSvu.h>
#include <UnitTestApi.h>
#include "DrvLeon.h"
#include "OsDrvShaveL2Cache.h"
#include "SharingDataWithShave.h"

#include "rtems_config.h"
#include "app_config.h"
// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
//choose one unused mutex for the printf message

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
u32 CMX_DIRECT_DATA expectedBufferShaveLeon[SMALL_DATA_LENGTH];
u32 CMX_DIRECT_DATA expectedBufferBetweenShaves[SMALL_DATA_LENGTH];
extern u32 CMX_DIRECT_DATA lrt_losToLrt;

// 4: Static Local Data
// ----------------------------------------------------------------------------

__attribute__((aligned(8))) int l2cDataPartitionId;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

void shareSmallDataWithLrt(void)
{
    DrvLeonRTStartupStart();
    lrt_losToLrt = VALUE_SET_IN_LOS;

    DrvLeonRTWaitExecution();

    printf("Share data between leons\n");
    unitTestCompare(lrt_losToLrt, VALUE_SET_IN_LRT);
}

void shareSmallData(void)
{
    LeonShareSmallDataWithShave(expectedBufferShaveLeon, 
                                expectedBufferBetweenShaves);
    shareSmallDataWithLrt();
}

void shareBigData(void)
{
    LeonShareBigDataWithShave();
}

void myriadCaches()
{
    shareSmallData();
    shareBigData();
}

void *POSIX_Init(void *args)
{
    int idx;
    UNUSED(args);

    unitTestInit();
    // swcLeonDisableCaches();

    int status = initClocksAndMemory(&l2cDataPartitionId);
    if (status != OS_MYR_DRV_SUCCESS)
    {
        printf("Error initializing clocks and memory.\n");
        exit(1);
    }

    for(idx = 0; idx< SMALL_DATA_LENGTH; idx++)
        expectedBufferShaveLeon[idx] = 2*idx;

    for(idx = 0; idx< SMALL_DATA_LENGTH; idx++)
        expectedBufferBetweenShaves[idx] = idx;

    myriadCaches();
    unitTestFinalReport();
    exit(0);
}



