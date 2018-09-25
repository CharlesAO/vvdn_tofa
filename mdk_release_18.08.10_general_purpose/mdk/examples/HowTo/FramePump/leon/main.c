///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///


// 1: Includes
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include "commonTests.h"


void POSIX_Init(void *args)
{
    (void)args;

    runApp();
}
