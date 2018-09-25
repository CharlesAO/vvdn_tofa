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
#include <rtems.h>
#include <rtems/bspIo.h>
#include "rtems_config.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
/* Sections decoration is require here for downstream tools */
static int rc1, rc2;
static int counters[2]={0,0};
static pthread_t thread1, thread2;
static sem_t sem;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
void *functionC(void *led);
void *functionD(void *led);

// 6: Functions Implementation
// ----------------------------------------------------------------------------
void *POSIX_Init (void *args)
{
    UNUSED(args);

    pthread_attr_t attr;

    initClocksAndMemory();
    printf ("\n");
    printf ("RTEMS POSIX Started\n");  /* initialise variables */

    if(pthread_attr_init(&attr) !=0) {
        printf("pthread_attr_init error\n");
    }
    if(pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED) != 0) {
        printf("pthread_attr_setinheritsched error\n");
    }
    if(pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0) {
        printf("pthread_attr_setschedpolicy error\n");
    }

    if(sem_init(&sem, 0, 0) == -1) {
        printf("sem_init error\n");
    }

    if ((rc1=pthread_create( &thread1, &attr, &functionC,&counters[0]))) {
        printf("Thread 1 creation failed: %d\n", rc1);
    }
    else {
        printf("Thread 1 created\n");
    }

    if((rc2=pthread_create( &thread2, &attr, &functionD,&counters[1]))) {
        printf("Thread 2 creation failed: %d\n", rc2);
    }
    else {
        printf("Thread 2 created\n");
    }

    if(pthread_join( thread1, NULL)) {
        printf("pthread_join error!\n");
    }
    if(pthread_join( thread2, NULL)) {
        printf("pthread_join error!\n");
    }

    exit(0);
    return NULL;
}

void *functionC(void *cnt)
{
    volatile int *cntInt =(int *)cnt;

    for (;;) {
        *cntInt=*cntInt + 1;
        printf("C: %d\n", *cntInt);
    }
}

void *functionD(void *cnt)
{
    volatile int *cntInt =(int *)cnt;

    for(;;){
        *cntInt=*cntInt + 1;
        printf("D: %d\n", *cntInt);
    }
}


