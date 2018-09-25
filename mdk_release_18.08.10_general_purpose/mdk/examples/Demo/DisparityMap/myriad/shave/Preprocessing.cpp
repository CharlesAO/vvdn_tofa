///
/// @file Preprocessing.cpp
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Module used to compute necessary stuff for rectification;
///

#include "Preprocessing.h"
#include <Defines.h>
#include <Sections.h>

#include <svuCommonShave.h>

#include <matrixInv3x3_fp32.h>
#include <convertYUV400ToYUV422.h>

#ifdef __PC__
#define matrixInvert			mvcvMatrixInv3x3_fp32
#define convertYUV400ToYUV422	mvcvConvertYUV400ToYUV422
#else
#define matrixInvert			mvcvMatrixInv3x3_fp32_asm
#define convertYUV400ToYUV422	mvcvConvertYUV400ToYUV422_asm
#endif

dmaTransactionList_t CMX_DMA lst1, lst2;
fp32 mBuffer[3*3];

extern "C" void Preprocessing(fp32* H, fp32* inverseH)
{
    fp32* inMatrix[9] = {&H[0], &H[3], &H[6], &H[1], &H[4], &H[7], &H[2], &H[5], &H[8]};
    fp32* outMatrix[9] = {&mBuffer[0], &mBuffer[3], &mBuffer[6], &mBuffer[1], &mBuffer[4], &mBuffer[7], &mBuffer[2], &mBuffer[5], &mBuffer[8]};

    matrixInvert(inMatrix, outMatrix);

    inverseH[0] = outMatrix[0][0];
    inverseH[1] = outMatrix[1][0];
    inverseH[2] = outMatrix[2][0];

    inverseH[3] = outMatrix[0][1];
    inverseH[4] = outMatrix[1][1];
    inverseH[5] = outMatrix[2][1];

    inverseH[6] = outMatrix[0][2];
    inverseH[7] = outMatrix[1][2];
    inverseH[8] = outMatrix[2][2];

    SHAVE_HALT;
}

extern "C" void Postprocessing(u8* input, u16* output, u32 width, u32 height)
{
    dmaTransactionList_t *ref1, *ref2;

    dmaRequesterId id1 = dmaInitRequester(3);

    u8 inLineBuf[MAX_WIDTH];
    u16 outLineBuf[MAX_WIDTH];

    u8* inLinePtr = &inLineBuf[0];
    u16* outLinePtr = &outLineBuf[0];

    for (u32 i = 0; i < height; i++)
    {
        ref1 = dmaCreateTransaction(id1, &lst1, (u8*)(input + i * width), (u8*)inLinePtr, width);
        dmaLinkTasks(ref1, 0);
	    dmaStartListTask(ref1);
	    dmaWaitTask(ref1);

        convertYUV400ToYUV422(inLinePtr, outLinePtr, width);

        ref2 = dmaCreateTransaction(id1, &lst2, (u8*)outLinePtr, (u8*)(output + i * width), width*2);
        dmaLinkTasks(ref2, 0);
	    dmaStartListTask(ref2);
	    dmaWaitTask(ref2);
    }
    SHAVE_HALT;
}
