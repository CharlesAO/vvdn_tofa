// 1: Includes
// ----------------------------------------------------------------------------
#include "los_app_config.h"
#include "lrt_app_config.h"
#include "setup.h"
#include <CameraConfig.h>
#include <Sections.h>
#include <Buffers.h>

#include <stdio.h>
#include <string.h>

#include <swcShaveLoader.h>
#include <swcTestUtils.h>
#include <DrvShaveL2Cache.h>
#include <swcCrc.h>
#include <UnitTestApi.h>
#include <bicubicWarpApi.h>
#include <VcsHooksApi.h>
#ifdef MV0212
#include "MV0212.h"
#include "brdGpioCfgs/brdMv0182R5GpioDefaults.h"
#elif defined(MV0182)
#include <Board182Api.h>
#endif
#include <DrvCDCEL.h>
#include <DrvCpr.h>
#include <DrvLeon.h>
#include <DrvGpioDefines.h>
#include <registersMyriad.h>
#include <DrvADV7513.h>
#include <CamGenericApi.h>
#include <DrvMss.h>
#include <sipp.h>
#include <sippTestCommon.h>
#include <LeonIPCApi.h>
#include <imx208_2L_1936x1096_Raw10_30Hz.h>

// 2:  Source Specific defines and types
// ----------------------------------------------------------------------------
#define PROF_PRINT          printf
#define MSG_QUEUE_SIZE      1000
#define MSG_SIZE            1

#define INVERSION_SHAVE     0
#define CONVERSION_SHAVE    0
#define PROFILING_SHAVE     0
#define SIPP_SHAVE          4

#ifdef MV0212
#define NUM_I2C_DEVS 3
#endif
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
static I2CM_Device * i2cHandle;
volatile u32 var_debug = 0x55555555;
// Camera buffers
u8 camLeftBuf[MAX_USED_BUF][CAM_FRAME_SIZE_BYTES]            DDR_BSS ALIGNED(8);
u8 camRightBuf[MAX_USED_BUF][CAM_FRAME_SIZE_BYTES]           DDR_BSS ALIGNED(8);
u8 lutLeftBuf[MAX_USED_BUF][ALGO_FRAME_SIZE_BYTES]           DDR_BSS ALIGNED(8);
u8 lutRightBuf[MAX_USED_BUF][ALGO_FRAME_SIZE_BYTES]          DDR_BSS ALIGNED(8);
u8 lutRightBufRectified[MAX_USED_BUF][ALGO_FRAME_SIZE_BYTES] DDR_BSS ALIGNED(8);

// Algorithm buffers
u8 dispMap1[PATCH_WIDTH * PATCH_HEIGHT] DDR_BSS ALIGNED(8);
u8 dispMap2[PATCH_WIDTH * PATCH_HEIGHT] DDR_BSS ALIGNED(8);
u8 dispMap3[PATCH_WIDTH * PATCH_HEIGHT] DDR_BSS ALIGNED(8);
u8 dispMap4[PATCH_WIDTH * PATCH_HEIGHT] DDR_BSS ALIGNED(8);

u8* disparityMap[MAX_PATCHES] = {dispMap1, dispMap2, dispMap3, dispMap4};

fp32* xyRectifiedBuffer[ALGO_FRAME_SIZE_BYTES * 2]                   DDR_BSS ALIGNED(8);
u8 frameBufferDisparities[ALGO_FRAME_SIZE_BYTES]                     DDR_BSS ALIGNED(8);
u8 frameBufferDisparitiesMedian[MAX_USED_BUF][ALGO_FRAME_SIZE_BYTES] DDR_BSS ALIGNED(8);

// USB buffers - must be aligned to 128 bytes
u8 usbDisparities1[USB_FRAME_SIZE_BYTES + PAYLOAD_HEADER_OFFSET] DDR_BSS ALIGNED(128);
u8 usbDisparities2[USB_FRAME_SIZE_BYTES + PAYLOAD_HEADER_OFFSET] DDR_BSS ALIGNED(128);
u8 usbDisparities3[USB_FRAME_SIZE_BYTES + PAYLOAD_HEADER_OFFSET] DDR_BSS ALIGNED(128);

u8* usbBufPtr[MAX_USED_BUF] = {usbDisparities1, usbDisparities2, usbDisparities3};

// LOS - LRT messaging buffers
leonIPCChannel_t LRTtoLOSChannel CMX_DIRECT_DATA;
uint32_t messagePool[MSG_QUEUE_SIZE * MSG_SIZE] CMX_DIRECT_DATA;

