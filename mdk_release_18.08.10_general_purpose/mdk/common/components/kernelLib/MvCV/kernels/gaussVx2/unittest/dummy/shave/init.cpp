#include <gaussVx2.h>
#include <stdio.h>
#include <mv_types.h>
#include <svuCommonShave.h>


#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 5

u8 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width;

u8* inLine[TEST_FRAME_HEIGHT];
u8** inLines;



int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] = (u8*)input[i];
	}

	inLines = (u8**)inLine;
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvGaussVx2(inLines, output, width);
	#else
	mvcvGaussVx2_asm(inLines, output, width);
	#endif
	
	SHAVE_HALT;
	return 0;
}
