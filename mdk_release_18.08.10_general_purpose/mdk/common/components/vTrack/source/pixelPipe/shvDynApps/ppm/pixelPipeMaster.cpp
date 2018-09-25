///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     pixelPipeline for vtrack project
///

#include <string.h>
#include <stdio.h>
#include <new>
#include <stdlib.h>
#include <assert.h>

#include "mv_types.h"
#include "mvMacros.h"
#include "svuCommonShave.h"
#include "swcFrameTypes.h"
#include "pixelPipeHelper.h"
#include "pixelPipeGaussShaveHelper.h"

#include "vTrack.h"
#include "DrvSvuDefines.h"
#include "gaussHx2.h"
#include "gaussVx2.h"
#include "thresholdFilter.h"
#include "nonMax3x3_fp32.h"

#include "gaussFilter.h"
#include "cornerFilter.h"
#include "cornerImpl.h" //TODO: get rid of this

//#define PP_DEBUG

#if defined(MA2150) || defined(MA2450)
#include "scCmxDma.h"
#include "scCmxDmaDefines.h"
#define ShDrvCmxDmaInitialize ScCmxDmaInitialize
#define ShDrvCmxDmaTransactionHnd ScCmxDmaTransactionHnd
#define ShDrvCmxDmaCreateTransaction ScCmxDmaCreateTransaction
#define ShDrvCmxDmaStartTransfer ScCmxDmaStartTransfer
#define ShDrvCmxDmaWaitTransaction ScCmxDmaWaitTransaction
#else
#include "ShDrvCmxDma.h"
#endif
#include "pixelPipeMaster.h"
#if defined(MA2150) || defined(MA2450)
const int SIZEOF_BUF = sizeof(MvCvFilter) + sizeof(pixelPipeParams_t) + sizeof(PixelPipeMaster) + sizeof(GaussFilter) + sizeof(CornerFilter) + (64*6);
#else
const int SIZEOF_BUF = sizeof(MinMaxHW) + sizeof(pixelPipeParams_t) + sizeof(PixelPipeMaster) + sizeof(GaussFilter) + sizeof(CornerFilter) + (64*6);
#endif

PixelPipeMaster* ppm;


extern "C"
void pixelPipeMasterEntry(u8* buf_start, u32 buf_size) {
    u8* buf = ALIGN_UP(buf_start, 64);
    pixelPipeParams_t* config = (pixelPipeParams_t*) buf;
    buf += ALIGN_UP(sizeof(*config), 64);

    if(config->isInitialised){
        ppm = (PixelPipeMaster*) buf;
    } else {
        u8* buf_ppm = buf;
        buf += ALIGN_UP(sizeof(PixelPipeMaster), 64);
        u8* buf_gauss = buf;
        buf += ALIGN_UP(sizeof(GaussFilter), 64);
        u8* buf_corner = buf;
        buf += ALIGN_UP(sizeof(CornerFilter), 64);
        u8* buf_minmax = buf;
#if defined(MA2150) || defined(MA2450)
        buf += ALIGN_UP(sizeof(MvCvFilter), 64);
#else
        buf += ALIGN_UP(sizeof(MinMaxHW), 64);
#endif
        ppm = new (buf_ppm) PixelPipeMaster(config, buf_gauss, buf_corner, buf_minmax);
        assert(buf-buf_start <= SIZEOF_BUF);
        assert(buf_size >= SIZEOF_BUF);
//        config->isInitialised = true;
    }
    ShDrvCmxDmaInitialize(NULL);
    frameBuffer*          inputImage = config->in_img;
    ppm->pixelPipe(inputImage, config->pyramidBuffer, config->thresholds, config->feature_cells);
    SHAVE_HALT;
}

void* swcSolveShaveRelAddr(void* inAddr)
{
    u32 window=0;
    u32 windowBase;
    u32 * windowRegPtr = (u32 *)(SHAVE_0_BASE_ADR + (SVU_SLICE_OFFSET * scGetShaveNumber()) + SLC_TOP_OFFSET_WIN_A);

    u32 resolved;
    u32 inAddrs= (u32) inAddr;
    switch (inAddrs >> 24)
    {
        case 0x1C: window=0; break;
        case 0x1D: window=1; break;
        case 0x1E: window=2; break;
        case 0x1F: window=3; break;
        default  : return(inAddr);  break;  //absolute address, no translation is to be done
    }
    windowBase = windowRegPtr[window];
    resolved = ((inAddrs & 0x00FFFFFF) + windowBase);

    return (void*)resolved;
}

