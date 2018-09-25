///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    arithmeticAdd kernel function call for unitary test
///

#include "thresholdBinaryU8.h"
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920 + 8
#define TEST_FRAME_HEIGHT 1

u8 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width __attribute__ ((aligned (16)));
u8 thresh __attribute__ ((aligned (16)));
u8* inLine[TEST_FRAME_HEIGHT];
u8* outLine[TEST_FRAME_HEIGHT];
u8** inLines;
u8** outLines;

int main( void )
{
//	void thresholdBinaryU8(u8** in, u8** out, u8 threshold, u32 width)

	int i;
	for(i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] = (u8*)input[i];
		outLine[i] = (u8*)output[i];
	}

	inLines = (u8**)inLine;
	outLines = (u8**)outLine;
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvThresholdBinaryU8(inLines, outLines, thresh, width);
	#else
	#ifdef MOVICOMPILE_OPTIMIZED
		mvcvThresholdBinaryU8_opt(inLines, outLines, thresh, width);
        	#else
		mvcvThresholdBinaryU8_asm(inLines, outLines, thresh, width);
		#endif	
	#endif
	SHAVE_HALT; 
    return 0;
}