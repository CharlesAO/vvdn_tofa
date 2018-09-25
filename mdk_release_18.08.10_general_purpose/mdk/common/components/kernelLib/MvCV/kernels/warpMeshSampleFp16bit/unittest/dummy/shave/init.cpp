///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "warpMeshSampleFp16bit.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"


#define TILE_HEIGHT 100
#define TILE_WIDTH  100
#define TILE_SIZE 16
#define PADDING 32


half  __attribute__ ((aligned (16))) input_tile[TILE_HEIGHT][TILE_WIDTH];
half  __attribute__ ((aligned (16))) output_tile[TILE_SIZE * TILE_SIZE];


u32 tile_width;
u32 tile_height;
half **tile;
half *intile[TILE_HEIGHT];
 
 half mx[TILE_SIZE * (TILE_SIZE + PADDING)];
 half my[TILE_SIZE * (TILE_SIZE + PADDING)];


int main( void )
{
	for(int i = 0; i < TILE_HEIGHT; i++)
	{
		intile[i] = (half*)input_tile[i];
	}
	
	tile = (half**)intile;
	
	
	#ifdef UNIT_TEST_USE_C_VERSION
		mvcvWarpMeshSampleFp16bit(tile, output_tile, mx, my, tile_width, tile_height);
	#else
	    mvcvWarpMeshSampleFp16bit_asm(tile, output_tile, mx, my, tile_width, tile_height);
	#endif
	
	SHAVE_HALT;
    return 0;
}
