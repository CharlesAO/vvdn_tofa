///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     fastAtan2Positive kernel function call for unitary test
///

#include "fastAtan2Positive.h"
#include "svuCommonShave.h"
#include <stdio.h>
#include "mv_types.h"
#define ALIGN32 __attribute__((aligned(4)))
ALIGN32 half x;
ALIGN32 half y;

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) result_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) result __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) result_u32postPad[4] __attribute__ ((aligned (16)));


int main( void )
{
    #ifdef UNIT_TEST_USE_C_VERSION
    result = mvcvFastAtan2Positive(y,x);
    #else
    result = mvcvFastAtan2Positive_asm(y,x);
    #endif
    SHAVE_HALT; 
    return 0;
}
