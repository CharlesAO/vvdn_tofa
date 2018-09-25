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
#include <OsDrvShaveL2Cache.h>
#include "app_config.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define L2CACHE_CFG                (SHAVE_L2CACHE_NORMAL_MODE)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
// Setup all the clock configurations needed by this application and also the ddr
int initClocksAndMemory(void)
{
    u32 sc;
    tyAuxClkDividerCfg auxClkAllOn[] =
    {
        {AUX_CLK_MASK_UART, CLK_SRC_REFCLK0, 96, 625},   // Give the UART an SCLK that allows it to generate an output baud rate of of 115200 Hz (the uart divides by 16)
        {0,0,0,0}, // Null Terminated List
    };

    // Configure the system
    OsDrvCprInit();
    OsDrvCprOpen();
    OsDrvCprAuxClockArrayConfig(auxClkAllOn);

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

    //Assign the one partition defined to all shaves
    for (int i = 0; i < 12; i++)
    {
        OsDrvShaveL2CSetNonWindowedPartition(i, 0, NON_WINDOWED_DATA_PARTITION);
        OsDrvShaveL2CSetNonWindowedPartition(i, 0, NON_WINDOWED_INSTRUCTIONS_PARTITION);
    }

    OsDrvShaveL2CachePartitionInvalidate(0);
    return 0;
}
