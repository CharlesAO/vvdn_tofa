///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     IntegralImageSumF32 kernel function call for unitary test
///

#include <integralImageSumFloatM2.h>
#include <stdio.h> 
#include "svuCommonShave.h"
#include <mv_types.h>

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 1

u8 input[TEST_FRAME_WIDTH]  __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
float __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

float prev[TEST_FRAME_WIDTH]  __attribute__ ((aligned (16)));

u32 width  __attribute__ ((aligned (16)));
u32 runNr  __attribute__ ((aligned (16)));

float*  previsionOut[TEST_FRAME_HEIGHT];
float** previsionOutLnPointer;

int main( void )
{
	previsionOut[0] = (float*)prev;
	previsionOutLnPointer = (float**)previsionOut;
    
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvIntegralimage_sum_f32_M2(output, input, runNr, previsionOutLnPointer, width);
	#else
	mvcvIntegralimage_sum_f32_M2_asm(output, input, runNr, previsionOutLnPointer, width);
	#endif

	SHAVE_HALT; 
    return 0;
}
