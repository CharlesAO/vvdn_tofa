///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    
///

#include <gtmFp16ToFp16.h>
#include <stdio.h>
#include <stdlib.h>
#include <mv_types.h>
#include <moviVectorTypes.h>
#include <svuCommonShave.h>

#define PADDING 16
#define TEST_FRAME_WIDTH 1920 + PADDING
#define INDEX 1920

half input[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

half scurve[INDEX] __attribute__ ((aligned (16)));
half* in;
half* out;
half* s_curve;
u32 width __attribute__ ((aligned (16)));
int nPoints __attribute__ ((aligned (16)));

int main( void )
{
	in = (half*)input;
	out = (half*)output;
	s_curve = (half*)scurve;
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvispGtmFp16ToFp16(in, out, width, s_curve, nPoints);
	#else
   	mvispGtmFp16ToFp16_asm(in, out, width, s_curve, nPoints);
   	//mvispGtmFp16ToFp16_asm(input, output, width, scurve, nPoints);
	#endif
	
	SHAVE_HALT;
	return 0;
}
