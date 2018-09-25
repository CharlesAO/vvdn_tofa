///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    Application configuration Leon file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <OsDrvCpr.h>
#include "app_config.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define SYS_CLK_KHZ 12000

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
static const tyAuxClkDividerCfg auxClk[] =
{
    {
        .auxClockEnableMask = (u32)(1 << CSS_AUX_TSENS),
        .auxClockSource = CLK_SRC_REFCLK0,
        .auxClockDivNumerator = 1,
        .auxClockDivDenominator = 10,
    },
    {0,0,0,0}, // Null Terminated List
};

static const tySocClockConfig pSocClockConfig =
{
    .refClk0InputKhz = SYS_CLK_KHZ /* 12 MHz */,
    .refClk1InputKhz = 0 /* refClk1 not enabled for now */,
    .targetPll0FreqKhz = 600000, /* PLL0 target freq = 600 MHz */
    .targetPll1FreqKhz = 0, /* PLL1 not used for now */
    .clkSrcPll1 = CLK_SRC_REFCLK0, /* refClk1 is also not enabled for now */
    .masterClkDivNumerator = 1,
    .masterClkDivDenominator = 1,
    .cssDssClockEnableMask = DEFAULT_CORE_CSS_DSS_CLOCKS,
    .upaClockEnableMask = DEFAULT_UPA_CLOCKS,
    .mssClockEnableMask = MSS_CLOCKS_BASIC,
    .pAuxClkCfg = auxClk

};
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------


// 6: Functions Implementation
// ----------------------------------------------------------------------------

// Setup all the clock configurations needed by this application and also the DDR
int initClocksAndMemory(void)
{
    int retVal;

    // Configure the system
    retVal = OsDrvCprInit();
    if(retVal)
        return retVal;

    retVal = OsDrvCprOpen();
    if(retVal)
        return retVal;

    retVal = OsDrvCprSetupClocks(&pSocClockConfig);
    if(retVal)
        return retVal;

    return 0;
}
