///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     RTEMS configuration
///

#include <rtems.h>
#include <DrvCpr.h>

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

#define CONFIGURE_MAXIMUM_POSIX_THREADS          8
#define CONFIGURE_MAXIMUM_POSIX_MUTEXES          2
#define CONFIGURE_MAXIMUM_POSIX_KEYS             2
#define CONFIGURE_MAXIMUM_POSIX_EVENTS           8
#define CONFIGURE_MAXIMUM_POSIX_SEMAPHORES      20
#define CONFIGURE_MAXIMUM_POSIX_MESSAGE_QUEUES   0
#define CONFIGURE_MAXIMUM_POSIX_TIMERS           2
#define CONFIGURE_MAXIMUM_REGIONS                1

#define CONFIGURE_MAXIMUM_USER_EXTENSIONS        1

static void Fatal_extension(
    Internal_errors_Source  the_source, bool is_internal, Internal_errors_t  the_error);

#define CONFIGURE_INITIAL_EXTENSIONS { .fatal = &Fatal_extension }

void *POSIX_Init (void *args);

#include <rtems/confdefs.h>

#ifndef DEFAULT_APP_CLOCK_KHZ
#define DEFAULT_APP_CLOCK_KHZ 266000
#endif

/// Set the booting clocks, default clock for the aplication
#define MSS_DEF_CLOCKS  DEFAULT_RTEMS_MSS_LRT_CLOCKS | \
                        (1UL << MSS_SPMP) | \
                        (1UL << MSS_AMC) | \
                        (1UL << MSS_LCD) | \
                        (1UL << MSS_AXI_MON) | \
                        (1UL << MSS_MXI_DEFSLV) | \
                        (1UL << MSS_MXI_CTRL) | \
                        (1UL << MSS_AXI_BRIDGE) | \
                        (1UL << MSS_AXI_MON) | \
                        (1UL << MSS_AXI_MON) | \
                        (1UL << MSS_AXI_MON) | \
                        (1UL << MSS_SPMP)

#define APP_SIPP_CLOCKS (DEV_SIPP_SIGMA          |   \
                      DEV_SIPP_LSC            |   \
                      DEV_SIPP_RAW            |   \
                      DEV_SIPP_DBYR           |   \
                      DEV_SIPP_DOGL           |   \
                      DEV_SIPP_LUMA           |   \
                      DEV_SIPP_SHARPEN        |   \
                      DEV_SIPP_CGEN           |   \
                      DEV_SIPP_MED            |   \
                      DEV_SIPP_CHROMA         |   \
                      DEV_SIPP_CC             |   \
                      DEV_SIPP_LUT            |   \
                      DEV_SIPP_UPFIRDN0       |   \
                      DEV_SIPP_UPFIRDN1       |   \
                      DEV_SIPP_UPFIRDN2       |   \
                      DEV_SIPP_MIPI_RX0       |   \
                      DEV_MSS_SIPP_CLK_GATES  |   \
                      DEV_SIPP_MIPI           |   \
                      DEV_SIPP_SIPP_ABPSLV    |   \
                      DEV_SIPP_APB_SLV)


// Set the system clocks at startup
#ifdef MA2X8X
BSP_SET_CLOCK(24000, DEFAULT_APP_CLOCK_KHZ, 1, 1, DEFAULT_RTEMS_CSS_LOS_CLOCKS, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0);
#else
BSP_SET_CLOCK(12000, DEFAULT_APP_CLOCK_KHZ, 1, 1, DEFAULT_RTEMS_CSS_LOS_CLOCKS, MSS_DEF_CLOCKS, APP_UPA_CLOCKS, APP_SIPP_CLOCKS, 0);
#endif
// Set the L2C at startup
BSP_SET_L2C_CONFIG(0, L2C_REPL_LRU, 0, L2C_MODE_COPY_BACK, 0, NULL);

// User extension to be able to catch abnormal terminations
static void Fatal_extension(
    Internal_errors_Source  the_source,
    bool                    is_internal,
    Internal_errors_t       the_error
    )
{
    switch(the_source)
    {
    case INTERNAL_ERROR_CORE:
        printk(" : %s \n", rtems_internal_error_text(the_error));
        break;
    case RTEMS_FATAL_SOURCE_EXIT:
        if(the_error)
            printk("\nExited with error code %d\n", the_error);
        break; // normal exit
    case RTEMS_FATAL_SOURCE_ASSERT:
        printk("\n%s : %d in %s \n",
               ((rtems_assert_context *)the_error)->file,
               ((rtems_assert_context *)the_error)->line,
               ((rtems_assert_context *)the_error)->function);
        break;
    case RTEMS_FATAL_SOURCE_EXCEPTION:
        rtems_exception_frame_print((const rtems_exception_frame *) the_error);
        break;
    default:
        printk ("\nSource %d Internal %d Error %d  0x%X:\n",
                the_source, is_internal, the_error, the_error);
        break;
    }
}
