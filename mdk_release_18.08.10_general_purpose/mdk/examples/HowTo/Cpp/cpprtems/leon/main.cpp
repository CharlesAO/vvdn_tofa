///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///
// 1: Includes
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <rtems.h>
#include <pthread.h>
#include <sched.h>
#include <fcntl.h>
#include <mv_types.h>
#include <mvMacros.h>
#include <rtems/cpuuse.h>
#include "bsp.h"
#include <DrvCpr.h>
#include "Geometry.h"
#include <UnitTestApi.h>

#include "app_config.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
int rc1, rc2;
Geometry globalobj;
pthread_t thread1;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
void *functionC(void *ptr);

// 6: Functions Implementation
// ----------------------------------------------------------------------------
extern "C" void *POSIX_Init (void *args)
{
    UNUSED(args);

    pthread_attr_t attr;
    s32 sc;

    sc = initClocksAndMemory();
    if(sc)
        exit(sc);
    unitTestInit ();
    printf ("\n");
    printf ("RTEMS POSIX Started\n");  /* initialise variables */

    sc = pthread_attr_init(&attr);
    if(sc) {
        printf("pthread_attr_init error");
        exit(sc);
    }
    sc = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    if(sc) {
        printf("pthread_attr_setinheritsched error");
        exit(sc);
    }

    sc = pthread_attr_setschedpolicy(&attr, SCHED_RR);
    if(sc) {
        printf("pthread_attr_setschedpolicy error");
        exit(sc);
    }

    if ((rc1=pthread_create( &thread1, &attr, &functionC, &globalobj))) {
        printf("Thread 1 creation failed: %d\n", rc1);
        exit(rc1);
    }
    else {
        printf("Thread 1 created\n");
    }

    if(pthread_join( thread1, NULL)) {
        printf("pthread_join error!\n");
    }

    exit(0);
}

void *functionC(void *ptr)
{
    Geometry *obj = (Geometry *)ptr;
    for(int i=0;i<10;i++){
        obj->printme();
    }
    unitTestFinalReport ();

    return NULL; // just so the compiler thinks we returned something
}
