///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///

// 1: Includes
// ----------------------------------------------------------------------------
// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Common libraries
#include "mv_types.h"
#include "registersMyriad.h"
#include "swcShaveLoader.h"
#include "swcLeonUtils.h"
#include "swcFrameTypes.h"
#include "swcCrc.h"

// Drivers
#ifdef MV0212
#include "MV0212.h"
#include "brdGpioCfgs/brdMv0182R5GpioDefaults.h"
#elif defined(MV0182)
#include "Board182Api.h"
#endif
#include "DrvCDCEL.h"
#include "brdMv0198.h"
#include "DrvLeonL2C.h"
#include "DrvSvu.h"
#include "DrvLeon.h"
#include "DrvRegUtils.h"
#include "DrvTimer.h"
#include "DrvRegUtils.h"
#include "DrvShaveL2Cache.h"

#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
#include "DrvTempSensor.h"
#endif

// Components
#include "UnitTestApi.h"
#include "VcsHooksApi.h"
#include "dbgTracerApi.h"

// Local
#include "app_config.h"

// Shared
#include "kernel_config.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#ifndef TEST_FRAME_WIDTH
#define TEST_FRAME_WIDTH    1280
#endif
#ifndef TEST_FRAME_HEIGHT
#define TEST_FRAME_HEIGHT   720
#endif

#define SHAVES_LIST         {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}

#define GOLDEN_CRC          0x14a4be4f

#ifndef NB_FRAMES_TO_PROF
#define NB_FRAMES_TO_PROF 1000
#endif

#ifndef MAX_SAMPLES_PER_TEST
#define MAX_SAMPLES_PER_TEST 100
#endif

#define CLOCKS_FOR_SHAVES(X) ((1<<X)-1)

typedef enum
{
    WITHOUT_CRC32_CHECK = 0,
    WITH_CRC32_CHECK    = 1,
} crc32Check;

typedef struct
{
    u32 shaveNumber;
    u32 lastFrameCRC;
    u64 totalClocks;
    s32 sampleNumber;
    tyAdcResultAllRails power[MAX_SAMPLES_PER_TEST];

    u32 averageClocks;
    u32 averageFps;
    u32 averageGflops;
    fp32 averagemW;
    fp32 averagemA;
#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    fp32 temperature_CSS;
    fp32 temperature_MSS;
    fp32 temperature_UPA0;
    fp32 temperature_UPA1;
#endif
} Profile;

#ifdef MV0212
#define NUM_I2C_DEVS 3
#endif
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
static I2CM_Device * i2c2Handle;

// Shaves entrypoints
extern u32  MaxFlops0_AppStart3KernelsStripes;
extern u32  MaxFlops1_AppStart3KernelsStripes;
extern u32  MaxFlops2_AppStart3KernelsStripes;
extern u32  MaxFlops3_AppStart3KernelsStripes;
extern u32  MaxFlops4_AppStart3KernelsStripes;
extern u32  MaxFlops5_AppStart3KernelsStripes;
extern u32  MaxFlops6_AppStart3KernelsStripes;
extern u32  MaxFlops7_AppStart3KernelsStripes;
extern u32  MaxFlops8_AppStart3KernelsStripes;
extern u32  MaxFlops9_AppStart3KernelsStripes;
extern u32  MaxFlops10_AppStart3KernelsStripes;
extern u32  MaxFlops11_AppStart3KernelsStripes;

u32* MaxFlops_AppStartStripes[SHAVES_USED]={
        &MaxFlops0_AppStart3KernelsStripes,
        &MaxFlops1_AppStart3KernelsStripes,
        &MaxFlops2_AppStart3KernelsStripes,
        &MaxFlops3_AppStart3KernelsStripes,
        &MaxFlops4_AppStart3KernelsStripes,
        &MaxFlops5_AppStart3KernelsStripes,
        &MaxFlops6_AppStart3KernelsStripes,
        &MaxFlops7_AppStart3KernelsStripes,
        &MaxFlops8_AppStart3KernelsStripes,
        &MaxFlops9_AppStart3KernelsStripes,
        &MaxFlops10_AppStart3KernelsStripes,
        &MaxFlops11_AppStart3KernelsStripes
};