// Camera configuration
GenericCamSpec *staticCamConfig = &imx208_2L_1936x1096_RAW10_30Hz_camCfg;

volatile ALIGNED(4) CMX_DIRECT_DATA u32 bProcessingLeft = 0;
volatile ALIGNED(4) CMX_DIRECT_DATA u32 bProcessingRight = 0;
volatile ALIGNED(4) CMX_DIRECT_DATA u32 usbFrameCtr = FIRST_OUTGOING_BUF_ID;
volatile ALIGNED(4) u32 newCamFrameCtrLeft = FIRST_INCOMING_BUF_ID;
volatile ALIGNED(4) u32 newCamFrameCtrRight = FIRST_INCOMING_BUF_ID;
volatile ALIGNED(4) u32 processingFrameCtr;

GenericCameraHandle camHndlLeft, camHndlRight;
CamUserSpec userCamLeftConfig, userCamRightConfig;
frameSpec camLeftFrameSpec, camRightFrameSpec;
callbacksListStruct callbacks = {0};
interruptsCallbacksListType isrCallbacks = {0};

frameBuffer camLeftFrame[MAX_USED_BUF];
frameBuffer camRightFrame[MAX_USED_BUF];

// SIPP pipelines
extern u16 lutTable[256] ALIGNED(8);  //lut table in CMX only
SippPipeline *pl, *plLutLeft, *plLutRight, *plMedian, *plDownscale, *plUpscale;
SippFilter *dmaInLeft, *dmaInRight, *dmaIn;
SippFilter *lutLeft, *lutRight, *median;
SippFilter *dmaOutLeft, *dmaOutRight, *dmaOut;
DmaParam *dmaInCfgLeft, *dmaInCfgRight, *dmaInCfg;
DmaParam *dmaOutCfgLeft, *dmaOutCfgRight, *dmaOutCfg;
LutParam *lutCfgLeft, *lutCfgRight;
MedParam *medCfg;

// ----------------------------------------------------------------------------
// Shave entrypoints
extern u32 DisparityMap0_disparityMapInit;
extern u32 DisparityMap1_disparityMapInit;
extern u32 DisparityMap2_disparityMapInit;
extern u32 DisparityMap3_disparityMapInit;

extern u32 DisparityMap0_disparityMapPyrInit;
extern u32 DisparityMap1_disparityMapPyrInit;
extern u32 DisparityMap2_disparityMapPyrInit;
extern u32 DisparityMap3_disparityMapPyrInit;

extern u32 DisparityMap0_disparityMapRunPatches;
extern u32 DisparityMap1_disparityMapRunPatches;
extern u32 DisparityMap2_disparityMapRunPatches;
extern u32 DisparityMap3_disparityMapRunPatches;

extern u32 DisparityMap0_Preprocessing;
static u32 FnDisparityMapPreprocess = (u32)&DisparityMap0_Preprocessing;

extern u32 DisparityMap0_Postprocessing;
static u32 FnDisparityMapPostprocess = (u32)&DisparityMap0_Postprocessing;

static u32 listShaves[SHAVES_USED] = {0, 1, 2, 3};
static u32 entryPointsDisparityMap[SHAVES_USED][2] = {
    {(u32)&DisparityMap0_disparityMapInit, (u32)&DisparityMap0_disparityMapRunPatches},
    {(u32)&DisparityMap1_disparityMapInit, (u32)&DisparityMap1_disparityMapRunPatches},
    {(u32)&DisparityMap2_disparityMapInit, (u32)&DisparityMap2_disparityMapRunPatches},
    {(u32)&DisparityMap3_disparityMapInit, (u32)&DisparityMap3_disparityMapRunPatches}};

// Homography for rectification
static float homography[9] = {1.04105487029507, -0.002282253354078164, -21.72629830610492,
                    0.01366118255700026, 1.020929871530532, -0.9085457892898896,
                    1.824049003017467e-005, 1.82809478390662e-005, 1};
static float homographyInversed[9];

// Run modes
typedef enum
{
    CAMERA_DEMO = 0,
    STATIC_IMAGES = 1,
} runMode_t;

#ifdef PROFILING
const volatile runMode_t runConfig = STATIC_IMAGES;
#else
const volatile runMode_t runConfig = CAMERA_DEMO;
#endif

// 4: Static Function Prototypes
// ----------------------------------------------------------------------------
frameBuffer* AllocateNextLeftCamFrameBuf(void);
frameBuffer* AllocateNextRightCamFrameBuf(void);

