#ifndef IEXTREMA_LOCATOR_H
#define IEXTREMA_LOCATOR_H
#include "pixelPipeHelper.h"
#include "swcFrameTypes.h"
#include "iFilter.h"

class IExtremaLocator {
public:
    virtual void initFrame(tvFeatureCell** _detectedFeatures, fp32* thresholds)=0;
    virtual void callFilter(u32 lineNo, HARRIS_OUTPUT_TYPE* (&lines)[3])=0; // TODO: Make the 3 related to kernel size. template maybe
    virtual void extractResultsFromPreviousCall()=0;
    virtual void waitFilter()=0;
    virtual void endFrame()=0;
    virtual ~IExtremaLocator(){}
};
#endif