// Buffers
DDR_BUF     u8 in_image[FRAME_SIZE];
DDR_BUF     u8 out_image[FRAME_SIZE];

// 4: Static Local Data
// ----------------------------------------------------------------------------
// List of Shaves
static const swcShaveUnit_t shaveList[SHAVES_USED] = SHAVES_LIST;
static const u32 shavesNumber[] = { 0, 1, 2, 4, 8, 12 };
static const u32 runs = sizeof(shavesNumber) / sizeof(shavesNumber[0]);

static frameSpec   testFrameSpec;
static frameBuffer inBuffer;
static frameBuffer outBuffer;

static tyBrd198Handle powerMonHandle;
static I2CM_Device* i2c;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

void initBuffers(int width, int height)
{
    // Set the test frame spec
    testFrameSpec.type = YUV420p;
    testFrameSpec.height = height;
    testFrameSpec.width = width;
    testFrameSpec.stride = width;
    testFrameSpec.bytesPP = 1;

    // Set the input frame buffer
    inBuffer.spec = testFrameSpec;
    inBuffer.p1 = (u8*)in_image;
    inBuffer.p2 = (u8*)(u32)(in_image) + width * height;
    inBuffer.p3 = (u8*)(u32)(in_image) + width * height +  width * height /4;

    outBuffer.spec = testFrameSpec;
    outBuffer.p1 = out_image;
    outBuffer.p2 = out_image + width * height;
    outBuffer.p3 = out_image + width * height +  width * height /4;
}

u32 calcCrc()
{
    swcLeonDataCacheFlushBlockWhilePending();
    return swcCalcCrc32(out_image, TEST_FRAME_WIDTH, be_pointer);
}

void runShaves(u32 shavesNumber, u32* entryPoints[], u32 frameNumber)
{
    u32 i;

    for (i = 0; i < shavesNumber; i++)
    {
        DrvCprPowerTurnOnIsland(POWER_ISLAND_SHAVE_0 + i);

        swcResetShave(shaveList[i]);
        swcSetShaveWindowsToDefault(shaveList[i]);
        swcSetAbsoluteDefaultStack(shaveList[i]);
        swcStartShaveCC(shaveList[i], (u32)entryPoints[i], "iiiiiii", (u32)out_image, (u32)in_image, TEST_FRAME_WIDTH, TEST_FRAME_HEIGHT, shavesNumber, i, frameNumber);
    }
}

/* The Shaves are started only once for the set number of frames because
 * the workload length must accomodate the long power rails sampling time (20ms).
 * If the Shaves were started and stopped for each frame (which takes few ms)
 * we would read lower power numbers because the Shaves would have already
 * finished by the time the rails are sampled.
 */
void run(u32 shavesNumber, u32* entryPoints[], Profile* test,crc32Check crcCheck)
{
    tyTimeStamp timeStamp;
    u64 tickCount;

    // Make sure every test starts clean to get honest performance numbers
    DrvShaveL2CachePartitionFlushAndInvalidate(PARTITION_0);
    // Cold start each time
    turnOnShavePowerIslands(shavesNumber);

    printf("Starting shaves (%lu)...", shavesNumber);

    // Start timer
    DrvTimerStartTicksCount(&timeStamp);

    // Kick Shaves for a fixed workload and start profiling
    runShaves(shavesNumber, entryPoints, NB_FRAMES_TO_PROF);

    while (swcShavesRunningArr((u32*)shaveList, shavesNumber) || (shavesNumber == 0))
    {
        // Blocking call to sample all power rails. Takes about 20ms.
        u32 ret = Brd198SampleAllRails(&powerMonHandle, &test->power[test->sampleNumber]);
        if (ret != DRV_BRD198_DRV_SUCCESS)
            printf("Error reading power rails: %lu\n", ret);

        test->sampleNumber++;

        // Sample rails until the buffer is filled
        if (test->sampleNumber >= MAX_SAMPLES_PER_TEST)
            break;
    }

    while (swcShavesRunningArr((u32*)shaveList, shavesNumber))
        ;

    // Count elapsed cycles
    DrvTimerGetElapsedTicks(&timeStamp, &tickCount);

    // Compute CRC on the last frame
    if(crcCheck == WITH_CRC32_CHECK)
    {
        test->lastFrameCRC = calcCrc();
    }
    else
    {
        //assign the expected value
        test->lastFrameCRC = GOLDEN_CRC;
    }

#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    // Measure die temperatures
    (void)DrvTempSensorGetSample(TSENS_CSS, &test->temperature_CSS);
    if(crcCheck != WITH_CRC32_CHECK) // in case no shave used the MCC_CPU power island is stopped, to read temperature it needs to be started
        DrvCprPowerTurnOnIsland(POWER_ISLAND_MSS_CPU);
    DrvCprPowerTurnOnIsland(POWER_ISLAND_MSS_SIPP);
    (void)DrvTempSensorGetSample(TSENS_MSS, &test->temperature_MSS);
    DrvCprPowerTurnOffIsland(POWER_ISLAND_MSS_SIPP);
    if(crcCheck != WITH_CRC32_CHECK) // in case no shave used and we started MSS_CPU then stopp it after temperature read.
        DrvCprPowerTurnOffIsland(POWER_ISLAND_MSS_CPU);
    (void)DrvTempSensorGetSample(TSENS_UPA0, &test->temperature_UPA0);
    (void)DrvTempSensorGetSample(TSENS_UPA1, &test->temperature_UPA1);
#endif

    // Discard last power sample to avoid the case when the Shaves finish their
    // workload and halt just before the sampling starts.
    if (shavesNumber > 0)
        test->sampleNumber--;
    // Total number of clocks spent by Shaves processing the workload
    test->totalClocks = tickCount;

    turnOffShavePowerIslands();

    printf("done\n");
}