// 5: Functions Implementation
// ----------------------------------------------------------------------------

void generateLutTable(void)
{
    u16 i;

    // Look-up table for 10->8 bits conversion
    for (i = 0; i < 256; i++)
    {
        lutTable[i] = i;
    }
}

void prepareDriverData(void)
{
    int ibuf;

    // User extra settings
    userCamLeftConfig.mipiControllerNb  = CAM_B1_MIPICTRL;                      // Left camera connected to CAMB connecter of the board
    userCamLeftConfig.receiverId        = SIPP_DEVICE1;                         //
#ifdef MV0212
    userCamLeftConfig.sensorResetPin    = MV0212_MV0200_SENSOR_RIGHT_RST_GPIO;
#elif defined(MV0182)
    userCamLeftConfig.sensorResetPin    = MV0182_MV0200_SENSOR_RIGHT_RST_GPIO;   //
#endif
    userCamLeftConfig.stereoPairIndex   = CAM_B1_RIGHT_ADDR;                     //

    userCamLeftConfig.windowColumnStart = CAM_WINDOW_START_COLUMN;
    userCamLeftConfig.windowRowStart    = CAM_WINDOW_START_ROW;
    userCamLeftConfig.windowWidth       = CAM_WINDOW_WIDTH;
    userCamLeftConfig.windowHeight      = CAM_WINDOW_HEIGHT;
    userCamLeftConfig.generateSync      = NULL;


    userCamRightConfig.mipiControllerNb  = CAM_B2_MIPICTRL; // Right camera connected to CAMB connecter of the board
    userCamRightConfig.receiverId        = CIF_DEVICE1;
#ifdef MV0212
    userCamRightConfig.sensorResetPin    = MV0212_MV0200_SENSOR_LEFT_RST_GPIO;
#elif defined(MV0182)
    userCamRightConfig.sensorResetPin    = MV0182_MV0200_SENSOR_LEFT_RST_GPIO;   //
#endif
    userCamRightConfig.stereoPairIndex   = CAM_B2_LEFT_ADDR;

    userCamRightConfig.windowColumnStart = CAM_WINDOW_START_COLUMN;
    userCamRightConfig.windowRowStart    = CAM_WINDOW_START_ROW;
    userCamRightConfig.windowWidth       = CAM_WINDOW_WIDTH;
    userCamRightConfig.windowHeight      = CAM_WINDOW_HEIGHT;
    userCamRightConfig.generateSync      = NULL;

    // Synchronize local buffers spec with driver configuration
    camLeftFrameSpec.width   = CAM_WINDOW_WIDTH;
    camLeftFrameSpec.height  = CAM_WINDOW_HEIGHT;
    camLeftFrameSpec.bytesPP = CAM_BPP;
    camLeftFrameSpec.stride  = CAM_WINDOW_WIDTH * CAM_BPP;
    camLeftFrameSpec.type    = RAW16;

    camRightFrameSpec.width   = CAM_WINDOW_WIDTH;
    camRightFrameSpec.height  = CAM_WINDOW_HEIGHT;
    camRightFrameSpec.bytesPP = CAM_BPP;
    camRightFrameSpec.stride  = CAM_WINDOW_WIDTH * CAM_BPP;
    camRightFrameSpec.type    = RAW16;

    for (ibuf = 0; ibuf < MAX_USED_BUF;ibuf++)
    {
        camLeftFrame[ibuf].spec = camLeftFrameSpec;
        camLeftFrame[ibuf].p1    = (unsigned char*)camLeftBuf[ibuf];
        camLeftFrame[ibuf].p2    = NULL;
        camLeftFrame[ibuf].p3    = NULL;

        camRightFrame[ibuf].spec = camRightFrameSpec;
        camRightFrame[ibuf].p1   = (unsigned char*)camRightBuf[ibuf];
        camRightFrame[ibuf].p2   = NULL;
        camRightFrame[ibuf].p3   = NULL;
    }

    return;
}

