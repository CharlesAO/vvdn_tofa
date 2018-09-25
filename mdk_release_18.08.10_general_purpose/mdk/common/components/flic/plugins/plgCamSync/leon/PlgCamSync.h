#ifndef CAMERA_SYNC_H__
#define CAMERA_SYNC_H__



#include <Flic.h>
#include <Receiver.h>
#include <Sender.h>

#include <ImgFrame.h>
#include <messages.h>


class PlgCamSync : public ThreadedPlugin
{
  public:
    SReceiver<ImgFramePtr> inO[2];  //frame from frame-pool
    MSender  <ImgFramePtr> out[2];  //send frame to consumer, composite frame for isp frame+isp
    MSender<StereoImgMsg> outStereo;

    void    *threadFunc(void *);
    void     Create();
};

#endif
