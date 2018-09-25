///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     arithmeticSubFp16ToFp16 kernel function call for unitary test
///

#include "arithmeticSubFp16ToFp16.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920 + 32
#define TEST_FRAME_HEIGHT 1

half input1[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH]  __attribute__ ((aligned (16)));
half input2[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH]  __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half  __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH]  __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width;
half* inLine1[TEST_FRAME_HEIGHT];
half* inLine2[TEST_FRAME_HEIGHT];
half* outLine[TEST_FRAME_HEIGHT];
half** inLines1;
half** inLines2;
half** outLines;


int main( void )
{
	inLine1[0] = (half*)input1[0];
	inLine2[0] = (half*)input2[0];
	outLine[0] = (half*)output[0] + 16;

	inLines1 = (half**)inLine1;
	inLines2 = (half**)inLine2;
	outLines = (half**)outLine;
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvArithmeticSubFp16ToFp16(inLines1, inLines2, outLines, width);
	#else
	mvcvArithmeticSubFp16ToFp16_asm(inLines1, inLines2, outLines, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
