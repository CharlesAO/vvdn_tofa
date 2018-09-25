///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief      meshGenaratekernel function call for unitary test
///

#include "meshGenerate.h"
#include <stdio.h>
#include <stdlib.h>
#include "mv_types.h"
#include "svuCommonShave.h"

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 2


/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outputmesh_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) output_mesh[TEST_FRAME_WIDTH * 2] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outputmesh_u32postPad[4] __attribute__ ((aligned (16)));

float mat[6] __attribute__ ((aligned (16)));
u32 width __attribute__ ((aligned (16)));
u32 posx __attribute__ ((aligned (16)));
u32 posy __attribute__ ((aligned (16)));

int main( void )
{
//void MeshGenerate(half *mesh, float *mat, u32 width, u32 posx, u32 posy)

    #ifdef UNIT_TEST_USE_C_VERSION
    mvcvMeshGenerate(output_mesh, mat, width, posx, posy);
    #else
    mvcvMeshGenerate_asm(output_mesh, mat, width, posx, posy);
    #endif

    SHAVE_HALT;
    return 0;
}

