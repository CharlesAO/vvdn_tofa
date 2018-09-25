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

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define SYS_CLK_KHZ             (12000) // 12MHz

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

    sc = DrvCprInitClocks(SYS_CLK_KHZ, 0, 0, 0, 0);
    if(sc)
        return sc;

    sc = DrvTimerInit();
    if(sc)
        return sc;

    return 0;
}

