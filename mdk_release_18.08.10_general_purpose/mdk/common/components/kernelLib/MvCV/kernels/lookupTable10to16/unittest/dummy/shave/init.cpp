///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Shave hello world source code
///

#include <lookupTable10to16.h>
#include <stdio.h> 
#include "svuCommonShave.h"
#include <mv_types.h>

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 2
#define NMB_TEST_FRAMES 2
#define OUT_DATA_LENGTH (TEST_FRAME_WIDTH * TEST_FRAME_HEIGHT * 3 / 2)
#define EXTRA_OUT_REQUIRED_SPACE 0

u16 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u16 __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width __attribute__ ((aligned (16)));
u32 height __attribute__ ((aligned (16)));
u16* inLine[TEST_FRAME_HEIGHT];
u16* outLine[TEST_FRAME_HEIGHT];
u16 lut[256] __attribute__ ((aligned (16)));
u16** src;
u16** dest;

int main( void )
{
    for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
    {
        inLine[i] = (u16*)input[i];
        outLine[i] = (u16*)output[i]; 
    }
        
    src = (u16**)inLine;
    dest = (u16**)outLine;
     
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvLUT10to16(src, dest, lut, width, height);
	#else
	mvcvLUT10to16_asm(src, dest, lut, width, height);
	#endif

    SHAVE_HALT; 
    return 0;
}
