///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Shave hello world source code
///

#include "channelExtract.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 16
#define NMB_TEST_FRAMES 2
#define OUT_DATA_LENGTH (TEST_FRAME_WIDTH * TEST_FRAME_HEIGHT * 3 / 2)
#define EXTRA_OUT_REQUIRED_SPACE 0

u8 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) __attribute__ ((aligned (16))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH];
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 plane;
u32 width;
u8* in[TEST_FRAME_HEIGHT];
u8* out[TEST_FRAME_HEIGHT];
u8** inLines;
u8** outLines;

int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		in[i] = (u8*)input[i];
		out[i] = (u8*)output[i];
	}

	inLines = (u8**)in;
	outLines = (u8**)out;

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvChannelExtract(in,out,width,plane);
	#else
	#ifdef MOVICOMPILE_OPTIMIZED
		mvcvChannelExtract_opt(in,out,width,plane);
		#else 
		mvcvChannelExtract_asm(in,out,width,plane);
		#endif
	#endif
	
	SHAVE_HALT;
    return 0;
}
