///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon file
///

// 1: Includes
// ----------------------------------------------------------------------------

#include "app_config.h"
#include <registersMyriad.h>
#include <DrvCpr.h>
#include <DrvDdr.h>
#include <DrvTimer.h>
#include <DrvRegUtils.h>
#include <DrvShaveL2Cache.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data 
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

int initClocksAndMemory(void)
{
    int i;

    DrvCprInitClocks(12000, 0, 0, 0, 0);

    DrvCprSysDeviceAction(UPA_DOMAIN, ASSERT_RESET,  -1);
    DrvCprSysDeviceAction(UPA_DOMAIN, DEASSERT_RESET,-1);

    DrvTimerInit();

    //Set Shave L2 cache partitions
    DrvShaveL2CacheSetupPartition(SHAVEPART256KB);

    //Allocate Shave L2 cache set partitions
    DrvShaveL2CacheAllocateSetPartitions();

    //Assign the one partition defined to all shaves
    for (i = 0; i < SHAVES_USED; i++)
    {
        DrvShaveL2CacheSetLSUPartId(i, PARTITION_0);
    }

    return 0;
}

