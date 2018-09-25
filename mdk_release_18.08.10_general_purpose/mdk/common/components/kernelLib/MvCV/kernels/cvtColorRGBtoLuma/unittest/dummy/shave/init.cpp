///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    cvtColorRGBtoLuma kernel function call for unitary test
///

#include "cvtColorRGBtoLuma.h"
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 1

u8 inputR[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH];
u8 inputG[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH];
u8 inputB[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH];

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outputY_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) outputY[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outputY_u32postPad[4] __attribute__ ((aligned (16)));


u32 width;
u8* inLineR[TEST_FRAME_HEIGHT];
u8* inLineG[TEST_FRAME_HEIGHT];
u8* inLineB[TEST_FRAME_HEIGHT];
u8* outLineY[TEST_FRAME_HEIGHT];

u8** inLinesR;
u8** inLinesG;
u8** inLinesB;
u8** outLinesY;




int main( void )
{
//	void cvtColorRGBtoLuma_asm(u8** inR, u8** inG, u8** inB, u8** yOut, u32 width) 

	inLineR[0] = (u8*)inputR[0];
	inLineG[0] = (u8*)inputG[0];
	inLineB[0] = (u8*)inputB[0];
	outLineY[0] = (u8*)outputY[0];
	
	inLinesR = (u8**)inLineR;
	inLinesG = (u8**)inLineG;
	inLinesB = (u8**)inLineB;
	outLinesY = (u8**)outLineY;

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvCvtColorRGBtoLuma(inLinesR, inLinesG, inLinesB, outLinesY, width);
	#else
	mvcvCvtColorRGBtoLuma_asm(inLinesR, inLinesG, inLinesB, outLinesY, width);
	#endif

	SHAVE_HALT; 
    return 0;
}
