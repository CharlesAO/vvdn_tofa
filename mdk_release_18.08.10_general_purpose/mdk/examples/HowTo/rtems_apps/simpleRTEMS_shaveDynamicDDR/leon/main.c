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
#include <errno.h>
#include <rtems.h>
#include <rtems/bspIo.h>
#include "OsDrvSvu.h"
#include <DrvDdr.h>
#include <DrvTimer.h>
#include <OsDrvShaveL2Cache.h>
#include <UnitTestApi.h>
#include "assert.h"
#include "rtems_config.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define SHAVE_MAX_NO  2

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
static int rc1;
static pthread_t thread1, thread2;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
void* function1(void *arg);

// 6: Functions Implementation
// ----------------------------------------------------------------------------

typedef struct
{
    u16            shaves[SHAVE_MAX_NO];
    u16            shaveL2cPartId[MAX_SHAVE_L2C_PARTITIONS];
    u32            noShaves;
    u32            appImageAddr;
    u32            entryPoint;
    u32            inputDataAddr[2];
    u32            outputDataAddr;
} ShaveTask;

ShaveTask addTask =
{
    { 0, 1 },
    { 0, 1 },
	SHAVE_MAX_NO,
    (u32)&adddyn,
    (u32)&addshave_shaveentry,
    {
        (u32)&addshave_myint1, (u32)&addshave_myint2
    },
    (u32)&addshave_myintrez
};

ShaveTask subTask =
{
    { 2, 3 },
    { 2, 3 },
	SHAVE_MAX_NO,
    (u32)&subdyn,
    (u32)&subshave_shaveentry,
    {
        (u32)&subshave_myint1, (u32)&subshave_myint2
    },
    (u32)&subshave_myintrez
};

void createThread(ShaveTask* task, pthread_t* thread, char* threadName)
{
    pthread_attr_t attr;
    s32 sc;

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

    if( (rc1=pthread_create(thread, &attr, function1, (void*)task) ) ){
        printf("%s creation failed: %d\n", threadName, rc1);
        exit(rc1);
    }
    else {
        printf("%s created\n", threadName);
    }
}

void *POSIX_Init (void *args)
{
    UNUSED(args);

    int retVal;
    u32 i;
    OS_MYRIAD_DRIVER_STATUS_CODE sc;

    unitTestInit();

    retVal = initClocksAndMemory();

    retVal = InitShaveL2C();

    //Declare once all the usable L2 cache partitions, same size (16KB)
    for(i=0; i < MAX_SHAVE_L2C_PARTITIONS; i++)
    {
     	sc = OsDrvShaveL2CGetPartition(SHAVEPART16KB, NULL);  //the shave - partition mapping is statically declared, so no need to obtain a partition id
     	if ( sc != OS_MYR_DRV_SUCCESS )
         {
         	printf("Failed to declare a partition; application exited\n");
         	exit(-sc);
         }
    }

    //configure the window for add shaves
    for(i=0; i < addTask.noShaves; i++)
    {
		sc = OsDrvShaveL2CSetWindowPartition(addTask.shaves[i], SHAVEL2CACHEWIN_A, addTask.shaveL2cPartId[i]);     //Assign the partitions defined to each shave
		if ( sc != OS_MYR_DRV_SUCCESS )
		 {
			printf("Failed to set the WIN add L2C partition %ld; application exited\n", i);
			exit(-sc);
		 }
    }

    //configure the window for sub shaves
    for(i=0; i < subTask.noShaves; i++)
    {
		sc = OsDrvShaveL2CSetWindowPartition(subTask.shaves[i], SHAVEL2CACHEWIN_A, subTask.shaveL2cPartId[i]);     //Assign the statically defined partitions to each shave
		if ( sc != OS_MYR_DRV_SUCCESS )
		 {
			printf("Failed to set the WIN add L2C partition %ld; application exited\n", i);
			exit(-sc);
		 }
    }

    // Allocate the declared shave L2 cache set partitions
    sc = OsDrvShaveL2CacheAllocateSetPartitions();
	if ( sc != OS_MYR_DRV_SUCCESS )
	 {
		printf("Failed to set the WIN add L2C partition %ld; application exited\n", i);
		exit(-sc);
	 }

    /* ================*/
    /* Initialize POSIX system */
    /* ================ */
    printf ("\n");
    printf ("RTEMS POSIX Started\n");  /* Initialize variables */

    retVal = OsDrvSvuInit();  // init the os shave driver

    createThread(&addTask, &thread1, "Add thread");
    createThread(&subTask, &thread2, "Sub thread");

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    unitTestFinalReport();
    exit(retVal);
}

