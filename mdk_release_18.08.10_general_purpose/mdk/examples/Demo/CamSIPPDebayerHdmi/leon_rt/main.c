///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mv_types.h"
#include "DrvDdr.h"
#include <DrvTimer.h>
#include "brdMv0182.h"

#include "app_config.h"
// Include sipp graph designer generated interface header
#include "Cam214SIPPDebayerHdmi.h"
// Include ISP configuration header
#include "ISPPipeSettings.h"

#include <DrvGpio.h>
#include "brdGpioCfgs/brdMv0182GpioDefaults.h"

// Include Dev board headers:
#ifdef MV0212
    #include "MV0212.h"
    #include "brdGpioCfgs/brdMv0182R5GpioDefaults.h"
#elif defined(MV0182)
    #include <Board182Api.h>
#endif

// Include Cam headers:
#include "imx214_2L_2104x1560_Raw10_30Hz_binning.h"
#include "imx378_4L_4056x3040_Raw10_30Hz.h"
#include "CamGenericApi.h"

// Include LCD headers:
#include "LcdApi.h"
#include "DrvADV7513.h"    //HDMI controller
#include "LcdCEA1080p60.h"

#include "DrvCDCEL.h"      // included for the value of define: EXT_PLL_CFG_148_24_24MHZ

/*                 [ Defines ]                             */
// Define frame buffer size
#define NR_OF_USED_CAM_BUFS             4
#define NR_OF_USED_LCD_BUFS             4

// Defines used to specify input image format and buffer size required
#define CAM_WINDOW_START_COLUMN      0
#define CAM_WINDOW_START_ROW         0
#define CAM_IMAGE_WIDTH_IMX214      1920
#define CAM_IMAGE_HEIGHT_IMX214     1080
#define CAM_IMAGE_WIDTH_IMX378      4032
#define CAM_IMAGE_HEIGHT_IMX378     2376
#define CAM_BPP              2
#define CAM_BUFFER_SIZE_IMX214   ((CAM_IMAGE_WIDTH_IMX214 * CAM_IMAGE_HEIGHT_IMX214) * CAM_BPP)
#define CAM_BUFFER_SIZE_IMX378   ((CAM_IMAGE_WIDTH_IMX378 * CAM_IMAGE_HEIGHT_IMX378) * CAM_BPP)


// Defines used to specify output image format and buffer size required
#define LCD_IMAGE_WIDTH      1920
#define LCD_IMAGE_HEIGHT     1080
#define LCD_BPP              1
#define LCD_OUTPUT_FORMAT    YUV420p
// Use the CAM_IMAGE_* width/height to calculate buffer size.
#define LCD_Y_BUFFER_SIZE   ((LCD_IMAGE_WIDTH * LCD_IMAGE_HEIGHT) * LCD_BPP)
#define LCD_U_BUFFER_SIZE   (((LCD_IMAGE_WIDTH * LCD_IMAGE_HEIGHT) * LCD_BPP)/4)
#define LCD_V_BUFFER_SIZE   (((LCD_IMAGE_WIDTH * LCD_IMAGE_HEIGHT) * LCD_BPP)/4)
#define LCD_IMAGE_STRIDE    (LCD_IMAGE_WIDTH * LCD_BPP)
// I/O port's
#ifdef MV0212
#define NUM_I2C_DEVS 3
#endif

/*                 [ Global variables ]                             */
// Choose CAM configuration:
GenericCamSpec *camConfig;
GenericCameraHandle          camHndl;
CamUserSpec                  userConfig;
frameSpec                    camFrameSpec;
callbacksListStruct          callbacks = {0};
interruptsCallbacksListType  isrCallbacks = {0};
u8 sensor=0;
// Choose LCD configuration:
ADV7513ContfigMode_t ADV7513Config = ADV7513_1080P60;
LCDHandle                    lcdHndl;
frameSpec                    lcdFrameSpec;

// Camera buffer declaration, stored in ddr and aligned for maximum cache efficiency:
ALIGNED(64) DDR_AREA UInt8     camBuf[NR_OF_USED_CAM_BUFS][CAM_BUFFER_SIZE_IMX378];

frameBuffer        camFrame[NR_OF_USED_CAM_BUFS];

