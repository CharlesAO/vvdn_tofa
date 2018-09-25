/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    oPipe: 13Mpix real life params with dbyr.y buffer distributed
///                  over 3 slices

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <UnitTestApi.h>
#include <VcsHooksApi.h>
#include <DrvCpr.h>
#include <DrvTimer.h>
#include "DrvTempSensor.h"
#include "Opipe.h"
#include "OpipeApps.h"
#include "OpipeInternals.h"
#include <swcCrc.h>
#include "app_config.h"
#include "isp_params_wrap.h"
#include "brdMv0198.h"
#ifdef MV0212
#include "MV0212.h"
#include "brdGpioCfgs/brdMv0182R5GpioDefaults.h"
#include "DrvCDCEL.h"
#elif defined(MV0182)
#include "brdMv0182.h"
#include "Board182Api.h"
#endif
#include "DrvLeonL2C.h"
#include "DrvAmcMa2x5x.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
//Number of runs
#define N_RUNS 10

//Input resolution
#define IMG_W ISPC_BAYER_W
#define IMG_H ISPC_BAYER_H

//Output resolution
#define O_IMG_W  ((IMG_W * N -1)/D + 1)
#define O_IMG_H  ((IMG_H * N -1)/D + 1)

#define CHROMA_SZ (O_IMG_W*O_IMG_H/4) // yuv420

#define EXPECTED_CRC_VALUE  0xd0a13e50
#define OUTPUT_IMAGE "Lab_8bpp_4208x3120_P420.yuv"

typedef struct
{
    tyAdcResultAllRails power;
    fp32 avgmW;
    fp32 avgmA;
} Prof;

#ifdef MV0212
#define NUM_I2C_DEVS 3
#endif
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
static I2CM_Device * i2c2Handle;
// 4: Static Local Data
// ----------------------------------------------------------------------------

// Main full buffers in DDR
static u16    iBuf[        IMG_W * IMG_H            ] SECTION(".ddr.bss") ALIGNED(8);    // ISPC_BAYER_BITS_STILL
static u8   yuvBuf[O_IMG_W * O_IMG_H + 2 * CHROMA_SZ] SECTION(".ddr.bss") ALIGNED(8);    // yuv420

//Circular buffers in CMX (DO NOT CHANGE LAYOUT !!!)
static u16 cDbyrY  [  IMG_W * DBYR_Y_H     ] SECTION(".shv0.S.data") ALIGNED(8);  // Slices [0,1,2]
static u16 cSigma  [  IMG_W * I_CBUFF_H    ] SECTION(".shv3.S.data") ALIGNED(8);  // Bayer is 10bit
static u16 cSharpY [  IMG_W * SHARP_Y_H    ] SECTION(".shv3.S.data") ALIGNED(8);
static u8  cLut    [  IMG_W * O_CBUFF_H * 3] SECTION(".shv3.S.data") ALIGNED(8);
static u8  cUpfirDn[O_IMG_W * O_CBUFF_H * 3] SECTION(".shv3.S.data") ALIGNED(8);  // As for yuv444

static OpipeMF opF;
static u64 startTime, endTime, frameTime;
static tyBrd198Handle powerMonHandle;
static I2CM_Device* i2c;
static Prof test;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
static void appBuildPipe();
static void showPower(float temp_CSS,float temp_MSS,float temp_UPA0,float temp_UPA1);
static void stopTimer();
static void initPowerMeasurement(I2CM_Device* i2cBus);
static void DrvSliceDbyrLumaBuff(Opipe *p);

// 6: Functions Implementation
// ----------------------------------------------------------------------------
static void appBuildPipe()
{
  //Must specify buffers first
  opF.in.cBufSigma.base = (uint32_t)cSigma;   opF.in.cBufSigma.h = I_CBUFF_H;
  opF.cBufDbyrY.base    = (uint32_t)cDbyrY;   opF.cBufDbyrY.h    = DBYR_Y_H;
  opF.cBufSharp.base    = (uint32_t)cSharpY;  opF.cBufSharp.h    = SHARP_Y_H;
  opF.cBufLut.base      = (uint32_t)cLut;     opF.cBufLut.h      = O_CBUFF_H;
  opF.cBufPoly.base     = (uint32_t)cUpfirDn; opF.cBufPoly.h     = O_CBUFF_H;

  OpipeCreateFull(&opF, BPP(2));
}

