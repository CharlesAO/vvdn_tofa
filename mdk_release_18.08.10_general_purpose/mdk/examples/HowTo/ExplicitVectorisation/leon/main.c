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
#include <OsDrvSvu.h>
#include <rtems.h>
#include <rtems/bspIo.h>
#include <DrvShaveL2Cache.h>
#include <DrvCpr.h>
#include "rtems_config.h"
#include <UnitTestApi.h>

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define SHAVE_USED 0

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// include here for convenience
// for complex shave modules split to separate shave_export.h file or similar
extern u32 exvec0_main;
extern volatile int exvec0_myint1[4];
extern volatile int exvec0_myint2[4];
extern volatile int exvec0_myintrez[4];

// 4: Static Local Data
// ----------------------------------------------------------------------------
static osDrvSvuHandler_t handler;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
// simple test to validate vector addition in Shave0
void *POSIX_Init (void *args)
{
    UNUSED(args);

    u32 i;
    u32 test_pass = 1;
    u32 running;
    s32 sc;
    
    unitTestInit ();
    sc = initClocksAndMemory();
    if(sc)
        exit(sc);
    sc = OsDrvSvuInit();
    if(sc)
        exit(sc);
    printf("Executing Explicit Vectorisation test...\n");

    // Set input parameters Shaves
    exvec0_myint1[0] = 4;
    exvec0_myint1[1] = 25;
    exvec0_myint1[2] = 313;
    exvec0_myint1[3] = 13;

    exvec0_myint2[0] = 4;
    exvec0_myint2[1] = 30;
    exvec0_myint2[2] = 400;
    exvec0_myint2[3] = 500;

    sc = OsDrvSvuOpenShave(&handler, SHAVE_USED, OS_MYR_PROTECTION_SEM);
    if(sc)
        exit(sc);

    if (sc == OS_MYR_DRV_SUCCESS)
    {
        sc = OsDrvSvuResetShave(&handler);
        if(sc)
            exit(sc);
        sc = OsDrvSvuSetAbsoluteDefaultStack(&handler);
        if(sc)
            exit(sc);
        sc = OsDrvSvuStartShave(&handler,(u32)&exvec0_main);
        if(sc)
            exit(sc);
        sc = OsDrvSvuWaitShaves(1, &handler, OS_DRV_SVU_WAIT_FOREVER, &running);
        if(sc)
            exit(sc);
        sc = OsDrvSvuCloseShave(&handler);
        if(sc)
            exit(sc);
    }
    else {
        printf("[T2] cannot open shave %d\n", SHAVE_USED);
    }

    // validate result
    for (i = 0; i < 3; i++)
        if (exvec0_myintrez[i] != (exvec0_myint1[i] + exvec0_myint2[i]))
            test_pass = 0;

    if (test_pass)
        printf("\n Explicit Vector Add passed \n\n");
    else
        printf("Explicit Vector Add failed\n");
    unitTestFinalReport ();

    exit(0);
}
