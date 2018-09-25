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
#include "OsDrvTimer.h"
#include "OsDrvCpr.h"

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
          .auxClockEnableMask     = AUX_CLK_MASK_USB_CTRL_SUSPEND_CLK,
          .auxClockSource         = CLK_SRC_PLL0,
          .auxClockDivNumerator   = 1,
          .auxClockDivDenominator = 24
      },
      {0, 0, 0, 0}, // Null Terminated List
    };

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

int initClocksAndMemory(void)
{
    s32 sc;
    // Configure the system
    sc = OsDrvCprInit();
    if(sc)
        return sc;
    sc = OsDrvCprOpen();
    if(sc)
        return sc;
    sc = OsDrvCprAuxClockArrayConfig(auxClk);
    if(sc)
        return sc;

    sc = OsDrvTimerInit();
    if(sc)
        return sc;

    return 0;
}
