///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     RTEMS configuration Leon header
///
#ifndef RTEMS_CONFIG_H_
#define RTEMS_CONFIG_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include "app_config.h"
#include "fatalExtension.h"

#if defined(__RTEMS__)

#if !defined (__CONFIG__)
#define __CONFIG__

// 2: Defines
// ----------------------------------------------------------------------------

/* ask the system to generate a configuration table */
#define CONFIGURE_INIT

#define CONFIGURE_MICROSECONDS_PER_TICK         1000    /* 1 millisecond */

#define CONFIGURE_TICKS_PER_TIMESLICE           10      /* 10 milliseconds */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_MINIMUM_TASK_STACK_SIZE      4096

#define CONFIGURE_MAXIMUM_TASKS                 15

#define CONFIGURE_MAXIMUM_TIMERS                4

#define CONFIGURE_MAXIMUM_SEMAPHORES 15
#define CONFIGURE_MAXIMUM_DRIVERS           8
#define CONFIGURE_MAXIMUM_DEVICES 8

#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 20

#define CONFIGURE_APPLICATION_NEEDS_ZERO_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_NULL_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_STUB_DRIVER

#define CONFIGURE_MAXIMUM_USER_EXTENSIONS    1
#define CONFIGURE_INITIAL_EXTENSIONS         { .fatal = Fatal_extension }

// 3:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
// 4:  Functions (non-inline)
// ----------------------------------------------------------------------------


rtems_task Init (rtems_task_argument argumentx);

#include <rtems/confdefs.h>

#endif // __CONFIG__

#endif // __RTEMS__

// Program the booting clocks
// System Clock configuration on start-up
BSP_SET_CLOCK(DEFAULT_OSC_CLOCK_KHZ, DEFAULT_APP_CLOCK_KHZ, 1, 1,
              (DEFAULT_RTEMS_CSS_LOS_CLOCKS | DEV_CSS_SPI0),
              DEFAULT_RTEMS_MSS_LRT_CLOCKS,
              0, 0, 0);

// Program the  L2C at startup
BSP_SET_L2C_CONFIG(1, L2C_REPL_LRU, 0, L2C_MODE_WRITE_THROUGH, 0, NULL);

#endif // LEON_RTEMS_CONFIG_H_