static void showPower(float temp_CSS,float temp_MSS,float temp_UPA0,float temp_UPA1)
{

    fp32 avgmW = 0;
    fp32 avgDdrMw = 0;
    fp32 coreMw;

    // Compute all averages across all samples
    fp32 ddrMw;
    fp32 ddrMa;

    UNUSED(ddrMa);

    avgmW += test.power.totalMilliWatts;

    Brd198GetDdrPowerAndCurrent(&powerMonHandle, &test.power, &ddrMw, &ddrMa);

    avgDdrMw += ddrMw;

    test.avgmW = avgmW ;

    coreMw = test.avgmW - avgDdrMw;

    printf("Core power = %6.02fmW, DDR power = %6.02fmW, On-die junction temperature: CSS=%2.1fC, MSS=%2.1fC, UPA0=%2.1fC, UPA1=%2.1fC\n\n", coreMw, avgDdrMw, temp_CSS,temp_MSS,temp_UPA0,temp_UPA1);
}

static void stopTimer()
{
    //Stop timer
    DrvTimerGetSystemTicks64(&endTime);
}

static void initPowerMeasurement(I2CM_Device* i2cBus)
{
    s32 retVal;

    i2c = i2cBus;
    if (i2c == NULL)
    {
        printf("I2C not initialized for MV0198\n");
    }

    retVal = Brd198Init(&powerMonHandle, i2c, NULL);
    if (retVal != DRV_BRD198_DRV_SUCCESS)
    {
        printf("Board 198 init error\n");
        exit(-1);
    }
}