void *function1(void *arg)
{
    int osDrvStat = OS_MYR_DRV_SUCCESS;

    u32 i;
    u32 aux_address;
    const u32* aux_in;
    u32* aux_out;
    ShaveTask* task = (ShaveTask*)arg;
    osDrvSvuHandler_t shvhand;
    u32 runningShaves;

    for (i = 0; (i < task->noShaves) && (osDrvStat == OS_MYR_DRV_SUCCESS); i++)
    {
        if ((osDrvStat += OsDrvSvuOpenShave(&shvhand,task->shaves[i], OS_MYR_PROTECTION_SEM)) != OS_MYR_DRV_SUCCESS)
        {
             printf("Open fail shave number: %u !!\n With error code:%d\n",task->shaves[i],osDrvStat);
        }
        else
        {
           osDrvStat += OsDrvSvuSetAbsoluteDefaultStack(&shvhand);
           osDrvStat += OsDrvSvuSetShaveWindowsToDefault(&shvhand);
           osDrvStat += OsDrvSvuLoadShvdlib((u8 *)task->appImageAddr, &shvhand);
           osDrvStat += OsDrvSvuResetShave(&shvhand);

           // Set input parameters Shaves, note that the input parameters are now window relative addresses
           // these must be resolved to absolute addresses so Leon may access.

           osDrvStat += OsDrvSvuSolveShaveRelAddr(task->inputDataAddr[0], task->shaves[i], &aux_address);
           aux_out = (u32 *)aux_address;
           aux_out[0] = 4;
           aux_out[1] = 25;
           aux_out[2] = 313;
           aux_out[3] = 13;
           printf("S%u Data set 1 at address 0x%lX : %ld %ld %ld %ld\n", task->shaves[i], aux_address, aux_out[0], aux_out[1], aux_out[2], aux_out[3]);

           osDrvStat += OsDrvSvuSolveShaveRelAddr(task->inputDataAddr[1], task->shaves[i], &aux_address);
           aux_out = (u32 *)aux_address;
           aux_out[0] = 4;
           aux_out[1] = 30;
           aux_out[2] = 400;
           aux_out[3] = 500;

           printf("S%u Data set 2 at address 0x%lX : %ld %ld %ld %ld\n", task->shaves[i], aux_address, aux_out[0], aux_out[1], aux_out[2], aux_out[3]);

           printf("S%u Start at addr:0x%lX \n", task->shaves[i], task->entryPoint);

           // invalidate shave partition before starting the shave
           (void)OsDrvShaveL2CachePartitionInvalidate(task->shaveL2cPartId[i]);

           osDrvStat += OsDrvSvuStartShaveCC(&shvhand, (u32)task->entryPoint,"i",i);
        }
    }

    if (osDrvStat == OS_MYR_DRV_SUCCESS)
    {
       for (i = 0; i < task->noShaves; i++)
       {
          osDrvStat += OsDrvSvuWaitShaves(i, &shvhand, OS_DRV_SVU_WAIT_FOREVER, &runningShaves);

          // Check the result, but flush and invalidate the shave L2 cache partition
          (void)OsDrvShaveL2CachePartitionFlush(task->shaveL2cPartId[i],PERFORM_INVALIDATION);

          // Read the results from ddr
          osDrvStat += OsDrvSvuSolveShaveRelAddr((u32)task->outputDataAddr, task->shaves[i], &aux_address);
          aux_in = (u32 *)aux_address;
          printf("S %u res from address 0x%lX: %ld %ld %ld %ld\n", task->shaves[i], aux_address, aux_in[0], aux_in[1], aux_in[2], aux_in[3]);
       }
    }
    return NULL;
}


