/// =====================================================================================
///
///        @file:      main.c
///        @brief:     Stereo Camera capture and stream to HDMI + Save to SDCard
///        @copyright: All code copyright Movidius Ltd 2013, all rights reserved.
///                  For License Warranty see: common/license.txt
/// =====================================================================================
///

/// System Includes
/// -------------------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <rtems.h>
#include <DrvLeon.h>
#include <DrvLeonL2C.h>
#include <DrvShaveL2Cache.h>
#include <DrvTimer.h>
#include <OsDrvCpr.h>
#include <OsDrvTimer.h>
#include "OsDrvSdio.h"
#include <rtems/bdpart.h>
#include <rtems/fsmount.h>
#include <rtems/ide_part_table.h>
#include "DrvGpio.h"

/// Application Includes
/// -------------------------------------------------------------------------------------
#include "rtems_config.h"
#include "common_defines.h"

/// Source Specific #defines and types (typedef,enum,struct)
/// -------------------------------------------------------------------------------------
/// Terminal escapes
#define PRINTF_BOLD_RED     "\033[1m\033[31m"  /// Bold red
#define PRINTF_RESET        "\033[0m"          /// Reset formatting

//#define PRINT_PER_FRAME_DURATIONS

#define WRITE_SIZE         (LCD_LEFT_FRAME_SIZE_BYTES) /// = (LCD_RIGHT_FRAME_SIZE_BYTES)
#define ERASE_BLOCK_SIZE   (CONFIGURE_BDBUF_CACHE_MEMORY_SIZE)
#define NR_FRAMES_TO_WRITE (500)   /// Gives aprox size of 2 gb (limited by fat32)

/// If none is defined, by default save left only
#if !defined (SAVE_LEFT_CAMERA) && !defined (SAVE_RIGHT_CAMERA)
#define SAVE_LEFT_CAMERA
#endif

#if defined (SAVE_LEFT_CAMERA) && defined (SAVE_RIGHT_CAMERA)
#define NR_FRAMES_TO_WRITE_TOTAL    (NR_FRAMES_TO_WRITE * 2)
#else
#define NR_FRAMES_TO_WRITE_TOTAL    (NR_FRAMES_TO_WRITE)
#endif

/// Global Data (Only if absolutely necessary)
/// -------------------------------------------------------------------------------------
//Entry point of the leon_rt processor
extern u32      *lrt_start;

extern u8 lrt_lcdLeftBuf[MAX_USED_BUF][LCD_LEFT_FRAME_SIZE_BYTES];
extern u8 lrt_lcdRightBuf[MAX_USED_BUF][LCD_RIGHT_FRAME_SIZE_BYTES];

volatile uint64_t writing_times[NR_FRAMES_TO_WRITE * 2];
volatile uint32_t write_counter_perframe = 0;
volatile uint32_t write_counter_total = 0;

/// Static Local Data
/// -------------------------------------------------------------------------------------
static const rtems_fstab_entry fs_table [] = {
{
     .source = "/dev/sdc0",
     .target = "/mnt/sdcard",
     .type = "dosfs",
     .options = RTEMS_FILESYSTEM_READ_WRITE,
     .report_reasons = RTEMS_FSTAB_ANY,
     .abort_reasons = RTEMS_FSTAB_OK
    },
    {
     .source = "/dev/sdc01",
     .target = "/mnt/sdcard",
     .type = "dosfs",
     .options = RTEMS_FILESYSTEM_READ_WRITE,
     .report_reasons = RTEMS_FSTAB_ANY,
     .abort_reasons = RTEMS_FSTAB_NONE
    }
};

/// Static Function Prototypes
/// -------------------------------------------------------------------------------------
static void *sdCardExample(void *arg);
static void report();

/// Functions Implementation
/// -------------------------------------------------------------------------------------

