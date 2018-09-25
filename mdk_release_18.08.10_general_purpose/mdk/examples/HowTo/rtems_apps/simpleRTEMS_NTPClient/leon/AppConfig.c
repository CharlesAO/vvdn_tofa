///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    Application configuration Leon file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <OsDrvCpr.h>
#include "OsDrvTimer.h"
#include "DrvDdr.h"
#include <OsDrvShaveL2Cache.h>
#include "AppConfig.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define L2CACHE_CFG                 (SHAVE_L2CACHE_NORMAL_MODE)
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
	u32 sc; 
	OsDrvCprInit();
    OsDrvCprOpen();
    DrvDdrInitialise(NULL);

   // Set the shave L2 Cache mode
    sc = OsDrvShaveL2CacheInit(L2CACHE_CFG);
    if(sc)
       return sc;

    //Set Shave L2 cache partitions
    int partition_id;
    OsDrvShaveL2CGetPartition(SHAVEPART256KB, &partition_id);

    //Allocate Shave L2 cache set partitions
    sc = OsDrvShaveL2CacheAllocateSetPartitions();
    if(sc)
        return sc;

    OsDrvShaveL2CachePartitionInvalidate(0);
    return 0;
}

