///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    integralImageSqSum kernel function call for unitary test
///

#include <integralImageSquareSumM2.h>
#include <stdio.h> 
#include "svuCommonShave.h"
#include <mv_types.h>

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 1

unsigned char input[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
unsigned int __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

unsigned int prev[TEST_FRAME_WIDTH]  __attribute__ ((aligned (16)));

unsigned int width  __attribute__ ((aligned (16)));
unsigned int runNr  __attribute__ ((aligned (16)));

unsigned int*  previsionOut[TEST_FRAME_HEIGHT];
unsigned int** previsionOutLnPointer;


int main( void )
{
	previsionOut[0] = (unsigned int*)prev;
	previsionOutLnPointer = (unsigned int**)previsionOut;
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvIntegralimage_sqsum_u32_M2(output, input, runNr, previsionOutLnPointer, width);
	#else
		#ifdef MOVICOMPILE_OPTIMIZED
		mvcvIntegralimage_sqsum_u32_M2_opt(output, input, runNr, previsionOutLnPointer, width);
		#else
		mvcvIntegralimage_sqsum_u32_M2_asm(output, input, runNr, previsionOutLnPointer, width);
		#endif
	#endif	

	SHAVE_HALT; 
    return 0;
}
