#ifndef GAUSS_FILTER_H
#define GAUSS_FILTER_H
#include "fifoCommApi.h"
#include "pixelPipeHelper.h"
#include "iFilter.h"

class GaussFilter : public IFilter{
   public:
        GaussFilter(fifoCommTask_t* _gaussTask, frameSpec spec, uint32_t num_input_images, u8** pyrInputLine, u8** buf, void* _helperDmaTrans);
        void startFrame(tvPyramidBuffer* _pyramidBuffer);
        virtual void startTask(u32 line);
        virtual void postprocessLastResults() {}
        virtual void waitTask();
        virtual void endFrame();
private:
        void (*startFn)(fifoCommTask_t*, void*);
        void* (*waitFn)(fifoCommTask_t*);
        gaussParameters_t gaussParameters[PP_MAX_INPUT_IMAGES];
        fifoCommTask_t* gaussTask;
        tvPyramidBuffer* pyramidBuffer;
        uint32_t num_inputImages;

        u8* gaussBufferPointers[PP_MAX_INPUT_IMAGES][(MAX_PYRAMID_LEVELS - 1) * GAUSS_V_SPAN]; // no need for the last level since that's the image itself

        u8* gaussBuffers[4][PP_MAX_INPUT_IMAGES][GAUSS_V_SPAN] __attribute__((aligned(16)));
        u8** pyramidInputLineBaseAddrPtr;
        u32 lastLineProcessedGauss;
        void* helperDmaTrans;
};


#endif
