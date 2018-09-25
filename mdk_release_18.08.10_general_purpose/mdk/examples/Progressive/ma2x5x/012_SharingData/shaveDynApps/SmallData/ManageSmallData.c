///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Shave hello world source code
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <swcWhoAmI.h>
#include <svuCommonShave.h>

// 2:  Source Specific #defines and types (typedef, enum, struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
char helloprintfString[]="Hello from local CMX from shave ";
char helloPrintfDelimiter[50]="-------------------------------\n";

#define SMALL_DATA_LENGTH 30
#define TEST_VALUE 0xCAFE;

unsigned int bufferedSharedWithLeon[SMALL_DATA_LENGTH];
extern volatile u32 bufferSharedBetweenLeonAndShave[SMALL_DATA_LENGTH];
// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

void testSmallDataSharedWithLeon(void)
{
    for(int idx = 0; idx < SMALL_DATA_LENGTH; idx++)
        bufferSharedBetweenLeonAndShave[idx] *= 2;
}

void TestSmallDatSharedBetweenShaves(u32* buffSharedBetweenShaves, u32 shaveNumber)
{
    int idx;
    if(shaveNumber == 0)
    {
        for(idx = 0; idx< SMALL_DATA_LENGTH/2; idx++)
            buffSharedBetweenShaves[idx] = idx;
    }
    else
    {
        for(idx = SMALL_DATA_LENGTH/2; idx < SMALL_DATA_LENGTH; idx++){
            buffSharedBetweenShaves[idx] =  idx;
        }
    }
}
void Entry(u32* buffSharedBetweenShaves)
{
    unsigned int shaveNumber;

    shaveNumber=(swcWhoAmI()-PROCESS_SHAVE0);

    // Test only on one shave is enough
    if(shaveNumber == 0)
        testSmallDataSharedWithLeon();

    TestSmallDatSharedBetweenShaves(buffSharedBetweenShaves, shaveNumber);
 }
