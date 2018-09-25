///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <mv_types.h>
#include <svuCommonShave.h>
#include <swcFrameTypes.h>
#include "swcCdma.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define MAX_LINE_WIDTH 1280

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
u8 bufferedInLine[MAX_LINE_WIDTH * 3];
u8 bufferedOutLine[MAX_LINE_WIDTH];
u8* input[3];
u8* output[1];
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

void sobel_asm(u8** in, u8** out, u32 width);
dmaTransactionList_t __attribute__((section(".cmx.cdmaDescriptors"))) task1, task2, task3, task4;

void Sobel(u8* inYplane, u8* outYplane, int width, int height)
{
    u8* inAddress;
    u8* outAddress;

    dmaTransactionList_t *ref1, *ref2, *ref3, *ref4;

    int line;

    input[0] = &bufferedInLine[0];
    input[1] = &bufferedInLine[1];
    input[2] = &bufferedInLine[2];
    output[0] = &bufferedOutLine[0];

    u32 id1 = dmaInitRequester(1); 

    for (line = 0; line < height; line++)
    {
        inAddress = (u8*) ((u32) inYplane + line * width);
        outAddress = (u8*) ((u32) outYplane + line * width);

        ref1 = dmaCreateTransaction(id1, &task1, inAddress, &bufferedInLine[0], width);
        ref2 = dmaCreateTransaction(id1, &task2, inAddress, &bufferedInLine[1], width);
        ref3 = dmaCreateTransaction(id1, &task3, inAddress, &bufferedInLine[2], width);
        dmaLinkTasks(ref1, 2, ref2, ref3);

        dmaStartListTask(ref1);
        dmaWaitTask(ref1);

        sobel_asm(input, output, width);

        /****************************************************
        * To keep this example simple, no padding was added to the image, therefore the first
        * and last columns have undefined values. In order for the output image to be the same
        * on every run, the two columns will be set to black.
        ****************************************************/
        bufferedOutLine[0] = 0x00;
        bufferedOutLine[width - 1] = 0x00;

        ref4 = dmaCreateTransaction(id1, &task4, &bufferedOutLine[0], outAddress, width);
        dmaStartListTask(ref4);
        dmaWaitTask(ref4);
    }

    return;
}

void CopyPlane(u8* inPlane, u8* outPlane, int width, int height)
{
    u8* inAddress;
    u8* outAddress;

    dmaTransactionList_t *ref1, *ref2;

    u32 id1 = dmaInitRequester(1); 

    int line;

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

void startSobel(frameBuffer* inFrame, frameBuffer* outFrame)
{
    Sobel(inFrame->p1, outFrame->p1, inFrame->spec.width, inFrame->spec.height);
    CopyPlane(inFrame->p2, outFrame->p2, inFrame->spec.width / 2, inFrame->spec.height / 2);
    CopyPlane(inFrame->p3, outFrame->p3, inFrame->spec.width / 2, inFrame->spec.height / 2);

    SHAVE_HALT;

    return;
}
