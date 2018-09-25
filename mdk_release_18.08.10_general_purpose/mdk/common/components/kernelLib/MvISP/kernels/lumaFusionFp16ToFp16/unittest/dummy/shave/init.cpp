///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    
///

#include <lumaFusionFp16ToFp16.h>
#include <stdio.h>
#include <stdlib.h>
#include <mv_types.h>
#include <moviVectorTypes.h>
#include <svuCommonShave.h>

#define PADDING 32
#define TEST_FRAME_WIDTH 1920 + PADDING
#define INDEX 1920

half inLumaShort[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
half inLumaLong[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outputAlpha_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) outputAlpha[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outputAlpha_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t __attribute__((section(".kept.data"))) outputLumaFusionprePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) outputLumaFusion[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outputLumaFusionpostPad[4] __attribute__ ((aligned (16)));


u32 width __attribute__ ((aligned (16)));
float params[4] __attribute__ ((aligned (16)));

int main( void )
{
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvispLumaFusionFp16ToFp16(inLumaShort, inLumaLong, outputAlpha, outputLumaFusion, width, (LumaFusionParam *)params);
	#else
   	mvispLumaFusionFp16ToFp16_asm(inLumaShort, inLumaLong, outputAlpha, outputLumaFusion, width, (LumaFusionParam *)params);
   	//mvispGtmFp16ToFp16_asm(input, output, width, scurve, nPoints);
	#endif
	
	SHAVE_HALT;
	return 0;
}
