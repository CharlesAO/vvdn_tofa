///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Main function that starts USB DataPump
///

#include <stdio.h>
#include <stdlib.h>
#include <rtems.h>
#include <time.h>
#include <bsp.h>

#include "app_config.h"
#include "rtems_config.h"
#include "OsDrvUsbPhy.h"
#include "usbpumpdebug.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "OsDrvCpr.h"

#include "usbpump_application_rtems_api.h"
#ifdef RTEMS_USB_HOST_BUILD
# include "usbpump_application_rtems_host_api.h"
#endif  /* RTEMS_USB_HOST_BUILD */

#ifndef RTEMS_USB_HOST_BIND_FILE_SYSTEM
# define RTEMS_USB_HOST_BIND_FILE_SYSTEM    0
#endif

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
/* Sections decoration is require here for downstream tools */

#ifndef DISABLE_LEON_DCACHE
# define USBPUMP_MDK_CACHE_ENABLE        1
#else
# define USBPUMP_MDK_CACHE_ENABLE        0
#endif

static USBPUMP_APPLICATION_RTEMS_CONFIGURATION sg_DataPump_AppConfig =
USBPUMP_APPLICATION_RTEMS_CONFIGURATION_INIT_V3(
    /* nEventQueue */             64,
    /* pMemoryPool */             NULL,
    /* nMemoryPool */             0,
    /* DataPumpTaskPriority */    100,
    /* DebugTaskPriority */       200,
    /* UsbInterruptPriority */    10,
    /* pDeviceSerialNumber */     NULL,
    /* pUseBusPowerFn */          NULL,
    /* fCacheEnabled */           USBPUMP_MDK_CACHE_ENABLE,
    /* DebugMask */               UDMASK_ERRORS | UDMASK_ANY,
    /* pPlatformIoctlFn */        NULL,
    /* fDoNotWaitDebugFlush */    0
    );

static tyAuxClkDividerCfg auxClk[] =
{
    {
        .auxClockEnableMask     = AUX_CLK_MASK_USB_CTRL_SUSPEND_CLK,
        .auxClockSource         = CLK_SRC_PLL0,
        .auxClockDivNumerator   = 1,
        .auxClockDivDenominator = 24
    },
    {0, 0, 0, 0}, // Null Terminated List
};

#ifndef FILE_BUFFER_SIZE
# define FILE_BUFFER_SIZE   (1024 * 1024)
#endif

#ifndef READ_FILE_NAME
# define READ_FILE_NAME     "read.dat"
#endif

#ifndef WRITE_FILE_NAME
# define WRITE_FILE_NAME    "write.dat"
#endif

#if RTEMS_USB_HOST_BIND_FILE_SYSTEM
static u32 sFileBuffer[FILE_BUFFER_SIZE / sizeof(u32)];
#endif  /* RTEMS_USB_HOST_BIND_FILE_SYSTEM */

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
void *shellCreate(void *ctx)
{
    UNUSED(ctx);

    printf("Starting RTEMS shell\n");
    rtems_shell_init(
        "SHLL",                        // task name
        RTEMS_MINIMUM_STACK_SIZE * 4,  // task stack size
        100,                           // task priority
        "/dev/console",                // device name
        false,                         // run forever
        true,                          // wait for shell to terminate
        NULL                           // no login check
    );
    return NULL;
}

int lsusb_main(int argc, char **argv)
{
    unsigned int BufferSize;

    if (argc < 2)
        BufferSize = 2048;
    else
    {
        sscanf(argv[1], "%u", &BufferSize);
        if (BufferSize < 256)
            BufferSize = 256;
    }

    UsbPump_Rtems_DataPump_ListUsbDevice(BufferSize);
    return 0;
}

int setdebugflags_main(int argc, char **argv)
{
    char *pObjectNamePattern;
    unsigned int ClearDebugFlags;
    unsigned int SetDebugFlags;

    if (argc < 4)
        {
        printf("usage: setdebugflags object_name_pattern clear_debug_flags set_debug_flags\n");
        return 1;
        }

    pObjectNamePattern = argv[1];
    ClearDebugFlags = 0;
    SetDebugFlags = 0;
    sscanf(argv[2], "%x", &ClearDebugFlags);
    sscanf(argv[3], "%x", &SetDebugFlags);

    printf("Setting debug flags for \"%s\" clear=%x set=%x\n",
        pObjectNamePattern,
        ClearDebugFlags,
        SetDebugFlags
        );

    UsbPump_Rtems_DataPump_SetDebugFlags(
        pObjectNamePattern,
        ClearDebugFlags,
        SetDebugFlags
        );
    return 0;
}

#if RTEMS_USB_HOST_BIND_FILE_SYSTEM
static void *File_System_Performance_Thread(void *arg);
#endif  /* RTEMS_USB_HOST_BIND_FILE_SYSTEM */

// 6: Functions Implementation
// ----------------------------------------------------------------------------