// LCD out buffer declaration, stored in ddr and aligned for maximum cache efficiency:
ALIGNED(64) DDR_AREA UInt8     lcdBufY[NR_OF_USED_LCD_BUFS][LCD_Y_BUFFER_SIZE];
ALIGNED(64) DDR_AREA UInt8     lcdBufU[NR_OF_USED_LCD_BUFS][LCD_U_BUFFER_SIZE];
ALIGNED(64) DDR_AREA UInt8     lcdBufV[NR_OF_USED_LCD_BUFS][LCD_V_BUFFER_SIZE];

frameBuffer        lcdFrame[NR_OF_USED_LCD_BUFS];

// Variable to store current cam frame index
ALIGNED(64) volatile u32 CamFrameCtr     = 1;
// Variable to store current lcd frame index
ALIGNED(64) volatile u32 LcdFrameCtr     = 0;


// Variable to store SIPP pipe configuration
ALIGNED(64) Cam214SIPPDebayerHdmi myPl;

// Flag to signal that the cam frame writing is done
volatile int camFrameReceivedSync = 0;

static u8 protocolReadSample2[] = I2C_PROTO_READ_16BA;
struct specs
{
  int width;
  int height;
};
struct specs camSpec;

/*                 [ Function prototypes ]                             */
frameBuffer* AllocateNextCamFrameBuf(void);
void* CheckCamNotification(u32 notificationType);

/*                 [ Function definitions ]                             */

