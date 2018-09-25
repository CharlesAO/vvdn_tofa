///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     minMax kernel function call for unitary test
///
#include "localMaxMin3x3_s16.h"
#include <stdio.h>
#include "mv_types.h"
#include "svuCommonShave.h"

#define PADDING 16
#define TEST_FRAME_WIDTH 640
#define MAX_WIDTH (TEST_FRAME_WIDTH + PADDING)

#define MAX_NUMBER_OF_EXTREMAS 322
short input[MAX_WIDTH * 3] __attribute__((section(".kept.data")));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) minLocationList_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u32 __attribute__((section(".kept.data"))) minLocationList[MAX_NUMBER_OF_EXTREMAS] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) minLocationList_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) maxLocationList_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u32 __attribute__((section(".kept.data"))) maxLocationList[MAX_NUMBER_OF_EXTREMAS] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) maxLocationList_u32postPad[4] __attribute__ ((aligned (16)));

u32 width __attribute__((section(".kept.data")));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) minCount_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u32 __attribute__((section(".kept.data"))) minCount __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) minCount_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) maxCount_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u32 __attribute__((section(".kept.data"))) maxCount __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) maxCount_u32postPad[4] __attribute__ ((aligned (16)));

u32 candidateLocationIn[TEST_FRAME_WIDTH];
u32 countIn;
short* line[3];
short** inLines;

int main( void )
{
    line[0] = input + PADDING/2;
    line[1] = input + MAX_WIDTH + PADDING/2;
    line[2] = input + MAX_WIDTH * 2 + PADDING/2;
	inLines = (short**)line;
    #ifdef UNIT_TEST_USE_C_VERSION
    mvcvLocalMaxMin3x3_s16(inLines, candidateLocationIn, countIn, minLocationList, maxLocationList, &minCount, &maxCount);
    #else
    mvcvLocalMaxMin3x3_s16_asm(inLines, candidateLocationIn, countIn, minLocationList, maxLocationList, &minCount, &maxCount);
    #endif
    SHAVE_HALT;
    return 0;
}
