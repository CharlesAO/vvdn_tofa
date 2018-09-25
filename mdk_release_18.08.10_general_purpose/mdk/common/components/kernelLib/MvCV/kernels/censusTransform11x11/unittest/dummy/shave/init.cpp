///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     CensusTransform11x11 kernel function call for unitary test
///

#include "censusTransform11x11.h"
#include <stdio.h>
#include "svuCommonShave.h"
#include "mv_types.h"
#include <moviVectorTypes.h>

#define PADDING 16
#define TEST_FRAME_WIDTH (1920 + PADDING)
#define TEST_FRAME_HEIGHT 11


u16 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH*16] __attribute__ ((aligned (16))) ;
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));



u16* inLine[TEST_FRAME_HEIGHT];
u16** inLines;

u32 width;

int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i]	= (u16*)input[i] + PADDING / 2;
	}

	inLines	= (u16**)inLine;

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvCensusTransform11x11(inLines, output, width);
	#else
	#ifdef MOVICOMPILE_OPTIMIZED
        	mvcvCensusTransform11x11_opt(inLines, output, width);
		#else
		mvcvCensusTransform11x11_asm(inLines, output, width);
		#endif
	#endif
	SHAVE_HALT; 
	return 0;
}
