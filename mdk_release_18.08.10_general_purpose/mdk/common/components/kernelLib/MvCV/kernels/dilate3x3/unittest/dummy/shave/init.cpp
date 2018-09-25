///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Dilate3x3 source code
///

#include "dilate3x3.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define PADDING 16
#define TEST_FRAME_WIDTH 1920 + PADDING
#define TEST_FRAME_HEIGHT 3
#define KERNEL_SIZE 4

u8 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH];
u8 matrix[KERNEL_SIZE][KERNEL_SIZE];// = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width;
u32 height;
u8* inLine[TEST_FRAME_HEIGHT];
u8* outLine[TEST_FRAME_HEIGHT];
u8* kernelLine[KERNEL_SIZE];

u8** src;
u8** dst;
u8** kernel;

int main( void )
{
    /*width = 8;
	
	input[0][0] = 0;  input[0][1] = 0; input[0][2] = 0; input[0][3] = 0;
    input[0][4] = 0;  input[0][5] = 0;  input[0][6] = 0; input[0][7] = 0; 
	input[0][8] = 7;  input[0][9] = 13; input[0][10] = 20; input[0][11] = 9;
	input[0][12] = 2;  input[0][13] = 8;  input[0][14] = 12; input[0][15] = 3; 
	
	input[1][0] = 0;  input[1][1] = 0; input[1][2] = 0; input[1][3] = 0;
    input[1][4] = 0;  input[1][5] = 0;  input[1][6] = 0; input[1][7] = 0;     
	input[1][8] = 1;  input[1][9] = 10; input[1][10] = 6;  input[1][11] = 11; 
    input[1][12] = 15; input[1][13] = 10; input[1][14] = 9;  input[1][15] = 9; 
	
	input[2][0] = 0;  input[2][1] = 0; input[2][2] = 0; input[2][3] = 0;
    input[2][4] = 0;  input[2][5] = 0;  input[2][6] = 0; input[2][7] = 0;     
	input[2][8] = 9;  input[2][9] = 8;  input[2][10] = 3;  input[2][11] = 4; 
    input[2][12] = 7;  input[2][13] = 4;  input[2][14] = 1;  input[2][15] = 0; */
	
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
    {
        inLine[i] = (u8*)input[i] + PADDING / 2;
        outLine[i] = (u8*)output[i];
    }
		
	for( int i = 0; i < KERNEL_SIZE; i++)
	{
		kernelLine[i] = matrix[i]; // pointer to the i-th row
	}
	
    src = (u8**)inLine;
    dst = (u8**)outLine;
    kernel = (u8**)kernelLine;
    
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvDilate3x3(src, dst, kernel, width);
	#else
	mvcvDilate3x3_asm(src, dst, kernel, width);
	#endif

    SHAVE_HALT; 
    return 0;
}
