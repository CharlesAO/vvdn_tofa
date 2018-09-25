///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#ifdef MV0212
#include "MV0212.h"
#include "brdGpioCfgs/brdMv0182R5GpioDefaults.h"
#elif defined(MV0182)
#include <Board182Api.h>
#endif

#include "DrvCDCEL.h"
#include "DrvCpr.h"
#include "CamGenericApi.h"
#include "sipp.h"
#include "sippTestCommon.h"
#include "DrvLeon.h"
#include "imx208_2L_1936x1096_Raw10_15Hz.h"
#include "imx208_2L_1936x1096_Raw10_60Hz.h"
#include "lrt_app_config.h"
#include "cam_config.h"
#include "LeonIPCApi.h"
#include "DrvTimer.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define FRAME_READY 1

#ifdef MV0212
#define NUM_I2C_DEVS 3
#endif
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
static I2CM_Device * i2c0Handle;

leonIPCChannel_t __attribute__((section(".cmx_direct.data"))) LRTtoLOSChannel;
extern leonIPCChannel_t LOStoLRTChannel;
uint32_t __attribute__((section(".cmx_direct.data"))) messagePool[MSG_QUEUE_SIZE * MSG_SIZE];
static uint32_t receivedMessage;
static uint32_t currentSpeed;

// 60Hz camera spec used for USB 3 (SuperSpeed mode)
GenericCamSpec *staticCamConfig_SS = &imx208_2L_1936x1096_RAW10_60Hz_camCfg;
// 15Hz camera spec used for USB 2 (HighSpeed mode)
GenericCamSpec *staticCamConfig_HS = &imx208_2L_1936x1096_RAW10_15Hz_camCfg;

volatile ALIGNED(4) u32 newCamFrameCtr  = FIRST_INCOMING_BUF_ID;
volatile ALIGNED(4)  __attribute__((section(" .cmx_direct.data")))  u32 usbFrameCtr = FIRST_OUTGOING_BUF_ID;

u8 camBuf[MAX_USED_BUF][CAM_FRAME_SIZE_BYTES] DDR_AREA ALIGNED(8);

// to correctly send frames on USB from DDR, each buffer must be aligned to 128 bytes
// each frame will be send in an USB bulk transfer and it must be preceded by the USB stream header
u8 usbBuf1[PAYLOAD_HEADER_OFFSET + CAM_FRAME_SIZE_BYTES] __attribute__((section(".ddr_direct.bss")));
u8 usbBuf2[PAYLOAD_HEADER_OFFSET + CAM_FRAME_SIZE_BYTES] __attribute__((section(".ddr_direct.bss")));
u8 usbBuf3[PAYLOAD_HEADER_OFFSET + CAM_FRAME_SIZE_BYTES] __attribute__((section(".ddr_direct.bss")));
u8 usbBuf4[PAYLOAD_HEADER_OFFSET + CAM_FRAME_SIZE_BYTES] __attribute__((section(".ddr_direct.bss")));


u8* usbBufPtr[MAX_USED_BUF] = {usbBuf1, usbBuf2, usbBuf3, usbBuf4};

volatile u32 ALIGNED(4) processingFrameCtr;

GenericCameraHandle          camHndl;
callbacksListStruct          callbacks = {0};
interruptsCallbacksListType  isrCallbacks = {0};
CamUserSpec                  userCamConfig;
frameSpec                    camFrameSpec;

frameBuffer          camFrame[MAX_USED_BUF];

SippPipeline *pl;

SippFilter   *dmaIn;
SippFilter   *lut;
SippFilter   *dmaOut;

DmaParam     *dmaInCfg;
DmaParam     *dmaOutCfg;
LutParam     *lutCfg;

volatile u32 frameReady;

static void usbAttached(struct leonIPCChannel_t *channel)
{
    int status;

    if (LeonIPCReadMessage(channel, &receivedMessage) == IPC_SUCCESS)
    {
        if (currentSpeed != receivedMessage)
        {
            // restart the camera
            status = CamStop(&camHndl);
            if (status != CAM_SUCCESS)
            {
                printf("\n Could not stop the camera(%d).\n", status);
                return;
            }
            newCamFrameCtr  = FIRST_INCOMING_BUF_ID;
            processingFrameCtr = newCamFrameCtr + 1;
            DrvTimerSleepMs(1500);
            if (receivedMessage == USB_SUPER_SPEED)
            {
                status = CamInit(&camHndl, staticCamConfig_SS, &userCamConfig, &callbacks, i2c0Handle);
                if (!status)
                {
                    printf("Camera initialized in SuperSpeed mode\n");
                }
            }
            else
            {
                status = CamInit(&camHndl, staticCamConfig_HS, &userCamConfig, &callbacks, i2c0Handle);
                if (!status)
                {
                    printf("Camera initialized in HighSpeed mode\n");
                }
            }
            if (status)
            {
                printf("\n Camera configuration failed (%d).\n", status);
            }

            status  = CamStart(&camHndl);
            if (status != CAM_SUCCESS)
            {
                printf("\n Camera failed to start (%d).\n", status);
                return;
            }

            currentSpeed = receivedMessage;
        }
    }
}

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
frameBuffer* AllocateNextCamFrameBuf(void);

