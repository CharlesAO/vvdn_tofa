///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     pixelUnpackWB kernel function call for unitary test
///

#include <pixelUnpackerWB.h>
#include <stdio.h> 
#include "svuCommonShave.h"
#include <mv_types.h>

#define TEST_FRAME_WIDTH 1920

u32 input32[TEST_FRAME_WIDTH] __attribute__ ((aligned (32)));
u8 input8[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u16 __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH * 4] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u16 awbCoef[4] __attribute__ ((aligned (16)));
u32 line_no __attribute__ ((aligned (16)));

u32 width __attribute__ ((aligned (16)));
u32 shift __attribute__ ((aligned (16)));

int main( void )
{
	#ifdef UNIT_TEST_USE_C_VERSION
	mvispPixelUnpackerWB(input32, input8, output, width, shift, awbCoef, line_no);
	#else
	mvispPixelUnpackerWB_asm(input32, input8, output, width, shift, awbCoef, line_no);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
