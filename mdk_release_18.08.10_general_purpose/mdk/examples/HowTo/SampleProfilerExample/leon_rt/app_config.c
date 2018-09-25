///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <DrvCpr.h>
#include <DrvDdr.h>
#include <DrvShaveL2Cache.h>
#include "app_config.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define L2CACHE_CFG                (SHAVE_L2CACHE_NORMAL_MODE)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// Sections decoration is required here for downstream tools

// 4: Static Local Data 
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
int initClocksAndMemory(void)
{   
    s32 sc;
    DrvCprSysDeviceAction(UPA_DOMAIN, ASSERT_RESET,  -1);
    DrvCprSysDeviceAction(UPA_DOMAIN, DEASSERT_RESET,-1);
    DrvCprInit();

    DrvCprInitClocks(12000, 0, 0, 0, 0);

    //DrvDdrInitialise(NULL); - DDR is initialized on leon os.
    //If leon os is missing and only leon rt part exist,
    //DDR should be initialized here, because sample profiler use DDR

    // Set the shave L2 Cache mode
    sc = DrvShaveL2CacheSetMode(L2CACHE_CFG);
    if(sc)
        return sc;

    //Set Shave L2 cache partitions
    DrvShaveL2CacheSetupPartition(SHAVEPART256KB);

    //Allocate Shave L2 cache set partitions
    sc = DrvShaveL2CacheAllocateSetPartitions();
    if(sc)
        return sc;
    
    //Assign the one partition defined to all shaves
    for (int i = 0; i < SHAVES_USED; i++)
    {
        DrvShaveL2CacheSetLSUPartId(i, PARTITION_0);
    }

    DrvShaveL2CachePartitionInvalidate(PARTITION_0);
    return 0;
}

