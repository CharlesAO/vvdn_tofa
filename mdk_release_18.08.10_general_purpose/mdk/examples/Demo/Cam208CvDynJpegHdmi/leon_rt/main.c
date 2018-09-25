///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

// 1: Includes
// ----------------------------------------------------------------------------
#include "lrt_app_config.h"
#include "assert.h"
#ifdef MV0212
#include "MV0212.h"
#include "brdGpioCfgs/brdMv0182R5GpioDefaults.h"
#elif defined(MV0182)
#include <Board182Api.h>
#endif
#include "LcdApi.h"
#include <stdio.h>
#include <string.h>
#include <DrvLeon.h>
#include "LeonIPCApi.h"
#include "DrvCDCEL.h"
#include "DrvCpr.h"
#include "DrvGpio.h"
#include "DrvMss.h"
#include <DrvTimer.h>
#include "registersMyriad.h"
#include <sipp.h>
#include <sippTestCommon.h>
#include "DrvADV7513.h"
#include "LcdCEA1080p60.h"
#include "CamGenericApi.h"
#include "imx208_2L_1936x1096_Raw10Converted8_60Hz.h"
#include "Cam208CvHdmiModel.h"
#include "commonDefine.h"

#include "leon/JpegEncoderApi.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

#define MAX_USED_BUF             3
#define FIRST_INCOMING_BUF_ID    1
#define FIRST_OUTGOING_BUF_ID    0

#define CAM_WINDOW_START_COLUMN      (0)
#define CAM_WINDOW_START_ROW         (8)           //8 is minimum for CB tests (first 8 lines contains "noise"; to be cropped, for CB CRC test)

#define CAM_WINDOW_WIDTH    1920
#define CAM_WINDOW_HEIGHT   1080

#define LCD_WINDOW_WIDTH    CAM_WINDOW_WIDTH
#define LCD_WINDOW_HEIGHT   CAM_WINDOW_HEIGHT
#define CAM_BPP 1    //RAW10
#define LCD_BPP 1    //YUV422

#define CAM_FRAME_SIZE_BYTES    (CAM_WINDOW_WIDTH * CAM_WINDOW_HEIGHT * CAM_BPP)      //2 bytes per pixel (only 10bits used) for RAW10
#define LCD_FRAME_SIZE_BYTES    (LCD_WINDOW_WIDTH * LCD_WINDOW_HEIGHT * LCD_BPP)      //3 bpp for RGB888 ( RGB888 -> YUV422 conversion performed in LCD - CSC enabled)

#define DDR_AREA                 __attribute__((section(".ddr.bss")))
#define DDR_UNCACHED             __attribute__((section(".ddr_direct.bss")))

#ifdef MV0212
#define NUM_I2C_DEVS 3
#endif

#define BUFF_SIZE_SHAVE 32*1024

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
static I2CM_Device * i2c0Handle;
static I2CM_Device * i2c2Handle;

leonIPCChannel_t __attribute__((section(".cmx_direct.data"))) LOStoLRTChannel;
uint32_t __attribute__((section(".cmx_direct.data"))) received;

uint32_t __attribute__((section(".cmx_direct.data"))) saveJpeg;

//jpg buffer should max out at 8.25bits * width * height
uint8_t DDR_AREA jpgbuff[LCD_WINDOW_WIDTH * LCD_WINDOW_HEIGHT * 2];

volatile jpegFrameBuffer DDR_UNCACHED  *pToSdcardBuf;
// 4: Static Local Data
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
GenericCamSpec *camConfig = &imx208_2L_1936x1096_RAW10CONV8_60Hz_camCfg;

volatile ALIGNED(4) u32 newCamFrameCtr = FIRST_INCOMING_BUF_ID;
volatile ALIGNED(4) u32 lcdFrameCtr    = FIRST_OUTGOING_BUF_ID;
volatile ALIGNED(4) u32 lcdFrameCnt = 0;

u8 DDR_AREA ALIGNED(8) camBuf[MAX_USED_BUF][CAM_FRAME_SIZE_BYTES];
u8 DDR_AREA ALIGNED(8) lcdBuf[MAX_USED_BUF][LCD_FRAME_SIZE_BYTES];
u8 DDR_AREA  lcdDummyChroma[LCD_FRAME_SIZE_BYTES];

u8 DDR_AREA  toSdcardBuf[LCD_FRAME_SIZE_BYTES];

uint32_t iBuf[2];
uint32_t oBuf[2];

#define LAST_SVU 6
#define SVU_MASK 0x7F //shaves [0..6]
ADV7513ContfigMode_t ADV7513Config = ADV7513_1080P60;
// ISP Pipeline interface
int                  dbgLevel = 1; //SIPP debug level

GenericCameraHandle          camHndl;
CamUserSpec                  userConfig;
frameSpec                    camFrameSpec;
frameBuffer                  camFrame[MAX_USED_BUF];

frameBuffer          sdCardFrame;
volatile jpegFrameBuffer DDR_UNCACHED  jpeg;


callbacksListStruct          callbacks = {0};
interruptsCallbacksListType  isrCallbacks = {0};


