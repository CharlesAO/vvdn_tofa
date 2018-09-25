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

// Local defines used for simple test
#define SHAVE_NUMBER 0
#define CRC_USED_STEP0 0xc3ec8483
#define CRC_USED_STEP1 0x6aa3bcd7
#define CRC_USED_STEP2 0x6aa3bcd7
#define CRC_USED_STEP3 0x16ccaa6a
#define CRC_USED_STEP4 0x16ccaa6a

// Shave entrypoints
extern u32 AsmOptimisation0_AsmOptimisationInit;
extern u32 AsmOptimisation0_AsmOptimisationRun;

static u32 FnAsmOptimisationInit 	= (u32)&AsmOptimisation0_AsmOptimisationInit;
static u32 FnAsmOptimisationRun 	= (u32)&AsmOptimisation0_AsmOptimisationRun;


// Initialize buffers 
void initAlgo(swcShaveUnit_t svu, u32 width)
{    
    swcResetShave(svu);
    swcSetAbsoluteDefaultStack(svu);
    swcStartShaveCC(svu, FnAsmOptimisationInit, "i", width);
    swcWaitShave(svu);
    DrvShaveL2CachePartitionFlushAndInvalidate(svu);
}

// Run convolution1x7
void runAlgo(swcShaveUnit_t svu, u8* inputframe, u8* outputframe, u32 width, u32 height, u32 step_used)
{    
    // Declare performance monitoring structure
    performanceStruct perfStr;
	
    // Initialize the profiling structure
    swcShaveProfInit(&perfStr);
    
    swcResetShave(svu);
    swcSetAbsoluteDefaultStack(svu);

    // Start profiling
    swcShaveProfStartGatheringFields(svu, PERF_INSTRUCT_COUNT);
    swcStartShaveCC(svu, FnAsmOptimisationRun, "iiiii", inputframe, outputframe, width, height, step_used);
    swcWaitShave(svu);
    // Stop profiling
    swcShaveProfStopFieldsGathering(svu, PERF_INSTRUCT_COUNT);

    // Request Profiling report
    swcShaveProfPrint(svu, &perfStr);

    DrvShaveL2CachePartitionFlushAndInvalidate(svu);
}

// Main function
int main()
{
	// Init board
        initClocksAndMemory();
	DrvShaveL2CachePartitionFlushAndInvalidate(SHAVE_NUMBER);

	printf("AsmOptimisation algorithm.\n");

	// Image dimensions
	const u32 width = MAX_WIDTH;
	const u32 height = MAX_HEIGHT;

        loadMemFromFileSimple("input/DunLoghaire_320x240.bin", width * height, (void*)inputframe);
        // Unit test based on Crc
	unitTestInit();

	// Init and run 
        initAlgo(SHAVE_NUMBER, width);
	
	printf("\nStep 0:  No optimisation - C level\n");
        runAlgo(SHAVE_NUMBER, (u8*)inputframe, (u8*)outputframe, width, height, 0);
        u32 crcUniTest0 = swcCalcCrc32(outputframe, sizeof(unsigned char) * width * height, le_pointer);    
	
	printf("\nStep 1:  First version of code written in Asm\n");
	runAlgo(SHAVE_NUMBER, (u8*)inputframe, (u8*)outputframe, width, height, 1);
	u32 crcUniTest1 = swcCalcCrc32(outputframe, sizeof(unsigned char) * width * height, le_pointer);    
    
	printf("\nStep 2:  Remove unnecesary \"nop's\" and start putting instructions in parallel - Asm\n");
	runAlgo(SHAVE_NUMBER, (u8*)inputframe, (u8*)outputframe, width, height, 2);
	u32 crcUniTest2 = swcCalcCrc32(outputframe, sizeof(unsigned char) * width * height, le_pointer);    
	
	printf("\nStep 3:  loop parallelization - change instructions in order to have less instructions in loop - Asm\n");
        runAlgo(SHAVE_NUMBER, (u8*)inputframe, (u8*)outputframe, width, height, 3);
        u32 crcUniTest3 = swcCalcCrc32(outputframe, sizeof(unsigned char) * width * height, le_pointer);    

        printf("\nStep 4:  loop parallelization - make the loop to be made only on the principal unit used\n");
        runAlgo(SHAVE_NUMBER, (u8*)inputframe, (u8*)outputframe, width, height, 4);
        u32 crcUniTest4 = swcCalcCrc32(outputframe, sizeof(unsigned char) * width * height, le_pointer);    

	unitTestAssert((crcUniTest0 == CRC_USED_STEP0) && (crcUniTest1 == CRC_USED_STEP1) && (crcUniTest2 == CRC_USED_STEP2) && (crcUniTest3 == CRC_USED_STEP3)
                        && (crcUniTest4 == CRC_USED_STEP4));
	unitTestFinalReport();
    
    return 0;
}
