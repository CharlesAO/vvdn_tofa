///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     gauss1x5_u16in_u32out kernel function call for unitary test
///

#include <gauss1x5_u16in_u32out.h>
#include <stdio.h>
#include <mv_types.h>
#include <svuCommonShave.h>

#define PADDING 16
#define TEST_FRAME_WIDTH 1920 + 16
#define TEST_FRAME_HEIGHT 16

u16 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH];

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u32 __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width;

u16* inLine[TEST_FRAME_HEIGHT];
u32* outLine[TEST_FRAME_HEIGHT];
u16** inLines;
u32** outLines;



int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] = (u16*)input[i] + PADDING / 2;
		outLine[i] = (u32*)output[i];
	}

	inLines = (u16**)inLine;
	outLines = (u32**)outLine;
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvGauss1x5_u16in_u32out(inLines, outLines, width);
	#else
	#ifdef MOVICOMPILE_OPTIMIZED
		mvcvGauss1x5_u16in_u32out_opt(inLines, outLines, width);
		#else
		mvcvGauss1x5_u16in_u32out_asm(inLines, outLines, width);
		#endif
	#endif
	
	SHAVE_HALT;
	return 0;
}
