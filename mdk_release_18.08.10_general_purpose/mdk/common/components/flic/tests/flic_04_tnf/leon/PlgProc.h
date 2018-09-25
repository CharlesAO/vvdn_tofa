///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Mockup TNF plugin
///

#ifndef __PLG_PROC_H__
#define __PLG_PROC_H__

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <ImgFrame.h>

class PlgProc : public ThreadedPlugin
{
  public:
    SReceiver<ImgFramePtr> inI; //in [Input frame ]-read only
    MReceiver<ImgFramePtr> inO; //in [Output frame]-write only
    MSender  <ImgFramePtr> out; //out msg to consumers

    virtual void *threadFunc(void *);
    virtual void Create();
};

#endif