// Build a pipeline for LUT with two independent paths, one for every camera
void appBuildPipeline()
{
    plLutLeft = sippCreatePipeline(SIPP_SHAVE, SIPP_SHAVE, SIPP_MBIN(0));
    plLutRight = sippCreatePipeline(SIPP_SHAVE, SIPP_SHAVE, SIPP_MBIN(0));

    // Left subpipe
    dmaInLeft  = sippCreateFilter(plLutLeft, 0x00, CAM_WINDOW_WIDTH, CAM_WINDOW_HEIGHT, N_PL(1), SZ(UInt16), SIPP_AUTO, (FnSvuRun)SIPP_DMA_ID, "DMA_IN");
    lutLeft    = sippCreateFilter(plLutLeft, 0x00, CAM_WINDOW_WIDTH, CAM_WINDOW_HEIGHT, N_PL(1), SZ(UInt8), SIPP_AUTO, (FnSvuRun)SIPP_LUT_ID, "SIPP_LUT");
    dmaOutLeft = sippCreateFilter(plLutLeft, 0x00, CAM_WINDOW_WIDTH, CAM_WINDOW_HEIGHT, N_PL(1), SZ(UInt8), SIPP_AUTO, (FnSvuRun)SIPP_DMA_ID, "DMA_OUT");

    sippLinkFilter(lutLeft, dmaInLeft, 1, 1);
    sippLinkFilter(dmaOutLeft, lutLeft, 1, 1);

    // Right subpipe
    dmaInRight  = sippCreateFilter(plLutRight, 0x00, CAM_WINDOW_WIDTH, CAM_WINDOW_HEIGHT, N_PL(1), SZ(UInt16), SIPP_AUTO, (FnSvuRun)SIPP_DMA_ID, "DMA_IN");
    lutRight    = sippCreateFilter(plLutRight, 0x00, CAM_WINDOW_WIDTH, CAM_WINDOW_HEIGHT, N_PL(1), SZ(UInt8), SIPP_AUTO, (FnSvuRun)SIPP_LUT_ID, "SIPP_LUT");
    dmaOutRight = sippCreateFilter(plLutRight, 0x00, CAM_WINDOW_WIDTH, CAM_WINDOW_HEIGHT, N_PL(1), SZ(UInt8), SIPP_AUTO, (FnSvuRun)SIPP_DMA_ID, "DMA_OUT");

    sippLinkFilter(lutRight, dmaInRight, 1, 1);
    sippLinkFilter(dmaOutRight, lutRight, 1, 1);
}

// Build a pipeline for Median filter
void appBuildPipelineMedian()
{
    plMedian = sippCreatePipeline(SIPP_SHAVE, SIPP_SHAVE, SIPP_MBIN(0));

    dmaIn  = sippCreateFilter(plMedian, 0x00, CAM_WINDOW_WIDTH, CAM_WINDOW_HEIGHT, N_PL(1), SZ(UInt8), SIPP_AUTO, (FnSvuRun)SIPP_DMA_ID, 0);
    median = sippCreateFilter(plMedian, 0x00, CAM_WINDOW_WIDTH, CAM_WINDOW_HEIGHT, N_PL(1), SZ(UInt8), SIPP_AUTO, (FnSvuRun)SIPP_MED_ID, 0);
    dmaOut = sippCreateFilter(plMedian, 0x00, CAM_WINDOW_WIDTH, CAM_WINDOW_HEIGHT, N_PL(1), SZ(UInt8), SIPP_AUTO, (FnSvuRun)SIPP_DMA_ID, 0);

    sippLinkFilter(median, dmaIn, MEDIAN_KERNEL_SIZE, MEDIAN_KERNEL_SIZE);
    sippLinkFilter(dmaOut, median, 1, 1);
}
// Configure LUT filter
void appConfigureFilters(void)
{
    dmaInCfgLeft  = (DmaParam*)dmaInLeft->params;
    dmaOutCfgLeft = (DmaParam*)dmaOutLeft->params;
    lutCfgLeft    = (LutParam*)lutLeft->params;

    dmaInCfgRight  = (DmaParam*)dmaInRight->params;
    dmaOutCfgRight = (DmaParam*)dmaOutRight->params;
    lutCfgRight    = (LutParam*)lutRight->params;

    dmaInCfgLeft->ddrAddr = (UInt32)&camLeftBuf[0];
    dmaOutCfgLeft->ddrAddr = (UInt32)&lutLeftBuf[0];

    dmaInCfgRight->ddrAddr = (UInt32)&camRightBuf[0];
    dmaOutCfgRight->ddrAddr = (UInt32)&lutRightBuf[0];

    // LUT configuration for left subpipe
    lutCfgLeft->cfg   = (0      )  | // LUT type: 0 for integers
                        (0  << 1)  | // channel mode : OFF, as we use a single plane here
                        (10 << 3)  | // integer input width: 10 bits
                        (0  << 8);   // numer of LUT tables: a single table (with 16 ranges/regions)
    lutCfgLeft->sizeA = 0x44444444; // LUT size: 2^4= 16 entries/range, 16 entries x 16 ranges = 256 total entries in LUT
    lutCfgLeft->sizeB = 0x44444444;
    lutCfgLeft->lut   = lutTable;
    lutCfgLeft->frmDim = ((CAM_WINDOW_HEIGHT << 16) | CAM_WINDOW_WIDTH);

    // LUT configuration for right subpipe: same configuration values like for the left but different addresses
    lutCfgRight->cfg   = (0      )  | //LUT type: 0 for integers
                         (0  << 1)  | //channel mode : OFF, as we use a single plane here
                         (10 << 3)  | //integer input width: 10 bits
                         (0  << 8);   //numer of LUT tables: a single table (with 16 ranges/regions)

    lutCfgRight->sizeA = 0x44444444;  // LUT size: 2^4= 16 entries/range, 16 entries x 16 ranges = 256 total entries in LUT
    lutCfgRight->sizeB = 0x44444444;  //
    lutCfgRight->lut   = lutTable;
    lutCfgRight->frmDim = ((CAM_WINDOW_HEIGHT << 16) | CAM_WINDOW_WIDTH);

    configureLutFilterSiliconSpecific(lutCfgLeft);
    configureLutFilterSiliconSpecific(lutCfgRight);
}

