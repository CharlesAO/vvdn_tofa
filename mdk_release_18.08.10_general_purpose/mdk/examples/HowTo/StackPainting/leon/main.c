///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Main leon file
///

// 1: Includes
// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <swcShaveLoader.h>
#include <DrvShaveL2Cache.h>
#include <DrvTimer.h>
#include <DrvSvu.h>
#include <DrvLeon.h>
#include <VcsHooksApi.h>
#include <UnitTestApi.h>
#include <swcCrc.h>
#include <DrvLeonL2C.h>
#include "app_config.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

#define SHAVE_MAX_NO            3
#define TEST_CANARY_VALUE       0xBEEFC0FE
#define TEST_PATTERN1           0xA1FA1FA1
#define TEST_PATTERN2           0x0FA110FF
#define TEST_PATTERN3           0x10ADAB13
#define MAX_TEST_STACK_SIZE     512
#define MAX_PATTERN_SIZE        (MAX_TEST_STACK_SIZE / 2)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

extern u32 StackPainter0_Entry;
extern u32 StackPainter1_Entry;
extern u32 StackPainter2_Entry;


u32 startShave[SHAVE_MAX_NO] = {
   (u32) &StackPainter0_Entry,
   (u32) &StackPainter1_Entry,
   (u32) &StackPainter2_Entry
};

// 4: Static Local Data
// ----------------------------------------------------------------------------

static const u32 testPattern[] = {
    [                    0 ...    MAX_PATTERN_SIZE - 1] = TEST_PATTERN1, 
    [    MAX_PATTERN_SIZE ... 2 * MAX_PATTERN_SIZE - 1] = TEST_PATTERN2, 
    [2 * MAX_PATTERN_SIZE ... 3 * MAX_PATTERN_SIZE - 1] = TEST_PATTERN3
};


// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------
#ifdef PRINT_STACK
void dumpMemory(u32 *dataBuffer, int bufferSize)
{
    int i, j;

    printf("\n            ");

    for (i = 0; i < 16 && i < bufferSize; i += 4)
    {
        printf("%8x ", i);
    }

    for (i = 0, j = 0; i < bufferSize; i++)
    {
        if (0 == i % 4)
        {
            printf("\n0x%08X: ", (unsigned int) &dataBuffer[j]);
            
            j += 4;
        }
        
        printf("%08lx ", dataBuffer[i]);
    }

    printf("\n");
}
#endif

int main(void)
{
    u32 *stackStart;
    u32 *stackEnd;
    u32 shaveStackSize;
    u32 shaveStackLength;
    u32 fillStackSize;
    u32 fillStackLength;
    s32 sc, shv;

    s32 testProportion[] = {29, 71, 49};

    unitTestInit();
    sc = initClocksAndMemory();

    if (sc)
      return sc;

    for (shv = 0; shv < SHAVE_MAX_NO; shv++)
    {
        printf("TEST ON SHAVE %lu\n", shv);
        shaveStackSize   = MAX_TEST_STACK_SIZE * testProportion[shv] / 100;
        //Make shaveStackSize multiple of sizeof(u32)
        shaveStackSize   = (shaveStackSize & ~7) + sizeof(u32);
        shaveStackLength = shaveStackSize / sizeof(u32);
    
        fillStackLength = shaveStackLength * testProportion[shv] / 100;
        //Make fillStackLength greater than zero
        fillStackLength = fillStackLength ? fillStackLength : 1;
        fillStackSize   = fillStackLength * sizeof(u32);

        swcResetShave(shv);
        swcSetAbsoluteDefaultStack(shv);
        swcStateConsideredShaveStackSize(shv, shaveStackSize);

        // Get default stack pointer (register I19) in accordance with moviCompile.pdf (point 9.3 Stack Pointer, page 37)
        stackStart = (u32 *) DrvSvutIrfRead(shv, 19) - sizeof(u32);
        stackEnd   = stackStart - shaveStackLength + 1;

        // Fill stack area with canary value
        swcStackPainter(shv,  TEST_CANARY_VALUE);

        printf("Unused stack before processing:%lu W\n", swcGetUnusedShaveFreeStack(shv, TEST_CANARY_VALUE));
        //Testing stack painter with test pattern
        swcStartShaveCC(shv, startShave[shv], "iii", stackStart, &testPattern[shv * MAX_PATTERN_SIZE], fillStackLength);
        swcWaitShave(shv);
        swcLeonFlushDcache();

        unitTestMemCompare( (uint32_t*) ((uint32_t)stackStart - fillStackSize+4), &testPattern[shv * MAX_PATTERN_SIZE], fillStackSize);
        
        #ifdef PRINT_STACK
            dumpMemory(stackEnd, shaveStackLength);
        #else
            char testDumpFile[] = "shvX_stack.bin";
            testDumpFile[3] = '0' + shv;

            printf("\nSaving stack SHAVE%lu into file %s\n", shv, testDumpFile);
            saveMemoryToFile((u32) stackEnd, shaveStackSize, testDumpFile);
            printf("Unused stack after processing:%lu W\n\n\n", swcGetUnusedShaveFreeStack(shv, TEST_CANARY_VALUE));
        #endif
    }
    
    unitTestFinalReport();
    return 0;
}