void *POSIX_Init (void *args)
{
    UNUSED(args);

    s32 sc;
    pthread_attr_t attr;
    pthread_t thr;

    OsDrvCprInit();
    OsDrvCprOpen();
    sc = OsDrvCprAuxClockArrayConfig(auxClk);
    if (sc)
        exit(sc);

    sc = OsDrvUsbPhyInit(NULL);
    if(sc)
        exit(sc);
    if (UsbPump_Rtems_DataPump_HostStartup(&sg_DataPump_AppConfig) != NULL)
        printf("\n\nUsbPump_Rtems_DataPump_Startup()!\n\n\n");
    else
    {
        printf("\n\nUsbPump_Rtems_DataPump_Startup() failed!\n\n\n");
        exit(1);
    }

    sc = pthread_attr_init(&attr);
    if (sc)
    {
        printf("pthread_attr_init error");
        exit(sc);
    }
    sc = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    if (sc)
    {
        printf("pthread_attr_setinheritsched error");
        exit(sc);
    }
    sc = pthread_attr_setschedpolicy(&attr, SCHED_RR);
    if (sc)
    {
        printf("pthread_attr_setschedpolicy error");
        exit(sc);
    }

    // Create MSD client object
# if RTEMS_USB_HOST_BIND_FILE_SYSTEM
    UsbPump_Rtems_DataPump_MsdClientCreate(
        UsbPump_Rtems_DataPump_GetPlatform(),
        UDMASK_ERRORS | UDMASK_ANY | UDMASK_FLOW,
        "/mnt/usb",
        (8 * 1024),
        150
        );

    sc = pthread_create(&thr, &attr, &File_System_Performance_Thread, NULL);
    if (sc)
    {
        printf("Performance thread creation failed: %d\n", (int) sc);
        exit(sc);
    }
# else /* ! RTEMS_USB_HOST_BIND_FILE_SYSTEM */
    UsbPump_Rtems_HostApp_MsdClientCreate(
        UsbPump_Rtems_DataPump_GetPlatform(),
        UDMASK_ERRORS | UDMASK_FLOW,
        (1024 * 1024)
        );
# endif /* RTEMS_USB_HOST_BIND_FILE_SYSTEM */

    // Create GCD client object
    UsbPump_Rtems_HostApp_GcdClientCreate(
        UsbPump_Rtems_DataPump_GetPlatform(),
        UDMASK_ERRORS | UDMASK_ANY,
        (24 * 1024),
        1000,
        TRUE    /* fPerformanceMeasure */
        );

    rtems_shell_add_cmd(
        "lsusb",        /* name */
        "lsusb buffer_size",    /* usage */
        "user",         /* topic */
        lsusb_main
        );

    rtems_shell_add_cmd(
        "setdebugflags",    /* name */
        "lsusb object_name_pattern clear_debug_flags set_debug_flags",  /* usage */
        "user",         /* topic */
        setdebugflags_main
        );

    sc = pthread_create(&thr, &attr, &shellCreate, NULL);
    if (sc)
    {
        printf("Shell thread creation failed: %d\n", (int) sc);
        exit(sc);
    }

    sc = pthread_join(thr, NULL);
    if (sc)
    {
        printf("pthread_join error!");
        exit(sc);
    }

    return NULL;
}

/// ===  FUNCTION  ======================================================================
///  Name:  File_System_Performance_Thread
///  Description: thread to open,verify,write and close files on USB drive
/// =====================================================================================
#if RTEMS_USB_HOST_BIND_FILE_SYSTEM
static void *File_System_Performance_Thread(void *arg)
{
    UNUSED(arg);

    int fd1 = 0;
    int fd2 = 0;
    unsigned i;
    long TimeTickS, TimeTickE, readByte, writeByte;
    u8 *pBuffer;
    rtems_status_code sc;

    printf("Starting File_System_Performance_Thread\n");
    while (fd1 <= 0)
    {
        // Try to create the file if does not exist
        fd1 = open("/mnt/usb_0_0/" READ_FILE_NAME, O_RDONLY);
        // If it does exist, just open it
        if (fd1 <= 0)
        {
            rtems_task_wake_after(1000);
        }
    }
    printf("Open /mnt/usb_0_0/" READ_FILE_NAME "\n");

    // Start read test
    printf("Start READ test...\n" );
    readByte = 0;
    TimeTickS = rtems_clock_get_ticks_since_boot();
    while ((sc = read(fd1, sFileBuffer, sizeof(sFileBuffer)) > 0))
        {
        readByte += sizeof(sFileBuffer);
        }
    TimeTickE = rtems_clock_get_ticks_since_boot();

    if (TimeTickE > TimeTickS)
        TimeTickE -= TimeTickS;
    else
        TimeTickE += 0 - TimeTickS;

    printf(" Read %ld bytes TimeTicks(%ld) %d KB/sec\n\n",
        readByte,
        TimeTickE,
        (int)(readByte/TimeTickE*rtems_clock_get_ticks_per_second()/1024)
        );

    while (fd2 <= 0)
    {
        //Try to create the file if does not exist
        fd2 = open("/mnt/usb_0_0/" WRITE_FILE_NAME, O_RDWR | O_CREAT);
        // If it does exist, just open it
        if (fd2 <= 0)
        {
            rtems_task_wake_after(1000);
        }
    }

    for (pBuffer = (u8 *) sFileBuffer, i = 0; i < sizeof(sFileBuffer); ++i)
    {
        pBuffer[i] = (u8)(0x30 + i%10);
    }

    printf("Start WRITE test...\n" );
    writeByte=0;
    TimeTickS = rtems_clock_get_ticks_since_boot();
    for (i = 0; i < 128; ++i)
    {
        sc = write(fd2, sFileBuffer, sizeof(sFileBuffer));
        if (sc > 0)
            writeByte += sizeof(sFileBuffer);
        else
            break;
    }
    TimeTickE = rtems_clock_get_ticks_since_boot();

    if (TimeTickE > TimeTickS)
        TimeTickE -= TimeTickS;
    else
        TimeTickE += 0 - TimeTickS;

    printf(" Write %ld bytes TimeTicks(%ld) %d KB/sec\n\n",
        writeByte,
        TimeTickE,
        (int)(writeByte/TimeTickE*rtems_clock_get_ticks_per_second()/1024)
        );

    fsync(fd1);
    close(fd1);
    fsync(fd2);
    close(fd2);

    printf("End File_System_Performance_Thread\n");

    return NULL;
}
#endif  /* RTEMS_USB_HOST_BIND_FILE_SYSTEM */
