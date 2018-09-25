#include <gaussHx2_fp16.h>
#include <stdio.h>
#include <mv_types.h>
#include <moviVectorTypes.h>
#include <svuCommonShave.h>

//16 padding vaules
#define TEST_FRAME_WIDTH 1920 + 16

half __attribute__ ((aligned (16)))  input[1][TEST_FRAME_WIDTH] ;

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) output[1][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width ;

half* inLine[1];
half* outLine[1];
half** inLines;
half** outLines;


int main( void )
{
	inLine[0] = (half*)input[0] + 8;
	outLine[0] = (half*)output[0];
	
	inLines    = (half**)inLine;
	outLines   = (half**)outLine;

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvGaussHx2_fp16(inLines, outLines, width);
	#else
	mvcvGaussHx2_fp16_asm(inLines, outLines, width);
	#endif

	SHAVE_HALT;
	return 0;
}
