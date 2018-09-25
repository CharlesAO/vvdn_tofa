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
#include "stdio.h"

#include <DrvCpr.h>
#include <DrvDdr.h>
#include <DrvRegUtils.h>

#include <registersMyriad.h>
#include <DrvShaveL2Cache.h>
#include <DrvSvu.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define L2CACHE_CFG     (SHAVE_L2CACHE_NORMAL_MODE)
#define APP_UPA_CLOCKS (DEV_UPA_SH0       | \
                        DEV_UPA_SHAVE_L2  | \
                        DEV_UPA_CDMA      | \
                        DEV_UPA_CTRL      )

#define MSS_BASIC_CLOCKS (DEV_MSS_APB_SLV     |     \
                          DEV_MSS_APB2_CTRL   |     \
                          DEV_MSS_RTBRIDGE    |     \
                          DEV_MSS_RTAHB_CTRL  |     \
                          DEV_MSS_LRT         |     \
                          DEV_MSS_LRT_DSU     |     \
                          DEV_MSS_LRT_L2C     |     \
                          DEV_MSS_LRT_ICB     |     \
                          DEV_MSS_AXI_BRIDGE  |     \
                          DEV_MSS_MXI_CTRL    |     \
                          DEV_MSS_MXI_DEFSLV  |     \
                          DEV_MSS_AXI_MON     |     \
                          DEV_MSS_AMC         |     \
                          DEV_MSS_SIPP        |     \
                          DEV_MSS_TIM)
                         
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
    tyAuxClkDividerCfg appAuxClkCfg[] =
    {
        {AUX_CLK_MASK_UART, CLK_SRC_REFCLK0, 96, 625},   // Give the UART an SCLK that allows it to generate an output baud rate of of 115200 Hz (the uart divides by 16)

        {0, 0, 0, 0 }, // Null Terminated List
    };

    // This is needed for the simulator at the moment!
    // Enable all aux clocks to ensure sclk to uart enabled
    tySocClockConfig clk_cfg_mvcv =
    {
        .refClk0InputKhz = 12000,   // Default 12Mhz input clock
        .refClk1InputKhz = 0, // Assume no secondary oscillator for now
        .targetPll0FreqKhz = 480000,
        .targetPll1FreqKhz = 0, // DDR frequency of 266.5Mhz will be multipled by 2 in DDR_PHY to give 533Mhz with is a rate of 1066 when clocked on each edge
        .clkSrcPll1 = CLK_SRC_REFCLK0, // Supply both PLLS from REFCLK0
        .masterClkDivNumerator = 1,
        .masterClkDivDenominator = 1,
        .cssDssClockEnableMask = DEFAULT_CORE_CSS_DSS_CLOCKS,
        .mssClockEnableMask = MSS_BASIC_CLOCKS,
        .upaClockEnableMask = APP_UPA_CLOCKS,
        .pAuxClkCfg = appAuxClkCfg,
    };

// 4: Static Local Data 
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

int initClocksAndMemory(void)
{
    int i;
    // This is needed for the simulator at the moment!
    // Enable all aux clocks to ensure sclk to uart enabled
    DrvCprInit();  
    DrvCprStartAllClocks();
    DrvCprSetupClocks(&clk_cfg_mvcv); 

    #ifdef INT_DDR
    
    DrvDdrInitialise(NULL);

    //printf("\nEnable Shave L2 Cache\n\n");
    DrvShaveL2CacheSetMode(SHAVE_L2CACHE_NORMAL_MODE);
    
    DrvShaveL2CacheClearPartitions();
    
    //Set Shave L2 cache partitions
    DrvShaveL2CacheSetupPartition(SHAVEPART256KB);

    //Allocate Shave L2 cache set partitions
    DrvShaveL2CacheAllocateSetPartitions();

    //Assign the one partition defined to all shaves
   for (i = 0; i < SHAVES_USED; i++)
    {
        DrvShaveL2CacheSetLSUPartId(i, PARTITION_0);
    }

    DrvShaveL2CachePartitionInvalidate(PARTITION_0);
    #endif

    #ifdef AUTOCHECK_ELF_FILE
    // for vcs, to avoid xx, initialization all the irf, vrf registers, now xor v0, v0, v0 will make v0 = 0
    for(i = 0; i < 32; i++){
        DrvSvutIrfWrite(0, i, 0xAAAAAAAA);
        DrvSvutVrfWrite(0, i, 0,  0xAAAAAAAA);
        DrvSvutVrfWrite(0, i, 1,  0xAAAAAAAA);
        DrvSvutVrfWrite(0, i, 2,  0xAAAAAAAA);
        DrvSvutVrfWrite(0, i, 3,  0xAAAAAAAA);
    }
    #endif

    return 0;
}






