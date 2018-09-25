///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Convolution7x7 kernel function call for unitary test
///

#include "ssdPointLine7x7U8U32.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

// 8 padding values added to the maximum width
#define TEST_FRAME_WIDTH 1920 + 16
#define TEST_FRAME_HEIGHT 7

u8 input1[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u8 input2[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u32 __attribute__((section(".kept.data"))) output[1][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width __attribute__ ((aligned (16)));

u8* inLine1[TEST_FRAME_HEIGHT];
u8* inLine2[TEST_FRAME_HEIGHT];
u32* outLine[1];

u8** inLines1;
u8** inLines2;
u32** outLines;


int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine1[i] = (u8*)input1[i] + 8;
		inLine2[i] = (u8*)input2[i] + 8;
	}
	outLine[0] = (u32*)output[0];

	inLines1 = (u8**)inLine1;
	inLines2 = (u8**)inLine2;
	
	outLines = (u32**)outLine;

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvSsdPointLine7x7U8U32(inLines1, inLines2, outLines, width);
	#else
		#ifdef MOVICOMPILE_OPTIMIZED
		mvcvSsdPointLine7x7U8U32_opt(inLines1, inLines2, outLines, width);
		#else
		mvcvSsdPointLine7x7U8U32_asm(inLines1, inLines2, outLines, width);
		#endif
	#endif
	

	
	SHAVE_HALT; 
    return 0;
}