// Configure Median filter
void appConfigureFilterMedian(void)
{
    dmaInCfg  = (DmaParam*)dmaIn->params;
    dmaOutCfg = (DmaParam*)dmaOut->params;
    medCfg    = (MedParam*)median->params;

    dmaInCfg->ddrAddr    = (UInt32)&frameBufferDisparities[0];
    dmaOutCfg->ddrAddr   = (UInt32)&frameBufferDisparitiesMedian[0];

    // Median config
    medCfg->cfg   =  MEDIAN_KERNEL_SIZE                                   //kernel sz
                   |(((MEDIAN_KERNEL_SIZE * MEDIAN_KERNEL_SIZE)>>1) << 8) //output sel
                   |( 0x1ff << 16);                                       //threshold
}

// Callback for left camera
frameBuffer* AllocateNextLeftCamFrameBuf(void)
{
    ++newCamFrameCtrLeft;
    return (&camLeftFrame[newCamFrameCtrLeft % MAX_USED_BUF]);
}

// Callback for right camera
frameBuffer* AllocateNextRightCamFrameBuf(void)
{
    ++newCamFrameCtrRight;
    return (&camRightFrame[newCamFrameCtrRight % MAX_USED_BUF]);
}

// Preprocessing function for computing H^(-1)
void preprocessing(fp32 *H, fp32 *inverseH)
{
    performanceStruct perfStr;
    if (runConfig == STATIC_IMAGES)
    {
        swcShaveProfInit(&perfStr);
        PROF_PRINT("--- SHAVE Profiling for Inverse Homography H^-1 --- \n");
        swcShaveProfStartGathering(INVERSION_SHAVE, &perfStr);
    }

    swcResetShave(INVERSION_SHAVE);
    swcSetAbsoluteDefaultStack(INVERSION_SHAVE);

    // Compute inverse of homography
    swcStartShaveCC(INVERSION_SHAVE, FnDisparityMapPreprocess, "ii", H,
                    inverseH);

    swcWaitShave(INVERSION_SHAVE);

    if (runConfig == STATIC_IMAGES)
    {
        swcShaveProfStopGathering(INVERSION_SHAVE, &perfStr);
        swcShaveProfPrint(INVERSION_SHAVE, &perfStr);
        PROF_PRINT("-------------------------------------------------- \n");
    }

    DrvShaveL2CachePartitionFlushAndInvalidate(PARTITION_0);
}

// Postprocessing function for converting from YUV400 to YUV422
void postprocessing(u8* input, u16* output, u32 width, u32 height)
{
    performanceStruct perfStr;
    if (runConfig == STATIC_IMAGES)
    {
        swcShaveProfInit(&perfStr);
        PROF_PRINT("--- SHAVE Profiling for Postprocessing YUV422 Conversion --- \n");
        swcShaveProfStartGathering(CONVERSION_SHAVE, &perfStr);
    }

    swcResetShave(CONVERSION_SHAVE);
    swcSetAbsoluteDefaultStack(CONVERSION_SHAVE);

    // Compute inverse of homography
    swcStartShaveCC(CONVERSION_SHAVE, FnDisparityMapPostprocess, "iiii", input,
                    output, width, height);

    swcWaitShave(CONVERSION_SHAVE);

    if (runConfig == STATIC_IMAGES)
    {
        swcShaveProfStopGathering(CONVERSION_SHAVE, &perfStr);
        swcShaveProfPrint(CONVERSION_SHAVE, &perfStr);
        PROF_PRINT("------------------------------------------------------------ \n");
    }

    DrvShaveL2CachePartitionFlushAndInvalidate(PARTITION_0);
}

