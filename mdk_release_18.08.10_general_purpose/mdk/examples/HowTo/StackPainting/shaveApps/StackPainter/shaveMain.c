///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

#include <mv_types.h>
#include <svuCommonShave.h>

u32 __attribute__((section(".cmx.data"))) *stackTop, *stackEnd;

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

int Entry(register u32 *stackStart, register u32 *testPattern, register u32 fillSize)
{
    stackTop = stackStart;
    stackEnd = stackStart - fillSize;

    while (stackEnd != stackTop)
    {
        *stackTop-- = *testPattern++;
    }

    SHAVE_HALT;
    return 0;
}
