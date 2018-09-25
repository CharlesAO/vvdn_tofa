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
#include <stdlib.h>
#include <rtems.h>
#include <rtems/bspIo.h>
#include <DrvCpr.h>
#include <DrvLeon.h>
#include <UnitTestApi.h>
#include "rtems_config.h"

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
    printf("LeonRT Stop.\n");

    unitTestFinalReport();
    exit(0);
}




