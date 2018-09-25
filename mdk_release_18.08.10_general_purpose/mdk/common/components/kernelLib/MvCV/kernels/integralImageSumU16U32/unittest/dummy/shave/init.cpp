///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     IntegralImageSumU32 kernel function call for unitary test
///

#include <integralImageSumU16U32.h>
#include <stdio.h>
#include <mv_types.h>
#include <svuCommonShave.h>

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 5

unsigned short in[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
unsigned int __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

unsigned int prev[TEST_FRAME_WIDTH];

unsigned int width __attribute__ ((aligned (16)));
unsigned int runNr __attribute__ ((aligned (16)));

unsigned int*  previsionOut[TEST_FRAME_HEIGHT];
unsigned int** previsionOutLnPointer;

int main( void )
{
	previsionOut[0] = (unsigned int*)prev;
	previsionOutLnPointer = (unsigned int**)previsionOut;
    
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvIntegralImageSumU16U32(output, in, runNr, previsionOutLnPointer, width);
	#else
	mvcvIntegralImageSumU16U32_asm(output, in, runNr, previsionOutLnPointer, width);
	#endif

	SHAVE_HALT;
	return 0;
}
