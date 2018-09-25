///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file 
///


// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include "DrvLeon.h"
#include "swcShaveLoader.h"
#include "DrvCpr.h"
#include <OsCross.h>
#include <dbgTracerApi.h>

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define DDR_BUF __attribute__((section(".ddr.data")))
#define TEST_FRAME_WIDTH    1280
#define TEST_FRAME_HEIGHT   720
#define FRAME_SIZE          (TEST_FRAME_WIDTH * TEST_FRAME_HEIGHT*3/2)

#define SHAVES_USED_FROM_LRT 6
#define KERNEL_SIZE 3

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern u8 inputFrame;

// SHAVE
extern u32 TraceProfiller0_ApplicationStart;
extern u32 TraceProfiller1_ApplicationStart;
extern u32 TraceProfiller2_ApplicationStart;
extern u32 TraceProfiller3_ApplicationStart;
extern u32 TraceProfiller4_ApplicationStart;
extern u32 TraceProfiller5_ApplicationStart;

u32* TraceProfiller_ApplicationStart[SHAVES_USED_FROM_LRT]= {
	&TraceProfiller0_ApplicationStart,
	&TraceProfiller1_ApplicationStart,
	&TraceProfiller2_ApplicationStart,
	&TraceProfiller3_ApplicationStart,
	&TraceProfiller4_ApplicationStart,
	&TraceProfiller5_ApplicationStart,
};

DDR_BUF unsigned char out_image[FRAME_SIZE];

// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------
int main(void)
{

    LOG_INFO("LeonRT starts\n");
    osCrossInit();

	u32 shaveIndex;

	for (shaveIndex = 0; shaveIndex < 6; shaveIndex++)
	{
		LOG_INFO("Starting shave %ld ...", shaveIndex);

		swcResetShave(shaveIndex);
		swcSetAbsoluteDefaultStack(shaveIndex);
		swcStartShaveCC(shaveIndex, (u32)TraceProfiller_ApplicationStart[shaveIndex], "iiiiiii", (u32)out_image,(u32)&inputFrame, TEST_FRAME_WIDTH, TEST_FRAME_HEIGHT, SHAVES_USED_FROM_LRT, shaveIndex, KERNEL_SIZE);
		swcWaitShave(shaveIndex);
	}

	LOG_INFO("LeonRT finished\n");
	DrvLeonRTSignalStop();

    return 0;
} 