int main()
{
    u32 run;
    fp32 avgFps;             // frames/s
    fp32 pixelClockCycle;    // cc/pixels
    fp32 megaPixelTime;      // MPs/s
    s32 boardStatus;
    u32 crc32;
    float temperature_CSS;
    float temperature_MSS;
    float temperature_UPA0;
    float temperature_UPA1;
    int32_t tempSensorStat = 0;

    unitTestInit ();

    swcLeonSetPIL (2);    //Interrupts get enabled here, by lowering the priority

    initClocksAndMemory ();
    DrvCprStartAllClocks ();

    tempSensorStat = DrvTempSensorSetMode(TSENS_CSS, TSENS_ONE_SHOT_MODE, TSENS_SAMPLE_TEMP);
    tempSensorStat += DrvTempSensorSetMode(TSENS_MSS, TSENS_ONE_SHOT_MODE, TSENS_SAMPLE_TEMP);
    tempSensorStat += DrvTempSensorSetMode(TSENS_UPA0, TSENS_ONE_SHOT_MODE, TSENS_SAMPLE_TEMP);
    tempSensorStat += DrvTempSensorSetMode(TSENS_UPA1, TSENS_ONE_SHOT_MODE, TSENS_SAMPLE_TEMP);
    if (tempSensorStat != 0)
    {
        printf("Error code %d returned while executing Set Mode for Temperature Sensor\n", (int)tempSensorStat);
    }
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
    boardStatus = BoardInitExtPll(EXT_PLL_CFG_148_24_24MHZ);
    if (boardStatus != BRDCONFIG_SUCCESS)
    {
    	printf("Error: board initialization failed with %ld status\n",
    			boardStatus);
    	return -1;
    }
    rc = BoardGetI2CInfo(info, NUM_I2C_DEVS);
    i2c2Handle=info[2].handler;
#elif defined(MV0182)
    boardStatus = BoardInitialise(0);
    i2c2Handle=gAppDevHndls.i2c2Handle;
    if (boardStatus != B_SUCCESS)
    {
    	printf("Error: board initialization failed with %ld status\n",
    			boardStatus);
    	return -1;
    }
#endif

    reduceBaselinePower ();

    initPowerMeasurement (i2c2Handle);
    u32 freq =  DrvCprGetClockFreqKhz(SYS_CLK, NULL) / 1000;

    printf("\nIspHighThroughput benchmark\n");
    printf("===============================================================\n");
    printf("Resolution: %lux%lu\n", (u32)IMG_W, (u32)IMG_H);
    printf("Frequency:  %luMHz\n\n", freq);
    printf("Loading 13MP file...        (estimated loading time: 2min)\n");
    // INPUT_IMAGE is defined in Makefile
    loadMemFromFileSimple (INPUT_IMAGE, IMG_W * IMG_H * 2, iBuf);

    DrvSetSippClkCtrlRegister((0x18FFF | (1<<MSS_SP_SIPP_ABPSLV)));

    OpipeReset  (); //general inits
    appBuildPipe(); //calls OpipeInit(p)
    opF.p.cbPreStart = DrvSliceDbyrLumaBuff;
    opF.p.cbEndOfFrame = stopTimer;

    DrvSetAmcPriorities(0x0,0x40000,0x208400,0x00000000);

    printf("\nProcessing %d iterations\n\n", N_RUNS);
    //Frame based setup:
    appSetParams(&opF.p);
    //Setup resolution AFTER polyFir params pointers are set !!!
    OpipeSetSizeMF (&opF, IMG_W, IMG_H);

    for(run=0; run < N_RUNS; run++)
    {
        opF.pIn->ddr.base    = (uint32_t)iBuf;
        opF.pOutY->ddr.base  = (uint32_t)&yuvBuf[0];
        opF.pOutUV->ddr.base = (uint32_t)&yuvBuf[O_IMG_W*O_IMG_H];
        //Actual run:
        DrvTimerGetSystemTicks64(&startTime);
        OpipeStart (&opF.p);
        // Blocking call to sample all power rails. Takes about 20ms.
        DrvTimerSleepMicro(2000);// small wait 2ms before sampling power rails - to ensure pipe is really active
        u32 ret = Brd198SampleAllRails(&powerMonHandle,&(test.power));

        // Needed to check the clock allocation on multiple boards
        // See http://dub30/bugzilla/show_bug.cgi?id=24166
        /*
        OpipeWait(&opF.p);
        while (1==1){
            OpipeStart (&opF.p);
            OpipeWait(&opF.p);
        }
        */

        if (ret != DRV_BRD198_DRV_SUCCESS)
        {
            printf("Error reading power rails: %lu\n", ret);
            exit(-1);
        }
        OpipeWait(&opF.p);

        tempSensorStat = DrvTempSensorGetSample(TSENS_CSS, &temperature_CSS);
        tempSensorStat += DrvTempSensorGetSample(TSENS_MSS, &temperature_MSS);
        tempSensorStat += DrvTempSensorGetSample(TSENS_UPA0, &temperature_UPA0);
        tempSensorStat += DrvTempSensorGetSample(TSENS_UPA1, &temperature_UPA1);
        if (tempSensorStat != 0)
        {
            printf("Error code %d returned while executing Read Temperature Sensor value\n", (int)tempSensorStat);
        }
        frameTime = endTime - startTime;
        avgFps = (fp32) (DrvCprGetClockFreqKhz(SYS_CLK, NULL) * 1000) / (float) frameTime;
        pixelClockCycle = (fp32)frameTime / (fp32)(IMG_W * IMG_H);
        megaPixelTime = (fp32)(avgFps * IMG_W * IMG_H)/1000000;
        printf("%.03f fps, %llu cc/frame, %.03f cc/pixel, %.03f MP/s\n", avgFps, frameTime, pixelClockCycle, megaPixelTime);
        showPower(temperature_CSS,temperature_MSS,temperature_UPA0,temperature_UPA1);
    }

    printf("===============================================================\n");
    // Check CRC
    printf("Validating test...\n");
    crc32 = swcCalcCrc32(yuvBuf, sizeof(yuvBuf), be_pointer);
    unitTestAssert(crc32 == EXPECTED_CRC_VALUE);
    // Save output image
    printf("Saving output image - will take about 3 minutes ...\n");
    saveMemoryToFile((uint32_t)yuvBuf, sizeof(yuvBuf), OUTPUT_IMAGE);

    printf ("Memory dumping finished.\n");

    unitTestFinalReport();
    return 0;
}

static void DrvSliceDbyrLumaBuff(Opipe *p){
    #define SLICE_WIDTH 2048
    #define FIRST_SLC 0

    DrvSetSliceDbyrLumaBuff(p,SLICE_WIDTH, FIRST_SLC);

}