#ifdef PP_DEBUG
void
PixelPipeMaster::DEBUG_printHarrisBufferPointers()
{
    u32 i;

    printf("------\n");
    for (i=0; i<N_HARRIS_LINE_BUFFS; i++)
    {
        printf("%x\n", (u32)harrisBufferPointers[i]);
    }
}

void
PixelPipeMaster::DEBUG_printBufferLine(int line)
{
    printf("Line %x: %x %x %x %x %x %x %x %x    \n",
            line,
            harrisBuffer[line][PADDING_H],
            harrisBuffer[line][PADDING_H+1],
            harrisBuffer[line][PADDING_H+2],
            harrisBuffer[line][PADDING_H+3],
            harrisBuffer[line][PADDING_H+4],
            harrisBuffer[line][PADDING_H+5],
            harrisBuffer[line][PADDING_H+6],
            harrisBuffer[line][PADDING_H+7]
    );
}

void
PixelPipeMaster::DEBUG_printHarrisBufferPartial()
{
    printf("--- Harris Buffer\n");
    for(u32 i=0; i<= GAUSS_V_SPAN; i++)
        DEBUG_printBufferLine(i);
}

void
PixelPipeMaster::DEBUG_printBufferLineIndicatedByPointer(int index)
{
    printf("%x HARRIS[pointer[%d]] %x %x %x %x %x %x\n",
            (u32)harrisBufferPointers[index],
           index,
           harrisBufferPointers[index][PADDING_H],
           harrisBufferPointers[index][PADDING_H + 1],
           harrisBufferPointers[index][PADDING_H + 2],
           harrisBufferPointers[index][PADDING_H + 3],
           harrisBufferPointers[index][PADDING_H + 4],
           harrisBufferPointers[index][PADDING_H + 5]
           );
}

void
PixelPipeMaster::DEBUG_printHarrisBufferIndicatedByPointers()
{
    printf("--- Harris Buffer indicated by pointers\n");
    for(u32 i=0; i<N_HARRIS_LINE_BUFFS; i++)
        DEBUG_printBufferLineIndicatedByPointer(i);
}
#endif //PP_DEBUG

// Processes the first image lines required for starting Harris filter.
// Lines required: (KerneSize - 1) / 2.
// The MA2100 Harris filter doesn't handle automatic padding, therefore
// the padding is done by(u32*)swcSolveShaveRelAddrirst image line in the padding area.
    void
PixelPipeMaster::processFirstLinesForHarris(u32 fullLineBufferSize)
{
    ShDrvCmxDmaTransactionHnd hnd[PP_MAX_INPUT_IMAGES];

    u8 line;

    // Get image line 0

    for (u32 i = 0; i < numInputImages; i++)
    {
        u8* next_line_ptr = &harrisBufferPointers[HARRIS_PADDING][PADDING_H] + i * (2 * PADDING_H + width);
        frameBuffer* inImg = &m_inputImages[i];
        ShDrvCmxDmaCreateTransaction(&hnd[i], &list[i],
                (u8*)(inImg->p1 + (0) * strideBytes),
                (u8*) next_line_ptr,
                strideBytes);
        ShDrvCmxDmaStartTransfer(&hnd[i]);
        ShDrvCmxDmaWaitTransaction(&hnd[i]);
        padLine(next_line_ptr, width);
    }
    for (line = 1; line <=(HARRIS_KERNEL_SIZE-1)/2; line++)
    {
        u8* lines[PP_MAX_INPUT_IMAGES];
        for (u32 i = 0; i < numInputImages; i++)
        {
            lines[i] = &harrisBufferPointers[HARRIS_PADDING + 1][PADDING_H] + i * (2 * PADDING_H + width);
            frameBuffer* inImg = &m_inputImages[i];
            ShDrvCmxDmaCreateTransaction(&hnd[i], &list[i],
                    (u8*) (inImg->p1 + (line) * strideBytes),
                    (u8*) lines[i],
                    strideBytes);
            ShDrvCmxDmaStartTransfer(&hnd[i]);
        }
        if(gaussFilter) {
            gaussFilter->startTask(line - 1);
        }
        if(cornerFilter) {
            cornerFilter->callHarrisFilter();
            // no need to wait here, the filter is getting the first kernel_size/2 lines before producing line
        }
        (void)fullLineBufferSize;
        for (u32 i = 0; i < numInputImages; i++)
        {
            ShDrvCmxDmaWaitTransaction(&hnd[i]);
            padLine(lines[i], width);
        }
        if(gaussFilter) {
            gaussFilter->waitTask();
        }
        ROTATE_BUFFER(harrisBufferPointers, N_HARRIS_LINE_BUFFS);
    }
}

