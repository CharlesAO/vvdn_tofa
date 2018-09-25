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
#include "mv_types.h"
#include "app_config.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
//#include <unistd.h>

#include "blis.h"
#include "Defines.h"
#include "Helpers.h"

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
#ifdef LEON_MALLOC_ISSUE
/* actual pool sizes (bytes)
  for mc x kc blocks of A  136320
  for kc x nc panels of B  8458368
  for mc x nc panels of C  128
                         ----------
                          8594816
  The rest of DDR memory is used for leon_heap => */
#if (defined MYRIAD1)
 #define LEON_HEAP  ( 54*1024*1024)
#elif (defined MYRIAD2)
 #define LEON_HEAP  (118*1024*1024)
#endif
unsigned char DDR_LEON_HEAP ALIGNED(16) leon_heap[LEON_HEAP];
#endif

// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
extern void test_libblis();

// 6: Functions Implementation
// ----------------------------------------------------------------------------
int main(void)
{
    initClocksAndMemory();
#ifdef SHAVE_BLIS
    printf("\nStart using %d SHAVEs!\n", numberShaves);
#else
    printf("\nStart using LEON only!\n");
#endif
#ifdef LEON_MALLOC_ISSUE
    bli_init_leon(leon_heap, LEON_HEAP);
#endif

    test_libblis();

    return 0;
}

