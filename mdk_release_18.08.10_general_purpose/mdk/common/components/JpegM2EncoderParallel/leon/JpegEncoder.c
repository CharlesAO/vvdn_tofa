///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     JPEG Encoding Functions.
///
/// This is the implementation of a simple JPEG library implementing JPEG Encoding operations.
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <mv_types.h>
#include <DrvShaveL2Cache.h>
#include <swcShaveLoader.h>
#include <DrvTimer.h>
#include <DrvLeonL2C.h>
#include <DrvMutex.h>
#include <DrvSvu.h>
#include <DrvIcb.h>
#include <DrvCmxDma.h>

#include "JpegEncoderApi.h"
#include "jpegDefines.h"

extern u32 JpegM2EncoderParallel0_start;
extern u32 JpegM2EncoderParallel1_start;
extern u32 JpegM2EncoderParallel2_start;
extern u32 JpegM2EncoderParallel3_start;
extern u32 JpegM2EncoderParallel4_start;
extern u32 JpegM2EncoderParallel5_start;
extern u32 JpegM2EncoderParallel6_start;
extern u32 JpegM2EncoderParallel7_start;
extern u32 JpegM2EncoderParallel8_start;
extern u32 JpegM2EncoderParallel9_start;
extern u32 JpegM2EncoderParallel10_start;
extern u32 JpegM2EncoderParallel11_start;

fp startShave[SHAVE_MAX_NO] =
{
    &JpegM2EncoderParallel0_start,
    &JpegM2EncoderParallel1_start,
    &JpegM2EncoderParallel2_start,
    &JpegM2EncoderParallel3_start,
    &JpegM2EncoderParallel4_start,
    &JpegM2EncoderParallel5_start,
    &JpegM2EncoderParallel6_start,
    &JpegM2EncoderParallel7_start,
    &JpegM2EncoderParallel8_start,
    &JpegM2EncoderParallel9_start,
    &JpegM2EncoderParallel10_start,
    &JpegM2EncoderParallel11_start
};

// Shave L2 Cache partition numbers
int partNo[SHAVE_MAX_NO] = {
    PARTITION_0,
    PARTITION_1,
    PARTITION_2,
    PARTITION_3,
    PARTITION_4,
    PARTITION_5,
    PARTITION_6,
    PARTITION_7,
    PARTITION_0, //temporary limited to 8 partitions
    PARTITION_1,
    PARTITION_2,
    PARTITION_3
};

dmaTransactionList_t __attribute__((section(".cmx.cdmaDescriptors"))) tasks[SHAVE_MAX_NO * 2];
volatile dmaTransactionList_t *ref;
dmaTransactionList_t  *refs[SHAVE_MAX_NO * 2];
volatile jpegCMXoutput outputInfo[SHAVE_MAX_NO * 2];
volatile int shavesStatus[SHAVE_MAX_NO];
volatile int shaveDone;
volatile int lastSliceStarted;
volatile int lastSliceCopiedToDDR;
volatile int outputCount;

frameBuffer gimginfoPtr[SHAVE_MAX_NO];
formatSpecific formatS;
shaveinfotype shaveInfo;
shaveinfotype shaveInfoArray[SHAVE_MAX_NO];

formatSpecific getFormatSpecificParams(int jpegFormat);

void handlerForAllShaves(int shv);
int getNextIndexToCopyToDDR();
int getFreeOutputIndex();
void setOutputIndexForCurrentShave(int shv);

u32 getFreeShave(int shaveNo);
u32 getDoneShave(int shaveNo);

void dmaHandler(dmaTransactionList* ListPtr,  void *userContext);

void initOutputStruct()
{
    int i;
    for(i = 0; i < SHAVE_MAX_NO * 2; i++)
    {
        outputInfo[i].done = 0;
        outputInfo[i].sliceNo = -1;
        outputInfo[i].sliceSize = -1;
        outputInfo[i].shaveNo = -1;
        outputInfo[i].used = 0;
    }
}

