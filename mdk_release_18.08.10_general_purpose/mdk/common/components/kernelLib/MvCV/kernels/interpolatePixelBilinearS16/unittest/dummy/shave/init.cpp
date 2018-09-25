///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     interpolatePixelBilinearS16 kernel function call for unitary test
///

#include "interpolatePixelBilinearS16.h"
#include "svuCommonShave.h"
#include <stdio.h>
#include "mv_types.h"

short line1[320] __attribute__ ((aligned (16)));
short line2[320] __attribute__ ((aligned (16)));
float x __attribute__ ((aligned (16)));
float y __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
float __attribute__((section(".kept.data"))) output __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

int main( void )
{

#ifdef UNIT_TEST_USE_C_VERSION
    output = mvcvInterpolatePixelBilinearS16(line1, line2, x, y);
#else
    output = mvcvInterpolatePixelBilinearS16_asm(line1, line2, x, y);
#endif

    SHAVE_HALT; 
    return 0;
}
