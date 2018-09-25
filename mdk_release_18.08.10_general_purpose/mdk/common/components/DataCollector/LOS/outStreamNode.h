#ifndef OUTSTREAM_H__
#define OUTSTREAM_H__
#include <messages.h>

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <BehQueueOvr.h>

#include "imuFrame.h"
#include <XLink.h>
#include "commStructs.h"
#include "PlgImu.h" //TODO: we don't want dependencies. This may need to go to an IMU buffering node

#define MAX_META_GROUPS 2

typedef struct outStreamContext_t
{
    int unused;
}outStreamContext_t;

class PlgOutStream : public ThreadedPlugin{
  public:
    int camCnt;
    int lowLatencyEnable;
    int metaGroupCount;

    //TODO: in future we just need to add variable number of channels here, we don't want dependencies on specific types
//   SReceiver<ImgFramePtr> inFrame;
//   SReceiver<ImgFramePtr> inMeta;
   SReceiver<ImgFramePtr> inIMU;
   SReceiver<StereoImgMsg> inFrames;
   // variable number of inputs depending on the application
   SReceiver<StereoImgAndMetaMsg> syncIn[MAX_META_GROUPS];
   SemSync            sync;
   //each input has different type. When linking the inputs, the user shall tell what type each input has, so this plugin can tag the data for the PC.
   MsgGid inputGid[MAX_META_GROUPS];

   void Create(void);
   void *threadFunc(void *);

   // Create a new metadata input. The function will return the index in the input array which represents that input. This number can be used then to link the specific plugin to this input
   int AddMetaInput(MsgGid grp);

  private:
   void  outStreamSendOverUsb(void *data, outStreamType_t type, MsgGid Id, int subId);
   void outStreamUSBFullWrite(void* buff, u32 cnt);

   imuFrame imuPacket;
   streamId_t usbLinkStream;
//   OvrOldCmd ovrBeh;

};



#endif