// The JPEG encoding algorithm
u32 JPEG_encode(frameBuffer imgInfo, u8 *output, u32 shaveNo, u32 inBuffSizeShave, int jpegFormat)
{
    shaveDone = 0;
    lastSliceStarted = 0;
    lastSliceCopiedToDDR = -1;
    outputCount = 0;
    shaveInfo.inputBuffSizePerShave = inBuffSizeShave;

    initOutputStruct();

    shaveInfo.shavesNo = shaveNo;
    shaveInfo.first = 1;
    shaveInfo.last = 0;
    u32 lastShvBlocks = 0;
    u32 runsNo = 0;
    u32 lastShv;
    u32 shv;

    formatS = getFormatSpecificParams(jpegFormat);

    u32 x, y;
    u32 yl = (imgInfo.spec.height) - (formatS.blockH - 1);
    u32 xl = (imgInfo.spec.width) - (formatS.blockW - 1);

    x = xl &  formatS.maskX;
    if (xl & ~formatS.maskX)
        x += formatS.blockW;

    y = yl & formatS.maskY;
    if (yl & ~formatS.maskY)
        y += formatS.blockH ;

    int blocksNo = (x * y) / formatS.blockSize;

    int totalBlocks = blocksNo;
    u32 blocksPerChunk = imgInfo.spec.width / formatS.blockW;
    u32 blocksProcPerShv = 0;
    u32 maxBlocksPerShv = formatS.maxBlocksPerShv;
    u32 countBlocks = 0;

    if((blocksPerChunk > maxBlocksPerShv) || ((u32)totalBlocks < (shaveNo * blocksPerChunk)))
    {
        while (1)
        {
            countBlocks++;
            if ((maxBlocksPerShv < countBlocks) || ((u32)totalBlocks < (shaveNo * countBlocks)))
                break;

            if ((blocksPerChunk % countBlocks) == 0)
                blocksProcPerShv = countBlocks;
        }
    }
    else
    {
        while(1)
        {
            if ((maxBlocksPerShv < blocksProcPerShv + blocksPerChunk) ||
                ((u32)totalBlocks < (shaveNo * blocksProcPerShv  + blocksPerChunk)))
                break;
            blocksProcPerShv += blocksPerChunk;
        }
    }

    while (1)
    {
        runsNo++;
        lastShv = 0;

        for (shv = 0; shv < shaveNo; shv++)
        {
            if ((u32)totalBlocks < blocksProcPerShv)
                lastShvBlocks = totalBlocks;

            totalBlocks -= blocksProcPerShv;
            if (totalBlocks < 0)
                break;

            lastShv++;
        }

        if (totalBlocks < 0)
            break;
    }
    u32 lastRun = shaveNo * (runsNo-1) + lastShv + 1;

    u32 widthPerShv = blocksProcPerShv * formatS.blockW > imgInfo.spec.width ?
                      imgInfo.spec.width : blocksProcPerShv * formatS.blockW;
    u32 heightPerShv = ((blocksProcPerShv - 1) / blocksPerChunk) * formatS.blockH + formatS.blockH;

    u32 widthLastShv = lastShvBlocks * formatS.blockW > imgInfo.spec.width ?
                       imgInfo.spec.width : lastShvBlocks * formatS.blockW;
    u32 heightLastShv = ((lastShvBlocks - 1) / blocksPerChunk) * formatS.blockH + formatS.blockH;

    if(lastShvBlocks == 0)
    {
        lastRun--;
        widthLastShv = widthPerShv;
        heightLastShv = heightPerShv;
    }

    for (shv = 0; shv < shaveNo; shv++)
    {
        gimginfoPtr[shv].spec.width = widthPerShv;
        gimginfoPtr[shv].spec.height = heightPerShv;

        u32 tmpX = (widthPerShv * lastSliceStarted) % imgInfo.spec.width;
        u32 tmpY = ((widthPerShv * lastSliceStarted) / imgInfo.spec.width) * heightPerShv;

        u32 lumaOffset = tmpY * imgInfo.spec.width + tmpX;
        u32 chromaOffset = (tmpY / formatS.getBlockCoefY) *
                           (imgInfo.spec.width / formatS.getBlockCoefX)
                           + tmpX / formatS.getBlockCoefX;

        gimginfoPtr[shv].p1 = imgInfo.p1 + lumaOffset;
        gimginfoPtr[shv].p2 = imgInfo.p2 + chromaOffset;
        gimginfoPtr[shv].p3 = imgInfo.p3 + chromaOffset;

        shaveInfo.restartMarker = 0xFFD0 + (lastSliceStarted % MAX_RST_MARKERS);
        shaveInfo.currentShave = lastSliceStarted;

        swcResetShave(shv);
        swcSetAbsoluteDefaultStack(shv);
        DrvLL2CFlushOpOnAllLines(LL2C_OPERATION_WRITE_BACK, 0);
        /*Copy shaveInfo into a local array location to allow changing the original*/
        shaveInfoArray[shv]=shaveInfo;
        swcStartShaveAsyncCC(shv, (u32) startShave[shv], (irq_handler)&handlerForAllShaves, "iiiiiii",
                             &gimginfoPtr[shv], &shaveInfoArray[shv], outputInfo[shv].outCMXdata,
                             &outputInfo[shv].sliceSize, imgInfo.spec.width,
                             imgInfo.spec.height, &formatS);

        shavesStatus[shv] = JPG_SHAVE_BUSY;
        outputInfo[shv].sliceNo = lastSliceStarted;
        outputInfo[shv].shaveNo = shv;

        lastSliceStarted++;
        outputInfo[shv].used = 1;
        shaveInfo.first = 0;
    }

    dmaRequesterId dmaId = DrvCmxDmaInitRequesterOnAgent(1, 0);
    while (lastSliceCopiedToDDR  < (int)(lastRun-1))
    {
        if(shaveDone)
        {
            shv = getDoneShave(shaveNo);
            if(shv != INDEX_NOT_FOUND)
            {
                //from handler
                swcResetShave(shv);
                DrvShaveL2CachePartitionFlushAndInvalidate(partNo[shv]);
                setOutputIndexForCurrentShave(shv);

                shavesStatus[shv] = JPG_SHAVE_IDLE;

                DrvIcbDisableIrq(shv + IRQ_SVE_0);
                DrvIcbIrqClear(shv + IRQ_SVE_0);
                DrvIcbIrqUnsetup(shv + IRQ_SVE_0);
            }

            // Disable interrupts while we update shaveDone
            int pil = swcLeonSetPIL(15);
            shaveDone--;
            swcLeonSetPIL(pil);
        }
        u32 outputIndex;
        u32 outputIndexToCopy;

        // Check if there is any output chunk ready to be copied to DDR
        outputIndexToCopy = getNextIndexToCopyToDDR();
        if (outputIndexToCopy != INDEX_NOT_FOUND)
        {
            lastSliceCopiedToDDR++;
            u8 *uncachedOutputAddr;
            uncachedOutputAddr = (u8*)((intptr_t)outputInfo[outputIndexToCopy].outCMXdata | 0x08000000);
            u32 chunkSize =  swcLeonReadNoCacheI32(&outputInfo[outputIndexToCopy].sliceSize);
            refs[outputIndexToCopy] = DrvCmxDmaCreateTransaction(dmaId, &tasks[outputIndexToCopy], uncachedOutputAddr, output + outputCount, chunkSize);
            tasks[outputIndexToCopy].linkAddress = NULL;
            outputCount += chunkSize;
            DrvCmxDmaStartTaskAsync(refs[outputIndexToCopy], &dmaHandler, (void*)&outputInfo[outputIndexToCopy]);
        }

        // Check if there is another chunk to process
        if ((u32)lastSliceStarted < lastRun)
        {
            outputIndex = getFreeOutputIndex();

            // If there is a free slot in the outputInfo proceed with
            // check if any shave is free; if yes start preparing data and start shv
            if (outputIndex != INDEX_NOT_FOUND)
            {
                shv = getFreeShave(shaveNo);
                if(shv == JPG_NO_FREE_SHAVE)
                    continue;
                outputInfo[outputIndex].used = 1;
                outputInfo[outputIndex].sliceNo = lastSliceStarted;
                outputInfo[outputIndex].shaveNo = shv;
                outputInfo[outputIndex].done = 0;

                if ((u32)lastSliceStarted == (lastRun-1))
                {
                    shaveInfo.last = 1;
                    gimginfoPtr[shv].spec.width = widthLastShv;
                    gimginfoPtr[shv].spec.height = heightLastShv;
                }
                else
                {
                    gimginfoPtr[shv].spec.width = widthPerShv;
                    gimginfoPtr[shv].spec.height = heightPerShv;
                }

                u32 tmpX = (widthPerShv * lastSliceStarted) % imgInfo.spec.width;
                u32 tmpY = ((widthPerShv * lastSliceStarted) / imgInfo.spec.width) * heightPerShv;

                u32 lumaOffset = tmpY * imgInfo.spec.width + tmpX;
                u32 chromaOffset = (tmpY / formatS.getBlockCoefY) *
                                (imgInfo.spec.width / formatS.getBlockCoefX)
                                + tmpX / formatS.getBlockCoefX;

                gimginfoPtr[shv].p1 = imgInfo.p1 + lumaOffset;
                gimginfoPtr[shv].p2 = imgInfo.p2 + chromaOffset;
                gimginfoPtr[shv].p3 = imgInfo.p3 + chromaOffset;


                shaveInfo.restartMarker = 0xFFD0 + (lastSliceStarted % MAX_RST_MARKERS);
                shaveInfo.currentShave = shv;

                shaveInfoArray[shv]=shaveInfo;
                swcSetAbsoluteDefaultStack(shv);
                DrvLL2CFlushOpOnAllLines(LL2C_OPERATION_WRITE_BACK, 0);
                swcStartShaveAsyncCC(shv, (u32) startShave[shv],
                                     (irq_handler)&handlerForAllShaves, "iiiiiii",
                                     &gimginfoPtr[shv],
                                     &shaveInfoArray[shv],
                                     outputInfo[outputIndex].outCMXdata,
                                     &outputInfo[outputIndex].sliceSize,
                                     imgInfo.spec.width,
                                     imgInfo.spec.height,
                                     &formatS);
                lastSliceStarted++;
            }
        }
    }

    DrvLL2CFlushOpOnAllLines(LL2C_OPERATION_WRITE_BACK, 0);
    return outputCount;
}

