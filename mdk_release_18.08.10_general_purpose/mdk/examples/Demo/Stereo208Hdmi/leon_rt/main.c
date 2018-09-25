///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#ifdef MV0212
#include "MV0212.h"
#include "brdGpioCfgs/brdMv0182R5GpioDefaults.h"
#elif defined(MV0182)
#include <Board182Api.h>
#endif
#include "DrvCDCEL.h"
#include "DrvCpr.h"
#include "DrvGpioDefines.h"
#include "registersMyriad.h"
#include "DrvADV7513.h"
#include "CamGenericApi.h"
#include "LcdApi.h"
#include "DrvMss.h"
#include "sipp.h"
#include "sippTestCommon.h"
#include "LcdCEA1080p60.h"
#include "imx208_2L_1936x1096_Raw10_30Hz.h"
#include "lrt_app_config.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define MAX_USED_BUF             3
#define FIRST_INCOMING_BUF_ID    2
#define FIRST_OUTGOING_BUF_ID    0

#define CAM_WINDOW_START_COLUMN  0
#define CAM_WINDOW_START_ROW     8   //cut the embedded info frame

#define CAM_LEFT_WINDOW_WIDTH    960 //the current width must be x16
#define CAM_RIGHT_WINDOW_WIDTH   960 //the current width must be x16
#define CAM_WINDOW_HEIGHT        1080

// ----------------------------------------------------------------------------
#define LCD_LEFT_WINDOW_WIDTH    CAM_LEFT_WINDOW_WIDTH
#define LCD_RIGHT_WINDOW_WIDTH   CAM_RIGHT_WINDOW_WIDTH
#define LCD_WINDOW_HEIGHT        CAM_WINDOW_HEIGHT

#define LCD_WINDOW_LEFT_STARTX   0
#define LCD_WINDOW_LEFT_STARTY   0
#define LCD_WINDOW_RIGHT_STARTX  LCD_LEFT_WINDOW_WIDTH
#define LCD_WINDOW_RIGHT_STARTY  0

#define CAM_BPP 2    //2 bytes per pixel (only 10bits used) for the only one plane of RAW10
#define LCD_BPP 2    //2bpp for the only one plane of YUV422i

#define CAM_LEFT_FRAME_SIZE_BYTES    (CAM_LEFT_WINDOW_WIDTH * CAM_WINDOW_HEIGHT * CAM_BPP)
#define CAM_RIGHT_FRAME_SIZE_BYTES   (CAM_RIGHT_WINDOW_WIDTH * CAM_WINDOW_HEIGHT * CAM_BPP)
#define LCD_LEFT_FRAME_SIZE_BYTES    (LCD_LEFT_WINDOW_WIDTH * LCD_WINDOW_HEIGHT * LCD_BPP)
#define LCD_RIGHT_FRAME_SIZE_BYTES   (LCD_RIGHT_WINDOW_WIDTH * LCD_WINDOW_HEIGHT * LCD_BPP)

#define DDR_AREA      __attribute__((section(".ddr.bss")))

#ifdef MV0212
#define NUM_I2C_DEVS 3
#endif
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
static I2CM_Device * i2c0Handle;
static I2CM_Device * i2c2Handle;

GenericCamSpec *staticCamConfig = &imx208_2L_1936x1096_RAW10_30Hz_camCfg;

volatile ALIGNED(4) u32 newCamFrameCtrLeft  = FIRST_INCOMING_BUF_ID;
volatile ALIGNED(4) u32 newCamFrameCtrRight = FIRST_INCOMING_BUF_ID;
volatile ALIGNED(4) u32 lcdFrameCtr         = FIRST_OUTGOING_BUF_ID;

