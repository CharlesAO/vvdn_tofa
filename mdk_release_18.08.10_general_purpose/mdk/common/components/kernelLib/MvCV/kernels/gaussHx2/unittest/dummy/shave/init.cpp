#include <gaussHx2.h>
#include <stdio.h>
#include <mv_types.h>
#include <svuCommonShave.h>

//4 padding vaules
#define TEST_FRAME_WIDTH 3840 + 4

u8 input[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width;

int main( void )
{
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvGaussHx2(input + 2, output, width);
	#else
	mvcvGaussHx2_asm(input + 2, output, width);
	#endif

    SHAVE_HALT; 
	return 0;
}
