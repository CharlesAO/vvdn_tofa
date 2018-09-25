///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    maxPool3x3hh kernel function call for unitary test
///

#include "maxPool3x3hh.h"
#include "svuCommonShave.h"
#include "mv_types.h"
#include "stdio.h"


#define PADDING 8
#define TEST_FRAME_WIDTH 1920 + PADDING
#define TEST_FRAME_HEIGHT 3

half input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH]  __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) output[1][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width  __attribute__ ((aligned (16)));
half* inLine[TEST_FRAME_HEIGHT];
half* outLine[TEST_FRAME_HEIGHT];
half** inLines;
half** outLines;

int main( void )
{
    int i;
    for(i = 0; i < TEST_FRAME_HEIGHT; i++)
    {
        inLine[i] = (half*)input[i];
        outLine[i] = (half*)output[i];
    }
    inLines = (half**)inLine;
    outLines = (half**)outLine;

    #ifdef UNIT_TEST_USE_C_VERSION
    mvcvMaxPool3x3hh(inLines, outLines, width);
    #else
    mvcvMaxPool3x3hh_asm(inLines, outLines, width);
    #endif

    SHAVE_HALT;
    return 0;
}
