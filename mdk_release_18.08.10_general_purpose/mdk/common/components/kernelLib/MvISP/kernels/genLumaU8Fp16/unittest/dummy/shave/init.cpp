///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    
///

#include <genLumaU8Fp16.h>
#include <stdio.h>
#include <stdlib.h>
#include <mv_types.h>
#include <moviVectorTypes.h>
#include <svuCommonShave.h>

// 8 padding values added to the maximum width
#define TEST_FRAME_WIDTH 1920 + 16
#define TEST_FRAME_HEIGHT 1

u8 inR[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u8 inG[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u8 inB[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width __attribute__ ((aligned (16)));
half coefs[3] __attribute__ ((aligned (16)));

int main( void )
{
	#ifdef UNIT_TEST_USE_C_VERSION
	mvispGenLumaU8Fp16(inR, inG, inB, output, coefs, width);
	#else
    mvispGenLumaU8Fp16_asm(inR, inG, inB, output, coefs, width);
	#endif
	
	SHAVE_HALT;
	return 0;
}