void checkResults(Profile* tests, u32 testsNumber)
{
    u32 i;
    s32 k;
    fp32 averageClocks;

    // Compute all averages across all samples
    for (i = 0; i < testsNumber; i++)
    {
        unitTestAssert(tests[i].lastFrameCRC == GOLDEN_CRC);

        float averagemW = 0;
        float averagemA = 0;

        for (k = 0; k < tests[i].sampleNumber; k++)
        {
            averagemW += tests[i].power[k].totalMilliWatts;
            averagemA += tests[i].power[k].totalMilliAmps;
        }

        tests[i].averagemW = averagemW / tests[i].sampleNumber;
        tests[i].averagemA = averagemA / tests[i].sampleNumber;

        averageClocks = (fp32)tests[i].totalClocks / NB_FRAMES_TO_PROF;
        tests[i].averageFps = (u32)((DrvCprGetClockFreqKhz(SYS_CLK, NULL) * 1000) / averageClocks);
        tests[i].averageGflops = (u32)(NUMBER_KERNELS * (KERNEL_SIZE * KERNEL_SIZE * 2) * TEST_FRAME_WIDTH * TEST_FRAME_HEIGHT * (u64)tests[i].averageFps / (u64)pow(2.0, 30));
    }
}

void showReport(Profile* tests, u32 testsNumber)
{
    u32 i;
    u8 crcResult[10];

    printf("Report\n");
    printf("============================================================\n");

    for (i = 0; i < testsNumber; i++)
    {
        if (tests[i].lastFrameCRC == GOLDEN_CRC)
        {
            printf("%4lu frames: %14luxSHAVEs. ", (u32)NB_FRAMES_TO_PROF, tests[i].shaveNumber);
        }
        else
        {
            strncpy((char*)crcResult, "not ok\0", sizeof(crcResult));
            printf("%4lu frames: CRC %6s, %2luxSHAVEs. ", (u32)NB_FRAMES_TO_PROF, crcResult, tests[i].shaveNumber);
        }
        printf("Result: %3lu fps, %2lu gflops, power %7.2f mW\n", tests[i].averageFps, tests[i].averageGflops, tests[i].averagemW);
#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
        printf("On-die junction temperature: CSS=%2.1fC, MSS=%2.1fC, UPA0=%2.1fC, UPA1=%2.1fC\n",tests[i].temperature_CSS,tests[i].temperature_MSS,tests[i].temperature_UPA0,tests[i].temperature_UPA1);
#endif
    }
}

/* Run most common combinations of numbers of Shaves using the most common scheduling
 * pattern, capturing profiling information.
 */
