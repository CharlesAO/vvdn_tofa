///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "jpegGetBlockY420.h"
#include <stdio.h>
#include "svuCommonShave.h"
#include "mv_types.h"

#define TILE_WIDTH 8
#define TILE_HEIGHT 8
#define IMG_WIDTH 128
#define IMG_HEIGHT 128

u32 mode __attribute__ ((aligned (16)));
u32 imginfo_width __attribute__ ((aligned (16)));
u32 imgspec_x __attribute__ ((aligned (16)));
u32 imgspec_y __attribute__ ((aligned (16)));
u8 imginfo_Y[(IMG_HEIGHT+1)*IMG_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) imgspecY_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
short __attribute__((section(".kept.data"))) imgspecY[TILE_WIDTH][TILE_HEIGHT] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) imgspecY_u32postPad[4] __attribute__ ((aligned (16)));

int main( void )
{

    #ifdef UNIT_TEST_USE_C_VERSION
    mvcvjpegGetBlockY420(mode, imginfo_Y, imginfo_width, imgspec_x, imgspec_y, imgspecY);
    #else
    #ifdef MOVICOMPILE_OPTIMIZED
        mvcvjpegGetBlockY420_opt(mode, imginfo_Y, imginfo_width, imgspec_x, imgspec_y, imgspecY);
        #else
        mvcvjpegGetBlockY420_asm(mode, imginfo_Y, imginfo_width, imgspec_x, imgspec_y, imgspecY);
           #endif
    #endif

    SHAVE_HALT;
    return 0;
}
