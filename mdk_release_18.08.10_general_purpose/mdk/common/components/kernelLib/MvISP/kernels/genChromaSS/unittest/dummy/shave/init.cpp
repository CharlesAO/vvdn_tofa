///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    
///

#include "genChromaSS.h"
#include <stdio.h>
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 16

u8  input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));


int eps __attribute__ ((aligned (16)));
float chromaScale[3] __attribute__ ((aligned (16)));
u32 width __attribute__ ((aligned (16)));

u8 *outLineP[3];
u8 *inLineP[6];

int main( void )
{
    outLineP[0] = &output[0][0];
    outLineP[1] = &output[1][0];
    outLineP[2] = &output[2][0];
    inLineP[0]  = &input[0][0];
    inLineP[1]  = &input[1][0];
    inLineP[2]  = &input[2][0];    
    inLineP[3]  = &input[3][0];    
    inLineP[4]  = &input[4][0];    
    inLineP[5]  = &input[5][0];    
	
	mvispGenChromaSS_asm(outLineP, inLineP, eps, chromaScale, width);
	SHAVE_HALT; 
	return 0;
}
