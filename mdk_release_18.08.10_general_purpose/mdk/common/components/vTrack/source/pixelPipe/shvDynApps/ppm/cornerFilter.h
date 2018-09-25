#ifndef CORNER_FILTER_H
#define CORNER_FILTER_H
#include "pixelPipeHelper.h"
#include "swcFrameTypes.h"
#include "iExtremaLocator.h"
#include "iFilter.h"

// Class that provides the keypoints for an input frame
// Usage: startFrame  - Does stuff reqd before the lines are processed & process the first few lines
//        startTask - process one line
//        waitTask - waits for line to be processed
//        endFrame - processes the last few lines & does post frame process steps

class CornerFilter : public IFilter {
   public:
        CornerFilter(frameSpec spec, uint32_t num_input_images, u8* harrisBaseAddr, u8** buf,
                     u8* minMaxObjBuf, cornerConfig_t algType, fifoCommTask_t* _cornerTask, u32 cellGridDimension, u32 maxNumFeatures);
        void startFrame(tvFeatureCell** _detectedFeatures);
        virtual void startTask(u32 line);
        virtual void postprocessLastResults();
        virtual void waitTask();
        void endFrame();

        //stuff that ideally wouldn't be in the public api
        void initExtLoc(fp32* _thresholds, tvFeatureCell** _detectedFeatures);
        void callHarrisFilter();
        void waitHarrisFilter();
private:
        void clearFeatures (tvFeatureCell** features);

        void callHarrisAndCornerFilter(u32 lineNo);

        void waitHarrisAndCornerFilter();
        u32 substractCornerLineOffset(u32 processedImageLineNo);


        void initSippFilter(u32 height, u8 useShiTomasi, u8 useSobel, u8 scaleExponent);


        HARRIS_OUTPUT_TYPE* cornerScoresPointers[N_CORNER_LINE_BUFFS]; // These pointers will point to cornerScoresBuffer locations.
        HARRIS_OUTPUT_TYPE*  cornerScoresBuffer[N_CORNER_LINE_BUFFS] __attribute__((aligned(16)));

        int harrisInputBaseAdr;
        int harrisOutputBaseAdr;
        int harrisInterruptStatusAdr;
        int harrisInterruptClearAdr;
        int harrisInterruptMask;

        uint32_t num_inputImages;
        u8* harrisInputLineBaseAddr;
        int lastLineProcessedCorner;
        
        u32 paddedWidth;
        u32 maxNumFeatures;
        u32 nCells;
        u32 maxFeatPerCell;
        uint32_t subtrahend;
        bool waitHarrisAndCornerFilterCalled;
        bool postprocessedFirstResults;

        IExtremaLocator* extLoc;

};


#endif
