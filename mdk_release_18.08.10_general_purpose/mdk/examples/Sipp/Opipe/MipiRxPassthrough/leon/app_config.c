///   
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt   
///
/// @brief     Key application system settings
/// 
/// 
/// 

// 1: Includes
// ----------------------------------------------------------------------------
#include "app_config.h"
#include "DrvCpr.h"
#include "DrvDdr.h"
#include "swcLeonUtils.h"
#include "DrvShaveL2Cache.h"
#include "DrvTimer.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// Sections decoration is require here for downstream tools

// 4: Static Local Data 
// ----------------------------------------------------------------------------

static tyAuxClkDividerCfg auxClk[] =
{
       {
          .auxClockEnableMask     = AUX_CLK_MASK_MEDIA,         // SIPP Clock
          .auxClockSource         = CLK_SRC_SYS_CLK_DIV2 ,      //
          .auxClockDivNumerator   = 1,                          //
          .auxClockDivDenominator = 1,                          //
      },
      {0, 0, 0, 0}, // Null Terminated List
};

static tySocClockConfig clocksConfig =
{
    .refClk0InputKhz         = SYS_CLK_KHZ      /* input oscillator source at 12 MHz */,
    .refClk1InputKhz         = 0,
    .targetPll0FreqKhz       = 480000,          /* system clock at 360 Mhz */
    .targetPll1FreqKhz       = 0,               /* will be set by DDR driver */
    .clkSrcPll1              = CLK_SRC_REFCLK0, /* refClk1 is also not enabled for now */
    .masterClkDivNumerator   = 1,
    .masterClkDivDenominator = 1,
    .cssDssClockEnableMask   = APP_CSS_DSS_CLOCKS,
    .mssClockEnableMask      = APP_MSS_CLOCKS,
    .sippClockEnableMask     = DEV_SIPP_MIPI_TX0, /*used to drive RX1 via callback*/
    .upaClockEnableMask      = APP_UPA_CLOCKS,
    .pAuxClkCfg              = auxClk,
};


// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

int initClocksAndMemory(void)
{
    int i;

    // Configure the system
    DrvCprInit();

    // Disable all the SHAVE and USB ISLANDS
    for (i=0;i<12;i++)
        DrvCprPowerTurnOffIsland(POWER_ISLAND_SHAVE_0 + i);
    DrvCprPowerTurnOffIsland(POWER_ISLAND_USB);
    DrvCprPowerTurnOffIsland(POWER_ISLAND_DSS);

    DrvCprSysDeviceAction(UPA_DOMAIN,  DEASSERT_RESET,    APP_UPA_CLOCKS );
    DrvCprSysDeviceAction(MSS_DOMAIN,  DEASSERT_RESET,    APP_MSS_CLOCKS );
    DrvCprSysDeviceAction(CSS_DOMAIN,  DEASSERT_RESET,    APP_CSS_DSS_CLOCKS);

    DrvCprSetupClocks(&clocksConfig);

    return 0;
}
