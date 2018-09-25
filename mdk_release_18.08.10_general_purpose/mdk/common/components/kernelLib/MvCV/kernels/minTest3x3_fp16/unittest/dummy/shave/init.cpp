///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     minMax kernel function call for unitary test
///

#include <stdio.h>
#include "svuCommonShave.h"
#include "mv_types.h"
#include "minTest3x3_fp16.h"

#define TEST_FRAME_WIDTH 640
#define TEST_FRAME_HEIGHT 2
#define MAX_NUMBER_OF_EXTREMAS 320
half inBuffer[TEST_FRAME_WIDTH * 3] __attribute__ ((aligned (16)));
half inBufferCandidates[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u32 minLocationsIn[MAX_NUMBER_OF_EXTREMAS] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) minLocationsOut_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u32 __attribute__((section(".kept.data"))) minLocationsOut[MAX_NUMBER_OF_EXTREMAS] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) minLocationsOut_u32postPad[4] __attribute__ ((aligned (16)));

u32 width __attribute__ ((aligned (16)));
u32 minCountIn __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) minCountOut_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u32 __attribute__((section(".kept.data"))) minCountOut __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) minCountOut_u32postPad[4] __attribute__ ((aligned (16)));

half* line[3] ;

int main( void )
{
    line[0] = inBuffer;
    line[1] = &inBuffer[width];
    line[2] = &inBuffer[width * 2];
    #ifdef UNIT_TEST_USE_C_VERSION
    mvcvMinTest3x3_fp16(inBufferCandidates, line, width, minLocationsIn, minLocationsOut, minCountIn, &minCountOut);
    #else
    mvcvMinTest3x3_fp16_asm(inBufferCandidates, line, width, minLocationsIn, minLocationsOut, minCountIn, &minCountOut);
    #endif
    SHAVE_HALT; 
    return 0;
}