u8 camLeftBuf[MAX_USED_BUF][CAM_LEFT_FRAME_SIZE_BYTES] DDR_AREA ALIGNED(8);
u8 camRightBuf[MAX_USED_BUF][CAM_RIGHT_FRAME_SIZE_BYTES] DDR_AREA ALIGNED(8);
u8 lcdLeftBuf[MAX_USED_BUF][LCD_LEFT_FRAME_SIZE_BYTES] DDR_AREA ALIGNED(8);
u8 lcdRightBuf[MAX_USED_BUF][LCD_RIGHT_FRAME_SIZE_BYTES] DDR_AREA ALIGNED(8);
LCDLayerOffset lcdLayerOffsetLeft = {LCD_WINDOW_LEFT_STARTX, LCD_WINDOW_LEFT_STARTY}, lcdLayerOffsetRight = {LCD_WINDOW_RIGHT_STARTX,LCD_WINDOW_RIGHT_STARTY};
volatile u32 ALIGNED(4) processingFrameCtr;
GenericCameraHandle  camHndlLeft, camHndlRight;
CamUserSpec          userCamLeftConfig, userCamRightConfig;
frameSpec            camLeftFrameSpec, camRightFrameSpec;
frameSpec            lcdLeftFrameSpec, lcdRightFrameSpec;
frameBuffer          camLeftFrame[MAX_USED_BUF];
frameBuffer          camRightFrame[MAX_USED_BUF];
frameBuffer          lcdFrameLeft[MAX_USED_BUF];
frameBuffer          lcdFrameRight[MAX_USED_BUF];
LCDHandle            lcdHndl;

callbacksListStruct          callbacks = {0};
interruptsCallbacksListType  isrCallbacks = {0};

extern u16 lutTable[];

SippPipeline *plLeft, *plRight;
SippFilter   *dmaInLeft, *dmaInRight;
SippFilter   *lutLeft, *lutRight;
SippFilter   *dmaOutLeft, *dmaOutRight;
DmaParam     *dmaInCfgLeft, *dmaInCfgRight;
DmaParam     *dmaOutCfgLeft, *dmaOutCfgRight;
LutParam     *lutCfgLeft, *lutCfgRight;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
frameBuffer* AllocateNextLeftCamFrameBuf(void);
frameBuffer* AllocateNextRightCamFrameBuf(void);

ADV7513ContfigMode_t ADV7513Config = ADV7513_1080P60;


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
    userCamLeftConfig.mipiControllerNb  = CAM_B1_MIPICTRL;                     //left camera connected to CAMB connecter of the board
    userCamLeftConfig.receiverId        = SIPP_DEVICE1;                        //
#ifdef MV0212
    userCamLeftConfig.sensorResetPin    = MV0212_MV0200_SENSOR_RIGHT_RST_GPIO;
#elif defined(MV0182)
    userCamLeftConfig.sensorResetPin    = MV0182_MV0200_SENSOR_RIGHT_RST_GPIO;   //
#endif
    userCamLeftConfig.stereoPairIndex   = CAM_B1_RIGHT_ADDR;                    //

    userCamLeftConfig.windowColumnStart = CAM_WINDOW_START_COLUMN;
    userCamLeftConfig.windowRowStart    = CAM_WINDOW_START_ROW;
    userCamLeftConfig.windowWidth       = CAM_LEFT_WINDOW_WIDTH;
    userCamLeftConfig.windowHeight      = CAM_WINDOW_HEIGHT;
    userCamLeftConfig.generateSync      = NULL;


    userCamRightConfig.mipiControllerNb  = CAM_B2_MIPICTRL;         //right camera connected to CAMB connecter of the board
    userCamRightConfig.receiverId        = CIF_DEVICE1;             //
#ifdef MV0212
    userCamRightConfig.sensorResetPin    = MV0212_MV0200_SENSOR_LEFT_RST_GPIO;
#elif defined(MV0182)
    userCamRightConfig.sensorResetPin    = MV0182_MV0200_SENSOR_LEFT_RST_GPIO;   //
