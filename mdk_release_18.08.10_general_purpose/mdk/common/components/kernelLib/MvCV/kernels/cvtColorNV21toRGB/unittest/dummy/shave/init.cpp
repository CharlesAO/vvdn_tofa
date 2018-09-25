///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     CvtColorNV21toRGBApp kernel function call for unitary test
///

#include "cvtColorNV21toRGB.h"
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 1

u8 inputY[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH];
u8 inputUV[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH];

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outputR_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) outputR[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outputR_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outputG_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) outputG[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outputG_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outputB_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) outputB[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outputB_u32postPad[4] __attribute__ ((aligned (16)));

u32 width;
u8* inLineY[TEST_FRAME_HEIGHT];
u8* inLineUV[TEST_FRAME_HEIGHT];
u8* outLineR[TEST_FRAME_HEIGHT];
u8* outLineG[TEST_FRAME_HEIGHT];
u8* outLineB[TEST_FRAME_HEIGHT];
u8** inLinesY;
u8** inLinesUV;
u8** outLinesR;
u8** outLinesG;
u8** outLinesB;



int main( void )
{
//	void cvtColorNV21toRGB(u8** inputY, u8** inputUV, u8** rOut, u8** gOut, u8** bOut, u32 width);

	inLineY[0] = (u8*)inputY[0];
	inLineUV[0] = (u8*)inputUV[0];
	outLineR[0] = (u8*)outputR[0];
	outLineG[0] = (u8*)outputG[0];
	outLineB[0] = (u8*)outputB[0];


	inLinesY = (u8**)inLineY;
	inLinesUV = (u8**)inLineUV;
	outLinesR = (u8**)outLineR;
	outLinesG = (u8**)outLineG;
	outLinesB = (u8**)outLineB;

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvCvtColorNV21toRGB(inLinesY, inLinesUV, outLinesR, outLinesG, outLinesB, width);
	#else
	mvcvCvtColorNV21toRGB_asm(inLinesY, inLinesUV, outLinesR, outLinesG, outLinesB, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
