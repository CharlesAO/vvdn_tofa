///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    genChromaRGB16 kernel function call for unitary test
///

#include <mv_types.h>
#include <genChromaRGB16.h>
#include <stdlib.h>
#include <svuCommonShave.h>

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 3

 //u16 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] = {
 //{6242, 10118, 6158, 2645, 10588, 10505, 9234, 14125},
 //{3612, 16012, 10029, 5007, 8714, 4697, 7491, 9171},
 //{14356, 8152, 10753, 280, 4557, 8371, 16091, 10623}
// };
//u8 inputY[TEST_FRAME_WIDTH] = {58, 136, 10, 114, 230, 253, 91, 192};


u16 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u8 inputY[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

u32 eps __attribute__ ((aligned (16)));
u32 nBits __attribute__ ((aligned (16)));
u32 width __attribute__ ((aligned (16)));
float  chromaScale1[3] __attribute__ ((aligned (16)));
float *chromaScale;

u16* inLine[TEST_FRAME_HEIGHT];
u8* outLine[TEST_FRAME_HEIGHT];
 
u16** inLines;

u8** outLines;

 //make run_test SRV_IP="-srvIP: 192.168.72.127" SRV_PORT="-srvPort: 15000"

int main( void )
{
	inLine[0] = (u16*)input[0];
	inLine[1] = (u16*)input[1];
	inLine[2] = (u16*)input[2];
	outLine[0] = (u8*)output[0];
	outLine[1] = (u8*)output[1];
	outLine[2] = (u8*)output[2];
	
	inLines = (u16**)inLine;
	outLines = (u8**)outLine;
	
	 // nBits = 10;
 	 // eps = 191;
	 // width = 16; 

	
	chromaScale = (float *)chromaScale1;
	#ifdef UNIT_TEST_USE_C_VERSION
	mvispGenChromaRGB16(inLines, inputY, outLines, width, eps, nBits, chromaScale);
	#else
	mvispGenChromaRGB16_asm(inLines, inputY, outLines, width, eps, nBits, chromaScale);
	#endif
	
	SHAVE_HALT;
	return 0;
}

