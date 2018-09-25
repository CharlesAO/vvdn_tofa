///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     cvtColorKernelYUV422ToRGB kernel function call for unitary test
///

#include "cvtColorKernelYUV422ToRGB.h"
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 1

u8 input[TEST_FRAME_HEIGHT][2 * TEST_FRAME_WIDTH];

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outputR_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) outputR[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outputR_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outpuG_u32prePad[4] __attribute__ ((aligned (16)));
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
u8* inLine[TEST_FRAME_HEIGHT];
u8* outLineR[TEST_FRAME_HEIGHT];
u8* outLineG[TEST_FRAME_HEIGHT];
u8* outLineB[TEST_FRAME_HEIGHT];
u8** inLines;
u8** outLinesR;
u8** outLinesG;
u8** outLinesB;



int main( void )
{
//	void cvtColorKernelYUV422ToRGB(u8** input, u8** rOut, u8** gOut, u8** bOut, u32 width);

	inLine[0] = (u8*)input[0];
	outLineR[0] = (u8*)outputR[0];
	outLineG[0] = (u8*)outputG[0];
	outLineB[0] = (u8*)outputB[0];


	inLines = (u8**)inLine;
	outLinesR = (u8**)outLineR;
	outLinesG = (u8**)outLineG;
	outLinesB = (u8**)outLineB;

	//mvcvCvtColorKernelYUV422ToRGB_asm(inLines, outLinesR, outLinesG, outLinesB, width);
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvCvtColorKernelYUV422ToRGB(inLines, outLinesR, outLinesG, outLinesB, width);
	#else
	mvcvCvtColorKernelYUV422ToRGB_asm(inLines, outLinesR, outLinesG, outLinesB, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
