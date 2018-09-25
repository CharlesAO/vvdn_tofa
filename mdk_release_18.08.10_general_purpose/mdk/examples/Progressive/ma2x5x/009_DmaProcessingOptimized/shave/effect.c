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
#include <swcCdma.h>
#include <swcWhoAmI.h>
#include "effect.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define MAX_LINE_WIDTH 80
#define CLAMP(X) (u8)__builtin_shave_cmu_clamp0_i32_rr_int(X, 255)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
u8 bufferedInLine[MAX_LINE_WIDTH * 2];
u8 bufferedOutLine[MAX_LINE_WIDTH];
dmaTransactionList_t __attribute__((section(".cmx.cdmaDescriptors"))) task1, task2, task3;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

void DarkenLine(u8* inLine, u8* outLine, int width)
{
    int i;
    for (i = 0; i < width; i++)
    {
        outLine[i] = CLAMP(inLine[i] - 20);
    }
    return;
}



void Darkness(u8* inYplane, u8* outYplane, int width, int height)
{
    u8* inAddress;
    u8* outAddress;
    int buffused = 0;
	int line;

    dmaTransactionList_t *ref1, *ref2, *ref3;

    // Initialize tasks handle
    u32 id1 = dmaInitRequester(1);

    inAddress = (u8*)((u32)inYplane);

    // //Bring in the first line for processing
    ref1 = dmaCreateTransaction(id1, &task1, inAddress, &bufferedInLine[0], width);
    dmaStartListTask(ref1);
    dmaWaitTask(ref1);

    // Update the buffused
    buffused = 1 - buffused;

    // Apply the darkness effect on the entire image
    for (line = 0; line < height; line++)
    {
    	// Using double buffering leads to different offsets in managing the input/output addresses
       	inAddress = (u8*)((u32)inYplane + (line + 1) * width);
        outAddress = (u8*)((u32)outYplane + line * width);

    	// //Bring in the input line
        ref2 = dmaCreateTransaction(id1, &task2, inAddress, &bufferedInLine[buffused * width], width);
        dmaStartListTask(ref2);

        // Update the buffused: the buffer in which a new data is brought is different than the one on which we apply our effect.
        buffused = 1 - buffused;

        // Apply the darkness effect on the previous line
        DarkenLine(bufferedInLine + buffused * width, bufferedOutLine, width);

    	// // Wait for the DMA to finish bringing the new  line.
        dmaWaitTask(ref2);

        ref3 = dmaCreateTransaction(id1, &task3,  &bufferedOutLine[0], outAddress, width);
        dmaStartListTask(ref3);
        dmaWaitTask(ref3);
    }

    return;
}


void CopyPlane(u8* inPlane, u8* outPlane, int width, int height)
{
    u8* inAddress;
    u8* outAddress;
    int line;
    dmaTransactionList_t *ref1, *ref2;
    
    u32 id1 = dmaInitRequester(1);

    for (line = 0; line < height; line++)
    {
        inAddress = (u8*)((u32)inPlane + line * width);
        outAddress = (u8*)((u32)outPlane + line * width);

        ref1 = dmaCreateTransaction(id1, &task1, inAddress, &bufferedInLine[0], width);
        ref2 = dmaCreateTransaction(id1, &task2, &bufferedInLine[0], outAddress, width);
        dmaLinkTasks(ref1, 1, ref2);
      
        dmaStartListTask(ref1);
        dmaWaitTask(ref1);
    }

    return;
}
