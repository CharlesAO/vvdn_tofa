///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <rtems.h>
#include <rtems/bspIo.h>
#include <fcntl.h>
#include <mv_types.h>
#include <rtems/fsmount.h>
#include <rtems/bdpart.h>
#include <OsDrvCpr.h>
#include <OsDrvSdio.h>
#include "rtems_config.h"
#include <OsBrdMv0182.h>
#include <DrvGpio.h>
#include <assert.h>

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define BUFFER_SIZE                                 (1024*1024*5)
#define DEFAULT_SDIO_INT_PRIORITY                   10
#define SDIO_SLOT_USED                              1
#define SDIO_DEVNAME_USED                           "/dev/sdc0"

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
/* Sections decoration is require here for downstream tools */
/* Mount Table */
static const rtems_fstab_entry fs_table [] = {
   {
     .source = "/dev/sdc0",
     .target = "/mnt/sdcard",
     .type = "dosfs",
     .options = RTEMS_FILESYSTEM_READ_WRITE,
     .report_reasons = RTEMS_FSTAB_NONE,
     .abort_reasons = RTEMS_FSTAB_OK
    },
    {
     .source = "/dev/sdc01",
     .target = "/mnt/sdcard",
     .type = "dosfs",
     .options = RTEMS_FILESYSTEM_READ_WRITE,
     .report_reasons = RTEMS_FSTAB_NONE,
     .abort_reasons = RTEMS_FSTAB_NONE
    }
};
/* Read/Write buffers */
static u8 write_buffer[BUFFER_SIZE];
static u8 read_buffer[BUFFER_SIZE];

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
void *sdCardExample(void *arg);

// 6: Functions Implementation
// ----------------------------------------------------------------------------

void *POSIX_Init (void *args)
{
    UNUSED(args);

    pthread_attr_t attr;
    int status, rc1;
    pthread_t thread1;
    rtems_status_code sc;
    // Provide info to the sd card driver
    osDrvSdioEntries_t info = { 1,  // Number of slots
                                10, // Interrupt priority
                              {{1, // Card slot to be used
                                "/dev/sdc0", // Device name
                                SDIO_SDR50, // Max speed mode
                                NULL}}}; // Voltage switch callback function
    // Board configuration used
    const tyOsBoard0182Configuration brdConfig[] =
    {
        {
            .type = MV182_END,
            .value = 0
        },
    };

    printf ("\nRTEMS POSIX Started\n");
    // Init Clocks
    initClocksAndMemory();

    // Init board driver
    status = osBoard0182Initialise((tyOsBoard0182Configuration*) brdConfig);
    printf("\nosBoard0182Initialise %d\n", status);

    if(pthread_attr_init(&attr) !=0)
        printf("pthread_attr_init error\n");

    if (pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED) != 0)
        printf("pthread_attr_setinheritsched error\n");

    if(pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0)
        printf("pthread_attr_setschedpolicy error\n");

    // Init the Sdio Driver
    printf("\nSdio driver initialising \n");
    status = OsDrvSdioInit(&info);
    printf("\nOsDrvSdioInit sc %s \n", rtems_status_text(status));

    sc = rtems_bdpart_register_from_disk(SDIO_DEVNAME_USED);
    printf("\nrtems_bdpart_register_from_disk sc %s \n", rtems_status_text(sc));

    // Mount the dosfs
    sc = rtems_fsmount( fs_table, sizeof(fs_table)/sizeof(fs_table[0]), NULL);
    printf("\nMounting File System %s \n", rtems_status_text(sc));

    if ((rc1 = pthread_create( &thread1, &attr, &sdCardExample,NULL)))
        printf("Thread 1 creation failed: %d\n", rc1);
    else
        printf("\nThread 1 created\n");

    if (pthread_join(thread1, NULL))
        printf("pthread_join error!\n");
    exit(0);
}

static void fillInBuffers(void)
{
    int i = 0;

    for (i = 0; i < BUFFER_SIZE; i++)
    {
        // This buffer should contain data
        write_buffer[i] = i+1;
        // This buffer should be empty
        read_buffer[i] = 0;
    }
}

void *sdCardExample(void *arg)
{
    UNUSED(arg);

    int fd = 0;
    rtems_status_code sc;
    const char file[] = "/mnt/sdcard/myfile";

    // Init buffers
    fillInBuffers();

    // Try to create the file if does not exist
    printf("\nCreating file %s\n", file);
    fd = creat(file, S_IRWXU | S_IRWXG | S_IRWXO ) ;
    assert(fd);

    printf("\nWriting %d bytes to file\n", BUFFER_SIZE);
    sc = write(fd, write_buffer, BUFFER_SIZE);
    assert(sc);

    printf("\nPerform fsync\n");
    sc = fsync(fd);
    assert((sc == 0));

    printf("\nClosing file\n");
    sc = close(fd);
    assert((sc == 0));

    // Validate written data
    printf("\nOpening file %s \n", file);
    fd = open("/mnt/sdcard/myfile", O_RDWR);
    assert(fd);

    printf("\nRead %d characters\n", BUFFER_SIZE);
    sc = read(fd, read_buffer, BUFFER_SIZE);
    assert(sc);

    sc = close(fd);
    // Check Values of read and written data
    printf("\nVerifying data...\n");

    assert(memcmp(read_buffer, write_buffer, BUFFER_SIZE) == 0);

    // Unmount file system
    sc  = unmount("/mnt/sdcard");
    if (sc == 0)
        printf("\nCard successfully unmounted\n\n");
    else
        printf("\nError unmounting card %s\n", rtems_status_text(sc));
    assert(sc == 0);
    // Exit thread
    pthread_exit(0);
    return NULL; // just so the compiler thinks we returned something
}







