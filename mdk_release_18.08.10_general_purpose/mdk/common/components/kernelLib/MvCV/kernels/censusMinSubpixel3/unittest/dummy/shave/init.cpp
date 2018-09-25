///
/// @file
/// @copyright Three code copyright Movidius Ltd 2013, Three rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "censusMinSubpixel3.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_LINE_WIDTH 640
#define DISPARITIES 3
#define PADDING 16 
#define OFFSET PADDING/2 

u8 __attribute__ ((aligned (16))) input[TEST_LINE_WIDTH * DISPARITIES + PADDING];
/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) out_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) out[TEST_LINE_WIDTH + PADDING] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) out_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outm_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) outm[TEST_LINE_WIDTH + PADDING] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outm_u32postPad[4] __attribute__ ((aligned (16)));


half __attribute__ ((aligned (16))) *output;
u8 __attribute__ ((aligned (16))) *outputm;
u32 __attribute__ ((aligned (16))) width;

int main( void )
{

	output = (half *)out + OFFSET;
	outputm = (u8 *)outm + OFFSET;

	#ifdef UNIT_TEST_USE_C_VERSION
		mvcvCensusMinSubpixel3(input, output, outputm, width);
	#else
		mvcvCensusMinSubpixel3_asm(input, output, outputm, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
