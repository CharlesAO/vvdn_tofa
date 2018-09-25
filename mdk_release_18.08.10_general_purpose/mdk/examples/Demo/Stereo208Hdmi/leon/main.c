///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     
///
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <DrvLeon.h>
#include <DrvCpr.h>
#include <DrvTimer.h>
#include <DrvDdr.h>
#include <swcLeonUtils.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <DrvShaveL2Cache.h>
#include <bsp.h>
#include "rtems_config.h"
#include "los_app_config.h"


// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
//Entry point of the leon_rt processor
extern u32      *lrt_start;


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
    DrvLeonRTStartup((u32)&lrt_start); 
    DrvLeonRTWaitExecution();
    exit(0);
}
