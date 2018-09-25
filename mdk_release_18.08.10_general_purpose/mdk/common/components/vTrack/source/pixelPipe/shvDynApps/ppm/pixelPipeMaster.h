#ifndef PIXEL_PIPE_MASTER_H
#define PIXEL_PIPE_MASTER_H
#include "pixelPipeHelper.h"
#include "iFilter.h"
#define MAX_FILTERS 2

#if defined(MA2150) || defined(MA2450)
#define ShDrvCmxDmaTransaction ScCmxDmaTransaction
#endif

class GaussFilter;
class CornerFilter;

class PixelPipeMaster {
    public:
        PixelPipeMaster(pixelPipeParams_t* params, u8* buf_gauss, u8* buf_corner, u8* buf_minmax);


        void pixelPipe(frameBuffer* params, tvPyramidBuffer* pyramidBuffer, fp32* thresholds, tvFeatureCell** detectedFeatures);



    private:
        void processFirstLinesForHarris(u32 fullLineBufferSize);
        void processFirstLinesForCorner(u8 startLine, fp32* _thresholds, tvFeatureCell** _detectedFeatures);
        void processFirstCornerLinesWithoutResults(u32 line);
        void processCorners (u32 indexOfNewLineToFetch);
        void processBottomLinesForMA2150(u32 startLine);
        void clearFeatures (tvFeatureCell** features);

#ifdef PP_DEBUG
        void DEBUG_printHarrisBufferPointers();
        void DEBUG_printBufferLine(int line);
        void DEBUG_printHarrisBufferPartial();
        void DEBUG_printBufferLineIndicatedByPointer(int index);
        void DEBUG_printHarrisBufferIndicatedByPointers();
#endif

        u8* ppInit(u8* memBufPtr);
        tvFeatureCell** detectedFeatures;

        u32 width;
        u32 height;
        u32 cellGridDimension;
        u32 maxNumFeatures;
        u32 paddedStrideBytes;
        u32 strideBytes;

        u8* harrisBufferPointers[N_HARRIS_LINE_BUFFS];               // These pointers will point to harrisBuffer locations.

        u8* harrisBuffer[N_HARRIS_LINE_BUFFS] __attribute__((aligned(16)));

        u32 numInputImages; // For accessing it use setNumberOfInputImages() and numberOfInputImages().
        frameBuffer* m_inputImages;
        GaussFilter* gaussFilter;
        CornerFilter* cornerFilter;
        IFilter* filters[MAX_FILTERS];
        u32 num_filters;

        ShDrvCmxDmaTransaction* list;
};

#endif
