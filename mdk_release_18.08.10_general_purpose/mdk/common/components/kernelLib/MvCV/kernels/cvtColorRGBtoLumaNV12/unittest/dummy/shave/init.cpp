///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    cvtColorRGBtoLuma kernel function call for unitary test
///

#include "cvtColorRGBtoLumaNV12.h"
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 1

u8 inputR[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u8 inputG[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u8 inputB[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outputY_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) outputY[TEST_FRAME_WIDTH + 32] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outputY_u32postPad[4] __attribute__ ((aligned (16)));


unsigned int width;
float mat[3*3];
float offset[1*3];

int main( void )
{
//	
	//		width = 32;
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvCvtColorRGBtoLumaNV12(inputR, inputG, inputB, outputY, mat, offset, width);
	#else
	mvcvCvtColorRGBtoLumaNV12_asm(inputR, inputG, inputB, outputY, mat, offset,width);
	#endif

	SHAVE_HALT; 
    return 0;
}
