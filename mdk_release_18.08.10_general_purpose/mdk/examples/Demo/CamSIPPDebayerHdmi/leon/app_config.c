///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Key application system settings
///
///
///

// 1: Includes
// ----------------------------------------------------------------------------
#include "DrvDdr.h"
#include "assert.h"
#include "DrvShaveL2Cache.h"
#include "OsDrvTimer.h"
#include "OsDrvCpr.h"
#include "app_config.h"
#include "swcLeonUtils.h"
// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
      
#define L2CACHE_CFG                (SHAVE_L2CACHE_NORMAL_MODE)
#define PARTITION_0                (0)
#define SHAVES_USED                (12)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// Sections decoration is require here for downstream tools

// 4: Static Local Data
// ----------------------------------------------------------------------------
static tyAuxClkDividerCfg auxClk[] =
{
      {
          .auxClockEnableMask     = AUX_CLK_MASK_MEDIA,         // SIPP Clock
          .auxClockSource         = CLK_SRC_SYS_CLK ,      //
          .auxClockDivNumerator   = 1,                          //
          .auxClockDivDenominator = 1,                          //
      },
      {
          .auxClockEnableMask     = (AUX_CLK_MASK_CIF0 | AUX_CLK_MASK_CIF1),  // CIFs Clock
          .auxClockSource         = CLK_SRC_SYS_CLK,       //
          .auxClockDivNumerator   = 1,                          //
          .auxClockDivDenominator = 1,                          //
      },
      {
          .auxClockEnableMask     = CLOCKS_MIPICFG,             // MIPI CFG + ECFG Clock
          .auxClockSource         = CLK_SRC_SYS_CLK     ,       //
          .auxClockDivNumerator   = 1,                          //
          .auxClockDivDenominator = 24,                         //  the MIPI cfg clock should be <= 20 Mhz !
      },
      {0, 0, 0, 0}, // Null Terminated List
    };

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------
static tySocClockConfig clocksConfig =
{
    .refClk0InputKhz = SYS_CLK_KHZ /* 12 MHz */,
    .refClk1InputKhz = 0 /* refClk1 not enabled for now */,
    .targetPll0FreqKhz = PLL_DESIRED_FREQ_KHZ, /* PLL0 target freq = 580 MHz */
    .targetPll1FreqKhz = 0, /* set in DDR driver */
    .clkSrcPll1 = CLK_SRC_REFCLK0, /* refClk1 is also not enabled for now */
    .masterClkDivNumerator = 1,
    .masterClkDivDenominator = 1,
    .cssDssClockEnableMask = DEFAULT_CORE_CSS_DSS_CLOCKS,
    .upaClockEnableMask = DEFAULT_UPA_CLOCKS,
    .mssClockEnableMask = MSS_CLOCKS, /* MSS clocks enabled */
    .sippClockEnableMask = APP_SIPP_CLOCKS,
    .pAuxClkCfg = auxClk,
};

int initClocksAndMemory(void)
{
	u32 i;
    s32 sc;
    // Configure the system
    sc = OsDrvCprInit();
    if(sc)
        return sc;
    sc = OsDrvCprOpen();
    if(sc)
        return sc;

    DrvCprSysDeviceAction(SIPP_DOMAIN, ASSERT_RESET, DEV_SIPP_MIPI | DEV_SIPP_APB_SLV | DEV_SIPP_MIPI_RX0 | DEV_SIPP_MIPI_RX1 );
    DrvCprSysDeviceAction(MSS_DOMAIN, ASSERT_RESET,  DEV_MSS_SIPP | DEV_MSS_AMC | DEV_MSS_LCD | DEV_MSS_CIF0);

    OsDrvCprSetupClocks(&clocksConfig);

    DrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, -1);
    DrvCprSysDeviceAction(CSS_DOMAIN, DEASSERT_RESET, -1);
    DrvCprSysDeviceAction(SIPP_DOMAIN, DEASSERT_RESET, -1);
    DrvCprSysDeviceAction(UPA_DOMAIN, DEASSERT_RESET, -1);

    sc = OsDrvTimerInit();
    if(sc)
        return sc;

    DrvDdrInitialise(NULL);

    // Set the shave L2 Cache mode
    DrvShaveL2CacheSetMode(L2CACHE_CFG);

    //Set Shave L2 cache partitions
    DrvShaveL2CacheSetupPartition(SHAVEPART256KB);

    //Allocate Shave L2 cache set partitions
    sc = DrvShaveL2CacheAllocateSetPartitions();
    if(sc)
        return sc;

    //Assign the one partition defined to all shaves
    for (i = 0; i < SHAVES_USED; i++)
    {
        DrvShaveL2CacheSetLSUPartId(i, PARTITION_0);
    }

    DrvShaveL2CachePartitionFlushAndInvalidate(PARTITION_0);
    return 0;
}




