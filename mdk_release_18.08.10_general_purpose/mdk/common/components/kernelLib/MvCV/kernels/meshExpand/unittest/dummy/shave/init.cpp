///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     arithmeticSubFp16ToFp16 kernel function call for unitary test
///

#include "meshExpand.h"
#include <stdio.h>
#include <stdlib.h>
#include "mv_types.h"
#include "svuCommonShave.h"

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 64

//#define HEIGHT_PADDING 64

#define TEST_Mesh_HEIGHT 2
#define OUT_PADDING 16
#define INPUT_PADDING 16

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH + 2 * OUT_PADDING] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u8 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH+INPUT_PADDING] __attribute__ ((aligned (16)));
half mesh[TEST_Mesh_HEIGHT][TEST_FRAME_WIDTH+INPUT_PADDING] __attribute__ ((aligned (16)));
//u8 output[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

u32 width __attribute__ ((aligned (16)));
u8* inLine[TEST_FRAME_HEIGHT];
half* meshh[TEST_Mesh_HEIGHT];

u8** inLines;
half** meshp;
float my __attribute__ ((aligned (16)));

int main( void )
{

    int i;
    for (i = 0; i < TEST_FRAME_HEIGHT; i++){
    inLine[i] = (u8*)input[i];
    }

    meshh[0] = (half*)mesh[0];
    meshh[1] = (half*)mesh[1];

    inLines = (u8**)inLine;
    meshp = (half**)meshh;

    #ifdef UNIT_TEST_USE_C_VERSION
    mvcvMeshExpand(&output[OUT_PADDING], inLines, width, meshp, my);
    #else
    mvcvMeshExpand_asm(&output[OUT_PADDING], &inLines[32], width, meshp, my);
    #endif

    SHAVE_HALT;
    return 0;
}
