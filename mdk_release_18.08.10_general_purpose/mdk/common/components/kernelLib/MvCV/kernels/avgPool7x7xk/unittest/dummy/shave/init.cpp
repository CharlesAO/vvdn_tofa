///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     avgPool7x7xk kernel function call for unitary test
///

#include "avgPool7x7xk.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 240 + 32
#define TEST_FRAME_HEIGHT 49

half input1[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half  __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width;
half* inLine1[TEST_FRAME_HEIGHT];
half* outLine[TEST_FRAME_HEIGHT];
half** inLines1;
half** outLines;


int main( void )
{
	int i;
	for(i=0;i<TEST_FRAME_HEIGHT;i++)
	{
	inLine1[i] = (half*)input1[i];
	}
	outLine[0] = (half*)output[0] + 16;

	inLines1 = (half**)inLine1;
	outLines = (half**)outLine;
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvAvgPool7x7xk(inLines1, outLines, width);
	#else
	mvcvAvgPool7x7xk_asm(inLines1, outLines, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
