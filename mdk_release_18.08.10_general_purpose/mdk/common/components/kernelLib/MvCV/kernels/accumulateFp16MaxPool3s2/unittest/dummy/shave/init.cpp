///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     accumulateFp16MaxPool3s2 kernel function call for unitary test
///

#include "accumulateFp16MaxPool3s2.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"


#define PADDING 32
#define TEST_FRAME_WIDTH 1920 + PADDING
#define TEST_FRAME_HEIGHT 3

half input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width;
half* inLine[TEST_FRAME_HEIGHT];
half* outLine[TEST_FRAME_HEIGHT];
half** inLines;
half** outLines;


int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] = (half*)input[i];
		outLine[i] = (half*)output[i];

	}

	inLines = (half**)inLine;
	outLines = (half**)outLine;
	
	#ifdef UNIT_TEST_USE_C_VERSION
		mvcvAccumulateFp16MaxPool3s2(outLines, inLines, width);
	#else   
		mvcvAccumulateFp16MaxPool3s2_asm(outLines, inLines, width);
	#endif
	
	SHAVE_HALT;
    return 0;
}
