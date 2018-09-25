///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    arithmeticSubU16 kernel function call for unitary test
///

#include "arithmeticSubU16.h"
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 1

u16 input1[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u16 input2[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
s16 __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width;
u16* inLine1[TEST_FRAME_HEIGHT];
u16* inLine2[TEST_FRAME_HEIGHT];
s16* outLine[TEST_FRAME_HEIGHT];
u16** inLines1;
u16** inLines2;
s16** outLines;

int main( void )
{

	inLine1[0] = (u16*)input1[0];
	inLine2[0] = (u16*)input2[0];
	outLine[0] = (s16*)output[0];

	inLines1 = (u16**)inLine1;
	inLines2 = (u16**)inLine2;
	outLines = (s16**)outLine;

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvArithmeticSubU16(inLines1, inLines2, outLines, width);
	#else
	mvcvArithmeticSubU16_asm(inLines1, inLines2, outLines, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
