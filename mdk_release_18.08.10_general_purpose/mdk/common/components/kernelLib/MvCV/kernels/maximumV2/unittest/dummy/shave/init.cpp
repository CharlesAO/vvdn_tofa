///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     MaximumV2 kernel function call for unitary test
///

#include "maximumV2.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 12


half input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
half output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
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
	mvcvMaximumV2(inLines, outLines, width);
	#else
	mvcvMaximumV2_asm(inLines, outLines, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
