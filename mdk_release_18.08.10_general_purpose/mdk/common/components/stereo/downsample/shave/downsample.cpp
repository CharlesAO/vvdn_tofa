///
/// @file downsample.c
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief donwsample SHAVE application
///

#include <scCmxDma.h>
#include <svuCommonShave.h>

#include <Defines.h>
#include <pyrdown.h>
#include <string.h>
#define DMA_USAGE
#define BUF_LINES   (5 + 2)

#ifdef UNIT_TEST_MIDDLEBURY
#define mvcvPyrdown mvcvPyrdown
#else
#define mvcvPyrdown mvcvPyrdown_asm
#endif

static u8 inputLineBuffer[(MAX_WIDTH + 2*LINE_PADDING + MAX_PADDING_16)*BUF_LINES] ALIGNED(16);
static u8 outputDownsampleBuffer[MAX_WIDTH] ALIGNED(16);

static ScCmxDmaTransaction list1, list2;

static void addPadding(u8 *pointer, u32 width, u32 paddingSize);
static void rotateBuffers(u8* ptrs[]);

extern "C" void downsampleImage(u8* inImg, u8* outImg, u32 width, u32 height)
{
    const int kernelPadding = 2;
    const int kernelSize = 5;
    u8* bufferIn[BUF_LINES];
    u8* bufferOut[2];

    ScCmxDmaTransactionHnd hnd1, hnd2;
#ifdef DMA_USAGE
    ScCmxDmaInitialize(NULL);
#endif
    // Allocate needed memory for 7 lines
    for (u32 i = 0; i < BUF_LINES; i++)
    {
        bufferIn[i] = (u8*)inputLineBuffer + i * (width + 2 * kernelPadding);
    }

    bufferOut[0] = &outputDownsampleBuffer[0];

    // Dma copy first 5 lines
    u32 halfKernel = 2;
    for (u32 i = 0; i < kernelSize; i++)
    {
        bufferIn[i] += kernelPadding;
        if (i <= halfKernel)
        {
#ifdef DMA_USAGE
            ScCmxDmaCreateTransaction(&hnd1, &list1,
                            (u8*)inImg,
                            (u8*)bufferIn[i],
                            width);
            ScCmxDmaStartTransfer(&hnd1);
            ScCmxDmaWaitTransaction(&hnd1);
#else
            memcpy((u8*)bufferIn[i], (u8*)inImg, width);
#endif
        }
        else
        {
#ifdef DMA_USAGE
            ScCmxDmaCreateTransaction(&hnd1, &list1,
                            (u8*)(inImg + (i - halfKernel) * width),
                            (u8*)bufferIn[i],
                            width);
            ScCmxDmaStartTransfer(&hnd1);
            ScCmxDmaWaitTransaction(&hnd1);
#else
            memcpy((u8*)bufferIn[i], (u8*)(inImg + (i - halfKernel) * width), width);
#endif
        }

        // Add padding
        addPadding(bufferIn[i], width, kernelPadding);
    }

    u32 widthHalf = (u32)((width + 1) / 2);
    u32 offsetBuffer = halfKernel + 1;
    u32 it = 0;
    u32 line5, line6;
    bufferIn[5] += kernelPadding;
    bufferIn[6] += kernelPadding;

    for (u32 i = 0; i < height; i+=2)
    {
        if ((i + offsetBuffer + 1) >= height)
        {
            line5 = line6 = height - 1;
        }
        else
        {
            line5 = i + offsetBuffer;
            line6 = line5 + 1;
        }
#ifdef DMA_USAGE
        // Transfer next 2 lines
        ScCmxDmaCreateTransaction(&hnd1, &list1,
                (u8*)(inImg + line5 * width),
                (u8*)bufferIn[5],
                width);
        ScCmxDmaCreateTransaction(&hnd2, &list2,
                (u8*)(inImg + line6 * width),
                (u8*)bufferIn[6],
                width);
        ScCmxDmaLinkTransactions(&hnd1, 2, &hnd1, &hnd2);
        ScCmxDmaStartTransfer(&hnd1);
#else
        memcpy((u8*)bufferIn[5], (u8*)(inImg + line5 * width), width);
        memcpy((u8*)bufferIn[6], (u8*)(inImg + line6 * width), width);
#endif

        // Resize one image line
        mvcvPyrdown(bufferIn, &bufferOut[0], width);
#ifdef DMA_USAGE
        ScCmxDmaWaitTransaction(&hnd1);
#endif

        // Add padding and rotate buffers
        addPadding(bufferIn[5], width, kernelPadding);
        addPadding(bufferIn[6], width, kernelPadding);
        rotateBuffers(bufferIn);
#ifdef DMA_USAGE
        // Copy result to output buffer image
        ScCmxDmaCreateTransaction(&hnd1, &list1,
                (u8*)bufferOut[0],
                (u8*)(outImg + it * widthHalf),
                widthHalf);
        ScCmxDmaStartTransfer(&hnd1);
        ScCmxDmaWaitTransaction(&hnd1);
#else
        memcpy((u8*)(outImg + it * widthHalf), (u8*)bufferOut[0], widthHalf);
#endif
        it++;
    }
}

static void addPadding(u8 *pointer, u32 width, u32 paddingSize)
{
    u32 widthAux = width - 1;

    for (u32 i = 1; i <= paddingSize; i++)
    {
        *(pointer - i) = *pointer;
        *(pointer + widthAux + i) = *(pointer + widthAux);
    }
}

static void rotateBuffers(u8* ptrs[])
{
    // Keep the first pointers because they will be overwritten
    u8* tmp1 = ptrs[0];
    u8* tmp2 = ptrs[1];

    // Shift all pointers to the left location
    ptrs[0] = ptrs[2];
    ptrs[1] = ptrs[3];
    ptrs[2] = ptrs[4];
    ptrs[3] = ptrs[5];
    ptrs[4] = ptrs[6];

    ptrs[5] = tmp1;
    ptrs[6] = tmp2;
}
