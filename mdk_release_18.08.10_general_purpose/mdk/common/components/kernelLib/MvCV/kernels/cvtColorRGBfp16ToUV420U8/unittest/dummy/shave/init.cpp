///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    cvtColorRGBtoUV kernel function call for unitary test
///

#include "cvtColorRGBfp16ToUV420U8.h"
#include "svuCommonShave.h"
#include "mv_types.h"
#include <moviVectorTypes.h>

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 3

half inputRGB[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outputU_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) outputU[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outputU_u32postPad[4] __attribute__ ((aligned (16)));


/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outputV_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) outputV[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outputV_u32postPad[4] __attribute__ ((aligned (16)));



u32 width;

half* inLinesRGB[TEST_FRAME_HEIGHT];

int main( void )
{
//	cvtColorRGBfp16ToUV420U8(half* inRGB[3], u8* uOut, u8* vOut, u32 width)

	int i;
	for (i = 0; i < TEST_FRAME_HEIGHT; i++) {
	inLinesRGB[i] = (half*)&inputRGB[i]; 
	}
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvCvtColorRGBfp16ToUV420U8(inLinesRGB, outputU, outputV, width);
	#else
	mvcvCvtColorRGBfp16ToUV420U8_asm(inLinesRGB, outputU, outputV, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
