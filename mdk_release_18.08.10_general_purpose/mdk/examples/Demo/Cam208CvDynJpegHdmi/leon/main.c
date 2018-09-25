///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Variation of "IpipePPTest09, IpipePPTest09_a
///            PP inputs 960x128 and outputs 480x64 RGB data for VideoSipp
///            LOS starts LRT which executes the PP test.
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <DrvLeon.h>
#include <DrvCpr.h>
#include <DrvTimer.h>
#include <DrvDdr.h>
#include <swcLeonUtils.h>
#include "LeonIPCApi.h"
#include <stdlib.h>
#include <stdio.h>
//#include "app_config.h"
#include <rtems.h>
#include "rtems_config.h"
#include <rtems/fsmount.h>
#include <rtems/bdpart.h>
#include <bsp.h>
#include "DrvI2cMaster.h"
#include <OsDrvSdio.h>
#include <DrvGpio.h>
#include <OsBrdMv0182.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include "commonDefine.h"
#include "swcShaveLoader.h"
#include "swcLeonUtils.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define DEFAULT_SDIO_INT_PRIORITY                   10
#define SDIO_SLOT_USED                              1
#define SDIO_DEVNAME_USED "/dev/sdc0"

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
//Entry point of the leon_rt processor
extern u32      *lrt_start;

extern jpegFrameBuffer * volatile lrt_pToSdcardBuf;

#define MSG_SIZE       1
#define MSG_QUEUE_SIZE 10

extern leonIPCChannel_t lrt_LOStoLRTChannel;
uint32_t __attribute__((section(".cmx_direct.data"))) messagePool[MSG_QUEUE_SIZE * MSG_SIZE];
uint32_t __attribute__((section(".cmx_direct.data"))) myMessage;

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

static tyI2cConfig  i2c2MasterCfg =
{
        .device = IIC3_BASE_ADR,
        .sclPin = 79,
        .sdaPin = 80,
        .speedKhz =  100,
        .addressSize = ADDR_7BIT,
        .errorHandler = NULL
};
static I2CM_Device I2cDevice;
static u8 protocolWriteSample2[] = I2C_PROTO_WRITE_16BA;
static u8 protocolReadSample2[]  = I2C_PROTO_READ_16BA;
static u32 btnSlave=0x36;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
void writeSdCard(u8  *buff, u32 size);
void *makeJpegPicture();


static void initWM8325(){
  s32 status;
  //0x80C0 is GPIO Input Normal mode
  u32 gpioInput=0x80C080C0; // doubled to write next register too

  status = DrvI2cMInitFromConfig(&I2cDevice, &i2c2MasterCfg);
  if(status) {
    printf("I2C Setup step 1 failed\n");
    exit(1);
  }
  // GPIO1 Control register address is 0x4038, writing GPIO2 too
  status = DrvI2cMTransaction(&I2cDevice, btnSlave, 0x4038, protocolWriteSample2, (u8 *) &gpioInput, 4);
  if(status) {
    printf("I2C Setup step 2 failed\n");
    exit(1);
  }
}

static inline u32 readButton(u8* buttons)
{
    s32 status;
    u8 currButton[2];

    status = DrvI2cMTransaction(&I2cDevice, btnSlave, 0x400c, protocolReadSample2, currButton, 2);
    *buttons = currButton[1];
    return status;
}

// 6: Functions Implementation
// ----------------------------------------------------------------------------
void *POSIX_Init (void *args)
{
  UNUSED(args);
  u32 status, rc;
  rtems_status_code sc;
  u8 buttons, prevButtons=0;
  pthread_attr_t attr;
  pthread_t thread1;

  status = initClocksAndMemory();
  if(status){
      exit(status);
  }

  status = LeonIPCTxInit(&lrt_LOStoLRTChannel, messagePool, MSG_QUEUE_SIZE, MSG_SIZE);
  if(status) exit(status);

  DrvLeonRTStartup((u32)&lrt_start);

  DrvLeonRTWaitForBoot();
  initWM8325();

  // Provide info to the sd card driver
  osDrvSdioEntries_t info = { 1,   // Number of slots
      DEFAULT_SDIO_INT_PRIORITY,   // Interrupt priority
      {{SDIO_SLOT_USED,            // Card slot to be used
      SDIO_DEVNAME_USED,           // Device name
      SDIO_SDR50,                  // Max speed mode
      NULL}}};                     // Voltage switch callback function

  // Init the Sdio Driver
  printf("\nSdio driver initialising \n");
  while(1)
  {
    status = OsDrvSdioInit(&info);
    if(status!= RTEMS_SUCCESSFUL)
    {
        printf("\nERROR: SDIO was not succesfully initialised\n");
        printf("SD Card not found\n");
        exit(status);
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

      while(1)
      {
        if(readButton(&buttons))
        {
          printf("I2C communication failed\n");
          exit(1);
        }
        if ((buttons == 1) && (prevButtons != buttons))
        {
          printf("Button pressed 0x%08X\n",buttons);
          myMessage=0xB0050000 | buttons;
          status = LeonIPCSendMessage(&lrt_LOStoLRTChannel, &myMessage);
          if(status) {
            printf("IPC fail\n");
            DrvTimerSleepMs(200);
            exit(status);
          }
          rc = pthread_create(&thread1, &attr, &makeJpegPicture, NULL);
          if (rc)
          {
            printf("Thread  creation failed: %lu\n", rc);
            exit(rc);
          }
          else
          {
            printf("Thread  created\n");
          }
          sc = pthread_join(thread1, NULL);
          if (sc)
          {
              printf("pthread_join error!");
              exit(sc);
          }
        }
        prevButtons=buttons;
      }
    }
  }
}

void writeSdCard(u8 *buff, u32 size)
{
    int fd = 0;

    rtems_status_code sc;
    char file[100];
    static u32 fileNum = 0;
    char ftmp[] = "/mnt/sdcard/warpCnt.tmp";
    char txt[10];
    int fx;

    printf("Checking for next available filename on SDCard\n");

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

    sprintf(file, "/mnt/sdcard/Cam208CvDyn_1920x1080_P422_%03lu.jpg", fileNum++);

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

    printf("\nClosing file\n\n");
    sc = close(fd);
    if (sc != 0)
        printf("close failed, ret %d, err %s\n", sc, strerror(errno));
}

void *makeJpegPicture()
{
    printf("Thread started\n");

    // wait for Lrt to prepare jpeg data
    while(lrt_pToSdcardBuf == NULL);

    writeSdCard(lrt_pToSdcardBuf->buf, lrt_pToSdcardBuf->size);
    lrt_pToSdcardBuf = NULL;

    return NULL; // hush the compiler warning
}
