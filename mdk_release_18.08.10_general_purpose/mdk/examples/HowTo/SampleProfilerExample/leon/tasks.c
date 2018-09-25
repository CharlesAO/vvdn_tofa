///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

// 1: Includes
// ----------------------------------------------------------------------------
#include "system.h"
#include <stdio.h>
#include "OsDrvSvu.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define NUMBER_OF_SHAVES    8
#define WAIT_FOR_ONE_SHAVE  1

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern int spExample0_profile;
extern int spExample1_profile;
extern int spExample2_profile;
extern int spExample3_profile;
extern int spExample4_profile;
extern int spExample5_profile;
extern int spExample6_profile;
extern int spExample7_profile;

static u32 entrypoints[] =
{
    (u32)&spExample0_profile,
    (u32)&spExample1_profile,
    (u32)&spExample2_profile,
    (u32)&spExample3_profile,
    (u32)&spExample4_profile,
    (u32)&spExample5_profile,
    (u32)&spExample6_profile,
    (u32)&spExample7_profile,
};

// 4: Static Local Data
// ----------------------------------------------------------------------------
static swcShaveUnit_t SHAVE_NUMBER[NUMBER_OF_SHAVES] = {0, 1, 2, 3, 4, 5, 6, 7};
static osDrvSvuHandler_t handler[NUMBER_OF_SHAVES];

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

rtems_task SVU_task1(rtems_task_argument unused)
{
    UNUSED(unused);

    u32 status;
    u32 shaveIndex;
    u32 running;
    for (shaveIndex = 0; shaveIndex < NUMBER_OF_SHAVES; shaveIndex++)
    {
        status = OsDrvSvuOpenShave(&handler[shaveIndex], SHAVE_NUMBER[shaveIndex],OS_MYR_PROTECTION_SEM);
        if ( status == OS_MYR_DRV_SUCCESS)
        {
            status = OsDrvSvuResetShave(&handler[shaveIndex]);
            if(status != OS_MYR_DRV_SUCCESS)
                printf("Cannot reset shave %lu, status %lu \n", SHAVE_NUMBER[shaveIndex],status);

            status = OsDrvSvuSetAbsoluteDefaultStack(&handler[shaveIndex]);
            if(status != OS_MYR_DRV_SUCCESS)
                printf("Cannot set stack shave %lu, status %lu \n", SHAVE_NUMBER[shaveIndex],status);

            printf("Starting shave %lu from LOS rtems task...\n", SHAVE_NUMBER[shaveIndex]);
            status = OsDrvSvuStartShave(&handler[shaveIndex], entrypoints[shaveIndex]);
            if(status != OS_MYR_DRV_SUCCESS)
                printf("Cannot start shave %lu, status %lu \n", SHAVE_NUMBER[shaveIndex],status);

            status = OsDrvSvuWaitShaves(WAIT_FOR_ONE_SHAVE, &handler[shaveIndex], OS_DRV_SVU_WAIT_FOREVER, &running);
            if(status != OS_MYR_DRV_SUCCESS)
                printf("Cannot wait shave %lu, status %lu \n", SHAVE_NUMBER[shaveIndex],status);

            status = OsDrvSvuCloseShave(&handler[shaveIndex]);
            printf("Shave %lu execution finished in LOS rtems task.\n", SHAVE_NUMBER[shaveIndex]);

            if(status != OS_MYR_DRV_SUCCESS)
                printf("Cannot close shave %lu, status %lu \n", SHAVE_NUMBER[shaveIndex],status);
        }
        else
        {
               printf("Cannot open shave %lu, status %lu \n", SHAVE_NUMBER[shaveIndex],status);
        }
    }
    rtems_event_send(initId, TASK(1));
    rtems_task_delete(RTEMS_SELF);
}