// 6: Functions Implementation
// ----------------------------------------------------------------------------

void generateLutTable(void)
{
    u16 i;

    //look-up table for 10->8 bits conversion; it should contain 1024 values but the current implementation reduce the table to only 256, as this is enough (8 bits output)
    //also add the neutral 0x80 pattern for the YUV422 format
    for (i = 0; i < 256; i++)
    {
        lutTable[i] = i | 0x8000;
    }
}

void prepareDriverData(void)
{
   int ibuf;

   //user extra settings
   userCamConfig.mipiControllerNb  = CAM_B1_MIPICTRL;                     //camera connected to CAMB connecter of the board
   userCamConfig.receiverId        = SIPP_DEVICE1;                        //
#ifdef MV0212
   userCamConfig.sensorResetPin    = MV0212_MV0200_SENSOR_RIGHT_RST_GPIO;
#elif defined(MV0182)
   userCamConfig.sensorResetPin    = MV0182_MV0200_SENSOR_RIGHT_RST_GPIO;   //
#endif
   userCamConfig.stereoPairIndex   = CAM_B1_RIGHT_ADDR;                    //

   userCamConfig.windowColumnStart = CAM_WINDOW_START_COLUMN;
   userCamConfig.windowRowStart    = CAM_WINDOW_START_ROW;
   userCamConfig.windowWidth       = CAM_WINDOW_WIDTH;
   userCamConfig.windowHeight      = CAM_WINDOW_HEIGHT;
   userCamConfig.generateSync      = NULL;

   //synchronize local buffers spec with driver configuration
   camFrameSpec.width   = CAM_WINDOW_WIDTH;
   camFrameSpec.height  = CAM_WINDOW_HEIGHT;
   camFrameSpec.bytesPP = CAM_BPP;
   camFrameSpec.stride  = CAM_WINDOW_WIDTH * CAM_BPP;
   camFrameSpec.type    = RAW16;

   for (ibuf = 0; ibuf < MAX_USED_BUF;ibuf++)
   {
      camFrame[ibuf].spec  = camFrameSpec;
      camFrame[ibuf].p1    = (unsigned char*)camBuf[ibuf];
      camFrame[ibuf].p2    = NULL;
      camFrame[ibuf].p3    = NULL;
   }
   return;
}


// Build a pipeline with two independent paths, one for every camera
void appBuildPipeline()
{
   pl      = sippCreatePipeline(0, 2, SIPP_MBIN(mbinImgSipp));

   //B&W implementation : use look-up tables for conversion from RAW10 -> RAW8 (both stored on 16bits)
   // + add a neutral 0x80 pattern in the unused byte, in order to obtain YUV422i (storage 16bits)

   dmaIn       = sippCreateFilter(pl, 0x00,     CAM_WINDOW_WIDTH, CAM_WINDOW_HEIGHT, N_PL(1), SZ(UInt16),  SIPP_AUTO, (FnSvuRun)SIPP_DMA_ID, "DMA_IN");
   lut         = sippCreateFilter(pl, 0x00,     CAM_WINDOW_WIDTH, CAM_WINDOW_HEIGHT, N_PL(1), SZ(UInt16),  SIPP_AUTO, (FnSvuRun)SIPP_LUT_ID, "SIPP_LUT");
   dmaOut      = sippCreateFilter(pl, 0x00,     CAM_WINDOW_WIDTH, CAM_WINDOW_HEIGHT, N_PL(1), SZ(UInt16),  SIPP_AUTO, (FnSvuRun)SIPP_DMA_ID, "DMA_OUT");

   sippLinkFilter(lut   , dmaIn, 1, 1);
   sippLinkFilter(dmaOut, lut  , 1, 1);
}


void appConfigureFilters(void)
{
   dmaInCfg  = (DmaParam*)dmaIn->params;
   dmaOutCfg = (DmaParam*)dmaOut->params;
   lutCfg    = (LutParam*)lut->params;

   dmaInCfg->ddrAddr    = (UInt32)&camBuf[0];
   dmaOutCfg->ddrAddr   = (UInt32)&usbBufPtr[0][PAYLOAD_HEADER_OFFSET];

   //LUT config
   lutCfg->cfg   =        (0      )  | //LUT type: 0 for integers
                          (0  << 1)  | //channel mode : OFF, as we use a single plane here
                          (10 << 3)  | //integer input width: 10 bits
                          (0  << 8);   //numer of LUT tables: a single table (with 16 ranges/regions)
   lutCfg->sizeA =         0x44444444; // LUT size: 2^4= 16 entries/range, 16 entries x 16 ranges = 256 total entries in LUT
   lutCfg->sizeB =         0x44444444; //
   lutCfg->lut   =         lutTable;

   configureLutFilterSiliconSpecific(lutCfg);
}

