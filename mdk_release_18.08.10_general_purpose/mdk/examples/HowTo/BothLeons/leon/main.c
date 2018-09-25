///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///


// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <DrvLeon.h>
#include "app_config.h"
#include <UnitTestApi.h>

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern u32 lrt_start;

// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
int main(void)
{
    s32 sc;

    sc = initClocksAndMemory();
    if(sc)
        return sc;
    unitTestInit ();
    printf("LeonRT Start Addr: 0x%lX\n",(u32)&lrt_start);

    printf("\n\nHello from LeonOS!\n\n\n");

    DrvLeonRTStartup((u32)&lrt_start); // Start the LeonRT application
    DrvLeonRTWaitExecution();
    unitTestFinalReport ();
    return 0;
}
