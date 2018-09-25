///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Simple effect code
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <mv_types.h>
#include <svuCommonShave.h>
#include "effect.h"
#include "swcCdma.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define MAX_LINE_WIDTH 1280

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
u8 bufferedInLine[MAX_LINE_WIDTH];
u8 bufferedOutLine[MAX_LINE_WIDTH];
dmaTransactionList_t __attribute__((section(".cmx.cdmaDescriptors"))) task1, task2;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

void Brightness(u8* inPlane, u8* outPlane, int width, int height)
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
        dmaStartListTask(ref1);
        dmaWaitTask(ref1);

        //Apply effect
        BrightenLineAsm(bufferedInLine, bufferedOutLine, width);

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
