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
#include "effect.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define MAX_LINE_WIDTH 80
//#define (u8)__builtin_shave_cmu_clamp0_i32_rr(X, 255)
#define CLAMP(X) (u8)__builtin_shave_cmu_clamp0_i32_rr_int(X, 255)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
u8 bufferedInLine[MAX_LINE_WIDTH];
u8 bufferedOutLine[MAX_LINE_WIDTH];
void *handle;
u8 SHAVE_NUMBER[2] = {6, 7};
dmaTransactionList __attribute__((section(".cmx.cdmaDescriptors"))) transactionStruct;
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
    int line;

    dmaRequesterId reqId = dmaInitRequester(1);


    for (line = 0; line < height / 2; line++)
    {
        inAddress = (u8*)((u32)inYplane + line * width);
        outAddress = (u8*)((u32)outYplane + line * width);

        //Bring in the input line
        dmaCreateTransaction(reqId, &transactionStruct, (u8*)inAddress, (u8*)&bufferedInLine[0], width);
        dmaStartListTask(&transactionStruct);
        dmaWaitTask(&transactionStruct);

        DarkenLine(bufferedInLine, bufferedOutLine, width);

        //copy it into the output line
        dmaCreateTransaction(reqId, &transactionStruct, (u8*)&bufferedOutLine[0], (u8*)outAddress, width);
        dmaStartListTask(&transactionStruct);
        dmaWaitTask(&transactionStruct);
    }
    return;
}

void Brightness(u8* inYplane, u8* outYplane, int width, int height)
{
    u8* inAddress;
    u8* outAddress;
    int line;

    dmaRequesterId reqId = dmaInitRequester(1);

    for (line = height / 2; line < height; line++)
    {
        inAddress = (u8*)((u32)inYplane + line * width);
        outAddress = (u8*)((u32)outYplane + line * width);

        //Bring in the input line
        dmaCreateTransaction(reqId, &transactionStruct, (u8*)inAddress, (u8*)&bufferedInLine[0], width);
        dmaStartListTask(&transactionStruct);
        dmaWaitTask(&transactionStruct);

        BrightenLineAsm(bufferedInLine, bufferedOutLine, width);

        //copy it into the output line
        dmaCreateTransaction(reqId, &transactionStruct, (u8*)&bufferedOutLine[0], (u8*)outAddress, width);
        dmaStartListTask(&transactionStruct);
        dmaWaitTask(&transactionStruct);
    }
    return;
}

void CopyPlane(u8* inPlane, u8* outPlane, int width, int height)
{
    u8* inAddress;
    u8* outAddress;
    int line;

    dmaRequesterId reqId = dmaInitRequester(1);

    for (line = 0; line < height; line++)
    {
        inAddress = (u8*)((u32)inPlane + line * width);
        outAddress = (u8*)((u32)outPlane + line * width);
        //Bring in the input line
        dmaCreateTransaction(reqId, &transactionStruct, (u8*)inAddress, (u8*)&bufferedInLine[0], width);
        dmaStartListTask(&transactionStruct);
        dmaWaitTask(&transactionStruct);

        //copy it into the output line
        dmaCreateTransaction(reqId, &transactionStruct, (u8*)&bufferedInLine[0], (u8*)outAddress, width);
        dmaStartListTask(&transactionStruct);
        dmaWaitTask(&transactionStruct);
    }
    return;
}
