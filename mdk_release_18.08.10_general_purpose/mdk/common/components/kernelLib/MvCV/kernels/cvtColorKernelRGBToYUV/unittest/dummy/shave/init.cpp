///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     cvtColor kernel function call for unitary test
///

#include "cvtColorKernelRGBToYUV.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 2

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));


u8 input[TEST_FRAME_HEIGHT*TEST_FRAME_WIDTH];

u32 width;
u32 height;

u8 y[TEST_FRAME_HEIGHT*TEST_FRAME_WIDTH];
u8 u[TEST_FRAME_HEIGHT*TEST_FRAME_WIDTH / 4];
u8 v[TEST_FRAME_HEIGHT*TEST_FRAME_WIDTH / 4];
u8 rgb[TEST_FRAME_HEIGHT*TEST_FRAME_WIDTH * 3];

int main( void )
{
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvCvtColorKernelRGBToYUV( rgb, y, u, v, width);
	#else
	mvcvCvtColorKernelRGBToYUV_asm( rgb, y, u, v, width);
	#endif

	SHAVE_HALT; 
    return 0;
}