/// ===  FUNCTION  ======================================================================
///  Name:  POSIX_Init
///  Description: rtems posix api init function
/// =====================================================================================
void *POSIX_Init (void *args)
{
    UNUSED(args);

    pthread_attr_t attr;
    rtems_status_code sc;
    int status, rc1;
    pthread_t thread1;

    printf ("\nRTEMS POSIX Started\n");

    status = initClocksAndMemory();
    if(status)
        exit(status);

    DrvLeonRTStartup((u32)&lrt_start);
    //Waity for LRT to signal boot completed
    DrvLeonRTWaitForBoot();
    printf("LeonRT has succesfully booted and streams are coming :).\n");

    printf ("\nRTEMS POSIX Started\n");

    status = pthread_attr_init(&attr);
    if(status !=0) {
          printf("pthread_attr_init error");
          exit(status);
    }
    status = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    if(status != 0) {
          printf("pthread_attr_setinheritsched error");
          exit(status);
    }
    status = pthread_attr_setschedpolicy(&attr, SCHED_RR);
    if(status != 0) {
          printf("pthread_attr_setschedpolicy error");
          exit(status);
    }
    // Provide info to the sd card driver
    osDrvSdioEntries_t info = { 1,  // Number of slots
                                10, // Interrupt priority
                              {{1, // Card slot to be used
                                "/dev/sdc0", // Device name
                                SDIO_SDR50, // Max speed mode
                                NULL}}}; // Voltage switch callback function
    // Init the Sdio Driver
    printf("\nSdio driver initialising \n");
    sc = OsDrvSdioInit(&info);
    if (sc != 0)
      printf("\nOsDrvSdioInit sc %s \n", rtems_status_text(sc));
    assert((sc == 0));

    sc = rtems_bdpart_register_from_disk("/dev/sdc0");

    printf("\nrtems_bdpart_register_from_disk sc %s \n", rtems_status_text(sc));

    if ((rc1 = pthread_create( &thread1, &attr, &sdCardExample,NULL))) {
      printf("\nThread 1 creation failed: %d\n", rc1);
      exit(rc1);
    }
    else {
      printf("\nThread 1 created\n");
    }

    sc = pthread_join( thread1, NULL);
    if(sc) {
      printf("pthread_join error!");
      exit(sc);
    }

    /// Print report
    report();

    DrvLeonRTWaitExecution();

    exit(0);
}

/// ===  FUNCTION  ======================================================================
///  Name:  report
///  Description: generates SDIO Write Speed statistics
/// =====================================================================================
static void report()
{
    uint32_t i;
    uint64_t avg = 0;
    uint64_t max = writing_times[0];

    for(i=0; i < (NR_FRAMES_TO_WRITE_TOTAL); i++)
    {
#ifdef PRINT_PER_FRAME_DURATIONS
        printf ( "%3d - %llu \n",i,writing_times[i]);
#endif
        avg += (writing_times[i] / NR_FRAMES_TO_WRITE_TOTAL);

        if(writing_times[i] > max)
        {
            max = writing_times[i];
        }
    }

    float average_time = (float)avg / (float)(PLL_DESIRED_FREQ_KHZ*1000);
    float average_speed_mbps = (float)WRITE_SIZE / average_time;
    average_speed_mbps = average_speed_mbps / (1024*1024);

    float max_time = (float)max / (float)(PLL_DESIRED_FREQ_KHZ*1000);
    float min_speed = (float)ERASE_BLOCK_SIZE / max_time;
    min_speed = min_speed / (1024*1024);

    printf ( "------------------------------------\n" );
    printf ( "Write size: %lu bytes\n", (uint32_t)WRITE_SIZE );
    printf ( "Nr of frames written: %d per sensor \n", NR_FRAMES_TO_WRITE );
    printf ( "Average CC: %llu \n",avg );
    printf ( "Average time: %4.4f seconds\n",average_time);
    printf ( PRINTF_BOLD_RED "Average mbps: %4.2f Mb/s" PRINTF_RESET "\n",average_speed_mbps );
    printf ( "------------------------------------\n" );
    printf ( "Min speed CC: %llu \n", max );
    printf ( "Min speed time: %4.4f seconds\n", max_time );
    printf ( PRINTF_BOLD_RED "Min speed mbps: %4.2f Mb/s" PRINTF_RESET "\n", min_speed );
    printf ( "------------------------------------\n" );
}

