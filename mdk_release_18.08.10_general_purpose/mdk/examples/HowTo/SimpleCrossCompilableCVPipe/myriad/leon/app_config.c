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
#include <DrvRegUtils.h>
#include <DrvShaveL2Cache.h>
#include <DrvTimer.h>
#include "DrvDdr.h"
#include "app_config.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

#define L2CACHE_CFG                (SHAVE_L2CACHE_NORMAL_MODE)

#ifndef BUP_PLL0_FREQ
// no PLL by default
#define BUP_PLL0_FREQ 300000
#endif

#ifndef BUP_PLL1_FREQ
// no PLL by default
#define BUP_PLL1_FREQ 300000
#endif

									 
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// Sections decoration is required here for downstream tool

// 4: Static Local Data 
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
tyAuxClkDividerCfg appAuxClkCfg600_266[] =
{
    {AUX_CLK_MASK_UART ,CLK_SRC_REFCLK0,1,1},   // Give the UART a 12Mhz input source so that debug messages work
    {0,0,0,0}, // Null Terminated List
};
tySocClockConfig appClockConfig =
{
    .refClk0InputKhz         = 12000,           // Default 12Mhz input clock
    .refClk1InputKhz         = 0,               // Assume no secondary oscillator for now
    .targetPll0FreqKhz       = BUP_PLL0_FREQ, 
    .targetPll1FreqKhz       = 0,
    .clkSrcPll1              = CLK_SRC_REFCLK0, // Supply both PLLS from REFCLK0
    .masterClkDivNumerator   = 1,
    .masterClkDivDenominator = 1,
    .cssDssClockEnableMask   = DEFAULT_CORE_CSS_DSS_CLOCKS,
    .mssClockEnableMask      = 0xFFFFFFFF,             // Basic clocks to allow LRT and busses to function
    .sippClockEnableMask     = 0xFFFFFFFF,                            // Disable all SIPP clocks by default for now
    .upaClockEnableMask      = DEFAULT_UPA_CLOCKS,           // Enable all Shave clocks, CMXDMA, Shave L2Cache and UPA Control interfaces by default
    .pAuxClkCfg              = appAuxClkCfg600_266,
}; 

int initClocksAndMemory(void)
{
    s32 sc;

    sc = DrvTimerInit(); //needed to initialize LeonOS timer
    if(sc)
        return sc;
    sc = DrvCprInit();
    if(sc)
        return sc;
    sc = DrvCprSetupClocks(&appClockConfig);
    if(sc)
        return sc;
    DrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, 0xFFFFFFFF);
    DrvCprSysDeviceAction(UPA_DOMAIN, DEASSERT_RESET, APP_UPA_CLOCKS);

    DrvDdrInitialise(NULL);
    
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
