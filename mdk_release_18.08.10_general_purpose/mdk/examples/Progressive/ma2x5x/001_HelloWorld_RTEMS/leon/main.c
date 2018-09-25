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
#include <rtems.h>
#include <rtems/bspIo.h>
#include "rtems_config.h"
#include "mv_types.h"

//4: Static Local Data
// ----------------------------------------------------------------------------
static int rc1, rc2;
static int counters[2] ={0, 0};
static pthread_t thread1, thread2;
static sem_t sem;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
void *functionC(void *led);
void *functionD(void *led);

// 6: Functions Implementation
// ----------------------------------------------------------------------------
void *POSIX_Init(void *args)
{
    UNUSED(args);

    pthread_attr_t attr;
    int sc;

    sc = initClocksAndMemory();
    if (sc)
        exit(sc);

    printf("\n");
    printf("RTEMS POSIX Started\n"); /* initialise variables */

    sc = pthread_attr_init(&attr);
    if (sc)
    {
        printf("pthread_attr_init error");
        exit(sc);
    }
    sc = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    if (sc)
    {
        printf("pthread_attr_setinheritsched error");
        exit(sc);
    }
    sc = pthread_attr_setschedpolicy(&attr, SCHED_RR);
    if (sc)
    {
        printf("pthread_attr_setschedpolicy error");
        exit(sc);
    }
    sc = sem_init(&sem, 0, 0);
    if (sc == -1)
    {
        printf("sem_init error\n");
        exit(sc);
    }

    rc1 = pthread_create(&thread1, &attr, &functionC, &counters[0]);
    if (rc1)
    {
        printf("Thread 1 creation failed: %d\n", rc1);
        exit(rc1);
    }
    else
    {
        printf("Thread 1 created\n");
    }

    rc2 = pthread_create(&thread2, &attr, &functionD, &counters[1]);
    if (rc2)
    {
        printf("Thread 2 creation failed: %d\n", rc2);
        exit(rc2);
    }
    else
    {
        printf("Thread 2 created\n");
    }

    sc = pthread_join(thread1, NULL);
    if (sc)
    {
        printf("pthread_join error!");
        exit(sc);
    }

    sc = pthread_join(thread2, NULL);
    if (sc)
    {
        printf("pthread_join error!");
        exit(sc);
    }

    exit(0);
}

void *functionC(void *cnt)
{
    volatile int *cntInt = (int *) cnt;

    for (;;)
    {
        *cntInt = *cntInt + 1;
        printf("C: %d\n", *cntInt);
    }
}

void *functionD(void *cnt)
{
    volatile int *cntInt = (int *) cnt;

    for (;;)
    {
        *cntInt = *cntInt + 1;
        printf("D: %d\n", *cntInt);
    }
}


