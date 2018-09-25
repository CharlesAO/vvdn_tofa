///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon header
///

#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_

#define DEFAULT_APP_CLOCK_KHZ		600000
#ifdef MA2X8X
#define DEFAULT_OSC_CLOCK_KHZ		24000
#else
#define DEFAULT_OSC_CLOCK_KHZ		12000
#endif
#define CLOCK_DIVIDER_NOMINATOR     1
#define CLOCK_DIVIDER_DENOMINATOR   1
#define L2CACHE_NORMAL_MODE 		(0x6)  // In this mode the L2Cache acts as a cache for the DRAM
#define L2CACHE_CFG     		    (L2CACHE_NORMAL_MODE)
#define BIGENDIANMODE   		    (0x01000786)

#define NUMTASKS 3
#define TASK(i) (1<<(31-(i)))
#ifdef MA2X8X
#define GPIO_ID  31  // (camB_pin17 on mv235)
#else
#define GPIO_ID  81  // (DIP SWITCH8 on mv182/mv212)
#endif

#define LP_GPIO_32KHZ_STATE_IDX           0

#define SEMLOCK    rtems_semaphore_obtain(protected_print, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
#define SEMUNLOCK  rtems_semaphore_release(protected_print);
#define DPRINTK(...) do{ SEMLOCK; printk(__VA_ARGS__);SEMUNLOCK; }while(0)
#define DPRINTF(...) do{ SEMLOCK; printf(__VA_ARGS__);SEMUNLOCK; }while(0)

#if defined(__RTEMS__)

#if !defined (__CONFIG__)
#define __CONFIG__

/* ask the system to generate a configuration table */
#define CONFIGURE_INIT

#define CONFIGURE_MICROSECONDS_PER_TICK         1000    /* 1 millisecond */
#define CONFIGURE_TICKS_PER_TIMESLICE           10      /* 10 milliseconds */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS    10
#define CONFIGURE_MINIMUM_TASK_STACK_SIZE         8192
#define CONFIGURE_MAXIMUM_TASKS                      7
#define CONFIGURE_MAXIMUM_TIMERS                     4
#define CONFIGURE_MAXIMUM_SEMAPHORES                10
#define CONFIGURE_MAXIMUM_DRIVERS                    8
#define CONFIGURE_MAXIMUM_DEVICES                    8
#define CONFIGURE_APPLICATION_NEEDS_ZERO_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_NULL_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_STUB_DRIVER

rtems_task Init(rtems_task_argument argumentx);

#include "fatalExtension.h"


#define CONFIGURE_MAXIMUM_USER_EXTENSIONS    1
#define CONFIGURE_INITIAL_EXTENSIONS         { .fatal = Fatal_extension }

//this include is put here and not at the top, because it depends on the definitions that precede
#include <rtems/confdefs.h>

#endif // __CONFIG__

BSP_SET_CLOCK(DEFAULT_OSC_CLOCK_KHZ, DEFAULT_APP_CLOCK_KHZ, CLOCK_DIVIDER_NOMINATOR, CLOCK_DIVIDER_DENOMINATOR, DEFAULT_RTEMS_CSS_LOS_CLOCKS | ( 1ULL << CSS_I2C2), DEFAULT_RTEMS_MSS_LRT_CLOCKS, 0,0,0);
BSP_SET_L2C_CONFIG(1, DEFAULT_RTEMS_L2C_REPLACEMENT_POLICY, DEFAULT_RTEMS_L2C_LOCKED_WAYS,DEFAULT_RTEMS_L2C_MODE, 0, 0);

#endif // __RTEMS__

#endif // _APP_CONFIG_H_
