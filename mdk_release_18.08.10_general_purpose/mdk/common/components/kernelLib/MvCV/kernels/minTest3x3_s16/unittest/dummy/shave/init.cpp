///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     minMin kernel function call for unitary test
///

#include "minTest3x3_s16.h"
#include <stdio.h>
#include "mv_types.h"
#include "svuCommonShave.h"

#define TEST_FRAME_WIDTH 320
#define MAX_NUMBER_OF_EXTREMAS 320

s16 inBuffer[TEST_FRAME_WIDTH * 3] __attribute__ ((aligned (16)));
s16 inBufferCandidates[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u32 minLocationsIn[MAX_NUMBER_OF_EXTREMAS] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) minLocationsOut_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u32 __attribute__((section(".kept.data"))) minLocationsOut[MAX_NUMBER_OF_EXTREMAS] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) minLocationsOut_u32postPad[4] __attribute__ ((aligned (16)));

u32 minCountIn __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) minCountOut_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u32 __attribute__((section(".kept.data"))) minCountOut __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) minCountOut_u32postPad[4] __attribute__ ((aligned (16)));

s16* inputLines[3];


int main( void )
{
    inputLines[0] = inBuffer;
    inputLines[1] = &inBuffer[TEST_FRAME_WIDTH];
    inputLines[2] = &inBuffer[TEST_FRAME_WIDTH * 2];

#ifdef UNIT_TEST_USE_C_VERSION
    mvcvMinTest3x3_s16(inBufferCandidates, inputLines, minLocationsIn, minLocationsOut, minCountIn, &minCountOut);
#else
    mvcvMinTest3x3_s16_asm(inBufferCandidates, inputLines, minLocationsIn, minLocationsOut, minCountIn, &minCountOut);
#endif
   
    SHAVE_HALT;
    return 0;
}
