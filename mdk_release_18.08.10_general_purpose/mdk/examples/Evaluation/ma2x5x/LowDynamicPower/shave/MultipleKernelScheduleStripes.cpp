///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main shave file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mv_types.h"
#include "svuCommonShave.h"

#include "swcCdma.h"

#include "mvcv.h"

#include "kernel_config.h"
#include "kernel_buffers.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
//                in                out             1280            720              1              0
static void run(u8* outFrameBuffer, u8* inFrameBuffer, u32 frameWidth, u32 frameHeight, u32 nrShaves,
                u32 sliceIndex)
{
    u32 sliceHeight  = ((sliceIndex + 1) == nrShaves)   ? (frameHeight - sliceIndex * (frameHeight / nrShaves))
                                                        : (frameHeight / nrShaves);

    u8  lineIndex = sliceIndex * (frameHeight/nrShaves);
    u8* inAddress   = inFrameBuffer + lineIndex * frameWidth;
    u8* outAddress  = outFrameBuffer + lineIndex * frameWidth;

    u32 idx;

    static dmaTransactionList_t CMX_DMA task1, task2, task3;
    static dmaTransactionList_t CMX_DMA *ref1, *ref2, *ref3; 

    initPointers(in7LinesAddress1, in7LinesBuffer1, LINES_NUM, COLUMN_PADDING);
    initPointers(in7LinesAddress2, in7LinesBuffer2, LINES_NUM, COLUMN_PADDING);
    initPointers(in7LinesAddress3, in7LinesBuffer3, LINES_NUM, COLUMN_PADDING);
    initPointers(outLinesAddress, outLinesBuffer, 2, 0);

    u32 id1 = dmaInitRequester(0);

    if ((lineIndex - UPPER_LINE_PADDING) < 0 )
    {
        for (idx = 0; idx < LINE_PADDING + 1; idx++)
        {
            ref1 = dmaCreateTransaction(id1, &task1, inAddress, in7LinesAddress1[idx], frameWidth);
            dmaStartListTask(ref1);
            dmaWaitTask(ref1);
        }
    }
    else
    {
        u8 *tempAddress;

        //for padding, a number of UPPER_LINE_PADDING lines are needed from the previous image slice
        for (idx = 0; idx < UPPER_LINE_PADDING; idx++)
        {
            tempAddress = inAddress - frameWidth * (UPPER_LINE_PADDING - idx);
            ref1 = dmaCreateTransaction(id1, &task1, tempAddress , in7LinesAddress1[idx], frameWidth);
            dmaStartListTask(ref1);
            dmaWaitTask(ref1);
        }
        tempAddress = inAddress;

        //bring in the internal cmx buffer the following
        //(LINE_PADDING - UPPER_LINE_PADDING) input lines needed for the conv7x7
        for (idx = UPPER_LINE_PADDING; idx < LINE_PADDING; idx++)
        {
            ref1 = dmaCreateTransaction(id1, &task1, inAddress, in7LinesAddress1[idx], frameWidth);
            dmaStartListTask(ref1);
            dmaWaitTask(ref1);
            tempAddress += frameWidth * idx;
        }
    }

    for (idx = 0; idx < LINE_PADDING + 1; idx++)
    {
        mvcvConvolution7x7_asm(in7LinesAddress1, in7LinesAddress2, (half*)mask7, frameWidth);
        rotatePointers(in7LinesAddress2, LINES_NUM);
    }

    for (idx = 0; idx < LINE_PADDING + 1; idx++)
    {
        mvcvConvolution7x7_asm(in7LinesAddress2, in7LinesAddress3, (half*)mask7, frameWidth);
        rotatePointers(in7LinesAddress3, LINES_NUM);
    }

    mvcvConvolution7x7_asm(in7LinesAddress3, &outLinesAddress[0], (half*)mask7, frameWidth);

    rotatePointers(in7LinesAddress1, LINES_NUM);
    rotatePointers(in7LinesAddress2, LINES_NUM);
    rotatePointers(in7LinesAddress3, LINES_NUM);
    rotatePointers(outLinesAddress, 2);

    // Process all lines for this Shave
    for (idx = UPPER_LINE_PADDING + 1; idx < sliceHeight + 14; idx++, inAddress += frameWidth)
    {
        ref2 = dmaCreateTransaction(id1, &task2, inAddress, in7LinesAddress1[0], frameWidth);
        ref3 = dmaCreateTransaction(id1, &task3, outLinesAddress[1], outAddress, frameWidth);
        dmaLinkTasks(ref2, 1, ref3);
        dmaStartListTask(ref2);

        outAddress += frameWidth;

        // Chain filters
        mvcvConvolution7x7_asm(in7LinesAddress1, in7LinesAddress2, (half*)mask7, frameWidth);
        mvcvConvolution7x7_asm(in7LinesAddress2, in7LinesAddress3, (half*)mask7, frameWidth);
        mvcvConvolution7x7_asm(in7LinesAddress3, &outLinesAddress[0], (half*)mask7, frameWidth);

        dmaWaitTask(ref2);

        rotatePointers(in7LinesAddress1, LINES_NUM);
        rotatePointers(in7LinesAddress2, LINES_NUM);
        rotatePointers(in7LinesAddress3, LINES_NUM);
        rotatePointers(outLinesAddress, 2);
    }
    
    ref3 = dmaCreateTransaction(id1, &task3, outLinesAddress[1], outAddress, frameWidth);
    dmaStartListTask(ref3);
    dmaWaitTask(ref3);
}

extern "C" void AppStart3KernelsStripes(u8* outFrameBuffer, u8* inFrameBuffer, u32 frameWidth,
                                        u32 frameHeight, u32 nrShaves, u32 sliceIndex, u32 framesToRun)
{
    for (u32 frames = 0; frames < framesToRun; frames++)
    {
        run(outFrameBuffer, inFrameBuffer, frameWidth, frameHeight, nrShaves, sliceIndex);
    }

    // Stopping the shave
    exit(1);
}
