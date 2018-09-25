#ifndef PLGFLICVISON_H__
#define PLGFLICVISON_H__


#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <ImgFrame.h>
#include <BehQueueOvr.h>
#include <rotationMatMsg.h>
#include "messages.h"
#include "ConfigMessage.h"

//                                  VISION BASE Plugin
//
//                    +-------------------------------------------+
//                    |         Vision Base Plugin                |
//                    |                                           |
//   Left Image +---->|            +---------------+              |
//                    | frameTo -->|- - - - - - - -|- - - - - >   |+--> Stereo Images
//                    | Process[]  |    Derived    |--> metaOut[] |     + metadata
//  Right Image +---->|            |    Plugin     |              |
//                    | rotMatIn-->|               |              |+--> meta0
//  Rot Mat In  +---->|            +---------------+              |+--> meta1
//                    +-------------------------------------------+
//                               ^
//                               |
//                               +
//                         Empty metadata


typedef struct customProcCtx
{
    u32 camCnt;

    // will produce only one output frame for every frameRateDivider input frames
    u32 frameRateDivider;
    // will produce frame only if frameCount % frameRateDivider == dropCondition
    u32 dropCondition;
}customProcCtx;



class visionBasePlg : public ThreadedPlugin{
  public:
    MReceiver<ImgFramePtr> emptyMeta;
    SReceiver<ConfigMessage> cfg; // TODO make cfg generic
    SReceiver<ImgFramePtr> frameLeft;
    SReceiver<ImgFramePtr> frameRight;
    SReceiver<RotMatMsg> rotMatIn;
    MSender<StereoImgAndMetaMsg> synchronizedOut;
    MSender<ImgFramePtr> meta0;
    MSender<ImgFramePtr> meta1;

    customProcCtx ctx;
    bool gyroAssist;
    void Create();
    virtual ~visionBasePlg() {}
    virtual void releaseAllFrms()
    {
        for(int i=0; i<VISION_BASE_MAX_NUM_CAMS; i++) {
            frames[i]=nullptr;
            meta[i]=nullptr;
            synchronizedMsg.meta[i]=nullptr;
            synchronizedMsg.frame[i]=nullptr;
        }
    }

private:
    void* threadFunc(void *context);
    int ReceiveInputs();
    
    // Implementation of this can be blank if nothing is required
    // by inheriting plugin
    virtual void* pluginSpecificInit(void)=0;
    
    // Implementation of pluginSpecificProcess must:
    // Use the input available @ frameToProcess and rotationVector
    // Populate output @ metaOut
    virtual void* pluginSpecificProcess()=0;
    
    // output of the vision plugin
    StereoImgAndMetaMsg synchronizedMsg;
    ImgFramePtr meta[VISION_BASE_MAX_NUM_CAMS];
    // input frames
    ImgFramePtr frames[VISION_BASE_MAX_NUM_CAMS];
protected:
    // inputs to the vision plugin (accessed using the below)
    ImgFramePtr* frameToProcess[VISION_BASE_MAX_NUM_CAMS];
    float rotationVector[3];
    // outputs from vision plugin
    ImgFramePtr* metaOut[VISION_BASE_MAX_NUM_CAMS];

    bool messageAvailable = false;
    ConfigMessage messageReceived;
};



#endif
