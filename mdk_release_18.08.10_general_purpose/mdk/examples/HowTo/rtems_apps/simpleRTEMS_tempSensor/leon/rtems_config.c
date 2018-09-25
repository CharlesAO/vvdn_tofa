///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     RTEMS configuration Leon header
///
// 1: Includes
// ----------------------------------------------------------------------------
#include <rtems.h>
#include <rtems/bspIo.h>
#include <bsp.h>
#include "fatalExtension.h"

// 2: Defines
// ----------------------------------------------------------------------------

/* ask the system to generate a configuration table */
#define CONFIGURE_INIT

#ifndef RTEMS_POSIX_API
#define RTEMS_POSIX_API
#endif

#define CONFIGURE_MICROSECONDS_PER_TICK         1000    /* 1 millisecond */

#define CONFIGURE_TICKS_PER_TIMESLICE           10      /* 5 milliseconds */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_POSIX_INIT_THREAD_TABLE

#define  CONFIGURE_MINIMUM_TASK_STACK_SIZE      2048

#define CONFIGURE_UNIFIED_WORK_AREAS
#define CONFIGURE_UNLIMITED_OBJECTS

#define CONFIGURE_MAXIMUM_TASKS                 4

#define CONFIGURE_MAXIMUM_THREADS               8
#define CONFIGURE_MAXIMUM_POSIX_THREADS         8

#define CONFIGURE_MAXIMUM_POSIX_MUTEXES         8

#define CONFIGURE_MAXIMUM_POSIX_KEYS            8

#define CONFIGURE_MAXIMUM_SEMAPHORES            8
#define CONFIGURE_MAXIMUM_POSIX_SEMAPHORES      8

#define CONFIGURE_MAXIMUM_POSIX_MESSAGE_QUEUES  8

#define CONFIGURE_MAXIMUM_POSIX_TIMERS          4

#define CONFIGURE_MAXIMUM_TIMERS                4

#define CONFIGURE_MAXIMUM_DRIVERS               10
#define CONFIGURE_MAXIMUM_DEVICES               10

#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 10

#define CONFIGURE_MAXIMUM_USER_EXTENSIONS    1
#define CONFIGURE_INITIAL_EXTENSIONS         { .fatal = Fatal_extension }

// 3:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
// 4:  Functions (non-inline)
// ----------------------------------------------------------------------------


void *POSIX_Init( void *argument );

#include <rtems/confdefs.h>

// Program the booting clocks
// Programme the system clocks at Startup
BSP_SET_CLOCK(12000, 504000, 1, 1,
              DEFAULT_RTEMS_CSS_LOS_CLOCKS,
              DEFAULT_RTEMS_MSS_LRT_CLOCKS,
              UPA_SHAVE_L2,
              0, 0);

// Programme the  L2C at startup
BSP_SET_L2C_CONFIG(1, L2C_REPL_LRU, 0, L2C_MODE_WRITE_THROUGH, 0, NULL);




