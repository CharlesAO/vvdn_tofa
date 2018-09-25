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
#include "app_config.h"
#include "assert.h"
#include "DrvCpr.h"
#include "DrvTimer.h"
#include "registersMyriad.h"
#include "DrvGpio.h"
#include "DrvLeonL2C.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define SYS_CLK_KHZ             (12000) // 12MHz
#define PLL_DESIRED_FREQ_KHZ    (580000)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

int init(void)
{
    s32 sc;

    sc = DrvCprInitClocks(SYS_CLK_KHZ, 0, PLL_DESIRED_FREQ_KHZ, 0, PLL_DESIRED_FREQ_KHZ);
    if(sc)
        return sc;
    //    (u32 ref0InClkKhz, u32 ref1InClkKhz, u32 currentPll0ClockKhz, u32 currentPll1ClockKhz, u32 currentSysClockKhz)

    sc = DrvTimerInit();
    if(sc)
        return sc;

    DrvLL2CInitWriteThrough();

    return 0;
}

