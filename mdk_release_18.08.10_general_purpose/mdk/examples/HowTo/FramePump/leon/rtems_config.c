///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon header
///

#include <bsp.h>
#include <rtems.h>
#include <mv_types.h>
#include "fatalExtension.h"

#define DEFAULT_APP_CLOCK_KHZ       (600000)
#define DEFAULT_OSC_CLOCK_KHZ       (12000)


// System Clock configuration on start-up
BSP_SET_CLOCK(DEFAULT_OSC_CLOCK_KHZ,
              DEFAULT_APP_CLOCK_KHZ,
              1, 1,
              DEFAULT_RTEMS_CSS_LOS_CLOCKS,
              0, 0, 0, 0);

// Program L2 cache behaviour
BSP_SET_L2C_CONFIG(1,
                   DEFAULT_RTEMS_L2C_REPLACEMENT_POLICY,
                   DEFAULT_RTEMS_L2C_LOCKED_WAYS,
                   L2C_MODE_WRITE_THROUGH,
                   0, 0);


/* ask the system to generate a configuration table */
#define CONFIGURE_INIT

#ifndef RTEMS_POSIX_API
#define RTEMS_POSIX_API
#endif

#define CONFIGURE_MICROSECONDS_PER_TICK         (1000) /* 1 millisecond */

#define CONFIGURE_TICKS_PER_TIMESLICE           (1)    /* 1 millisecond(s) */

#ifndef NO_PRINT
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#endif

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MINIMUM_TASK_STACK_SIZE       (3*1024)

#define CONFIGURE_MAXIMUM_TASKS                 (12)
#define CONFIGURE_MAXIMUM_SEMAPHORES            (20)

#define CONFIGURE_UNIFIED_WORK_AREAS
#define CONFIGURE_UNLIMITED_OBJECTS

// POSIX STUFF
#define CONFIGURE_POSIX_INIT_THREAD_TABLE
#define CONFIGURE_MAXIMUM_POSIX_THREADS         (12)
#define CONFIGURE_MAXIMUM_POSIX_MUTEXES         (12)
#define CONFIGURE_MAXIMUM_POSIX_KEYS            (12)
#define CONFIGURE_MAXIMUM_POSIX_SEMAPHORES      (12)
#define CONFIGURE_MAXIMUM_POSIX_MESSAGE_QUEUES  (12)
#define CONFIGURE_MAXIMUM_POSIX_TIMERS          (12)

#define CONFIGURE_MAXIMUM_TIMERS                (20)
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES        (20)

#define CONFIGURE_MAXIMUM_DRIVERS               (20)
#define CONFIGURE_MAXIMUM_DEVICES               (20)

#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS (40)

#define CONFIGURE_APPLICATION_NEEDS_LIBBLOCK
#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM

#include <rtems/shell.h>
#define CONFIGURE_SHELL_COMMANDS_INIT
#define CONFIGURE_SHELL_COMMANDS_ALL
#include <rtems/shellconfig.h>

void POSIX_Init (void *args);

void  Fatal_extension(
    Internal_errors_Source  the_source,
    bool                    is_internal,
    Internal_errors_t       the_error
    );

#define CONFIGURE_MAXIMUM_USER_EXTENSIONS    (1)
#define CONFIGURE_INITIAL_EXTENSIONS         { .fatal = Fatal_extension }

#include <rtems/confdefs.h>
