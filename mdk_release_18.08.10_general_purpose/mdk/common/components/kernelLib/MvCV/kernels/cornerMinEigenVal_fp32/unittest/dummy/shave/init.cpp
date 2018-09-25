///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     CornerMinEigenVal_f32 kernel function call for unitary test
///

#include "cornerMinEigenVal_fp32.h"
#include <stdio.h>
#include "svuCommonShave.h"
#include "mv_types.h"

// 4 (kernel_size - 1) padding values added to the maximum width
#define TEST_FRAME_WIDTH 1920 + 16
#define TEST_FRAME_HEIGHT 16

fp32 __attribute__ ((aligned (32))) buffer[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH];

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
fp32 __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));;

u8 __attribute__ ((aligned (16))) input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH];


fp32 *interim;
u32 width;
u8* inLine[TEST_FRAME_HEIGHT];
fp32* outLine[TEST_FRAME_HEIGHT];
u8** inLines;
fp32** outLines;



int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] = (u8*)input[i]+8;
		outLine[i] = (fp32*)output[i];
	}

	inLines = (u8**)inLine;
	outLines = (fp32**)outLine;
	interim = (fp32*) buffer;

    #ifdef UNIT_TEST_USE_C_VERSION
	mvcvCornerMinEigenVal_fp32(inLines, outLines, interim,  width);
	#else
		#ifdef MOVICOMPILE_OPTIMIZED
		mvcvCornerMinEigenVal_fp32_opt(inLines, outLines, interim,  width);
		#else
		mvcvCornerMinEigenVal_fp32_asm(inLines, outLines, interim,  width);
		#endif
	#endif

	SHAVE_HALT;
    return 0;
}
