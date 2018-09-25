///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     fast9 kernel function call for unitary test
///

#include <scale05Lanc6HV.h>
#include <stdio.h> 
#include "svuCommonShave.h"
#include <mv_types.h>

#define PADDING 8
#define TEST_FRAME_WIDTH (1920 + PADDING * 2)
#define TEST_FRAME_HEIGHT 6

u8  input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

unsigned int width;

u8* inLine[TEST_FRAME_HEIGHT];
u8** inLines;

__attribute__((dllexport)) int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] = (u8*)(&input[i][PADDING]);
	}
	inLines = (u8**)inLine;
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvispScale05Lanc6HV(inLines, output, width);
	#else
	mvispScale05Lanc6HV_asm(inLines, output, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
