///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    conv3x3fp32Scharr kernel function call for unitary test
///

#include <conv3x3fp32Scharr.h>
#include <mv_types.h>
#include <svuCommonShave.h>
#include <stdio.h>
//#define UNIT_TEST_USE_C_VERSION
#define SIZE 200


u8 src[SIZE*SIZE] __attribute__ ((aligned (16)));
fp32 dstX[SIZE*SIZE];
fp32 dstY[SIZE*SIZE];
fp32 smooth_k[2];
s32 height;
s32 width;

int main( void )
{

	#ifdef UNIT_TEST_USE_C_VERSION
		mvcvConv3x3fp32Scharr(src, dstX, dstY, smooth_k, height, width);
	#else
		mvcvConv3x3fp32Scharr_asm(src, dstX, dstY, smooth_k, height, width);

	#endif

	SHAVE_HALT;
    return 0;
}
