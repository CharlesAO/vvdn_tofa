///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    sumOfSquaredDiff7x7U8ToU32 kernel function call for unitary test
///

#include "sumOfSquaredDiff7x7U8ToU32.h"
#include "svuCommonShave.h"
#include "mv_types.h"
#include "stdio.h"

#define PADDING 16
#define TEST_FRAME_WIDTH 1920 + PADDING
#define TEST_FRAME_HEIGHT 7

u8 input1[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u8 input2[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u32 __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));



u32 width __attribute__ ((aligned (16)));
u8* inLine1[TEST_FRAME_HEIGHT];
u8* inLine2[TEST_FRAME_HEIGHT];
u32* outLine[TEST_FRAME_HEIGHT];
u8** inLines1;
u8** inLines2;
u32** outLines;

int main( void )
{
	int i;
	for(i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine1[i] = (u8 *)input1[i] + PADDING / 2;
		inLine2[i] = (u8 *)input2[i] + PADDING / 2;
		outLine[i] = (u32 *)output[i];
	}

	inLines1 = (u8 **)inLine1;
	inLines2 = (u8 **)inLine2;
	outLines = (u32 **)outLine;
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvSumOfSquaredDiff7x7U8ToU32(inLines1, inLines2, outLines, width);
	#else
	mvcvSumOfSquaredDiff7x7U8ToU32_asm(inLines1, inLines2, outLines, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
