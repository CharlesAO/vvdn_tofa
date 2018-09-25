///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <swcFrameTypes.h>
#include <OsDrvSvu.h>
#include <OsDrvTimer.h>
#include "rtems_config.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define FRAME_WIDTH    80
#define FRAME_HEIGHT   60

#define CMX_FRAMES __attribute__((section(".cmx.data")))

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern u8  inputFrame;
extern u32 brthY0_start;

// 4: Static Local Data
// ----------------------------------------------------------------------------
u8 CMX_FRAMES outputFrame[FRAME_WIDTH * FRAME_HEIGHT * 3 / 2];

frameSpec   testFrameSpec;
frameBuffer inBuffer;
frameBuffer outBuffer;
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

    tyTimeStamp  g_ticks;
    u64 passed_cycles;
    u64 passed_cycles_bi;
    u32 use_builtins;
    u32 running;
    s32 sc;

    sc = initClocksAndMemory();
    if(sc)
        exit(sc);
    sc = OsDrvTimerInit();
    if(sc)
        exit(sc);
    sc = OsDrvSvuInit();
    if(sc)
        exit(sc);

    printf ("RTEMS POSIX Started\n");  /* initialise variables */
    InitTestBuffers(FRAME_WIDTH, FRAME_HEIGHT);

    sc = OsDrvSvuOpenShave(&handler, SHAVE_NUMBER, OS_MYR_PROTECTION_SEM);
    if (sc == OS_MYR_DRV_SUCCESS)
    {
        // Test normal code
        sc = OsDrvTimerStartTicksCount(&g_ticks);
        if(sc)
            exit(sc);
        sc = OsDrvSvuResetShave(&handler);
        if(sc)
            exit(sc);
        sc = OsDrvSvuSetAbsoluteDefaultStack(&handler);
        if(sc)
            exit(sc);

        use_builtins = 0;
        sc = OsDrvSvuStartShaveCC(&handler, (u32)&brthY0_start, "iii", ((u32)&inBuffer), ((u32)&outBuffer), use_builtins);
        if(sc)
            exit(sc);
        sc = OsDrvSvuWaitShaves(1, &handler, OS_DRV_SVU_WAIT_FOREVER, &running);
        if(sc)
            exit(sc);
        sc = OsDrvTimerGetElapsedTicks(&g_ticks, &passed_cycles);
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

    sc = OsDrvSvuOpenShave(&handler, SHAVE_NUMBER, OS_MYR_PROTECTION_SEM);
    if (sc == OS_MYR_DRV_SUCCESS)
    {

        // Test code with builtins
        sc = OsDrvTimerStartTicksCount(&g_ticks);
        if(sc)
            exit(sc);

        sc = OsDrvSvuResetShave(&handler);
        if(sc)
            exit(sc);
        sc = OsDrvSvuSetAbsoluteDefaultStack(&handler);
        if(sc)
            exit(sc);

        use_builtins = 1;
        sc = OsDrvSvuStartShaveCC(&handler, (u32)&brthY0_start, "iii", ((u32)&inBuffer), ((u32)&outBuffer), use_builtins);
        if(sc)
            exit(sc);
        sc = OsDrvSvuWaitShaves(1, &handler, OS_DRV_SVU_WAIT_FOREVER, &running);
        if(sc)
            exit(sc);
        sc = OsDrvTimerGetElapsedTicks(&g_ticks, &passed_cycles_bi);
        if(sc)
            exit(sc);
        sc = OsDrvSvuCloseShave(&handler);
        if(sc)
            exit(sc);
    }
    else {
        printf("[T2] cannot open shave %d\n", SHAVE_NUMBER);
    }


    printf("\nBrightness applied on Y plane, rest of planes copied.\n\n");
    printf("Without built-ins: %llu cycles \n", passed_cycles);
    printf("   With built-ins: %llu cycles \n", passed_cycles_bi);

    printf("\nTo check the image use:\n");
    printf("mdbg::savefile image_80x60_P420.yuv outputFrame 7200\n");

    exit(0);
}


