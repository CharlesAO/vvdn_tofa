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
#include <errno.h>
#include <rtems.h>
#include <rtems/bspIo.h>
#include <UnitTestApi.h>
#include "DrvShaveL2Cache.h"
#include "OsDrvSvu.h"
#include "assert.h"
#include "rtems_config.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern u32 addshave_shaveentry;
extern u32 subshave_shaveentry;

extern u8 adddyn;
extern u8 subdyn;

extern u32 addshave_myint1;
extern u32 addshave_myint2;
extern u32 addshave_myintrez;

extern u32 subshave_myint1;
extern u32 subshave_myint2;
extern u32 subshave_myintrez;

// 4: Static Local Data
// ----------------------------------------------------------------------------
static int rc1, rc2;
static pthread_t thread1, thread2;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
void* function1(void *arg);


// 6: Functions Implementation
// ----------------------------------------------------------------------------

void *POSIX_Init (void *args)
{
    UNUSED(args);

    pthread_attr_t attr;
    rtems_status_code status;

    u32 args1[6];
    u32 args2[6];
    /* ================*/
    /* Initialize POSIX system */
    /* ================ */
    printf ("\n");
    printf ("RTEMS POSIX Started\n");  /* Initialize variables */

    unitTestInit();
    status = initClocksAndMemory();
    if(status)
        exit(status);

    OsDrvSvuInit();  // init the os shave driver

    status = pthread_attr_init(&attr);
    if(status) {
        printf("pthread_attr_init error");
        exit(status);
    }
    status = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    if(status) {
        printf("pthread_attr_setinheritsched error");
        exit(status);
    }
    status = pthread_attr_setschedpolicy(&attr, SCHED_RR);
    if(status) {
        printf("pthread_attr_setschedpolicy error");
        exit(status);
    }

    args1[0] = (u32)&adddyn;
    args1[1] = 0;//Shave id
    args1[2] = (u32)&addshave_shaveentry;
    args1[3] = (u32)&addshave_myint1;
    args1[4] = (u32)&addshave_myint2;
    args1[5] = (u32)&addshave_myintrez;


    if( (rc1=pthread_create( &thread1, &attr, function1, (void *)args1)) ){
        printf("Thread 1 creation failed: %d\n", rc1);
        exit(rc1);
    }
    else {
        printf("Thread 1 created\n");
    }

    status = pthread_join(thread1, NULL);
    if(status) {
        printf("pthread_join error!");
        exit(status);
    }

    args2[0] = (u32)&subdyn;
    args2[1] = 1;//Shave id
    args2[2] = (u32)&subshave_shaveentry;
    args2[3] = (u32)&subshave_myint1;
    args2[4] = (u32)&subshave_myint2;
    args2[5] = (u32)&subshave_myintrez;

    if( (rc2=pthread_create( &thread2, &attr, function1, (void *)args2))) {
        printf("Thread 2 creation failed: %d\n", rc2);
        exit(rc2);
    }
    else {
        printf("Thread 2 created\n");
    }

    status = pthread_join(thread2, NULL);
    if(status) {
        printf("pthread_join error!");
        exit(status);
    }

    unitTestFinalReport();
    exit(0);

}

void *function1(void *arg)
{
    UNUSED(arg);
    u32 aux_address = 0;
    u32 *aux = NULL;
    int osRetCod = OS_MYR_DRV_SUCCESS;
    osDrvSvuHandler_t chandler;

    u32 shaveDynLibAddress= *(((u32 *)arg)+0);
    u32 shaveNumber       = *(((u32 *)arg)+1);
    u32 shaveEntryAddress = *(((u32 *)arg)+2);
    u32 myint1_address    = *(((u32 *)arg)+3);
    u32 myint2_address    = *(((u32 *)arg)+4);
    u32 result_address    = *(((u32 *)arg)+5);

    osRetCod += OsDrvSvuOpenShave(&chandler, shaveNumber, OS_MYR_PROTECTION_SELF); // open shave
    if (osRetCod != OS_MYR_DRV_SUCCESS)
    {
        printf ("Open shave failed with error code: %d\n",osRetCod);
    }
    else
    {
        osRetCod += OsDrvSvuSetShaveWindowsToDefault(&chandler);
        osRetCod += OsDrvSvuSetAbsoluteDefaultStack(&chandler);
        osRetCod += OsDrvSvuLoadShvdlib((u8 *)shaveDynLibAddress, &chandler);
        osRetCod += OsDrvSvuResetShave(&chandler);

        // Set input parameters Shaves, note that the input parameters are now window relative addresses
        osRetCod += OsDrvSvuSolveShaveRelAddr(myint1_address, shaveNumber, &aux_address);
        aux = (u32 *)aux_address;
        *(aux)   = 4;
        *(aux+1) = 25;
        *(aux+2) = 313;
        *(aux+3) = 13;
        printf("Data set 1 : %ld %ld %ld %ld\n", aux[0], aux[1], aux[2], aux[3]);

        osRetCod += OsDrvSvuSolveShaveRelAddr(myint2_address, shaveNumber, &aux_address);
        aux = (u32 *)aux_address;
        *(aux)   = 4;
        *(aux+1) = 30;
        *(aux+2) = 400;
        *(aux+3) = 500;
        printf("Data set 2 : %ld %ld %ld %ld\n", aux[0], aux[1], aux[2], aux[3]);

        osRetCod += OsDrvSvuRunShave(&chandler, shaveEntryAddress);  //starting shave with the entry point specified at address ptr+2

        // check the result
        osRetCod += OsDrvSvuSolveShaveRelAddr(result_address,  shaveNumber, &aux_address);
        aux = (u32 *)aux_address;
        printf("res  : %ld %ld %ld %ld\n", aux[0], aux[1], aux[2], aux[3]);
        if (osRetCod != OS_MYR_DRV_SUCCESS)
        {
            printf ("OsDrvSvu... function was not successful, error code  %d", osRetCod);
        }
    }
    return NULL; // just so the compiler thinks we returned something
}


        // these must be resolved to absolute addresses so Leon may access.
