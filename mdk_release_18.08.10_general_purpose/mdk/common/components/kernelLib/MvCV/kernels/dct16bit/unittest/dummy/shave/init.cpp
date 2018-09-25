///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "dct16bit.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"


#define TILE_HEIGHT 8
#define TILE_WIDTH  8



short  input_tile[TILE_HEIGHT][TILE_WIDTH];


/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outputtile_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
short __attribute__((section(".kept.data"))) output_tile[TILE_HEIGHT][TILE_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outputtile_u32postPad[4] __attribute__ ((aligned (16))); 

int main( void )
{
	
	#ifdef UNIT_TEST_USE_C_VERSION
	   mvcvDCT16bit(input_tile,output_tile);
	#else
	   mvcvDCT16bit_asm(input_tile, output_tile);
   
	#endif
	
	SHAVE_HALT;
    return 0;
}