// Processes the first image lines required for starting the Corner detection.
// Corner detection requires 3 Harris lines.
// Should be called only after the initial lines are available for Harris filter
// (use processFirstLinesForHarris() for getting them)
void
PixelPipeMaster::processFirstLinesForCorner(u8 startLine, fp32* _thresholds, tvFeatureCell** _detectedFeatures)
{
    ShDrvCmxDmaTransactionHnd hnd[PP_MAX_INPUT_IMAGES];
    u8 line = startLine;
    //harris
    //FIXME: copied and pasted to unroll. make bettetr
    if(cornerFilter) {
        cornerFilter->callHarrisFilter();
        cornerFilter->waitHarrisFilter();
    }
    u8* lines[PP_MAX_INPUT_IMAGES];
    //dma
    for (u32 i = 0; i < numInputImages; i++)
    {
        lines[i] = &harrisBufferPointers[HARRIS_PADDING + 1][PADDING_H] + i * (2 * PADDING_H + width);
        frameBuffer* inImg = &m_inputImages[i];
        ShDrvCmxDmaCreateTransaction(&hnd[i], &list[i],
                (u8*) (inImg->p1 + (line+1) * strideBytes),
                (u8*) lines[i],
                strideBytes);
        ShDrvCmxDmaStartTransfer(&hnd[i]);
    }
    //gauss
    if(gaussFilter) {
        gaussFilter->startTask(line);
    }
    //dma wait
    for (u32 i = 0; i < numInputImages; i++)
    {
        ShDrvCmxDmaWaitTransaction(&hnd[i]);
        padLine(lines[i], width);
    }

    ROTATE_BUFFER(harrisBufferPointers, N_HARRIS_LINE_BUFFS);
    //gauss wait
    if(gaussFilter) {
        gaussFilter->waitTask();
    }

    // at this stage we have 1 harris output line.
    // is a good time to call initFrame for the ext locator (can do it's pre-frame init stuff)
    if(cornerFilter){
        cornerFilter->initExtLoc(_thresholds, _detectedFeatures);
    }
    line++;
    // line = startline + 1
    for (; line < startLine + 2; line++)
    {
        if(cornerFilter) {
            cornerFilter->callHarrisFilter();
            cornerFilter->waitHarrisFilter();
        }
        u8* lines[PP_MAX_INPUT_IMAGES];
        for (u32 i = 0; i < numInputImages; i++)
        {
            lines[i] = &harrisBufferPointers[HARRIS_PADDING + 1][PADDING_H] + i * (2 * PADDING_H + width);
            frameBuffer* inImg = &m_inputImages[i];
            ShDrvCmxDmaCreateTransaction(&hnd[i], &list[i],
                    (u8*) (inImg->p1 + (line+1) * strideBytes),
                    (u8*) lines[i],
                    strideBytes);
            ShDrvCmxDmaStartTransfer(&hnd[i]);
        }
        if(gaussFilter) {
            gaussFilter->startTask(line);
        }
        for (u32 i = 0; i < numInputImages; i++)
        {
            ShDrvCmxDmaWaitTransaction(&hnd[i]);
            padLine(lines[i], width);
        }
        ROTATE_BUFFER(harrisBufferPointers, N_HARRIS_LINE_BUFFS);
        if(gaussFilter) {
            gaussFilter->waitTask();
        }
    }
}

