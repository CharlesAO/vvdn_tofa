///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///
// 1: Includes
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include <swcFrameTypes.h>
#include <stdio.h>
#include <DrvCpr.h>
#include "OsDrvSvu.h"
#include "rtems_config.h"
// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define FRAME_WIDTH    80
#define FRAME_HEIGHT   60

#define CMX_FRAMES __attribute__((section(".cmx.data")))

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern u8 inputFrame;
extern u32 cpyPlns0_start;

// 4: Static Local Data
// ----------------------------------------------------------------------------
static u8 CMX_FRAMES outputFrame[FRAME_WIDTH * FRAME_HEIGHT * 3 / 2];
static frameSpec   testFrameSpec;
static frameBuffer inBuffer;
static frameBuffer outBuffer;
static osDrvSvuHandler_t handler;

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

    outBuffer.spec = testFrameSpec;
    outBuffer.p1 = outputFrame;
    outBuffer.p2 = outputFrame + width * height;
    outBuffer.p3 = outputFrame + width * height + width * height / 4;

    return;
}


void *POSIX_Init (void *args)
{
    UNUSED(args);

    u32 running;
    s32 sc;

    sc = initClocksAndMemory();
    if(sc)
        exit(sc);
    sc = OsDrvSvuInit();
    if(sc)
        exit(sc);
    printf ("RTEMS POSIX Started\n");  /* initialise variables */

    InitTestBuffers(FRAME_WIDTH, FRAME_HEIGHT);

    if (OsDrvSvuOpenShave(&handler, SHAVE_NUMBER, OS_MYR_PROTECTION_SEM) == OS_MYR_DRV_SUCCESS)
    {
        sc = OsDrvSvuResetShave(&handler);
        if(sc)
            exit(sc);
        sc = OsDrvSvuSetAbsoluteDefaultStack(&handler);
        if(sc)
            exit(sc);
        sc = OsDrvSvuStartShaveCC(&handler,(u32)&cpyPlns0_start, "ii", ((u32)&inBuffer), ((u32)&outBuffer));
        if(sc)
            exit(sc);
        sc = OsDrvSvuWaitShaves(1, &handler, OS_DRV_SVU_WAIT_FOREVER, &running);
        if(sc)
            exit(sc);
        sc = OsDrvSvuCloseShave(&handler);
        if(sc)
            exit(sc);
    }
    else {
        printf("[T2] cannot open shave %d\n", SHAVE_NUMBER);
        exit(sc);
    }


    printf("\nCopy planes done!\n\n");

    printf("To check the result use:\n");
    printf("mdbg::savefile image_80x60_P420.yuv outputFrame 7200\n");

    exit(0);
}


