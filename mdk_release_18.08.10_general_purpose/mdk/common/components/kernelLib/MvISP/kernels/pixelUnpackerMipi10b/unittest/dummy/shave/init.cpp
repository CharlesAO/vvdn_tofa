///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     pixelUnpackMipi10b kernel function call for unitary test
///

#include "pixelUnpackerMipi10b.h"
#include <stdio.h>
#include <stdlib.h>
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920
#define OUT_PADDING 16

u32 in[TEST_FRAME_WIDTH / 2] __attribute__ ((aligned (32)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u16 __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH + 2 * OUT_PADDING] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 coefs[4] __attribute__ ((aligned (16)));
u32 lineNo __attribute__ ((aligned (16)));
u32 width __attribute__ ((aligned (16)));
int noMipiRxWorkaround __attribute__ ((aligned (16)));



int main( void )
 {

	#ifdef UNIT_TEST_USE_C_VERSION
	mvispPixelUnpackerMipi10b(in, &output[OUT_PADDING], width, lineNo, coefs, noMipiRxWorkaround);
	#else
	mvispPixelUnpackerMipi10b_asm(in, &output[OUT_PADDING], width, lineNo, coefs, noMipiRxWorkaround);
	#endif
		
	SHAVE_HALT; 
	return 0;
}
