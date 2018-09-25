///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "warpMeshSample10bit.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"


#define TILE_HEIGHT 100
#define TILE_WIDTH  100
#define TILE_SIZE 16
#define PADDING 32


u16  __attribute__ ((aligned (16))) input_tile[TILE_HEIGHT][TILE_WIDTH];

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outputtile_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u16 __attribute__((section(".kept.data"))) output_tile[TILE_SIZE * TILE_SIZE] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outputtile_u32postPad[4] __attribute__ ((aligned (16)));

u32 tile_width __attribute__ ((aligned (16)));
u32 tile_height __attribute__ ((aligned (16)));
u16 **tile;
u16 *intile[TILE_HEIGHT];
 
 half mx[TILE_SIZE * (TILE_SIZE + PADDING)] __attribute__ ((aligned (16)));
 half my[TILE_SIZE * (TILE_SIZE + PADDING)] __attribute__ ((aligned (16)));


int main( void )
{
	for(int i = 0; i < TILE_HEIGHT; i++)
	{
		intile[i] = (u16*)input_tile[i];
	}
	
	tile = (u16**)intile;
	
	
	#ifdef UNIT_TEST_USE_C_VERSION
		mvcvWarpMeshSample10bit(tile, output_tile, mx, my, tile_width, tile_height);
	#else
	   mvcvWarpMeshSample10bit_asm(tile, output_tile, mx, my, tile_width, tile_height);
	#endif
	
	SHAVE_HALT;
    return 0;
}
