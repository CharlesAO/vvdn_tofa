///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Shave hello world source code
///

#include <pixelPos.h>
#include <stdio.h> 
#include "svuCommonShave.h"
#include <mv_types.h>

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 2


/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) pixelPosition_u32prePad[4] __attribute__ ((aligned (16)))  ;
u32 __attribute__((section(".kept.data")))  pixelPosition __attribute__ ((aligned (16))) ;
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) pixelPosition_u32postPad[4] __attribute__ ((aligned (16)))  ;

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) status_u32prePad[4] __attribute__ ((aligned (16)))  ;
u32 __attribute__((section(".kept.data")))  status __attribute__ ((aligned (16))) ;
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) status_u32postPad[4] __attribute__ ((aligned (16)))  ;



u8 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH];
u32 width;
u32 pixelValue;


u8* line;
u8 mask[TEST_FRAME_WIDTH * TEST_FRAME_HEIGHT];


int main( void )
{
	line = input[0];

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvPixelPos(&line, mask, width, (u8)pixelValue, &pixelPosition, (u8*)&status);
	#else
	mvcvPixelPos_asm(&line, mask, width, (u8)pixelValue, &pixelPosition, (u8*)&status);
	#endif

	SHAVE_HALT; 
    return 0;
}
