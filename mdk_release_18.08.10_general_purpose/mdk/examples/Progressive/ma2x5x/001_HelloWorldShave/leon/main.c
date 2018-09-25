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
#include <UnitTestApi.h>

#include <DrvShaveL2Cache.h>
#include <bsp.h>
#include <DrvCpr.h>
#include <DrvLeon.h>
#include "OsDrvSvu.h"
#include "rtems_config.h"
#include <unistd.h>
// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define SHAVE_USED 0

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern u32 helloShave0_main;

// 4: Static Local Data
// ----------------------------------------------------------------------------
static osDrvSvuHandler_t handler;
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

void *POSIX_Init(void *args)
{
    UNUSED(args);

    u32 running;
    s32 sc;
    unitTestInit ();
    sc = initClocksAndMemory();
    if (sc)
        exit(sc);
    sc = OsDrvSvuInit();
    if (sc)
        exit(sc);
    sc = OsDrvSvuOpenShave(&handler, SHAVE_USED, OS_MYR_PROTECTION_SEM);
    if (sc == OS_MYR_DRV_SUCCESS)
    {
        sc = OsDrvSvuResetShave(&handler);
        if (sc)
            exit(sc);
        sc = OsDrvSvuSetAbsoluteDefaultStack(&handler);
        if (sc)
            exit(sc);
        sc = OsDrvSvuStartShave(&handler, (u32) &helloShave0_main);
        if (sc)
            exit(sc);
        sc = OsDrvSvuWaitShaves(1, &handler, OS_DRV_SVU_WAIT_FOREVER, &running);
        if (sc)
            exit(sc);
        sc = OsDrvSvuCloseShave(&handler);
        if (sc)
            exit(sc);
    }
    else
    {
        printf("[T2] cannot open shave %d\n", SHAVE_USED);
    }
    unitTestFinalReport ();
    exit(0);
}
