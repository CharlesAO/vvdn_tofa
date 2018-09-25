///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    Application configuration Leon file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <OsDrvSvu.h>
#include <OsDrvShaveL2Cache.h>
#include "app_config.h"

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

// Setup all the clock configurations needed by this application and also the DDR
int initClocksAndMemory(int * dataPartitionID)
{
    s32 sc;

    /* Init Shave L2Cache */
    sc=OsDrvShaveL2CacheInit(SHAVEL2C_MODE_NORMAL);
    if(sc)
        return sc;

    /* reset L2C configuration */
    sc=OsDrvShaveL2CResetPartitions();
    if (sc)
        return sc;

    /* Config Shave L2 Cache */
    sc = OS_MYR_DRV_SUCCESS;
    uint32_t i;

    //We'll configure 4 partitions: two for code and  two  for data
    int partID[4];

    /* Set Shave L2 cache partitions 16K / shave is sufficient */
    sc += OsDrvShaveL2CGetPartition(SHAVEPART64KB, &partID[0]);
    sc += OsDrvShaveL2CGetPartition(SHAVEPART64KB, &partID[1]);

    sc += OsDrvShaveL2CGetPartition(SHAVEPART64KB, &partID[2]);
    sc += OsDrvShaveL2CGetPartition(SHAVEPART64KB, &partID[3]);

    for (i=0; i < 2; i++){
        /* Assign the first partition as instruction partition for all shaves */
        sc += OsDrvShaveL2CSetNonWindowedPartition(i,partID[0],
                                            NON_WINDOWED_INSTRUCTIONS_PARTITION);
        /* Assign the second partition as data partition for all shaves */
        sc += OsDrvShaveL2CSetNonWindowedPartition(i,partID[1],
                                            NON_WINDOWED_DATA_PARTITION);
    }

    for (i=2; i < 4; i++){
           /* Assign the first partition as instruction partition for all shaves */
           sc += OsDrvShaveL2CSetNonWindowedPartition(i,partID[2],
                                            NON_WINDOWED_INSTRUCTIONS_PARTITION);
           /* Assign the second partition as data partition for all shaves */
           sc += OsDrvShaveL2CSetNonWindowedPartition(i,partID[3],
                                            NON_WINDOWED_DATA_PARTITION);
           sc += OsDrvShaveL2CSetWindowPartition(i,SHAVEL2CACHEWIN_A,partID[3]);
       }

    /* Allocate Shave L2 cache set partitions */
    sc += OsDrvShaveL2CacheAllocateSetPartitions();
    //Invalidate cache
    OsDrvShaveL2CachePartitionInvalidate(partID[0]);
    OsDrvShaveL2CachePartitionInvalidate(partID[1]);
    OsDrvShaveL2CachePartitionInvalidate(partID[2]);
    OsDrvShaveL2CachePartitionInvalidate(partID[3]);

    /* Return the data partition ID, 
    that can be used to flush and invalidate the partition*/
    *dataPartitionID = partID[3];

    //Initialize the SHAVE driver
    sc=OsDrvSvuInit();
    if(sc) return sc;

    return sc;
}
