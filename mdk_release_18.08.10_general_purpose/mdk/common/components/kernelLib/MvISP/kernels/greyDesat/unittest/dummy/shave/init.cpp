///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    greyDesat kernel function call for unitary test
///

#include <greyDesat.h>
#include <mv_types.h>
#include <stdlib.h>
#include <stdio.h>
#include <svuCommonShave.h>

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 3

u8 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u8* inLine[TEST_FRAME_HEIGHT];
u8* outLine[TEST_FRAME_HEIGHT];

int offset __attribute__ ((aligned (16)));
int slope __attribute__ ((aligned (16)));
int grey[3] __attribute__ ((aligned (16)));
u32 width __attribute__ ((aligned (16)));


int main( void )
{
	int i;
	for (i = 0; i < TEST_FRAME_HEIGHT; i++) {
	inLine[i] = (u8*)input[i];
	outLine[i] = (u8*)output[i];
	} 

	#ifdef UNIT_TEST_USE_C_VERSION
	mvispGreyDesat(inLine, outLine, offset, slope, grey, width);
	#else
	mvispGreyDesat_asm(inLine, outLine, offset, slope, grey, width);
	#endif
	
	SHAVE_HALT;
	return 0;
}