#endif
    userCamRightConfig.stereoPairIndex   = CAM_B2_LEFT_ADDR;        //
    userCamRightConfig.windowColumnStart = CAM_WINDOW_START_COLUMN;
    userCamRightConfig.windowRowStart    = CAM_WINDOW_START_ROW;
    userCamRightConfig.windowWidth       = CAM_RIGHT_WINDOW_WIDTH;
    userCamRightConfig.windowHeight      = CAM_WINDOW_HEIGHT;
    userCamRightConfig.generateSync      = NULL;

    //synchronize local buffers spec with driver configuration
    camLeftFrameSpec.width   = CAM_LEFT_WINDOW_WIDTH;
    camLeftFrameSpec.height  = CAM_WINDOW_HEIGHT;
    camLeftFrameSpec.bytesPP = CAM_BPP;
    camLeftFrameSpec.stride  = CAM_LEFT_WINDOW_WIDTH * CAM_BPP;
    camLeftFrameSpec.type    = RAW16;

    camRightFrameSpec.width   = CAM_RIGHT_WINDOW_WIDTH;
    camRightFrameSpec.height  = CAM_WINDOW_HEIGHT;
    camRightFrameSpec.bytesPP = CAM_BPP;
    camRightFrameSpec.stride  = CAM_RIGHT_WINDOW_WIDTH * CAM_BPP;
    camRightFrameSpec.type    = RAW16;

    for (ibuf = 0; ibuf < MAX_USED_BUF;ibuf++)
    {
        camLeftFrame[ibuf].spec  = camLeftFrameSpec;
        camLeftFrame[ibuf].p1    = (unsigned char*)camLeftBuf[ibuf];
        camLeftFrame[ibuf].p2    = NULL;
        camLeftFrame[ibuf].p3    = NULL;

        camRightFrame[ibuf].spec = camRightFrameSpec;
        camRightFrame[ibuf].p1   = (unsigned char*)camRightBuf[ibuf];
        camRightFrame[ibuf].p2   = NULL;
        camRightFrame[ibuf].p3   = NULL;
    }

    lcdLeftFrameSpec.width   = LCD_LEFT_WINDOW_WIDTH;
    lcdLeftFrameSpec.height  = LCD_WINDOW_HEIGHT;
    lcdLeftFrameSpec.stride  = LCD_LEFT_WINDOW_WIDTH * LCD_BPP;
    lcdLeftFrameSpec.bytesPP = LCD_BPP;
    lcdLeftFrameSpec.type    = YUV422i;

    lcdRightFrameSpec.width   = LCD_RIGHT_WINDOW_WIDTH;
    lcdRightFrameSpec.height  = LCD_WINDOW_HEIGHT;
    lcdRightFrameSpec.stride  = LCD_RIGHT_WINDOW_WIDTH * LCD_BPP;
    lcdRightFrameSpec.bytesPP = LCD_BPP;
    lcdRightFrameSpec.type    = YUV422i;

    for (ibuf = 0; ibuf < MAX_USED_BUF;ibuf++)
    {
        lcdFrameLeft[ibuf].spec = lcdLeftFrameSpec;  //the resulted YUV out of the LUT filter is packed (linear)
        lcdFrameLeft[ibuf].p1   = &lcdLeftBuf[ibuf][0];
        lcdFrameLeft[ibuf].p2   = NULL;
        lcdFrameLeft[ibuf].p3   = NULL;

        lcdFrameRight[ibuf].spec = lcdRightFrameSpec;  //the resulted YUV out of the LUT filter is packed (linear)
        lcdFrameRight[ibuf].p1   = &lcdRightBuf[ibuf][0];
        lcdFrameRight[ibuf].p2   = NULL;
        lcdFrameRight[ibuf].p3   = NULL;
    }

    return;
}


// Build two pipelines, one for every camera
void appBuildPipelines()
{
    plLeft      = sippCreatePipeline(0, 1, SIPP_MBIN(mbinImgSipp));
    plRight     = sippCreatePipeline(2, 3, SIPP_MBIN(mbinImgSipp));

    //B&W implementation : use look-up tables for conversion from RAW10 -> RAW8 (both stored on 16bits)
    // + add a neutral 0x80 pattern in the unused byte, in order to obtain YUV422i (storage 16bits)

    //left pipe
    dmaInLeft       = sippCreateFilter(plLeft, 0x00,     CAM_LEFT_WINDOW_WIDTH, CAM_WINDOW_HEIGHT, N_PL(1), SZ(UInt16),  SIPP_AUTO, (FnSvuRun)SIPP_DMA_ID, "DMA_IN");
    lutLeft         = sippCreateFilter(plLeft, 0x00,     CAM_LEFT_WINDOW_WIDTH, CAM_WINDOW_HEIGHT, N_PL(1), SZ(UInt16),  SIPP_AUTO, (FnSvuRun)SIPP_LUT_ID, "SIPP_LUT");
    dmaOutLeft      = sippCreateFilter(plLeft, 0x00,     CAM_LEFT_WINDOW_WIDTH, CAM_WINDOW_HEIGHT, N_PL(1), SZ(UInt16),  SIPP_AUTO, (FnSvuRun)SIPP_DMA_ID, "DMA_OUT");

    sippLinkFilter(lutLeft   , dmaInLeft, 1, 1);
    sippLinkFilter(dmaOutLeft, lutLeft  , 1, 1);

    //right pipe
    dmaInRight       = sippCreateFilter(plRight, 0x00,     CAM_RIGHT_WINDOW_WIDTH, CAM_WINDOW_HEIGHT, N_PL(1), SZ(UInt16), SIPP_AUTO,  (FnSvuRun)SIPP_DMA_ID, "DMA_IN");
    lutRight         = sippCreateFilter(plRight, 0x00,     CAM_RIGHT_WINDOW_WIDTH, CAM_WINDOW_HEIGHT, N_PL(1), SZ(UInt16), SIPP_AUTO,  (FnSvuRun)SIPP_LUT_ID, "SIPP_LUT");
    dmaOutRight      = sippCreateFilter(plRight, 0x00,     LCD_RIGHT_WINDOW_WIDTH, LCD_WINDOW_HEIGHT, N_PL(1), SZ(UInt16), SIPP_AUTO,  (FnSvuRun)SIPP_DMA_ID, "DMA_OUT");

    sippLinkFilter(lutRight   , dmaInRight, 1, 1);
    sippLinkFilter(dmaOutRight, lutRight  , 1, 1);
}


