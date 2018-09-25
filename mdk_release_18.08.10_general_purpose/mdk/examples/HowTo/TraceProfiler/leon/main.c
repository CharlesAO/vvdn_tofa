///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application Leon file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <DrvCpr.h>
#include <DrvLeon.h>
#include <dbgTracerApi.h>
#include <LocalLogEvents.h>
#include <OsDrvSvu.h>
#include "rtems_config.h"
#include "system.h"
#include <VcsHooksApi.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define DDR_BUF __attribute__((section(".ddr.data")))
#define TEST_FRAME_WIDTH    1280
#define TEST_FRAME_HEIGHT   720
#define FRAME_SIZE          (TEST_FRAME_WIDTH * TEST_FRAME_HEIGHT*3/2)
#define SHAVES_USED_FROM_LOS 6
#define KERNEL_SIZE 7


// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
rtems_id initId;
// Input frame
extern u8 lrt_inputFrame;
// LRT
extern u32 lrt_start;
// Shave
extern u32 TraceProfiller6_ApplicationStart;
extern u32 TraceProfiller7_ApplicationStart;
extern u32 TraceProfiller8_ApplicationStart;
extern u32 TraceProfiller9_ApplicationStart;
extern u32 TraceProfiller10_ApplicationStart;
extern u32 TraceProfiller11_ApplicationStart;

u32* TraceProfiller_ApplicationStart[SHAVES_USED_FROM_LOS]= {
        &TraceProfiller6_ApplicationStart,
        &TraceProfiller7_ApplicationStart,
        &TraceProfiller8_ApplicationStart,
        &TraceProfiller9_ApplicationStart,
        &TraceProfiller10_ApplicationStart,
        &TraceProfiller11_ApplicationStart,
};

DDR_BUF unsigned char out_image_los[FRAME_SIZE];

// 4: Static Local Data
// ----------------------------------------------------------------------------
static osDrvSvuHandler_t handler[6];
static swcShaveUnit_t SHAVE_NUMBER[] = {6, 7, 8, 9, 10, 11};

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

rtems_task Init(rtems_task_argument argument);
rtems_task SVU_task(rtems_task_argument argument);




rtems_task Init(rtems_task_argument argument)
{
    UNUSED(argument);

    u32 floating_atribute = RTEMS_LOCAL | RTEMS_FLOATING_POINT;
    rtems_name task_name;
    rtems_status_code status;
    UNUSED(status);
    rtems_id task1_id;
    u32 eventsOut;

    initClocksAndMemory();
    LOG_INFO("LOS Init(%d)", argument);

    // init trace logger
    // passing PROFILE_INFO=trace at compile time

    // log the start of our app
    LOG_INFO_EVENT(LOG_EVENT_MY_APP_RUN,1);

    initId = rtems_task_self();
    // Create New Task
    task_name = rtems_build_name( 'S', 'H', 'V' , 'n');

    // Task creation
    status = rtems_task_create(
            task_name, 1, RTEMS_MINIMUM_STACK_SIZE * 2,
            RTEMS_PREEMPT | RTEMS_TIMESLICE | RTEMS_INTERRUPT_LEVEL(0),
            floating_atribute, &task1_id);

    OsDrvSvuInit();
    // Start Task
    status = rtems_task_start(task1_id, SVU_task, 1);
    int pthread_setname_np(pthread_t thread, const char *name);
    pthread_setname_np(task1_id, "my_task");

    rtems_event_receive(TASK(1), RTEMS_WAIT|RTEMS_EVENT_ALL, RTEMS_NO_TIMEOUT, &eventsOut);
    // shutdown RTEMS
    rtems_shutdown_executive(0);
}


// bulk messages test
void _vcs_save_file(void * __restrict data, size_t size) {
    saveMemoryToFile((uint32_t)data, size, "buf.out");
}

void printMsg(int lvl) {
     if ( lvl != -1 ) {
          MV_LOG_LEVEL(MV_UNIT_NAME) = '0' + lvl;
      }
      LOG_TRACE("log trace\n");
      LOG_DEBUG("log debug\n");
      LOG_INFO("log info\n");
      LOG_WARNING("log warning\n");
      LOG_ERROR("log error\n");
      LOG_FATAL("log fatal\n");

}

rtems_task SVU_task(rtems_task_argument argument)
{
    UNUSED(argument);

    u32 running;

    // Start the LRT
    DrvLeonRTStartup((u32)(&lrt_start)); // Start the LeonRT application

    // test dynamic log level
    printMsg(-1); // default level
    for (int i=0; i<7; i++) {
        printMsg(i);
    }
  

    int i;
    for (i = 0; i < 6; i++)
    {
        if (OsDrvSvuOpenShave(&handler[i], SHAVE_NUMBER[i], OS_MYR_PROTECTION_SEM) == OS_MYR_DRV_SUCCESS)
    {
            OsDrvSvuResetShave(&handler[i]);
            OsDrvSvuSetAbsoluteDefaultStack(&handler[i]);
            OsDrvSvuStartShaveCC(&handler[i],(u32)TraceProfiller_ApplicationStart[i], "iiiiiii", (u32)out_image_los,(u32)&lrt_inputFrame, TEST_FRAME_WIDTH, TEST_FRAME_HEIGHT, SHAVES_USED_FROM_LOS, i, KERNEL_SIZE);
            OsDrvSvuWaitShaves(1, &handler[i], OS_DRV_SVU_WAIT_FOREVER, &running);
        }
        else {
            LOG_WARNING("[T2] cannot open shave %d\n", i);
        }
    }

    for (i = 0; i < 6; i++)
    {
        OsDrvSvuCloseShave(&handler[i]);
    }


    // test bulk messages
    {
        char byteArray[] = { 0xca, 0xfe, 0x1e, 0x1e, 0x01, 0x23, 0x45, 0x67, 0x89 };
        LOG_BULK_INFO(byteArray, sizeof(byteArray));
    }


    DrvLeonRTWaitExecution();
    //Mark the end of our application
    LOG_INFO_EVENT(LOG_EVENT_MY_APP_RUN, 0);

    // explicitly set the first prefix info
    // FL_STRn (n=1..5) is the printf-like format string
    // FL_ARGn (n=1..5) is a comma separated values for the formatted string
#ifdef FL_STR1
#undef FL_STR1
#undef FL_ARG1
#define FL_STR1 "pre%s: "
#define FL_ARG1 ,"fix"
#endif
    LOG_INFO("LeonOS finished\n");

    exit(0);
}
