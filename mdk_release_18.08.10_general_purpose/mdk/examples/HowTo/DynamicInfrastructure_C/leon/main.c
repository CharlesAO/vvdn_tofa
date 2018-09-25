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
#include "OsDrvSvu.h"
#include "rtems_config.h"
#include "theDynContext.h"
#include <UnitTestApi.h>

// 4: Local defines
// ----------------------------------------------------------------------------
#define NO_OF_MESG_APP_INSTANCES_ONE 1
#define NO_OF_MESG_APP_INSTANCES_TWO 1

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
extern DynamicContext_t MODULE_DATA(MesgOne);
extern DynamicContext_t MODULE_DATA(MesgTwo);

extern unsigned int lrt_start;

unsigned int __attribute__((section(".cmx.bss"))) someArray[200];

// 6: Functions Implementation
// ----------------------------------------------------------------------------

void *POSIX_Init(void *args) {
    int shaveUsedA, shaveUsedB, status;
    u32 running;
    swcShaveUnit_t shaveListMesgOne[NO_OF_MESG_APP_INSTANCES_ONE] = {0};
    swcShaveUnit_t shaveListMesgTwo[NO_OF_MESG_APP_INSTANCES_TWO] = {3};

    UNUSED(args);
    /* ================*/
    /* Initialize POSIX system */
    /* ================ */
    unitTestInit ();
    printf("\n");
    printf("RTEMS POSIX Started with some CMX at 0x%X is set as 0x%X\n",
            (unsigned int) &someArray[100], someArray[100]); /* Initialize variables */

    status = initClocksAndMemory();
    if (status != OS_MYR_DRV_SUCCESS)
           printf("Error initializing clocks and memory.\n");

    /* Init and configure L2 Cache */
    status = InitShaveL2C();
    if (status != OS_MYR_DRV_SUCCESS)
        printf("Error initializing shave L2 cache. \n");

    status = ConfigShaveL2C();
    if (status != OS_MYR_DRV_SUCCESS)
        printf("Error configuring shave L2 cache. \n");

    status = OsDrvSvuInit();  /* init the OS shave driver */
    if (status)
        exit(status);

    printf("Launching first shave program with dyncontext at 0x%X\n",
            (unsigned int) &MODULE_DATA(MesgOne));

    status = OsDrvSvuSetupDynShaveApps(&MODULE_DATA(MesgOne), shaveListMesgOne, NO_OF_MESG_APP_INSTANCES_ONE);
    if (status != OS_MYR_DYN_INFR_SUCCESS)
        OSDRV_DYN_INFR_CHECK_CODE(status);

    status = OsDrvSvuOpenShaves(shaveListMesgOne, NO_OF_MESG_APP_INSTANCES_ONE, OS_MYR_PROTECTION_SELF);
    if (status != OS_MYR_DYN_INFR_SUCCESS)
        OSDRV_DYN_INFR_CHECK_CODE(status);

    status = OsDrvSvuRunShaveAlgo(&MODULE_DATA(MesgOne), &shaveUsedA);
    if (status != OS_MYR_DYN_INFR_SUCCESS)
        OSDRV_DYN_INFR_CHECK_CODE(status);

    status = OsDrvSvuDynWaitShaves(shaveListMesgOne, NO_OF_MESG_APP_INSTANCES_ONE, OS_DRV_SVU_WAIT_FOREVER, &running);
    if (status != OS_MYR_DYN_INFR_SUCCESS)
        OSDRV_DYN_INFR_CHECK_CODE(status);

    printf("Operations above executed by shave %d \n", shaveUsedA);
    printf("Launching second shave program on a different shave\n");

    status = OsDrvSvuSetupDynShaveApps(&MODULE_DATA(MesgTwo), shaveListMesgTwo, NO_OF_MESG_APP_INSTANCES_TWO);
    if (status != OS_MYR_DYN_INFR_SUCCESS)
        OSDRV_DYN_INFR_CHECK_CODE(status);

    status = OsDrvSvuOpenShaves(shaveListMesgTwo, NO_OF_MESG_APP_INSTANCES_TWO, OS_MYR_PROTECTION_SELF);
    if (status != OS_MYR_DYN_INFR_SUCCESS)
        OSDRV_DYN_INFR_CHECK_CODE(status);

    status = OsDrvSvuRunShaveAlgo(&MODULE_DATA(MesgTwo), &shaveUsedB);
    if (status != OS_MYR_DYN_INFR_SUCCESS)
        OSDRV_DYN_INFR_CHECK_CODE(status);

    status = OsDrvSvuDynWaitShaves(shaveListMesgTwo, NO_OF_MESG_APP_INSTANCES_TWO, OS_DRV_SVU_WAIT_FOREVER, &running);
    if (status != OS_MYR_DYN_INFR_SUCCESS)
        OSDRV_DYN_INFR_CHECK_CODE(status);

    printf("Operations above executed by shave %d \n", shaveUsedB);
    printf("Launching both at the same time:\n");

    status = OsDrvSvuRunShaveAlgo(&MODULE_DATA(MesgTwo), &shaveUsedB);
    if (status != OS_MYR_DYN_INFR_SUCCESS)
        OSDRV_DYN_INFR_CHECK_CODE(status);

    status = OsDrvSvuRunShaveAlgo(&MODULE_DATA(MesgOne), &shaveUsedA);
    if (status != OS_MYR_DYN_INFR_SUCCESS)
        OSDRV_DYN_INFR_CHECK_CODE(status);

    status = OsDrvSvuDynWaitShaves(shaveListMesgTwo, NO_OF_MESG_APP_INSTANCES_TWO, OS_DRV_SVU_WAIT_FOREVER, &running);
    if (status != OS_MYR_DYN_INFR_SUCCESS)
        OSDRV_DYN_INFR_CHECK_CODE(status);

    status = OsDrvSvuDynWaitShaves(shaveListMesgOne, NO_OF_MESG_APP_INSTANCES_ONE, OS_DRV_SVU_WAIT_FOREVER, &running);
    if (status != OS_MYR_DYN_INFR_SUCCESS)
        OSDRV_DYN_INFR_CHECK_CODE(status);

    printf("Operations above executed by shaves %d and %d\n", shaveUsedB, shaveUsedA);

    /* close now the shaves used */
    status = OsDrvSvuCloseShaves(shaveListMesgOne, NO_OF_MESG_APP_INSTANCES_ONE);
    if (status != OS_MYR_DYN_INFR_SUCCESS)
        OSDRV_DYN_INFR_CHECK_CODE(status);

    status = OsDrvSvuCloseShaves(shaveListMesgTwo, NO_OF_MESG_APP_INSTANCES_TWO);
    if (status != OS_MYR_DYN_INFR_SUCCESS)
        OSDRV_DYN_INFR_CHECK_CODE(status);

    /* free the Leon OS applications memory */
    status = OsDrvSvuCleanupDynShaveApps(&MODULE_DATA(MesgTwo));
    if (status != OS_MYR_DYN_INFR_SUCCESS)
        OSDRV_DYN_INFR_CHECK_CODE(status);

    status = OsDrvSvuCleanupDynShaveApps(&MODULE_DATA(MesgOne));
    if (status != OS_MYR_DYN_INFR_SUCCESS)
        OSDRV_DYN_INFR_CHECK_CODE(status);

    /* And launch the LeonRT and wait for it to finish */
    DrvLeonRTStartup((u32) &lrt_start);
    DrvLeonRTWaitExecution();

    unitTestFinalReport ();
    exit(0);
}

/* User extension to be able to catch abnormal terminations */
