#include "cvtColorRGBfp16ToLumaU8.h"
#include "svuCommonShave.h"
#include "mv_types.h"
#include <moviVectorTypes.h>

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 3


half input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));


u32 width;
half* inLineRGB[TEST_FRAME_HEIGHT];

int main( void )
{
//    cvtColorRGBfp16ToLumaU8(half* inRGB[3], u8* yOut, u32 width)

    for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
    {
        inLineRGB[i] = (half*)input[i];
    }

    #ifdef UNIT_TEST_USE_C_VERSION
    mvcvCvtColorRGBfp16ToLumaU8(inLineRGB, output, width);
    #else
    #ifdef MOVICOMPILE_OPTIMIZED
        mvcvCvtColorRGBfp16ToLumaU8_opt(inLineRGB, output, width);
        #else
        mvcvCvtColorRGBfp16ToLumaU8_asm(inLineRGB, output, width);
        #endif
    #endif
    SHAVE_HALT;
    return 0;
}