void
PixelPipeMaster::processFirstCornerLinesWithoutResults(u32 line) {
    ShDrvCmxDmaTransactionHnd hnd[PP_MAX_INPUT_IMAGES];

    u8* lines[PP_MAX_INPUT_IMAGES];
    for (u32 i = 0; i < numInputImages; i++)
    {
        lines[i] = &harrisBufferPointers[HARRIS_PADDING + 1][PADDING_H] + i * (2 * PADDING_H + width);
        frameBuffer* inImg = &m_inputImages[i];
        ShDrvCmxDmaCreateTransaction(&hnd[i], &list[i],
                (u8*) (inImg->p1 + ((line + 1) * strideBytes)),
                (u8*) lines[i],
                strideBytes);
        ShDrvCmxDmaStartTransfer(&hnd[i]);
    }

    for(u32 i = 0; i<num_filters; i++) {
        filters[i]->startTask(line);

    }

    for (u32 i = 0; i < numInputImages; i++)
    {
        ShDrvCmxDmaWaitTransaction(&hnd[i]);
        padLine(lines[i], width);
    }
    ROTATE_BUFFER(harrisBufferPointers, N_HARRIS_LINE_BUFFS);

    for(u32 i = 0; i<num_filters; i++) {
        filters[i]->waitTask();
    }

}

// Processes corners: calls Harris filter, generates pyramid lines, fetches the next line and
// filters the corners found by Harris filter.
// The selected corners are saved in "detectedFeatures".
// Should be called only after the initial lines are available
// (use processFirstLinesForCorner() for getting them).
void
PixelPipeMaster::processCorners (u32 line)
{

    u8* lines[PP_MAX_INPUT_IMAGES];
    ShDrvCmxDmaTransactionHnd hnd[PP_MAX_INPUT_IMAGES];
    for (u32 i = 0; i < numInputImages; i++)
    {
        lines[i] = &harrisBufferPointers[HARRIS_PADDING + 1][PADDING_H] + i * (2 * PADDING_H + width);
        frameBuffer* inImg = &m_inputImages[i];
        ShDrvCmxDmaCreateTransaction(&hnd[i], &list[i],
                (u8*) (inImg->p1 + ((line + 1) * strideBytes)),
                (u8*) lines[i],
                strideBytes);
        ShDrvCmxDmaStartTransfer(&hnd[i]);
    }


    for(u32 i = 0; i<num_filters; i++) {
        filters[i]->startTask(line);
    }

    for (u32 i = 0; i < numInputImages; i++)
    {
        ShDrvCmxDmaWaitTransaction(&hnd[i]);
        padLine(lines[i], width);
    }
    ROTATE_BUFFER(harrisBufferPointers, N_HARRIS_LINE_BUFFS);

    //currently only the HW version of the corner filter processes the previous results here
    //in this case, it is important to ensure that the corner filter runs before the gauss filter
    //(so that the wait time is used for processing)
    for(u32 i = 0; i<num_filters; i++) {
        filters[i]->postprocessLastResults();
        filters[i]->waitTask();
    }
}

void
PixelPipeMaster::processBottomLinesForMA2150(u32 startLine)
{
    u32 line;
    line = startLine;
    if(cornerFilter) {
        cornerFilter->endFrame();
    }
    if(gaussFilter) {
        gaussFilter->endFrame();
    }
}

//TODO:clean this.
u8*
PixelPipeMaster::ppInit(u8* memBufPtr)
{
    u8* curAddr;
    u8 i;
    curAddr = (u8*)memBufPtr;

    for(i = 0; i < N_HARRIS_LINE_BUFFS; i++)
    {
        harrisBuffer[i] = curAddr;
        harrisBufferPointers[i] = harrisBuffer[i];
        curAddr += (width + 2 * PADDING_H) * PP_MAX_INPUT_IMAGES ;
    }
    return curAddr;
}