// Initialization of line buffers for Disparity Map
void initializeDisparityMap(AlgorithmConfig* algoCfg, u32 width)
{
    u32 shaveNumber, shaveIdx;
    performanceStruct perfStr;

    if (runConfig == STATIC_IMAGES)
    {
        swcShaveProfInit(&perfStr);
        PROF_PRINT("--- SHAVE Profiling for Disparity Map Initialization --- \n");
        swcShaveProfStartGathering(PROFILING_SHAVE, &perfStr);
    }

    for (shaveIdx = 0; shaveIdx < SHAVES_USED; shaveIdx++)
    {
        shaveNumber = listShaves[shaveIdx];
        swcResetShave(shaveNumber);
        swcSetAbsoluteDefaultStack(shaveNumber);

        swcStartShaveCC(shaveNumber, entryPointsDisparityMap[shaveNumber][0],
                        "ii", algoCfg, width); // disparityMapInit
    }
    swcWaitShaves(SHAVES_USED, listShaves);

    if (runConfig == STATIC_IMAGES)
    {
        swcShaveProfStopGathering(CONVERSION_SHAVE, &perfStr);
        swcShaveProfPrint(CONVERSION_SHAVE, &perfStr);
        PROF_PRINT("------------------------------------------------------- \n");
    }

    DrvShaveL2CachePartitionFlushAndInvalidate(PARTITION_0);
}

// Run Disparity Map computation
void runDisparityMap(AlgorithmConfig* algoCfg, DisparityConfig* dispCfg,
                     u32 width, u32 height)
{
    u32 shaveNumber, shaveIdx;
    performanceStruct perfStr;

    if (runConfig == STATIC_IMAGES)
    {
        swcShaveProfInit(&perfStr);
        PROF_PRINT("--- SHAVE Profiling for Disparity Map Computation --- \n");
        swcShaveProfStartGathering(PROFILING_SHAVE, &perfStr);
    }

    for (shaveIdx = 0; shaveIdx < SHAVES_USED; shaveIdx++)
    {
        shaveNumber = listShaves[shaveIdx];
        swcResetShave(shaveNumber);

        swcSetAbsoluteDefaultStack(shaveNumber);

        swcStartShaveCC(shaveNumber, entryPointsDisparityMap[shaveNumber][1],
                        "iiii", algoCfg, &dispCfg[shaveNumber], width, height);
    }

    swcWaitShaves(SHAVES_USED, listShaves);

    if (runConfig == STATIC_IMAGES)
    {
        swcShaveProfStopGathering(PROFILING_SHAVE, &perfStr);
        swcShaveProfPrint(PROFILING_SHAVE, &perfStr);
        PROF_PRINT("--------------------------------------------------------- \n");
    }

    DrvShaveL2CachePartitionFlushAndInvalidate(PARTITION_0);
}

void computeDisparityMap(AlgorithmConfig* algoCfg, DisparityConfig* dispCfg,
                         u32 width, u32 height, u32 frameNumber)
{

    // Split image into patches
    if (runConfig == CAMERA_DEMO)
        computePatches((u8*) &lutRightBuf[frameNumber],
                       (u8*) &lutRightBufRectified[frameNumber], disparityMap,
                       algoCfg, dispCfg, width, height);
    else
    if (runConfig == STATIC_IMAGES)
        computePatches((u8*) &frameBufferLeft[0], (u8*) &frameBufferRight[0],
                       disparityMap, algoCfg, dispCfg, width, height);

    // Run disparity map computation on 4 SHAVEs
    initializeDisparityMap(algoCfg, width);
    runDisparityMap(algoCfg, dispCfg, PATCH_WIDTH, PATCH_HEIGHT);

    // Re-construct disparity map from patches
    recomputeImageFromPatches(&frameBufferDisparities[0], dispCfg);

    // Apply median filtering on disparity map
    dmaInCfg->ddrAddr = (UInt32) &frameBufferDisparities[0];
    dmaOutCfg->ddrAddr = (UInt32) &frameBufferDisparitiesMedian[frameNumber];

    sippProcessFrame(plMedian);
}

