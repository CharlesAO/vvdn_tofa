#ifndef CORNER_SW_H
#define CORNER_SW_H
#include "fifoCommApi.h"
#include "pixelPipeHelper.h"
#include "swcFrameTypes.h"
#include "iExtremaLocator.h"
#include "cornerSWFuncs.h"

extern const u32 pp_cornersX_cornerFilter;

inline void startCornerTaskOnMasterShave(fifoCommTask_t* unused, void* params) {
    (void)unused;
    cornerFilter(params);
}
inline void* waitCornerTaskOnMasterShave(fifoCommTask_t* unused) {
    (void)unused;
    return NULL;
}

class MvCvFilter : public IExtremaLocator {
    public:
        MvCvFilter(fifoCommTask_t* _cornerTask, frameSpec spec, uint32_t num_input_images, u32 _maxNumFeatures, u32 _cellGridDimension, bool insertAndSort);
        virtual ~MvCvFilter(){}
        virtual void initFrame(tvFeatureCell** _detectedFeatures, fp32* thresholds);
        virtual void callFilter(u32 lineNo, HARRIS_OUTPUT_TYPE* (&lines)[3]);
        virtual void extractResultsFromPreviousCall() {}
        virtual void waitFilter();
        virtual void endFrame();

    private:
        void (*startFn)(fifoCommTask_t*, void*);
        void* (*waitFn)(fifoCommTask_t*);
        fifoCommTask_t* cornerTask;
        cornerParameters_t cornerParameters[PP_MAX_INPUT_IMAGES];

        uint32_t num_inputImages;
        uint32_t paddedWidth;
        uint32_t height;
        HARRIS_OUTPUT_TYPE* lastLinesProcessed[2];
        u32 cellGridDimension;
        u32 nCells;
};

#endif

