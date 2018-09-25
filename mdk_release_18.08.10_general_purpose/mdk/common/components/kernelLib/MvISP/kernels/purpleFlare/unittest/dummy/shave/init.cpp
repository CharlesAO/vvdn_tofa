#include "purpleFlare.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define PADDING 8
#define TEST_FRAME_WIDTH 1920 + PADDING
#define TEST_FRAME_HEIGHT 3

u16 inputGreen[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u16 inputBlue[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u16 __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));


u32 width __attribute__ ((aligned (16)));
u8 strength __attribute__ ((aligned (16)));
u16* inGreen;
u16* inBlue[TEST_FRAME_HEIGHT];
u16* outBlue;

u16** blue;


__attribute__((dllexport)) int main( void )
{
    for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
    {
        inBlue[i] = (u16*)inputBlue[i] + PADDING / 2;
    }

    inGreen = &inputGreen[0] + PADDING / 2;
    outBlue = &output[0];

    blue = (u16**)inBlue;

#ifdef UNIT_TEST_USE_C_VERSION
    mvispPurpleFlare(inGreen, blue, outBlue, strength, width);
#else
    mvispPurpleFlare_asm(inGreen, blue, outBlue, strength, width);
#endif

    SHAVE_HALT;
    return 0;
}
