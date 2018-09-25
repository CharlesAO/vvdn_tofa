/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    oPipe: low-light small resolution real-life params
///

#include <stdio.h>
#include <DrvCpr.h>
#include <DrvDdr.h>
#include <DrvTimer.h>
#include "Opipe.h"
#include "OpipeApps.h"
#include "isp_params_wrap.h"
#include "app_config.h"
#include "LcdApi.h"
#include "DrvCDCEL.h"
#include "DrvADV7513.h"
#include "DrvGpio.h"
#ifdef MV0212
#include "MV0212.h"
#include "brdGpioCfgs/brdMv0182R5GpioDefaults.h"
#elif defined(MV0182)
#include <Board182Api.h>
#endif
#include "imx214_2L_2104x1560_Raw10_30Hz_binning.h"
#include "imx378_4L_4056x3040_Raw10_30Hz.h"

#include "CamGenericApi.h"
#include "LcdCEA1080p60.h"
#include "brdMv0182.h"

// 2:  Source specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define MAX_USED_BUF             3   //minimum 2 for ping-pong
#define FIRST_INCOMING_BUF_ID    1   //this is in fact 2, because CamGeneric call once the getFrame fc. before start

#define CAM_BPP 2
#define LCD_BPP 1

//Input resolution
#define IMG_W_IMX214 ISPC_BAYER_W_IMX214
#define IMG_H_IMX214 ISPC_BAYER_H_IMX214

#define IMG_W_IMX378 ISPC_BAYER_W_IMX378
#define IMG_H_IMX378 ISPC_BAYER_H_IMX378

//Output resolution
#define O_IMG_W_IMX214  ((IMG_W_IMX214 * N -1)/D_IMX214 + 1)
#define O_IMG_H_IMX214  ((IMG_H_IMX214 * N -1)/D_IMX214 + 1)

#define O_IMG_W_IMX378  ((IMG_W_IMX378 * N -1)/D_IMX378 + 1)
#define O_IMG_H_IMX378  ((IMG_H_IMX378 * N -1)/D_IMX378 + 1)

//LCD resolution
#define LCD_WINDOW_WIDTH  1920
#define LCD_WINDOW_HEIGHT   1080

#ifdef MV0212
#define NUM_I2C_DEVS 3
#endif

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//Main full buffers in DDR
uint8_t  yuvBufY [MAX_USED_BUF][   O_IMG_W_IMX378*O_IMG_H_IMX378   ]    SECTION(".ddr.bss") ALIGNED(8);
uint8_t  yuvBufUV[MAX_USED_BUF][2*(O_IMG_W_IMX378*O_IMG_H_IMX378/4)]    SECTION(".ddr.bss") ALIGNED(8);
uint8_t  camBuf  [MAX_USED_BUF][IMG_W_IMX378 * IMG_H_IMX378 * CAM_BPP]  SECTION(".ddr.bss") ALIGNED(8);

//Circular bufffers in CMX
uint16_t oCircBuffCmx7[O_CBUFF_H *IMG_W_IMX378]   SECTION(".cmx.data") ALIGNED(16);
uint16_t cSigma  [  IMG_W_IMX378 * I_CBUFF_H    ] SECTION(".cmx.data") ALIGNED(16);  //Bayer is 10bit
uint16_t cDbyrY  [  IMG_W_IMX378 * DBYR_Y_H     ] SECTION(".cmx.data") ALIGNED(16);
uint16_t cSharpY [  IMG_W_IMX378 * SHARP_Y_H    ] SECTION(".cmx.data") ALIGNED(16);
uint8_t  cLut    [  IMG_W_IMX378 * O_CBUFF_H * 3] SECTION(".cmx.data") ALIGNED(16);
uint8_t  cUpfirDn[O_IMG_W_IMX378 * O_CBUFF_H * 3] SECTION(".cmx.data") ALIGNED(16);  //as for yuv444
u8 sensor=0;
OpipeMF opMF; // main or full pipe data struct
OpipeRx pRx;  // MipiRx->DDR

static GenericCamSpec *camConfig;
static ADV7513ContfigMode_t ADV7513Config = ADV7513_1080P60;

volatile u32 newCamFrameCtr = FIRST_INCOMING_BUF_ID;

static GenericCameraHandle  camHndl;
static CamUserSpec          userConfig;
static frameSpec            camFrameSpec;
static frameBuffer          camFrame[MAX_USED_BUF];
static frameSpec            lcdFrameSpec;
static frameBuffer          lcdFrame[MAX_USED_BUF];

static callbacksListStruct          callbacks = {0};

static u8 protocolReadSample2[] = I2C_PROTO_READ_16BA;


