///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Erode7x7 kernel function call for unitary test
///

#include "f32UnitTestSample.h"
#include <stdio.h>
#include "mv_types.h"
#include <svuCommonShave.h>

#define MAX_NUM_VECTORS 100

fp32 input_a[4];
fp32 input_b[MAX_NUM_VECTORS][4];

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
fp32 __attribute__((section(".kept.data"))) output[MAX_NUM_VECTORS] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

fp32 *in_b[4];
u32 num_vectors;

int main( void )
{

	in_b[0] =  (fp32*) input_b[0];
	in_b[1] =  (fp32*) input_b[1];
	in_b[2] =  (fp32*) input_b[2];
	in_b[3] =  (fp32*) input_b[3];
	
	mvuDotProductStreaming((fp32*) input_a, (fp32**) in_b, (fp32*) output, num_vectors);
	
	SHAVE_HALT;
	return 0;
}