PixelPipeMaster::PixelPipeMaster(pixelPipeParams_t* inputParams, u8* buf_gauss,
                                 u8* buf_corner,u8* buf_minmax) :
    cellGridDimension(inputParams->cellGridDimension),
    maxNumFeatures(inputParams->maxNumFeatures),
    numInputImages(inputParams->num_pyrs) {
    assert(PP_MAX_INPUT_IMAGES >= numInputImages);

    frameBuffer*          inputImage = inputParams->in_img;
    width               = inputImage[0].spec.width;
    height              = inputImage[0].spec.height;
    uint32_t bytesPP    = inputImage[0].spec.bytesPP;
    paddedStrideBytes   = width + 2 * PADDING_H * bytesPP;
    strideBytes         = width * bytesPP;
    u8* curAddr = ppInit(inputParams->svuBuff);
    u32 svu_buf_size = inputParams->svuBuffSize;
    num_filters = 0;
    u8* cmx_buf_pos = (u8*) ALIGN_UP(inputParams->dmaBuff, 64);
    u32 cmx_buf_size = inputParams->dmaBuffSize;
    void* helperDmaTrans;
    list = (ShDrvCmxDmaTransaction*)cmx_buf_pos;
    cmx_buf_pos += (sizeof(ShDrvCmxDmaTransaction) * PP_MAX_INPUT_IMAGES);
    helperDmaTrans = cmx_buf_pos;
    cmx_buf_pos += sizeof(ShDrvCmxDmaTransaction);

    //Below we instanciate the Filters. Note the order of the filters is important for performance to allow optimal usage of resources.
    //In the current version of the implementation, corner filter should be filter[0] as it is the only one with an implementation for
    //postprocessLastResults. That ensures that it can run while waiting for the filters to execute the current line
    if(inputParams->corCfg.alg != CORNER_OFF) {
        cornerFilter = new (buf_corner) CornerFilter(inputImage[0].spec, numInputImages,
                                                     &harrisBufferPointers[0][0], &curAddr,
                                                     buf_minmax, inputParams->corCfg,
                                                     inputParams->cornerTask,
                                                     cellGridDimension, maxNumFeatures);
        filters[num_filters] = cornerFilter;
        num_filters++;
    }
    if(inputParams->pyrAlg != PYRAMID_OFF) {
        gaussFilter = new (buf_gauss) GaussFilter(inputParams->gaussTask, inputImage[0].spec,
                                                  numInputImages, harrisBufferPointers,
                                                  &curAddr, helperDmaTrans);
        filters[num_filters] = gaussFilter;
        num_filters++;
    }
    ALIGN_UP(curAddr, 64);
    //check that curAddr is still ok
    assert((int32_t)curAddr - (int32_t)inputParams->svuBuff <= (int32_t)svu_buf_size);

    //check the cmx buf(for dma)
    assert((int32_t)cmx_buf_pos - (int32_t)inputParams->dmaBuff <= (int32_t)cmx_buf_size);
}
void
PixelPipeMaster::pixelPipe(frameBuffer* inputImage, tvPyramidBuffer* pyramidBuffer,
                           fp32* thresholds, tvFeatureCell** detectedFeatures)
{
    m_inputImages = inputImage;
    u32 currentLine = 0; // zero-based index

    for(u32 i = 0; i < N_HARRIS_LINE_BUFFS; i++)
    {
        harrisBufferPointers[i] = harrisBuffer[i];
    }
    if(gaussFilter) {
        gaussFilter->startFrame(pyramidBuffer);
    }
    if(cornerFilter) {
        cornerFilter->startFrame(detectedFeatures);
    }
    processFirstLinesForHarris(numInputImages * paddedStrideBytes);
    currentLine = (HARRIS_KERNEL_SIZE-1)/2;
    processFirstLinesForCorner(currentLine, thresholds, detectedFeatures);
    currentLine = (HARRIS_KERNEL_SIZE-1)/2+2;
    processFirstCornerLinesWithoutResults(currentLine);
    currentLine++; //(HARRIS_KERNEL_SIZE-1)/2 + 3
    // this will dma in a line in advance so will dma in more than height. As we are not using it, it is ok, we may remove it in the future
    for (; currentLine < height - 1  ; currentLine++)
        processCorners(currentLine);

    // process last line without rotating and dma
    for(u32 i = 0; i<num_filters; i++) {
        filters[i]->startTask(currentLine);
    }
    for(u32 i = 0; i<num_filters; i++) {

        filters[i]->postprocessLastResults();
        filters[i]->waitTask();
    }

    processBottomLinesForMA2150(currentLine + 1);
}
