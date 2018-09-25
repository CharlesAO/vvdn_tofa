///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "tile_samplerRGB888.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"



#define TILE_HEIGHT 100
#define TILE_WIDTH  100

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outputtile_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output_tile[16 * 3 * 16 ] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outputtile_u32postPad[4] __attribute__ ((aligned (16)));

u8  input_tile[TILE_HEIGHT * TILE_WIDTH * 3] __attribute__ ((aligned (16)));

u32 tile_width __attribute__ ((aligned (16)));
u32 tile_height __attribute__ ((aligned (16)));

float xcoord[4] __attribute__ ((aligned (16)));
float ycoord[4] __attribute__ ((aligned (16)));

int main( void )
{
	//printf("%x %x\n", input_tile, output_tile);
	#ifdef UNIT_TEST_USE_C_VERSION
		mvcvtile_samplerRGB888(input_tile, output_tile, xcoord, ycoord, tile_width, tile_height);
	#else
		mvcvtile_samplerRGB888_asm(input_tile, output_tile, xcoord, ycoord, tile_width, tile_height);
	#endif
	
	SHAVE_HALT;
    return 0;
}
