///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///
// 1: Includes
// ----------------------------------------------------------------------------
// System settings
#include "app_config.h"

// Standard libraries
#include <stdio.h>
#include <string.h>

// Drivers
#include <DrvShaveL2Cache.h>

// Common code
#include <swcShaveLoader.h>
#include <swcTestUtils.h>
#include <swcCrc.h>

// Components
#include <UnitTestApi.h>
#include <VcsHooksApi.h>

// Local code
#include <Buffers.h>

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------

#define CRC_USED         0x51799046

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// Shave entrypoints
extern u32 SimpleCrossCompilableCVPipe0_SimpleCrossCompilableCVPipeInit;
extern u32 SimpleCrossCompilableCVPipe0_SimpleCrossCompilableCVPipeRun;

// 4: Static Local Data
// ----------------------------------------------------------------------------
static u32 FnSimpleCrossCompilableCVPipeInit 	= (u32)&SimpleCrossCompilableCVPipe0_SimpleCrossCompilableCVPipeInit;
static u32 FnSimpleCrossCompilableCVPipeRun 	= (u32)&SimpleCrossCompilableCVPipe0_SimpleCrossCompilableCVPipeRun;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
// Initialize buffers for DOG algorithm
void initAlgo(swcShaveUnit_t svu, u32 width)
{
    swcResetShave(svu);
    swcSetAbsoluteDefaultStack(svu);
    swcStartShaveCC(svu, FnSimpleCrossCompilableCVPipeInit, "i", width);
    swcWaitShave(svu);
    DrvShaveL2CachePartitionFlushAndInvalidate(PARTITION_0);
}

// Run DOG algorithm
void runAlgo(swcShaveUnit_t svu, u8* inputframe, u8* outputframe, u32 width, u32 height)
{

	// Declare performance monitoring structure
    performanceStruct perfStr;

	// Initialize the profiling structure
    swcShaveProfInit(&perfStr);

    swcResetShave(svu);
    swcSetAbsoluteDefaultStack(svu);

    // Start profiling
    swcShaveProfStartGatheringFields(svu, PERF_INSTRUCT_COUNT);
    swcStartShaveCC(svu, FnSimpleCrossCompilableCVPipeRun, "iiii", inputframe, outputframe, width, height);
    swcWaitShave(svu);
    // Stop profiling
	swcShaveProfStopFieldsGathering(svu, PERF_INSTRUCT_COUNT);

	// Request Profiling report
    swcShaveProfPrint(svu, &perfStr);

    DrvShaveL2CachePartitionFlushAndInvalidate(PARTITION_0);
}

// Main function
int main()
{
  s32 sc;
  // Unit test based on Crc
  unitTestInit();

  // Init board
  sc = initClocksAndMemory();
  if(sc) {
      return sc;
  }
	DrvShaveL2CachePartitionFlushAndInvalidate(PARTITION_0);

	printf("SimpleCrossCompilableCVPipe algorithm.\n");

	// Image dimensions
	const u32 width = MAX_WIDTH;
	const u32 height = MAX_HEIGHT;

    loadMemFromFileSimple("../input/DunLoghaire_320x240.yuv", width * height, (void*)inputframe);

	// Init and run DOG computation
    initAlgo(SHAVE_NUMBER, width);
    runAlgo(SHAVE_NUMBER, (u8*)inputframe, (u8*)outputframe, width, height);

    u32 crcUniTest = swcCalcCrc32(outputframe, sizeof(unsigned char) * width * height, le_pointer);

	unitTestAssert(crcUniTest == CRC_USED);

	unitTestFinalReport();

    return 0;
}