frameSpec            lcdFrameSpec;
frameBuffer          lcdFrame[MAX_USED_BUF];

LCDHandle            lcdHndl;

volatile u32 ALIGNED(4) processingFrameCtr;


// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
frameBuffer* AllocateNextCamFrameBuf(void);

// 6: Functions Implementation
// ----------------------------------------------------------------------------
void prepareDriverData(void)
{
    int ibuf;

    //user extra settings
    userConfig.mipiControllerNb  = CAM_B1_MIPICTRL;                     //use the first camera connected to B1 connecter of the board
    userConfig.receiverId        = SIPP_DEVICE1;                        //CIF_DEVICE1
#ifdef MV0212
    userConfig.sensorResetPin    = MV0212_MV0200_SENSOR_RIGHT_RST_GPIO;
#elif defined(MV0182)
    userConfig.sensorResetPin    = MV0182_MV0200_SENSOR_RIGHT_RST_GPIO;   //
#endif
    userConfig.stereoPairIndex   = CAM_B1_RIGHT_ADDR;                    //

    userConfig.windowColumnStart = CAM_WINDOW_START_COLUMN;
    userConfig.windowRowStart    = CAM_WINDOW_START_ROW;
    userConfig.windowWidth       = CAM_WINDOW_WIDTH;
    userConfig.windowHeight      = CAM_WINDOW_HEIGHT;
    userConfig.generateSync      = NULL;

    //synchronise local buffers spec with driver configuration
    camFrameSpec.width   = CAM_WINDOW_WIDTH;
    camFrameSpec.height  = CAM_WINDOW_HEIGHT;
    camFrameSpec.bytesPP = CAM_BPP;
    camFrameSpec.stride  = CAM_WINDOW_WIDTH * CAM_BPP;
    camFrameSpec.type    = RAW16;

    //prepare output buffers
    for (ibuf = 0; ibuf < MAX_USED_BUF;ibuf++)
    {
        camFrame[ibuf].p1    = (unsigned char*)camBuf[ibuf];
        camFrame[ibuf].p2    = NULL;
        camFrame[ibuf].p3    = NULL;
        camFrame[ibuf].spec  = camFrameSpec;
    }

    isrCallbacks.getBlock     = NULL;
    isrCallbacks.getFrame     = AllocateNextCamFrameBuf;
    isrCallbacks.notification = NULL;
    callbacks.isrCbfList      = &isrCallbacks;
    callbacks.sensorCbfList   = NULL;


    //init LCD data
    lcdFrameSpec.width   = LCD_WINDOW_WIDTH;
    lcdFrameSpec.height  = LCD_WINDOW_HEIGHT;
    lcdFrameSpec.stride  = LCD_WINDOW_WIDTH * LCD_BPP;
    lcdFrameSpec.bytesPP = LCD_BPP;
    lcdFrameSpec.type    = YUV422p;

    for (ibuf = 0; ibuf < MAX_USED_BUF; ibuf++)
    {
        lcdFrame[ibuf].spec = lcdFrameSpec;  //the resulted YUV out of RGB2YUV SW filter is packed (linear)
        lcdFrame[ibuf].p1   = &lcdBuf[ibuf][0];
        lcdFrame[ibuf].p2   = &lcdDummyChroma[0];
        lcdFrame[ibuf].p3   = &lcdDummyChroma[LCD_WINDOW_WIDTH * LCD_WINDOW_HEIGHT / 2];
    }

    return;
}

frameBuffer* AllocateNextCamFrameBuf(void)
{
    ++newCamFrameCtr;
    return ( &camFrame[newCamFrameCtr % MAX_USED_BUF] );
}

frameBuffer* allocateLcdFrame(int layer)
{
    (void) layer;// "use" the variables to hush the compiler warning.

    lcdFrameCnt++;
    return ( &lcdFrame[lcdFrameCtr  % MAX_USED_BUF] );
}

void configSippModelApp(Cam208CvHdmiModel *pPl, UInt32 inBuff, UInt32 outBuff)
{
   DmaParam            *dmaIn0Cfg          = (DmaParam         *)pPl->dmaIn0->params;
   DmaParam            *dmaOut0Cfg         = (DmaParam         *)pPl->dmaOut0->params;

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // here go through all the filter instances and assign the value for all parameter
  // fields as defined in the filter property window
   dmaIn0Cfg->ddrAddr   = inBuff;
   dmaOut0Cfg->ddrAddr  = outBuff;
}

void ReceiveIPC(struct leonIPCChannel_t *channel)
{
    uint32_t msgNo, i;
    s32 sc;
    LeonIPCNumberOfPendingMessages(channel, &msgNo);
    for (i = 0; i < msgNo; i++)
    {
        sc = LeonIPCReadMessage(channel, &received);
        if (sc == IPC_SUCCESS)
        {
            switch (received)
            {
              case 0xB0050001:
                saveJpeg=1;
                break;
              case 0xB0050002:
                break;
              default:
                printf("Received 0x%8lX\n",received);
                break;
            }
        }
        else
        {
            exit(sc);
        }
    }

}

