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
#include <DrvSvu.h>
#include "Shave.h"
#include "app_config.h"
#include <UnitTestApi.h>

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define SHAVE_USED 0

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern unsigned int cppshave0_main;
extern char cppshave0_shaveStr[];

// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

int main(void)
{
    s32 sc;

    sc = initClocksAndMemory();
    if(sc)
        return sc;
        
    unitTestInit ();
    printf("\n\nHello from LEON!\n\n\n");
    Shave *shave;
    Shave *test;

    //This is an example usecase of c++ and shave
    shave = Shave::gethandle(SHAVE_USED);
    test = Shave::gethandle(SHAVE_USED);

    if (((u32)shave)!=((u32)test)){
      printf("Error in singleton design pattern!\n");
    }

    shave->init();
    shave->start((u32)&cppshave0_main);
    shave->wait();
    shave->deinit();
    shave->start((u32)&cppshave0_main);
    shave->wait();
    shave->deinit();
    shave->start((u32)&cppshave0_main);
    shave->wait();
    shave->deinit();

    printf("%s", cppshave0_shaveStr);

    Shave::destroy(SHAVE_USED);
    unitTestFinalReport ();

    return 0;
}
