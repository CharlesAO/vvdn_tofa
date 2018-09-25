///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

// 1: Includes
// ----------------------------------------------------------------------------
#include "shaveStartJpeg.h"
#include "dct16bit.h"
#include <swcWhoAmI.h>
#include <swcCdma.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
dmaTransactionList_t task1, task2, task3;
volatile dmaTransactionList_t *ref1, *ref2, *ref3;

// Missing header for mvcvjpegGetBlockY420_asm
void mvcvjpegGetBlockY420_asm(u32 mode, u8 *imginfo_Y, u32 imginfo_width, u32 imgspec_x,u32 imgspec_y, short imgspecY[8][8]);

// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

void start(frameBuffer *imginfo, shaveinfotype* shaveInfo, u8 *output,
          u32 *outputSize, u32 imgWidth, u32 imgHeight, formatSpecific *formatS)
{
    unsigned char yBuffer[formatS->lumaBufferSize];
    unsigned char uBuffer[formatS->chromaBufferSize];
    unsigned char vBuffer[formatS->chromaBufferSize];

    u32 huffOutputPos = 0;
    frameBuffer imginfoLocal;
    imginfoLocal.p1 = yBuffer;
    imginfoLocal.p2 = uBuffer;
    imginfoLocal.p3 = vBuffer;
    imginfoLocal.spec.height = imginfo->spec.height;
    imginfoLocal.spec.width = imginfo->spec.width;

    dmaRequesterId dmaId = dmaInitRequester(1);
    ref1 = dmaCreateTransactionSrcStride(dmaId, &task1, imginfo->p1, imginfoLocal.p1,
           imginfoLocal.spec.width * (imginfoLocal.spec.height),
           imginfoLocal.spec.width, imgWidth);
    ref2 = dmaCreateTransactionSrcStride(dmaId, &task2, imginfo->p2, imginfoLocal.p2,
           imginfoLocal.spec.width / formatS->getBlockCoefX * (imginfoLocal.spec.height / formatS->getBlockCoefY),
           imginfoLocal.spec.width / formatS->getBlockCoefX, imgWidth / formatS->getBlockCoefX);
    ref3 = dmaCreateTransactionSrcStride(dmaId, &task3, imginfo->p3, imginfoLocal.p3,
           imginfoLocal.spec.width / formatS->getBlockCoefX * (imginfoLocal.spec.height / formatS->getBlockCoefY),
           imginfoLocal.spec.width / formatS->getBlockCoefX, imgWidth / formatS->getBlockCoefX);

    dmaLinkTasks((dmaTransactionList_t *)ref1, 2, ref2, ref3);
    dmaStartListTask((dmaTransactionList_t *)ref1);

    huffman_ctx[0].dc = 0;
    huffman_ctx[1].dc = 0;
    huffman_ctx[2].dc = 0;

    bitbuffer_t extraBits;
    extraBits.buf = 0;
    extraBits.n = 0;
    u32 yl = (imginfoLocal.spec.height) - (formatS->blockH - 1);
    u32 xl = (imginfoLocal.spec.width) - (formatS->blockW - 1);
    imgspectype imgspec;

    imgspec.x = xl & formatS->maskX;
    if (xl & ~formatS->maskX)
        imgspec.x += formatS->blockW;

    imgspec.y = yl & formatS->maskY;
    if (yl & ~formatS->maskY)
        imgspec.y += formatS->blockH;

    u32 macroBlocksNo = (imgspec.x * imgspec.y) / formatS->blockSize;

    if (shaveInfo->first)
    {
        huffman_start((imgHeight & formatS->maskY),
                      ((imgWidth) & formatS->maskX), output,
                      &huffOutputPos, formatS->subsampling);
        start_section(output, &huffOutputPos, macroBlocksNo);
    }

    dmaWaitTask((dmaTransactionList_t *)ref1);

    for (imgspec.y = 0; imgspec.y < yl; imgspec.y += formatS->blockH)
    {
        for (imgspec.x = 0; imgspec.x < xl; imgspec.x += formatS->blockW)
        {
            //Y0
            mvcvjpegGetBlockY420_asm(0, imginfoLocal.p1, imginfoLocal.spec.width,
                                     imgspec.x, imgspec.y, imgspec.Y[0]);
            mvcvDCT16bit_asm(imgspec.Y[0], imgspec.Y[0]);
            mvcvHuffman_encode_asm(HUFFMAN_CTX_Y, (const short *)imgspec.Y[0], output,
                                   &huffOutputPos, &extraBits, SHAVE_OUTPUT_BUFFER_SIZE);

            if (formatS->lumaValuesPerChroma >= 2)
            {
                //Y1
                mvcvjpegGetBlockY420_asm(1, imginfoLocal.p1, imginfoLocal.spec.width,
                                         imgspec.x, imgspec.y, imgspec.Y[1]);
                mvcvDCT16bit_asm(imgspec.Y[1], imgspec.Y[1]);
                mvcvHuffman_encode_asm(HUFFMAN_CTX_Y, (const short *)imgspec.Y[1], output,
                                       &huffOutputPos, &extraBits,
                                       SHAVE_OUTPUT_BUFFER_SIZE);
            }

            if(formatS->lumaValuesPerChroma == 4)
            {
                //Y2
                mvcvjpegGetBlockY420_asm(2, imginfoLocal.p1, imginfoLocal.spec.width,
                                         imgspec.x, imgspec.y, imgspec.Y[2]);
                mvcvDCT16bit_asm(imgspec.Y[2], imgspec.Y[2]);
                mvcvHuffman_encode_asm(HUFFMAN_CTX_Y, (const short *)imgspec.Y[2], output,
                                       &huffOutputPos, &extraBits,
                                       SHAVE_OUTPUT_BUFFER_SIZE);
                //Y3
                mvcvjpegGetBlockY420_asm(3, imginfoLocal.p1, imginfoLocal.spec.width,
                                         imgspec.x, imgspec.y, imgspec.Y[3]);
                mvcvDCT16bit_asm(imgspec.Y[3], imgspec.Y[3]);
                mvcvHuffman_encode_asm(HUFFMAN_CTX_Y, (const short *)imgspec.Y[3], output,
                                       &huffOutputPos, &extraBits,
                                       SHAVE_OUTPUT_BUFFER_SIZE);
            }

            //U0
            mvcvjpegGetBlockY420_asm(0, imginfoLocal.p2,
                                     imginfoLocal.spec.width / formatS->getBlockCoefX,
                                     imgspec.x / formatS->getBlockCoefX,
                                     imgspec.y / formatS->getBlockCoefY, imgspec.U);
            mvcvDCT16bit_asm(imgspec.U, imgspec.U);
            mvcvHuffman_encode_asm(HUFFMAN_CTX_Cb, (const short *)imgspec.U, output,
                           &huffOutputPos, &extraBits, SHAVE_OUTPUT_BUFFER_SIZE);

            //V0
            mvcvjpegGetBlockY420_asm(0, imginfoLocal.p3,
                                     imginfoLocal.spec.width / formatS->getBlockCoefX,
                                     imgspec.x / formatS->getBlockCoefX,
                                     imgspec.y / formatS->getBlockCoefY, imgspec.V);
            mvcvDCT16bit_asm(imgspec.V, imgspec.V);
            mvcvHuffman_encode_asm(HUFFMAN_CTX_Cr, (const short *)imgspec.V, output,
                           &huffOutputPos, &extraBits, SHAVE_OUTPUT_BUFFER_SIZE);
        }
    }

    if (shaveInfo->last)
        huffman_stop(output, &huffOutputPos, &extraBits);
    else
        reset_interval(output, &huffOutputPos, shaveInfo->restartMarker,
                       &extraBits);
    *outputSize = huffOutputPos;

    SHAVE_HALT;
}
