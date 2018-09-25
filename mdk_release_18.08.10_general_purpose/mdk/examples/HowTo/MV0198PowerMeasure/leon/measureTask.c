///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Simple Example showing MV0198 Power Measure usage.
///            It can be used for testing purposes, too
///

// 1: Includes
// ----------------------------------------------------------------------------

#include "app_config.h"
#include "brdMv0198.h"
#include <stdlib.h>
#include <stdio.h>
#include <DrvLeon.h>
// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------

// Diagnostic print utiltity
//#define DRV_DEBUG
#ifdef DRV_DEBUG
#   define DPRINTF(...) printf(__VA_ARGS__)
#else
#   define DPRINTF(...)
#endif

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------


// i2c2 bus handler
extern I2CM_Device *lrt_i2c2Bus;

// RTEMS Task Measure
rtems_task Measure_task(rtems_task_argument unused)
{
    (void) unused; // "use" the variables to hush the compiler warning.

    int retVal;
    tyBrd198Handle powerMonHandle;
    tyAdcResultAllRails res;
    //printf("\nstart measure\n");

    retVal = Brd198Init(&powerMonHandle, lrt_i2c2Bus,NULL);
    if(retVal != 0)
    {
         printf("Board 198 init error\n");

    }
    while(1)
    {
        if(DrvLeonRTHasStopped())
        {
            printf("Measurement was finished \n");
            exit(0);
        }
        Brd198SampleAllRails(&powerMonHandle, &res);
    }

rtems_task_delete(RTEMS_SELF);
}