void appConfigureFilters(void)
{
    dmaInCfgLeft  = (DmaParam*)dmaInLeft->params;
    dmaOutCfgLeft = (DmaParam*)dmaOutLeft->params;
    lutCfgLeft    = (LutParam*)lutLeft->params;

    dmaInCfgRight  = (DmaParam*)dmaInRight->params;
    dmaOutCfgRight = (DmaParam*)dmaOutRight->params;
    lutCfgRight    = (LutParam*)lutRight->params;

    dmaInCfgLeft->ddrAddr    = (UInt32)&camLeftBuf[0];
    dmaOutCfgLeft->ddrAddr   = (UInt32)&lcdLeftBuf[0];

    dmaInCfgRight->ddrAddr    = (UInt32)&camRightBuf[0];
    dmaOutCfgRight->ddrAddr   = (UInt32)&lcdRightBuf[0];

    //LUT config left
    lutCfgLeft->cfg     =  (0      )  | //LUT type: 0 for integers
                    (0  << 1)  | //channel mode : OFF, as we use a single plane here
                    (10 << 3)  | //integer input width: 10 bits
                    (0  << 8);   //numer of LUT tables: a single table (with 16 ranges/regions)
    lutCfgLeft->sizeA   =  0x44444444; // LUT size: 2^4= 16 entries/range, 16 entries x 16 ranges = 256 total entries in LUT
    lutCfgLeft->sizeB   =  0x44444444; //
    lutCfgLeft->lut     =  lutTable;
    lutCfgRight->frmDim =  ((CAM_WINDOW_HEIGHT << 16) | CAM_LEFT_WINDOW_WIDTH);

    //LUT config right
    lutCfgRight->cfg    =  (0      )  | //LUT type: 0 for integers
                    (0  << 1)  | //channel mode : OFF, as we use a single plane here
                    (10 << 3)  | //integer input width: 10 bits
                    (0  << 8);   //numer of LUT tables: a single table (with 16 ranges/regions)
    lutCfgRight->sizeA  =  0x44444444;  // LUT size: 2^4= 16 entries/range, 16 entries x 16 ranges = 256 total entries in LUT
    lutCfgRight->sizeB  =  0x44444444;  //
    lutCfgRight->lut    =  lutTable;
    lutCfgRight->frmDim =  ((CAM_WINDOW_HEIGHT << 16) | CAM_LEFT_WINDOW_WIDTH);

    configureLutFilterSiliconSpecific(lutCfgLeft);
    configureLutFilterSiliconSpecific(lutCfgRight);
}

frameBuffer* AllocateNextLeftCamFrameBuf(void)
{
    ++newCamFrameCtrLeft;
    return ( &camLeftFrame[newCamFrameCtrLeft % MAX_USED_BUF] );
}

frameBuffer* AllocateNextRightCamFrameBuf(void)
{
    ++newCamFrameCtrRight;
    return ( &camRightFrame[newCamFrameCtrRight % MAX_USED_BUF] );
}


