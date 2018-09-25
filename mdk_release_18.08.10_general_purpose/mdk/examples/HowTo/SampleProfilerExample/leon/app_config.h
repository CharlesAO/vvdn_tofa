
///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon header
///

#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_

#define L2CACHE_CFG        (SHAVE_L2CACHE_NORMAL_MODE)

#define DEFAULT_APP_CLOCK_KHZ       300000
#define DEFAULT_OSC_CLOCK_KHZ       12000

#if defined(__RTEMS__)

#include <rtems/bspIo.h>
#include "fatalExtension.h"

#if !defined (__CONFIG__)
#define __CONFIG__

/* ask the system to generate a configuration table */
#define CONFIGURE_INIT

#ifndef RTEMS_POSIX_API
#define RTEMS_POSIX_API
#endif

#define CONFIGURE_MICROSECONDS_PER_TICK         100    /* 1 millisecond */

#define CONFIGURE_TICKS_PER_TIMESLICE           10      /* 10 milliseconds */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define  CONFIGURE_MINIMUM_TASK_STACK_SIZE      4096
#define CONFIGURE_MAXIMUM_TASKS                 4

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_EXTRA_TASK_STACKS         (5 * RTEMS_MINIMUM_STACK_SIZE)

#define CONFIGURE_MAXIMUM_TIMERS                4

#define CONFIGURE_MAXIMUM_MUTEXES               4
#define CONFIGURE_MAXIMUM_SEMAPHORES            4



#define CONFIGURE_MAXIMUM_USER_EXTENSIONS    1
#define CONFIGURE_INITIAL_EXTENSIONS         { .fatal = Fatal_extension }


void *POSIX_Init (void *args);

#include <rtems/confdefs.h>

#endif // __CONFIG__

#endif // __RTEMS__

#endif // _APP_CONFIG_H_
