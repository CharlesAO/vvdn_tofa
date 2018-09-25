///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     CornerMinEigenVal kernel function call for unitary test
///

#include "cornerMinEigenVal_patched.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

//CornerMinEigenVal_patched(u8 *input_buffer, u32 posy, u8 *out_pix, u32 width, u32 stride)
// 4 (kernel_size - 1) padding values added to the maximum width
#define TEST_FRAME_WIDTH 1920 + 16
#define TEST_FRAME_HEIGHT 16

u8 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH];

u32 width;
int posx;
u8* inLine[TEST_FRAME_HEIGHT];
u8** inLines;


/*output pre pad marker*/
uint32_t outputprePad;
/*output data marker*/
u8 output[1];
/*output post pad marker*/
uint32_t outputpostPad;



int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] = (u8*)input[i] +8;
	}

	inLines = (u8**)inLine;

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvCornerMinEigenVal_patched(inLines, posx, output, width);
	#else
	mvcvCornerMinEigenVal_patched_asm(inLines, posx, output, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
