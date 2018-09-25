///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief main leon RT file
///

// 1: Includes
// ----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lrt_app_config.h"
#ifdef MV0212
#include "MV0212.h"
#include "brdGpioCfgs/brdMv0182R5GpioDefaults.h"
#elif defined(MV0182)
#include <Board182Api.h>
#endif
#include "DrvCDCEL.h"
#include "DrvCpr.h"
#include "DrvADV7513.h"
#include "CamGenericApi.h"
#include "LcdApi.h"
#include "DrvMss.h"
#include <DrvLeon.h>
#include <DrvTimer.h>
#include "DrvShaveL2Cache.h"
#include "swcShaveLoader.h"
#include "LcdCEA1080p60.h"
#include "imx208_2L_1936x1096_Raw10Converted8_60Hz.h"
#include "DrvCmxDma.h"
#include "commonDefine.h"
#include "registersMyriad.h"
#include "imageWarp.h"
#include "JpegEncoderApi.h"

#include "mesh.h"

// 2:  Source specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------

#define FRAMES_PER_WARP 45  //the number of frames per warp effect
#define WARP_EFFECTS 4     //the number of warp effects used in the application

#define MAX_USED_BUF             10 //minimum 2 for ping-pong
#define FIRST_INCOMING_BUF_ID    1
#define FIRST_OUTGOING_BUF_ID    0

#define CAM_WINDOW_START_COLUMN  0
#define CAM_WINDOW_START_ROW     8   //cut the embedded info of the first 8 lines

#define WINDOW_WIDTH             1920
#define WINDOW_HEIGHT            1080
#define WINDOW_MARGIN            160

#define CAM_BPP 1   //RAW10 cut down to 8 bits by the SIPP filter:
//a simple solution available for this black&white sensor,
//which avoid the usage of a debayer filter
#define LCD_BPP 1   //YUV422p: the planar YUV422p type has 1 bpp for EVERY PLANE
//(indication to every plane DMAs, not an indicator
//for global storage)

#define CAM_FRAME_SIZE_BYTES     (WINDOW_WIDTH * WINDOW_HEIGHT * CAM_BPP)
#define LCD_CHROMA_SIZE_BYTES    (WINDOW_WIDTH * WINDOW_HEIGHT * LCD_BPP )

#define DDR_AREA                 __attribute__((section(".ddr.bss")))
#define DDR_UNCACHED             __attribute__((section(".ddr_direct.bss")))

#define SHAVE_NUMBER 0
#define WM8325_SLAVE_ADDRESS 0x36

/* Define used to specify how many bytes to read from i2C */
#define TWO_BYTES_SIZE       2
#define BUTTON_READ_DEBOUNCE_TRIGGER 5

#define INPUT_BUFF_SIZE_SHAVE 32*1024

enum warpEffect {
    IDENTITY_WARP,
    FISHEYE_WARP,
    WAVE_WARP,
    PERSPECTIVE_WARP,
};
#ifdef MV0212
#define NUM_I2C_DEVS 3
#endif
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
static I2CM_Device * i2c0Handle;
static I2CM_Device * i2c2Handle;
static I2CM_Device I2cDevice;
static u8 protocolReadSample2[]  = I2C_PROTO_READ_16BA;
volatile u8 prevButton[2];
u32  ButtonReadDebounceCounter;
const u8 controlValue[2] = {0x80, 0xc0};

// 4: Static Local Data
// ----------------------------------------------------------------------------

GenericCamSpec *camConfig = &imx208_2L_1936x1096_RAW10CONV8_60Hz_camCfg;
ADV7513ContfigMode_t ADV7513Config = ADV7513_1080P60;

volatile u32 newCamFrameCtr = FIRST_INCOMING_BUF_ID;
volatile u32 lcdFrameCtr    = FIRST_OUTGOING_BUF_ID;

u8 DDR_AREA  __attribute__((aligned(16)))  camBuf[MAX_USED_BUF][CAM_FRAME_SIZE_BYTES];
u8 DDR_AREA  __attribute__((aligned(16)))  lcdBuf[MAX_USED_BUF][CAM_FRAME_SIZE_BYTES];
u8 DDR_AREA  toSdcardBuf[CAM_FRAME_SIZE_BYTES];
u8 DDR_AREA  jpgbuff[CAM_FRAME_SIZE_BYTES];

volatile jpegFrameBuffer DDR_UNCACHED  *pToSdcardBuf;
u8 DDR_AREA  lcdDummyChroma[LCD_CHROMA_SIZE_BYTES];

