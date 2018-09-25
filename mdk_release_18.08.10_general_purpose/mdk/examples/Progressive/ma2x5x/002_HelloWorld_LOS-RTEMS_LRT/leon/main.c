///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Main leon file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <rtems.h>
#include <rtems/bspIo.h>
#include <DrvLeon.h>
#include "rtems_config.h"
#include <UnitTestApi.h>

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// Leon RT start address
extern u32 lrt_start;

// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

void *POSIX_Init (void *args)
{
    UNUSED(args);

    s32 sc;

    sc = initClocksAndMemory();
    if(sc)
        exit(sc);

    printf("LeonOS Started.\n");
    printf("LeonRT Start Addr: 0x%lX\n",(u32)&lrt_start);

    // Start the LeonRT application
    DrvLeonRTStartup((u32)&lrt_start);
    DrvLeonRTWaitExecution();

    printf("LeonRT Stop.\n");
    unitTestFinalReport ();
    exit(0);
}
