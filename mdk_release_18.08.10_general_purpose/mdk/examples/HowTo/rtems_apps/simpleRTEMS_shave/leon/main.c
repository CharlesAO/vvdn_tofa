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
#include <rtems/bspIo.h>
#include "OsDrvSvu.h"
#include "swcFrameTypes.h"
#include "assert.h"
#include "rtems_config.h"


// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define CMX_FRAMES __attribute__((section(".cmx.data")))
#define FRAME_WIDTH    80
#define FRAME_HEIGHT   60

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern u8  inputFrame;
extern u32 brthYASM6_start;
extern u32 brthYASM7_start;

// 4: Static Local Data
// ----------------------------------------------------------------------------
static u8 SHAVE_NUMBER[2] = {6, 7};
static int rc1, rc2;
static pthread_t thread1, thread2;
static sem_t sem1, sem2;
static u8 CMX_FRAMES outputFrame[FRAME_WIDTH * FRAME_HEIGHT * 3 / 2];
static frameSpec   testFrameSpec;
static frameBuffer inBuffer;
static frameBuffer outBuffer;
static osDrvSvuHandler_t handler[2];

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
void* function1(void *unused);
void* function2(void *unused);
void* handler1(u32 arg);
void* handler2(u32 arg);
//void swcStartShaveAsync_rtems(u32 shave_nr, u32 entry_point, rtems_interrupt_handler function);

// 6: Functions Implementation
// ----------------------------------------------------------------------------

// Init
void *POSIX_Init (void *args)
{
    UNUSED(args);

    pthread_attr_t attr;

    initClocksAndMemory();
    OsDrvSvuInit();

    printf ("\n");
    printf ("RTEMS POSIX Started\n");

    if(pthread_attr_init(&attr) !=0) {
        printf("pthread_attr_init error");
    }
    if(pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED) != 0) {
        printf("pthread_attr_setinheritsched error");
    }
    if(pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0) {
        printf("pthread_attr_setschedpolicy error");
    }

    if(sem_init(&sem1, 0, 0) == -1) {
        printf("sem_init error\n");
    }
    if(sem_init(&sem2, 0, 0) == -1) {
        printf("sem_init error\n");
    }

    if( (rc1=pthread_create( &thread1, &attr, &function1, NULL)) ){
        printf("Thread 1 creation failed: %d\n", rc1);
    }
    else {
        printf("Thread 1 created\n");
    }
    pthread_join(thread1, NULL);
    if( (rc2=pthread_create( &thread2, &attr, &function2, NULL))) {
        printf("Thread 2 creation failed: %d\n", rc2);
    }
    else {
        printf("Thread 2 created\n");
    }

    pthread_join(thread2, NULL);
    OsDrvSvuCloseShave(&handler[0]);
    OsDrvSvuCloseShave(&handler[1]);

    exit(0);
}

void InitTestBuffers(int width, int height)
{
   testFrameSpec.width = width;
    testFrameSpec.height = height;
    testFrameSpec.stride = width;
    testFrameSpec.type = YUV420p;
    testFrameSpec.bytesPP = 1;

    inBuffer.spec = testFrameSpec;
    inBuffer.p1 = (u8*)(&inputFrame);
    inBuffer.p2 = (u8*)((u32)(&inputFrame) + width * height);
    inBuffer.p3 = (u8*)((u32)(&inputFrame) + width * height + width * height / 4);

    outBuffer.spec = testFrameSpec;
    outBuffer.p1 = outputFrame;
    outBuffer.p2 = outputFrame + width * height;
    outBuffer.p3 = outputFrame + width * height + width * height / 4;

    return;
}

void *function1(void *unused)
{
    UNUSED(unused);

    u32 running;

    InitTestBuffers(FRAME_WIDTH, FRAME_HEIGHT);

    if (OsDrvSvuOpenShave(&handler[0], SHAVE_NUMBER[0], OS_MYR_PROTECTION_SEM) == OS_MYR_DRV_SUCCESS)
    {

        OsDrvSvuResetShave(&handler[0]);
        OsDrvSvuSetAbsoluteDefaultStack(&handler[0]);

        OsDrvSvuStartShaveCC(&handler[0],(u32)&brthYASM6_start,  "iii", ((u32)&inBuffer), ((u32)&outBuffer), 0);
        OsDrvSvuWaitShaves(1, &handler[0], OS_DRV_SVU_WAIT_FOREVER, &running);
        printf("Function 1\n");
    }
    else {
        printf("[T2] cannot open shave %d\n", SHAVE_NUMBER[0]);
    }

    pthread_exit((void*)0);
    return NULL; //just so the compiler thinks we returned something
}

void *function2(void *unused)
{
    UNUSED(unused);

    u32 running;

    InitTestBuffers(FRAME_WIDTH, FRAME_HEIGHT);

    if (OsDrvSvuOpenShave(&handler[1], SHAVE_NUMBER[1], OS_MYR_PROTECTION_SEM) == OS_MYR_DRV_SUCCESS)
    {

        OsDrvSvuResetShave(&handler[1]);
        OsDrvSvuSetAbsoluteDefaultStack(&handler[1]);

        OsDrvSvuStartShaveCC(&handler[1],(u32)&brthYASM7_start,  "iii", ((u32)&inBuffer), ((u32)&outBuffer), 1);
        OsDrvSvuWaitShaves(1, &handler[1], OS_DRV_SVU_WAIT_FOREVER, &running);
        printf("Function 2\n");
    }
    else {
        printf("[T2] cannot open shave %d\n", SHAVE_NUMBER[1]);
    }

    pthread_exit((void*)0);
    return NULL; //just so the compiler thinks we returned something
}



