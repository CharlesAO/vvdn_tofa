///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <DrvCpr.h>
#include "app_config.h"
#include <DrvShaveL2Cache.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define L2CACHE_CFG     (SHAVE_L2CACHE_NORMAL_MODE)

#define MSS_CLOCKS_BASIC (DEV_MSS_APB_SLV     |     \
                          DEV_MSS_APB2_CTRL   |     \
                          DEV_MSS_RTBRIDGE    |     \
                          DEV_MSS_RTAHB_CTRL  )

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
static tyAuxClkDividerCfg auxClkCfgExample[] =
{
    {AUX_CLK_MASK_UART, CLK_SRC_REFCLK0, 96, 625},   // Give the UART an SCLK that allows it to generate an output baud rate of of 115200 Hz (the uart divides by 16)
    {0,0,0,0}, // Null Terminated List
};

static tySocClockConfig clkCfgExample =
{
    .refClk0InputKhz         = 12000,           // Default 12Mhz input clock
    .refClk1InputKhz         = 0,               // Assume no secondary oscillator for now
    .targetPll0FreqKhz       = 492000,
    .targetPll1FreqKhz       = 0,
    .clkSrcPll1              = CLK_SRC_REFCLK0, // Supply both PLLS from REFCLK0
    .masterClkDivNumerator   = DONT_TOUCH_MASTER_DIVIDER,
    .masterClkDivDenominator = DONT_TOUCH_MASTER_DIVIDER,
    .cssDssClockEnableMask   = DEFAULT_CORE_CSS_DSS_CLOCKS,
    .mssClockEnableMask      = MSS_CLOCKS_BASIC,             // Basic clocks to busses to function
    .sippClockEnableMask     = 0,                            // Disable all SIPP clocks by default for now
    .upaClockEnableMask      = DEFAULT_UPA_CLOCKS,           // Enable all Shave clocks, CMXDMA, Shave L2Cache and UPA Control interfaces by default
    .pAuxClkCfg              = auxClkCfgExample,
};

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

int initClocksAndMemory(void)
{  
    u32 sc;
    DrvCprInit();
    DrvCprSetupClocks(&clkCfgExample);

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
    
    DrvShaveL2CachePartitionInvalidate(PARTITION_0);

    return 0;
}

