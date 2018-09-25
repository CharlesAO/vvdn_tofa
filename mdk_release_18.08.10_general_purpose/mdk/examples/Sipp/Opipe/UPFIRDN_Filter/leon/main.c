/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    main leon file: oPipe-SIGMA only test (passthrough)
/// 

// 1: Includes
// ----------------------------------------------------------------------------
#include "DrvLeon.h"
#include "app_config.h"
#include <stdio.h>

#include <DrvCpr.h>
#include "DrvDdr.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern u32 lrt_start;
// 4: Static Local Data
// ----------------------------------------------------------------------------
//Main full buffers in DDR

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
int main()
{
    initClocksAndMemory();
    DrvLeonRTStartup((u32)&lrt_start); // Start the LeonRT application
    DrvLeonRTWaitExecution();
    return 0;
}
