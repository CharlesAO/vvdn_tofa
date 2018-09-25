///   
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved. 
///            For License Warranty see: common/license.txt   
///
/// @brief     Key application system settings
/// 
/// 
/// 

// 1: Includes
// ----------------------------------------------------------------------------
#include "lrt_app_config.h"

#include "DrvCpr.h"
#include "DrvTimer.h"


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
int initClocksAndMemory(void)
{
    s32 sc;

    sc = DrvCprInitClocks(SYS_CLK_KHZ, 0, 0, 0, 0);
    //    (u32 ref0InClkKhz, u32 ref1InClkKhz, u32 unused0, u32 unused1, u32 unused2)
    if(sc)
        return sc;
    sc = DrvTimerInit();
    if(sc)
        return sc;
    return 0;
}
