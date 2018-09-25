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
#include <DrvLeonL2C.h>
#include <rtems.h>
#include <rtems/bspIo.h>
#include <DrvCpr.h>
#include <DrvLeon.h>
#include <UnitTestApi.h>
#include "rtems_config.h"


// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define MTRR_SUPPORT_MYFUNC_1(x) __attribute__((section(".cmx.text.myfunc1"))) x
#define MTRR_SUPPORT_MYFUNC_2(x) __attribute__((section(".cmx.text.myfunc2"))) x

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern u32 MYFUNC1_START, MYFUNC1_STOP;
extern u32 MYFUNC2_START, MYFUNC2_STOP;

// 4: Static Local Data
// ----------------------------------------------------------------------------
static int rc1, rc2;
static int counters[2]={0,0};
static pthread_t thread1, thread2;
static  sem_t sem;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
void MTRR_SUPPORT_MYFUNC_1(myfunc1)();
void MTRR_SUPPORT_MYFUNC_2(myfunc2)();

void *functionC(void *led);
void *functionD(void *led);

// 6: Functions Implementation
// ----------------------------------------------------------------------------
void *POSIX_Init (void *args)
{
    UNUSED(args);

    pthread_attr_t attr;
    s32 sc;

    unitTestInit ();
    sc = initClocksAndMemory();
    if(sc)
        exit(sc);
    printf ("\n");
    printf ("RTEMS POSIX Started\n");  /* initialise variables */

    u32 funcsize1 = (&MYFUNC1_STOP -  &MYFUNC1_START) * 4;
    u32 funcsize2 = (&MYFUNC2_STOP -  &MYFUNC2_START) * 4;
    printf("Calculated sizes in bytes: %ld %ld\n", funcsize1, funcsize2);

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

    sc = sem_init(&sem, 0, 0);
    if(sc == -1) {
        printf("sem_init error\n");
        exit(sc);
    }
       //LL2C_UNCACHED LL2C_WRITE_THROUGH
    sc = DrvLL2CSetMTRRfunc(myfunc1, funcsize1*2, LL2C_UNCACHED, 0);
    if(sc)
        exit(sc);
    sc = DrvLL2CSetMTRRfunc(myfunc2, funcsize2*2, LL2C_UNCACHED, 0);
    if(sc)
        exit(sc);

    if ((rc1=pthread_create( &thread1, &attr, functionC,&counters[0]))) {
        printf("Thread 1 creation failed: %d\n", rc1);
        exit(rc1);
    }
    else {
        printf("Thread 1 created\n");
    }

    if((rc2=pthread_create( &thread2, &attr, functionD,&counters[1]))) {
        printf("Thread 2 creation failed: %d\n", rc2);
        exit(rc2);
    }
    else {
        printf("Thread 2 created\n");
    }

    sc = pthread_join( thread1, NULL);
    if(sc) {
        printf("pthread_join error!");
        exit(sc);
    }
    sc = pthread_join( thread2, NULL);
    if(sc) {
        printf("pthread_join error!");
        exit(sc);
    }

    unitTestFinalReport();
    exit(0);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing" // this is for the last printf parameter below
void MTRR_SUPPORT_MYFUNC_1(myfunc1)(){

   float val;

       val = 3.142423f * 24.0f/34.0f *123.0f - 56.7f;

    printf("val: 0x%lx\n", *(u32 *)&val);

}
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing" // this is for the last printf parameter below
void MTRR_SUPPORT_MYFUNC_2(myfunc2)(){
    float val;
    //TODO: these constants are in rodata section, change this function
    val = 3.142423f * 24.0f/4.1f *123.0f - 56.7f;

    printf("val: 0x%lx\n", *(u32 *)&val);
}
#pragma GCC diagnostic pop

void *functionC(void *cnt)
{
    UNUSED(cnt);

    u32 i;
    for (i=0;i<50;i++)
    {
        myfunc1();
    }
    return NULL; // just so the compiler thinks we returned something
}

void *functionD(void *cnt)
{
    UNUSED(cnt);

    u32 i;
    for (i=0;i<50;i++)
    {
        myfunc2();
    }
    return NULL; // just so the compiler thinks we returned something
}