frameBuffer* AllocateNextCamFrameBuf(void)
{
   ++newCamFrameCtr;
   return (&camFrame[newCamFrameCtr % MAX_USED_BUF]);
}

tyTimeStamp mySIPPTimer;
u64 cyclesSIPP;
volatile float sippTimePassed;

int main()
{
    s32 status;
    camErrorType camStatus;
    s32 boardStatus;
    u32 newFrame = FRAME_READY;

    status = initClocksAndMemory();
    if(status)
        return status;
#ifdef MV0212
    int32_t rc;
    uint32_t rev;
    BoardI2CInfo info[NUM_I2C_DEVS];
    BoardConfigDesc config[] =
    {
        {
            BRDCONFIG_GPIO,
            // use this for the sake of testing as it has the same gpio config as the MV0212 R0
            (void *)brdMV0182R5GpioCfgDefault
        },
        {
            BRDCONFIG_END,
            NULL
        }
    };

    rc = BoardInit(config);
    if (rc!=BRDCONFIG_SUCCESS)
    {
    	printf("Error: board initialization failed with %ld status\n",
    	    			rc);
        return rc;
    }

    rc = BoardGetPCBRevision(&rev);
    if (rc!=BRDCONFIG_SUCCESS)
    {
    	printf("Error: board configuration read failed with %ld status\n",
    	    			rc);
        return rc;
    }
    printf("Board Mv0212 initialized, revision = %lu \n", rev);

    boardStatus = BoardInitExtPll(EXT_PLL_CFG_148_24_24MHZ);
    if (boardStatus != BRDCONFIG_SUCCESS)
    {
    	printf("Error: board initialization failed with %ld status\n",
    			boardStatus);
    	return -1;
    }
    rc = BoardGetI2CInfo(info, NUM_I2C_DEVS);
    i2c0Handle=info[0].handler;
#elif defined(MV0182)
    boardStatus = BoardInitialise(EXT_PLL_CFG_148_24_24MHZ);
    i2c0Handle=gAppDevHndls.i2c0Handle;
    if (boardStatus != B_SUCCESS)
    {
    	printf("Error: board initialization failed with %ld status\n",
    			boardStatus);
    	return -1;
    }
#endif
    swcLeonSetPIL(0);

    generateLutTable();

    prepareDriverData();

    printf("Configuring the SIPP pipeline\n");
    sippInitialize();
    appBuildPipeline();
    appConfigureFilters();
    sippFinalizePipeline(pl);

    printf("Configuring imx208 camera and datapath\n");
    //start camera
    isrCallbacks.getBlock     = NULL;
    isrCallbacks.getFrame     = AllocateNextCamFrameBuf;
    isrCallbacks.notification = NULL;
    callbacks.isrCbfList      = &isrCallbacks;
    callbacks.sensorCbfList   = NULL;

    camStatus  = CamInit( &camHndl, staticCamConfig_HS, &userCamConfig, &callbacks, i2c0Handle);     // this is a SIPP camera
    if (camStatus != CAM_SUCCESS)
    {
        printf("\n Camera configuration failed (%d).\n", camStatus);
        return -1;
    }

    camStatus  = CamStart( &camHndl);
    if (camStatus != CAM_SUCCESS)
    {
        printf("\n Camera failed to start (%d).\n", camStatus);
        return -1;
    }

    processingFrameCtr = newCamFrameCtr;

    status = LeonIPCTxInit(&LRTtoLOSChannel, messagePool, MSG_QUEUE_SIZE, MSG_SIZE);
    if(status)
        return status;
    status = LeonIPCRxInit(&LOStoLRTChannel, usbAttached, IRQ_DYNAMIC_4, 5);
    if(status)
        return status;
    DrvLeonRTSignalBootCompleted();

    while(1)
    {
       if( newCamFrameCtr != processingFrameCtr )
       {
          processingFrameCtr = newCamFrameCtr;

          //do not process from/to same buffers as the ones used by CAM, but from/to shifted ones
          dmaInCfg->ddrAddr    = (UInt32)&camBuf[(newCamFrameCtr - 2) % MAX_USED_BUF];
          dmaOutCfg->ddrAddr   = (UInt32)&usbBufPtr[(usbFrameCtr + 1) % MAX_USED_BUF][PAYLOAD_HEADER_OFFSET];
          DrvTimerStartTicksCount(&mySIPPTimer);
          sippProcessFrame(pl);
          DrvTimerGetElapsedTicks(&mySIPPTimer,&cyclesSIPP);
          sippTimePassed=(float)DrvTimerTicksToMs(cyclesSIPP);
          usbFrameCtr++;
          LeonIPCSendMessage(&LRTtoLOSChannel, &newFrame);
       }
    }

    return 0;
}
