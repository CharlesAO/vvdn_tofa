///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "downsampleBilinearLine.h"
#include <stdio.h>
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920


unsigned short __attribute__ ((aligned (16))) inLine[TEST_FRAME_WIDTH] ;
/*output pre pad marker*/
uint32_t __attribute__((section(".kept.data"))) tempLineprePad __attribute__ ((aligned (16)));
/*output data marker*/
unsigned int __attribute__((section(".kept.data"))) tempLine[TEST_FRAME_WIDTH]__attribute__ ((aligned (16))) ;
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) tempLinepostPad __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t __attribute__((section(".kept.data"))) outLineprePad __attribute__ ((aligned (16)));
/*output data marker*/
unsigned short __attribute__((section(".kept.data")))  outLine[TEST_FRAME_WIDTH]__attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outLinepostPad __attribute__ ((aligned (16)));

unsigned int inWidth;
unsigned int select_line;

int main( void )
{

    #ifdef UNIT_TEST_USE_C_VERSION
        mvcvDownsampleBilinearLine(inLine, tempLine, outLine, inWidth, select_line);
    #else
        mvcvDownsampleBilinearLine_asm(inLine, tempLine, outLine, inWidth, select_line);
    #endif

    SHAVE_HALT;
    return 0;
}
