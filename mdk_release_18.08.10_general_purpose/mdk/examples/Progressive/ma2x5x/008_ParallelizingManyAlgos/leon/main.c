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
#include <OsDrvSvu.h>
#include <stdio.h>
#include <OsDrvTimer.h>
#include <DrvShaveL2Cache.h>
#include <DrvCpr.h>
#include "rtems_config.h"


// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define FRAME_WIDTH    80
#define FRAME_HEIGHT   60
#define LIST_LEN       1280

#define DDR_FRAMES __attribute__((section(".ddr.data")))

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern u8  inputFrame;
extern u32 brthYASM0_startBrightness;
extern u32 brthYASM1_startBrightness;
extern u32 sobel2_startSobel;

u32 entryPoints[SHAVES_USED] = {
        (u32)&brthYASM0_startBrightness,
        (u32)&brthYASM1_startBrightness,
        (u32)&sobel2_startSobel
};


// 4: Static Local Data
// ----------------------------------------------------------------------------
static u8 DDR_FRAMES outputFrame[FRAME_WIDTH * FRAME_HEIGHT * 3 / 2];
static frameBuffer __attribute__((section(".cmx.data"))) inBuffer[SHAVES_USED], outBuffer[SHAVES_USED];
static osDrvSvuHandler_t handler[SHAVES_USED];

static swcShaveUnit_t EffectShaves[SHAVES_USED] = {0, 1, 2 };
static tyTimeStamp  g_ticks;
static u64 passed_cycles;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------
void InitTestBuffers()
{
    int i;
    u32 sh_width;
    u32 sh_height;
    sh_width = FRAME_WIDTH;
    sh_height = FRAME_HEIGHT / SHAVES_USED;

    for (i = 0; i < SHAVES_USED; i++)
    {

        inBuffer[i].spec.width = sh_width;
        inBuffer[i].spec.height = sh_height;
        inBuffer[i].spec.stride = sh_width;
        inBuffer[i].spec.type = YUV420p;
        inBuffer[i].spec.bytesPP = 1;

        inBuffer[i].p1 = (u8*)((u32)&inputFrame + sh_width * sh_height * i);
        inBuffer[i].p2 = (u8*)((u32)&inputFrame + FRAME_WIDTH * FRAME_HEIGHT + (sh_width * sh_height / 4) * i);
        inBuffer[i].p3 = (u8*)((u32)&inputFrame + FRAME_WIDTH * FRAME_HEIGHT + FRAME_WIDTH * FRAME_HEIGHT / 4 + (sh_width * sh_height / 4) * i);

        outBuffer[i].spec.width = sh_width;
        outBuffer[i].spec.height = sh_height;
        outBuffer[i].spec.stride = sh_width;
        outBuffer[i].spec.type = YUV420p;
        outBuffer[i].spec.bytesPP = 1;

        outBuffer[i].p1 = (u8*)((u32)&outputFrame[0] + sh_width * sh_height * i);
        outBuffer[i].p2 = (u8*)((u32)&outputFrame[0] + FRAME_WIDTH * FRAME_HEIGHT + (sh_width * sh_height / 4) * i);
        outBuffer[i].p3 = (u8*)((u32)&outputFrame[0] + FRAME_WIDTH * FRAME_HEIGHT + FRAME_WIDTH * FRAME_HEIGHT / 4 + (sh_width * sh_height / 4) * i);
    }

    return;
}

void *POSIX_Init (void *args)
{
    UNUSED(args);

    int i;
    u32 running;
    s32 sc;

    sc = initClocksAndMemory();
    if(sc)
        exit(sc);
    printf ("RTEMS POSIX Started...\n");

    InitTestBuffers();

    sc = OsDrvSvuInit();
    if(sc)
        exit(sc);
    sc = OsDrvTimerInit();
    if(sc)
        exit(sc);
    sc = OsDrvTimerStartTicksCount(&g_ticks);
    if(sc)
        exit(sc);

    for (i = 0; i < SHAVES_USED; i++)
    {
        sc = OsDrvSvuOpenShave(&handler[i], EffectShaves[i], OS_MYR_PROTECTION_SEM);
        if (sc == OS_MYR_DRV_SUCCESS)
        {
            sc = OsDrvSvuResetShave(&handler[i]);
            if(sc)
                exit(sc);
            sc = OsDrvSvuSetAbsoluteDefaultStack(&handler[i]);
            if(sc)
                exit(sc);
            sc = OsDrvSvuStartShaveCC(&handler[i], entryPoints[i], "ii", (u32)&inBuffer[i], (u32)&outBuffer[i]);
            if(sc)
                exit(sc);
       }
        else {
            printf("cannot open shave %d\n", i);
            exit(sc);
        }
    }
    sc = OsDrvSvuWaitShaves(SHAVES_USED, handler, OS_DRV_SVU_WAIT_FOREVER, &running);
    if(sc)
        exit(sc);

    sc = OsDrvTimerGetElapsedTicks(&g_ticks, &passed_cycles);
    if(sc)
        exit(sc);

    for (i = 0; i < SHAVES_USED; i++)
    {
        sc = OsDrvSvuCloseShave(&handler[i]);
        if(sc)
            exit(sc);
    }
    printf("\nBrightness and Sobel effect applied.\n");
    printf("\nLeon executed %lu cycles \n",(u32)(passed_cycles));

    printf("\nTo check the result use:\n");
    printf("mdbg::savefile image_80x60_P420.yuv outputFrame 7200\n");

    exit(0);
}