int main()
{
    Cam208CvHdmiModel sipp;
    u32 status;
    s32 boardStatus;
    camErrorType camStatus;
    LCDLayerOffset lcdLayerStartOffset = {0, 0};
    s32 sc;

    pToSdcardBuf = NULL;

    sc = initClocksAndMemory();
    if(sc)
        return sc;

    DrvCprStartAllClocks();

    saveJpeg=0;
    jpeg.buf = jpgbuff;
    jpeg.size = sizeof(jpgbuff);

    status = LeonIPCRxInit(&LOStoLRTChannel, &ReceiveIPC, IRQ_DYNAMIC_5, 5);
    if(status) return status;
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
    i2c2Handle=info[2].handler;
#elif defined(MV0182)
    boardStatus = BoardInitialise(EXT_PLL_CFG_148_24_24MHZ);
    i2c0Handle=gAppDevHndls.i2c0Handle;
    i2c2Handle=gAppDevHndls.i2c2Handle;
    if (boardStatus != B_SUCCESS)
    {
    	printf("Error: board initialization failed with %ld status\n",
    			boardStatus);
    	return -1;
    }
#endif
    swcLeonSetPIL(0);

    prepareDriverData();
    memset(lcdDummyChroma, 0x80, sizeof(lcdDummyChroma)); //prefill the common UV buffer of the output YUV422p with constant neutral pattern(luma Y plane will be overwritten by the SIPP DMA);
    //this is acceptable for the imx208 sensor, which output grayscale image, so only luma is valuable, the chroma can be neutral.
    printf("Initializing SIPP pipe\n");
    sippInitialize();

    buildCam208CvHdmiModel(&sipp);
    iBuf[0] = (UInt32)&camBuf[0]; //Mandatory, before ipipeConfigVideo
    oBuf[0] = (UInt32)&lcdBuf[0]; //Luma
    configCam208CvHdmiModel(&sipp);

    sippFinalizePipeline(sipp.pl);

    //connect the LCD to output parallel bus (by GPIO) instead of MIPI Tx
    DrvMssConnectLcdToGpio();

    printf("Configuring camera and datapath\n");
    camStatus = CamInit(&camHndl, camConfig, &userConfig, &callbacks, i2c0Handle);

    if (camStatus != CAM_SUCCESS)
    {
        printf("\n Camera configuration failed (%d).\n", camStatus);
        return -1;
    }

    printf(" Starting camera ...\n");
    camStatus = CamStart(&camHndl);

    if (camStatus != CAM_SUCCESS)
    {
        printf("\n Camera failed to start (%d).\n", camStatus);
        return -1;
    }

    printf("Configuring HDMI chip ADV7513...\n\n");
    status = initADV7513reg(i2c2Handle, ADV7513Config);

    if (status == 0)
        printf("ADV7513 Configured\n");
    else {
        printf("ADV7513 Configuration failed %lu \n", status);
        return -2;
    }

    LCDInit(&lcdHndl, &lcdSpec1080p60, NULL, LCD1);
    LCDInitLayer(&lcdHndl, VL1, &lcdFrameSpec, lcdLayerStartOffset);
    LCDSetupCallbacks(&lcdHndl, &allocateLcdFrame, NULL, NULL, NULL);

    printf("Starting LCD...\n");
    LCDStart(&lcdHndl);

    processingFrameCtr = newCamFrameCtr;
    printf("Loop ...\n");
    DrvLeonRTSignalBootCompleted();

    while (1)
    {
        if (newCamFrameCtr != processingFrameCtr) {

            processingFrameCtr = newCamFrameCtr;
            configSippModelApp(&sipp, (UInt32)&camBuf[(processingFrameCtr -2) % MAX_USED_BUF], (UInt32)&lcdBuf[(lcdFrameCtr + 1) % MAX_USED_BUF]);
            sippProcessFrame(sipp.pl);
            lcdFrameCtr++;

            if ((saveJpeg) && (pToSdcardBuf==NULL)) {

                for(int i=0;i<12;i++){
                swcWaitShave(i);
                }

                sippStopSvus(sipp.pl);

                memcpy(toSdcardBuf,lcdFrame[(lcdFrameCtr + 1) % MAX_USED_BUF].p1,LCD_FRAME_SIZE_BYTES);
                sdCardFrame.p1=toSdcardBuf;
                sdCardFrame.p2=lcdDummyChroma;
                sdCardFrame.p3=lcdDummyChroma;
                sdCardFrame.spec=lcdFrameSpec;

                saveJpeg=0;
                printf("Start encoding JPEG\n");
                JPEG_encode(sdCardFrame, jpgbuff, NB_SHAVES_FOR_JPEGENC, BUFF_SIZE_SHAVE, JPEG_422_PLANAR);
                printf("Ended encoding JPEG\n");

                printf("Triggering sdCard writing on LeonOS\n");
                pToSdcardBuf = &jpeg;

                sippReschedulePipeline(sipp.pl);
            }
        }
    } //while
    return 0;
}
