///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     cvtColorKernelRGBToYUV422 kernel function call for unitary test
///

#include "cvtColorKernelRGBToYUV422.h"
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 1

u8 inputR[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH];
u8 inputG[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH];
u8 inputB[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH];

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][2*TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));


u32 width;
u8* inLineR[TEST_FRAME_HEIGHT];
u8* inLineG[TEST_FRAME_HEIGHT];
u8* inLineB[TEST_FRAME_HEIGHT];
u8* outLine[TEST_FRAME_HEIGHT];
u8** inLinesR;
u8** inLinesG;
u8** inLinesB;
u8** outLines;


int main( void )
{
//	void cvtColorKernelRGBToYUV422(u8** rIn, u8** gIn, u8** bIn, u8** output, u32 width);

	inLineR[0] = (u8*)inputR[0];
	inLineG[0] = (u8*)inputG[0];
	inLineB[0] = (u8*)inputB[0];
	outLine[0] = (u8*)output[0];

	inLinesR = (u8**)inLineR;
	inLinesG = (u8**)inLineG;
	inLinesB = (u8**)inLineB;
	outLines = (u8**)outLine;

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvCvtColorKernelRGBToYUV422(inLinesR, inLinesG, inLinesB, outLines, width);
	#else
	mvcvCvtColorKernelRGBToYUV422_asm(inLinesR, inLinesG, inLinesB, outLines, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
