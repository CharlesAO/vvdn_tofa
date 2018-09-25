///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "warpMeshExpand.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"



#define TILE_HEIGHT 100
#define TILE_WIDTH  100


half xcoord[4] __attribute__ ((aligned (16)));
half ycoord[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) mx_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) mx[TILE_HEIGHT * TILE_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) mx_u32postPad[4] __attribute__ ((aligned (16)));


/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) my_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) my[TILE_HEIGHT * TILE_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) my_u32postPad[4] __attribute__ ((aligned (16)));

int main( void )
{
	//printf("%x %x\n", input_tile, output_tile);
	#ifdef UNIT_TEST_USE_C_VERSION
		mvcvWarpMeshExpand(xcoord, ycoord, mx, my);
	#else
		mvcvWarpMeshExpand_asm(xcoord, ycoord, mx, my);
	#endif
	
	SHAVE_HALT;
    return 0;
}
