///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Simple Example showing mv0198App Power Measure usage.
///            It can be used for testing purposes, too
///


// 1: Includes
#include <swcShaveLoader.h>
#include "app_config.h"
#include "DrvTimer.h"
#include "DrvSvu.h"
#ifdef MV0212
#include "MV0212.h"
#include "brdGpioCfgs/brdMv0182R5GpioDefaults.h"
#elif defined(MV0182)
#include <Board182Api.h>
#endif
#include "DrvCDCEL.h"
#include <DrvLeon.h>
#include <dbgTracerApi.h>

// Drivers
#include <DrvShaveL2Cache.h>

// Components
#include <VcsHooksApi.h>

// Standard libraries
#include <stdio.h>
#include <stdlib.h>
// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define DDR_BUF __attribute__((section(".ddr.data")))

#define FRAME_WIDTH   1280
#define FRAME_HEIGHT   720

#define SHAVE_0    0
#define SHAVE_1    1
#define SHAVE_2    2
#define SHAVE_3    3
#define SHAVE_4    4
#define SHAVE_5    5
#define SHAVE_6    6
#define SHAVE_7    7
#define SHAVE_8    8
#define SHAVE_9    9

#define SHAVE_0_CLK      0x001
#define SHAVES_1_3_CLK   0x00E
#define	SHAVES_4_9_CLK   0x3F0

#define PADDING_SIZE		2
DDR_BUF u8   	outputFrameLocation[FRAME_WIDTH * FRAME_HEIGHT];


#ifdef MV0212
#define NUM_I2C_DEVS 3
#endif
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
I2CM_Device			*i2c2Bus;

extern u32  mv0198App0_shaveRun[];
extern u32  mv0198App1_shaveRun[];
extern u32  mv0198App2_shaveRun[];
extern u32  mv0198App3_shaveRun[];
extern u32  mv0198App4_shaveRun[];
extern u32  mv0198App5_shaveRun[];
extern u32  mv0198App6_shaveRun[];
extern u32  mv0198App7_shaveRun[];
extern u32  mv0198App8_shaveRun[];
extern u32  mv0198App9_shaveRun[];

u32* shavesEntryRun[10] =
{
    mv0198App0_shaveRun,
    mv0198App1_shaveRun,
    mv0198App2_shaveRun,
    mv0198App3_shaveRun,
    mv0198App4_shaveRun,
    mv0198App5_shaveRun,
    mv0198App6_shaveRun,
    mv0198App7_shaveRun,
    mv0198App8_shaveRun,
mv0198App9_shaveRun,
};

extern u8 frameLocation;

// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