frameBuffer* allocateLcdFrame(int layer)
{
    frameBuffer *lcdFrame = NULL;
    if (layer == VL1)
        lcdFrame = &lcdFrameLeft[lcdFrameCtr  % MAX_USED_BUF];
    if (layer == VL2)
        lcdFrame = &lcdFrameRight[lcdFrameCtr  % MAX_USED_BUF];

    return ( lcdFrame );
}


int main()
{
    s32  status;
    camErrorType camStatus;
    s32 boardStatus;

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

    generateLutTable();

    prepareDriverData();

    printf("Configuring the SIPP pipeline\n");
    sippInitialize();
    appBuildPipelines();
    appConfigureFilters();
    sippFinalizePipeline(plLeft);
    sippFinalizePipeline(plRight);

    //connect the LCD to output parallel bus (by GPIO) instead of MIPI Tx
    DrvMssConnectLcdToGpio();

    printf("Configuring imx208 dual cameras and datapath\n");
    isrCallbacks.getBlock     = NULL;
    isrCallbacks.notification = NULL;
    callbacks.isrCbfList      = &isrCallbacks;
    callbacks.sensorCbfList   = NULL;

    //start left camera
    isrCallbacks.getFrame     = AllocateNextLeftCamFrameBuf;

    camStatus  = CamInit( &camHndlLeft, staticCamConfig, &userCamLeftConfig, &callbacks, i2c0Handle);     // this is a SIPP camera
    if (camStatus != CAM_SUCCESS)
    {
        printf("\n Left camera configuration failed (%d).\n", camStatus);
        return -1;
    }
    camStatus  = CamStart( &camHndlLeft );
    if (camStatus != CAM_SUCCESS)
    {
        printf("\n Left camera failed to start (%d).\n", camStatus);
        return -1;
    }

    //start right camera
    isrCallbacks.getFrame     = AllocateNextRightCamFrameBuf;

    camStatus  = CamInit( &camHndlRight, staticCamConfig, &userCamRightConfig, &callbacks, i2c0Handle);     // this is a CIF camera
    if (camStatus != CAM_SUCCESS)
    {
        printf("\n Right camera configuration failed (%d).\n", camStatus);
        return -1;
    }

    camStatus  = CamStart( &camHndlRight );
    if (camStatus != CAM_SUCCESS)
    {
        printf("\n Right camera failed to start (%d).\n", camStatus);
        return -1;
    }


    printf("Configuring the HDMI chip ADV7513\n");
    status = initADV7513reg(i2c2Handle, ADV7513Config);

    if (status != 0)
    {
        printf("ADV7513 Configuration failed (%ld) \n", status);
        return -1;
    }

    printf("Configuring the LCD\n");
    LCDInit(&lcdHndl, &lcdSpec1080p60, NULL, LCD1);
    LCDInitLayer(&lcdHndl, VL1, &lcdLeftFrameSpec, lcdLayerOffsetLeft);
    LCDInitLayer(&lcdHndl, VL2, &lcdRightFrameSpec, lcdLayerOffsetRight);

    LCDSetupCallbacks(&lcdHndl, &allocateLcdFrame, NULL, NULL, NULL);

    printf("\n Starting dual streaming on LCD...\n");
    LCDStart(&lcdHndl);

    //the processing is based on the left cam timings !
    processingFrameCtr = newCamFrameCtrLeft;


    while(1)
    {
        if( newCamFrameCtrLeft != processingFrameCtr )
        {
            processingFrameCtr = newCamFrameCtrLeft;

            //do not process from/to same buffers as the ones used by CAM/LCD, but from/to shifted ones
            dmaInCfgLeft->ddrAddr    = (UInt32)&camLeftBuf[(newCamFrameCtrLeft -2) % MAX_USED_BUF];
            dmaInCfgRight->ddrAddr   = (UInt32)&camRightBuf[(newCamFrameCtrRight -2) % MAX_USED_BUF]; //cam right frame might not be updated here, in this case process the old frame
            dmaOutCfgLeft->ddrAddr   = (UInt32)&lcdLeftBuf[(lcdFrameCtr + 1) % MAX_USED_BUF];
            dmaOutCfgRight->ddrAddr  = (UInt32)&lcdRightBuf[(lcdFrameCtr + 1) % MAX_USED_BUF];

            sippProcessFrame(plLeft);
            sippProcessFrame(plRight);

            lcdFrameCtr++;
        }
    }

    return 0;
}
