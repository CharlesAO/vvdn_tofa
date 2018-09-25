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
#include "app_config.h"
#include <swcLeonUtils.h>
#include <DrvLeonL2C.h>
#include <DrvLeon.h>

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

int main(void)
{

    printf("\n\nHello from LeonRT!\n\n\n");
    DrvLeonRTSignalStop();

    return 0;
}
