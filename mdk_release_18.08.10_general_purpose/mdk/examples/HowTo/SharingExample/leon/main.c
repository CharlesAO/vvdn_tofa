///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     How to example showing different ways of sharing data between shaves and leon
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "mv_types.h"
#include <bsp.h>
#include <DrvCpr.h>
#include <DrvLeon.h>
#include <UnitTestApi.h>
// project includes
#include "rtems_config.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// Leon RT start address
extern u32 lrt_start;
// Shared data between the two Leon cores - note the "lrt_" prefix
extern volatile int lrt_shared3[];

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

    unitTestInit();
    sc = initClocksAndMemory();
    if(sc)
        exit(sc);
    printf("LeonOS Started.\n");
    printf("LeonRT Start Addr: 0x%lX\n",(u32)&lrt_start);

    // Start the LeonRT application
    DrvLeonRTStartup((u32)&lrt_start);
    DrvLeonRTWaitExecution();
    printf("Shared data between Leons: %d %d %d %d\n", lrt_shared3[0], lrt_shared3[1], lrt_shared3[2], lrt_shared3[3]);

    printf("\nDone.\n");

    unitTestFinalReport();
    exit(0);
}
