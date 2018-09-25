///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon file
///

// 1: Includes
// ----------------------------------------------------------------------------

#include <registersMyriad.h>
#include <DrvCpr.h>
#include <DrvSvu.h>
#include <DrvRegUtils.h>
#include <DrvShaveL2Cache.h>
#include <DrvDdr.h>
#include <DrvLeonL2C.h>

#include "app_config.h"


// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

#define L2CACHE_CFG     (SHAVE_L2CACHE_NORMAL_MODE)

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
    s32 i;

    tyAuxClkDividerCfg auxClkAllOn[] =
    {
        {AUX_CLK_MASK_UART, CLK_SRC_REFCLK0, 96, 625},   // Give the UART an SCLK that allows it to generate an output baud rate of of 115200 Hz (the uart divides by 16)
        {0,0,0,0}, // Null Terminated List
    };

    tySocClockConfig appClockConfig600_266 =
    {
        .refClk0InputKhz         = 12000,           // Default 12Mhz input clock
        .refClk1InputKhz         = 0,               // Assume no secondary oscillator for now
        .targetPll0FreqKhz       = 266000,
        .targetPll1FreqKhz       = 0,               // set in DDR driver
        .clkSrcPll1              = CLK_SRC_REFCLK0, // Supply both PLLS from REFCLK0
        .masterClkDivNumerator   = 1,
        .masterClkDivDenominator = 1,
        .cssDssClockEnableMask   = DEFAULT_CORE_CSS_DSS_CLOCKS,
        .mssClockEnableMask      = APP_MSS_CLOCKS,
        .upaClockEnableMask      = APP_UPA_CLOCKS,
        .pAuxClkCfg              = auxClkAllOn,
    };

    DrvCprInit();
    DrvCprSetupClocks(&appClockConfig600_266);

    DrvDdrInitialise(NULL);

    DrvCprSysDeviceAction(UPA_DOMAIN, DEASSERT_RESET, APP_UPA_CLOCKS);
    DrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, APP_MSS_CLOCKS);

    // Invalidate entire L2Cache
    DrvLL2CFlushOpOnAllLines(LL2C_OPERATION_INVALIDATE, /*disable cache?:*/ 0);

    //Set Shave L2 cache partitions
    DrvShaveL2CacheSetupPartition(SHAVEPART256KB);

    //Allocate Shave L2 cache set partitions
    DrvShaveL2CacheAllocateSetPartitions();

    //Assign the one partition defined to all shaves
    for (i = 0; i < 12; i++)
    {
        DrvShaveL2CacheSetLSUPartId(i, 0);
    }

    DrvShaveL2CachePartitionInvalidate(0);

    swcLeonSetPIL(0);

    return 0;
}
