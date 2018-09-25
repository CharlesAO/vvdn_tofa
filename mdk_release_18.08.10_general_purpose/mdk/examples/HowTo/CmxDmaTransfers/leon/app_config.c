///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Key application system settings
///
///
///

// 1: Includes
// ----------------------------------------------------------------------------
#include "OsDrvCpr.h"
#include "OsDrvShaveL2Cache.h"
#include "app_config.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define L2CACHE_CFG                (SHAVE_L2CACHE_NORMAL_MODE)
#define L2CACHE_PARTITION_SIZE  (SHAVEPART256KB)
#define L2CACHE_PARTITION_TYPE  (NON_WINDOWED_DATA_PARTITION)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// Sections decoration is require here for downstream tools

// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

int initClocksAndMemory(void)
{
    int32_t sc;

    tyAuxClkDividerCfg auxClkAllOn[] =
    {
        {AUX_CLK_MASK_UART, CLK_SRC_REFCLK0, 96, 625},   // Give the UART an SCLK that allows it to generate an output baud rate of of 115200 Hz (the uart divides by 16)
        {0,0,0,0}, // Null Terminated List
    };

    // Configure the system
    sc = OsDrvCprInit();
    if(sc)
        return sc;
    sc = OsDrvCprOpen();
    if(sc)
        return sc;
    sc = OsDrvCprAuxClockArrayConfig(auxClkAllOn);
    if(sc)
        return sc;

    // Set the shave L2 Cache mode
    sc = OsDrvShaveL2CacheInit(L2CACHE_CFG);
    if(sc)
        return sc;

    int partitionNumber;

    sc = OsDrvShaveL2CGetPartition(L2CACHE_PARTITION_SIZE, &partitionNumber);
    if(sc)
        return sc;

    sc = OsDrvShaveL2CacheAllocateSetPartitions();
    if(sc)
        return sc;

    for(int shaveNumber = 0; shaveNumber < SHAVES_USED; shaveNumber++)
    {
        sc = OsDrvShaveL2CSetNonWindowedPartition((shaveId_t)shaveNumber, partitionNumber, L2CACHE_PARTITION_TYPE);
        if(sc)
            return sc;
    }

    sc = OsDrvShaveL2CachePartitionInvalidate(partitionNumber);
    if(sc)
        return sc;

    return 0;
}
