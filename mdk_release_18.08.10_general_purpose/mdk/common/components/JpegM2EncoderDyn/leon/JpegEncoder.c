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
#include <stdlib.h>
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

#include "theDynContext.h"

#define ASSERT_JPEG_COMP(...)   if(!(__VA_ARGS__)) { abort(); }
//#include <assert.h>
//#define ASSERT_JPEG_COMP(...)  assert(__VA_ARGS__)

extern DynamicContext_t MODULE_DATA(JpegEncoderShaveDynlib);

__attribute__((aligned(16)))
dmaTransactionList_t __attribute__((section(".cmx.cdmaDescriptors"))) tasks[SHAVE_MAX_NO * 2];

__attribute__((aligned(16)))
volatile dmaTransactionList_t __attribute__((section(".cmx.data"))) *ref;

__attribute__((aligned(16)))
dmaTransactionList_t __attribute__((section(".cmx.cdmaDescriptors"))) task1[SHAVE_MAX_NO], task2[SHAVE_MAX_NO], task3[SHAVE_MAX_NO];

__attribute__((aligned(16)))
volatile dmaTransactionList_t __attribute__((section(".cmx.cdmaDescriptors"))) *ref1[SHAVE_MAX_NO], *ref2[SHAVE_MAX_NO], *ref3[SHAVE_MAX_NO];

__attribute__((aligned(16)))
dmaTransactionList_t __attribute__((section(".cmx.cdmaDescriptors"))) *refs[SHAVE_MAX_NO * 2];

static swcShaveUnit_t __attribute__((section(".cmx.data"))) jpegshv[SHAVE_MAX_NO];
/*Caution! It is mandatory that this structure stays in CMX.
 * Speed is the most important factor.
 * Caution2! because we assume this stays in CMX, pointer arithmetics on this structure will
 * only deal with CMX memory for uncached access, I.E. mask with 0x08000000*/
volatile jpegCMXoutput __attribute__((section(".cmx.data"))) outputInfo[SHAVE_MAX_NO * 2];
volatile int __attribute__((section(".cmx.data"))) shavesStatus[SHAVE_MAX_NO];
volatile int __attribute__((section(".cmx.data"))) shaveDone;
volatile int __attribute__((section(".cmx.data"))) shaveNo;
volatile int __attribute__((section(".cmx.data"))) lastSliceStarted;
volatile int __attribute__((section(".cmx.data"))) lastSliceCopiedToDDR;
volatile int __attribute__((section(".cmx.data"))) outputCount;

frameBuffer __attribute__((section(".cmx.data"))) gimginfoPtr[SHAVE_MAX_NO];
formatSpecific __attribute__((section(".cmx.data"))) formatS;
shaveinfotype __attribute__((section(".cmx.data"))) shaveInfo;
shaveinfotype __attribute__((section(".cmx.data"))) shaveInfoArray[SHAVE_MAX_NO];

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

u32 JPEG_encodeSpecific(frameBuffer imgInfo, u8 *output, u32 shaveNo, u32 inBuffSizeShave, int jpegFormat, swcShaveUnit_t *shaves);

u32 JPEG_encode(frameBuffer imgInfo, u8 *output, u32 localShaveNo, u32 inBuffSizeShave, int jpegFormat)
{
    int i;
    for (i=0;i<SHAVE_MAX_NO;jpegshv[i]=i,i+=1);
    shaveNo=localShaveNo;
    return JPEG_encodeSpecific(imgInfo, output, shaveNo, inBuffSizeShave, jpegFormat, jpegshv);
}


