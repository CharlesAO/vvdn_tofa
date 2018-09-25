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

#include <registersMyriad.h>
#include <DrvCpr.h>
#include <DrvSvu.h>
#include <DrvRegUtils.h>
#include <DrvShaveL2Cache.h>
#include <DrvDdr.h>
#include <DrvLeonL2C.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
// Standard CMX configuration

// Enable needed Shave clocks, CMXDMA, Shave L2Cache and UPA Control interfaces
#define APP_UPA_CLOCKS (DEV_UPA_SH0       | \
                        DEV_UPA_SH1       | \
                        DEV_UPA_SH2       | \
                        DEV_UPA_SH3       | \
                        DEV_UPA_SH4       | \
                        DEV_UPA_SH5       | \
                        DEV_UPA_SH6       | \
                        DEV_UPA_SH7       | \
                        DEV_UPA_SH8       | \
                        DEV_UPA_SH9       | \
                        DEV_UPA_SH10      | \
                        DEV_UPA_SH11      | \
                        DEV_UPA_SHAVE_L2  | \
                        DEV_UPA_CDMA      | \
                        DEV_UPA_CTRL      )

// Enable clocks needed for Leon and busses access
#define EXTRACLOCKS   (DEV_MSS_APB_SLV     | \
                       DEV_MSS_APB2_CTRL   | \
                       DEV_MSS_RTBRIDGE    | \
                       DEV_MSS_RTAHB_CTRL  | \
                       DEV_MSS_LRT         | \
                       DEV_MSS_LRT_DSU     | \
                       DEV_MSS_LRT_L2C     | \
                       DEV_MSS_LRT_ICB     | \
                       DEV_MSS_AXI_BRIDGE  | \
                       DEV_MSS_MXI_CTRL  )

#define L2CACHE_CFG     (SHAVE_L2CACHE_NORMAL_MODE)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// Sections decoration is required here for downstream tools
u32 __l2_config   __attribute__((section(".l2.mode")))  = L2CACHE_CFG;

// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

int initClocksAndMemory(void)
{
    int i;

    tyAuxClkDividerCfg appAuxClkCfg[] =
		    {
			{   
			    0,
                            0,
                            0,
                            0 
			}, // Null Terminated List
                    };

    tySocClockConfig appClockConfig600_266 =
                    {
                        .refClk0InputKhz = 12000,      // Default 12Mhz input clock
                        .refClk1InputKhz = 0,          // Assume no secondary oscillator for now
                        .targetPll0FreqKhz = 600000,
                        .targetPll1FreqKhz = 0,        // DDR frequency of 266.5Mhz will be 
						       // multipled by 2 in DDR_PHY to give 533Mhz
						       //  with is a rate of 1066 when clocked on 
						       //  each edge
                        .clkSrcPll1 = CLK_SRC_REFCLK0, // Supply both PLLS from REFCLK0
                        .masterClkDivNumerator = 1,
                        .masterClkDivDenominator = 1,
                        .cssDssClockEnableMask = DEFAULT_CORE_CSS_DSS_CLOCKS,
                        .mssClockEnableMask = EXTRACLOCKS,
                        .upaClockEnableMask = APP_UPA_CLOCKS,
                        .pAuxClkCfg = appAuxClkCfg,
                    };

    DrvCprInit();
    DrvCprSetupClocks(&appClockConfig600_266);

    DrvDdrInitialise(NULL);

    DrvCprSysDeviceAction(UPA_DOMAIN, DEASSERT_RESET, APP_UPA_CLOCKS);
    DrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, EXTRACLOCKS);

    // Invalidate entire Leon L2Cache
    DrvLL2CFlushOpOnAllLines(LL2C_OPERATION_INVALIDATE, 0);

    //Set Shave L2 cache partitions
    DrvShaveL2CacheSetupPartition(SHAVEPART16KB);

    //Allocate Shave L2 cache set partitions
    DrvShaveL2CacheAllocateSetPartitions();

    //Assign each Shave its own 16KB partition
    for (i = 0; i < 12; i++)
    {
        DrvShaveL2CacheSetLSUPartId(i, i % MAX_SHAVE_L2C_PARTITIONS);     //wrap around
        DrvShaveL2CachePartitionInvalidate(i % MAX_SHAVE_L2C_PARTITIONS); //
    }

    return 0;
}