GenericCameraHandle  camHndl;
CamUserSpec          userConfig;
frameSpec            camFrameSpec;
frameBuffer          camFrame[MAX_USED_BUF];
frameBuffer          sdCardFrame;
volatile jpegFrameBuffer DDR_UNCACHED  jpeg;
frameSpec            lcdFrameSpec;
frameBuffer          lcdFrame[MAX_USED_BUF];
LCDHandle            lcdHndl;

callbacksListStruct          callbacks = {0};
interruptsCallbacksListType  isrCallbacks = {0};

volatile u32 processingFrameCtr;
tileList tileNodes[MESH_HEIGHT * MESH_WIDTH];

static tyI2cConfig  i2c2MasterCfg =
{
    .device = IIC3_BASE_ADR,
    .sclPin = 79,
    .sdaPin = 80,
    .speedKhz =  100,
    .addressSize = ADDR_7BIT,
    .errorHandler = NULL
};
static u8 protocolWriteSample2[] = I2C_PROTO_WRITE_16BA;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

void runWarp(swcShaveUnit_t svu, meshStruct* mesh,
             frameBuffer *inputFb,
             frameBuffer *outputFb,
             unsigned short paddingvalue);
void prepareDriverData(void);
frameBuffer* AllocateNextCamFrameBuf(void);
frameBuffer* allocateLcdFrame(int layer);

// 6: Functions Implementation
// ----------------------------------------------------------------------------

void runWarp(swcShaveUnit_t svu, meshStruct* mesh,
             frameBuffer *inputFb,
             frameBuffer *outputFb,
             unsigned short paddingvalue)
{
    IMGWARP_start(svu, mesh, tileNodes, inputFb, outputFb, paddingvalue);
    DrvShaveL2CachePartitionFlushAndInvalidate(PARTITION_0);
}

void prepareDriverData(void)
{
    s32 ibuf;

    //user extra settings
    //the SIPP camera connected to 1st position of CAMB connector
    userConfig.mipiControllerNb  = CAM_B1_MIPICTRL;
    userConfig.receiverId        = SIPP_DEVICE1;
#ifdef MV0212
    userConfig.sensorResetPin    = MV0212_MV0200_SENSOR_RIGHT_RST_GPIO;
#elif defined(MV0182)
    userConfig.sensorResetPin    = MV0182_MV0200_SENSOR_RIGHT_RST_GPIO;   //
#endif
    userConfig.stereoPairIndex   = CAM_B1_RIGHT_ADDR;
    userConfig.windowColumnStart = CAM_WINDOW_START_COLUMN;
    userConfig.windowRowStart    = CAM_WINDOW_START_ROW;
    userConfig.windowWidth       = WINDOW_WIDTH;
    userConfig.windowHeight      = WINDOW_HEIGHT;
    userConfig.generateSync      = NULL;

    //synchronize local buffers spec with driver configuration
    camFrameSpec.width   = WINDOW_WIDTH;
    camFrameSpec.height  = WINDOW_HEIGHT;
    camFrameSpec.bytesPP = CAM_BPP;
    camFrameSpec.stride  = WINDOW_WIDTH * CAM_BPP;
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

    //initialize the LCD data
    lcdFrameSpec.width   = WINDOW_WIDTH;
    lcdFrameSpec.height  = WINDOW_HEIGHT;
    lcdFrameSpec.stride  = WINDOW_WIDTH * LCD_BPP;
    lcdFrameSpec.bytesPP = LCD_BPP;
    lcdFrameSpec.type    = YUV422p;

    for (ibuf = 0; ibuf < MAX_USED_BUF;ibuf++)
    {
        lcdFrame[ibuf].spec = lcdFrameSpec;
        lcdFrame[ibuf].p1   = &lcdBuf[ibuf][0];
        lcdFrame[ibuf].p2   = lcdDummyChroma;    //use dummy UV planes
        lcdFrame[ibuf].p3   = lcdDummyChroma;    //
    }

    return;
}

frameBuffer* AllocateNextCamFrameBuf(void)
{
    newCamFrameCtr++;
    return ( &camFrame[newCamFrameCtr % MAX_USED_BUF] );
}

frameBuffer* allocateLcdFrame(int layer)
{
    UNUSED(layer);

    return ( &lcdFrame[lcdFrameCtr  % MAX_USED_BUF] );
}

