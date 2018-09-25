///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Main leon file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <rtems.h>
#include <rtems/bspIo.h>
#include <swcFrameTypes.h>
#include "rtems_config.h"
#include "mv_types.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define FRAME_WIDTH    80
#define FRAME_HEIGHT   60

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern u8 inputFrame;

// 4: Static Local Data
// ----------------------------------------------------------------------------
static frameSpec   testFrameSpec;
static frameBuffer inBuffer;


// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

void InitTestBuffers(int width, int height)
{
    testFrameSpec.width = width;
    testFrameSpec.height = height;
    testFrameSpec.stride = width;
    testFrameSpec.type = YUV420p;
    testFrameSpec.bytesPP = 1;

    inBuffer.spec = testFrameSpec;
    inBuffer.p1 = (u8*)(&inputFrame);
    inBuffer.p2 = (u8*)((u32)(&inputFrame) + width * height);
    inBuffer.p3 = (u8*)((u32)(&inputFrame) + width * height + width * height / 4);

    return;
}


void *POSIX_Init (void *args)
{
    UNUSED(args);

    int i;
    s32 sc;

    sc = initClocksAndMemory();
    if(sc)
        exit(sc);
    printf ("RTEMS POSIX Started\n");  /* initialise variables */

    InitTestBuffers(FRAME_WIDTH, FRAME_HEIGHT);

    printf("\nInput image:\n");
    for (i = 0; i < 8; i++) {
        printf(" %X", inBuffer.p1[i]);
    }
    printf(" ...\n");

    exit(0);
}