void ConfigureCamLcdDriver(void)
{
    int ibuf;

    //user extra settings
    userConfig.mipiControllerNb  = CAM_A1_MIPICTRL;                //use the first camera connected to A1 connecter of the board
    userConfig.receiverId        = CIF_DEVICE0;                    //
#ifdef MV0212
    userConfig.sensorResetPin    = MV0212_MV0201_SENSOR_RST_GPIO;
#elif defined(MV0182)
    userConfig.sensorResetPin    = MV0182_MV0201_SENSOR_RST_GPIO;   //
#endif
    // userConfig.stereoPairIndex   = CAM_A_ADDR;              //
    userConfig.windowColumnStart = CAM_WINDOW_START_COLUMN;
    userConfig.windowRowStart    = CAM_WINDOW_START_ROW;
    userConfig.windowWidth       = camSpec.width;
    userConfig.windowHeight      = camSpec.height;
    userConfig.generateSync      = NULL;

    //synchronize local buffers spec with user config and driver configuration
    camFrameSpec.width   = camSpec.width;
    camFrameSpec.height  = camSpec.height;
    camFrameSpec.bytesPP = CAM_BPP;
    camFrameSpec.stride  = camSpec.width * CAM_BPP;
    camFrameSpec.type    = RAW16;

    // Init cam buffer
    memset(camBuf,0,sizeof(camBuf));

    //prepare output buffers
    for (ibuf = 0; ibuf < NR_OF_USED_CAM_BUFS; ibuf++)
    {
        camFrame[ibuf].p1    = (unsigned char*)&camBuf[ibuf][0];
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
    lcdFrameSpec.width   = LCD_IMAGE_WIDTH;
    lcdFrameSpec.height  = LCD_IMAGE_HEIGHT;
    lcdFrameSpec.stride  = LCD_IMAGE_STRIDE;
    lcdFrameSpec.bytesPP = LCD_BPP;
    lcdFrameSpec.type    = LCD_OUTPUT_FORMAT;

    // Init lcd buffers
    memset(lcdBufY,0,sizeof(lcdBufY));
    memset(lcdBufU,0,sizeof(lcdBufU));
    memset(lcdBufV,0,sizeof(lcdBufV));

    for (ibuf = 0; ibuf < NR_OF_USED_LCD_BUFS; ibuf++)
    {
        lcdFrame[ibuf].spec = lcdFrameSpec;  //the resulted YUV out of RGB2YUV SW filter is packed
        lcdFrame[ibuf].p1   = (unsigned char *) &lcdBufY[ibuf][0];
        lcdFrame[ibuf].p2   = (unsigned char *) &lcdBufU[ibuf][0];
        lcdFrame[ibuf].p3   = (unsigned char *) &lcdBufV[ibuf][0];
    }

    return;
}

void configIOBuffer(Cam214SIPPDebayerHdmi *pVideo, UInt8 *inputBuf,
                                               UInt8 *outputYBuf,
                                               UInt8 *outputUBuf,
                                               UInt8 *outputVBuf)
{
    DmaParam *dmaInCfg  = (DmaParam *)pVideo->dmaIn0->params;
    DmaParam *dmaOutYCfg = (DmaParam *)pVideo->dmaOutY->params;
    DmaParam *dmaOutUCfg = (DmaParam *)pVideo->dmaOutU->params;
    DmaParam *dmaOutVCfg = (DmaParam *)pVideo->dmaOutV->params;

   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   //Dma-In params
    dmaInCfg->ddrAddr  = (UInt32)inputBuf;

   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   //Dma-Out params
    dmaOutYCfg->ddrAddr = (UInt32)outputYBuf;
    dmaOutUCfg->ddrAddr = (UInt32)outputUBuf;
    dmaOutVCfg->ddrAddr = (UInt32)outputVBuf;
}

#ifdef MV0182
static u32 I2CErrorHandlerIgnore(I2CM_StatusType i2cCommsError, u32 __attribute__((unused)) slaveAddr, u32 __attribute__((unused)) regAddr)
{
   UNUSED(i2cCommsError);
    return I2CM_STAT_OK;
}
#endif
/*****************  Call back functions ****************/
frameBuffer* AllocateNextCamFrameBuf(void)
{
    ++CamFrameCtr;
    camFrameReceivedSync = 1;
    return ( &camFrame[CamFrameCtr % NR_OF_USED_CAM_BUFS] );
}

frameBuffer* allocateLcdFrame(int layer)
{
    UNUSED(layer);
    return ( &lcdFrame[(LcdFrameCtr-2) % NR_OF_USED_LCD_BUFS] );
}

/**************** MAIN ***************/
int main()
{
    // Var decl and init
    I2CM_Device * i2c1Handle;
    I2CM_Device * i2c2Handle;

    LCDHandle lcdHndl;
    LCDLayerOffset lcdLayerStartOffset = {0, 0};

    camErrorType  camStatus;
    s32           boardStatus;
    u8            hdmiStatus;
    unsigned int camID  = 0;
    int statusI2c=0;
    u8 bytes[2];

    // Inform lrt of the system clock
    init();

   swcLeonSetPIL(0);

#ifdef MV0212
    int32_t rc;
    uint32_t rev;
    BoardI2CInfo info[NUM_I2C_DEVS];
    BoardConfigDesc config[] =
    {
        {
            BRDCONFIG_GPIO,
            // Use this for the sake of testing as it has the same gpio config as the MV0212 R0
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
        printf("Error: mv0212 board initialization failed with %ld status\n",
                        rc);
        return rc;
    }

    rc = BoardGetPCBRevision(&rev);
    if (rc!=BRDCONFIG_SUCCESS)
    {
        printf("Error: mv0212 board configuration read failed with %ld status\n",
                        rc);
        return rc;
    }
    printf("Board Mv0212 initialized, revision = %lu \n", rev);

    boardStatus = BoardInitExtPll(EXT_PLL_CFG_148_24_24MHZ);
    if (boardStatus != BRDCONFIG_SUCCESS)
    {
        printf("Error: mv0212 board ext pll initialization failed with %ld status\n",
                boardStatus);
        return -1;
    }
    rc = BoardGetI2CInfo(info, NUM_I2C_DEVS);
    i2c1Handle=info[1].handler;
    i2c2Handle=info[2].handler;
#elif defined(MV0182)
    printf("Board initialize\n");
    DrvGpioInitialiseRange(brdMV0182GpioCfgDefault);
    boardStatus = BoardInitialise(EXT_PLL_CFG_148_24_24MHZ);
    i2c1Handle=gAppDevHndls.i2c1Handle;
    i2c2Handle=gAppDevHndls.i2c2Handle;
    if (boardStatus != B_SUCCESS)
    {
        printf("Error: mv0182 board initialization failed with %ld status\n",
                boardStatus);
        return -1;
    }
    printf("Board initialized\n");
    DrvI2cMSetErrorHandler(i2c1Handle,I2CErrorHandlerIgnore);
#endif

    printf("Configuring camera and lcd drivers\n");
    /*Camera sensor detection*/
    DrvGpioSetPin(MV0182_MV0201_SENSOR_RST_GPIO, 1);
    DrvTimerSleepMs(5);
    DrvGpioSetPin(MV0182_MV0201_SENSOR_RST_GPIO, 0);
    DrvTimerSleepMs(5);
    DrvGpioSetPin(MV0182_MV0201_SENSOR_RST_GPIO, 1);
    DrvTimerSleepMs(5);
    //imx378
    statusI2c=DrvI2cMTransaction(i2c1Handle, IMX378_I2C_ADDRESS, IMX378_REG_ID, protocolReadSample2, bytes, 2);
    camID=bytes[0]<<8 | (bytes[1]);

    if ((statusI2c != I2CM_STAT_OK) || camID != IMX378_ID)
    {
      camID=0;
    }
    //imx214
    if(camID==0)
    {
    statusI2c=DrvI2cMTransaction(i2c1Handle, IMX214_I2C_ADDRESS, IMX214_REG_ID, protocolReadSample2, bytes, 2);
    camID=bytes[0]<<8 | (bytes[1]);
    }
    if (statusI2c != I2CM_STAT_OK)
    {
       printf("\n Camera sensor read fail with status = (%d).\n", statusI2c);
       camID=0;
       return -1;
    }

    switch(camID) {

    case IMX378_ID :
    printf("Camera sensor: IMX378 \n");
       sensor=2;
       camConfig = &imx378_4L_4056x3040_RAW10_30Hz_camCfg;
       camSpec.width=    CAM_IMAGE_WIDTH_IMX378;
       camSpec.height=   CAM_IMAGE_HEIGHT_IMX378;
      break;

    case IMX214_ID :
       printf("Camera sensor: IMX214 \n");
       sensor=1;
       camConfig = &imx214_2L_2104x1560_RAW10_30Hz_binn_camCfg;
       camSpec.width=    CAM_IMAGE_WIDTH_IMX214;
       camSpec.height=   CAM_IMAGE_HEIGHT_IMX214;
      break;

    default :
       printf("Camera sensor is not supported for this board!\n");
    return -1;
    }

    ConfigureCamLcdDriver();

    camStatus = CamInit( &camHndl, camConfig, &userConfig, &callbacks, i2c1Handle );
    if (camStatus != CAM_SUCCESS)
    {
        printf("\n Camera configuration failed (%d).\n", camStatus);
        return -1;
    }

    camStatus = CamStart( &camHndl );
    if (camStatus != CAM_SUCCESS)
    {
        printf("\n Camera failed to start (%d).\n", camStatus);
        return -1;
    }
    printf("Configuring the HDMI chip ADV7513\n");
    hdmiStatus = initADV7513reg(i2c2Handle, ADV7513Config);

    if (hdmiStatus == 0)
        printf("ADV7513 Configured\n");
    else{
        printf("ADV7513 Configuration failed %d \n", hdmiStatus);
        return -1;
    }
    printf("Configuring the LCD\n");
    LCDInit(&lcdHndl, &lcdSpec1080p60, NULL, LCD1);
    LCDInitLayer(&lcdHndl, VL1, &lcdFrameSpec, lcdLayerStartOffset);
    LCDSetupCallbacks(&lcdHndl, &allocateLcdFrame, NULL, NULL, NULL);

    camFrameReceivedSync = 0;
    while(camFrameReceivedSync == 0);

    printf("Init Sipp platform..\n");
    sippPlatformInit();

    printf("Build SIPP pipe..\n");
    buildCam214SIPPDebayerHdmi(&myPl);

    printf("Configure ISP parameters..\n");
    ISPPipeCreateParams();

    printf("Configure SIPP pipe..\n");
    configCam214SIPPDebayerHdmi(&myPl);

    printf("Configure initial sipp buffers \n");
    configIOBuffer(&myPl, camBuf[((CamFrameCtr - 2) % NR_OF_USED_CAM_BUFS)],
                          lcdBufY[(LcdFrameCtr % NR_OF_USED_LCD_BUFS)],
                          lcdBufU[(LcdFrameCtr % NR_OF_USED_LCD_BUFS)],
                          lcdBufV[(LcdFrameCtr % NR_OF_USED_LCD_BUFS)]);

    LCDStart(&lcdHndl);
    printf("Entering infinite loop..\n\n");
    while(1)
    {
        if (camFrameReceivedSync != 0)
        {
            camFrameReceivedSync = 0;

            LcdFrameCtr++;

            configIOBuffer(&myPl, camBuf[((CamFrameCtr - 2) % NR_OF_USED_CAM_BUFS)],
                                  lcdBufY[(LcdFrameCtr % NR_OF_USED_LCD_BUFS)],
                                  lcdBufU[(LcdFrameCtr % NR_OF_USED_LCD_BUFS)],
                                  lcdBufV[(LcdFrameCtr % NR_OF_USED_LCD_BUFS)]);

            sippProcessFrame(myPl.pl);
        }
    }

    return 0;
}