static inline int readButton(u8 *currButton)
{
    I2CM_StatusType sc;
    u8 lButtonStatus[TWO_BYTES_SIZE];
    int retVal = 0;

    sc = DrvI2cMTransaction(&I2cDevice, WM8325_SLAVE_ADDRESS,
                            0x400c,
                            protocolReadSample2,
                            (u8 *) &lButtonStatus[0],
                            TWO_BYTES_SIZE);
    if(sc == I2CM_STAT_OK)
    {
        if ( (*currButton != lButtonStatus[1])
                        && (ButtonReadDebounceCounter == 0) )
        {
            ButtonReadDebounceCounter = BUTTON_READ_DEBOUNCE_TRIGGER;
            *currButton = lButtonStatus[1];
            retVal = 1;
        }

        if (ButtonReadDebounceCounter > 0)
        {
            ButtonReadDebounceCounter--;
        }
    }
    else
    {
        retVal = -1;
    }

    return retVal;
}


int main()
{
    s32 status;
    u32 line;
    u32 warpEffectStyle = IDENTITY_WARP;
    camErrorType camStatus;
    LCDLayerOffset lcdLayerStartOffset = {0, 0};
    s32 boardStatus;
    meshStruct mesh;
    frameBuffer* currFrame;
    s32 buffcount;
    int readButtonStatus;
    ButtonReadDebounceCounter = 0;
    u8  button = 0;
    u8  previousButton = 0;
    pToSdcardBuf=0;

    jpeg.size=0;
    jpeg.buf=jpgbuff;

    DrvCmxDmaInitDefault();
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
        printf("Error: board initialization failed with %ld status\n", rc);
        return rc;
    }

    rc = BoardGetPCBRevision(&rev);
    if (rc!=BRDCONFIG_SUCCESS)
    {
        printf("Error: board configuration read failed with %ld status\n", rc);
        return rc;
    }
    printf("Board Mv0212 initialized, revision = %lu \n", rev);

    boardStatus = BoardInitExtPll(EXT_PLL_CFG_148_24_24MHZ);
    if (boardStatus != BRDCONFIG_SUCCESS)
    {
        printf("Error: board initialization failed with %ld status\n", boardStatus);
        return -1;
    }

    rc = BoardGetI2CInfo(info, NUM_I2C_DEVS);
    if (rc!=BRDCONFIG_SUCCESS)
    {
        printf("Error: board configuration read failed with %ld status\n", rc);
        return rc;
    }
    i2c0Handle=info[0].handler;
    i2c2Handle=info[2].handler;
#elif defined(MV0182)
    boardStatus = BoardInitialise(EXT_PLL_CFG_148_24_24MHZ);
    i2c0Handle=gAppDevHndls.i2c0Handle;
    i2c2Handle=gAppDevHndls.i2c2Handle;
    if (boardStatus != B_SUCCESS)
    {
        printf("Error: board initialization failed with %ld status\n", boardStatus);
        return -1;
    }
