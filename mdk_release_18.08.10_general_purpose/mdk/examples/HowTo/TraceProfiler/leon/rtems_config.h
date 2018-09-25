///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     RTEMS configuration Leon header
///
#ifndef LEON_RTEMS_CONFIG_H_
#define LEON_RTEMS_CONFIG_H_


#ifndef _RTEMS_CONFIG_H_
#define _RTEMS_CONFIG_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include "app_config.h"
#include "fatalExtension.h"

// 2: Defines
// ----------------------------------------------------------------------------

#if defined(__RTEMS__)

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
#define CONFIGURE_MAXIMUM_SEMAPHORES            4

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_EXTRA_TASK_STACKS             (5 * RTEMS_MINIMUM_STACK_SIZE)

#define CONFIGURE_MAXIMUM_TIMERS                4

#define CONFIGURE_MAXIMUM_USER_EXTENSIONS       1


#define CONFIGURE_INITIAL_EXTENSIONS            { .fatal = Fatal_extension }

// 3:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
// 4:  Functions (non-inline)
// ----------------------------------------------------------------------------


void *POSIX_Init (void *args);

#include <rtems/confdefs.h>

#endif // __CONFIG__

#endif // __RTEMS__

// Set the booting clocks
// Set the system clocks at Startup
BSP_SET_CLOCK(12000, 266000, 1, 1, DEFAULT_CORE_CSS_DSS_CLOCKS, APP_MSS_CLOCKS, APP_UPA_CLOCKS, 0, 0);

// Set L2 cache behaviour
BSP_SET_L2C_CONFIG(0, DEFAULT_RTEMS_L2C_REPLACEMENT_POLICY, DEFAULT_RTEMS_L2C_LOCKED_WAYS,DEFAULT_RTEMS_L2C_MODE, 0, 0);
#endif // _RTEMS_CONFIG_H_

#endif // LEON_RTEMS_CONFIG_H_
