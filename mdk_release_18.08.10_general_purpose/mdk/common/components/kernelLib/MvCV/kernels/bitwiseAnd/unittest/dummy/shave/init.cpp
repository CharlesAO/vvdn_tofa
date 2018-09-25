///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     bitwiseAnd kernel function call for unitary test
///

#include "bitwiseAnd.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 1

u8 input1[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u8 input2[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width;
u8* inLine1[TEST_FRAME_HEIGHT];
u8* inLine2[TEST_FRAME_HEIGHT];
u8* outLine[TEST_FRAME_HEIGHT];
u8** inLines1;
u8** inLines2;
u8** outLines;



int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine1[i] = (u8*)input1[i];
		inLine2[i] = (u8*)input2[i];
		outLine[i] = (u8*)output[i];
	}

	inLines1 = (u8**)inLine1;
	inLines2 = (u8**)inLine2;
	outLines = (u8**)outLine;
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvBitwiseAnd(inLines1, inLines2, outLines, width);
	#else
	#ifdef MOVICOMPILE_OPTIMIZED
		mvcvBitwiseAnd_opt(inLines1, inLines2, outLines, width);
		#else
		mvcvBitwiseAnd_asm(inLines1, inLines2, outLines, width);
		#endif
	#endif
	SHAVE_HALT; 
    return 0;
}
