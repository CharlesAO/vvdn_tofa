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
//only make this as a temporary fix, till everything will be put in place for ma2x8x
#ifdef CVMA2480
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

#define MSS_BASIC_CLOCKS (DEV_MSS_LRT         |     \
                          DEV_MSS_LRT_DSU     |     \
                          DEV_MSS_LRT_L2C     |     \
                          DEV_MSS_LRT_ICB     |     \
                          DEV_MSS_AMC         |     \
                          DEV_MSS_SIPP        |     \
                          DEV_MSS_TIM)
#else
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
#endif

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

int initClocksAndMemory(void)
{
    int i;
    int shaveNumber;

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
			#ifdef CVMA2480 // only do this for ma2480 purpose
                        .targetPll0FreqKhz = 266000,
                        .targetPll1FreqKhz = 0, // DDR frequency of 266.5Mhz will be multipled by 2 in DDR_PHY to give 533Mhz with is a rate of 1066 when clocked on each edge
			#else
			.targetPll0FreqKhz = 480000,
                        .targetPll1FreqKhz = 0, // DDR frequency of 480 Mhz will be multipled by 2 in DDR_PHY to give 533Mhz with is a rate of 1066 when clocked on each edge
			#endif
                        .clkSrcPll1 = CLK_SRC_REFCLK0, // Supply both PLLS from REFCLK0
                        .masterClkDivNumerator = 1,
                        .masterClkDivDenominator = 1,
                        .cssDssClockEnableMask = DEFAULT_CORE_CSS_DSS_CLOCKS,
                        .mssClockEnableMask = MSS_BASIC_CLOCKS,
                        .upaClockEnableMask = APP_UPA_CLOCKS,
                        .pAuxClkCfg = appAuxClkCfg,
    };

    DrvCprInit();
    DrvCprStartAllClocks();
    DrvCprSetupClocks(&clk_cfg_mvcv);

    #if defined(INT_DDR) || defined(USE_SHAVE_L2_CACHE)

    DrvDdrInitialise(NULL);

    //printf("\nEnable Shave L2 Cache\n\n");
    SET_REG_WORD(L2C_MODE_ADR, SHAVE_L2CACHE_NORMAL_MODE);

    DrvShaveL2CacheClearPartitions();

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
    #endif

    #ifdef AUTOCHECK_ELF_FILE
    // for vcs, to avoid xx, initialization all the irf, vrf registers, now xor v0, v0, v0 will make v0 = 0
    for(i = 0; i < 32; i++){
		for(shaveNumber = 0; shaveNumber < 12 ;shaveNumber++){
            DrvSvutIrfWrite(shaveNumber, i, 0x41551563 + i);     // make preserved registers as random as possible
            DrvSvutVrfWrite(shaveNumber, i, 0,  0x88855969 + i); // for testing purpose
            DrvSvutVrfWrite(shaveNumber, i, 1,  0x21175989 + i);
            DrvSvutVrfWrite(shaveNumber, i, 2,  0x22725615 + i);
            DrvSvutVrfWrite(shaveNumber, i, 3,  0x20841619 + i);
		}
    }
    #endif
    return 0;
}
