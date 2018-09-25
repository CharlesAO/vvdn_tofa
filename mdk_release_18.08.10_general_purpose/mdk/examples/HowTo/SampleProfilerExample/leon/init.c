///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <rtems.h>
#include <bsp.h>
#include "OsDrvCpr.h"
#include "OsDrvSvu.h"
#include "system.h"
#include "app_config.h"
#include <DrvLeon.h>
#include <UnitTestApi.h>
#include "DrvDdr.h"
#include <DrvShaveL2Cache.h>

#define EXTRACLOCKS  (DEV_MSS_APB_SLV     | \
                      DEV_MSS_APB2_CTRL   | \
                      DEV_MSS_RTBRIDGE    | \
                      DEV_MSS_RTAHB_CTRL  | \
                      DEV_MSS_LRT         | \
                      DEV_MSS_LRT_DSU     | \
                      DEV_MSS_LRT_L2C     | \
                      DEV_MSS_LRT_ICB     | \
                      DEV_MSS_AXI_BRIDGE  | \
                      DEV_MSS_MXI_CTRL  )
#define APP_CSS_DSS_CLOCKS             ( DEV_CSS_LOS             |    \
                                         DEV_CSS_LAHB_CTRL       |    \
                                         DEV_CSS_APB4_CTRL       |    \
                                         DEV_CSS_CPR             |    \
                                         DEV_CSS_LOS_L2C         |    \
                                         DEV_CSS_LOS_ICB         |    \
                                         DEV_CSS_GPIO            |    \
                                         DEV_CSS_JTAG            |    \
                                         DEV_CSS_UART            |    \
                                         DEV_CSS_MSS_MAS         |    \
                                         DEV_CSS_UPA_MAS         |    \
                                         DEV_CSS_LOS_DSU         |    \
                                         DEV_CSS_AON                  )

#define APP_MSS_CLOCKS          ( DEV_MSS_SIPP          | \
                                  DEV_MSS_AMC           | \
                                  DEV_MSS_LRT           | \
                                  DEV_MSS_LRT_DSU       | \
                                  DEV_MSS_LRT_L2C       | \
                                  DEV_MSS_LRT_ICB         )

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
// System Clock configuration on start-up
BSP_SET_CLOCK(DEFAULT_OSC_CLOCK_KHZ, 300000, 1, 1,
              DEFAULT_RTEMS_CSS_LOS_CLOCKS, DEFAULT_RTEMS_MSS_LRT_CLOCKS | EXTRACLOCKS |
			  DEV_CSS_LOS_DSU|DEV_CSS_LOS_TIM|DEV_CSS_MSS_MAS, /*| APP_CSS_DSS_CLOCKS |  APP_MSS_CLOCKS*/
			  APP_UPA_CLOCKS,0,0);

// Program L2 cache behaviour
BSP_SET_L2C_CONFIG(0, DEFAULT_RTEMS_L2C_REPLACEMENT_POLICY, DEFAULT_RTEMS_L2C_LOCKED_WAYS,DEFAULT_RTEMS_L2C_MODE, 0, 0);

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
rtems_id initId;

// Leon RT start address
extern u32 lrt_start;

// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
rtems_task Init(rtems_task_argument argument)
{
    (void) argument; // "use" the variables to hush the compiler warning.

    rtems_name task_name;
    rtems_status_code status;
    rtems_id task_id;
    u32 eventsOut;

    printf("Starting profiler example...\n");

    unitTestInit();
    DrvCprInit();
    DrvDdrInitialise(NULL);
    initId = rtems_task_self();

    status = OsDrvSvuInit();
    if(status != 0)
          printf("Error SVU init, status %u!= 0\n",status);


    // Create New Task
    task_name = rtems_build_name( 'S', 'H', 'V', '0' );

    // Task creation
    status = rtems_task_create(
                    task_name, 1, RTEMS_MINIMUM_STACK_SIZE * 2,
                    RTEMS_DEFAULT_MODES,
                    RTEMS_DEFAULT_ATTRIBUTES, &task_id);

    // Start Task
    status = rtems_task_start(task_id, SVU_task1, 1);

    // wait for task to end
    rtems_event_receive(TASK(1), RTEMS_WAIT|RTEMS_EVENT_ALL, RTEMS_NO_TIMEOUT, &eventsOut);

    // Start LRT
    printf("Starting LRT..\n");
    DrvLeonRTStartup((u32)&lrt_start);

    // wait for LRT to end
    DrvLeonRTWaitExecution();
    printf("LRT stoped. LOS exiting...\n");


    printf("Sample profiler example executed successfully\n");

    DrvSvuFlushInvalidateL1DataCache(0, 12, SVUL1DATACACHE_FLUSH_INVALIDATE_ALL);
    for(int s=0; s<12; s++) DrvShaveL2CachePartitionFlushAndInvalidate(s%MAX_SHAVE_L2C_PARTITIONS);
    unitTestFinalReport();
    // shutdown RTEMS
    rtems_shutdown_executive(0);
}