formatSpecific getFormatSpecificParams(int jpegFormat)
{
    formatSpecific formatS;

    if (jpegFormat == JPEG_420_PLANAR)
    {
        formatS.maskX = 0xfffffff0;
        formatS.maskY = 0xfffffff0;
        formatS.blockW = 16;
        formatS.blockH = 16;
        formatS.lumaBufferSize = shaveInfo.inputBuffSizePerShave / 6 * 4;
        formatS.chromaBufferSize = shaveInfo.inputBuffSizePerShave/ 6;
        formatS.subsampling = 0x22;
        formatS.getBlockCoefX = 2;
        formatS.getBlockCoefY = 2;
        formatS.lumaValuesPerChroma = 4;
    }
    if (jpegFormat == JPEG_422_PLANAR)
    {
        formatS.maskX = 0xfffffff0;
        formatS.maskY = 0xfffffff8;
        formatS.blockW = 16;
        formatS.blockH = 8;
        formatS.lumaBufferSize = shaveInfo.inputBuffSizePerShave / 8 * 4;
        formatS.chromaBufferSize = shaveInfo.inputBuffSizePerShave / 8 * 2;
        formatS.subsampling = 0x21;
        formatS.getBlockCoefX = 2;
        formatS.getBlockCoefY = 1;
        formatS.lumaValuesPerChroma = 2;
    }
    if (jpegFormat == JPEG_444_PLANAR)
    {
        formatS.maskX = 0xfffffff8;
        formatS.maskY = 0xfffffff8;
        formatS.blockW = 8;
        formatS.blockH = 8;
        formatS.lumaBufferSize = shaveInfo.inputBuffSizePerShave / 12 * 4;
        formatS.chromaBufferSize = shaveInfo.inputBuffSizePerShave / 12 * 4;
        formatS.subsampling = 0x11;
        formatS.getBlockCoefX = 1;
        formatS.getBlockCoefY = 1;
        formatS.lumaValuesPerChroma = 1;
    }
    if((jpegFormat == JPEG_444_PLANAR) || (jpegFormat == JPEG_422_PLANAR) || (jpegFormat == JPEG_420_PLANAR)) {

        formatS.blockSize = formatS.blockW * formatS.blockH;
        formatS.maxBlocksPerShv = formatS.lumaBufferSize / formatS.blockSize;
    }
    else { //wrong format
        assert((jpegFormat == JPEG_444_PLANAR) || (jpegFormat == JPEG_422_PLANAR) || (jpegFormat == JPEG_420_PLANAR));
    }

    return formatS;
}

