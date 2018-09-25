///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    
///

#include <genLumaFp16.h>
#include <stdio.h>
#include <mv_types.h>
#include <moviVectorTypes.h>
#include <svuCommonShave.h>

// 8 padding values added to the maximum width
#define TEST_FRAME_WIDTH 1920 + 16
#define TEST_FRAME_HEIGHT 1

unsigned short inR[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
unsigned short inG[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
unsigned short inB[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width = 40 ;
float normVal = 0.004f ;

int main( void )
{
	#ifdef UNIT_TEST_USE_C_VERSION
	mvispGenLumaFp16(output, inR, inG, inB, normVal, width);
	#else
	mvispGenLumaFp16_asm(output, inR, inG, inB, normVal, width);
	#endif
	
	SHAVE_HALT;
	return 0;
}