struct specs
{
  int width;
  int height;
  int o_width;
  int o_height;
};
struct specs camSpec;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
extern void appSetParamsMFPipe(Opipe *p,int sensor);
extern void appSetParamsRxPipe(Opipe *p);

// 6: Functions Implementation
// ----------------------------------------------------------------------------

//#################################################################################################
void appBuildPipeMF()
{
 //Must specify buffers first
   opMF.in.cBufSigma.base = (uint32_t)cSigma;   opMF.in.cBufSigma.h = I_CBUFF_H;
   opMF.cBufDbyrY.base    = (uint32_t)cDbyrY;   opMF.cBufDbyrY.h    = DBYR_Y_H;
   opMF.cBufSharp.base    = (uint32_t)cSharpY;  opMF.cBufSharp.h    = SHARP_Y_H;
   opMF.cBufLut.base      = (uint32_t)cLut;     opMF.cBufLut.h      = O_CBUFF_H;

   #if (CANCEL_UPFIRDN)
   opMF.cBufPoly.base     = 0x00000000;
   #else
   opMF.cBufPoly.base     = (uint32_t)cUpfirDn;   opMF.cBufPoly.h     = O_CBUFF_H;
   #endif

   OpipeCreateFull(&opMF, BPP(2));
}

static void prepareDriverData()
{
  int ibuf;

  //user extra settings
  userConfig.mipiControllerNb  = CAM_A1_MIPICTRL;
  userConfig.receiverId        = SIPP_DEVICE0;
#ifdef MV0212
    userConfig.sensorResetPin    = MV0212_MV0201_SENSOR_RST_GPIO;
#elif defined(MV0182)
    userConfig.sensorResetPin    = MV0182_MV0201_SENSOR_RST_GPIO;
#endif
  userConfig.stereoPairIndex   = CAM_A_ADDR;
  userConfig.generateSync      = NULL;

  //synchronize local buffers spec with driver configuration
  camFrameSpec.width   = camSpec.width;
  camFrameSpec.height  = camSpec.height;
  camFrameSpec.bytesPP = CAM_BPP;
  camFrameSpec.stride  = camSpec.width * CAM_BPP;
  camFrameSpec.type    = RAW16;

  //prepare output buffers
  for (ibuf = 0; ibuf < MAX_USED_BUF;ibuf++)
  {
     camFrame[ibuf].p1    = (unsigned char*)camBuf[ibuf];
     camFrame[ibuf].p2    = NULL;
     camFrame[ibuf].p3    = NULL;
     camFrame[ibuf].spec  = camFrameSpec;
  }

  callbacks.isrCbfList      = NULL;
  callbacks.sensorCbfList   = NULL;

  //initialize the LCD data
  lcdFrameSpec.width   = LCD_WINDOW_WIDTH;
  lcdFrameSpec.height  = LCD_WINDOW_HEIGHT;
  lcdFrameSpec.stride  = camSpec.o_width * LCD_BPP;
  lcdFrameSpec.bytesPP = LCD_BPP;
  lcdFrameSpec.type    = YUV420p;

  for (ibuf = 0; ibuf < MAX_USED_BUF;ibuf++)
  {
      lcdFrame[ibuf].spec = lcdFrameSpec;
      lcdFrame[ibuf].p1   = &yuvBufY[ibuf][0];
      lcdFrame[ibuf].p2   = &yuvBufUV[ibuf][0];                  // since the frame type is YUV420p, the first plane in the buffer is U
      lcdFrame[ibuf].p3   = &yuvBufUV[ibuf][camSpec.o_width*camSpec.o_height/4];  // since the frame type is YUV420p, the second plane in the buffer is V
  }

  return;
}
#ifdef MV0182
static u32 I2CErrorHandlerIgnore(I2CM_StatusType i2cCommsError, u32 __attribute__((unused)) slaveAddr, u32 __attribute__((unused)) regAddr)
{
   UNUSED(i2cCommsError);
    return I2CM_STAT_OK;
}
#endif
frameBuffer* allocateLcdFrame(int layer)
{
    (void) layer;// "use" the variables to hush the compiler warning.

   return ( &lcdFrame[(newCamFrameCtr - 1) % MAX_USED_BUF] );
}

void RxEofCallback(Opipe *p)
{
    (void) p;// "use" the variables to hush the compiler warning.

  newCamFrameCtr++;
    pRx.pOut->ddr.base = (uint32_t)camBuf[newCamFrameCtr % MAX_USED_BUF]; //next out buffers
    OpipeStart(&pRx.p); //restart...

}

