///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "censusMinConfidence32.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"


#define DISPARITIES 32
#define PADDING 32
#define TEST_FRAME_WIDTH (640 + PADDING)


unsigned char __attribute__ ((aligned (16))) input[TEST_FRAME_WIDTH * DISPARITIES];

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) minimumPosition_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
unsigned char __attribute__((section(".kept.data"))) minimumPosition[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) minimumPosition_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) minimumValue_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
unsigned char __attribute__((section(".kept.data"))) minimumValue[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) minimumValue_u32postPad[4] __attribute__ ((aligned (16)));

unsigned int width ;//= 16;

int main( void )
{
	#ifdef UNIT_TEST_USE_C_VERSION
		mvcvCensusMinConfidence32((unsigned char *)&input[16], (unsigned char *)&minimumPosition[16], (unsigned char *)&minimumValue[16], width);
	#else                                                                                                                           
		mvcvCensusMinConfidence32_asm((unsigned char *)&input[16], (unsigned char *)&minimumPosition[16], (unsigned char *)&minimumValue[16], width);
	#endif
	SHAVE_HALT; 
    return 0;
}
