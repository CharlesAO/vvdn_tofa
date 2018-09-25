///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    Application configuration Leon file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <registersMyriad.h>
#include <OsDrvCpr.h>
#include "OsDrvTimer.h"
#include <DrvRegUtils.h>
#include <OsDrvShaveL2Cache.h>
#include "DrvDdr.h"
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
int initClocksAndMemory(void) {
    tyAuxClkDividerCfg auxClkAllOn[] =
    {
        // Give the UART an SCLK that allows it to generate an output baud rate of of 115200 Hz (the uart divides by 16)
        { AUX_CLK_MASK_UART, CLK_SRC_REFCLK0, 96, 625 },
        { 0, 0, 0, 0 }, // Null Terminated List
    };

    // Configure the system
    OsDrvCprInit();
    OsDrvCprOpen();
    OsDrvCprAuxClockArrayConfig(auxClkAllOn);

    //Add pulse reset clocks for UPA and MSS
    OsDrvCprSysDeviceAction(UPA_DOMAIN, PULSE_RESET, APP_UPA_CLOCKS);

    return 0;
}
/* Init Shave L2 Cache */
int InitShaveL2C(void)
{
    s32 sc;
    /* Init Shave L2Cache */
    sc=OsDrvShaveL2CacheInit(L2CACHE_CFG);
    if(sc)
        return sc;

    /* reset L2C configuration */
    sc=OsDrvShaveL2CResetPartitions();
    return sc;
}

/* Config Shave L2 Cache */
int ConfigShaveL2C(void)
{
    s32 sc = OS_MYR_DRV_SUCCESS;
    uint32_t i;
    int partition_id[8];
    /* Set Shave L2 cache partitions 16K / shave is sufficient */
    for (int i = 0; i < 8; ++i) {
        sc += OsDrvShaveL2CGetPartition(SHAVEPART16KB, &partition_id[i]);
    }

    for (i=0; i < 12; i++) {
        /* Assign the single defined partition to all shaves */
        sc += OsDrvShaveL2CSetNonWindowedPartition(i, partition_id[0], NON_WINDOWED_INSTRUCTIONS_PARTITION);
        sc += OsDrvShaveL2CSetWindowPartition(i, SHAVEL2CACHEWIN_A, partition_id[1+(i%7)]);
    }
    /* Allocate Shave L2 cache set partitions */
    sc += OsDrvShaveL2CacheAllocateSetPartitions();
    
    for (int i = 0; i < 8; ++i) {
        OsDrvShaveL2CachePartitionInvalidate(partition_id[i]);
    }

    return sc;
}