static void processImage(u8 *inputFrame, u8 *outputFrame)
{
    int shavePartHeight = FRAME_HEIGHT / SHAVES_USED;
    int i;

    DrvTimerSleepMs(100);

    // Run one shave
    DrvCprTurnOnShaveMask(SHAVE_0_CLK);

    swcResetShave(SHAVE_0);
    swcSetAbsoluteDefaultStack(SHAVE_0);

    swcStartShaveCC(SHAVE_0, (u32) shavesEntryRun[SHAVE_0], "iiii",	&inputFrame[0 * shavePartHeight * FRAME_WIDTH],
            &outputFrame[0 * shavePartHeight * FRAME_WIDTH], FRAME_WIDTH, shavePartHeight);
    swcWaitShave(SHAVE_0);

    DrvShaveL2CachePartitionFlushAndInvalidate(PARTITION_0);
    DrvSvutStop(SHAVE_0);
    DrvCprTurnOffShaveMask(SHAVE_0_CLK);


    DrvTimerSleepMs(100);

    // Run three shaves
    DrvCprTurnOnShaveMask(SHAVES_1_3_CLK);

    for (i = SHAVE_1; i <= SHAVE_3; i++)
    {
        swcResetShave(i);
        swcSetAbsoluteDefaultStack(i);
    }

    swcStartShaveCC(SHAVE_1, (u32) shavesEntryRun[ SHAVE_1], "iiii", &inputFrame[1 * shavePartHeight * FRAME_WIDTH],
            &outputFrame[1 * shavePartHeight * FRAME_WIDTH], FRAME_WIDTH, shavePartHeight);
    swcStartShaveCC(SHAVE_2, (u32) shavesEntryRun[ SHAVE_2], "iiii", &inputFrame[2 * shavePartHeight * FRAME_WIDTH],
            &outputFrame[2 * shavePartHeight * FRAME_WIDTH], FRAME_WIDTH, shavePartHeight);
    swcStartShaveCC(SHAVE_3, (u32) shavesEntryRun[ SHAVE_3], "iiii", &inputFrame[3 * shavePartHeight * FRAME_WIDTH],
            &outputFrame[3 * shavePartHeight * FRAME_WIDTH], FRAME_WIDTH, shavePartHeight);

    swcWaitShave(SHAVE_1);
    swcWaitShave(SHAVE_2);
    swcWaitShave(SHAVE_3);

    DrvShaveL2CachePartitionFlushAndInvalidate(PARTITION_0);
    for (i = SHAVE_1; i <= SHAVE_3; i++)
    {
        DrvSvutStop(i);
    }

    DrvCprTurnOffShaveMask(SHAVES_1_3_CLK);
    DrvTimerSleepMs(100);

    // Run six shaves
    DrvCprTurnOnShaveMask(SHAVES_4_9_CLK);
    for (i = SHAVE_4; i <= SHAVE_9; i++)
    {
        swcResetShave(i);
        swcSetAbsoluteDefaultStack(i);
    }

    swcStartShaveCC(SHAVE_4, (u32) shavesEntryRun[ SHAVE_4], "iiii", &inputFrame[4 * shavePartHeight * FRAME_WIDTH],
            &outputFrame[4 * shavePartHeight * FRAME_WIDTH], FRAME_WIDTH, shavePartHeight);
    swcStartShaveCC(SHAVE_5, (u32) shavesEntryRun[ SHAVE_5], "iiii", &inputFrame[5 * shavePartHeight * FRAME_WIDTH],
            &outputFrame[5 * shavePartHeight * FRAME_WIDTH], FRAME_WIDTH, shavePartHeight);
    swcStartShaveCC(SHAVE_6, (u32) shavesEntryRun[ SHAVE_6], "iiii", &inputFrame[6 * shavePartHeight * FRAME_WIDTH],
            &outputFrame[6 * shavePartHeight * FRAME_WIDTH], FRAME_WIDTH, shavePartHeight);
    swcStartShaveCC(SHAVE_7, (u32) shavesEntryRun[ SHAVE_7], "iiii", &inputFrame[7 * shavePartHeight * FRAME_WIDTH],
            &outputFrame[7 * shavePartHeight * FRAME_WIDTH], FRAME_WIDTH, shavePartHeight);
    swcStartShaveCC(SHAVE_8, (u32) shavesEntryRun[ SHAVE_8], "iiii", &inputFrame[8 * shavePartHeight * FRAME_WIDTH],
            &outputFrame[8 * shavePartHeight * FRAME_WIDTH], FRAME_WIDTH, shavePartHeight);
    swcStartShaveCC(SHAVE_9, (u32) shavesEntryRun[ SHAVE_9], "iiii", &inputFrame[9 * shavePartHeight * FRAME_WIDTH],
            &outputFrame[9 * shavePartHeight * FRAME_WIDTH], FRAME_WIDTH, shavePartHeight - PADDING_SIZE);

    //last shave will process with two lines less
    //(first line and last line are padded with 0x00)

    swcWaitShave(SHAVE_4);
    swcWaitShave(SHAVE_5);
    swcWaitShave(SHAVE_6);
    swcWaitShave(SHAVE_7);
    swcWaitShave(SHAVE_8);
    swcWaitShave(SHAVE_9);

    DrvShaveL2CachePartitionFlushAndInvalidate(PARTITION_0);
    for (i = SHAVE_4; i <= SHAVE_9; i++) {
        DrvSvutStop(i);
    }

    DrvCprTurnOffShaveMask(SHAVES_4_9_CLK);
    DrvTimerSleepMs(100);
}
static void addPadding(u8* frame)
{
    s32 i;
    //fist line
    for(i = 0; i < FRAME_WIDTH; i++)
    {
        *(frame+i) = 0x00;
    }
    //last line
    for(i = 0; i < FRAME_WIDTH; i++)
    {
        *(frame+i+ FRAME_WIDTH* ( FRAME_HEIGHT-2) )= 0X00;
}
}
int main(void)
{
   	u8 *input;
   	s32 boardStatus;

   	initClocksAndMemory();
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
        printf("Error: board configuration read failed with %ld status\n",rc);
        return rc;
    }

    i2c2Bus	= info[2].handler;
#elif defined(MV0182)
    boardStatus = BoardInitialise(EXT_PLL_CFG_148_24_24MHZ);
    i2c2Bus	= gAppDevHndls.i2c2Handle;
    if (boardStatus != B_SUCCESS)
    {
    	printf("Error: board initialization failed with %ld status\n",
    			boardStatus);
    	return -1;
    }
#endif

  	DrvCprTurnOffShaveMask(0xFFF);

    //now i2c can be used from leon os
	DrvLeonRTSignalBootCompleted(); 	//the measurement will start

 	input = (u8*)((u32)&frameLocation );

 	processImage(input,&outputFrameLocation[0+FRAME_WIDTH]);	//first line will be padded

	DrvLeonRTSignalStop();

    addPadding(outputFrameLocation);

    saveMemoryToFile((u32)outputFrameLocation, FRAME_WIDTH * FRAME_HEIGHT, "output/output_image_1280x720_u8.raw");
    printf("Finish to download image\n");
    exit(0);
}