#endif

    swcLeonSetPIL(0);
    printf("Configuring camera and datapath\n");
    prepareDriverData();

    //prefill the common UV buffer of the output YUV422p with constant neutral
    //pattern(luma Y plane will be overwritten by the SIPP DMA);
    //this is acceptable for the imx208 sensor, which output grayscale image,
    //so only luma is valuable, the chroma can be neutral.
    memset(lcdDummyChroma, 0x80, sizeof(lcdDummyChroma));

    //connect the LCD to output parallel bus (by GPIO) instead of MIPI Tx
    DrvMssConnectLcdToGpio();

    camStatus = CamInit( &camHndl, camConfig, &userConfig, &callbacks, i2c0Handle );
    if (camStatus != CAM_SUCCESS)
    {
        printf("\n Camera configuration failed (%d).\n", camStatus);
        return -1;
    }

    camStatus = CamStart(&camHndl);
    if (camStatus != CAM_SUCCESS)
    {
        printf("\n Camera failed to start (%d).\n", camStatus);
        return -1;
    }

    printf("Configuring the HDMI chip ADV7513\n");
    status = initADV7513reg(i2c2Handle, ADV7513Config);

    if (status != 0)
    {
        printf("\n ADV7513 chip configuration failed with code %ld.\n", status);
        return -1;
    }

    printf("Configuring the LCD\n");
    LCDInit(&lcdHndl, &lcdSpec1080p60, NULL, LCD1);
    LCDInitLayer(&lcdHndl, VL1, &lcdFrameSpec, lcdLayerStartOffset);
    LCDSetupCallbacks(&lcdHndl, &allocateLcdFrame, NULL, NULL, NULL);

    /*Clean LCD buffer before LCD is started*/
    for(buffcount=0; buffcount<MAX_USED_BUF;buffcount++)
    {
        memset(lcdFrame[buffcount].p1, 0x0, WINDOW_WIDTH * WINDOW_HEIGHT);
    }

    LCDStart(&lcdHndl);

    //set MESH_WIDTH and MESH_HEIGHT;
    mesh.meshWidth = MESH_WIDTH;
    mesh.meshHeight = MESH_HEIGHT;

    // Initialize the I2C Driver
    printf("Configuring the I2C Driver\n");
    status = DrvI2cMInitFromConfig(&I2cDevice, &i2c2MasterCfg);
    if(status)
        exit(status);
    status = DrvI2cMTransaction(&I2cDevice, WM8325_SLAVE_ADDRESS,
                                0x4038,
                                protocolWriteSample2,
                                (u8 *) &controlValue[0],
                                TWO_BYTES_SIZE);
    if(status)
        exit(status);
    status = DrvI2cMTransaction(&I2cDevice, WM8325_SLAVE_ADDRESS,
                                0x4039,
                                protocolWriteSample2,
                                (u8 *) &controlValue[0],
                                TWO_BYTES_SIZE);
    if(status)
        exit(status);

    DrvLeonRTSignalBootCompleted();

    printf("\n  Streaming ... \n");
    while (1)
    {
        //'newCamFrameCtr' indicates the position where to store a new frame from
        //the camera, 'processingFrameCtr' points to the last updated frame
        processingFrameCtr = newCamFrameCtr - 1;

        DrvShaveL2CachePartitionFlushAndInvalidate(PARTITION_0);

        currFrame = &camFrame[processingFrameCtr % MAX_USED_BUF];

        //create frame margin of WINDOW_MARGIN, used to show better the warp effects
        memset(currFrame->p1, 0x0, WINDOW_WIDTH * WINDOW_MARGIN);
        memset(currFrame->p1 + (WINDOW_HEIGHT - WINDOW_MARGIN) * WINDOW_WIDTH,
               0x0, WINDOW_WIDTH * WINDOW_MARGIN);
        for(line = 0; line < WINDOW_HEIGHT; line++)
        {
            memset(currFrame->p1 + line * WINDOW_WIDTH, 0x0, WINDOW_MARGIN);
            memset(currFrame->p1 + line * WINDOW_WIDTH + (WINDOW_WIDTH - WINDOW_MARGIN),
                   0x0, WINDOW_MARGIN);
        }

        if( lcdFrameCtr % FRAMES_PER_WARP == 0)
            warpEffectStyle++;

        // set up the meshes
        switch(warpEffectStyle % WARP_EFFECTS)
        {
        case IDENTITY_WARP:
            mesh.meshX = (float*)identityMesh_x;
            mesh.meshY = (float*)identityMesh_y;
            break;
        case FISHEYE_WARP:
            mesh.meshX = (float*)fisheyeMesh_x;
            mesh.meshY = (float*)fisheyeMesh_y;
            break;
        case WAVE_WARP:
            mesh.meshX = (float*)waveMesh_x;
            mesh.meshY = (float*)waveMesh_y;
            break;
        case PERSPECTIVE_WARP:
            mesh.meshX = (float*)perspectiveMesh_x;
            mesh.meshY = (float*)perspectiveMesh_y;
            break;
        default:
            break;
        }

        // run imageWarp algorithm
        runWarp(SHAVE_NUMBER, &mesh, currFrame, &lcdFrame[(lcdFrameCtr + 1) % MAX_USED_BUF], 0);

        lcdFrameCtr++;
        previousButton = button;
        readButtonStatus = readButton(&button);
        if (readButtonStatus == 1)
        {
            if ((previousButton == 0) && (button == 1))
            {
                if(pToSdcardBuf==0)
                {
                    memcpy(toSdcardBuf,lcdFrame[(lcdFrameCtr + 1) % MAX_USED_BUF].p1,CAM_FRAME_SIZE_BYTES);
                    sdCardFrame.p1=toSdcardBuf;
                    sdCardFrame.p2=lcdDummyChroma;
                    sdCardFrame.p3=lcdDummyChroma;
                    sdCardFrame.spec=lcdFrameSpec;
                    IMGWARP_cleanup();
                    printf("Start encoding JPEG\n");
                    jpeg.size = JPEG_encode(sdCardFrame, jpeg.buf, 12, INPUT_BUFF_SIZE_SHAVE, JPEG_422_PLANAR);
                    printf("Ended encoding JPEG\n");
                    pToSdcardBuf=&jpeg;
                    while(pToSdcardBuf!=0)
                    {
                        DrvTimerSleepMs(500);
                        if(pToSdcardBuf==0)
                        {
                            printf("Waiting for LRT\n");
                        }
                    }
                }
            }
            if ((previousButton == 0) && (button == 2))
            {
                printf("Button B2 pressed, this button has no functionality defined\n");
            }
        }
        else
        {
            if (readButtonStatus < 0)
            {
                printf("Error, the button can not be read, pls check ret value of function: DrvI2cMTransaction\n");
            }
        }

    } //while
    return 0;
}
