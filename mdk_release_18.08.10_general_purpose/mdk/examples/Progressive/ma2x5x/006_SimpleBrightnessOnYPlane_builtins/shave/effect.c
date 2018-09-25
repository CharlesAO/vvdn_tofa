///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Simple effect code
///

#include <mv_types.h>
#include <svuCommonShave.h>
#include <moviVectorUtils.h>
#include "effect.h"
#include "swcCdma.h"

#define MAX_LINE_WIDTH 80

#define BRIGHT_ADD  (50)
#define CLAMP_VAL   (255)

u8 bufferedInLine[MAX_LINE_WIDTH];
u8 bufferedOutLine[MAX_LINE_WIDTH];

uchar8 clampVal = {CLAMP_VAL, CLAMP_VAL, CLAMP_VAL, CLAMP_VAL, CLAMP_VAL, CLAMP_VAL, CLAMP_VAL, CLAMP_VAL};
uchar8 addVal = {BRIGHT_ADD, BRIGHT_ADD, BRIGHT_ADD, BRIGHT_ADD, BRIGHT_ADD, BRIGHT_ADD, BRIGHT_ADD, BRIGHT_ADD};

dmaTransactionList_t __attribute__((section(".cmx.cdmaDescriptors"))) task1, task2;

void BrightenLine_builtins(u8* inLine, u8* outLine, int width)
{
    uchar8* outv;
    uchar8* inpv;
    short8 sum;
    short8 clmp;
    int i;

    outv = (uchar8*)outLine;
    inpv = (uchar8*)inLine;

    for (i = 0; i < width; i += 8)
    {
        sum = __builtin_shave_vau_add_i16_rr(mvuConvert_short8(inpv[i >> 3]), mvuConvert_short8(addVal));
        clmp = __builtin_shave_cmu_clamp_i16_rr_short8(sum, mvuConvert_short8(clampVal));
        outv[i >> 3] = mvuConvert_uchar8(clmp);
    }

    return;
}

void Effect_builtins(u8* inYplane, u8* outYplane, int width, int height)
{
    u8* inAddress;
    u8* outAddress;
    int line;

    dmaTransactionList_t *ref1, *ref2; 

    u32 id1 = dmaInitRequester(1);

    for (line = 0; line < height; line++)
    {
        inAddress = (u8*)((u32)inYplane + line * width);
        outAddress = (u8*)((u32)outYplane + line * width);

        ref1 = dmaCreateTransaction(id1, &task1, inAddress, &bufferedInLine[0], width);
        dmaStartListTask(ref1);
        dmaWaitTask(ref1);

        BrightenLine_builtins(bufferedInLine, bufferedOutLine, width);

        ref2 = dmaCreateTransaction(id1, &task2, &bufferedOutLine[0], outAddress, width);
        dmaStartListTask(ref2);
        dmaWaitTask(ref2);

    }

    return;
}

void BrightenLine(u8* inLine, u8* outLine, int width)
{
    int i;
    int t;

    for (i = 0; i < width; i++)
    {
        t = inLine[i] + (BRIGHT_ADD);

        if (t > (CLAMP_VAL)) {
            t = (CLAMP_VAL);
        }

        outLine[i] = t;
    }

    return;
}

void Effect(u8* inYplane, u8* outYplane, int width, int height)
{
    u8* inAddress;
    u8* outAddress;
    int line;

    dmaTransactionList_t *ref1, *ref2; 

    u32 id1 = dmaInitRequester(1);

    for (line = 0; line < height; line++)
    {
        inAddress = (u8*)((u32)inYplane + line * width);
        outAddress = (u8*)((u32)outYplane + line * width);

        ref1 = dmaCreateTransaction(id1, &task1, inAddress, &bufferedInLine[0], width);
        dmaStartListTask(ref1);
        dmaWaitTask(ref1);

        BrightenLine(bufferedInLine, bufferedOutLine, width);

        ref2 = dmaCreateTransaction(id1, &task2, &bufferedOutLine[0], outAddress, width);
        dmaStartListTask(ref2);
        dmaWaitTask(ref2);

    }

    return;
}

void CopyPlane(u8* inPlane, u8* outPlane, int width, int height)
{
    u8* inAddress;
    u8* outAddress;

    dmaTransactionList_t *ref1, *ref2; 

    int line;

    u32 id1 = dmaInitRequester(1);

    for (line = 0; line < height; line++)
    {
        inAddress = (u8*) ((u32) inPlane + line * width);
        outAddress = (u8*) ((u32) outPlane + line * width);

        ref1 = dmaCreateTransaction(id1, &task1, inAddress, &bufferedInLine[0], width);
        ref2 = dmaCreateTransaction(id1, &task2, &bufferedInLine[0], outAddress, width);
        dmaLinkTasks(ref1, 1, ref2);
      
        dmaStartListTask(ref1);
        dmaWaitTask(ref1);

    }

    return;
}
