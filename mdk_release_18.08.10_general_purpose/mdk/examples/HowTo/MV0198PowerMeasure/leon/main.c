///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Simple Example showing MV0198 Power Measure usage.
///            It can be used for testing purposes, too
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <DrvRegUtilsDefines.h>
#include <DrvLeon.h>
#include <DrvCpr.h>
#include <DrvTimer.h>
#include <DrvDdr.h>
#include <swcLeonUtils.h>
#include "app_config.h"
#include <stdlib.h>
#include <stdio.h>
#include <bsp.h>
#include "rtemsConfig.h"
#include <dbgTracerApi.h>
#include <dbgLogEvents.h>
#include <DrvLeon.h>

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define DDR_BUF __attribute__((section(".ddr.data")))

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

//Entry point of the leon_rt processor
extern u32      *lrt_start;

void *POSIX_Init (void *args)
{
    UNUSED(args);

    rtems_status_code status;
    rtems_id task_id;
    rtems_name task_name;
    printf("\n---- Measure Test App ----\n");

    initClocksAndMemory();

    DrvLeonRTStartup((u32)&lrt_start);

     // Create New Task
    task_name = rtems_build_name('P', 'M', 'E', 'S');

    // Task creation
    status = rtems_task_create(task_name, 1, RTEMS_MINIMUM_STACK_SIZE * 2,
                               RTEMS_DEFAULT_MODES, RTEMS_DEFAULT_ATTRIBUTES, &task_id);

    if (status != RTEMS_SUCCESSFUL)
    {
        printf("rtems_task_create failed with %d\n", status);
    }

    //wait for leon rt to initialize i2c
    while (!DrvLeonRTHasBooted())
        rtems_task_wake_after(1);

    // Start task that measure
    status = rtems_task_start(task_id, Measure_task, 1);
    if (status != RTEMS_SUCCESSFUL) {
        printf("rtems_task_start failed with %d\n", status);
    }

    rtems_task_delete( RTEMS_SELF);
	exit(0);
}
