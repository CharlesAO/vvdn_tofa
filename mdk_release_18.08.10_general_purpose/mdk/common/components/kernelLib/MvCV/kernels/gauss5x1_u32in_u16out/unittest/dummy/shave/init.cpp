///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Sobel kernel function call for unitary test
///

#include <gauss5x1_u32in_u16out.h>
#include <stdio.h>
#include <mv_types.h>
#include <svuCommonShave.h>

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 5

u32 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u16 __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width;

u32* inLine[TEST_FRAME_HEIGHT];
u16* outLine[TEST_FRAME_HEIGHT];
u32** inLines;
u16** outLines;



int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] = (u32*)input[i];
		outLine[i] = (u16*)output[i];
	}

	inLines = (u32**)inLine;
	outLines = (u16**)outLine;
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvGauss5x1_u32in_u16out(inLines, outLines, width);
	#else
	mvcvGauss5x1_u32in_u16out_asm(inLines, outLines, width);
	#endif
	
	SHAVE_HALT;
	return 0;
}
