///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     CensusTransform7x7 kernel function call for unitary test
///

#include "censusTransform7x7.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"
#include <moviVectorTypes.h>

#define PADDING 16
#define TEST_FRAME_WIDTH 1920 + PADDING
#define TEST_FRAME_HEIGHT 7

u8 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u64  __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH] __attribute__ ((aligned (16))) ;
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));


u8* inLine[TEST_FRAME_HEIGHT];
u8** inLines;

u32 width;

int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i]	= (u8*)input[i] + PADDING / 2;
	}

	inLines		= (u8**)inLine;
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvCensusTransform7x7(inLines, output, width);
	#else
	mvcvCensusTransform7x7_asm(inLines, output, width);
	#endif
	
	SHAVE_HALT; 
	return 0;
}