/// ===  FUNCTION  ======================================================================
///  Name:  sdCardExample
///  Description: thread to open,verify,write and close files on SDCard
/// =====================================================================================
static void *sdCardExample(void *arg)
{
    UNUSED(arg);

    int fd1 = 0;
    int fd2 = 0;
    int offset = 0;
    rtems_status_code sc;
    tyTimeStamp  g_ticks;

    // Mount the dosfs
    sc = rtems_fsmount( fs_table, sizeof(fs_table)/sizeof(fs_table[0]), NULL);
    printf("\nMounting File System status %s \n", rtems_status_text(sc));

#ifdef SAVE_LEFT_CAMERA
    //Try to create the file if does not exist
    fd1 = open("/mnt/sdcard/leftCam.yuv", O_RDWR | O_CREAT);
    //If using windows system, filename should not be longher than 8 characters
    // If it does exist, just open it
    if (fd1 == 0)
    {
        fd1 = open("/mnt/sdcard/leftCam.yuv", O_RDWR);
    }
    assert(fd1);

    /// Check if file isnt too big already for FAT32
    offset  = lseek(fd1, 0, SEEK_END);
    assert(offset >= 0);
#endif

#ifdef SAVE_RIGHT_CAMERA

    //Try to create the file if does not exist
    fd2 = open("/mnt/sdcard/rightCam.yuv", O_RDWR | O_CREAT);
    //If using windows system, filename should not be longher than 8 characters
    // If it does exist, just open it
    if (fd2 == 0)
    {
        fd2 = open("/mnt/sdcard/rightCam.yuv", O_RDWR);
    }
    assert(fd2);

    /// Check if file isnt too big already for FAT32
    offset  = lseek(fd2, 0, SEEK_END);
    assert(offset >= 0);
#endif

    /// Start writing the frames
    printf ( "Writing frames to sdcard... \n" );

    for(write_counter_perframe = 0; write_counter_perframe < NR_FRAMES_TO_WRITE; write_counter_perframe++)
    {
#ifdef SAVE_LEFT_CAMERA
        /// Start timer
        DrvTimerStartTicksCount(&g_ticks);
        /// === Write left frame ===
        sc = write(fd1, &lrt_lcdLeftBuf[write_counter_perframe % MAX_USED_BUF], (WRITE_SIZE));
        /// Sanity check
        assert(sc == (WRITE_SIZE));
        /// Read timer
        DrvTimerGetElapsedTicks(&g_ticks, (u64*)(&writing_times[write_counter_total++]));
#endif
#ifdef SAVE_RIGHT_CAMERA
        /// Restart timer
        DrvTimerStartTicksCount(&g_ticks);
        /// === Write right frame ===
        sc = write(fd2, &lrt_lcdRightBuf[write_counter_perframe % MAX_USED_BUF], (WRITE_SIZE));
        /// Sanity check
        assert(sc == (WRITE_SIZE));
        /// Read timer
        DrvTimerGetElapsedTicks(&g_ticks, (u64*)(&writing_times[write_counter_total++]));
#endif
    };

#ifdef SAVE_LEFT_CAMERA
    /// Sync
    sc = fsync(fd1);
    sc = close(fd1);
    assert((sc == 0));
#endif
#ifdef SAVE_RIGHT_CAMERA
    /// Sync
    sc = fsync(fd2);
    sc = close(fd2);
    assert((sc == 0));
#endif
    /// Unmount
    sc  = unmount("/mnt/sdcard");
    if (sc == 0)
        printf("\nCard successfully unmounted\n\n");
    else
        printf("\nFailed to unmount card!\n\n");
    assert(sc == 0);

    pthread_exit(0);
    return NULL; // just so the compiler thinks we returned something
}