u32 getDoneShave(int shaveNo)
{
    int i;
    for(i = 0; i < shaveNo; i++)
        if(shavesStatus[i] == JPG_SHAVE_STOPPED)
            return i;
    return INDEX_NOT_FOUND;
}

u32 getFreeShave(int shaveNo)
{
    int i = 0;
    for(i = 0; i < shaveNo; i++)
        if(shavesStatus[i] == 0)
        {
            shavesStatus[i] = JPG_SHAVE_BUSY;
            return i;
        }
    return JPG_NO_FREE_SHAVE;
}

int getNextIndexToCopyToDDR()
{
    int i;
    for (i = 0; i < SHAVE_MAX_NO * 2; i++)
        if (outputInfo[i].used && outputInfo[i].done && (outputInfo[i].sliceNo == (lastSliceCopiedToDDR+1)))
            return i;

    return INDEX_NOT_FOUND;
}

int getFreeOutputIndex()
{
    int i;
    for (i = 0; i < SHAVE_MAX_NO * 2; i++)
        if ((outputInfo[i].used == 0) && (outputInfo[i].done == 0))
            return i;

    return INDEX_NOT_FOUND;
}

void setOutputIndexForCurrentShave(int shv)
{
    int i;
    for (i = 0; i < SHAVE_MAX_NO * 2; i++)
        if (outputInfo[i].shaveNo == shv)
            outputInfo[i].done = 1;
}

void handlerForAllShaves(int irqSource)
{
    int shv = irqSource - IRQ_SVE_0;
    // Set STOP bit in control register
    SET_REG_WORD(SVU_CTRL_ADDR[shv] + SLC_OFFSET_SVU + SVU_OCR, OCR_STOP_GO | OCR_TRACE_ENABLE);
    // Clear any interrupts from previous test
    SET_REG_WORD(SVU_CTRL_ADDR[shv] + SLC_OFFSET_SVU + SVU_IRR, 0xFFFFFFFF);
    shavesStatus[shv] = JPG_SHAVE_STOPPED;
    ++shaveDone;
}

void dmaHandler(dmaTransactionList* ListPtr, void *userContext)
{
    UNUSED(ListPtr); // hush the compiler warning.

    jpegCMXoutput *copiedChunk;
    copiedChunk= (jpegCMXoutput*)userContext;

    copiedChunk->done = 0;
    copiedChunk->used = 0;
    copiedChunk->sliceNo = -1;
    copiedChunk->shaveNo = -1;
    copiedChunk->sliceSize = -1;
}