/*static*/volatile int bicubicWaitFlag = 0;
void bicubicIrqHandler()
{
    bicubicWaitFlag = 1;
}
// Main function
int main()
{
    // Variable declaration
    const u32 width = CAM_WINDOW_WIDTH;
    const u32 height = CAM_WINDOW_HEIGHT;
    s32 boardStatus;

    u32 status;
    UNUSED(status);
    camErrorType camStatus;

    DisparityConfig dispCfg[MAX_PATCHES];
    AlgorithmConfig algoCfg;

    s32 sc;

    // Initialize memory and camera configuration
    sc = initClocksAndMemory();
    if (sc)
        return sc;
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

    boardStatus = BoardInitExtPll(EXT_PLL_CFG_74_24_24MHZ);
    if (boardStatus != BRDCONFIG_SUCCESS)
    {
    	printf("Error: board initialization failed with %ld status\n",
    			boardStatus);
    	return -1;
    }
    rc = BoardGetI2CInfo(info, NUM_I2C_DEVS);
    i2cHandle=info[0].handler;
#elif defined(MV0182)
    boardStatus = BoardInitialise(EXT_PLL_CFG_148_24_24MHZ);
    i2cHandle=gAppDevHndls.i2c0Handle;
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

    // Generate rectified coordinates based on homography
    preprocessing(homography, homographyInversed);

    // Set bicubic context parameters
    bicubicWarpContext bicubicCtx;
    bicubicCtx.xyRectifiedBuffer = (fp32*)&xyRectifiedBuffer[0];
    bicubicCtx.homography = homographyInversed;
    bicubicCtx.width = MAX_WIDTH;
    bicubicCtx.height = MAX_HEIGHT;
    bicubicCtx.bpp = 1;
    bicubicCtx.pixelFormat = 1;
    bicubicCtx.hndl = &bicubicIrqHandler;
    bicubicCtx.irqNumber = IRQ_BICUBIC;
    bicubicWarpGenerateMeshHomographyRTP(&bicubicCtx);

    // Configure SIPP pipelines
    printf("Configuring the SIPP pipelines\n");
    sippInitialize();
    appBuildPipeline();
    appConfigureFilters();
    sippFinalizePipeline(plLutLeft);
    sippFinalizePipeline(plLutRight);

    appBuildPipelineMedian();
    appConfigureFilterMedian();
    sippFinalizePipeline(plMedian);

    // Start left camera
    printf("Configuring imx208 dual cameras and datapath\n");
    isrCallbacks.getBlock = NULL;
    isrCallbacks.getFrame = AllocateNextLeftCamFrameBuf;
    isrCallbacks.notification = NULL;
    callbacks.isrCbfList = &isrCallbacks;
    callbacks.sensorCbfList = NULL;
    camStatus = CamInit(&camHndlLeft, staticCamConfig, &userCamLeftConfig,
                        &callbacks, i2cHandle); // this is a SIPP camera

    if (camStatus != CAM_SUCCESS)
    {
        printf("\n Camera CAM_B1 configuration failed (%d).\n", camStatus);
        return -1;
    }
    CamStart(&camHndlLeft);

    // Start right camera
    isrCallbacks.getBlock = NULL;
    isrCallbacks.getFrame = AllocateNextRightCamFrameBuf;
    isrCallbacks.notification = NULL;
    callbacks.isrCbfList = &isrCallbacks;
    callbacks.sensorCbfList = NULL;
    camStatus = CamInit(&camHndlRight, staticCamConfig, &userCamRightConfig,
                        &callbacks, i2cHandle); // this is a CIF camera

    if (camStatus != CAM_SUCCESS)
    {
        printf("\n Camera CAM_B2 configuration failed (%d).\n", camStatus);
        return -1;
    }
    CamStart(&camHndlRight);

    // Initializations
    setBuffers((u8*) frameBufferDisparitiesMedian, (u8*) frameBufferDisparities,
               disparityMap, usbBufPtr);
    status = LeonIPCTxInit(&LRTtoLOSChannel, messagePool, MSG_QUEUE_SIZE,
                           MSG_SIZE);
    DrvLeonRTSignalBootCompleted();

    if (runConfig == STATIC_IMAGES)
    {
        loadMemFromFileSimple("Input//inputFrameLeft_640x480.bin",
                              MAX_WIDTH * MAX_HEIGHT, (void*) frameBufferLeft);
        loadMemFromFileSimple("Input//inputFrameRight_640x480.bin",
                              MAX_WIDTH * MAX_HEIGHT, (void*) frameBufferRight);
    }

    //the processing is based on the left cam timings !
    processingFrameCtr = newCamFrameCtrLeft;

    printf("Run Disparity Map algorithm.\n");
    tyTimeStamp timerDataBic, timerDataSipp;
    u64 cyclesElapsed = 0;
    u32 toSend = 0;
    /* TODO: Set this flag after each frame to wait the bicubic correctly.
       Right now this works because shave processing is slower than bicubic
       (producer quicker than consumer) */
    bicubicWaitFlag = 0;
    while(1)
    {
        if( newCamFrameCtrLeft != processingFrameCtr )
        {
            processingFrameCtr = newCamFrameCtrLeft;
            if (runConfig == STATIC_IMAGES)
            {
                status = DrvTimerStartTicksCount(&timerDataSipp);
            }

            // I. Convert input from raw10 to raw8 (yuv400)
            dmaInCfgLeft->ddrAddr   = (UInt32)&camLeftBuf[(processingFrameCtr-1) % MAX_USED_BUF];
            dmaInCfgRight->ddrAddr  = (UInt32)&camRightBuf[(processingFrameCtr-1) % MAX_USED_BUF];
            dmaOutCfgLeft->ddrAddr  = (UInt32)&lutLeftBuf[(processingFrameCtr-1) % MAX_USED_BUF];
            dmaOutCfgRight->ddrAddr = (UInt32)&lutRightBuf[(processingFrameCtr-1) % MAX_USED_BUF];
            sippProcessFrame(plLutLeft);
            sippProcessFrame(plLutRight);

            if (runConfig == STATIC_IMAGES)
            {
                status = DrvTimerGetElapsedTicks(&timerDataSipp, &cyclesElapsed);
                PROF_PRINT("--- LEON Profiling for SIPP YUV400 Conversion ---\n");
                PROF_PRINT("    Executed clock cycles:   %llu\n", cyclesElapsed);
                PROF_PRINT("-------------------------------------------------\n");
                status = DrvTimerStartTicksCount(&timerDataBic);
            }

            // II. Rectification step
            bicubicWarpInit(&bicubicCtx);
            bicubicCtx.inputImageBuffer = (u8*)&lutLeftBuf[(processingFrameCtr-1) % MAX_USED_BUF];
            bicubicCtx.outputImageBuffer = (u8*)&lutRightBufRectified[(processingFrameCtr-1) % MAX_USED_BUF];
            bicubicWarpProcessFrame(&bicubicCtx);
            while(!bicubicWaitFlag);

            if (runConfig == STATIC_IMAGES)
            {
                status = DrvTimerGetElapsedTicks(&timerDataBic, &cyclesElapsed);
                PROF_PRINT("--- LEON Profiling for Rectification Step ---\n");
                PROF_PRINT("    Executed clock cycles:   %llu\n", cyclesElapsed);
                PROF_PRINT("-------------------------------------------------\n");
            }

            // III. Compute disparity map
            computeDisparityMap(&algoCfg, dispCfg, width, height, (processingFrameCtr-2) % MAX_USED_BUF);

            // IV. Convert output from YUV400 to YUV422 to stream on USB
            postprocessing((u8*)&frameBufferDisparitiesMedian[(processingFrameCtr - 2) % MAX_USED_BUF], (u16*)&usbBufPtr[(usbFrameCtr) % MAX_USED_BUF][PAYLOAD_HEADER_OFFSET], width, height);

            // V. Notify USB that it can send the new processed frame
            toSend = 1;
            status = LeonIPCSendMessage(&LRTtoLOSChannel, &toSend);
            usbFrameCtr++;

            // For static images run algo only once
            if (runConfig == STATIC_IMAGES)
            {
                break;
            }
        }
    }

    if (runConfig == STATIC_IMAGES)
    {
        unitTestInit();

        // check CRC
        u32 crcUnitTest = swcCalcCrc32((u8*)&frameBufferDisparitiesMedian[(processingFrameCtr - 2) % MAX_USED_BUF],
                                       sizeof(unsigned char) * width * height, le_pointer);
        unitTestAssert(crcUnitTest == GOLDEN_CRC);

        // final report
        unitTestFinalReport();
    }

    printf("Algorithm finished successfully!\n");

    // Send LOS stop message
    DrvLeonRTSignalStop();

    return 0;
}
