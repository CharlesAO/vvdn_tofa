///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file 
///


// 1: Includes
// ----------------------------------------------------------------------------

#include <DrvLeonL2C.h>
#include <DrvLeon.h>
#include <DrvShaveL2Cache.h>
#include <stdio.h>
#include <string.h>
#include <swcLeonUtils.h>
#include <DrvSvu.h>
#include <swcShaveLoader.h>
#include "Shave.h"

extern u32 SimpleSipp0_shave_start;




// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define SHAVE_USED    0

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------


// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
char __attribute__((section(".cmx.data"))) msgbuff[128];
//extern "C" {

Shave *shave;
int  main(void)
{
    strncpy(msgbuff, "LeonRT Started.\n", 128);

    //This should be singleton design pattern, but dynamic allocation is not working?
    shave = Shave::gethandle(SHAVE_USED);
    shave->reset();
    shave->setdefaultstack();
    shave->start((u32)&SimpleSipp0_shave_start);
    shave->wait();
    Shave::destroy(SHAVE_USED);

    DrvLeonRTSignalStop();

    return 0;
}
//}
