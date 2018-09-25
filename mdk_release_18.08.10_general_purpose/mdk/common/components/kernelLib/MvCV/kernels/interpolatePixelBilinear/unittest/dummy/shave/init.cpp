///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     fastAtan2Positive kernel function call for unitary test
///

#include "interpolatePixelBilinear.h"
#include "svuCommonShave.h"
#include <stdio.h>
#include "mv_types.h"
#define ALIGN32 __attribute__((aligned(4)))
ALIGN32 half line1[320] __attribute__ ((aligned (16)));
ALIGN32 half line2[320] __attribute__ ((aligned (16)));
ALIGN32 fp32 x __attribute__ ((aligned (16)));
ALIGN32 fp32 y __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) output __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));


int main( void )
{
    #ifdef UNIT_TEST_USE_C_VERSION
    output = mvcvInterpolatePixelBilinear(line1, line2, x, y);
    #else
    output = mvcvInterpolatePixelBilinear_asm(line1, line2, x, y);
    #endif
    SHAVE_HALT; 
    return 0;
}
