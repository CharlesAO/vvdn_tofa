#include <genChromaFp16.h>
#include <stdio.h>
#include <mv_types.h>
#include <moviVectorTypes.h>
#include <svuCommonShave.h>

// 8 padding values added to the maximum width
#define TEST_FRAME_WIDTH 1920 + 16

u16 input[3][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[3][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

half inY[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
float eps  = 0.4f;
float normVal = 0.4f;
u32 width = 40;

u8 *outLineP[3];
u16 *inLineP[3];

int main( void )
{
    outLineP[0] = &output[0][0];
    outLineP[1] = &output[1][0];
    outLineP[2] = &output[2][0];
    inLineP[0] = &input[0][0];
    inLineP[1] = &input[1][0];
    inLineP[2] = &input[2][0];    
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvispGenChromaFp16(outLineP, inLineP, inY, eps, normVal, width);
	#else
	mvispGenChromaFp16_asm(outLineP, inLineP, inY, eps, normVal, width);
	#endif
	
	SHAVE_HALT;
	return 0;
}
