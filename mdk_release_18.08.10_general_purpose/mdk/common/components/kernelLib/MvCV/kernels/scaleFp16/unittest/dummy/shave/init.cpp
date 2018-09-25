///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     scaleFp16 kernel function call for unitary test
///

#include "scaleFp16.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 12

half input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output pre pad marker*/
half __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
half __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));
half scale;
u32 width;
half* inLine[TEST_FRAME_HEIGHT];
half* outLine[TEST_FRAME_HEIGHT];
half** inLines;
half** outLines;



int main( void )
{
	int i;
	
	for(i = 0; i<TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] = (half*)input[i];
		outLine[i] = (half*)output[i];
	}

	inLines = (half**)inLine;
	outLines = (half**)outLine;

#ifdef UNIT_TEST_USE_C_VERSION
	mvcvScaleFp16(inLines, outLines, scale, width);
#else
	mvcvScaleFp16_asm(inLines, outLines, scale, width);
#endif
	
	SHAVE_HALT; 
    return 0;
}
