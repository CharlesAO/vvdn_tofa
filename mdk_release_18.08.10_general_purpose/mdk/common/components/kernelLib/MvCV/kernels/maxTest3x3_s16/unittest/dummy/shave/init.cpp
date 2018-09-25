///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     maxMax kernel function call for unitary test
///

#include "maxTest3x3_s16.h"
#include <stdio.h>
#include "mv_types.h"
#include "svuCommonShave.h"

#define TEST_FRAME_WIDTH 320
#define MAX_NUMBER_OF_EXTREMAS 320

s16 inBuffer[TEST_FRAME_WIDTH * 3] __attribute__ ((aligned (16)));
s16 inBufferCandidates[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u32 maxLocationsIn[MAX_NUMBER_OF_EXTREMAS] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) maxLocationsOut_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u32 __attribute__((section(".kept.data"))) maxLocationsOut[MAX_NUMBER_OF_EXTREMAS] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) maxLocationsOut_u32postPad[4] __attribute__ ((aligned (16)));

u32 maxCountIn __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) maxCountOut_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u32 __attribute__((section(".kept.data"))) maxCountOut __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) maxCountOut_u32postPad[4] __attribute__ ((aligned (16)));

s16* inputLines[3];


int main( void )
{
    inputLines[0] = inBuffer;
    inputLines[1] = &inBuffer[TEST_FRAME_WIDTH];
    inputLines[2] = &inBuffer[TEST_FRAME_WIDTH * 2];

#ifdef UNIT_TEST_USE_C_VERSION
    mvcvMaxTest3x3_s16(inBufferCandidates, inputLines, maxLocationsIn, maxLocationsOut, maxCountIn, &maxCountOut);
#else
    mvcvMaxTest3x3_s16_asm(inBufferCandidates, inputLines, maxLocationsIn, maxLocationsOut, maxCountIn, &maxCountOut);
#endif
   
    SHAVE_HALT;
    return 0;
}