// The JPEG encoding algorithm
u32 JPEG_encodeSpecific(frameBuffer imgInfo, u8 *output, u32 shaveNo, u32 inBuffSizeShave, int jpegFormat, swcShaveUnit_t *shaves)
{
    shaveDone = 0;
    lastSliceStarted = 0;
    lastSliceCopiedToDDR = -1;
    outputCount = 0;
    shaveInfo.inputBuffSizePerShave = inBuffSizeShave;

    initOutputStruct();

    swcSetupDynShaveApps(&MODULE_DATA(JpegEncoderShaveDynlib), shaves, shaveNo);

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
        //And initialize dma transfer addresses too;
        shaveInfo.task1=&task1[shv];
        shaveInfo.task2=&task2[shv];
        shaveInfo.task3=&task3[shv];
        shaveInfo.ref1=&ref1[shv];
        shaveInfo.ref2=&ref2[shv];
        shaveInfo.ref3=&ref3[shv];

        swcResetShave(shaves[shv]);
        swcSetAbsoluteDefaultStack(shaves[shv]);
        DrvLL2CFlushOpOnAllLines(LL2C_OPERATION_WRITE_BACK, 0);
        /*Copy shaveInfo into a local array location to allow changing the original*/
        shaveInfoArray[shv]=shaveInfo;
        swcAssignShaveCallback(shaves[shv], (irq_handler)&handlerForAllShaves);
        swcRunShaveAlgoOnAssignedShaveCC(&MODULE_DATA(JpegEncoderShaveDynlib), shaves[shv], "iiiiiii",
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

    (void)DrvCmxDmaInitDefault();

    dmaRequesterId dmaId = DrvCmxDmaInitRequesterOnAgent(1, 0);
    shaveDone=0;
    while (lastSliceCopiedToDDR  < (int)(lastRun-1))
    {
        shv = getDoneShave(shaveNo);
        //Check for all shaves
        if (shv != INDEX_NOT_FOUND){
            //from handler
            swcResetShave(shaves[shv]);
            //Flush the data partition
            setOutputIndexForCurrentShave(shv);

            shavesStatus[shv] = JPG_SHAVE_IDLE;

            DrvIcbDisableIrq(shaves[shv] + IRQ_SVE_0);
            DrvIcbIrqClear(shaves[shv] + IRQ_SVE_0);
            DrvIcbIrqUnsetup(shaves[shv] + IRQ_SVE_0);
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
            u32 *uncachedChunkSizeAddr;
            uncachedChunkSizeAddr=(u32*)(((u32)&outputInfo[outputIndexToCopy].sliceSize) | 0x08000000);
            u32 chunkSize = *uncachedChunkSizeAddr;
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
                swcSetAbsoluteDefaultStack(shaves[shv]);
                DrvLL2CFlushOpOnAllLines(LL2C_OPERATION_WRITE_BACK, 0);

                swcAssignShaveCallback(shaves[shv], (irq_handler)&handlerForAllShaves);
                swcRunShaveAlgoOnAssignedShaveCC(&MODULE_DATA(JpegEncoderShaveDynlib), shaves[shv], "iiiiiii",
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
    swcCleanupDynShaveApps(&MODULE_DATA(JpegEncoderShaveDynlib));
    return outputCount;
}

static void internalFormatSpecParams(formatSpecific *formatS)
{
    u32 blockSizePerShaveWithPadding = formatS->blockW * (formatS->blockH);
    formatS->blockSize = blockSizePerShaveWithPadding;
    formatS->maxBlocksPerShv = formatS->lumaBufferSize / blockSizePerShaveWithPadding;
}

formatSpecific getFormatSpecificParams(int jpegFormat)
{
    formatSpecific formatS;

    switch(jpegFormat)
    {
        case (int)JPEG_420_PLANAR:
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
            internalFormatSpecParams(&formatS);
            break;
        }
        case (int)JPEG_422_PLANAR:
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
            internalFormatSpecParams(&formatS);
            break;
        }
        case (int)JPEG_444_PLANAR:
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
            internalFormatSpecParams(&formatS);
            break;
        }
        default:
        {
            formatS.maskX = 0;
            formatS.maskY = 0;
            formatS.blockW = 0;
            formatS.blockH = 0;
            formatS.lumaBufferSize = 0;
            formatS.chromaBufferSize = 0;
            formatS.subsampling = 0;
            formatS.getBlockCoefX = 0;
            formatS.getBlockCoefY = 0;
            formatS.lumaValuesPerChroma = 0;
            ASSERT_JPEG_COMP(0 && "Incorrect JPEG format selected");
            break;
        }
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
        if (outputInfo[i].sliceNo == (lastSliceCopiedToDDR+1)){
          if (outputInfo[i].used){
            if (outputInfo[i].done){
              return i;
            }
          }
        }

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
        {
            outputInfo[i].done = 1;
            outputInfo[i].shaveNo = INDEX_NOT_FOUND;
        }
}

void __attribute__((section(".cmx.text"))) handlerForAllShaves(int irqSource)
{
    unsigned int shv = irqSource - IRQ_SVE_0;
    unsigned int i;
    // Set STOP bit in control register
    SET_REG_WORD(SVU_CTRL_ADDR[shv] + SLC_OFFSET_SVU + SVU_OCR, OCR_STOP_GO | OCR_TRACE_ENABLE);
    // Clear any interrupts from previous test
    SET_REG_WORD(SVU_CTRL_ADDR[shv] + SLC_OFFSET_SVU + SVU_IRR, 0xFFFFFFFF);
    //Identify the shave in the shave list
    i=0;
    while (i<SHAVE_MAX_NO){
        if (jpegshv[i]==shv) break;
        i++;
    }
    //Mark the corresponding field as stopped
    shavesStatus[i] = JPG_SHAVE_STOPPED;
    shaveDone=1;
}

void __attribute__((section(".cmx.text"))) dmaHandler(dmaTransactionList* ListPtr, void *userContext)
{
    UNUSED(ListPtr); // hush the compiler warning.
    //Will use this to update finished slices;
    static jpegCMXoutput *copiedChunk;
    copiedChunk= (jpegCMXoutput*)userContext;

    //Signal there is an update to be done
    copiedChunk->done = 0;
    copiedChunk->used = 0;
    copiedChunk->sliceNo = -1;
    copiedChunk->shaveNo = -1;
    copiedChunk->sliceSize = -1;
}
