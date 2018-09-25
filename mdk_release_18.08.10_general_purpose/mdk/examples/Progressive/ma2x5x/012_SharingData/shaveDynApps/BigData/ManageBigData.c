///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Shave hello world source code
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <swcWhoAmI.h>
#include <svuCommonShave.h>
#include <swcCdma.h>

#define BIG_DATA_LENGTH 10240

// 2:  Source Specific #defines and types (typedef, enum, struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
dmaTransactionList_t __attribute__((section(".cmx.cdmaDescriptors"))) task1[CFG_NUM_SHAVES], task2[CFG_NUM_SHAVES];
u8 bufferedInLine[BIG_DATA_LENGTH];

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

void CopyBuffer(u8* inPlane, u8* outPlane, int width, int height)
{
    u8* inAddress;
    u8* outAddress;
    int line;
    int prio = 1;
    dmaTransactionList_t *ref1, *ref2;

    int id = (swcWhoAmI()-PROCESS_SHAVE0);

    u32 transId = dmaInitRequester(prio);

    for (line = 0; line < height; line++)
    {
        inAddress = (u8*) ((u32) inPlane + line * width);
        outAddress = (u8*) ((u32) outPlane + line * width);

        ref1 = dmaCreateTransaction(transId,
                                    &task1[id],
                                    inAddress,
                                    &bufferedInLine[0],
                                    width);

        ref2 = dmaCreateTransaction(transId,
                                    &task2[id],
                                    &bufferedInLine[0],
                                    outAddress,
                                    width);
        dmaLinkTasks(ref1, 1, ref2);
        dmaStartListTask(ref1);
        dmaWaitTask(ref1);
    }

    return;
}

int Entry( uint8_t * input, uint8_t * output, uint32_t width, uint32_t height)
{
     CopyBuffer(input, output, width, height);

    return 0;

}
