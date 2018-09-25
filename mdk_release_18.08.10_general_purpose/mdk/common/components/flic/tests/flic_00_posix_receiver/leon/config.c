///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     RTEMS configuration
///

#include <rtems.h>
#include <DrvCpr.h>
#include <fatalExtension.h>

#define APP_UPA_CLOCKS  (DEV_UPA_SHAVE_L2 | \
                         DEV_UPA_CDMA     | \
                         DEV_UPA_CTRL      )

// ask the system to generate a configuration table
#define CONFIGURE_INIT

#ifndef RTEMS_POSIX_API
#define RTEMS_POSIX_API
#endif

#define CONFIGURE_MICROSECONDS_PER_TICK         1000    // 1 millisecond
#define CONFIGURE_TICKS_PER_TIMESLICE           10      // 10 milliseconds

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_POSIX_INIT_THREAD_TABLE

#define CONFIGURE_MINIMUM_TASK_STACK_SIZE      2048

#define CONFIGURE_MAXIMUM_POSIX_THREADS         10
#define CONFIGURE_MAXIMUM_POSIX_MUTEXES          2
#define CONFIGURE_MAXIMUM_POSIX_KEYS             2
#define CONFIGURE_MAXIMUM_POSIX_EVENTS           2
#define CONFIGURE_MAXIMUM_POSIX_SEMAPHORES      10
#define CONFIGURE_MAXIMUM_POSIX_MESSAGE_QUEUES   4
#define CONFIGURE_MAXIMUM_POSIX_TIMERS           2

#define CONFIGURE_MAXIMUM_USER_EXTENSIONS        1
#define CONFIGURE_INITIAL_EXTENSIONS { .fatal = Fatal_extension }

void *POSIX_Init (void *args);

#include <rtems/confdefs.h>

#ifndef DEFAULT_APP_CLOCK_KHZ
#define DEFAULT_APP_CLOCK_KHZ 600000
#endif

// Set the booting clocks
// Set the system clocks at startup
#ifdef MA2X8X
BSP_SET_CLOCK(24000, DEFAULT_APP_CLOCK_KHZ, 1, 1, DEFAULT_RTEMS_CSS_LOS_CLOCKS, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0);
#else
BSP_SET_CLOCK(12000, DEFAULT_APP_CLOCK_KHZ, 1, 1, DEFAULT_RTEMS_CSS_LOS_CLOCKS, DEFAULT_RTEMS_MSS_LRT_CLOCKS, APP_UPA_CLOCKS, 0, 0);
#endif

// Set the L2C at startup
BSP_SET_L2C_CONFIG(0, L2C_REPL_LRU, 0, L2C_MODE_COPY_BACK, 0, NULL);