void runFull(Profile* test)
{
    u32 i;


    printf("Running stripe based scheduling\n");
    printf("============================================================\n");

    // Running dry, no Shaves
    test[0].shaveNumber = shavesNumber[0];

    //for 0 shaves, no crc calculation is needed because there is no output image
    run(shavesNumber[0], MaxFlops_AppStartStripes, &test[0],WITHOUT_CRC32_CHECK);
    // So that the fps comes out as 0
    test[0].totalClocks = 999999999999999999.9f;

    // Set everything for the real processing
    powerUpShavesAndDDR();

    //load input image after the ddr was initialized
    loadMemFromFileSimple(MV_EXTRA_DATA, TEST_FRAME_WIDTH * TEST_FRAME_HEIGHT, in_image);

    initBuffers(TEST_FRAME_WIDTH, TEST_FRAME_HEIGHT);

    for (i = 1; i < runs; i++)
    {
        DrvCprTurnOnShaveMask(CLOCKS_FOR_SHAVES(shavesNumber[i]));
        test[i].shaveNumber = shavesNumber[i];
        run(shavesNumber[i], MaxFlops_AppStartStripes, &test[i],WITH_CRC32_CHECK);
        DrvCprTurnOffShaveMask(CLOCKS_FOR_SHAVES(shavesNumber[i]));
    }
}

void initialisePowerMeasurement(I2CM_Device* i2cBus)
{
    int returnValue;

    i2c = i2cBus;
    assert(i2c != NULL && "I2C not initialized for MV0198");

    returnValue = Brd198Init(&powerMonHandle, i2c, NULL);
    assert(returnValue == DRV_BRD198_DRV_SUCCESS && "Board 198 init error");
}

void initialiseHardware()
{
    s32 boardStatus;
    initClocksAndMemory();
#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    DrvTempSensConfig tempParam = {1};
#endif

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
        exit(rc);
    }

    rc = BoardGetPCBRevision(&rev);
    if (rc!=BRDCONFIG_SUCCESS)
    {
    	printf("Error: board configuration read failed with %ld status\n",
    	    			rc);
        exit(rc);
    }
    boardStatus = BoardInitExtPll(EXT_PLL_CFG_148_24_24MHZ);
    if (boardStatus != BRDCONFIG_SUCCESS)
    {
    	printf("Error: board initialization failed with %ld status\n",
    			boardStatus);
        exit(-1);
    }
    rc = BoardGetI2CInfo(info, NUM_I2C_DEVS);
    i2c2Handle=info[2].handler;
#elif defined(MV0182)
    boardStatus = BoardInitialise(EXT_PLL_CFG_148_24_24MHZ);
    i2c2Handle=gAppDevHndls.i2c2Handle;
    if (boardStatus != B_SUCCESS)
    {
    	printf("Error: board initialization failed with %ld status\n",
    			boardStatus);
    }
#endif
    initialisePowerMeasurement(i2c2Handle);
#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    DrvTempSensorInitialise(&tempParam);
    (void)DrvTempSensorSetMode(TSENS_CSS, TSENS_ONE_SHOT_MODE, TSENS_SAMPLE_TEMP);
    (void)DrvTempSensorSetMode(TSENS_MSS, TSENS_ONE_SHOT_MODE, TSENS_SAMPLE_TEMP);
    (void)DrvTempSensorSetMode(TSENS_UPA0, TSENS_ONE_SHOT_MODE, TSENS_SAMPLE_TEMP);
    (void)DrvTempSensorSetMode(TSENS_UPA1, TSENS_ONE_SHOT_MODE, TSENS_SAMPLE_TEMP);
#endif
}

int main(void)
{
    Profile tests[runs];

    initialiseHardware();

    printf("LowDynamicPower benchmark\n");
    printf("============================================================\n");
    printf("Resolution: %dx%d\n", TEST_FRAME_WIDTH, TEST_FRAME_HEIGHT);
    printf("Processing: conv7x7 + conv7x7 + conv7x7 on u8\n");
    printf("Frequency: %dMhz\n\n", DrvCprGetClockFreqKhz(SYS_CLK, NULL) / 1000);

    memset(tests, 0, sizeof(tests));

    unitTestInit();

    runFull(tests);
    checkResults(tests, runs);
    showReport(tests, runs);

    unitTestFinalReport();

    return 0;
}
