///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Show how to use timer to measure Leon cycles
///
// 1: Includes
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <mv_types.h>
#include <OsDrvTimer.h>
#include <OsDrvSvu.h>
#include <rtems.h>
#include <rtems/bspIo.h>
#include <DrvCpr.h>
#include "rtems_config.h"
#include <UnitTestApi.h>

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define SHAVE_USED 0


// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// Global Data (Only if absolutely necessary)
extern u32 checkingCycles0_main;
extern volatile int checkingCycles0_myint1[4];
extern volatile int checkingCycles0_myint2[4];
extern volatile int checkingCycles0_myintrez[4];

// 4: Static Local Data
// ----------------------------------------------------------------------------
static osDrvSvuHandler_t handler;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------
void *POSIX_Init (void *args)
{
    UNUSED(args);

    tyTimeStamp  timer_data;
    u64          cycles_elapsed;
    u32 running;
    s32 sc;

    sc = initClocksAndMemory();
    if(sc)
        exit(sc);
    unitTestInit ();
    printf("RTEMS Started.\n");
    printf("Measuring execution time...\n");

    // Set input parameters Shaves
    checkingCycles0_myint1[0] = 1;
    checkingCycles0_myint1[1] = 2;
    checkingCycles0_myint1[2] = 3;
    checkingCycles0_myint1[3] = 4;

    checkingCycles0_myint2[0] = 4;
    checkingCycles0_myint2[1] = 3;
    checkingCycles0_myint2[2] = 2;
    checkingCycles0_myint2[3] = 1;


    // Initialise timer
    sc = OsDrvTimerInit();
    if(sc)
        exit(sc);
    sc = OsDrvSvuInit();
    if(sc)
        exit(sc);

    // start shave and wait for result
    sc = OsDrvSvuOpenShave(&handler, SHAVE_USED, OS_MYR_PROTECTION_SEM);
    if (sc == OS_MYR_DRV_SUCCESS)
    {

        sc = OsDrvTimerStartTicksCount(&timer_data);
        if(sc)
            exit(sc);
        sc = OsDrvSvuResetShave(&handler);
        if(sc)
            exit(sc);
        sc = OsDrvSvuSetAbsoluteDefaultStack(&handler);
        if(sc)
            exit(sc);

        sc = OsDrvSvuStartShave(&handler,(u32)&checkingCycles0_main);
        if(sc)
            exit(sc);
        sc = OsDrvSvuWaitShaves(1, &handler, OS_DRV_SVU_WAIT_FOREVER, &running);
        if(sc)
            exit(sc);
        // Measure number of elapsed cycles
        sc = OsDrvTimerGetElapsedTicks(&timer_data, &cycles_elapsed);
        if(sc)
            exit(sc);
        sc = OsDrvSvuCloseShave(&handler);
        if(sc)
            exit(sc);
    }
    else {
        printf("[T2] cannot open shave %d\n", SHAVE_USED);
        exit(sc);
    }


    printf("\nLeon Cycles Elapsed %lu \n", (u32)(cycles_elapsed));
    unitTestFinalReport ();

    exit(0);
}
