///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///


// 1: Includes
// ----------------------------------------------------------------------------
#include <DrvLeon.h>
#include <DrvTimer.h>
#include "app_config.h"
#include <rtems.h>
#include <rtems/bspIo.h>
#include "rtems_config.h"
#include <rtems/fsmount.h>
#include <rtems/bdpart.h>
#include <OsDrvSdio.h>
#include <DrvGpio.h>
#include <OsBrdMv0182.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include "commonDefine.h"
#include "swcShaveLoader.h"
#include "swcLeonUtils.h"
#include <stdlib.h>

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define DEFAULT_SDIO_INT_PRIORITY                   10
#define SDIO_SLOT_USED                              1
#define SDIO_DEVNAME_USED "/dev/sdc0"

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern u32 lrt_start;
extern jpegFrameBuffer * volatile lrt_pToSdcardBuf;

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
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
void writeSdCard(u8  *buff, u32 size);
void *makeJpegPicture();

// 6: Functions Implementation
// ----------------------------------------------------------------------------
void *POSIX_Init (void *args)
{
    UNUSED(args);
    int status,rc;
    rtems_status_code sc;
    pthread_attr_t attr;
    pthread_t thread1;

    printf("\nRTEMS POSIX Started\n");
    status = initClocksAndMemory();
    if(status)
        exit(status);
    DrvLeonRTStartup((u32)&lrt_start); // Start the LeonRT application

    //Waity for LRT to signal boot completed
    DrvLeonRTWaitForBoot();
    printf("LeonRT has succesfully booted\n");

    // Provide info to the sd card driver
    osDrvSdioEntries_t info = { 1,   // Number of slots
        DEFAULT_SDIO_INT_PRIORITY,   // Interrupt priority
        {{SDIO_SLOT_USED,            // Card slot to be used
        SDIO_DEVNAME_USED,			 // Device name
        SDIO_SDR50,					 // Max speed mode
        NULL}}};        			 // Voltage switch callback function

    // Init the Sdio Driver
    printf("\nSdio driver initialising \n");
    status = OsDrvSdioInit(&info);
    if(status==RTEMS_IO_ERROR)
    {
        printf("\n ERROR: Check if sdcard was inserted\n");
    }
    else
    {
        printf("\nOsDrvSdioInit sc %s \n", rtems_status_text(status));

        sc = rtems_bdpart_register_from_disk(SDIO_DEVNAME_USED);
        printf("\nrtems_bdpart_register_from_disk sc %s \n", rtems_status_text(sc));

        // Mount the dosfs
        sc = rtems_fsmount( fs_table, sizeof(fs_table)/sizeof(fs_table[0]), NULL);
        printf("\nMounting File System %s \n", rtems_status_text(sc));
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
        rc = pthread_create(&thread1, &attr, &makeJpegPicture, NULL);
        if (rc)
        {
            printf("Thread  creation failed: %d\n", rc);
            exit(rc);
        }
        else
        {
            printf("Thread  created\n");
        }
    }
    return NULL;
}

void writeSdCard(u8 *buff, u32 size)
{
    int fd = 0;

    rtems_status_code sc;
    char file[100];
    static u32 fileNum = 0;
    char ftmp[] = "/mnt/sdcard/warpCnt.tmp";
    int fx;
    char txt[10];

    fx = open(ftmp, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    if (read(fx, txt, 4) == 4)
    {
        sscanf(txt, "%04lu", &fileNum);
        printf("Scanned %lu \n", fileNum);
    }
    lseek(fx, 0, SEEK_SET);
    sprintf(txt, "%04lu", fileNum + 1);
    printf("Written to file %lu \n", fileNum + 1);

    sc = write(fx, txt, 4);
    assert(sc);
    fsync(fx);
    close(fx);

    fx = open(ftmp, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    if (read(fx, txt, 4) == 4)
    {
        sscanf(txt, "%04lu", &fileNum);
        printf("Scanned %lu \n", fileNum);
    }
    close(fx);

    sprintf(file, "/mnt/sdcard/warp_1920x1080_P400_%03lu.jpg", fileNum++);

    printf("\nCreating file %s\n", file);
    fd = creat(file, S_IRWXU | S_IRWXG | S_IRWXO);
    assert(fd);

    printf("\nWriting %lu bytes to file\n", size);
    sc = write(fd, buff, size);
    assert(sc);

    printf("\nPerform fsync\n");
    sc = fsync(fd);
    if (sc != 0)
        printf("fsync failed, ret %d, err %s\n", sc, strerror(errno));

    printf("\nClosing file\n");
    sc = close(fd);
    if (sc != 0)
        printf("close failed, ret %d, err %s\n", sc, strerror(errno));
}
void *makeJpegPicture()
{
    while (1)
    {
        if (lrt_pToSdcardBuf != 0)
        {
            writeSdCard(lrt_pToSdcardBuf->buf, lrt_pToSdcardBuf->size);
            lrt_pToSdcardBuf = 0;
        }
    }
    return NULL; // hush the compiler warning
}

