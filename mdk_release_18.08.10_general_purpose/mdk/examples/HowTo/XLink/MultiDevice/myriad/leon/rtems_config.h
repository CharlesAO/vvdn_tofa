#ifndef _LEON_RTEMS_CONFIG_H_
#define _LEON_RTEMS_CONFIG_H_

#include "fatalExtension.h"
#ifdef MA2X8X
#include "OsDrvInit.h"
#else
#include "OsDrvCpr.h"
#endif

#if defined(__RTEMS__)

#if !defined (__CONFIG__)
#define __CONFIG__

/* ask the system to generate a configuration table */
#define CONFIGURE_INIT

#ifndef RTEMS_POSIX_API
#define RTEMS_POSIX_API
#endif

#define CONFIGURE_MICROSECONDS_PER_TICK         1000    /* 1 millisecond */

#define CONFIGURE_TICKS_PER_TIMESLICE           1      /* 50 milliseconds */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#ifndef USE_USB_VSC
#ifndef USE_LINK_JTAG
#include "usb_uart.h"
#define CONFIGURE_APPLICATION_EXTRA_DRIVERS USB_UART_DRIVER_TABLE_ENTRY
#endif
#endif

#define CONFIGURE_POSIX_INIT_THREAD_TABLE
#define CONFIGURE_MINIMUM_TASK_STACK_SIZE      (8192)
#define CONFIGURE_MAXIMUM_TASKS                 40
#define CONFIGURE_MAXIMUM_POSIX_THREADS         10
#define CONFIGURE_MAXIMUM_POSIX_MUTEXES         8
#define CONFIGURE_MAXIMUM_POSIX_KEYS            8
#define CONFIGURE_MAXIMUM_POSIX_SEMAPHORES      8
#define CONFIGURE_MAXIMUM_POSIX_MESSAGE_QUEUES  8
#define CONFIGURE_MAXIMUM_POSIX_TIMERS          6
#define CONFIGURE_MAXIMUM_TIMERS                4
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES        1
#define CONFIGURE_MAXIMUM_SEMAPHORES            32
#define CONFIGURE_MAXIMUM_DRIVERS               16
#define CONFIGURE_MAXIMUM_DEVICES               16
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 30

#define CONFIGURE_POSIX_INIT_TASKS_TABLE

#define CONFIGURE_MAXIMUM_USER_EXTENSIONS    1
#define CONFIGURE_INITIAL_EXTENSIONS         { .fatal = Fatal_extension }

#ifdef MA2X8X
#define CONFIGURE_APPLICATION_EXTRA_DRIVERS OS_DRV_INIT_TABLE_ENTRY
#endif

void *POSIX_Init (void *args);
#include <rtems/confdefs.h>

#endif // __CONFIG__

#endif // __RTEMS__

// 3:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 4:  Functions (non-inline)
// ----------------------------------------------------------------------------
// Default clock used by the App
// System Clock configuration on start-up
BSP_SET_CLOCK(DEFAULT_OSC_CLOCK_KHZ,
              PLL_DESIRED_FREQ_KHZ,
              1,
              1,
              DEFAULT_RTEMS_CSS_LOS_CLOCKS | DEV_CSS_USB,
              DEFAULT_RTEMS_MSS_LRT_CLOCKS,
              DEFAULT_UPA_CLOCKS,
              0,
              0);
// Program L2 cache behaviour
BSP_SET_L2C_CONFIG(1, DEFAULT_RTEMS_L2C_REPLACEMENT_POLICY, DEFAULT_RTEMS_L2C_LOCKED_WAYS,DEFAULT_RTEMS_L2C_MODE, 0, 0);

BSP_SET_L1C_CONFIG(1, 1);
#endif /* _LEON_RTEMS_CONFIG_H_ */

