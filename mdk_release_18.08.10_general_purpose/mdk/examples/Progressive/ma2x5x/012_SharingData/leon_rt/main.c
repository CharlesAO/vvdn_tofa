///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Main Leon RT file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include "app_config.h"
#include <mv_types.h>
#include "swcLeonUtils.h"
#include "DrvLeon.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
u32 DDR_DIRECT_DATA losToLrt;

// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
int main(void)
{

    while(losToLrt != VALUE_SET_IN_LOS)
        ;
    losToLrt = VALUE_SET_IN_LRT;
    DrvLeonRTSignalStop();

}