//#################################################################################################
int main()
{
   u8 status;
   LCDHandle lcdHndl;
   LCDLayerOffset lcdLayerStartOffset = {0, 0};
   camErrorType camStatus;
   s32 boardStatus;

   I2CM_Device * i2c1Handle;
   I2CM_Device * i2c2Handle;
   unsigned int camID  = 0;
   int statusI2c=0;
   u8 bytes[2];

   swcLeonSetPIL(0);

   init();

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
    if (rc!=BRDCONFIG_SUCCESS)
    {
      printf("Error: board configuration read failed with %ld status\n",
                rc);
        return rc;
    }
    i2c1Handle=info[1].handler;
    i2c2Handle=info[2].handler;
#elif defined(MV0182)
    boardStatus = BoardInitialise(EXT_PLL_CFG_148_24_24MHZ);
    i2c1Handle=gAppDevHndls.i2c1Handle;
    i2c2Handle=gAppDevHndls.i2c2Handle;
    if (boardStatus != B_SUCCESS)
    {
      printf("Error: board initialization failed with %ld status\n",
          boardStatus);
      return -1;
    }
    DrvI2cMSetErrorHandler(i2c1Handle,I2CErrorHandlerIgnore);
#endif

   printf("Configuring the HDMI chip ADV7513\n");
   status = initADV7513reg(i2c2Handle, ADV7513Config);

   if (status == 0)
       printf("ADV7513 Configured\n");
   else{
       printf("ADV7513 Configuration failed %d \n", status);
       return -1;
   }
   printf("Configuring camera and datapath\n");
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
    camSpec.width=    IMG_W_IMX378;
    camSpec.height=   IMG_H_IMX378;
    camSpec.o_width=  O_IMG_W_IMX378;
    camSpec.o_height= O_IMG_H_IMX378;
    break;

   case IMX214_ID :
    printf("Camera sensor: IMX214 \n");
    sensor=1;
    camConfig = &imx214_2L_2104x1560_RAW10_30Hz_binn_camCfg;
    camSpec.width=    IMG_W_IMX214;
    camSpec.height=   IMG_H_IMX214;
    camSpec.o_width=  O_IMG_W_IMX214;
    camSpec.o_height= O_IMG_H_IMX214;
    break;


   default :
    printf("Camera sensor is not supported for this board!\n");
    return -1;

   }

   swcLeonSetPIL(0);
   prepareDriverData();

   camStatus = CamInit( &camHndl, camConfig, &userConfig, &callbacks, i2c1Handle );
   CamSetupInterrupts(&camHndl, 0, 0, 0, NULL, 1, 0, 0);
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

   OpipeReset ();  //general inits

   //Initialize a MipiRx pipeline that stores full output to DDR (mandatory after camera initialization)
   OpipeCreateRx(&pRx, camSpec.width, camSpec.height - 12,
                 SIPP_MIPI_RX0_ID, BPP(2),
                 (uint32_t)oCircBuffCmx7, O_CBUFF_H);

   appSetParamsRxPipe(&pRx.p);
   pRx.pOut->ddr.base = (uint32_t)camBuf[newCamFrameCtr % MAX_USED_BUF]; //initial output buffer and params
   pRx.p.cbEndOfFrame = RxEofCallback;

   appBuildPipeMF(); //calls OpipeInit(p)
   appSetParamsMFPipe(&opMF.p, sensor);
   OpipeSetSizeMF (&opMF, camSpec.width, camSpec.height);

   printf("Configuring the LCD\n");
   LCDInit(&lcdHndl, &lcdSpec1080p60, NULL, LCD1);
   LCDInitLayer(&lcdHndl, VL1, &lcdFrameSpec, lcdLayerStartOffset);
   LCDSetupCallbacks(&lcdHndl, &allocateLcdFrame, NULL, NULL, NULL);

   // Enable YUV420p to Yuv422i format conversion on LCD interface
   LCDEnYuv422i();

   LCDStart(&lcdHndl);
   printf("\nStreaming ... \n");

   OpipeStart(&pRx.p); //start the OpipeRx (mandatory after camera initialization)

   u32 oldCamFrameCtr = 1;

   while(1){

      if(newCamFrameCtr != oldCamFrameCtr)
        {
          oldCamFrameCtr = newCamFrameCtr;

          //Frame based setup:
          opMF.pIn->ddr.base    = (uint32_t)camBuf[newCamFrameCtr % MAX_USED_BUF];
          opMF.pOutY->ddr.base  = (uint32_t)yuvBufY[newCamFrameCtr % MAX_USED_BUF];
          opMF.pOutUV->ddr.base = (uint32_t)yuvBufUV[newCamFrameCtr % MAX_USED_BUF];

          OpipeStart (&opMF.p); //start the OpipeMF
          OpipeWait  (&opMF.p);
        }
    }

   return 0;
}
