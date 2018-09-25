///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "censusMatching16.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define PADDING 16
#define TEST_FRAME_WIDTH (1920 + PADDING)


unsigned int  input1[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
unsigned int  input2[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
unsigned char __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH*32] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));


unsigned int *in1;
unsigned int *in2;
unsigned char *out;
unsigned int width;

int main( void )
{
    in1 = (unsigned int *)input1 + PADDING;
    in2 = (unsigned int *)input2 + PADDING;
	out = (unsigned char *)output +  PADDING/2;
	
	//printf("%x %x %x \n", in1,in2,out);
	#ifdef UNIT_TEST_USE_C_VERSION
		mvcvCensusMatching16(in1,in2,out,width);
	#else
		mvcvCensusMatching16_asm(in1,in2,out,width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
