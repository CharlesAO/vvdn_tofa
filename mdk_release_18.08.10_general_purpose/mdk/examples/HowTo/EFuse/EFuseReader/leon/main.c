///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Basic eFuse reader example
///
///
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <DrvCpr.h>
#include <DrvEfuse.h>
#include "app_config.h"
#include <UnitTestApi.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// This defines an example 'serial number' field in the customer-use EFuse area
#define SERIAL_SIZEOF                   8
#define EFUSE_SERIAL_START              (EFUSE_CUSTOMER_USE_START + 0)
#define EFUSE_SERIAL_END                (EFUSE_CUSTOMER_USE_START + 0 + (SERIAL_SIZEOF * 8))

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
    efuseSet set;
    int i;
    u8 serial[SERIAL_SIZEOF];

    unitTestInit ();
    initClocksAndMemory();

    printf("eFuse reader example\n\n");

    // Read the entire set of EFuses.
    if((i = DrvEfuseReadEFuses(&set, APPLY_FIX, NORMAL_MODE)) != 0) {
        printf("DrvEfuseReadEFuses - error %d\n", i);
    } else {
        printf("EFuses read successfully\n\n");

        // Read and display fields from the efuseSet
        printf("IFSEL:  %lu\n",
            DrvEfuseGetValue32(&set, EFUSE_BOOTIFSEL_START, EFUSE_BOOTIFSEL_END));
        printf("IFCFG:  0x%016llx\n",
            DrvEfuseGetValue64(&set, EFUSE_BOOTIFCFG_START, EFUSE_BOOTIFCFG_END));

        // for the serial number field, we will read the data into a byte array
        DrvEfuseGetValuePtr(&set, EFUSE_SERIAL_START, EFUSE_SERIAL_END, serial);
        printf("Serial: ");
        for(i=0; i<SERIAL_SIZEOF; i++)
            printf("%s%02x", i==0?"":"-", serial[i]);
        printf("\n\n");

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing" // this is for the last printf parameter below
        printf("Dumping entire EFuse contents\n");
        for(i=0; i<(EFUSE_NUM_BITS/8); i+=4) {
            printf("  [%4u:%4u] : 0x%08lx\n",
                (i * 8) + 31,
                i * 8,
                *(u32*)&set.values[i]);
        }
    }

#pragma GCC diagnostic pop
    unitTestFinalReport ();

    return 0;
}
